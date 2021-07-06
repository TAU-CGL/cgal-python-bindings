// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_2_CONFIG_HPP
#define CGALPY_ALPHA_SHAPE_2_CONFIG_HPP

namespace as2 {

constexpr bool exact_comparison()
{ return DETECT_EXIST(CGALPY_AS2_EXACT_COMPARISON); }

// Exact comparison
template <bool b> struct Exact_comparison {};
template <> struct Exact_comparison<false> { typedef CGAL::Tag_false type; };
template <> struct Exact_comparison<true> { typedef CGAL::Tag_true type; };

}

#endif
