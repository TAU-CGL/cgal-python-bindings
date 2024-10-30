#ifndef CGALPY_EXPORT_LINE_3_HPP
#define CGALPY_EXPORT_LINE_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Line_3.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

template <typename Kernel_, typename C>
void export_line_3(C &c) {
  using Ker = Kernel_;
  using Pnt = typename Ker::Point_3;
  using Dir = typename Ker::Direction_3;
  using Vec = typename Ker::Vector_3;
  using Lin = typename Ker::Line_3;
  using Seg = typename Ker::Segment_3;
  using Ray = typename Ker::Ray_3;
  using Aff = typename Ker::Aff_transformation_3;
  using FT = typename Ker::FT;

  c.def(py::init<const Pnt&, const Pnt&>(),
        py::arg("p"), py::arg("q"), "introduces a line l passing through the points p and q.")
    .def(py::init<const Pnt&, const Dir&>(),
        py::arg("p"), py::arg("d"), "introduces a line l passing through point p with direction d.")
    .def(py::init<const Pnt&, const Vec&>(),
        py::arg("p"), py::arg("v"), "introduces a line l passing through point p and oriented by v.")
    .def(py::init<const Seg&>(),
        py::arg("s"), "returns the line supporting the segment s, oriented from source to target.")
    .def(py::init<const Ray&>(),
        py::arg("r"), "returns the line supporting the ray r, with the same orientation.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"), "Test for equality: two lines are equal, iff they have a non empty intersection and the same direction.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"), "Test for inequality.")
    .def("projection", &Lin::projection, py::arg("p"), "returns the orthogonal projection of p on l.")
    // .def("point", &Lin::point, py::arg("i"), "returns an arbitrary point on l.") // doesn't work for some reason
    .def("is_degenerate", &Lin::is_degenerate, "returns true iff line l is degenerated to a point.")
    .def("has_on", &Lin::has_on, py::arg("p"), "returns true iff p lies on l.")
    .def("perpendicular_plane", &Lin::perpendicular_plane, py::arg("p"), "returns the plane perpendicular to l passing through p.")
    .def("opposite", &Lin::opposite, "returns the line with opposite direction.")
    .def("to_vector", &Lin::to_vector, "returns a vector having the same direction as l.")
    .def("direction", &Lin::direction, "returns the direction of l.")
    .def("transform", &Lin::transform, py::arg("t"), "returns the line obtained by applying t on a point on l and the direction of l.")
    .def("__str__", &to_string<Lin>)
    .def("__repr__", &to_string<Lin>)
    ;

  add_extraction(c);
}


#endif
