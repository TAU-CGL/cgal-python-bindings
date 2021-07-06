// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_2_TYPES_HPP
#define CGALPY_ALPHA_SHAPE_2_TYPES_HPP

#include <CGAL/tags.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/triangulation_2_types.hpp"

namespace as2 {

typedef Exact_comparison<exact_comparison()>::type              Ec;

}

#endif
