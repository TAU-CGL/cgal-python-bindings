// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#ifndef CGALPY_EXPORT_FT_HPP
#define CGALPY_EXPORT_FT_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename FT_>
const typename FT_::Exact_type& ft_exact(const FT_& ft)
{ return ft.exact(); }

template <typename FT_>
const typename FT_::Approximate_type& ft_approx(const FT_& ft)
{ return ft.approx(); }

template <typename FT_>
double ft_to_double(const FT_& ft)
{ return CGAL::to_double(ft); }

template <typename FT_, typename C>
void export_ft(C c) {
  c.def(py::init<double>())
    .def(py::init<typename FT_::Exact_type>())
    .def(py::init<FT_>())
    .def("exact", &ft_exact<FT_>, py::return_internal_reference<>())
    //.def("approx", &ft__approx<FT_>, py::return_internal_reference<>())
    .def("to_double", &ft_to_double<FT_>)
    .def(py::self_ns::str(py::self_ns::self))
    .def(py::self_ns::repr(py::self_ns::self))
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(py::self *= py::self)
    .def(py::self / py::self)
    .def(py::self /= py::self)
    .def(-py::self)
    ;
}

#endif
