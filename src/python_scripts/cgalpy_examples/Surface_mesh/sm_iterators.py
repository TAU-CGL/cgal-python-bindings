import os
import sys
import importlib
if len(sys.argv) < 2:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm

m = Sm.Surface_mesh_3()

# u            x
# +------------+
# |            |
# |            |
# |      f     |
# |            |
# |            |
# +------------+
# v            w


# Add the points as vertices
u = m.add_vertex(CGALPY.Ker.Point_3(0,1,0))
v = m.add_vertex(CGALPY.Ker.Point_3(0,0,0))
w = m.add_vertex(CGALPY.Ker.Point_3(1,1,0))
x = m.add_vertex(CGALPY.Ker.Point_3(1,0,0))

m.add_face(u,v,w) # f = m.add_face(u,v,w)

print("all vertices ")

# The vertex iterator type is a nested type of the Vertex_range
vb_iter, ve_iter = m.vertices(), m.vertices()
for vb, ve in zip(vb_iter, ve_iter):
    print(f"{vb} {m.point(vb)}")

for vd in m.vertices():
    print(vd)

for vd in m.vertices():
    print(vd)

