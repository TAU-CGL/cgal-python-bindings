// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_CONFIG_HPP
#define CGALPY_CONFIG_HPP

#define EXPAND( x ) x

#define STR(s) #s
#define XSTR(s) STR(s)

// Define two genetic macros
#define SECOND_ARG(A,B,...) B
#define CONCAT2(A,B) A ## B

// If a macro is detected, add an arg, so the second one will be 1.
#define DETECT_EXIST_TRUE ~,1

// DETECT_EXIST merely concats a converted macro to the end of DETECT_EXIST_TRUE.
// If empty, DETECT_EXIST_TRUE converts fine.  If not 0 remains second argument.
#define DETECT_EXIST_IMPL(...) EXPAND(SECOND_ARG(__VA_ARGS__))
#define DETECT_EXIST(X) DETECT_EXIST_IMPL(CONCAT2(DETECT_EXIST_TRUE,X), 0, ~)

#endif //CONFIG_HPP
