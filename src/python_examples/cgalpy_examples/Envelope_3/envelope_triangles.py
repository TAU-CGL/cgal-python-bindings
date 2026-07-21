#!/usr/bin/python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

from fractions import Fraction
import importlib
import sys


def format_ft(value):
    text = str(value).replace("-0", "0")
    try:
        number = Fraction(text)
    except ValueError:
        return text

    if number.denominator == 1:
        return str(number.numerator)
    return f"{number.numerator}/{number.denominator}"


def format_point(point):
    return f"{format_ft(point.x())} {format_ft(point.y())}"


def label_text(surface):
    data = surface.data()
    if isinstance(data, bytes):
        return data.decode()
    return str(data)


def labels_suffix(obj):
    return "".join(f" {label_text(surface)}" for surface in obj.surfaces())


def print_face(face):
    if face.is_unbounded():
        print(f"[Unbounded face]-->  {face.number_of_surfaces()} triangles:{labels_suffix(face)}")
        return

    line = "[Face]  "
    for halfedge in face.outer_ccb():
        line += f"({format_point(halfedge.target().point())})  "

    line += f"-->  {face.number_of_surfaces()} triangles:{labels_suffix(face)}"
    print(line)


def print_edge(edge):
    print(
        f"[Edge]  ({format_point(edge.source().point())}) "
        f"({format_point(edge.target().point())}) "
        f"-->  {edge.number_of_surfaces()} triangles:{labels_suffix(edge)}"
    )


def print_diagram(diagram):
    for face in diagram.faces():
        print_face(face)

    for edge in diagram.edges():
        print_edge(edge)


def main():
    lib = "CGALPY" if len(sys.argv) < 2 else sys.argv[1]

    CGALPY = importlib.import_module(lib)
    Ker = CGALPY.Ker
    Aos2 = CGALPY.Aos2
    Env3 = CGALPY.Env3

    Point = Ker.Point_3
    Base_surface = Aos2.Env_triangle_traits_3.Surface_3
    Data_surface = Aos2.Env_surface_data_traits_3.Surface_3

    t1 = Base_surface(Point(0, 0, 0), Point(0, 6, 0), Point(5, 3, 4))
    t2 = Base_surface(Point(6, 0, 0), Point(6, 6, 0), Point(1, 3, 4))
    triangles = [Data_surface(t1, "A"), Data_surface(t2, "B")]

    min_diagram = Env3.lower_envelope_3(triangles)
    print()
    print("The minimization diagram:")
    print_diagram(min_diagram)

    max_diagram = Env3.upper_envelope_3(triangles)
    print()
    print("The maximization diagram:")
    print_diagram(max_diagram)


if __name__ == "__main__":
    main()
