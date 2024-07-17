#!/usr/bin/python3

import os
import sys
import time
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Ker = CGALPY.Ker
Kernel = Ker.Kernel
Rat_point = Ker.Point_2

Aos2 = CGALPY.Aos2
Traits = Aos2.Arr_bezier_curve_traits_2
Bezier_point = Traits.Point_2
Bezier_curve = Traits.Curve_2
Bezier_x_monotone_curve = Traits.X_monotone_curve_2

Bso2 = CGALPY.Bso2
Polygon = Bso2.General_polygon_2;
Polygon_with_holes = Bso2.General_polygon_with_holes_2;

# Read Bezier curves from a file
def read_bezier_curves(filename):
  curves = []
  with open(filename, 'r') as f:
    line = next(f)
    n = int(line)
    for i in range(n):
      line = next(f)
      k = int(line)
      points = []
      for j in range(k):
        line = next(f)
        points.append(Rat_point(line))
      cv = Bezier_curve(points)
      curves.append(cv)
    f.close()
  return curves

# Read a general polygon with holes formed by Bezier curves from the file.
def read_bezier_polygon(filename):
  curves = read_bezier_curves(filename)
  if not curves: return False, Polygon_with_holes()

  # Read the curves and construct the general polygon these curves form.
  xcvs = []
  first = True
  pgns = []
  kernel = Kernel()
  equal = kernel.equal_2_object()
  traits = Traits()
  mk_x_monotone = traits.make_x_monotone_2_object()
  for cv in curves:
    results = mk_x_monotone(cv)
    for res in results:
      if type(res) == Bezier_x_monotone_curve: xcvs.append(res)

    # Check whether the current curve closes a polygon namely, whether its
    # target point (the last control point) equals the source of the first
    # curve in the current chain.
    if first:
      # This is the first curve in the chain - store its source point.
      p0 = cv.control_point(0)
      first = False
      continue

    num_points = cv.number_of_control_points()
    # print("No. control points:", num_points)
    if not equal(p0, cv.control_point(num_points - 1)): continue

    # Push a new polygon into the polygon list. Make sure that the polygon
    # is counterclockwise-oriented if it represents the outer boundary
    # and clockwise-oriented if it represents a hole.
    pgn = Polygon(xcvs)
    orient = pgn.orientation()

    if (not pgns and orient == Ker.Result.CLOCKWISE) or (pgns and orient == Ker.Result.COUNTERCLOCKWISE):
      pgn.reverse_orientation()

    pgns.append(pgn)
    xcvs.clear()
    first = True

  if xcvs: return False, Polygon_with_holes()

  # Construct the polygon with holes.
  # The first polygon is the outer boundary and the rest are the holes.
  outer = pgns.pop(0)
  P = Polygon_with_holes(outer, pgns)
  return True, P

# Get the name of the input files from the command line, or use the default
# char_g.dat and char_m.dat files if no command-line parameters are given.
try: filename1 = sys.argv[2]
except: filename1 = 'char_g.dat'
try: filename2 = sys.argv[3]
except: filename1 = 'char_m.dat'

# Read the general polygons from the input files.
#   Polygon_with_holes P1, P2

tic = time.perf_counter()

rc,P1 = read_bezier_polygon(filename1)
if not rc: sys.exit("Failed to read {} ...".format(filename1))
# print(P1)

rc,P2 = read_bezier_polygon(filename2)
if not rc: sys.exit("Failed to read {} ...".format(filename2))
# print(P2)

toc = time.perf_counter()
print(f"Constructed the input polygons in {toc - tic:0.4f} seconds")

# Compute the intersection of the two polygons.
tic = time.perf_counter()
R = Bso2.intersection(P1, P2)
toc = time.perf_counter()

print("The intersection polygons are of sizes: {", end='')
for pgn in R: print(pgn.outer_boundary().size(), end=' ')
print(" }")
print(f"The intersection computation took {toc - tic:0.4f} seconds")
