// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_3_CONFIG_HPP
#define CGALPY_ALPHA_SHAPE_3_CONFIG_HPP

#include <CGAL/tags.h>
#include <CGAL/Alpha_shape_3.h>
#include <CGAL/Fixed_alpha_shape_3.h>

#include "CGALPY/config.hpp"

#define CGALPY_AS3_PLAIN                                0
#define CGALPY_AS3_FIXED                                1

#include <CGAL/tags.h>

namespace as3 {

constexpr bool exact_comparison()
{ return DETECT_EXIST(CGALPY_AS3_EXACT_COMPARISON); }

// Exact comparison
template <bool b> struct Exact_comparison {};
template <> struct Exact_comparison<false> { typedef CGAL::Tag_false type; };
template <> struct Exact_comparison<true> { typedef CGAL::Tag_true type; };

// Alpha shape types
template <int i, typename Tr, typename Ec> struct Alpha_shape {};
template <typename Tr, typename Ec> struct Alpha_shape<CGALPY_AS3_PLAIN, Tr, Ec>
{ typedef CGAL::Alpha_shape_3<Tr, Ec> type; };
template <typename Tr, typename Ec> struct Alpha_shape<CGALPY_AS3_FIXED, Tr, Ec>
{ typedef CGAL::Fixed_alpha_shape_3<Tr> type; };

}

#endif //CGALPY_ALPHA_SHAPE_3_CONFIG_HPP
