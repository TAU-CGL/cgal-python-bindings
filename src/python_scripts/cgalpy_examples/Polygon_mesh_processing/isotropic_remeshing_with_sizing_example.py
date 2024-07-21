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

filename = "meshes/nefertiti.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Not a valid input file.")
    sys.exit(1)

print(f"Start remeshing of {filename} ({Sm.num_faces(mesh)} faces)...")
tol = 0.001
edge_min_max = (0.001, 0.5)
sizing_field = Pmp.Adaptive_sizing_field(tol, edge_min_max, Sm.faces(mesh), mesh)

nb_iter = 5

Pmp.isotropic_remeshing(Sm.faces(mesh), sizing_field, mesh,
                        {"number_of_iterations": nb_iter,
                         "number_of_relaxation_steps": 3})

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})

print("Remeshing done.")

