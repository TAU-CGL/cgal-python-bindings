import time
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
Sm = CGALPY.Sm
Sms = CGALPY.Sms

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/fold.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    sys.exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

# This is a stop predicate (defines when the algorithm terminates).
# In this example, the simplification stops when the number of undirected edges
# left in the surface mesh drops below the specified number
stop_n = int(sys.argv[i]) if len(sys.argv) > i else Sm.num_edges(surface_mesh) // 2 - 1
i += 1
stop = Sms.Edge_count_stop_predicate(stop_n)

timer = time.perf_counter_ns()
# This the actual call to the simplification algorithm.
# The surface mesh and stop conditions are mandatory arguments.
# The index maps are needed because the vertices and edges
# of this surface mesh lack an "id()" field.
print(f"Collapsing edges of mesh: {filename}, aiming for {stop_n} final edges...")
filter = Sms.Bounded_normal_change_filter()
r = Sms.edge_collapse(surface_mesh, stop,
                      {"get_cost": Sms.LindstromTurk_cost(),
                       "filter": filter,
                       "get_placement": Sms.LindstromTurk_placement()
                      })
print(f"{(time.perf_counter_ns() - timer) / 1e9} sec")
Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out_py.off", surface_mesh, {"stream_precision": 17})
i += 1
