// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_INTERSECT_2_CALL_OPERATOR_HPP
#define CGALPY_INTERSECT_2_CALL_OPERATOR_HPP

#include <variant>

#include <nanobind/nanobind.h>

namespace py = nanobind;

//
template <typename T>
py::list intersect_2_call_operator(typename T::Intersect_2& intersect,
                               typename T::X_monotone_curve_2& xc1,
                               typename T::X_monotone_curve_2& xc2) {
  py::list res;
  using Point_2 = typename T::Point_2;
  using X_monotone_curve_2 = typename T::X_monotone_curve_2;
  using Multiplicity = typename T::Multiplicity;
  using pair = std::pair<Point_2, Multiplicity>;

  auto v = std::vector<std::variant<pair, X_monotone_curve_2>>();
  intersect(xc1, xc2, std::back_inserter(v));
  for (auto o : v) {
    if (pair* pa = std::get_if<pair>(&o)) {
      py::tuple tup = py::make_tuple(pa->first, pa->second);
      res.append(tup);
    }
    else if (X_monotone_curve_2* curve = std::get_if<X_monotone_curve_2>(&o)){
      res.append(*curve);
    }
  }
  return res;
}

#endif
