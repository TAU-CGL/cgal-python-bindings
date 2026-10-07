# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import sys
import importlib
from pathlib import Path

lib = "CGALPY"
i = 1
if len(sys.argv) > 1:
  arg = sys.argv[1]
  if arg.startswith("CGALPY"):
    lib = arg
    i = 2

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Smp = CGALPY.Smp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/nefertiti.off")
out = sys.argv[i + 1] if len(sys.argv) > i + 1 else "result.off"

mesh = Sm.read_polygon_mesh(filename)
if not Sm.is_triangle_mesh(mesh):
  sys.stderr.write("Invalid input file.\n")
  sys.exit(1)

err = Smp.parameterize_to_off(mesh, out)
if err != Smp.Error_code.OK:
  sys.stderr.write("Error: " + Smp.get_error_message(err) + "\n")
  sys.exit(1)

size = Path(out).stat().st_size
print("Wrote:", out, size)
assert size > 0
