// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_VISIBILITY_2_CONFIG_HPP
#define CGALPY_VISIBILITY_2_CONFIG_HPP

#include <CGAL/tags.h>

#include "CGALPY/config.hpp"

namespace vis2 {

constexpr bool regularization_category()
{ return DETECT_EXIST(CGALPY_VIS2_REGULARIZATION_CATEGORY); }

// Exact comparison
template <bool b> struct Regularization_category {};
template <> struct Regularization_category<false> { typedef CGAL::Tag_false type; };
template <> struct Regularization_category<true> { typedef CGAL::Tag_true type; };

}

#endif
