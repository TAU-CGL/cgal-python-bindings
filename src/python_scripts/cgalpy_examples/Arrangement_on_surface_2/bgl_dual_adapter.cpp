#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import string
from arr_print import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Aos2.Arr_linear_traits_2
X_monotone_curve_2 = Traits.X_monotone_curve_2
Point = Traits.Point_2
Segment = Traits.Segment_2
Line = Traits.Line_2

#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/graph_traits_dual_arrangement_2.h>
#include <CGAL/Arr_face_index_map.h>
#include "Extended_face_property_map.h"
#include "arr_exact_construction_segments.h"
#include "arr_print.h"

# typedef CGAL::Arr_face_extended_dcel<Traits, unsigned int> Dcel
# typedef CGAL::Arrangement_2<Traits, Dcel>                  Ex_arrangement
# typedef CGAL::Dual<Ex_arrangement>                         Dual_arrangement
# typedef CGAL::Arr_face_index_map<Ex_arrangement>           Face_index_map
# typedef Extended_face_property_map<Ex_arrangement,unsigned int>
#                                                            Face_property_map

# Construct an arrangement of seven intersecting line segments.
p1 = Point(1, 1)
p2 = Point(1, 4)
p3 = Point(2, 2)
p4 = Point(3, 7)
p5 = Point(4, 4)
p6 = Point(7, 1)
p7 = Point(9, 3)
arr = Arrangement()
Aos2.insert(arr, Segment(p1, p6))
Aos2.insert(arr, Segment(p1, p4))
Aos2.insert(arr, Segment(p2, p6))
Aos2.insert(arr, Segment(p3, p7))
Aos2.insert(arr, Segment(p3, p5))
Aos2.insert(arr, Segment(p6, p7))
Aos2.insert(arr, Segment(p4, p7))

# Create a mapping of the arrangement faces to indices.
index_map = Face_index_map(arr)

# Perform breadth-first search from the unbounded face, using the event
# visitor to associate each arrangement face with its discover time.
time = -1
boost::breadth_first_search(Dual_arrangement(arr), arr.unbounded_face(),
                            boost::vertex_index_map(index_map).visitor
                            (boost::make_bfs_visitor
                             (stamp_times(Face_property_map(), time,
                                          boost::on_discover_vertex()))))

# Print the discover time of each arrangement face.
f: Arrangement.Face
for f in arr.faces():
  print('Discover time {} for '.format(f.data()), end='')
  if f.is_unbounded(): print("the unbounded face.")
  else:
    print("face ")
    print_ccb(f.outer_ccb())
  print()
