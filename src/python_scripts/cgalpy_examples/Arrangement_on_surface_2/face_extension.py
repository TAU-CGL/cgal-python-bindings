#!/usr/bin/python

import os
import sys
import importlib
from enum import Enum
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Traits = Arrangement.Geometry_traits_2
Segment = Traits.X_monotone_curve_2
Arr_observer = Aos2.Arr_observer

id = 0
arr = Arrangement()
arr.unbounded_face().set_data(id);
id = id + 1

def index_face(f, new_f, is_hole):
  global id
  new_f.set_data(id)
  id = id + 1

obs = Arr_observer(arr)
obs.set_after_split_face(index_face)

Aos2.insert_non_intersecting_curve(arr, Segment(Point(4, 1), Point(7, 6)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(1, 6), Point(7, 6)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(4, 1), Point(1, 6)))
Aos2.insert(arr, Segment(Point(1, 3), Point(7, 3)))
Aos2.insert(arr, Segment(Point(1, 3), Point(4, 8)))
Aos2.insert(arr, Segment(Point(4, 8), Point(7, 3)))

# Go over all arrangement faces and print the index of each face and its
# outer boundary. The face index is stored in the data field.
print('{} faces:'.format(arr.number_of_faces()))
f: Arrangement.Face
for f in arr.faces():
  print('Face no. {}: '.format(f.data()), end='')
  if f.is_unbounded(): print("Unbounded.")
  else:
    ccb = f.outer_ccb()
    e = next(ccb)
    print('{} -> {}'.format(e.source().point(), e.target().point()), end='')
    for e in ccb: print(' --> {}'.format(e.target().point()), end='')
    print()

del obs
