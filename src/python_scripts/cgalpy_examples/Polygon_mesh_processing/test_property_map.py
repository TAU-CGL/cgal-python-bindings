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

mesh = Sm.read_polygon_mesh("meshes/elephant.off")

map, success = mesh.add_property_map_Vertex_point("v:point")
map, success = mesh.add_property_map_Vertex_bool("v:bool")
map, success = mesh.add_property_map_Vertex_size_t("v:size_t")
map, success = mesh.add_property_map_Vertex_vector("v:vector")
map, success = mesh.add_property_map_Vertex_int("v:int")

map, success = mesh.add_property_map_Edge_bool("e:bool")

map, success = mesh.add_property_map_Face_double("f:double")
map, success = mesh.add_property_map_Face_size_t("f:size_t")
map, success = mesh.add_property_map_Face_vector("f:vector")
map, success = mesh.add_property_map_Face_size_t("f:size_t")

map, success = mesh.add_property_map_Halfedge_size_t("h:size_t")




