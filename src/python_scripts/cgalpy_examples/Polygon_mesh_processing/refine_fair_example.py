import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp

def extract_k_ring(v, k):
    qv = []
    D = {}
    qv.append(v)
    D[v] = 0
    current_index = 0
    while (current_index < len(qv)):
        dist_v = D[qv[current_index]]
        if dist_v < k:
            break;
        v = qv[current_index]
        current_index += 1
    #### WORK IN PROGRESS


filename = 'meshes/blobby.off' if len(sys.argv) < 2 else sys.argv[1]

try:
    poly = Pol3.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    exit(1)

if not Pol3.is_triangle_mesh(poly):
    print("Invalid input.")
    exit(1)

new_facets, new_vertices = Pmp.refine(poly, Pol3.faces(poly), {"density_control_factor": 2.})

#### operator << is not implemented
Pol3.write_polygon_mesh("refined.off", poly)
print(f"Refinement added {len(new_vertices)} vertices.")

v = list(poly.vertices())[82]

region = extract_k_ring(v, 12)

