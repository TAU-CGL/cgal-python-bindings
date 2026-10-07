#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys
from pathlib import Path


def read_polygon(lines, index, Polygon, Point):
    n = int(lines[index])
    index += 1

    points = []
    for _ in range(n):
        x, y = lines[index].split()
        points.append(Point(int(x), int(y)))
        index += 1

    return Polygon(points), index


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]
    filename = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("pgn_holes.dat")

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Point = Ker.Point_2
    Pol2 = CGALPY.Pol2
    Polygon = Pol2.Polygon_2
    Polygon_with_holes = Pol2.Polygon_with_holes_2
    Bso2 = CGALPY.Bso2

    lines = [line.strip() for line in filename.read_text().splitlines() if line.strip()]

    outer, index = read_polygon(lines, 0, Polygon, Point)
    num_holes = int(lines[index])
    index += 1

    holes = []
    for _ in range(num_holes):
        hole, index = read_polygon(lines, index, Polygon, Point)
        holes.append(hole)

    polygon_with_holes = Polygon_with_holes(outer, holes)
    connected_points = Bso2.connect_holes(polygon_with_holes)

    for point in connected_points:
        print(f"({point})  ", end="")
    print()


if __name__ == "__main__":
    main()
