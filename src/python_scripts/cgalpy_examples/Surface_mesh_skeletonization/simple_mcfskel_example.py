#### TODO: this leaks ####
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
with open("skel-poly.polylines.txt", "w") as out:
    pol_visitor = Display_polylines(skeleton, out)
    Smsk.split_graph_into_polylines(skeleton, pol_visitor)


# Output skeleton points and the corresponding surface points
with open("correspondance-poly.polylines.txt", "w") as out:
    for v in skeleton.vertex_set():
        for vd in skeleton[v].vertices:
            out.write(f"2 {skeleton[v].point} {Pol3.get_vertex_point(tmesh, vd)}\n")


