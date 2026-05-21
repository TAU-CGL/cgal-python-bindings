// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <cgalpy/Kernel_d_docstrings.hpp>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace doc = cgalpy::docstrings::Kernel_d;

namespace kerd {

//!
void init_vector_d(Vector_d* pd, int d, py::list& lst) {
  auto begin = stl_forward_iterator<FT_d>(lst);
  auto end = stl_forward_iterator<FT_d>(lst, false);
  new (pd) Vector_d(d, begin, end);              // placement new
}

}

//!
void export_vector_d(py::class_<Vector_d>& vecd_c) {
  vecd_c.doc() = doc::CGAL_Vector_d__class__;
  using Kerd = Kernel_d;
  using Vecd = Vector_d;

  vecd_c.def(py::init<>(), doc::CGAL_Vector_d_Vector_d)
    .def("__init__", &kerd::init_vector_d)
    .def("__hash__", &hash_rational_point<is_epec_d_type(), Vecd>)
    .def("dimension", &Vecd::dimension, doc::CGAL_Vector_d_dimension)
    .def("cartesian", &Vecd::cartesian, doc::CGAL_Vector_d_cartesian)
    .def("__getitem__", &Vecd::operator[], doc::CGAL_Vector_d_operator)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D) || (CGAL_VERSION_NR > 1060200000)
    .def(py::self == py::self, doc::CGAL_Vector_d_operator_1)
    .def(py::self != py::self, doc::CGAL_Vector_d_operator_2)
#endif
    // .def(py::self > py::self)
    // .def(py::self < py::self)
    // .def(py::self <= py::self)
    // .def(py::self >= py::self)
    ;

  using Cci = Kerd::Cartesian_const_iterator_d;

  // There might be a better (automatic) way to handle this instead of a hard-coded '#if'...
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
  add_iterator<Cci, Cci, const FT_d&>("Cartesian_iterator", vecd_c);
#else
  add_iterator<Cci, Cci, FT_d>("Cartesian_iterator", vecd_c);
#endif

  vecd_c.def("cartesians",
             [](const Vecd& v)
             { return make_iterator(v.cartesian_begin(), v.cartesian_end()); },
             py::keep_alive<0, 1>());

  add_insertion(vecd_c, "__str__");
  add_insertion(vecd_c, "__repr__");
}
