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

void export_arr_Bezier_traits() {
  bp::scope traits_scope = bp::class_<Traits>("Traits")
    .def(bp::init<>())
    ;
}
