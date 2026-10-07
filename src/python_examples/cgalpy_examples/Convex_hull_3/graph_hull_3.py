#!/usr/bin/python

# Author: Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

import importlib
import sys
from pathlib import Path

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Ch3 = CGALPY.Ch3

Point_3 = Ker.Point_3


def read_off_points(path):
    tokens = []
    for line in path.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#") or line == "OFF":
            continue
        tokens.extend(line.split())

    number_of_vertices = int(tokens[0])
    coords = list(map(float, tokens[3:3 + 3 * number_of_vertices]))
    return [
        Point_3(coords[i], coords[i + 1], coords[i + 2])
        for i in range(0, len(coords), 3)
    ]


data_file = Path(__file__).resolve().parents[4] / "cgal" / "Data/data/meshes/star.off"
if not data_file.is_file():
    data_file = Path.home() / "trees/cgal/Data/data/meshes/star.off"

points = read_off_points(data_file)
hull = Ch3.convex_hull_3(points)

print(f"The convex hull contains {hull.number_of_vertices()} vertices")
