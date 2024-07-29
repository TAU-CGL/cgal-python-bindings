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
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp


filename = "meshes/sphere.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    polyhedron = Pol3.read_polygon_mesh(filename)
except:
    print("Invalid input file.")
    exit(1)

mean_curvature_map = Pol3.get(Pol3.dynamic_property_vertex_FT(), polyhedron)
Gaussian_curvature_map = Pol3.get(Pol3.dynamic_property_vertex_FT(), polyhedron)
principal_curvatures_and_directions_map = Pol3.get(Pol3.dynamic_property_vertex_PC(), polyhedron)

Pmp.interpolated_corrected_curvatures(polyhedron,
                                      {"vertex_mean_curvature_map": mean_curvature_map,
                                       "vertex_Gaussian_curvature_map": Gaussian_curvature_map,
                                       "vertex_principal_curvatures_and_directions_map": principal_curvatures_and_directions_map,
                                       # "vertex_point_map": Pol3.get_vertex_point_map(polyhedron),
# uncomment to use an expansion ball radius of 0.5 to estimate the curvatures
                                       # "ball_radius": 0.5
                                       })

i = 0

for v in Pol3.vertices(polyhedron):
    PC = Pol3.get(principal_curvatures_and_directions_map, v)
    print(f"{i}: HC = {Pol3.get(mean_curvature_map, v)}, GC = {Pol3.get(Gaussian_curvature_map, v)}\n"
          f"PC = [ {PC.min_curvature} , {PC.max_curvature} ]")
    i += 1

