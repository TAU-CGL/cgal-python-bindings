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
Sma = CGALPY.Sma

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/bear.off")

mesh = Sm.read_polygon_mesh(filename)
if not Sm.is_triangle_mesh(mesh):
  sys.stderr.write("Invalid input file.\n")
  sys.exit(1)

is_manifold, anchors, triangles = Sma.approximate_triangle_mesh(
  mesh,
  Sma.Verbose_level.MAIN_STEPS,
  200
)

print("#anchor points:", len(anchors))
print("#triangles:", len(triangles))
print("Manifold output:", is_manifold)

assert len(anchors) > 0
assert len(triangles) > 0
