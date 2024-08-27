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
Sm = CGALPY.Sm
Smsk = CGALPY.Smsk
Skeletonization = Smsk.Mean_curvature_flow_skeletonization
Skeleton = Skeletonization.Skeleton

tmesh = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/elephant.off"))
if not Sm.is_triangle_mesh(tmesh):
  print("Input geometry is not triangulated.")
  sys.exit(1)

skeleton = Skeleton()
skeletonization = Smsk.Mean_curvature_flow_skeletonization(tmesh)

# 1. Contract the mesh by mean curvature flow.
skeletonization.contract_geometry()

# 2. Collapse short edges and split bad triangles.
skeletonization.collapse_edges()
skeletonization.split_faces()

# 3. Fix degenerate vertices.
skeletonization.detect_degeneracies()

# Perform the above three steps in one iteration.
skeletonization.contract()

# Iteratively apply step 1 to 3 until convergence.
skeletonization.contract_until_convergence()

# Convert the contracted mesh into a curve skeleton and
# get the correspondent surface points
skeletonization.convert_to_skeleton(skeleton)

print(f"Number of vertices of the skeleton: {Smsk.num_vertices(skeleton)}")
print(f"Number of edges of the skeleton: {Smsk.num_edges(skeleton)}")

# Output all the edges of the skeleton.
with open("skel-sm.polylines.txt", "w") as out:
    for e in skeleton.m_edges:
        s = skeleton[e.m_source].point
        t = skeleton[e.m_target].point
        out.write(f"2 {s} {t}\n")

# Output skeleton points and the corresponding surface points
with open("correspondance-sm.polylines.txt", "w") as out:
    for v in skeleton.vertex_set():
        for vd in skeleton[v].vertices:
            out.write(f"2 {skeleton[v].point} {Sm.get_vertex_point(tmesh, vd)}\n")

