#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys
import time


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


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]
    filename = sys.argv[2] if len(sys.argv) > 2 else "tight.dat"

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Ms2 = CGALPY.Ms2

    with open(filename, "r") as inp:
        P = read_polygon(CGALPY, inp)

    print(f"Read an input polygon with {P.size()} vertices.")

    tic = time.perf_counter()
    inset_polygons = Ms2.approximated_inset_2(P, Ker.FT(1), 0.00001)
    toc = time.perf_counter()

    print(f"The inset comprises {len(inset_polygons)} polygon(s).")
    for pgn in inset_polygons:
        print(f"    Polygon with {pgn.size()} vertices.")
    print(f"Inset computation took {toc - tic:.6f} seconds.")


if __name__ == "__main__":
    main()
