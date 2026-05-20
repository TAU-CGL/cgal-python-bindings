#ifndef CGALPY_EXPORT_SEGMENT_3
#define CGALPY_EXPORT_SEGMENT_3

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Segment_3.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

template <typename Kernel_, typename C>
void export_segment_3(C &c) {
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
        py::arg("p"), py::arg("q"), "introduces a segment s with source p and target q.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"), "Test for equality: Two segments are equal, iff their sources and targets are equal.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"), "Test for inequality.")
    .def("source", &Seg::source, doc::Segment_3_source)    .def("target", &Seg::target, doc::Segment_3_target)    .def("min", &Seg::min, doc::Segment_3_min)    .def("max", &Seg::max, doc::Segment_3_max)    .def("vertex", &Seg::vertex,         py::arg("i"), "returns source or target of s: vertex(0) returns the source, vertex(1) returns the target.")
    .def("point", &Seg::point,         py::arg("i"), "returns vertex(i).")
    .def("__getitem__", &Seg::operator[],         py::arg("i"), "returns vertex(i).")
    .def("squared_length", &Seg::squared_length, doc::Segment_3_squared_length)    .def("to_vector", &Seg::to_vector, doc::Segment_3_to_vector)    .def("direction", &Seg::direction, doc::Segment_3_direction)    .def("opposite", &Seg::opposite, doc::Segment_3_opposite)    .def("supporting_line", &Seg::supporting_line, doc::Segment_3_supporting_line)    .def("is_degenerate", &Seg::is_degenerate, doc::Segment_3_is_degenerate)    .def("has_on", &Seg::has_on, doc::Segment_3_has_on)    .def("bbox", &Seg::bbox, doc::Segment_3_bbox)    .def("transform", &Seg::transform,         py::arg("t"), "returns the segment obtained by applying t on the source and the target of s.")
    .def("__str__", to_string<Seg>)
    .def("__repr__", to_string<Seg>)
    ;

  add_extraction(c);
}

#endif

