#ifndef CGALPY_EXPORT_RAY_3_HPP
#define CGALPY_EXPORT_RAY_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Ray_3.h>

#include "cgalpy/to_string.hpp"

namespace py = nanobind;

template <typename Kernel_, typename C>
void export_ray_3(C &c) {
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
        py::arg("p"), py::arg("q"), "introduces a ray r with source p and passing through point q.")
    .def(py::init<const Pnt&, const Dir&>(),
         py::arg("p"), py::arg("d"), "returns a point on r.\npoint(0) is the source. point(i), with i>0, is different from the source.")
    .def(py::init<const Pnt&, const Vec&>(),
         py::arg("p"), py::arg("v"), "returns the source of r")
    .def(py::init<const Pnt&, const Lin&>(),
         py::arg("p"), py::arg("l"), "introduces a ray r starting at source p with the same direction as l.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"), "Test for equality: two rays are equal, iff they have the same source and the same direction.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"), "Test for inequality.")
    .def("source", &Ray::source, "returns the source of r")
    .def("point", &Ray::point,
         py::arg("i"), "returns a point on r.\npoint(0) is the source. point(i), with i>0, is different from the source.")
    .def("direction", &Ray::direction, "returns the direction of r.")
    .def("to_vector", &Ray::to_vector, "returns a vector giving the direction of r.")
    .def("supporting_line", &Ray::supporting_line, "returns the line supporting r which has the same direction.")
    .def("opposite", &Ray::opposite, "returns the ray with the same source and the opposite direction.")
    .def("is_degenerate", &Ray::is_degenerate, "ray r is degenerate, if the source and the second defining point fall together (that is if the direction is degenerate).")
    .def("has_on", &Ray::has_on, "A point is on r, iff it is equal to the source of r, or if it is in the interior of r.")
    .def("transform", &Ray::transform,
         py::arg("t"), "returns the ray obtained by applying t on the source and on the direction of r.")
    .def("__str__", to_string<Ray>)
    .def("__repr__", to_string<Ray>)
    ;

  add_extraction(c);
}


#endif
