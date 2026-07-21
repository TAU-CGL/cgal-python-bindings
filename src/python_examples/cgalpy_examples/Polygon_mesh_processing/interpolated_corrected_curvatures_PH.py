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

CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/sphere.off")
try:
  polyhedron = Pmp.read_polygon_mesh(filename)
except:
  raise ValueError("Invalid input.")

mean_curvature_map = Pol3.get_dynamic_vertex_float_map(polyhedron)
Gaussian_curvature_map = Pol3.get_dynamic_vertex_float_map(polyhedron)
principal_curvatures_and_directions_map = Pol3.get_dynamic_vertex_Principal_curvatures_and_directions_map(polyhedron)

Pmp.interpolated_corrected_curvatures(polyhedron,
                                      {"vertex_mean_curvature_map": mean_curvature_map,
                                       "vertex_Gaussian_curvature_map": Gaussian_curvature_map,
                                       "vertex_principal_curvatures_and_directions_map": principal_curvatures_and_directions_map,
                                       # "vertex_point_map": Pol3.get_vertex_point_map(polyhedron),
# uncomment to use an expansion ball radius of 0.5 to estimate the curvatures
                                       # "ball_radius": 0.5
                                       })

i = 0

for v in polyhedron.vertices():
  PC = Pol3.get(principal_curvatures_and_directions_map, v)
  print(f"{i}: HC = {Pol3.get(mean_curvature_map, v)}, GC = {Pol3.get(Gaussian_curvature_map, v)}\n"
        f"PC = [ {PC.min_curvature} , {PC.max_curvature} ]")
  i += 1
