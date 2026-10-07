# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Smsi = CGALPY.Smsi

mesh = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/eight.off"))

p = Smsi.Edge_count_stop_predicate(300)
Smsi.edge_collapse(mesh, p)

Sm.write_polygon_mesh("out.off", mesh)
