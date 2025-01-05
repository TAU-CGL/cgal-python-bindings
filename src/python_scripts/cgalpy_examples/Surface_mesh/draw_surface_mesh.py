#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm

filename = sys.argv[1] if len(sys.argv) > 1 else CGALPY.data_file_path("meshes/elephant.off")

try:
    sm = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input file: ", filename)
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
