#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import importlib
import sys


def curve_data(curve):
    data = getattr(curve, "data")
    return data() if callable(data) else data


def print_curves(prefix, curves):
    labels = [str(curve_data(curve)) for curve in curves]
    if labels:
        print(prefix + " " + " ".join(labels))
    else:
        print(prefix)


def print_diagram(diag):
    edge = diag.leftmost()
    rightmost = diag.rightmost()

    while edge != rightmost:
        if edge.is_empty():
            print("Edge: [empty]")
        else:
            print_curves("Edge:", list(edge.curves()))

        vertex = edge.right()
        print_curves(f"Vertex ({vertex.point()}):", list(vertex.curves()))

        edge = vertex.right()

    assert edge.is_empty()
    print("Edge: [empty]")


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Aos2 = CGALPY.Aos2
    Env2 = CGALPY.Env2

    Point = Ker.Point_2
    Segment = Aos2.Arr_segment_traits_2.X_monotone_curve_2
    Labeled_segment = Aos2.Arr_curve_data_traits_2.X_monotone_curve_2

    raw_segments = [
        (Point(0, 1), Point(2, 3), "A"),
        (Point(1, 2), Point(4, 5), "B"),
        (Point(1, 5), Point(7, 2), "C"),
        (Point(4, 2), Point(6, 4), "D"),
        (Point(8, 3), Point(8, 6), "E"),
        (Point(9, 2), Point(12, 4), "F"),
        (Point(10, 2), Point(12, 1), "G"),
        (Point(11, 0), Point(11, 5), "H"),
    ]

    segments = [
        Labeled_segment(Segment(source, target), label)
        for source, target, label in raw_segments
    ]

    min_diag = Env2.lower_envelope_x_monotone_2(segments)
    print_diagram(min_diag)


if __name__ == "__main__":
    main()
