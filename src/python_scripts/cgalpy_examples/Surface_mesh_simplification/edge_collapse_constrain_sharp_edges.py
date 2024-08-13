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
CGALPY = importlib.import_module(lib)
Pol3 = CGALPY.Pol3
Sms = CGALPY.Sms

def is_border(e, sm):
    return (Pol3.face(Pol3.halfedge(e, sm), sm) == Pol3.null_face()) or (Pol3.face(Pol3.opposite(Pol3.halfedge(e, sm), sm), sm) == Pol3.null_face())

def point(vd, sm):
    return Pol3.get_vertex_point(sm, vd)


filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/cube-meshed.off")
print(f"Reading mesh from file: {filename}")
i += 1

try:
    surface_mesh = Pol3.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    exit(1)

if not Pol3.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    exit(1)


constraint_hmap = Pol3.get(Pol3.Edge_bool_map(), surface_mesh)

# map used to check that constrained_edges and the points of its vertices
# are preserved at the end of the simplification
# Warning: the computation of the dihedral angle is only an approximation and can
#          be far from the real value and could influence the detection of sharp
#          edges after the simplification
constrained_edges = {}
nb_sharp_edges = 0

# detect sharp edges
with open("constrained_edges.polylines.txt", "w") as cst_output:
    for e in Pol3.edges(surface_mesh):
        hd = Pol3.halfedge(e, surface_mesh)
        if is_border(e, surface_mesh):
            print("border")
            nb_sharp_edges += 1
            constraint_hmap[e] = True
            constrained_edges[e] = (point(Pol3.source(hd, surface_mesh), surface_mesh),
                                    point(Pol3.target(hd, surface_mesh), surface_mesh))
        else:
            angle = CGALPY.Ker.approximate_dihedral_angle(point(Pol3.target(Pol3.opposite(hd, surface_mesh), surface_mesh), surface_mesh),
                                                    point(Pol3.target(hd, surface_mesh), surface_mesh),
                                                    point(Pol3.target(Pol3.next(hd, surface_mesh), surface_mesh), surface_mesh),
                                                    point(Pol3.target(Pol3.next(Pol3.opposite(hd, surface_mesh), surface_mesh), surface_mesh), surface_mesh))
            if abs(angle) < 100:
                nb_sharp_edges += 1
                constraint_hmap[e] = True
                p = point(Pol3.source(hd, surface_mesh), surface_mesh)
                q = point(Pol3.target(hd, surface_mesh), surface_mesh)
                constrained_edges[e] = (p, q)
                cst_output.write(f"2 {p} {q}\n")

print(f"# sharp edges = {nb_sharp_edges}")

# Contract the surface mesh as much as possible
stop = Sms.Edge_count_stop_predicate(0)
placement = Sms.Constrained_placement_Midpoint_placement_Edge_bool_map(constraint_hmap)

print(f"Collapsing as many non-sharp edges of mesh: {filename} as possible...")
r = Sms.edge_collapse(surface_mesh, stop,
                      {"edge_is_constrained_map": constraint_hmap,
                       "placement": placement
                       })

print(f"\nFinished!\n{r} edges removed.\n",
      f"{Pol3.num_edges(surface_mesh)} final edges.\n")
Pol3.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh, {"stream_precision": 17})
i += 1

print("Checking sharped edges were preserved...")
# check sharp edges were preserved
constrained_before = len(constrained_edges)
constrained_after = sum([1 for e in Pol3.edges(surface_mesh) if constraint_hmap[e]])
assert constrained_before == constrained_after

print("OK")

print("Check that no removable edge has been forgotten...")
r = Sms.edge_collapse(surface_mesh,
                      stop,
                      {"edge_is_constrained_map": constraint_hmap,
                       "placement": placement})

assert r == 0

if r == 0:
    print("OK")
else:
    print(f"ERROR! {r} edges removed!")
    exit(1)

