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
Ker = CGALPY.Ker.Kernel

def print_point_set (point_set: CGALPY.Point_set_3):
    sys.stderr.write("Content of point set:\n")
    for pt in point_set:
        sys.stderr.write(f"* Point {pt}: {point_set.point(pt)} with normal {point_set.normal(pt)}\n")

point_set = CGALPY.Point_set_3()

# Add points
point_set.insert (Ker.Point_3(0., 0., 0.))
point_set.insert (Ker.Point_3(0., 0., 1.))
point_set.insert (Ker.Point_3(0., 1., 0.))

print_point_set(point_set) # Normals have default values

# Change normal values
it = next(iter(point_set))
point_set.set_normal(it, Ker.Vector_3(1., 0., 0.))
point_set.set_normal(it, Ker.Vector_3(0., 1., 0.))
point_set.set_normal(it, Ker.Vector_3(0., 0., 1.))

print_point_set(point_set)

# Add new item
new_item = point_set.insert(Ker.Point_3(1., 2., 3.))
point_set.set_normal(new_item, Ker.Vector_3(4., 5., 6.))

print_point_set(point_set)

# Add new item
new_item = point_set.insert(Ker.Point_3(7., 8., 9.))
point_set.set_normal(new_item, Ker.Vector_3(10., 11., 12.))

print_point_set(point_set) # New item has default values

point_set.remove (point_set.begin() + 2,
                  point_set.begin() + 4)

print_point_set(point_set) # New item has default values

# Display information
sys.stderr.write(f"Number of removed points: {point_set.number_of_removed_points()}\n")

point_set.collect_garbage()

# Display information (garbage should be gone)
sys.stderr.write(f"After garbage collection: {point_set.number_of_removed_points()}\n")

