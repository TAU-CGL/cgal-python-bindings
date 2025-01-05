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
Ker = CGALPY.Ker

point_set = CGALPY.Point_set_3()
CGALPY.read_point_set(CGALPY.data_file_path("points_3/building.ply"), point_set)

param = {"maximum_distance": 0.5,
         "maximum_angle": 10,
         "k_neighbors": 12,
         "minimum_region_size": 50}

# Algorithm.
ksr = CGALPY.Kinetic_surface_reconstruction_3(point_set, param)

ksr.detection_and_partition(1, param)

print(ksr.detect_planar_shapes(), " planar shapes detected")

vtx, polylist = ksr.reconstruct_with_ground(0.5)

if len(polylist) > 0:
    print(f"{len(polylist)} faces in reconstruction")
    Ker.write_polygon_soup("building_0.5.ply", vtx, polylist)
else:
    print("Reconstruction empty!")
    exit(1)
