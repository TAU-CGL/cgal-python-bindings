#!/usr/bin/python

import os
import sys
import importlib

lib = 'CGALPY'
filename = 'meshes/elephant.off'
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    if len(sys.argv) > 2: filename = sys.argv[2]
  else:
    filename = str
    if len(sys.argv) > 2: lib = sys.argv[2]

CGALPY = importlib.import_module(lib)
fullname = CGALPY.data_file_path(filename)
if not os.path.isfile(fullname): fullname = filename

Sm = CGALPY.Sm
try: sm = Sm.read_polygon_mesh(fullname)
except:
  print("Invalid input file: ", fullname)
  sys.exit(1)

# Internal color property maps are used if they exist and are called "v:color", "e:color" and "f:color".
vcm = sm.add_property_map_vertex_color("v:color")[0]
ecm = sm.add_property_map_edge_color("e:color")[0]
fcm = sm.add_property_map_face_color("f:color", CGALPY.white())[0]

for v in Sm.vertices(sm):
  if v.idx() % 2:
    vcm[v] = CGALPY.black()
  else:
    vcm[v] = CGALPY.blue()

for e in Sm.edges(sm):
  ecm[e] = CGALPY.gray()

fcm[Sm.faces(sm)[0]] = CGALPY.red()

# Draw!
Sm.draw(sm)
