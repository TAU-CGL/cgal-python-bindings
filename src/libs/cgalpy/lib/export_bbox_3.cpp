#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include <CGAL/Bbox_3.h>
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

//
void export_bbox_3(py::class_<CGAL::Bbox_3>& c) {
  c.def(py::init<>())
    .def(py::init<double, double, double, double, double, double>())
    .def("dimension", &CGAL::Bbox_3::dimension)
    .def("dilate", &CGAL::Bbox_3::dilate)
    .def("xmin", &CGAL::Bbox_3::xmin)
    .def("ymin", &CGAL::Bbox_3::ymin)
    .def("zmin", &CGAL::Bbox_3::zmin)
    .def("xmax", &CGAL::Bbox_3::xmax)
    .def("ymax", &CGAL::Bbox_3::ymax)
    .def("zmax", &CGAL::Bbox_3::zmax)
    .def("min", &CGAL::Bbox_3::min)
    .def("max", &CGAL::Bbox_3::max)
    .def("__str__", to_string<CGAL::Bbox_3>)
    .def("__repr__", to_string<CGAL::Bbox_3>)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(py::self += py::self)
    .def(py::self + py::self)
    ;
}

