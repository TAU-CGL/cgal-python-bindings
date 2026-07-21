#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Aos2 = CGALPY.Aos2
    Ss2 = CGALPY.Ss2

    Point = Ker.Point_2
    Segment = Aos2.Arr_segment_traits_2.Curve_2

    segments = [
        Segment(Point(1, 5), Point(8, 5)),
        Segment(Point(1, 1), Point(8, 8)),
        Segment(Point(3, 1), Point(3, 8)),
        Segment(Point(8, 5), Point(8, 8)),
    ]

    points = Ss2.compute_intersection_points(segments)

    print(f"Found {len(points)} intersection points: ")
    for point in points:
        print(point)

    sub_segments = Ss2.compute_subcurves(segments)
    print(f"Found {len(sub_segments)} interior-disjoint sub-segments.")

    assert Ss2.do_intersect(segments, False)


if __name__ == "__main__":
    main()
