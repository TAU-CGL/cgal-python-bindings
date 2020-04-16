#!/usr/bin/python3.7

# export PYTHONPATH="/home/efif/build/cgalpy/regular_release/src/libs/tri3"

from tri3_epic import *
p1 = Weighted_point_3(Point_3( 1, -1, -1), 4)
p2 = Weighted_point_3(Point_3(-1,  1, -1), 4)
p3 = Weighted_point_3(Point_3(-1, -1,  1), 4)
p4 = Weighted_point_3(Point_3( 1,  1,  1), 4)
p5 = Weighted_point_3(Point_3( 2,  2,  2), 1)
alphaShape  = Alpha_shape_3([p1, p2, p3, p4, p5])

# explore the 0-shape - It is dual to the boundary of the union.
# std::list<Cell_handle> cells;
# std::list<Facet>       facets;
# std::list<Edge>        edges;
# as.get_alpha_shape_cells(std::back_inserter(cells),
#                          Alpha_shape_3::INTERIOR);
# as.get_alpha_shape_facets(std::back_inserter(facets),
#                           Alpha_shape_3::REGULAR);
# as.get_alpha_shape_facets(std::back_inserter(facets),
#                           Alpha_shape_3::SINGULAR);
# as.get_alpha_shape_edges(std::back_inserter(edges),
#                          Alpha_shape_3::SINGULAR);
# print(" The 0-shape has : ")
# print(cells.len(), " interior tetrahedra")
# print(facets.len(), " boundary facets")
# print(edges.len(), " singular edges")
