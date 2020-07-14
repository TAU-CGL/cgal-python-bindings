// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_3_EXACT_COMPARISON_HPP
#define CGALPY_ALPHA_SHAPE_3_EXACT_COMPARISON_HPP

#include <CGAL/basic.h>

namespace as3 {

#if CGALPY_AS3_EXACT_COMPARISON == 0
typedef CGAL::Tag_false         Exact_comparison;
#elif CGALPY_AS3_EXACT_COMPARISON == 1
typedef CGAL::Tag_true          Exact_comparison;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AS3_EXACT_COMPARISON");
#endif

}

#endif
