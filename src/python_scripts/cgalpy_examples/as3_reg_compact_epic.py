#!/usr/bin/python3.7
# export PYTHONPATH=...

import sys
import importlib

if len(sys.argv) < 2:
    sys.exit('Library name missing')

AS3 = importlib.import_module(sys.argv[1]+".AS3")
Ker = importlib.import_module(sys.argv[1]+".Ker")

Point_3 = Ker.Point_3
Weighted_point_3 = Ker.Weighted_point_3
Alpha_shape_3 = AS3.Alpha_shape_3
Classification_type = AS3.Classification_type

p1 = Weighted_point_3(Point_3( 1, -1, -1), 4)
p2 = Weighted_point_3(Point_3(-1,  1, -1), 4)
p3 = Weighted_point_3(Point_3(-1, -1,  1), 4)
p4 = Weighted_point_3(Point_3( 1,  1,  1), 4)
p5 = Weighted_point_3(Point_3( 2,  2,  2), 1)
alphaShape = Alpha_shape_3([p1, p2, p3, p4, p5])

# explore the 0-shape - It is dual to the boundary of the union.
interiorCells = alphaShape.alpha_shape_cells(Classification_type.INTERIOR, 1);
regularFacets = alphaShape.alpha_shape_facets(Classification_type.REGULAR, 1);
singularFacets = alphaShape.alpha_shape_facets(Classification_type.SINGULAR, 1);
singularEdges = alphaShape.alpha_shape_edges(Classification_type.SINGULAR, 1);
print("The 0-shape has : ")
print(len(interiorCells), "interior tetrahedra")
print(len(regularFacets), "regular facets")
print(len(singularFacets), "singular facets")
print(len(singularEdges), "singular edges")

for cell in interiorCells:
    print(cell.is_valid())

for facet in regularFacets:
    print(facet.second)

for edge in singularEdges:
    print(edge.second, edge.third)
