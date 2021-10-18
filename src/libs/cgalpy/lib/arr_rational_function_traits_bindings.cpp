// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace bp = boost::python;

void export_arr_rational_function_traits() {
  typedef aos2::Geometry_traits_2       GT;
  bp::scope traits_scope = bp::class_<GT>("Traits")
    .def(bp::init<>())
    ;
}
