#!/usr/bin/python3

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

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/sphere.off")

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

mean_curvature_map, created = mesh.add_property_map_vertex_FT("v:mean_curvature_map", 0)
assert created

Gaussian_curvature_map, created = mesh.add_property_map_vertex_FT("v:Gaussian_curvature_map", 0)
assert created

principal_curvatures_and_directions_map, created = mesh.add_property_map_vertex_Principal_curvatures_and_directions("v:principal_curvatures_and_directions_map", Pmp.Principal_curvatures_and_directions([0, 0, Ker.Vector_3(0,0,0), Ker.Vector_3(0,0,0)]))
assert created

Pmp.interpolated_corrected_curvatures(mesh,
                                      {"vertex_mean_curvature_map": mean_curvature_map,
                                       "vertex_Gaussian_curvature_map": Gaussian_curvature_map,
                                       "vertex_principal_curvatures_and_directions_map": principal_curvatures_and_directions_map})

for v in Sm.vertices(mesh):
  PC = principal_curvatures_and_directions_map[v]
  print(f"{v.id()}: HC = {round(mean_curvature_map[v], 5)}, GC = {round(Gaussian_curvature_map[v], 5)}")
  print(f", PC = [ {round(PC.min_curvature, 5)} , {round(PC.max_curvature, 5)} ]")
