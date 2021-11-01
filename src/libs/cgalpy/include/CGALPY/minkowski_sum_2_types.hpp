// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_MINKOWSKI_SUM_2_TYPES_HPP
#define CGALPY_MINKOWSKI_SUM_2_TYPES_HPP

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

#include "CGALPY/kernel_types.hpp"

namespace ms2 {

typedef CGAL::Polygon_2<Kernel, Point_2_container>             Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel, Point_2_container>  Polygon_with_holes_2;

}

#endif
