import sys
import math
import importlib

lib = "CGALPY"
if len(sys.argv) > 1 and sys.argv[1].startswith("CGALPY"):
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ss = CGALPY.Ss
Kerd = CGALPY.Kerd

Point_d = Kerd.Point_d

points = [Point_d(2, [0, 0])]
tree = Ss.Kd_tree(points)

query = Point_d(2, [0, 0])
search = Ss.K_neighbor_search(tree, query, 1)
neighbors = search.k_neighbors()

for point, squared_distance in neighbors:
  x = point.cartesian(0)
  y = point.cartesian(1)
  distance = math.sqrt(float(str(squared_distance)))
  print(f"{x} {y} {distance:g}")
