// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_MAKE_X_MONOTONE_2_CALL_OPERATOR_HPP
#define CGALPY_MAKE_X_MONOTONE_2_CALL_OPERATOR_HPP

#include <boost/variant.hpp>
#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>

namespace py = nanobind;

//! Apply the make_x_monotone operator and append the resulting X-monotone
// elements to the returned Python list.
template <typename T>
py::list make_x_monotone_2_call_operator(typename T::Make_x_monotone_2 m,
                                         typename T::Curve_2& c) {
  using X_monotone_curve_2 = typename T::X_monotone_curve_2;
  using Point_2 = typename T::Point_2;
  using Result = boost::variant<Point_2, X_monotone_curve_2>;

  py::list lst;
  auto op =
    [&] (const Result& o) mutable {
      if (auto* point = boost::get<Point_2>(&o)) lst.append(*point);
      else if (auto* cv = boost::get<X_monotone_curve_2>(&o)) lst.append(*cv);
    };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  m(c, it);
  return lst;
}

#endif
