#ifndef CGALPY_EXPORT_CIRCLE_3_HPP
#define CGALPY_EXPORT_CIRCLE_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

// Export a three-dimensional circle.
template <typename Kernel, typename C>
void export_circle_3(C& c) {
  using Circle_3 = typename Kernel::Circle_3;
  using Pnt_3 = typename Kernel::Point_3;
  using Sphere_3 = typename Kernel::Sphere_3;
  using Vec_3 = typename Kernel::Vector_3;
  using Pln_3 = typename Kernel::Plane_3;
  using FT = typename Kernel::FT;

  c.def(py::init<const Pnt_3&, const FT&, const Pln_3&>(),
        py::arg("center"), py::arg("sq_r"), py::arg("plane"),
        "introduces a variable c of type Circle_3.")
    .def(py::init<const Pnt_3&, const FT&, const Vec_3&>(),
         py::arg("center"), py::arg("sq_r"), py::arg("n"),
         "introduces a variable c of type Circle_3.")
    .def(py::init<const Pnt_3&, const Pnt_3&, const Pnt_3&>(),
         py::arg("p"), py::arg("q"), py::arg("r"),
         "introduces a variable c of type Circle_3.")
    .def(py::init<const Sphere_3&, const Sphere_3&>(),
         py::arg("sphere1"), py::arg("sphere2"),
         "introduces a variable c of type Circle_3.")
    .def(py::init<const Sphere_3&, const Pln_3&>(),
         py::arg("sphere"), py::arg("plane"),
         "introduces a variable c of type Circle_3.")
    .def(py::init<const Pln_3&, const Sphere_3&>(),
         py::arg("plane"), py::arg("sphere"),
         "introduces a variable c of type Circle_3.")
    .def("center", &Circle_3::center,
        "returns the center of c.")
    .def("squared_radius", &Circle_3::squared_radius,
        "returns the squared radius of c.")
    .def("supporting_plane", &Circle_3::supporting_plane,
        "returns the supporting plane of c.")
    .def("diametral_sphere", &Circle_3::diametral_sphere,
        "returns the diametral sphere of c.")
    .def("area_divided_by_pi", &Circle_3::area_divided_by_pi,
        "returns the area of c, divided by π.")
    .def("approximate_area", &Circle_3::approximate_area,
        "returns an approximation of the area of c.")
    .def("squared_length_divided_by_pi_square", &Circle_3::squared_length_divided_by_pi_square,
        "returns the squared length of c, divided by π^2.")
    .def("approximate_squared_length", &Circle_3::approximate_squared_length,
        "returns an approximation of the squared length (i.e. perimeter) of c.")
    .def("has_on", &Circle_3::has_on,
         py::arg("p"))
    .def("bbox", &Circle_3::bbox,
        "returns a bounding box containing c.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"),
         "returns true, iff c1 and c2 are equal, i.e. if they have the same center, the same squared radius and the same supporting plane.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    ;
}

#endif
