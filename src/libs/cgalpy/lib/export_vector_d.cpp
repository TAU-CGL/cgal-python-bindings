// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "cgalpy/Kerd_docstrings.hpp"

namespace kerd_doc = cgalpy::kerd::docstrings;

namespace cgalpy {
namespace kerd {

//!
void init_vector_d(Vector_d* pd, int d, py::list& lst) {
  auto begin = stl_forward_iterator<FT_d>(lst);
  auto end = stl_forward_iterator<FT_d>(lst, false);
  new (pd) Vector_d(d, begin, end);              // placement new
}

}
} // namespace cgalpy

//!
void export_vector_d(py::class_<Vector_d>& vecd_c) {
  using Kerd = Kernel_d;
  using Vecd = Vector_d;

  vecd_c.def(py::init<>(), kerd_doc::Vector_d_Vector_d)
    .def("__init__", &cgalpy::kerd::init_vector_d,
         py::arg("d"), py::arg("coordinates"),
         kerd_doc::Vector_d_Vector_d_2)
    .def("__hash__", &hash_rational_point<is_epec_d_type(), Vecd>)
    .def("dimension", &Vecd::dimension,
         kerd_doc::Vector_d_dimension)
    .def("cartesian", &Vecd::cartesian, py::arg("i"),
         kerd_doc::Vector_d_cartesian)
    .def("__getitem__", &Vecd::operator[], py::arg("i"),
         kerd_doc::Vector_d_operator_subscript)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D) || (CGAL_VERSION_NR > 1060200000)
    .def(py::self == py::self)
    .def(py::self != py::self)
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
             py::keep_alive<0, 1>(),
             "Iterate over the Cartesian coordinates of the vector.");

  add_insertion(vecd_c, "__str__");
  add_insertion(vecd_c, "__repr__");
}
