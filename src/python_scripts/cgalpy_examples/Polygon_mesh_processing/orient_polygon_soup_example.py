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
Pmp = CGALPY.Pmp

def non_manifold_edge(id1, id2, nb_poly):
    print(f"The edge {id1}, {id2} is not manifold: {nb_poly} incident polygons.")

def non_manifold_vertex(id, nb_cycles):
    print(f"The vertex {id} is not manifold: {nb_cycles} connected components of vertices in the link.")

def duplicated_vertex(v1, v2):
    print(f"The vertex {v1} has been duplicated, its new id is {v2}.")

def vertex_id_in_polygon_replaced(p_id, i1, i2):
    print(f"In the polygon {p_id}, the index {i1} has been replaced by {i2}.")

def polygon_orientation_reversed(p_id):
    print(f"The polygon {p_id} has been reversed.")

v = Pmp.Default_orientation_visitor() # the Python user has to del this

Pmp.set_non_manifold_edge(v, non_manifold_edge)
Pmp.set_non_manifold_vertex(v, non_manifold_vertex)
Pmp.set_duplicated_vertex(v, duplicated_vertex)
Pmp.set_vertex_id_in_polygon_replaced(v, vertex_id_in_polygon_replaced)
Pmp.set_polygon_orientation_reversed(v, polygon_orientation_reversed)


filename = "meshes/tet-shuffled.off" if len(sys.argv) < 3 else sys.argv[2]

try:
    points, polygons = Sm.read_polygon_soup(filename)
except:
    print("Cannot open file ")
    exit(1)

if len(points) == 0:
    print("Cannot open file ")
    exit(1)


points, polygons = Pmp.orient_polygon_soup(points, polygons, {"visitor": v})

mesh = Pmp.polygon_soup_to_polygon_mesh(points, polygons)

del v # don't forget to del :)

# this returns a tuple, the pv or pf values are set to none if 
# point_to_vertex_output_iterator or polygon_to_face_output_iterator are True
mesh, pv, pf = Pmp.polygon_soup_to_polygon_mesh(points, polygons,
                                                {"point_to_vertex_output_iterator": True,
                                                 "polygon_to_face_output_iterator": True})


index = 0

#### Work in progress
