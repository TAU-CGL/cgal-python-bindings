#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
from arr_print import *

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpec_aos2Alg'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Pt2_Shift = Aos2.Pt2_Shift
Arrangement_2 = Aos2.Arrangement_2
Polynomial_1 = Aos2.Polynomial_1
Polynomial_2 = Aos2.Polynomial_2
Integer = Aos2.Integer
ipower = Aos2.ipower
Traits = Arrangement_2.Geometry_traits_2

# based on https://doc.cgal.org/latest/Arrangement_on_surface_2/Arrangement_on_surface_2_2algebraic_curves_8cpp-example.html#_a2
traits = Traits()

# Functor to create a curve from a Polynomial_2
construct_curve = traits.construct_curve_2_object()

i = Polynomial_2(Polynomial_1(Integer(1)))
shift = Pt2_Shift()
x = shift(i, 1, 0)
y = shift(i, 1, 1)

arr = Arrangement_2(traits)

# Construct an (unbounded line) with equation 3x-5y+2=0
f1 = 3*x-5*y+2*i
cv1 = construct_curve(f1)
print('Inserting curve', f1)
Aos2.insert(arr, cv1)

# Construct the ellipse x^2+3*y^2-10=0
f2 = ipower(x, 2) + 3*i*(ipower(y, 2)) - 10*i
cv2 = construct_curve(f2)
print('Inserting curve', f2)
Aos2.insert(arr, cv2)

# Construct a cubic curve with isoated point, and vertical asymptote
f3 = ipower(x, 2) + ipower(y, 2) + x*ipower(y, 2)
cv3 = construct_curve(f3)
print('Inserting curve', f3)
Aos2.insert(arr, cv3)

# Construct a curve of degree 6 with equation x^6+y^6-x^3y^3-12
f4 = ipower(x,6) + ipower(y,6) - ipower(x,3)*ipower(y,3) - 12*i
cv4 = construct_curve(f4)
print('Inserting curve', f4)
Aos2.insert(arr, cv4)

print_arrangement_size(arr)
