#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def read_polygon(CGALPY, inp):
    Ker = CGALPY.Ker
    Pol2 = CGALPY.Pol2

    first = inp.readline()
    while first and not first.strip():
        first = inp.readline()
    if not first:
        raise EOFError("unexpected EOF while reading polygon")

    tokens = first.split()
    n = int(tokens[0])
    coords = tokens[1:]

    while len(coords) < 2 * n:
        coords.extend(inp.readline().split())

    if len(coords) != 2 * n:
        raise ValueError(f"expected {2 * n} coordinates, got {len(coords)}")

    pgn = Pol2.Polygon_2()
    for i in range(n):
        pgn.push_back(Ker.Point_2(Ker.FT(coords[2 * i]), Ker.FT(coords[2 * i + 1])))
    return pgn


def format_polygon(pgn):
    points = [str(point) for point in pgn.vertices()]
    return f"[ {pgn.size()} vertices: (" + "".join(f"({point})" for point in points) + ") ]"


def format_polygon_with_holes(pwh):
    lines = [
        "{ Outer boundary = " + format_polygon(pwh.outer_boundary()),
        f"  {pwh.number_of_holes()} holes:",
    ]
    for i, hole in enumerate(list(pwh.holes()), 1):
        lines.append(f"    Hole #{i} = " + format_polygon(hole))
    lines.append(" }")
    return "\n".join(lines)


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]
    filename = sys.argv[2] if len(sys.argv) > 2 else "rooms_star.dat"

    CGALPY = importlib.import_module(lib)
    Ms2 = CGALPY.Ms2

    with open(filename, "r") as inp:
        P = read_polygon(CGALPY, inp)
        Q = read_polygon(CGALPY, inp)

    sum_pwh = Ms2.minkowski_sum_2(P, Q)
    print("P (+) Q = " + format_polygon_with_holes(sum_pwh))


if __name__ == "__main__":
    main()
