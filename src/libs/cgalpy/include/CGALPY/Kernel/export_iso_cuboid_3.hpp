#ifndef CGALPY_EXPORT_ISO_CUBOID_3_HPP
#define CGALPY_EXPORT_ISO_CUBOID_3_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Iso_cuboid_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

// Export a 3D iso-cuboid
template <typename Kernel_, typename C>
void export_iso_cuboid_3(C& c) {
  using Ker = Kernel_;
  using Pt = typename Ker::Point_3;
  using Rt = typename Ker::RT;
  using Ic = typename Ker::Iso_cuboid_3;

  c.def(py::init<const Pt&, const Pt&>(), py::arg("p"), py::arg("q"))
    .def(py::init<const Pt&, const Pt&, int>())
    .def(py::init<const Pt&, const Pt&, const Pt&,
                  const Pt&, const Pt&, const Pt&>(),
         py::arg("left"), py::arg("right"), py::arg("bottom"), py::arg("top"),
         py::arg("far"), py::arg("close"))
    .def(py::init<const Rt&, const Rt&, const Rt&,
                  const Rt&, const Rt&, const Rt&, const Rt&>(),
         py::arg("min_hx"), py::arg("min_hy"), py::arg("min_hz"),
         py::arg("max_hx"), py::arg("max_hy"), py::arg("max_hz"),
         py::arg("hw") = Rt(1))
    .def(py::init<const CGAL::Bbox_3&>(), py::arg("bbox"))
    .def("bbox", &Ic::bbox)
    .def("max", &Ic::max)
    .def("max_coord", &Ic::max_coord, py::arg("i"))
    .def("min", &Ic::min)
    .def("min_coord", &Ic::min_coord, py::arg("i"))
    .def("__getitem__", &Ic::operator[], py::arg("i"))
    .def("transform", &Ic::transform, py::arg("t"))
    .def("vertex", &Ic::vertex, py::arg("i"))
    .def("xmax", &Ic::xmax)
    .def("xmin", &Ic::xmin)
    .def("ymax", &Ic::ymax)
    .def("ymin", &Ic::ymin)
    .def("zmax", &Ic::zmax)
    .def("zmin", &Ic::zmin)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
