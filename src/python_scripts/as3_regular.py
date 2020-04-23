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
interiorCells = alphaShape.alpha_shape_cells(Classification_type.INTERIOR, 1);
regularFacets = alphaShape.alpha_shape_facets(Classification_type.REGULAR, 1);
singularFects = alphaShape.alpha_shape_facets(Classification_type.SINGULAR, 1);
singularEdges = alphaShape.alpha_shape_edges(Classification_type.SINGULAR, 1);
print("The 0-shape has : ")
print(len(interiorCells), "interior tetrahedra")
print(len(regularFacets), "regular facets")
print(len(singularFects), "singular facets")
print(len(singularEdges), "singular edges")

for cell in interiorCells:
  print(cell.is_valid())

for facet in regularFacets:
  print(facet.second)

for edge in singularEdges:
  print(edge.second, edge.third)
