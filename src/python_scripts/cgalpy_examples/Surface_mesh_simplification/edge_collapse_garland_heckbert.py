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
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Sms = CGALPY.Sms

def collapse_gh(gh_policy, mesh, ratio):
    start_time = time.perf_counter_ns()

    stop = Sms.Edge_count_ratio_stop_predicate(ratio)
    gh_cost = gh_policy.get_cost()
    gh_placement = gh_policy.get_placement()
    bncp_chooser = Sms.Bounded_normal_change_placement()
    placement = bncp_chooser(gh_placement)

    r = Sms.edge_collapse(mesh, stop, {
                          "get_cost": gh_cost,
                          "get_placement": placement
                          })

    end_time = time.perf_counter_ns()

    print(f"Time elapsed: {(end_time - start_time)/1e6}ms")
    print(f"\nFinished!\n{r} edges removed.\n{Sm.num_edges(mesh)} final edges.\n")



filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/cube-meshed.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    exit(1)

print(f"Input mesh has {Sm.num_vertices(surface_mesh)} nv",
      f"{Sm.num_edges(surface_mesh)} ne",
      f"{Sm.num_faces(surface_mesh)} nf")

ratio = float(sys.argv[i+1]) if len(sys.argv) > i else 0.2
i += 1
print(f"Collapsing edges of mesh: {filename}, aiming for {100*ratio}% of the input edges...")

policy = sys.argv[i] if len(sys.argv) > i else "cp" # classic plane
i += 1

if (policy == "cp"):
    collapse_gh(Sms.GarlandHeckbert_plane_policies(surface_mesh), surface_mesh, ratio)
elif (policy == "ct"):
    collapse_gh(Sms.GarlandHeckbert_triangle_policies(surface_mesh), surface_mesh, ratio)


Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh)

