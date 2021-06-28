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

typedef bp::return_value_policy<bp::copy_const_reference> Copy_const_reference;
typedef bp::return_value_policy<bp::return_by_value>      Return_by_value;

#endif
