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
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY
Pol3 = CGALPY.Pol3

# create and read Polyhedron
mesh = Pol3.read_polygon_mesh(CGALPY.data_file_path("meshes/cactus.off"))
if not Pol3.is_triangle_mesh(mesh):
  sys.stderr.write("Input is not a triangle mesh\n")
  sys.exit(1)

# create a property-map
sdf_property_map = Pol3.get(Pol3.dynamic_property_face_FT(), mesh)

# compute SDF values
min_max_sdf = CGALPY.sdf_values(mesh, sdf_property_map)

# It is possible to compute the raw SDF values and post-process them using
# the following lines:
# number_of_rays = 25  # cast 25 rays per face
# cone_angle = 2.0 / 3.0 * math.pi  # set cone opening-angle
# CGALPY.sdf_values(mesh, sdf_property_map, cone_angle, number_of_rays, False)
# min_max_sdf = CGALPY.sdf_values_postprocessing(mesh, sdf_property_map)

# print minimum & maximum SDF values
print("minimum SDF: ", min_max_sdf[0])
print("maximum SDF: ", min_max_sdf[1])

# print SDF values
for f in Pol3.faces(mesh):
  print(sdf_property_map[f])

print()
