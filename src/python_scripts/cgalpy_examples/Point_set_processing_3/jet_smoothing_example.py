import sys
import importlib

lib = 'CGALPY'
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str

CGALPY = importlib.import_module(lib)
Psp = CGALPY.Psp
Point_3 = CGALPY.Ker.Kernel.Point_3

points = [Point_3(0.0, 0.0, 0.001),
          Point_3(-0.1, -0.1, 0.002),
          Point_3(-0.1, -0.2, 0.001),
          Point_3(-0.1, 0.1, 0.002),
          Point_3(0.1, -0.1, 0.000),
          Point_3(0.1, 0.2, 0.001),
          Point_3(0.2, 0.0, 0.002),
          Point_3(0.2, 0.1, 0.000),
          Point_3(0.0, -0.1, 0.001)]

# Smoothing.
nb_neighbors = 8
points = Psp.jet_smooth_point_set(points, nb_neighbors)

print(f"{len(points)} point(s) kept")
