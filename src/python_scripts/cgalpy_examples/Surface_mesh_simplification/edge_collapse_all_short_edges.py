import os
import sys
import importlib
lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2

CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Sms = CGALPY.Sms

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/small_cube.off")
i += 1
try:
    surface_mesh = Pol3.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    sys.exit(1)

if not Pol3.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

threshold = float(sys.argv[i]) if len(sys.argv) > i else 0.2
i += 1

print(f"Collapsing edges with length larger than {threshold} of mesh: {filename}...")
r = Sms.edge_collapse(surface_mesh,
                      CGALPY.Sms.Edge_length_stop_predicate(threshold),
                      {"get_cost": Sms.Edge_length_cost(),
                      "get_placement": Sms.Midpoint_placement()})

print(f"\nFinished!\n{r} edges removed.\n{surface_mesh.size_of_halfedges()//2} final edges.")

Pol3.write_polygon_mesh("out.off", surface_mesh, {"stream_precision": 17})
