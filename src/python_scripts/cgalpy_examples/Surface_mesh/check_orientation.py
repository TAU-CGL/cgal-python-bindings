#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm

m = Sm.Surface_mesh_3()
u = m.add_vertex(Point_3(0,1,0));
v = m.add_vertex(Point_3(0,0,0));
w = m.add_vertex(Point_3(1,1,0));
x = m.add_vertex(Point_3(1,0,0));
m.add_face(u,v,w);
f = m.add_face(u,v,x);
if f == Sm.null_face():
  print("The face could not be added because of an orientation error.")
  f = m.add_face(u,x,v);
  assert(f != Sm.null_face());
