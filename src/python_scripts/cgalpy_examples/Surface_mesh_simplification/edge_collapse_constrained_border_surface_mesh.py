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

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/mesh_with_border.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    sys.exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

constrained_halfedges = surface_mesh.add_property_map_halfedge_tuple("h:vertices")[0]

nb_border_edges = 0
for hd in surface_mesh.halfedges():
    if Sm.is_border(hd, surface_mesh):
        constrained_halfedges[hd] = (surface_mesh.point(surface_mesh.source(hd)),
                                     surface_mesh.point(surface_mesh.target(hd)))
        nb_border_edges += 1

 # Contract the surface mesh as much as possible
stop = Sms.Edge_count_stop_predicate(0)
bem = surface_mesh.add_property_map_edge_bool("e:is_border")[0]
for e in surface_mesh.edges():
    bem[e] = Sm.is_border(e, surface_mesh)

# This the actual call to the simplification algorithm.
# The surface mesh and stop conditions are mandatory arguments.
print(f"Collapsing as many edges of mesh: {filename} as possible...")
r = Sms.edge_collapse(surface_mesh, stop,
                      {"edge_is_constrained_map": bem,
                       "placement": Sms.Constrained_placement_Midpoint_placement_Edge_bool_map(bem)
                      })

print(f"\nFinished!\n{r} edges removed.\n",
      f"{surface_mesh.number_of_edges()} final edges.\n")

Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh, {"stream_precision": 17})
i += 1

# now check!
for hd in surface_mesh.halfedges():
    if Sm.is_border(hd, surface_mesh):
        nb_border_edges -= 1
        if constrained_halfedges[hd] != (surface_mesh.point(surface_mesh.source(hd)),
                                         surface_mesh.point(surface_mesh.target(hd))):
            print("oops. send us a bug report")
assert nb_border_edges == 0
