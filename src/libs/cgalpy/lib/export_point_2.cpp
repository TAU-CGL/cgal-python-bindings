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
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"

namespace bp = nanobind;

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_epec_type() {
  return ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||
          (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||
          (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ));
}

void export_point_2(py::module_& m) {
  bp::class_<Point_2> p_co(m, "Point_2");
  p_co.def(bp::init<>())
    .def(bp::init<double, double>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<RT&, RT&>())
    .def(bp::init<Point_2&>())
    .def("x", &Point_2::x)
    .def("y", &Point_2::y)
    .def("hx", &Point_2::hx)
    .def("hy", &Point_2::hy)
    .def("hw", &Point_2::hw)
    .def("bbox", &Point_2::bbox)
    .def("cartesian", &Point_2::cartesian)
    .def("__getitem__", &Point_2::operator[])
#if ((CGALPY_KERNEL == CGALPY_KERNEL_EPIC) ||                           \
     (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE))
    // TODO: Returning address of local variable or temporary with EPEC kernel
    .def("coordinates", bp::range<>(&Point_2::cartesian_begin, &Point_2::cartesian_end))
#endif
    .def("dimension", &Point_2::dimension)
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self - bp::self)
    .def(bp::self += Vector_2())
    .def(bp::self -= Vector_2())
    .def(bp::self + Vector_2())
    .def(bp::self - Vector_2())
    // .setattr("__hash__", &hash_rational_point<is_epec_type(), Point_2>) NB
    // .setattr("__doc__", "Point_2") NB
    ;

  add_insertion(p_co, "__str__");
  add_insertion(p_co, "__repr__");
}
