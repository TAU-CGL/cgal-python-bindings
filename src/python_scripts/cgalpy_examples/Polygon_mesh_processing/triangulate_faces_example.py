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
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else "meshes/P.off"
i += 1
outfilename = sys.argv[i] if len(sys.argv) > i else "P_tri.off"

try:
  mesh = Sm.read_polygon_mesh(filename)
except:
  raise ValueError("Error: Invalid input.")
if mesh.is_empty():
  raise ValueError("Warning: empty file?")
if not Sm.is_triangle_mesh(mesh):
  print("Input mesh is not triangulated.")

if not Pmp.triangulate_faces(mesh):
  raise ValueError("Error: Could not triangulate faces.")

# Confirm that all faces are triangles.
faces = mesh.faces()
for f in faces:
  he = Sm.halfedge(f, mesh)
  if not Sm.is_triangle(he, mesh):
    raise ValueError("Error: non-triangular face left in mesh.")

Sm.write_polygon_mesh(outfilename, mesh)
