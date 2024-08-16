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

f = m.add_face(u,v,w,x)

print(f"vertices around vertex {v}")

#### TODO
