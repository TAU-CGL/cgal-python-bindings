#!/usr/bin/python

import os
import sys
import importlib
from arr_print import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Pt2_Shift = Aos2.Pt2_Shift
Arrangement_2 = Aos2.Arrangement_2
Polynomial_1 = Aos2.Polynomial_1
Polynomial_2 = Aos2.Polynomial_2
Integer = Aos2.Integer
Algebraic_real = Aos2.Algebraic_real_1
ipower = Aos2.ipower
Traits = Arrangement_2.Geometry_traits_2
X_monotone_curve = Traits.X_monotone_curve_2

traits = Traits()

make_xmon = traits.make_x_monotone_2_object()
ctr_cv = traits.construct_curve_2_object()
ctr_pt = traits.construct_point_2_object()
ctr_xseg = traits.construct_x_monotone_segment_2_object()

i = Polynomial_2(Polynomial_1(Integer(1)))
shift = Pt2_Shift()
x = shift(i, 1, 0)
y = shift(i, 1, 1)

# Construct a curve (C1) with the equation x^4+y^3-1=0.
f1 = ipower(x, 4) + ipower(y, 3) - 1
cv1 = ctr_cv(f1)

# Construct all x-monotone segments using the Make_x_mononotone functor.
segs = make_xmon(cv1)

# Cast all CGAL::Objects into X_monotone_segment
# (the vector might also contain Point objects for isolated points,
# but not in this case).
for xcv in segs:
  assert(type(xcv) == X_monotone_curve)

# Construct an ellipse (C2) with the equation 2*x^2+5*y^2-7=0.
f2 = 2*ipower(x, 2) + 5*ipower(y, 2) - 7
cv2 = ctr_cv(f2)

# Construct point on the upper arc (counting of arc numbers starts with 0).
p11 = ctr_pt(Algebraic_real(0), cv2, 1);

segs.extend(ctr_xseg(cv2, p11, Traits.POINT_IN_INTERIOR))

# Construct a vertical cusp (C3) with the equation x^2-y^3=0.
cv3 = ctr_cv(ipower(x, 2) - ipower(y, 3))

# Construct a segment containing the cusp point.
# This adds two X_monotone_curve objects to the vector,
# because the cusp is a critical point.
p21 = ctr_pt(Algebraic_real(-2), cv3, 0)
p22 = ctr_pt(Algebraic_real(2), cv3, 0)
segs.extend(ctr_xseg(cv3 ,p21, p22))

# Construct an unbounded curve, starting at x=3.
p23 = ctr_pt(Algebraic_real(3), cv3, 0)
segs.extend(ctr_xseg(cv3, p23, Traits.MIN_ENDPOINT))

# Construct another conic (C4) with the equation y^2-x^2+1=0.
cv4 = ctr_cv(ipower(y, 2) - ipower(x, 2) + 1)

p31 = ctr_pt(Algebraic_real(2), cv4, 1)
segs.extend(ctr_xseg(cv4, p31, Traits.MAX_ENDPOINT))

# Construct a vertical segment (C5).
cv5 = ctr_cv(x)
v1 = ctr_pt(Algebraic_real(0), cv3, 0)
v2 = ctr_pt(Algebraic_real(0), cv2, 1)
segs.extend(ctr_xseg(cv5, v1, v2))

arr = Arrangement_2(traits)

Aos2.insert(arr, segs)

# Add some isolated points.
ip1 = ctr_pt(Algebraic_real(2), cv4, 0)
ip2 = ctr_pt(Integer(1), Integer(2))
ip3 = ctr_pt(Algebraic_real(-1), Algebraic_real(2))
Aos2.insert(arr, [ip1, ip2, ip3])

print_arrangement_size(arr)
