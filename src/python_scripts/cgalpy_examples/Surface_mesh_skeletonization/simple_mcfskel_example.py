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
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY
Pol3 = CGALPY.Pol3
Smsk = CGALPY.Smsk
Skeletonization = Smsk.Mean_curvature_flow_skeletonization
Skeleton = Skeletonization.Skeleton

class Display_polylines:
  def start_new_polyline(self):
    self.polyline_size = 0
    self.sstr.seek(0)
    self.sstr.truncate(0)

  def add_node(self, v):
    self.polyline_size += 1
    #### TODO ####
    self.sstr.write(f" {self.skeleton[v].point}")

  def end_polyline(self):
    self.out.write(f"{self.polyline_size}{self.sstr.getvalue()}\n")

  def __init__(self, skeleton, out):
    self.skeleton = skeleton
    self.out = out
    self.polyline_size = 0
    self.sstr = io.StringIO()
    self.visitor = Pol3.Polyline_visitor()
    self.visitor.set_start_new_polyline(self.start_new_polyline)
    self.visitor.set_add_node(self.add_node)
    self.visitor.set_end_polyline(self.end_polyline)

  def __del__(self):
    self.sstr.close()
    del self.visitor

# This example extracts a medially centered skeleton from a given mesh.
tmesh = Pol3.read_polygon_mesh(CGALPY.data_file_path("meshes/elephant.off"))
if not Pol3.is_triangle_mesh(tmesh):
  print("Input geometry is not triangulated.")
  sys.exit(1)

skeleton = Skeleton()

Smsk.extract_mean_curvature_flow_skeleton(tmesh, skeleton)

print(f"Number of vertices of the skeleton: {Smsk.num_vertices(skeleton)}")
print(f"Number of edges of the skeleton: {Smsk.num_edges(skeleton)}")

# Output all the edges of the skeleton.
out = open("skel-poly.polylines.txt", "w")
pol_visitor = Display_polylines(skeleton, out)
Pol3.split_graph_into_polylines(tmesh, pol_visitor.visitor)
out.close()

#### TODO ####
# Output skeleton points and the corresponding surface points
# out = open("correspondance-poly.polylines.txt", "w")

