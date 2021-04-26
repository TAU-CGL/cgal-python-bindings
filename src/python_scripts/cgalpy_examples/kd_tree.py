#!/usr/bin/python3.9
# export PYTHONPATH=...
import os
import sys
import importlib

if len(sys.argv) < 2:
    print('Library name missing, assuming CGALPY')
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)

Ker = CGALPY.Ker
SS = CGALPY.SS

FT = Ker.FT
Gmpq = Ker.Gmpq
Point_d = SS.Point_d

# Compile the bindings with the CMake variable CGALPY_SPATIAL_SEARCHING_DIMENSION set to 2
assert(SS.get_spatial_searching_dimension() == 2)

k = 3  # for k nearest neighbors

points = [Point_d(2, [FT(n) for n in [4, 0]]), Point_d(2, [FT(n) for n in [-4, 0]]),
          Point_d(2, [FT(n) for n in [40, 0]]), Point_d(2, [FT(n) for n in [-40, 0]]),
          Point_d(2, [FT(n) for n in [1, 0]])]

# Insert the points into a k-d tree
tree = SS.Kd_tree(points)

all_points = []
tree.points(all_points)
for x in all_points:
    print(x)

query = Point_d(2, [FT(n) for n in [0, 0]])

eps = FT(Gmpq(0.0))  # 0.0 for exact NN, otherwise approximate NN
search_nearest = True  # Set this value to False in order to search farthest
sort_neighbors = False  # Set this value to True in order to obtain the neighbors sorted by distance

distance = SS.Euclidean_distance()

# Populate <lst> with the k nearest neighbors to <query> based on the distance metric
lst = []
search = SS.K_neighbor_search(tree, query, k, eps, search_nearest,
                           distance, sort_neighbors)
search.k_neighbors(lst)

print("Found", len(lst), "neighbors")
for pair in lst:
    print("Neighboring point is: ", pair[0])
    print("Squared distance from query is: ", pair[1])

# Search for points inside a sphere
s = SS.Fuzzy_sphere(Point_d(2, [FT(0), FT(0)]), FT(5), FT(0))
res = []
tree.search(s, res)
print("Points within distance 5 from (0,0):")
for p in res:
    print(p)

# Search for points inside a box
s = SS.Fuzzy_iso_box(Point_d(2, [FT(-1), FT(-1)]), Point_d(2, [FT(1), FT(1)]), FT(0))
res = []
tree.search(s, res)
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
# distance = SS.Distance_python(transformed_distance, min_distance_to_rectangle, \
#                            max_distance_to_rectangle, transformed_distance_for_value, \
#                            inverse_of_transformed_distance_for_value)
# K_neighbor_search_python(tree, query, k, eps, search_nearest, \
#                                    distance, sort_neighbors)
