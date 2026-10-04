"""Compare source dimensions, exported meshes and embedded viewer geometry."""
import ast
import argparse
import base64
import json
from pathlib import Path
import re
import subprocess
import tempfile

import numpy as np
import trimesh

ROOT = Path(__file__).parent
VIEWER = ROOT / 'viewer' / 'fragment.html'
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--openscad', required=True, help='OpenSCAD executable')
OPENSCAD = parser.parse_args().openscad
source = (ROOT / 'cyd-case.scad').read_text()
env = {}
constants = re.sub(r'//[^\n]*', '', source.split('module x_bore')[0])
for name, expr in re.findall(r'\b(\w+)\s*=\s*([^;]+);', constants):
    try:
        tree = ast.parse(expr, mode='eval')
        allowed = (ast.Expression, ast.Constant, ast.Name, ast.Load, ast.List,
                   ast.BinOp, ast.Add, ast.Sub, ast.Mult, ast.Div,
                   ast.UnaryOp, ast.USub, ast.Subscript)
        if all(isinstance(node, allowed) for node in ast.walk(tree)):
            env[name] = eval(compile(tree, '<cad constants>', 'eval'), {'__builtins__': {}}, env)
    except (SyntaxError, NameError):
        pass

html = VIEWER.read_text(encoding='utf-8')
embedded = json.loads(re.search(r'id="cyd-case-meshes">(.*?)</script>', html, re.S)[1])
meshes = {}
parts = {}
for name, data in embedded.items():
    mesh = trimesh.load_mesh(ROOT / 'draft-stl' / (name + '.stl'))
    packed = np.frombuffer(base64.b64decode(data['v']), dtype='<i2').reshape(-1, 3) / 100
    faces = np.frombuffer(base64.b64decode(data['f']), dtype='<u2').reshape(-1, 3)
    assert np.array_equal(faces, mesh.faces), name
    error = float(np.max(np.abs(packed - mesh.vertices)))
    assert error <= 0.00501, (name, error)
    assert mesh.is_watertight and mesh.is_winding_consistent
    meshes[name] = mesh
    parts[name] = {'local_bounds_mm': mesh.bounds.round(5).tolist(),
                   'full_part_size_mm': mesh.extents.round(5).tolist(),
                   'embedded_max_rounding_error_mm': round(error, 6)}

placements = {}
for name, coords in re.findall(r"add\('([^']+)',geometry\('[^']+'\),\[([^\]]+)\]", html):
    if name != 'button':
        placements[name] = np.array(ast.literal_eval('[' + coords + ']'))
expected = {
    'bezel': [0, 0, 0], 'shell': [0, 0, 0],
    'rail_cover': [env['rail_x']+2, env['rail_y']+6.5, env['depth']-env['rail_h']+env['rail_slop']],
    'stylus_cap': [env['tube_x']+env['tube_len']+3, env['tube_y'], env['tube_z']],
    'pod': [env['pod_x'], env['pod_y'], env['pod_z']],
    'pod_lid': [env['pod_x'], env['pod_y'], env['pod_z']+env['pod_size'][2]],
}
for name, coords in expected.items():
    assert np.allclose(placements[name], coords), (name, placements[name], coords)

def assembly_bounds(mode):
    active = ['bezel', 'shell', 'stylus_cap'] + (['pod', 'pod_lid'] if mode == 'battery' else ['rail_cover'] if mode == 'cover' else [])
    vertices = []
    for name in active:
        xyz = meshes[name].vertices.copy()
        if name == 'stylus_cap':
            xyz = xyz @ np.array([[0, 0, 1], [0, 1, 0], [-1, 0, 0]])
        vertices.append(xyz + placements[name])
    xyz = np.concatenate(vertices)
    return np.array([xyz.min(axis=0), xyz.max(axis=0)])

assemblies = {}
with tempfile.TemporaryDirectory(prefix='cyd-dimension-audit-') as directory:
    for mode in ['battery', 'cover', 'empty']:
        bounds = assembly_bounds(mode)
        if mode != 'empty':
            path = Path(directory) / (mode + '.stl')
            cmd = [OPENSCAD, '-o', str(path), '-D', 'part="assembly"', '-D',
                   'with_pod=' + ('true' if mode == 'battery' else 'false'), str(ROOT / 'cyd-case.scad')]
            result = subprocess.run(cmd, capture_output=True, text=True)
            assert result.returncode == 0 and 'ERROR:' not in result.stderr, result.stderr
            independent = trimesh.load_mesh(path).bounds
            assert np.allclose(independent, bounds, atol=0.001), (mode, independent, bounds)
            print(f'{mode}: independent assembly bounds match', flush=True)
        assemblies[mode] = {'bounds_mm': bounds.round(5).tolist(),
                            'size_mm': (bounds[1]-bounds[0]).round(5).tolist()}

