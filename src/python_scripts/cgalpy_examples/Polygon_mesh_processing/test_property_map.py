import os
import sys
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm

def test(result, iterable):
    map, success = result
    if not success:
        print("Error: could not add property map")
        exit(1)
    for v in iterable:
        print(map[v])
        break

mesh = Sm.read_polygon_mesh("meshes/elephant.off")

test(mesh.add_property_map_vertex_point("v:poinsdat"), mesh.vertices())
test(mesh.add_property_map_vertex_bool("v:bool"), mesh.vertices())
test(mesh.add_property_map_vertex_size_t("v:size_t"), mesh.vertices())
test(mesh.add_property_map_vertex_vector("v:vector"), mesh.vertices())

test(mesh.add_property_map_vertex_int("v:int"), mesh.vertices())

test(mesh.add_property_map_edge_bool("e:bool"), mesh.edges())

test(mesh.add_property_map_face_double("f:double"), mesh.faces())
test(mesh.add_property_map_face_size_t("f:size_t"), mesh.faces())
test(mesh.add_property_map_face_vector("f:vector"), mesh.faces())
test(mesh.add_property_map_face_size_t("f:sizedas_t"), mesh.faces())

test(mesh.add_property_map_halfedge_size_t("h:size_t"), mesh.halfedges())


