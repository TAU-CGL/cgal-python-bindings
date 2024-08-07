#!/usr/bin/python

import os
import sys
import importlib
from typing import Any

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
Pmp = CGALPY.Pmp

Pm: Any = None
def p3():
  global Pm
  try: Pm = CGALPY.Pol3
  except: return False
  else: return True

def sm3():
  global Pm
  try: Pm = CGALPY.Sm
  except: return False
  else: return True

if not (p3() or sm3()): raise ValueError("Cannot find a polygonal mesh.")

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

filename = sys.argv[i] if len(sys.argv) > i else 'meshes/tet-shuffled.off'

try: points, polygons = Pm.read_polygon_soup(filename)
except: raise ValueError("Cannot open file")
if len(points) == 0: raise ValueError("Input file empty")

points, polygons = Pmp.orient_polygon_soup(points, polygons, {"visitor": v})
mesh = Pmp.polygon_soup_to_polygon_mesh(points, polygons)
del v # don't forget to del :)

# this returns a tuple, the pv or pf values are set to none if
# point_to_vertex_output_iterator or polygon_to_face_output_iterator are True
mesh, pv, pf = Pmp.polygon_soup_to_polygon_mesh(points, polygons,
                                                {"point_to_vertex_output_iterator": True,
                                                 "polygon_to_face_output_iterator": True})

index = 0;
for f in mesh.faces():
  f.set_data(index);
  index = index + 1

if (Pmp.is_closed(mesh)):
  Pmp.orient_to_bound_a_volume(mesh)

Pm.write_polygon_mesh("tet-oriented1.off", mesh)
Pmp.reverse_face_orientations(mesh)
Pm.write_polygon_mesh("tet-oriented2.off", mesh)
