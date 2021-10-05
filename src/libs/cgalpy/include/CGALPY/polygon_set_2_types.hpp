// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYGON_SET_2_TYPES_HPP
#define CGALPY_POLYGON_SET_2_TYPES_HPP

#include <CGAL/Polygon_set_2.h>

#include "CGALPY/polygon_set_2_config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace bso2 {

typedef typename aos2::Dcel                                           Dcel;
typedef typename CGAL::Polygon_set_2<Kernel, Point_2_container, Dcel>
  Polygon_set_2;
typedef typename Polygon_set_2::Polygon_with_holes_2
  Polygon_with_holes_2;
typedef typename Polygon_set_2::Polygon_2                             Polygon_2;

}

#endif
