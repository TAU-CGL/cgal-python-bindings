#!/usr/bin/python3.9
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY_kerEpec_arr2AlgEx'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Arr2 = CGALPY.Arr2
Traits = Arr2.Traits
PT_2_Shift = Arr2.PT_2_Shift
Polynomial_2 = Arr2.Polynomial_2
Polynomial_1 = Arr2.Polynomial_1
Integer = Arr2.Integer
Arrangement_2 = Arr2.Arrangement_2
ipower = Arr2.ipower

# based on https://doc.cgal.org/latest/Arrangement_on_surface_2/Arrangement_on_surface_2_2algebraic_curves_8cpp-example.html#_a2
traits = Arr2.Traits()

# Functor to create a curve from a Polynomial_2
construct_curve = traits.construct_curve_2_object()

i = Polynomial_2(Polynomial_1(Integer(1)))
shift = PT_2_Shift()
x = shift(i, 1, 0)
y = shift(i, 1, 1)

arr = Arrangement_2()

# Construct an (unbounded line) with equation 3x-5y+2=0
f1 = 3*x-5*y+2*i
cv1 = construct_curve(f1)
print('Adding curve', f1, "to the arrangement")
Arr2.insert(arr, cv1)

# Construct the ellipse x^2+3*y^2-10=0
f2 = ipower(x, 2) + 3*i*(ipower(y, 2)) - 10*i
cv2 = construct_curve(f2)
print('Adding curve', f2, "to the arrangement")
Arr2.insert(arr, cv2)

# Construct a cubic curve with isoated point, and vertical asymptote
f3 = ipower(x, 2) + ipower(y, 2) + x*ipower(y, 2)
cv3 = construct_curve(f3)
print('Adding curve', f3, "to the arrangement")
Arr2.insert(arr, cv3)

# Construct a curve of degree 6 with equation x^6+y^6-x^3y^3-12
f4 = ipower(x,6)+ ipower(y,6) - ipower(x,3)*ipower(y,3) - 12*i
cv4 = construct_curve(f4)
print('Adding curve', f4, "to the arrangement")
Arr2.insert(arr, cv4)

print( "The arrangement size:")
print("V =", arr.number_of_vertices(), "E=", arr.number_of_edges(), "F=", arr.number_of_faces())