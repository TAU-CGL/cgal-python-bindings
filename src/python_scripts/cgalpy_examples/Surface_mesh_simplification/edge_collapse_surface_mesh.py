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


filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")
i += 1
try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Failed to read input mesh: " + filename)
    sys.exit(1)
if not Sm.is_triangle_mesh(mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

start_time = time.perf_counter_ns()
print(f"{Sm.num_vertices(mesh)} vertices, {Sm.num_edges(mesh)} edges (BEFORE)")

# In this example, the simplification stops when the number of undirected edges
# drops below 10% of the initial count
stop_ratio = float(sys.argv[i]) if len(sys.argv) > i else 0.1
i += 1
p = Sms.Edge_count_ratio_stop_predicate(stop_ratio)
r = Sms.edge_collapse(mesh, p)

end_time = time.perf_counter_ns()

print(f"\nFinished!\n{r} edges removed.\n{mesh.number_of_edges()} final edges.")
print(f"Time elapsed: {int((end_time - start_time) // 1e6)}ms")

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})
