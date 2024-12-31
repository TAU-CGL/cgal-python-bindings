// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_GET_HPP
#define CGALPY_GET_HPP

#include <CGAL/boost/graph/properties.h>

#include <nanobind/nanobind.h>

namespace internal {

template <typename T1, typename T2>
auto get(const T1& p, const T2& t) { return get(p, t); }

} // namespace internal

#endif
