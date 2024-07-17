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

#include <CGAL/Polyhedron_items_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_traits_with_normals_3.h>

#include "CGALPY/polyhedron_3_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace pol3 {

struct Polyhedron_items : public CGAL::Polyhedron_items_3 {
  template <typename Refs, typename Traits>
  struct Vertex_wrapper {
    using Pnt = typename Traits::Point_3;
    using Vb = typename Vertex_with_id<vertex_with_id(), Refs, Pnt>::type;
    typedef typename Vertex_extended<is_vertex_extended(), Vb, py::object>::type
      Vertex;
  };

  template <typename Refs, typename Traits>
  struct Halfedge_wrapper {
    typedef typename Halfedge_extended<is_halfedge_extended(), Refs,
                                       py::object>::type        Halfedge;
  };

  template <typename Refs, typename Traits>
  struct Face_wrapper {
    using Pln = typename Traits::Plane_3;
    using Fb = typename Face_with_id<face_with_id(), Refs, Pln>::type;
    typedef typename Face_extended<is_face_extended(), Fb, py::object>::type
      Face;
  };
};

#if CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_KERNEL_GEOMETRY_TRAITS
using Traits = Kernel;
#elif CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS
using Traits = CGAL::Polyhedron_traits_with_normals_3<Kernel>;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_POL3_GEOMETRY_TRAITS");
#endif

typedef CGAL::Polyhedron_3<Traits, Polyhedron_items> Polyhedron_3;
typedef Polyhedron_3::Point_3                        Point_3;
typedef Polyhedron_3::Vertex                         Vertex;
typedef Polyhedron_3::Halfedge                       Halfedge;
typedef Polyhedron_3::Face                           Face;

typedef Polyhedron_3::Vertex_handle                  Vertex_handle;
typedef Polyhedron_3::Halfedge_handle                Halfedge_handle;
typedef Polyhedron_3::Face_handle                    Face_handle;

typedef Polyhedron_3::Vertex_const_handle            Vertex_const_handle;
typedef Polyhedron_3::Halfedge_const_handle          Halfedge_const_handle;
typedef Polyhedron_3::Face_const_handle              Face_const_handle;

using Halfedge_around_facet_circulator =
  Polyhedron_3::Halfedge_around_facet_circulator;
using Halfedge_around_facet_const_circulator =
  Polyhedron_3::Halfedge_around_facet_const_circulator;

}

#endif
