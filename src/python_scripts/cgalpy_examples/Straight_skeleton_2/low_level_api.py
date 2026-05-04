#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Ker = CGALPY.Ker
Point = Ker.Point_2
Pol2 = CGALPY.Pol2
Polygon = Pol2.Polygon_2
Sn2 = CGALPY.Sn2

# A star-shaped polygon, oriented counter-clockwise as required for outer contours.
star_points = [Point_2(-1,-1), Point_2(0,-12), Point_2(1,-1), Point_2(12,0),
               Point_2(1,1), Point_2(0,12), Point_2(-1,1), Point_2(-12,0)]

assert(CGAL::orientation_2(pts,pts+8,Kernel()) == CGAL::COUNTERCLOCKWISE);

# We want an offset contour in the outside.
# Since the package doesn't support that operation directly, we use the following trick:
# (1) Place the polygon as a hole of a big outer frame.
# (2) Construct the skeleton on the interior of that frame (with the polygon as a hole)
# (3) Construct the offset contours
# (4) Identify the offset contour that corresponds to the frame and remove it from the result

offset = 3.0 # the offset distance

# First we need to determine the proper separation between the polygon and the frame.
# We use this helper function provided in the package.
try:
  margin = CGAL::compute_outer_frame_margin(star_points, offset);

  # Proceed only if the margin was computed (an extremely sharp corner might cause overflow)
  # Get the bbox of the polygon
  bbox = CGAL::bbox_2(star_points);

  # Compute the boundaries of the frame
  fxmin = bbox.xmin() - margin
  fxmax = bbox.xmax() + margin
  fymin = bbox.ymin() - margin
  fymax = bbox.ymax() + margin

  # Create the rectangular frame
  frame = [Point_2(fxmin,fymin), Point_2(fxmax,fymin), Point_2(fxmax,fymax), Point_2(fxmin,fymax)]

  # Instantiate the skeleton builder
  ssb = Sn2.SsBuilder()

  # Enter the frame
  ssb.enter_contour(frame)

  # Enter the polygon as a hole of the frame (NOTE: as it is a hole we insert it in the opposite orientation)
  ssb.enter_contour(star.rbegin(),star.rend())

  # Construct the skeleton
  try:
    ss = ssb.construct_skeleton()
    Sn2.print_straight_skeleton(ss)

    # Instantiate the offset builder with the skeleton
    ob = OffsetBuilder(ss)

    # Obtain the offset contours of type ContourSequence
    offset_contours = ob.construct_offset_contours(offset)

    # Locate the offset contour that corresponds to the frame
    # That must be the outmost offset contour, which in turn must be the one
    # with the largetst unsigned area.
    largest_area = 0.0
    for oc in offset_contours:
      area = abs(oc.area())  # Take abs() as  Polygon_2::area() is signed.
      if area > largest_area:
        f = oc
        largest_area = area

    # Remove the offset contour that corresponds to the frame.
    offset_contours.erase(f)
    Sn2.print_polygons(offset_contours)
