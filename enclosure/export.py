"""Rebuild draft STL parts and true CAD previews using OpenSCAD.

Usage: python export.py --openscad PATH_TO_OPENSCAD_COM
Mesh validation requires numpy, trimesh and networkx. No board uploads or Git operations.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor
import json
from pathlib import Path
import subprocess

ROOT = Path(__file__).resolve().parent
PARTS = ('bezel', 'shell', 'rail_cover', 'stylus_cap', 'pod', 'pod_lid',
         'divider', 'button', 'fit_coupon')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--openscad', required=True)
    args = parser.parse_args()
    import trimesh
    (ROOT / 'draft-stl').mkdir(exist_ok=True)
    (ROOT / 'renders').mkdir(exist_ok=True)

    def build(part):
        path = ROOT / 'draft-stl' / f'{part}.stl'
        cmd = [args.openscad, '--backend', 'CGAL', '-o', str(path), '-D', f'part="{part}"',
               str(ROOT / 'cyd-case.scad')]
        run = subprocess.run(cmd, capture_output=True, text=True)
        if run.returncode or not path.exists() or 'ERROR:' in run.stderr:
            raise RuntimeError(f'{part}: {run.stderr}')
        mesh = trimesh.load_mesh(path)
        pieces = mesh.split(only_watertight=False)
        expected = 3 if part == 'fit_coupon' else 1
        if (not mesh.is_watertight or not mesh.is_winding_consistent
                or mesh.volume <= 0 or len(pieces) != expected):
            raise RuntimeError(f'{part}: invalid topology or unexpected bodies: {len(pieces)}')
        result = dict(part=part, watertight=bool(mesh.is_watertight),
                      bodies=len(pieces), volume_mm3=round(float(mesh.volume), 2),
                      bounds_mm=mesh.bounds.round(3).tolist(),
                      triangles=len(mesh.faces))
        print(f'{part}: watertight, {len(pieces)} bodies', flush=True)
        return result

    with ThreadPoolExecutor(max_workers=2) as pool:
        checks = list(pool.map(build, PARTS))
    report = {'status': 'draft - physical fit and electrical/thermal validation pending',
              'parts': checks}
    (ROOT / 'mesh-checks.json').write_text(json.dumps(report, indent=2) + '\n')
    # OpenCSG views come directly from the same source as the exported STLs.
    views = [('rear', '40,0,25', False), ('front', '125,0,25', False),
             ('battery-installed', '40,0,25', True), ('top-edge', '90,0,180', False)]
    for name, angles, pod in views:
        cmd = [args.openscad, '-o', str(ROOT / 'renders' / f'{name}.png'),
               '--imgsize=1400,1000', '--viewall', '--autocenter',
               f'--camera=0,0,0,{angles},230', '--colorscheme=Tomorrow',
               '-D', f'with_pod={str(pod).lower()}', str(ROOT / 'cyd-case.scad')]
        subprocess.run(cmd, check=True, capture_output=True)
        print(f'Rendered {name}', flush=True)


if __name__ == '__main__':
    main()
