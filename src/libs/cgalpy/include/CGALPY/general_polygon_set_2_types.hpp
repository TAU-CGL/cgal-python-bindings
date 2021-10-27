// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_GENERAL_POLYGON_SET_2_TYPES_HPP
#define CGALPY_GENERAL_POLYGON_SET_2_TYPES_HPP

#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_config.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace bso2 {

typedef aos2::Dcel                                      Dcel;
typedef aos2::Traits_2                                  Traits_2;
typedef CGAL::General_polygon_set_2<Traits_2, Dcel>     General_polygon_set_2;
typedef General_polygon_set_2::Polygon_2                Polygon_2;
typedef General_polygon_set_2::Polygon_with_holes_2     Polygon_with_holes_2;
typedef General_polygon_set_2::Arrangement_2            Arrangement_2;
typedef Polygon_2::Curve_iterator                       Curve_iterator;

}

#endif
