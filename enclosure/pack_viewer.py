"""Refresh the explorer's embedded CAD meshes after export.py."""
import base64
import json
from pathlib import Path
import re

import numpy as np
import trimesh

root = Path(__file__).resolve().parent
data = {}
for name in ('bezel', 'shell', 'rail_cover', 'stylus_cap', 'pod', 'pod_lid', 'divider', 'button'):
    mesh = trimesh.load_mesh(root / 'draft-stl' / (name + '.stl'))
    assert mesh.is_watertight and len(mesh.vertices) < 65536
    vertices = np.rint(mesh.vertices * 100)
    assert np.max(np.abs(vertices)) < 32767
    data[name] = {'v': base64.b64encode(vertices.astype('<i2').tobytes()).decode(),
                  'f': base64.b64encode(mesh.faces.astype('<u2').tobytes()).decode()}
path = root / 'viewer' / 'fragment.html'
html = path.read_text(encoding='utf-8')
html, count = re.subn(r'(<script type="application/json" id="cyd-case-meshes">).*?(</script>)',
    lambda m: m[1] + json.dumps(data, separators=(',', ':')) + m[2], html, flags=re.S)
assert count == 1
path.write_text(html, encoding='utf-8')
print(f'{len(data)} CAD meshes embedded; {len(html.encode())} bytes')
