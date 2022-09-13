#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import string
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Aos2.Arr_segment_traits_2
Point = Traits.Point_2
Segment = Traits.X_monotone_curve_2
Overlay_function_traits = Aos2.Arr_overlay_function_traits

v: Arrangement.Vertex
h: Arrangement.Halfedge
f: Arrangement.Face

vcol1 = 0x00000080
hcol1 = 0x000000ff
fcol1 = 0x00ccccff
vcol2 = 0x00800000
hcol2 = 0x00ff0000
fcol2 = 0x00ffcccc

# Construct the first arrangement and assign colors to its features.
arr1 = Arrangement()
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(0, 0), Point(4, 0)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(0, 2), Point(4, 2)))
Aos2.insert_non_intersecting_curve(arr1, Segment(Point(0, 4), Point(4, 4)))
Aos2.insert(arr1, Segment(Point(0, 0), Point(0, 4)))
Aos2.insert(arr1, Segment(Point(2, 0), Point(2, 4)))
Aos2.insert(arr1, Segment(Point(4, 0), Point(4, 4)))
assert(arr1.number_of_faces() == 5)

for v in arr1.vertices(): v.set_data(vcol1)
for h in arr1.halfedges(): h.set_data(hcol1)
for f in arr1.faces(): f.set_data(fcol1)

# Construct the second arrangement and assign colors to its features.
arr2 = Arrangement()
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(0, 0), Point(6, 0)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(0, 3), Point(6, 3)))
Aos2.insert_non_intersecting_curve(arr2, Segment(Point(0, 6), Point(6, 6)))
Aos2.insert(arr2, Segment(Point(0, 0), Point(0, 6)))
Aos2.insert(arr2, Segment(Point(3, 0), Point(3, 6)))
Aos2.insert(arr2, Segment(Point(6, 0), Point(6, 6)))
assert(arr2.number_of_faces() == 5)

for v in arr2.vertices(): v.set_data(vcol2)
for h in arr2.halfedges(): h.set_data(hcol2)
for f in arr2.faces(): f.set_data(fcol2)

def blend(color1, color2):
  c11 = color1 & 0x000000ff
  c21 = color2 & 0x000000ff
  c12 = color1 & 0x0000ff00
  c22 = color2 & 0x0000ff00
  c13 = color1 & 0x00ff0000
  c23 = color2 & 0x00ff0000
  c1 = int((c11 + c21) / 2) & 0x000000ff
  c2 = int((c12 + c22) / 2) & 0x0000ff00
  c3 = int((c13 + c23) / 2) & 0x00ff0000
  return c1 | c2 | c3;

# Compute the overlay of the two arrangements, while blending the colors
# of their features.
ovl_arr = Arrangement()
# Overlay_color_traits<Ex_arrangement> overlay_traits
ot = Overlay_function_traits()
ot.set_vv_v(blend)
ot.set_ve_v(blend)
ot.set_vf_v(blend)
ot.set_ev_v(blend)
ot.set_fv_v(blend)
ot.set_ee_v(blend)
ot.set_ee_e(blend)
ot.set_ef_e(blend)
ot.set_fe_e(blend)
ot.set_ff_f(blend)

Aos2.overlay(arr1, arr2, ovl_arr, ot)

# Print the overlay-arrangement vertices and their colors.
for v in ovl_arr.vertices():
  print('{}: 0x{}'.format(v.point(), hex(v.data())))

del ot;
# std::hex << std::setfill('0') << std::setw(6) << vit.data()
