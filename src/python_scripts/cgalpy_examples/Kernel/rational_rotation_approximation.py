#!/usr/bin/python3

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
RT = Ker.RT
dirx = RT(1)
diry = RT(1)
eps_num = RT(1)
eps_den = RT(100)
res = Ker.rational_rotation_approximation(dirx, diry, eps_num, eps_den)
print(res[0], res[1], res[2])
