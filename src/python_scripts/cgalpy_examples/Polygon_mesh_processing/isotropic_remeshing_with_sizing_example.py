#!/usr/bin/python

import os
import sys
import importlib

lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/nefertiti.off"))
try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

print(f"Start remeshing of {filename} ({Sm.num_faces(mesh)} faces)...")
tol = 0.001
edge_min_max = (0.001, 0.5)
sizing_field = Pmp.Adaptive_sizing_field(tol, edge_min_max, Sm.faces(mesh), mesh)

nb_iter = 5

Pmp.isotropic_remeshing(Sm.faces(mesh), sizing_field, mesh,
                        {"number_of_iterations": nb_iter,
                         "number_of_relaxation_steps": 3})

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})

print("Remeshing done.")
