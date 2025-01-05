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
Sm = CGALPY.Sm
Smsk = CGALPY.Smsk
Skeletonization = Smsk.Mean_curvature_flow_skeletonization
Skeleton = Skeletonization.Skeleton

tmesh = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/elephant.off"))

if not Sm.is_triangle_mesh(tmesh):
  print("Input geometry is not triangulated.")
  sys.exit(1)

skeleton = Skeleton()

Smsk.extract_mean_curvature_flow_skeleton(tmesh, skeleton)

print("Number of vertices of the skeleton:", Smsk.num_vertices(skeleton))
print("Number of edges of the skeleton:", Smsk.num_edges(skeleton))

# Output all the edges of the skeleton.
with open("skel-sm.polylines.txt", "w") as output:
  for e in skeleton.m_edges:
    s = e.m_source
    t = e.m_target
    output.write("2 {} {}\n".format(s, t))
