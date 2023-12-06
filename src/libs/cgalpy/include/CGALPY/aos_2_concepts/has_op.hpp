// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HAS_OP_HPP
#define CGALPY_HAS_OP_HPP

struct Cmp_result_pnt_pnt {};

template <typename T, typename Gt>
struct has_op {
  using Pnt = typename Gt::Point_2;

  // operator-has-correct-sig :)
  template<typename A>
  static Cmp_result_pnt_pnt
  test(CGAL::Comparison_result(A::*)(const Pnt&, const Pnt&) const)
  { return Cmp_result_pnt_pnt(); }

  // operator-exists :)
  template <typename A>
  static decltype(test(&A::operator()))
  test(decltype(&A::operator()), void*) {
    // Operator exists. What about sig?
    using return_type = decltype(test(&A::operator()));
    return return_type();
  }

  // game over :(
  template<typename A>
  static std::false_type test(...) { return std::false_type(); }

  // This will be either `std::true_type` or `std::false_type`
  using type = decltype(test<T>(0, 0));
};

#endif
