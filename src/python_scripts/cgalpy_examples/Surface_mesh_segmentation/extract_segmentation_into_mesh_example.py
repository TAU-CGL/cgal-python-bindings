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
import CGALPY.CGALPY as CGALPY
Sm = CGALPY.Sm

filename = CGALPY.data_file_path("meshes/cactus.off")

mesh = Sm.read_polygon_mesh(filename)

sdf_property_map = mesh.add_property_map_face_FT("f:sdf")[0]

CGALPY.sdf_values(mesh, sdf_property_map)

# create a property-map for segment-ids
segment_property_map = mesh.add_property_map_face_size_t("f:sid")[0]

# segment the mesh using default parameters for number of levels, and smoothing lambda
# Any other scalar values can be used instead of using SDF values computed using the CGAL function
number_of_segments = CGALPY.segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map)

print("Number of segments:", number_of_segments)

#### TODO: face_filtered_graph ####

