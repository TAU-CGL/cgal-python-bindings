#!/usr/bin/python3
# export PYTHONPATH=...
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

# An observer that receives notifications of face splits and face mergers.
# class My_observer : public CGAL::Arr_observer<Arrangement> {
# public:
#   My_observer(Arrangement& arr) : CGAL::Arr_observer<Arrangement>(arr) {}

#   virtual void before_split_face(Face_handle, Halfedge_handle e) {
#     std::cout << "-> The insertion of :  [ " << e->curve()
#               << " ]  causes a face to split.\n"
#   }

#   virtual void before_merge_face(Face_handle, Face_handle, Halfedge_handle e) {
#     std::cout << "-> The removal of :  [ " << e->curve()
#               << " ]  causes two faces to merge.\n"
#   }
# }

# Construct the arrangement containing one diamond-shaped face.
arr = Arrangement()
obs = Arr_observer(arr)
# obs.set_before_split_face(lambda f, e:
#                           print('-> The insertion of [{}] causes a face to split'.format(e.curve())))
# obs.set_before_merge_face(lambda f1, f2, e:
#                           print('-> The removal of [{}] causes two faces to merge'.format(e.curve())))
# change to print in C++ style
obs.set_before_split_face(lambda f, e:
                          print('-> The insertion of : [ {} ] causes a face to split.'.format(e.curve())))
obs.set_before_merge_face(lambda f1, f2, e:
                          print('-> The removal of : [ {} ] causes two faces to merge.'.format(e.curve())))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(-1, 0), Point(0, 1)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(0, 1), Point(1, 0)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(1, 0), Point(0, -1)))
Aos2.insert_non_intersecting_curve(arr, Segment(Point(0, -1), Point(-1, 0)))

# Insert a vertical segment dividing the diamond into two, and a
# a horizontal segment further dividing the diamond into four.
s_v = Segment(Point(0, -1), Point(0, 1))
e_v = Aos2.insert_non_intersecting_curve(arr, s_v)
Aos2.insert(arr, Segment(Point(-1, 0), Point(1, 0)))
print_arrangement_size(arr)

# Now remove a portion of the vertical segment.
Aos2.remove_edge(arr, e_v) 	# the observer will make a notification
print_arrangement_size(arr)

del obs
