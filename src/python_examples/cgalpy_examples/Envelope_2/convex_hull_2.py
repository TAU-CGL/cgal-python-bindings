#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def curve_data(curve):
    data = getattr(curve, "data")
    return data() if callable(data) else data


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]
    filename = sys.argv[2] if len(sys.argv) > 2 else "ch_points.dat"

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Aos2 = CGALPY.Aos2
    Env2 = CGALPY.Env2

    Point = Ker.Point_2
    Line = Ker.Line_2
    Labeled_line = Aos2.Arr_curve_data_traits_2.X_monotone_curve_2

    with open(filename, "r") as inp:
        tokens = inp.read().split()

    number_of_points = int(tokens[0])
    coords = tokens[1:]

    points = []
    dual_lines = []

    for index in range(number_of_points):
        x = int(coords[2 * index])
        y = int(coords[2 * index + 1])

        points.append(Point(x, y))

        line = Line(Ker.FT(x), Ker.FT(-1), Ker.FT(-y))
        dual_lines.append(Labeled_line(line, index))

    min_diag = Env2.lower_envelope_x_monotone_2(dual_lines)
    max_diag = Env2.upper_envelope_x_monotone_2(dual_lines)

    def edge_point(edge):
        return points[int(curve_data(edge.curve()))]

    output = [f"The convex hull of {number_of_points} input points:"]

    edge = min_diag.leftmost()
    while edge != min_diag.rightmost():
        output.append(f" ({edge_point(edge)})")
        edge = edge.right().right()

    if int(curve_data(edge.curve())) != int(curve_data(max_diag.leftmost().curve())):
        output.append(f" ({edge_point(edge)})")

    edge = max_diag.leftmost()
    while edge != max_diag.rightmost():
        output.append(f" ({edge_point(edge)})")
        edge = edge.right().right()

    print("".join(output))


if __name__ == "__main__":
    main()
