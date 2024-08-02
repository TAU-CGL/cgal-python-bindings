import os
import sys
import importlib
if len(sys.argv) < 4:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[3]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename1 = "meshes/blobby.off" if len(sys.argv) < 2 else sys.argv[1]
filename2 = "meshes/eight.off" if len(sys.argv) < 3 else sys.argv[2]

try:
    mesh1 = Sm.read_polygon_mesh(filename1)
    mesh2 = Sm.read_polygon_mesh(filename2)
except:
    print("Invalid input.")
    exit(1)

print(f"Number of vertices before corefinement {Sm.num_vertices(mesh1)} and {Sm.num_vertices(mesh2)}")

Pmp.corefine(mesh1, mesh2)

print(f"Number of vertices after corefinement {Sm.num_vertices(mesh1)} and {Sm.num_vertices(mesh2)}")

Sm.write_polygon_mesh("mesh1_refined.off", mesh1, {"stream_precision": 17})
Sm.write_polygon_mesh("mesh2_refined.off", mesh2, {"stream_precision": 17})

