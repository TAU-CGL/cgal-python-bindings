// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase
// a commercial license for CGAL.

#ifndef CGALPY_NANOBIND_OBJECT_EQUALITY_HPP
#define CGALPY_NANOBIND_OBJECT_EQUALITY_HPP

#include <nanobind/nanobind.h>

// CGAL's consolidated curve-data utilities compare stored data with
// unqualified operator== from templates defined in namespace CGAL.
// This overload must be visible before those CGAL templates are parsed.
namespace CGAL {

inline bool operator==(const nanobind::object& a, const nanobind::object& b)
{ return a.equal(b); }

} // namespace CGAL

#endif
