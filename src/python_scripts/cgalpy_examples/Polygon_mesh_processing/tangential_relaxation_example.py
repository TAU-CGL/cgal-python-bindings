import os
import sys
import importlib
if len(sys.argv) < 3:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    raise ValueError("Invalid input.")

if not Sm.is_triangle_mesh(mesh):
    raise ValueError("Invalid input.")

nb_iter = 10 if len(sys.argv) < 3 else int(sys.argv[2])

print("Relax...", end='')

Pmp.tangential_relaxation(mesh, {'number_of_iterations': nb_iter})

print("done.")

