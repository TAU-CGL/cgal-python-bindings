import sys
import importlib

lib = "CGALPY"
i = 1
if len(sys.argv) > 1:
  arg = sys.argv[1]
  if arg.startswith("CGALPY"):
    lib = arg
    i = 2

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Smt = CGALPY.Smt

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/double-torus-example.off")

mesh = Sm.read_polygon_mesh(filename)

p1_p2, p2_p3 = Smt.open_path_homotopy(mesh)

print("Path p1 (pink)", "IS" if p1_p2 else "IS NOT", "base point homotopic with path p2 (green).")
print("Path p2 (green)", "IS" if p2_p3 else "IS NOT", "base point homotopic with path p3 (orange).")

assert isinstance(p1_p2, bool)
assert isinstance(p2_p3, bool)
