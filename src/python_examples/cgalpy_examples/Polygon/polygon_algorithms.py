#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def check_inside(CGALPY, Pol2, pt, points):
    print(f"The point {pt}", end="")
    side = Pol2.bounded_side_2(points, pt)
    if side == CGALPY.ON_BOUNDED_SIDE:
        print(" is inside the polygon.")
    elif side == CGALPY.ON_BOUNDARY:
        print(" is on the polygon boundary.")
    elif side == CGALPY.ON_UNBOUNDED_SIDE:
        print(" is outside the polygon.")
    else:
        raise RuntimeError(f"Unexpected bounded side: {side}")


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Pol2 = CGALPY.Pol2

    Point = Ker.Point_2
    points = [
        Point(0, 0),
        Point(5.1, 0),
        Point(1, 1),
        Point(0.5, 6),
    ]

    print("The polygon is " + ("" if Pol2.is_simple_2(points) else "not ") + "simple.")

    check_inside(CGALPY, Pol2, Point(0.5, 0.5), points)
    check_inside(CGALPY, Pol2, Point(1.5, 2.5), points)
    check_inside(CGALPY, Pol2, Point(2.5, 0), points)


if __name__ == "__main__":
    main()
