#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
import time
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2

# Define a traits class object and a constructor for rational functions.
ak1 = AK1()
traits = Traits(ak1)
construct = traits.construct_curve_2_object()

# Define a polynomial representing x.
x = Polynomial(CGAL::shift(Polynomial(1), 1))

# Define a container storing all arcs.
arcs = []

# Create the arcs (C1, C'1) of the rational functions (y = 1 / x, y = -1 / x).
P1 = Polynomial(1)
minusP1 = Polynomial(-P1)
Q1 = = Polynomial(x)
arcs.append(construct(P1, Q1))
arcs.append(construct(minusP1, Q1))

# Create the bounded segments (C2, C'2) of the parabolas (y = -4*x^2 + 3)
# and (y = 4*x^2 - 3), defined over [-sqrt(3)/2, sqrt(3)/2].
P2 = Polynomial(-4*x*x+3)
minusP2 = Polynomial(-P2)
roots = []
roots = ak1.solve_1_object()(P2)	# [-sqrt(3)/2, sqrt(3)/2]
arcs.append(construct(P2, roots[0].first, roots[1].first))
arcs.append(construct(minusP2, roots[0].first, roots[1].first))

# Create the arcs (C3, C'3) of (i) the rational function (y = 1 / 2*x) for
# x > 0, and (ii) the rational function (y = -1 / 2*x) for x < 0.
P3 = Polynomial(1)
minusP3 = Polynomial(-P3)
Q3 = Polynomial(2*x)
arcs.append(construct(P3, Q3, Alg_real(0), true))
arcs.append(construct(minusP3, Q3, Alg_real(0), false))

# Construct the arrangement of the six arcs and print its size.
arr = Arrangement(traits)
insert(arr, arcs)
print_unbounded_arrangement_size(arr)
