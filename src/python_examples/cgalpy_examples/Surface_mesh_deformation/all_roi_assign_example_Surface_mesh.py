# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import sys
import importlib
from pathlib import Path

lib = "CGALPY"
i = 1
if len(sys.argv) > 1:
  arg = sys.argv[1]
  if arg.startswith("CGALPY"):
    lib = arg
    i = 2

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Smdef = CGALPY.Smdef

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/plane.off")

mesh = Sm.read_polygon_mesh(filename)

deform_mesh = Smdef.Surface_mesh_deformation(mesh)

# Use the whole mesh as the region of interest.
deform_mesh.insert_roi_vertices(mesh)

control_1 = Sm.Vertex_index(213)
control_2 = Sm.Vertex_index(157)

deform_mesh.insert_control_vertex(control_1)
deform_mesh.insert_control_vertex(control_2)

if not deform_mesh.preprocess():
  sys.stderr.write("Error in preprocessing, check documentation of preprocess()\n")
  sys.exit(1)

deform_mesh.set_target_position(control_1, Ker.Point_3(-0.35, 0.40, 0.60))

deform_mesh.deform()
deform_mesh.deform()

deform_mesh.set_target_position(control_2, Ker.Point_3(0.55, -0.30, 0.70))
deform_mesh.deform(10, 0.0)

out1 = "deform_1.off"
if not Sm.write_polygon_mesh(out1, mesh):
  sys.stderr.write("Could not write deform_1.off\n")
  sys.exit(1)

control_3 = Sm.Vertex_index(92)
deform_mesh.insert_control_vertex(control_3)

if not deform_mesh.preprocess():
  sys.stderr.write("Error in preprocessing, check documentation of preprocess()\n")
  sys.exit(1)

deform_mesh.set_target_position(control_3, Ker.Point_3(0.55, 0.30, -0.70))
deform_mesh.deform(15, 0.0)

out2 = "deform_2.off"
if not Sm.write_polygon_mesh(out2, mesh):
  sys.stderr.write("Could not write deform_2.off\n")
  sys.exit(1)

print("Wrote:", out1, Path(out1).stat().st_size)
print("Wrote:", out2, Path(out2).stat().st_size)

assert Path(out1).stat().st_size > 0
assert Path(out2).stat().st_size > 0
