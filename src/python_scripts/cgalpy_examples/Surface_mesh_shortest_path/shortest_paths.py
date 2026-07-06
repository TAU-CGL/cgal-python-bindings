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
Smsh = CGALPY.Smsh

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/elephant.off")
seed = int(sys.argv[i + 1]) if len(sys.argv) > i + 1 else 7915421

mesh = Sm.read_polygon_mesh(filename)
if not Sm.is_triangle_mesh(mesh):
  sys.stderr.write("Invalid input file.\n")
  sys.exit(1)

face_location = [0.25, 0.5, 0.25]
target_face_index = Smsh.random_face_index(mesh, seed)
target_face = Sm.Face_index(target_face_index)

shortest_paths = Smsh.Surface_mesh_shortest_path(mesh)

print("Add source:", shortest_paths.point(target_face, face_location))
shortest_paths.add_source_point(target_face, face_location)

out = "shortest_paths_with_id.polylines.txt"
shortest_paths.write_shortest_paths_to_source_points(mesh, out)

size = Path(out).stat().st_size
print("Wrote:", out, size)
assert size > 0
