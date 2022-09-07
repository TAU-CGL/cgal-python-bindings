#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import arr_print as ap
from arr_print import *

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpec_aos2Alg'
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
Rational = Aos2.Rational
Traits = Aos2.Arr_rational_function_traits_2

#include <CGAL/CORE_BigInt.h>                      // Integer
#include <CGAL/CORE_BigRat.h>                      // Rational
#include <CGAL/Algebraic_kernel_d_1.h>             // Algebraic Kernel
#include <CGAL/Arr_rational_function_traits_2.h>   // Traits
#include <CGAL/Arrangement_2.h>                    // Arrangement

# typedef CORE::BigInt                               Integer;
# typedef CORE::BigRat                               Rational;
# typedef CGAL::Algebraic_kernel_d_1<Integer>           AK1;
# typedef CGAL::Arr_rational_function_traits_2<AK1>  Traits_2;

# typedef std::vector<Rational>                      Rat_vec;
# typedef Traits_2::Algebraic_real_1                 Alg_real_1;

# typedef CGAL::Arrangement_2<Traits_2>              Arrangement_2;

# CGAL::IO::set_pretty_mode(std::cout);             // for nice printouts.

# Traits class object
traits = Traits()
ctr_arc = traits.construct_x_monotone_curve_2_object()

# container storing all arcs
arcs = []

# Create an arc supported by the function y = 0.1x^4 - 0.6x^2 + 0.8 / 0.1,
# defined over the interval [-2.1, 2.1]:
P1 = []
Q1 = []
P1.append(Rational(8, 10))
P1.append(Rational(0))
P1.append(Rational(-6, 10))
P1.append(Rational(0))
P1.append(Rational(1, 10))

Q1.append(Rational(1,10))

l = Bound(-2.1)
r = Bound(2.1)
arcs.append(ctr_arc(P1, Q1, l, r))

# Create an arc supported by the function y = 0.1x / (0.1 + 0.1x^2),
# defined over the interval [-3, 3]:
P2 = []
Q2 = []
P2.append(Rational(0))
P2.append(Rational(1, 10))

Q2.append(Rational(1, 10))
Q2.append(Rational(0))
Q2.append(Rational(1, 10))

arcs.append(ctr_arc(P2, Q2, Alg_real(-3), Alg_real(3)))

# Create an arc supported by the parabola y = 0.8 - 0.1x^2 / 0.1,
# defined over the interval [-2, 3]:
P3 = []
Q3 = []
P3.append(Rational(4, 5))
P3.append(Rational(0))
P3.append(Rational(-1, 10))

Q3.append(Rational(1, 10))

arcs.append(ctr_arc(P3, Q3, Alg_real(-2), Alg_real(3)))

# Create an arc supported by the line y = -0.2x / 0.1,
# defined over the interval [-3, 0]:
P4 = []
Q4 = []
P4.append(Rational(0))
P4.append(Rational(-1, 5))
Q4.append(Rational(1, 10))
arcs.append(ctr_arc(P4, Q4, Alg_real(-3), Alg_real(0)))

# Print the arcs.
for arc in arcs: print(arc)

# Construct the arrangement of the four arcs.
arr = Arrangement(traits)
Aos2.insert(arr, arcs)
print_arrangement_size(arr)
