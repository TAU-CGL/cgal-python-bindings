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
#include <CGAL/Triangular_expansion_visibility_2.h>

#include "CGALPY/visibility_2_config.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace vis2 {
  using Rc = Regularization_category<regularization_category()>::type;
  using Arrangement_2 = aos2::Arrangement_2;
  using Point_2 = aos2::Point_2;
  using Halfedge = aos2::Halfedge;
  using Halfedge_const_handle = aos2::Halfedge_const_handle;
  using Face = aos2::Face;
  using Face_const_handle = aos2::Face_const_handle;

  using Simple_polygon_visibility_2 =
    CGAL::Simple_polygon_visibility_2<Arrangement_2, Rc>;

  using Triangular_expansion_visibility_2 =
    CGAL::Triangular_expansion_visibility_2<Arrangement_2, Rc>;
}

#endif
