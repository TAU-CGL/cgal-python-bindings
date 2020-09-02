from arr2_epec_seg import *

assert(get_kd_tree_dimension() == 2)  # Compile the bindings with the CMake variable CGALPY_DIMENSION set to 2

k = 3  # for k nearest neighbors

points = [Point_d(2, [FT(n) for n in [4, 0]]), Point_d(2, [FT(n) for n in [-4, 0]]),
          Point_d(2, [FT(n) for n in [40, 0]]), Point_d(2, [FT(n) for n in [-40, 0]]),
          Point_d(2, [FT(n) for n in [1, 0]])]

tree = Kd_tree(points)

all_points = []
tree.points(all_points)
for x in all_points:
    print(x)

query = Point_d(2, [FT(n) for n in [0, 0]])

eps = FT(Gmpq(0.0))  # 0.0 for exact NN, otherwise approximate NN
search_nearest = True  # Set this value to False in order to search farthest
sort_neighbors = False  # Set this value to True in order to obtain the neighbors sorted by distance

distance = Euclidean_distance()

search = K_neighbor_search(tree, query, k, eps, search_nearest,
                           distance, sort_neighbors)

lst = []
search.k_neighbors(lst)

print("Found", len(lst), "neighbors")
for pair in lst:
    print("Neighboring point is: ", pair[0])
    print("Squared distance from query is: ", pair[1])


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
# # For example, if d is a sqaured distance value then its inverse should be sqrt(d)
# def inverse_of_transformed_distance_for_value(d):
#     return FT(1)  # replace this with your implementation
#
#
# distance = Distance_python(transformed_distance, min_distance_to_rectangle, \
#                            max_distance_to_rectangle, transformed_distance_for_value, \
#                            inverse_of_transformed_distance_for_value)
# K_neighbor_search_python(tree, query, k, eps, search_nearest, \
#                                    distance, sort_neighbors)

