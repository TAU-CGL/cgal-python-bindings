import random
import os
import sys
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm


def create_mesh_with_cc_to_orient():
    try:
        mesh = Sm.read_polygon_mesh("meshes/elephant.off")
    except:
        print("Invalid input.")
        exit(1)

    # turn the mesh into a triangle soup, duplicating all the vertices and shuffling orientations
    points = []
    triangles = []
    for f in mesh.faces():
        h = mesh.halfedge(f)
        s = len(points)
        points.append(mesh.point(mesh.source(h)))
        points.append(mesh.point(mesh.target(h)))
        points.append(mesh.point(mesh.target(mesh.next(h))))
        triangles.append([s, s+1, s+2])
        if random.randint(0, 1) == 0:
            triangles[-1][0], triangles[-1][1] = triangles[-1][1], triangles[-1][0]

    Sm.clear(mesh)
    mesh = Pmp.polygon_soup_to_polygon_mesh(points, triangles)[0]
    return mesh


mesh = create_mesh_with_cc_to_orient()
Sm.write_polygon_mesh("to_orient.off", mesh, {"stream_precision": 17})

fbm = mesh.add_property_map_face_bool("fbm", False)[0]
is_orientable = Pmp.compatible_orientations(mesh, fbm)
assert is_orientable

faces_to_reverse = []
for f in mesh.faces():
    if fbm[f]:
        faces_to_reverse.append(f)
Pmp.reverse_face_orientations(faces_to_reverse, mesh)

Pmp.stitch_borders(mesh)

assert Sm.is_closed(mesh)
Sm.write_polygon_mesh("oriented_and_stitched.off", mesh, {"stream_precision": 17})

