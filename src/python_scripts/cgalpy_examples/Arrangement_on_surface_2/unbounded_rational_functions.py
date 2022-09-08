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
shift = Aos2.shift
ipower = Aos2.ipower
Polynomial = Aos2.Polynomial_1
Alg_kernel = Aos2.Algebraic_kernel_d_1
Alg_real = Aos2.Algebraic_real_1
Traits = Aos2.Arr_rational_function_traits_2

# Define a traits class object and a constructor for rational functions.
ak1 = Alg_kernel()
traits = Traits(ak1)
ctr_cv = traits.construct_curve_2_object()

# Define a polynomial representing x.
x = shift(Polynomial(1), 1)

# Define a container storing all arcs.
arcs = []

# Create the arcs (C1, C'1) of the rational functions (y = 1 / x, y = -1 / x).
P1 = Polynomial(1)
minusP1 = -P1
Q1 = x
arcs.append(ctr_cv(P1, Q1))
arcs.append(ctr_cv(minusP1, Q1))

# Create the bounded segments (C2, C'2) of the parabolas (y = -4*x^2 + 3)
# and (y = 4*x^2 - 3), defined over [-sqrt(3)/2, sqrt(3)/2].
P2 = -4*x*x+3
minusP2 = -P2
roots = []
solver = ak1.solve_1_object()
roots = solver(P2)	# [-sqrt(3)/2, sqrt(3)/2]
arcs.append(ctr_cv(P2, roots[0][0], roots[1][0]))
arcs.append(ctr_cv(minusP2, roots[0][0], roots[1][0]))

# Create the arcs (C3, C'3) of (i) the rational function (y = 1 / 2*x) for
# x > 0, and (ii) the rational function (y = -1 / 2*x) for x < 0.
P3 = Polynomial(1)
minusP3 = -P3
Q3 = 2*x
arcs.append(ctr_cv(P3, Q3, Alg_real(0), True))
arcs.append(ctr_cv(minusP3, Q3, Alg_real(0), False))

# Construct the arrangement of the six arcs and print its size.
arr = Arrangement(traits)
Aos2.insert(arr, arcs)
print_unbounded_arrangement_size(arr)
