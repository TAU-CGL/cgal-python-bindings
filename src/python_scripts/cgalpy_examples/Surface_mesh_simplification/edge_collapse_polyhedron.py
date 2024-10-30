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
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))
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

# This is a stop predicate (defines when the algorithm terminates).
# In this example, the simplification stops when the number of undirected edges
# left in the surface mesh drops below the specified number (1000)
edge_count_treshold = int(sys.argv[i]) if len(sys.argv) > i else 1000
i += 1
stop = Sms.Edge_count_stop_predicate(edge_count_treshold)

# This the actual call to the simplification algorithm.
# The surface mesh and stop conditions are mandatory arguments.
# The index maps are needed because the vertices and edges
# of this surface mesh lack an "id()" field.
print(f"Collapsing edges of Polyhedron: {filename}, aiming for {edge_count_treshold} final edges...")
r = Sms.edge_collapse(surface_mesh, stop)

print(f"\nFinished!\n{r} edges removed.\n{surface_mesh.size_of_halfedges()//2} final edges.")

Pol3.write_polygon_mesh("out.off", surface_mesh, {"stream_precision": 17})

