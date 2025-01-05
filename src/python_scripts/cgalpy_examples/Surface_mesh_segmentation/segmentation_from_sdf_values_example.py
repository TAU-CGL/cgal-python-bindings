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

mesh = Pol3.read_polygon_mesh(CGALPY.data_file_path("meshes/cactus.off"))
if not Pol3.is_triangle_mesh(mesh):
  sys.stderr.write("Input is not a triangle mesh\n")
  sys.exit(1)

# create a property-map for SDF values
sdf_property_map = Pol3.get(Pol3.dynamic_property_face_FT(), mesh)

# compute SDF values using default parameters for number of rays, and cone angle
min_max_sdf = CGALPY.sdf_values(mesh, sdf_property_map)

# create a property-map for segment-ids
segment_property_map = Pol3.get(Pol3.dynamic_property_face_size_t(), mesh)

# segment the mesh using default parameters for number of levels, and smoothing lambda
# Any other scalar values can be used instead of using SDF values computed using the CGAL function
number_of_segments = CGALPY.segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map)

print("Number of segments: ", number_of_segments)
# print segment-ids
for f in Pol3.faces(mesh):
  # ids are between [0, number_of_segments -1]
  print(segment_property_map[f])
print()

number_of_clusters = 4       # use 4 clusters in soft clustering
smoothing_lambda = 0.3  # importance of surface features, suggested to be in-between [0,1]

# Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
# This feature is relevant for segmenting the mesh several times with different parameters.
CGALPY.segmentation_from_sdf_values(
  mesh, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda)
