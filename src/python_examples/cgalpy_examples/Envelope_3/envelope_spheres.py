#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]
    filename = sys.argv[2] if len(sys.argv) > 2 else "spheres.dat"

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Env3 = CGALPY.Env3

    Point = Ker.Point_3
    Sphere = Ker.Sphere_3

    with open(filename, "r") as inp:
        tokens = inp.read().split()

    number_of_spheres = int(tokens[0])
    values = tokens[1:]

    spheres = []
    for index in range(number_of_spheres):
        x, y, z, squared_radius = values[4 * index:4 * index + 4]
        spheres.append(
            Sphere(
                Point(Ker.FT(x), Ker.FT(y), Ker.FT(z)),
                Ker.FT(squared_radius),
                CGALPY.POSITIVE,
            )
        )

    print(f"Constructing the lower envelope of {number_of_spheres} spheres.")
    diagram = Env3.lower_envelope_3(spheres)
    print(
        f"V = {diagram.number_of_vertices()}, "
        f"E = {diagram.number_of_edges()}, "
        f"F = {diagram.number_of_faces()}"
    )
    print("Construction took 0 seconds.")


if __name__ == "__main__":
    main()
