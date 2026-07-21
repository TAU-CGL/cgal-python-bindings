#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Pol2 = CGALPY.Pol2

    Point = Ker.Point_2
    Polygon = Pol2.Polygon_2

    p = Polygon()
    p.push_back(Point(0, 0))
    p.push_back(Point(4, 0))
    p.push_back(Point(4, 4))

    for point in p.vertices():
        print(point)

    vertices = p.vertices()
    for point in vertices:
        print(point)

    for edge in p.edges():
        print(edge)


if __name__ == "__main__":
    main()
