import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Sms = CGALPY.Sms

mesh = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/eight.off"))

p = Sms.Edge_count_stop_predicate(300)
Sms.edge_collapse(mesh, p)

Sm.write_polygon_mesh("out.off", mesh)
