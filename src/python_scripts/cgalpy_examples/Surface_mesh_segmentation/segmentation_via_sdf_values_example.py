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

# create a property-map for segment-ids
segment_property_map = Pol3.get(Pol3.dynamic_property_face_size_t(), mesh)

# calculate SDF values and segment the mesh using default parameters.
number_of_segments = CGALPY.segmentation_via_sdf_values(mesh, segment_property_map)

print("Number of segments: ", number_of_segments)

# print segment-ids
for f in Pol3.faces(mesh):
  # ids are between [0, number_of_segments -1]
  print(segment_property_map[f])
print()
