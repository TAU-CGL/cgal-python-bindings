import os
import sys
import importlib
lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2

CGALPY = importlib.import_module(lib)

points = [CGALPY.Ker.Kernel.Point_3(0.0, 0.0, 0.001),
          CGALPY.Ker.Kernel.Point_3(-0.1,-0.1, 0.002),
          CGALPY.Ker.Kernel.Point_3(-0.1,-0.2, 0.001),
          CGALPY.Ker.Kernel.Point_3(-0.1, 0.1, 0.002),
          CGALPY.Ker.Kernel.Point_3( 0.1,-0.1, 0.000),
          CGALPY.Ker.Kernel.Point_3( 0.1, 0.2, 0.001),
          CGALPY.Ker.Kernel.Point_3( 0.2, 0.0, 0.002),
          CGALPY.Ker.Kernel.Point_3( 0.2, 0.1, 0.000),
          CGALPY.Ker.Kernel.Point_3( 0.0,-0.1, 0.001)]

# Smoothing.
nb_neighbors = 8 # default is 24 for real-life point sets
points = CGALPY.jet_smooth_point_set(points, nb_neighbors)

print(f"{len(points)} point(s) kept")
