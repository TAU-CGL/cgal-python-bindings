import math
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
Sm = CGALPY.Sm

filename = CGALPY.data_file_path("meshes/cactus.off")

mesh = Sm.read_polygon_mesh(filename)

if not Sm.is_triangle_mesh(mesh):
  sys.stderr.write("Invalid input file.\n")
  sys.exit(1)

sdf_property_map = mesh.add_property_map_face_FT("f:sdf")[0]

# compute SDF values
# We can't use default parameters for number of rays, and cone angle
# and the postprocessing
CGALPY.sdf_values(mesh, sdf_property_map, 2.0 / 3.0 * math.pi, 25, True)

# create a property-map for segment-ids
segment_property_map = mesh.add_property_map_face_size_t("f:sid")[0]

# segment the mesh using default parameters for number of levels, and smoothing lambda
# Any other scalar values can be used instead of using SDF values computed using the CGAL function
number_of_segments = CGALPY.segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map)

print("Number of segments: ", number_of_segments)
# print segment-ids

for fd in Sm.faces(mesh):
  # ids are between [0, number_of_segments -1]
  print(segment_property_map[fd], " ")
print()

number_of_clusters = 4       # use 4 clusters in soft clustering
smoothing_lambda = 0.3  # importance of surface features, suggested to be in-between [0,1]

# Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
# This feature is relevant for segmenting the mesh several times with different parameters.
CGALPY.segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda)
