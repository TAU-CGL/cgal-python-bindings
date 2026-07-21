#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Gog = CGALPY.Gog
Random_points_in_disc_2 = Gog.Random_points_in_disc_2
generator = Random_points_in_disc_2(2)
for i in range(10):
  print(next(generator))
