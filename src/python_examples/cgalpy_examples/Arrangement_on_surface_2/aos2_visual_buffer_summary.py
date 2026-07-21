#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2:
  lib = "CGALPY"
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Bvr = CGALPY.Bvr

Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

segments = [
  Segment(Point(1, 0), Point(2, 4)),
  Segment(Point(5, 0), Point(5, 5)),
  Segment(Point(1, 0), Point(5, 3)),
  Segment(Point(0, 2), Point(6, 0)),
  Segment(Point(3, 0), Point(5, 5)),
]

arr = Arrangement()
Aos2.insert(arr, segments)

scene = Bvr.Graphics_scene()
print("empty_before", scene.empty())
Bvr.add_to_graphics_scene(arr, scene)
print("empty_after", scene.empty())
print("is_two_dimensional", scene.is_two_dimensional())

# Only geometry buffers are compared. COLOR_FACES was observed to be
# nondeterministic across repeated C++/Python runs.
indices = [
  ("POS_POINTS", Bvr.POS_POINTS),
  ("POS_SEGMENTS", Bvr.POS_SEGMENTS),
  ("POS_RAYS", Bvr.POS_RAYS),
  ("POS_LINES", Bvr.POS_LINES),
  ("POS_FACES", Bvr.POS_FACES),
  ("SMOOTH_NORMAL_FACES", Bvr.SMOOTH_NORMAL_FACES),
  ("FLAT_NORMAL_FACES", Bvr.FLAT_NORMAL_FACES),
]

for name, index in indices:
  buf = scene.buffer(index)
  print(
    name,
    "elements", scene.number_of_elements(index),
    "size", scene.get_size_of_index(index),
    "floats", len(buf),
    "sum", format(sum(buf), ".6f"),
  )
