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
Pol3 = CGALPY.Pol3

# create and read Polyhedron
mesh = Pol3.read_polygon_mesh(CGALPY.data_file_path("meshes/cactus.off"))
if not Pol3.is_triangle_mesh(mesh):
  sys.stderr.write("Input is not a triangle mesh\n")
  sys.exit(1)

# assign id field for each face
face_ids = Pol3.get(Pol3.dynamic_property_face_size_t(), mesh)
face_id = 0
for f in Pol3.faces(mesh):
    face_ids[f] = face_id
    face_id += 1

# create a property-map for SDF values
sdf_property_map = Pol3.get(Pol3.dynamic_property_face_FT(), mesh)

CGALPY.sdf_values(mesh, sdf_property_map)

# access SDF values
for f in Pol3.faces(mesh):
    print(f"{sdf_property_map[f]} ", end="")
print()

# create a property-map for segment-ids
segment_property_map = Pol3.get(Pol3.dynamic_property_face_size_t(), mesh)

CGALPY.segmentation_from_sdf_values(mesh, sdf_property_map, segment_property_map)

# access segment-ids
for f in Pol3.faces(mesh):
    print(f"{segment_property_map[f]} ", end="")
print()

