// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYHEDRON_3_TYPES_HPP
#define CGALPY_POLYHEDRON_3_TYPES_HPP

#include <nanobind/nanobind.h>

#include <CGAL/HalfedgeDS_vertex_base.h>

#include "CGALPY/polyhedron_3_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace pol3 {

struct Polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Vertex_wrapper {
    using Pnt = typename Traits::Point_3;
    typedef Vertex_extended<is_face_extended(), Refs, Pnt, py::object> Vertex;
  };

  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef Halfedge_extended<is_halfedge_extended(), Refs, py::object> Halfedge;
  };

  template <typename Refs, typename Traits>
  struct Face_wrapper {
    using Pln = typename Traits::Plane_3;
    typedef Facet_extended<is_facet_extended(), Refs, Pln, py::object> Face;
  };
};

typedef CGAL::Polyhedron_3<Kernel, Polyhedron_items> Polyhedron_2;

}

#endif
