#!/usr/bin/python3
# export PYTHONPATH=...
import os
import sys
import importlib
import timeit

if len(sys.argv) < 2:
    print('Library name missing, assuming CGALPY')
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]
CGALPY = importlib.import_module(lib)

Kerd = CGALPY.Kerd
Ss = CGALPY.Ss

if hasattr(Kerd, 'FT'):
    FT = Kerd.FT
else:
    FT = float
Point_d = Ss.Point_d

# Verify that the bindings are generated with the CMake flag
# CGALPY_SPATIAL_SEARCHING_DIMENSION set to 2
assert(Ss.get_spatial_searching_dimension() == 2)

k = 3  				# for k nearest neighbors
points = [Point_d(2, [FT(n) for n in [4, 0]]),
          Point_d(2, [FT(n) for n in [-4, 0]]),
          Point_d(2, [FT(n) for n in [40, 0]]),
          Point_d(2, [FT(n) for n in [-40, 0]]),
          Point_d(2, [FT(n) for n in [1, 0]])]
tree = Ss.Kd_tree(points)	# Insert the points into a k-d tree
all_points = tree.points()
for x in all_points:
    print(x)
query = Point_d(2, [FT(n) for n in [0, 0]])
eps = FT(0.0)           # 0.0 for exact NN, otherwise approximate NN
search_nearest = True  	# set to False to search farthest
sort_neighbors = False  # set to True to obtain the neighbors sorted by distance
distance = Ss.Euclidean_distance()	# The distance metric to use

starttime = timeit.default_timer()
# Populate <lst> with the k nearest neighbors to <query> based on the distance metric
search = Ss.K_neighbor_search(tree, query, k, eps, search_nearest,
                              distance, sort_neighbors)
lst = search.k_neighbors()
print("Neighbors search took ", timeit.default_timer() - starttime)

print("Found", len(lst), "neighbors")
for pair in lst:
    print("Neighboring point is: ", pair[0])
    print("Squared distance from query is: ", pair[1])

# Search for points inside a sphere
s = Ss.Fuzzy_sphere(Point_d(2, [FT(0), FT(0)]), FT(5), FT(0))
res = tree.search(s)
print("Points within distance 5 from (0,0):")
for p in res:
    print(p)

# Search for points inside a box
s = Ss.Fuzzy_iso_box(Point_d(2, [FT(-1), FT(-1)]), Point_d(2, [FT(1), FT(1)]), FT(0))
res = tree.search(s)
print("Points with no coordinate exceeding absolute value of 1:")
for p in res:
    print(p)

# CGALPY specific:
# In order to use a custom distance metric, define the following:
# # The following function returns the transformed distance between two points
# # (for Euclidean distance the transformed distance is the squared distance)
# def transformed_distance(p1, p2):
#     return FT(1)  # replace this with your implementation
#
#
# # The following function returns the transformed distance between the query
# # point q and the point on the boundary of the rectangle r closest to q.
# def min_distance_to_rectangle(q, r):
#     return FT(1)  # replace this with your implementation
#
#
# # The following function returns the transformed distance between the query
# # point q and the point on the boundary of the rectangle r furthest to q.
# def max_distance_to_rectangle(q, r):
#     return FT(1)  # replace this with your implementation
#
#
# # The following function returns the transformed distance for a value d
# # For example, if d is a value computed using the Euclidean distance, the transformed distance should be d*d
# def transformed_distance_for_value(d):
#     return FT(1)  # replace this with your implementation
#
#
# # The following function returns the inverse of the transformed distance for a value d
# # For example, if d is a squared distance value then its inverse should be sqrt(d)
# def inverse_of_transformed_distance_for_value(d):
#     return FT(1)  # replace this with your implementation
#
#
# distance = Ss.Distance_python(transformed_distance, min_distance_to_rectangle, \
#                            max_distance_to_rectangle, transformed_distance_for_value, \
#                            inverse_of_transformed_distance_for_value)
# K_neighbor_search_python(tree, query, k, eps, search_nearest, \
#                                    distance, sort_neighbors)
