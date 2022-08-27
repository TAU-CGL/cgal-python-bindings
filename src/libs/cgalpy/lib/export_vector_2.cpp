// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"

namespace bp = nanobind;

void export_vector_2(py::module_& m) {
  bp::class_<Vector_2> vector_c(m, "Vector_2");
  vector_c.def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2>())
    .def(bp::init<Ray_2>())
    .def(bp::init<Segment_2>())
    .def(bp::init<FT&, FT&, FT&>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<double, double>())
    .def("hx", &Vector_2::hx)
    .def("hy", &Vector_2::hy)
    .def("hw", &Vector_2::hw)
    .def("x", &Vector_2::x)
    .def("y", &Vector_2::y)
    .def("squared_length", &Vector_2::squared_length)
    .def("homogeneous", &Vector_2::homogeneous)
    .def("cartesian", &Vector_2::cartesian)
    .def("__getitem__", &Vector_2::operator[])
    .def("dimension", &Vector_2::dimension)
    .def("direction", &Vector_2::direction)
    .def("transform", &Vector_2::transform)
    .def("perpendicular", &Vector_2::perpendicular)
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(-bp::self)
    .def(bp::self * bp::self)
    .def(bp::self * FT())
    .def(FT() * bp::self)
    .def(bp::self *= FT())
    .def(bp::self / FT())
    .def(bp::self /= FT())
    //.def(py::self * RT())
    //.def(RT() * bp::self)
    //.def(bp::self *= RT())
    //.def(bp::self / RT())
    //.def(bp::self /= RT())
    //.setattr("__hash__", &hash<Vector_2>)
    ;

#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPIC) ||                           \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE))
  using Cci = Kernel::Cartesian_const_iterator_2;
  add_iterator<Cci, Cci, const FT&>("Cartesian_iterator", vector_c);
  vector_c.def("cartesians",
               [] (const Vector_2& v)
               { return make_iterator(v.cartesian_begin(), v.cartesian_end()); },
               py::keep_alive<0, 1>());
#endif

  add_insertion(vector_c, "__str__");
  add_insertion(vector_c, "__repr__");
}
