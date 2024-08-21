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

point_set = CGALPY.Point_set_3()
CGALPY.read_point_set(CGALPY.data_file_path("points_3/building.ply"), point_set)

param = {"maximum_distance": 0.1,
         "maximum_angle": 10,
         "minimum_region_size": 100,
         "reorient_bbox": True,
         "regularize_parallelism": True,
         "regularize_coplanarity": True,
         "angle_tolerance": 5,
         "maximum_offset": 0.02}

# Algorithm.
ksr = CGALPY.Kinetic_surface_reconstruction_3(point_set, param)

ksr.detection_and_partition(2, param)

vtx = []
polylist = []

lambdas = [0.3, 0.5, 0.7, 0.8, 0.9, 0.95, 0.99]

non_empty = False

for l in lambdas:
    vtx.clear()
    polylist.clear()

    vtx, polylist = ksr.reconstruct_with_ground(l)

    if len(polylist) > 0:
        non_empty = True
        Ker.write_polygon_soup(f"polylist_{l}.ply", vtx, polylist)

exit(1 if non_empty else 0)

