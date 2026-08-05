# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import sys
import importlib

lib = "CGALPY"
i = 1
if len(sys.argv) > 1:
  arg = sys.argv[1]
  if arg.startswith("CGALPY"):
    lib = arg
    i = 2

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Smdec = CGALPY.Smdec

params = {
  "maximum_depth": 10,
  "volume_error": 0.1,
  "maximum_number_of_convex_volumes": 9,
  "split_at_concavity": True,
  "maximum_number_of_voxels": 1000000,
}

# Try with an empty mesh.
empty_mesh = Sm.Surface_mesh_3()
convex_volumes = Smdec.approximate_convex_decomposition(empty_mesh, params)
assert len(convex_volumes) == 0
print("Empty mesh convex volumes:", len(convex_volumes))

# A sphere is already convex, so it should produce exactly one convex volume.
sphere = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/sphere.off"))
sphere_volumes = Smdec.approximate_convex_decomposition(sphere, params)
assert len(sphere_volumes) == 1
print("Sphere convex volumes:", len(sphere_volumes))

# The knot should be decomposed into at least one and at most nine convex volumes.
knot = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/knot2.off"))
knot_volumes = Smdec.approximate_convex_decomposition(knot, params)
assert len(knot_volumes) > 0
assert len(knot_volumes) <= 9
print("Knot convex volumes:", len(knot_volumes))
