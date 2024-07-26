#!/usr/bin/python
import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/sphere.off" if len(sys.argv) < 2 else sys.argv[1]

mesh = Sm.read_polygon_mesh(filename)

for v in Sm.vertices(mesh):
  # h, g = 0.0, 0.0
  p = Pmp.Principal_curvatures_and_directions()
  h, g = Pmp.interpolated_corrected_curvatures(v, mesh,
                                               {'vertex_mean_curvature': True,
                                                'vertex_Gaussian_curvature': True,
                                                'vertex_principal_curvatures_and_directions': p})

  print(f"{v.id()}: HC = {h}, GC = {g}, PC = [ {p.min_curvature} , {p.max_curvature} ]") #### Work in progress
