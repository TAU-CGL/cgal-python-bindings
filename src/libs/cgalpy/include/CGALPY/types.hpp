// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TYPES_HPP
#define CGALPY_TYPES_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

// Define two genetic macros
#define SECOND_ARG(A,B,...) B
#define CONCAT2(A,B) A ## B

// If a macro is detected, add an arg, so the second one will be 1.
#define DETECT_EXIST_TRUE ~,1

// DETECT_EXIST merely concats a converted macro to the end of DETECT_EXIST_TRUE.
// If empty, DETECT_EXIST_TRUE converts fine.  If not 0 remains second argument.
#define DETECT_EXIST_IMPL(...) SECOND_ARG(__VA_ARGS__)
#define DETECT_EXIST(X) DETECT_EXIST_IMPL(CONCAT2(DETECT_EXIST_TRUE,X), 0, ~)

typedef bp::return_value_policy<bp::copy_const_reference> Copy_const_reference;
typedef bp::return_value_policy<bp::return_by_value>      Return_by_value;

#endif
