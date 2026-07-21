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

    points = [
        Point(0, 0),
        Point(5.1, 0),
        Point(1, 1),
        Point(0.5, 6),
    ]

    pgn = Polygon(points)

    print("The polygon is " + ("" if pgn.is_simple() else "not ") + "simple.")
    print("The polygon is " + ("" if pgn.is_convex() else "not ") + "convex.")


if __name__ == "__main__":
    main()
