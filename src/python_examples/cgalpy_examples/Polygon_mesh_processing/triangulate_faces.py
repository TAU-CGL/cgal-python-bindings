#!/usr/bin/python

import os
import sys
import importlib

lib = 'CGALPY'
filename = 'meshes/P.off'
outfilename = 'P_tri.off'
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    if len(sys.argv) > 2: filename = sys.argv[2]
    if len(sys.argv) > 3: outfilename = sys.argv[3]
  else:
    filename = str
    if len(sys.argv) > 2: outfilename = sys.argv[2]
    if len(sys.argv) > 3: lib = sys.argv[3]

CGALPY = importlib.import_module(lib)
fullname = CGALPY.data_file_path(filename)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

try: mesh = Sm.read_polygon_mesh(fullname)
except: raise ValueError("Error: Invalid input.")

if mesh.is_empty(): raise ValueError("Warning: empty file?")
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
