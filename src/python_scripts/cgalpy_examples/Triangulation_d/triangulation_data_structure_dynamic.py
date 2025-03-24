#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Trid = CGALPY.Trid
Triangulation_ds = Trid.Triangulation_ds

ddim = 5                        # dimension of TDS with dynamic dimension
D = Triangulation_ds(ddim) 	# the argument is taken into account.

assert ddim == D.maximal_dimension()
assert -2 == D.current_dimension()
assert D.is_valid()
V = [None] * 5
V[0] = D.insert_increase_dimension()
V[1] = D.insert_increase_dimension(V[0])
V[2] = D.insert_increase_dimension(V[0])
V[3] = D.insert_increase_dimension(V[0])
V[4] = D.insert_in_full_cell(V[3].full_cell())
assert 6 == D.number_of_full_cells()
assert 2 == D.current_dimension()
assert D.is_valid()
