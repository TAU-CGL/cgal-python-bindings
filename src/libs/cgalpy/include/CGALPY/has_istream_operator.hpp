// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HAS_ISTREAM_OPERATOR_HPP
#define CGALPY_HAS_ISTREAM_OPERATOR_HPP

#include <iostream>
#include <type_traits>

template <typename Class>
struct has_istream_operator {
  template <typename V>
  static auto test(V*) ->
    decltype(std::declval<std::istream>() >> std::declval<V>());

  template<typename> static auto test(...) -> std::false_type;

  using type =
    typename std::is_same<std::istream&, decltype(test<Class>(0))>::type;
};

#endif
