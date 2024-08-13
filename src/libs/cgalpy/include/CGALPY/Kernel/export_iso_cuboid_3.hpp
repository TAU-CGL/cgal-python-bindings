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
  using Ker = Kernel; // kernel from Kernel, not Kernel_ because Kernel_ defined the cartesian kernel
  using Rt = typename Ker::RT;
  using Iso_cuboid_3 = typename Ker::Iso_cuboid_3;

  c.def(py::init<const Point_3&, const Point_3&>(),
        py::arg("p"), py::arg("q"))
    .def(py::init<const Point_3&, const Point_3&, int>())
    .def(py::init<const Point_3&, const Point_3&, const Point_3&, const Point_3&, const Point_3&, const Point_3&>(),
         py::arg("left"), py::arg("right"), py::arg("bottom"), py::arg("top"), py::arg("far"), py::arg("close"))
    .def(py::init<const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&>(),
         py::arg("min_hx"), py::arg("min_hy"), py::arg("min_hz"), py::arg("max_hx"), py::arg("max_hy"), py::arg("max_hz"), py::arg("hw") = Rt(1))
    .def(py::init<const CGAL::Bbox_3&>(),
         py::arg("bbox"))
    .def("bbox", &Iso_cuboid_3::bbox)
    .def("max", &Iso_cuboid_3::max)
    .def("max_coord", &Iso_cuboid_3::max_coord,
         py::arg("i"))
    .def("min", &Iso_cuboid_3::min)
    .def("min_coord", &Iso_cuboid_3::min_coord,
         py::arg("i"))
    .def("__getitem__", &Iso_cuboid_3::operator[],
         py::arg("i"))
    .def("transform", &Iso_cuboid_3::transform,
         py::arg("t"))
    .def("vertex", &Iso_cuboid_3::vertex,
         py::arg("i"))
    .def("xmax", &Iso_cuboid_3::xmax)
    .def("xmin", &Iso_cuboid_3::xmin)
    .def("ymax", &Iso_cuboid_3::ymax)
    .def("ymin", &Iso_cuboid_3::ymin)
    .def("zmax", &Iso_cuboid_3::zmax)
    .def("zmin", &Iso_cuboid_3::zmin)
    .def("__str__", to_string<Iso_cuboid_3>)
    .def("__repr__", to_string<Iso_cuboid_3>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif

