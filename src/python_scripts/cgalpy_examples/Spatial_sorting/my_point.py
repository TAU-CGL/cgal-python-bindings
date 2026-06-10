import sys
import importlib

lib = "CGALPY"
if len(sys.argv) > 1 and sys.argv[1].startswith("CGALPY"):
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
St = CGALPY.St

items = [
  (Ker.Point_2(14, 12), 3),
  (Ker.Point_2(1, 2), 0),
  (Ker.Point_2(414, 2), 5),
  (Ker.Point_2(4, 21), 1),
  (Ker.Point_2(7, 74), 2),
  (Ker.Point_2(74, 4), 4),
]

points = [point for point, _ in items]
colors_by_text = {str(point): color for point, color in items}

St.spatial_sort(points, Ker.Kernel())

print(" ".join(str(colors_by_text[str(point)]) for point in points), end=" \n")
