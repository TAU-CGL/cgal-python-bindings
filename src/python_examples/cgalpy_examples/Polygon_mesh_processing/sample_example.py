#!/usr/bin/python

# Author: Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Pmp = CGALPY.Pmp

filename = sys.argv[2] if len(sys.argv) > 2 else CGALPY.data_file_path("meshes/eight.off")
mesh = Pmp.read_polygon_mesh(filename)
points = Pmp.sample_triangle_mesh(mesh, {"number_of_points_per_face": 10})

print(len(points))
