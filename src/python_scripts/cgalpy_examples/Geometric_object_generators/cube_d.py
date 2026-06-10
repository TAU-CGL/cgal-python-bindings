#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Gog = CGALPY.Gog

nb_points = 10
dim = 5
size = 100.0

print(
  f"Generating {nb_points} random points in a cube in "
  f"{dim}D, coordinates from {-size:g} to {size:g}"
)

generator = Gog.Random_points_in_cube_d(dim, size)
points = [next(generator) for _ in range(nb_points)]
for point in points:
  print(f" {point}")
