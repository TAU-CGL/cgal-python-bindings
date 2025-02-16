// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_3_EXACT_COMPARISON_HPP
#define CGALPY_ALPHA_SHAPE_3_EXACT_COMPARISON_HPP

#include <CGAL/basic.h>

namespace as3 {

#if CGALPY_AS3_EXACT_COMPARISON == 0
using Exact_comparison = CGAL::Tag_false;
#elif CGALPY_AS3_EXACT_COMPARISON == 1
using Exact_comparison = CGAL::Tag_true;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AS3_EXACT_COMPARISON");
#endif

}

#endif
