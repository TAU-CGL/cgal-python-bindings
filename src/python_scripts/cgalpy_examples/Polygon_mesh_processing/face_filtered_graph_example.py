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

filename = "meshes/blobby_3cc.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    exit(1)

fccmap = mesh.add_property_map_face_size_t("f:CC")[0]
num = Pmp.connected_components(mesh, fccmap)
sys.stderr.write(f"- The graph has {num} connected components (face connectivity)\n")

print("The faces in component 0 are:")
ffg = Sm.Face_filtered_graph(mesh, 0, fccmap)
for f in mesh.faces():
    print(f"f{f}")

if num > 1:
    components = [0, 1]
    print("The faces in components 0 and 1 are:")
    ffg.set_selected_faces(components, fccmap)
    for f in Sm.faces(ffg):
        print(f"f{f}")

