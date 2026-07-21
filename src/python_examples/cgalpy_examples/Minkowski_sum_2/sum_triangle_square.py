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
    Ms2 = CGALPY.Ms2

    Point = Ker.Point_2
    Polygon = Pol2.Polygon_2

    # Construct the triangle.
    P = Polygon()
    P.push_back(Point(-1, -1))
    P.push_back(Point(1, -1))
    P.push_back(Point(0, 1))
    print("P =", P)

    # Construct the square.
    Q = Polygon()
    Q.push_back(Point(3, -1))
    Q.push_back(Point(5, -1))
    Q.push_back(Point(5, 1))
    Q.push_back(Point(3, 1))
    print("Q =", Q)

    # Compute the Minkowski sum.
    sum_pwh = Ms2.minkowski_sum_2(P, Q)
    assert sum_pwh.number_of_holes() == 0
    print("P (+) Q =", sum_pwh.outer_boundary())


if __name__ == "__main__":
    main()