pod_body = env['pod_size'][2]
shoe_depth = env['rail_h']-env['rail_slop']
gap = env['pod_z']-env['depth']
overlap = shoe_depth-gap
guard_depth = -parts['pod']['local_bounds_mm'][0][2]
guard_overlap = guard_depth-gap
assert abs(env['depth']+parts['pod']['full_part_size_mm'][2]-guard_overlap+2.4-47.6) < 0.001
compression = np.array(env['contact_tip_x'])-env['contact_face_x']
assert np.all(compression > 0) and np.all(compression < env['contact_stroke'])
assert abs(compression[0]-compression[1]-1) < 0.00001
assert env['contact_y'][1]-env['contact_y'][0] == 4
assert abs(env['contact_face_x']-env['contact_guard_x']-2.0) < 0.00001
assert abs(assemblies['battery']['size_mm'][2]-(env['depth']+gap+pod_body+2.4)) < 0.001
assert np.allclose(np.diff(np.array(env['mounts']), axis=0)[0], [94.5, 0])
assert abs(env['mounts'][2][1]-env['mounts'][0][1]-47.9) < 0.0001
assert abs(env['rail_x']+env['rail_len']-env['W']) < 0.0001
assert abs(parts['rail_cover']['full_part_size_mm'][2] + expected['rail_cover'][2] - env['depth']) < 0.0001
hardware = {}
for name, size, position in re.findall(r"box\('([^']+)',\[([^\]]+)\],\[([^\]]+)\]", html):
    hardware[name] = {'size_mm': ast.literal_eval('['+size+']'),
                      'position_mm': ast.literal_eval('['+position+']')}
assert hardware['board']['size_mm'][:2] == env['pcb']
assert hardware['battery']['size_mm'] == env['battery']
assert hardware['booster']['size_mm'] == env['boost_size']
assert hardware['booster']['position_mm'] == env['boost']
assert np.allclose(np.array(hardware['fan']['position_mm'])[:2]+12.5, env['fan'])
report = {
    'status': 'CAD dimensions verified; physical fit remains unverified',
    'units': 'mm', 'main_body_mm': [env['W'], env['H'], env['depth']],
    'pod_depth_accounting': {'body': pod_body, 'shoe': shoe_depth,
        'body_to_case_gap': round(gap, 5), 'shoe_in_case_overlap': round(overlap, 5),
        'deepest_guard_projection': round(guard_depth, 5),
        'guard_in_case_overlap': round(guard_overlap, 5),
        'lid': 2.4, 'installed_added_depth': round(gap+pod_body+2.4, 5)},
    'parts': parts, 'assemblies': assemblies, 'hardware_envelopes': hardware,
    'construction_dimensions': {
        'wall_mm': env['wall'], 'pcb_edge_gap_mm': env['gap'],
        'mounting_pitch_mm': [94.5, 47.9], 'board_reference_hole_diameter_mm': 3.2,
        'screen_aperture_mm': [78.6, 51.2],
        'track_length_mm': env['rail_len'], 'cover_length_mm': env['rail_len']-2,
        'rail_profile_allowance_mm': env['rail_slop'],
        'stylus_tube_length_mm': env['tube_len'], 'stylus_bore_mm': env['stylus_bore'],
        'stylus_outer_tube_diameter_mm': 12.8,
        'upper_edge_ports_center_x_width_mm': env['upper_ports'],
        'lower_edge_ports_center_x_width_mm': env['lower_ports'],
        'board_edge_port_z_center_height_mm': [env['port_z'], env['port_h']],
        'charge_socket_reservation_width_height_mm': [14, 8],
        'rail_power_contacts': {'mechanism': 'two axial pogo pins and two insulated gold pads',
            'status': 'UNVALIDATED electrical design',
            'pin_sku': '0947-0-15-20-77-14-11-0', 'pad_sku': 'S70-125161545R',
            'pin_derated_current_A': 5.6, 'pad_current_A': 6, 'design_load_A': 2,
            'y_axes_mm': env['contact_y'], 'z_axis_mm': env['contact_z'],
            'free_tip_x_mm_ground_positive': env['contact_tip_x'],
            'seated_pad_face_x_mm': env['contact_face_x'],
            'compression_mm_ground_positive': compression.round(5).tolist(),
            'engagement_order': 'GND first; BAT+ last. Removal BAT+ first.',
            'sequence_margin_mm': 1, 'pod_live_face_recess_mm': 2.2},
        'fan_mounting_pitch_mm': [20, 20], 'fan_hole_diameter_mm': 2.8,
        'fan_exhaust_diameter_mm': 23,
    },
    'limitations': ['Hardware envelopes do not prove component or connector fit.',
        'Port sizes, switch travel, fan holes and antenna clearance remain provisional.'],
}
(ROOT / 'dimension-audit.json').write_text(json.dumps(report, indent=2)+'\n')
print(json.dumps({k: report[k] for k in ['status', 'main_body_mm', 'pod_depth_accounting', 'assemblies']}, indent=2))
