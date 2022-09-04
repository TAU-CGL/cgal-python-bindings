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

class Color(Enum):
  BLUE = 0
  RED = 1
  WHITE = 2

# std::ostream& operator<<(std::ostream& os, const Color& color) {
#   switch (color) {
#    case BLUE: os << "BLUE"  break
#    case RED: os << "RED"   break
#    case WHITE: os << "WHITE" break
#    default: os << "ERROR!"
#   }
#   return os
# }

# std::istream& operator>>(std::istream& is, Color& color) {
#   std::string str
#   is >> str
#   if (str == "BLUE") color = BLUE
#   else if (str == "RED") color = RED
#   else if (str == "WHITE") color = WHITE
#   return is
# }

# typedef CGAL::Arr_extended_dcel<Traits, Color, bool, int>       Ext_dcel
# typedef CGAL::Arrangement_2<Traits, Ext_dcel>                   Ext_arrangement
# typedef CGAL::Arr_extended_dcel_text_formatter<Ext_arrangement> Formatter

# Construct the arrangement containing two intersecting triangles.
arr = Arrangement()

s1 = Segment(Point(4, 1), Point(7, 6))
s2 = Segment(Point(1, 6), Point(7, 6))
s3 = Segment(Point(4, 1), Point(1, 6))
s4 = Segment(Point(1, 3), Point(7, 3))
s5 = Segment(Point(1, 3), Point(4, 8))
s6 = Segment(Point(4, 8), Point(7, 3))

Aos2.insert_non_intersecting_curve(arr, s1)
Aos2.insert_non_intersecting_curve(arr, s2)
Aos2.insert_non_intersecting_curve(arr, s3)
Aos2.insert(arr, s4)
Aos2.insert(arr, s5)
Aos2.insert(arr, s6)

# Go over all arrangement vertices and set their colors.
v: Arrangement.Vertex
for v in arr.vertices():
  degree = v.degree()
  if degree == 0: v.set_data(Color.BLUE)    	# Isolated vertex
   elif if degree <= 2): v.set_data(Color.RED)	# Vertex represents an endpoint
   else v.set_data(Color.WHITE)    # Vertex represents an intersection point
  }

# Go over all arrangement edges and set their flags.
equal = traits.equal_2_object()
e: Arrangement.Halfedge
  # Check if the halfedge has the same direction as its associated
  # segment. Note that its twin always has an opposite direction.
  flag = equal(e.source().point(), e.curve().source())
  e.set_data(flag)
  e.twin().set_data(not flag)

# Go over all arrangement faces and print their outer boundary and indices.
f: Arrangement.Face
for f in arr.faces():
  boundary_size = 0
  if not f.is_unbounded():
    boundary_size = sum(1 for _ in f.outer_ccb())
  f.set_data(boundary_size)

# Write the arrangement to a file.
std::ofstream out_file("arr_ex_dcel_io.dat")
Formatter formatter
CGAL::IO::write(arr, out_file, formatter)
out_file.close()

# Read the arrangement from the file.
arr2 = Arrangement()
std::ifstream in_file("arr_ex_dcel_io.dat")

CGAL::IO::read(arr2, in_file, formatter)
in_file.close()

print("The arrangement vertices:")
for v in arr2.vertices():
  print('({}) - {}'.format(v.point(), v.data()))
