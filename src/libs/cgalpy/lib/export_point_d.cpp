// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <list>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace kerd {

// #if CGALPY_KERNEL_D_DIMENSION_TAG == CGALPY_KERNEL_D_DIMENSION_TAG_STATIC

// void init_point_d(Point_d* pd, py::list& lst) {
//   auto begin = stl_forward_iterator<FT_d>(lst);
//   auto end = stl_forward_iterator<FT_d>(lst, false);
// #if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
//      (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
//   new (pd) Point_d(begin, end);              // placement new
// #else
//   // Workaround a bug in CGAL
//   std::list<FT_d> tmp(begin, end);
//   new (pd) Point_d(tmp.begin(), tmp.end());  // placement new
// #endif
// }

// #else

// void init_point_d(Point_d* pd, int d, py::list& lst) {
//   auto begin = stl_forward_iterator<FT_d>(lst);
//   auto end = stl_forward_iterator<FT_d>(lst, false);
// #if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
//      (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
//   new (pd) Point_d(d, begin, end);              // placement new
// #else
//   // Workaround a bug in CGAL
//   std::list<FT_d> tmp(begin, end);
//   new (pd) Point_d(d, tmp.begin(), tmp.end());  // placement new
// #endif
// }

// #endif

//!
void init_point_d(Point_d* pd, int d, py::list& lst) {
  auto begin = stl_forward_iterator<FT_d>(lst);
  auto end = stl_forward_iterator<FT_d>(lst, false);
  new (pd) Point_d(d, begin, end);              // placement new
}

}

//!
void export_point_d(py::class_<Point_d>& pntd_c) {
  using Kerd = Kernel_d;
  using Pntd = Point_d;

  pntd_c.def(py::init<>())
    .def("__init__", &kerd::init_point_d)
    .def("__hash__", &hash_rational_point<is_epec_d_type(), Pntd>)
    .def("dimension", &Pntd::dimension)
    .def("cartesian", &Pntd::cartesian)
    .def("__getitem__", &Pntd::operator[])
    .def(py::self == py::self)
    .def(py::self != py::self)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
#endif
    ;

  using Cci = Kerd::Cartesian_const_iterator_d;
  add_iterator<Cci, Cci, const FT_d&>("Cartesian_iterator", pntd_c);
  pntd_c.def("cartesians",
             [](const Pntd& p)
             { return make_iterator(p.cartesian_begin(), p.cartesian_end()); },
             py::keep_alive<0, 1>());

  add_insertion(pntd_c, "__str__");
  add_insertion(pntd_c, "__repr__");
}
