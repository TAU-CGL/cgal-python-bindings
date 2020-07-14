// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYGON_2_TYPES_HPP
#define CGALPY_POLYGON_2_TYPES_HPP

#include <CGALPY/polygon_2_config.hpp>
#include <CGALPY/kernel_types.hpp>

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

typedef typename CGAL::Polygon_2<Kernel, Point_2_container>             Polygon_2;
typedef typename CGAL::Polygon_with_holes_2<Kernel, Point_2_container>  Polygon_with_holes_2;
typedef typename CGAL::Polygon_set_2<Kernel, Point_2_container>         Polygon_set_2;

#endif //CGALPY_POLYGON_2_TYPES_HPP