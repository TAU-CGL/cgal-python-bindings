#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys
import time
from fractions import Fraction
from pathlib import Path


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]
    filename1 = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("char_g.bps")
    filename2 = Path(sys.argv[3]) if len(sys.argv) > 3 else Path("char_m.bps")
    bop = sys.argv[4] if len(sys.argv) > 4 else "i"

    CGALPY = importlib.import_module(lib)

    Ker = CGALPY.Ker
    rat_point = Ker.Point_2

    Aos2 = CGALPY.Aos2
    Traits = Aos2.Arr_bezier_curve_traits_2
    bezier_curve = Traits.Curve_2
    bezier_x_monotone_curve = Traits.X_monotone_curve_2

    Bso2 = CGALPY.Bso2
    polygon = Bso2.General_polygon_2
    polygon_with_holes = Bso2.General_polygon_with_holes_2
    polygon_set = Bso2.General_polygon_set_2

    Result = CGALPY.Result

    traits = Traits()
    make_x_monotone = traits.make_x_monotone_2_object()

    def orientation_to_int(orientation):
        if orientation == Result.POSITIVE:
            return 1
        if orientation == Result.NEGATIVE:
            return -1
        return 0

    def make_point(x, y, double_format):
        if double_format:
            return rat_point(float(x), float(y))

        text = f"{x} {y}"
        try:
            return rat_point(text)
        except BaseException:
            return rat_point(float(Fraction(x)), float(Fraction(y)))

    def read_bezier_curve(tokens, index, double_format):
        n = int(tokens[index])
        index += 1

        control_points = []
        for _ in range(n):
            x = tokens[index]
            y = tokens[index + 1]
            index += 2

            point = make_point(x, y, double_format)
            if not control_points or str(control_points[-1]) != str(point):
                control_points.append(point)

        return bezier_curve(control_points), index

    def read_bezier_polygon_set(filename):
        print(f"Reading {filename}")

        tokens = filename.read_text().split()
        index = 0

        file_format = tokens[index]
        index += 1
        double_format = file_format.startswith("DOUBLE")

        number_of_regions = int(tokens[index])
        index += 1

        result_set = polygon_set()

        for _ in range(number_of_regions):
            polygons = []
            number_of_boundaries = int(tokens[index])
            index += 1

            for _ in range(number_of_boundaries):
                number_of_curves = int(tokens[index])
                index += 1

                x_monotone_curves = []
                for _ in range(number_of_curves):
                    curve, index = read_bezier_curve(tokens, index, double_format)

                    if curve.number_of_control_points() >= 2:
                        for obj in make_x_monotone(curve):
                            if type(obj) is bezier_x_monotone_curve:
                                x_monotone_curves.append(obj)

                pgn = polygon(x_monotone_curves)
                orientation = pgn.orientation()
                print(f"  Orientation: {orientation_to_int(orientation)}")

                if (
                    (not polygons and orientation == Result.NEGATIVE)
                    or (polygons and orientation == Result.POSITIVE)
                ):
                    print("    Reversing orientation: ")
                    pgn.reverse_orientation()

                polygons.append(pgn)

            pwh = polygon_with_holes(polygons[0])
            for hole in polygons[1:]:
                pwh.add_hole(hole)

            print(
                "  Inserting Bezier polygon with holes comprising "
                f"{len(polygons)} boundaries in total into set."
            )
            result_set.join(pwh)
            print("    Done.")

        return result_set

    tic = time.perf_counter()
    S1 = read_bezier_polygon_set(filename1)
    S2 = read_bezier_polygon_set(filename2)
    toc = time.perf_counter()

    print(f"Constructed the input polygons in {toc - tic:0.6f} seconds.")
    print()

    print("Starting boolean operation...")
    tic = time.perf_counter()

    if bop and bop[0] == "i":
        S1.intersection(S2)
        operation = "intersection"
    else:
        S1.join(S2)
        operation = "union"

    toc = time.perf_counter()
    print(f"The {operation} computation took {toc - tic:0.6f} seconds.")


if __name__ == "__main__":
    main()
