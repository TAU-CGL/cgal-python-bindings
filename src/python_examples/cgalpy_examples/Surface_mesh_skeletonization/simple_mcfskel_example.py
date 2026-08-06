# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import io
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
Pmp = CGALPY.Pmp
Smsk = CGALPY.Smsk
Skeletonization = Smsk.Mean_curvature_flow_skeletonization
Skeleton = Skeletonization.Skeleton

class Display_polylines(Smsk.Polyline_visitor):
  def start_new_polyline_fn(self):
    self.polyline_size = 0

  def add_node_fn(self, v):
    self.polyline_size += 1
    self.out.write(f"{self.skeleton[v].point}")

  def end_polyline_fn(self):
    self.out.write(f" {self.polyline_size}\n")

  def __init__(self, skeleton, out):
    self.skeleton = skeleton
    self.out = out
    self.polyline_size = 0
    Smsk.Polyline_visitor.__init__(self)
    self.set_start_new_polyline(self.start_new_polyline_fn)
    self.set_add_node(self.add_node_fn)
    self.set_end_polyline(self.end_polyline_fn)

def _smsk_noop(*args):
  return None

# This example extracts a medially centered skeleton from a given mesh.
tmesh = Pmp.read_polygon_mesh(CGALPY.data_file_path("meshes/elephant.off"))
if not Pol3.is_triangle_mesh(tmesh):
  print("Input geometry is not triangulated.")
  sys.exit(1)

vertex_point_map = Pol3.get_vertex_point_map(tmesh)

skeleton = Skeleton()

Smsk.extract_mean_curvature_flow_skeleton(tmesh, skeleton)

print(f"Number of vertices of the skeleton: {Smsk.num_vertices(skeleton)}")
print(f"Number of edges of the skeleton: {Smsk.num_edges(skeleton)}")

# Output all the edges of the skeleton.
with open("skel-poly.polylines.txt", "w") as out:
  pol_visitor = Display_polylines(skeleton, out)
  Smsk.split_graph_into_polylines(skeleton, pol_visitor)
  # Break the bound-method callback cycle before deleting the visitor.
  pol_visitor.set_start_new_polyline(_smsk_noop)
  pol_visitor.set_add_node(_smsk_noop)
  pol_visitor.set_end_polyline(_smsk_noop)
  del pol_visitor


# Output skeleton points and the corresponding surface points
with open("correspondance-poly.polylines.txt", "w") as out:
  for v in skeleton.vertex_set():
    for vd in skeleton[v].vertices:
      out.write(f"2 {skeleton[v].point} {vertex_point_map[vd]}\n")
