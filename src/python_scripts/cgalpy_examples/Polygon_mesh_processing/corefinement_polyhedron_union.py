#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import timeit
from typing import Any

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Pmp = CGALPY.Pmp

try: filename1 = argv[1]
except: filename1 = 'meshes/blobby.off'
try: filename2 = argv[2]
except: filename2 = 'meshes/eight.off'
# print(filename1, filename2)

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

# Verify that the ids are consecutive
def test_ids(faces, num):
  f = next(faces)
  id = f.id()
  print('First id of mesh {}:'.format(num), id)
  for f in faces:
    id = id + 1
    if f.id() != id:
      raise ValueError('Ids of mesh {} are not consecutive.'.format(num))
  print('Last id of mesh {}:'.format(num), id)

if not (p3() or sm3()): raise ValueError("Cannot find a polygonal mesh.")

try:
  mesh1 = Pm.read_polygon_mesh(filename1)
  mesh2 = Pm.read_polygon_mesh(filename2)
except:
  raise ValueError("Invalid input.")

normalize = 4.0
bound = 0.1
total = 0
count = 0

bound_faces = 0
tfaces = 0

bound_coplanar = 0
tcoplanar = 0
count_coplanar = 0

bound_intersection = 0
tintersection = 0
count_intersection = 0

def start_filtering_intersections():
  print('Visitor::start_filtering_intersections() at {} sec.'.
        format(timeit.timeit()))

def progress_filtering_intersections(d):
  global total, bound
  d = d / normalize
  total = total + d
  if total > bound:
    print('{} % in {} sec.'.format(total*100, f"{timeit.timeit():.2f}"))
    bound = bound + 0.1

def end_filtering_intersections():
  print('Visitor::end_filtering_intersections() at sec.'.
        format(f"{timeit.timeit():.2f}"))

def start_triangulating_faces(tf):
  print('Visitor::start_triangulation() with {} faces at {} sec.'.
        format(tf, f"{timeit.timeit():.2f}"))
  global tfaces, bound_faces
  tfaces = tf
  bound_faces = tf/10

def end_triangulating_faces():
  print('Visitor::end_triangulating_faces() at {} sec.'.
        format(f"{timeit.timeit():.2f}"))

#
def triangulating_faces_step(i):
  global bound_faces, tfaces
  if i > bound_faces:
    print('{} %'.format(float(i)/float(tfaces) * 100))
    bound_faces = bound_faces + tfaces/10

def start_handling_intersection_of_coplanar_faces(tc):
  global tcoplanar
  print('Visitor::start_handling_intersection_of_coplanar_faces() at {} sec.'.
        format(f"{timeit.timeit():.2f}"))
  tcoplanar = tc
  count_coplanar = 0
  bound_coplanar = tcoplanar/10

#
def end_handling_intersection_of_coplanar_faces():
   print('Visitor::end_handling_intersection_of_coplanar_faces() at {} sec.'.
         format(f"{timeit.timeit():.2f}"))

def intersection_of_coplanar_faces_step():
  global count_coplanar, bound_coplanar, tcoplanar
  count_coplanar = count_coplanar + 1
  if count_coplanar > bound_coplanar:
    num = float(count_coplanar)/float(tcoplanar) * 100
    print('Visitor::coplanar_faces: {}'.format(f"{num:.2f}"))
    bound_coplanar = bound_coplanar + tcoplanar/10

#
def start_handling_edge_face_intersections(ti):
  global tintersection, count_intersection, bound_intersection
  print('Visitor::start_handling_edge_face_intersections() at {} sec.'.
        format(f"{timeit.timeit():.2f}"))
  tintersection = ti
  count_intersection = 0
  bound_intersection = tintersection/10

#
def end_handling_edge_face_intersections():
  print('Visitor::end_handling_edge_face_intersections() at {} sec.'.
        format(f"{timeit.timeit():.2f}"))

def edge_face_intersections_step():
  global count_intersection, bound_intersection, tintersection
  count_intersection = count_intersection + 1
  if count_intersection > bound_intersection:
    num = float(count_intersection)/float(tintersection) * 100
    print('Visitor::intersection_points: {} %'.format(f"{num:.2f}"))
    bound_intersection = bound_intersection + tintersection/10

def start_building_output():
  print('Visitor::start_building_output() at {} sec.'.
        format(f"{timeit.timeit():.2f}"))

def end_building_output():
  print('Visitor::end_building_output() at {} sec.'.
        format(f"{timeit.timeit():.2f}"))

cv = Pmp.Corefine_visitor()
cv.set_start_filtering_intersections(start_filtering_intersections)
cv.set_progress_filtering_intersections(progress_filtering_intersections)
cv.set_end_filtering_intersections(end_filtering_intersections)

cv.set_start_triangulating_faces(start_triangulating_faces)
cv.set_end_triangulating_faces(end_triangulating_faces)
# cv.set_triangulating_faces_step(triangulating_faces_step)

cv.set_start_handling_intersection_of_coplanar_faces(start_handling_intersection_of_coplanar_faces)
cv.set_intersection_of_coplanar_faces_step(intersection_of_coplanar_faces_step)
cv.set_end_handling_intersection_of_coplanar_faces(end_handling_intersection_of_coplanar_faces)

cv.set_start_handling_edge_face_intersections(start_handling_edge_face_intersections)
cv.set_edge_face_intersections_step(edge_face_intersections_step)
cv.set_end_handling_edge_face_intersections(end_handling_edge_face_intersections)

cv.set_start_building_output(start_building_output)
cv.set_end_building_output(end_building_output)

start = timeit.timeit()

try:
  out = Pmp.corefine_and_compute_union(mesh1, mesh2, cv)
except:
  raise ValueError("Union could not be computed.")

face = next(mesh1.faces())
if hasattr(face, 'id'):
  test_ids(mesh1.faces(), 1)
  test_ids(mesh2.faces(), 2)
else: print("Face does not have an id.")

print("Union was successfully computed")
out_file = open("union.off", 'w')
out_file.write(str(out))
out_file.close()
del cv
