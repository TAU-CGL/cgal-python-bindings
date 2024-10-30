import random
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
Ker = CGALPY.Ker

def print_point_set(point_set: CGALPY.Point_set_3):
    color = point_set.property_map_color("color")
    intensity = point_set.property_map_FT("intensity")
    if color is None or intensity is None:
        print("No color or intensity map")
        return

    print("Content of point set:")
    for it in point_set:
        print(f"* Point {point_set.point(it)} with color {color[it]} and intensity {intensity[it]}")

point_set = CGALPY.Point_set_3()

black = CGALPY.Color(0, 0, 0)
success = False
color_map, success = point_set.add_property_map_color("color", black)
assert success

intensity_map, success = point_set.add_property_map_FT("intensity", 0.)
assert success

point_set.reserve(10)  # For memory optimization
for i in range(10):
    point_set.insert(Ker.Point_3(i, i, i))
    c = CGALPY.Color(random.getrandbits(8), random.getrandbits(8), random.getrandbits(8))
    color_map[i] = c
    intensity_map[i] = random.random()

print_point_set(point_set)

# Remove points with intensity less than 0.5
# Modifying the structure on the go is not recommended
points_to_remove = []
for it in point_set:
    if intensity_map[it] < 0.5:
        points_to_remove.append(it)
for pt in points_to_remove:
    point_set.remove(pt)

print_point_set(point_set)  # point set is filtered

