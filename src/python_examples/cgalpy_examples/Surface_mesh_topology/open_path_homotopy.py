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
Smt = CGALPY.Smt

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/double-torus-example.off")

mesh = Sm.read_polygon_mesh(filename)

def create_path_1(path):
  path.push_back_by_index(88)
  for _ in range(3):
    path.extend_positive_turn(2)

def create_path_2(path):
  path.push_back_by_index(300)
  for _ in range(3):
    path.extend_negative_turn(2)

def create_path_3(path):
  path.push_back_by_index(87)
  path.extend_positive_turn(1)
  for _ in range(3):
    path.extend_positive_turn(2)
  path.extend_positive_turn(1)

cst = Smt.Curves_on_surface_topology(mesh)
p1 = Smt.Path_on_surface(mesh)
p2 = Smt.Path_on_surface(mesh)
p3 = Smt.Path_on_surface(mesh)

create_path_1(p1)
create_path_2(p2)
create_path_3(p3)

p1_p2 = cst.are_base_point_homotopic(p1, p2)
p2_p3 = cst.are_base_point_homotopic(p2, p3)

helper_p1_p2, helper_p2_p3 = Smt.open_path_homotopy(mesh)

assert p1_p2 == helper_p1_p2
assert p2_p3 == helper_p2_p3
assert p1.length() == 4
assert p2.length() == 4
assert p3.length() == 6
assert isinstance(p1_p2, bool)
assert isinstance(p2_p3, bool)

print("Path p1 (pink)", "IS" if p1_p2 else "IS NOT", "base point homotopic with path p2 (green).")
print("Path p2 (green)", "IS" if p2_p3 else "IS NOT", "base point homotopic with path p3 (orange).")
