// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_2_CONFIG_HPP
#define CGALPY_ALPHA_SHAPE_2_CONFIG_HPP

#include <CGAL/tags.h>

namespace cgalpy {
namespace as2 {

constexpr bool exact_comparison()
{ return DETECT_EXIST(CGALPY_AS2_EXACT_COMPARISON); }

// Exact comparison
template <bool b> struct Exact_comparison {};
template <> struct Exact_comparison<false> { using type = CGAL::Tag_false; };
template <> struct Exact_comparison<true> { using type = CGAL::Tag_true; };

}
} // namespace cgalpy

#endif
