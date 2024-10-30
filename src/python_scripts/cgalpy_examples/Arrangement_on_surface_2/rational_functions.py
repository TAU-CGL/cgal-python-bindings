#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import arr_print as ap
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY_kerEpecInt_aos2AlgPl'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
shift = Aos2.shift
ipower = Aos2.ipower
Polynomial = Aos2.Polynomial_1
Alg_real = Aos2.Algebraic_real_1
Bound = Aos2.Bound
Traits = Aos2.Arr_rational_function_traits_2

# Define a traits class object and a constructor for rational functions.
traits = Traits()
ctr_xcv = traits.construct_x_monotone_curve_2_object()

# Define a polynomial representing x.
x = shift(Polynomial(1), 1)

# Define a container storing all arcs.
arcs = []

# Create an arc (C1) supported by the polynomial y = x^4 - 6x^2 + 8,
# defined over the (approximate) interval [-2.1, 2.1].
P1 = ipower(x,4) - 6*x*x + 8
l = Bound(-2.1)		# implicitly converted to Alg_real below
r = Bound(2.1)		# implicitly converted to Alg_real below
arcs.append(ctr_xcv(P1, l, r))

# Create an arc (C2) supported by the function y = x / (1 + x^2),
# defined over the interval [-3, 3].
P2 = x
Q2 = 1 + x*x
arcs.append(ctr_xcv(P2, Q2, Alg_real(-3), Alg_real(3)))

# Create an arc (C3) supported by the parbola y = 8 - x^2,
# defined over the interval [-2, 3].
P3 = 8 - x*x
arcs.append(ctr_xcv(P3, Alg_real(-2), Alg_real(3)))

# Create an arc (C4) supported by the line y = -2x,
# defined over the interval [-3, 0].
P4 = -2*x
arcs.append(ctr_xcv(P4, Alg_real(-3), Alg_real(0)))

# Construct the arrangement of the four arcs.
arr = Arrangement(traits)
Aos2.insert(arr, arcs)
print_arrangement(arr)
