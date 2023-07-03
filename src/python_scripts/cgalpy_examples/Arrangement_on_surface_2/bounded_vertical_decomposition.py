#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
import time
from read_objects import *
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Arrangement.Geometry_traits_2
Segment = Traits.Curve_2
Point = Traits.Point_2

#include <CGAL/Arr_vertical_decomposition_2.h>

# Construct the arrangement.
segments = [Segment(Point(0, 0), Point(3, 3)),
            Segment(Point(3, 3), Point(6, 0)),
            Segment(Point(2, 0), Point(5, 3)),
            Segment(Point(5, 3), Point(8, 0))]
arr = Arrangement()
Aos2.insert(arr, segments)

# Perform vertical ray-shooting from every vertex and locate the feature
# that lie below it and the feature that lies above it.
vd_list = Aos2.decompose(arr)

# Print the results.
for vd_item in vd_list:
  curr = vd_item[1]
  print('Vertex ({}) : '.format(vd_item[0].point()), end='')

  print(' feature below: ', end='')
  if type(curr[0]) == Arrangement.Halfedge: print('[{}]'.format(curr[0].curve()), end='')
  elif type(curr[0]) == Arrangement.Vertex: print('({})'.format(curr[0].point()), end='')
  elif type(curr[0]) == Arrangement.Face: print('NONE', end='')
  else: print('EMPTY', end='')

  print(' feature above: ', end='')
  if type(curr[1]) == Arrangement.Halfedge: print('[{}]'.format(curr[1].curve()))
  elif type(curr[1]) == Arrangement.Vertex: print('({})'.format(curr[1].point()))
  elif  type(curr[1]) == Arrangement.Face: print('NONE')
  else: print('EMPTY')
