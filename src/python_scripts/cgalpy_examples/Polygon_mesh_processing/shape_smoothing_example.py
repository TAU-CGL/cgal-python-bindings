import time
import os
import sys
import importlib
if len(sys.argv) < 5:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[4]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    exit(1)

vcmap, success = mesh.add_property_map_vertex_bool("v:bool")
if not success:
    print("Failed to add property map.")
    exit(1)

nb_iterations = 1000 if len(sys.argv) < 3 else int(sys.argv[2])
time = 0.0001 if len(sys.argv) < 4 else float(sys.argv[3])

constrained_vertices = 0

for v in Sm.vertices(mesh):
    if Sm.is_border(v, mesh):
        vcmap[v] = True
        constrained_vertices += 1

print(f"Constraining: {constrained_vertices} border vertices")

print(f"Smoothing shape... ({nb_iterations} iterations)")
Pmp.smooth_shape(mesh, time, {"number_of_iterations": nb_iterations, "vertex_is_constrained_map": vcmap})

Sm.write_polygon_mesh("mesh_shape_smoothed.off", mesh, {"stream_precision": 17})

print("Done!")

