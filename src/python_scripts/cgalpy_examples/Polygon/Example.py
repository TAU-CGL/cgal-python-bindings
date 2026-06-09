#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def pretty_number(x):
    if isinstance(x, float) and x.is_integer():
        return str(int(x))
    return str(x)


def pretty_point(x, y):
    return f"PointC2({pretty_number(x)}, {pretty_number(y)})"


def pretty_segment(a, b):
    return f"Segment_2({pretty_point(*a)}, {pretty_point(*b)})"


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Pol2 = CGALPY.Pol2

    Point = Ker.Point_2
    Polygon = Pol2.Polygon_2

    coords = [
        (0, 0),
        (4, 0),
        (4, 4),
        (2, 2),
        (0, 4),
    ]

    p = Polygon()
    for x, y in coords:
        p.push_back(Point(x, y))

    print("created the polygon p:")
    print("Polygon_2(")
    for x, y in coords:
        print(f"  {pretty_point(x, y)}")
    print(")")
    print()
    print()

    is_simple = p.is_simple()
    is_convex = p.is_convex()
    is_clockwise = str(p.orientation()) == "Result.NEGATIVE"
    area = p.area()

    print("polygon p is" + ("" if is_simple else " not") + " simple.")
    print("polygon p is" + ("" if is_convex else " not") + " convex.")
    print("polygon p is" + ("" if is_clockwise else " not") + " clockwise oriented.")
    print(f"the area of polygon p is {pretty_number(area)}")
    print()

    q = Point(1, 1)
    print(f"created point q = {pretty_point(1, 1)}")
    print()

    is_inside = p.bounded_side(q) == CGALPY.ON_BOUNDED_SIDE
    print("point q is" + ("" if is_inside else " not") + " inside polygon p.")
    print()

    for n, (x, y) in enumerate(coords):
        print(f"vertex {n} = {pretty_point(x, y)}")
    print()

    edges = list(zip(coords, coords[1:] + coords[:1]))
    for n, (a, b) in enumerate(edges):
        print(f"edge {n} = {pretty_segment(a, b)}")


if __name__ == "__main__":
    main()
