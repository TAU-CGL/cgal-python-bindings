#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import arr_print as ap

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpec_aos2Alg'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement_2 = Aos2.Arrangement_2
shift = Aos2.shift
ipower = Aos2.ipower
Polynomial = Aos2.Polynomial_1
Traits = Aos2.Arr_rational_function_traits_2

# Define a traits class object and a constructor for rational functions.
traits = Traits()
ctr_xcv = traits.construct_x_monotone_curve_2_object()

# Define a polynomial representing x.
x = Polynomial(shift(Polynomial(1), 1))

# Define a container storing all arcs.
arcs = []

# Create an arc (C1) supported by the polynomial y = x^4 - 6x^2 + 8,
# defined over the (approximate) interval [-2.1, 2.1].
P1 = Polynomial(ipower(x,4) - 6*x*x + 8)
l = Alg_real(Bound(-2.1))
r = Alg_real(Bound(2.1))
arcs.append(ctr_xcv(P1, l, r))

# Create an arc (C2) supported by the function y = x / (1 + x^2),
# defined over the interval [-3, 3].
P2 = Polynomial(x)
Q2 = Polynomial(1 + x*x)
arcs.append(ctr_xcv(P2, Q2, Alg_real(-3), Alg_real(3)))

# Create an arc (C3) supported by the parbola y = 8 - x^2,
# defined over the interval [-2, 3].
P3 = Polynomial(8 - x*x)
arcs.append(ctr_xcv(P3, Alg_real(-2), Alg_real(3)))

# Create an arc (C4) supported by the line y = -2x,
# defined over the interval [-3, 0].
P4 = Polynomial(-2*x)
arcs.append(ctr_xcv(P4, Alg_real(-3), Alg_real(0)))

# Construct the arrangement of the four arcs.
arr = Arrangement(traits)
Aos2.insert(arr, arcs)
print_arrangement(arr)
