// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/bind_polynomial.hpp"

namespace py = nanobind;

template<typename T>
T ipower(T& p, int i) { return CGAL::ipower(p, i); }

template<typename T>
T shift(T& p, int i) { return CGAL::shift(p, i); }

//
py::object export_arr_rational_function_traits(py::module_& m) {
  using GT = aos2::Geometry_traits_2;

  py::class_<GT> traits_c(m, "Arr_rational_function_traits_2");
  traits_c.def(py::init<>())
    ;

  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
  };
  Concepts concepts;
  export_AosTraits_2<GT>(traits_c, concepts);

  bind_polynomial<aos2::PT_1>(m, "Polynomial_1");
  m.def("shift", &shift<aos2::Polynomial_1>);
  m.def("ipower", &ipower<aos2::Polynomial_1>);

  return traits_c;
}
