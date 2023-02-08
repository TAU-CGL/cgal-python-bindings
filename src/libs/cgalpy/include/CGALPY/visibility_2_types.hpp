// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_VISIBILITY_2_TYPES_HPP
#define CGALPY_VISIBILITY_2_TYPES_HPP

#include <CGAL/Simple_polygon_visibility_2.h>

#include "CGALPY/visibility_2_config.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace vis2 {
  typedef Regularization_category<regularization_category()>::type   Rc;
  typedef aos2::Arrangement_2                           Arrangement_2;
  typedef aos2::Point_2                                 Point_2;
  typedef aos2::Halfedge                                Halfedge;
  typedef aos2::Halfedge_const_handle                   Halfedge_const_handle;
  typedef aos2::Face                                    Face;
  typedef aos2::Face_const_handle                       Face_const_handle;
  typedef CGAL::Simple_polygon_visibility_2<Arrangement_2, Rc>
    Simple_polygon_visibility_2;
}

#endif
