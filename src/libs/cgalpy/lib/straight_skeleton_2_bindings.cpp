// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <memory>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/vector.h>

#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/Straight_skeleton_2.h>
#include <CGAL/Straight_skeleton_2/IO/print.h>
#ifdef CGALPY_HAS_VISUAL
#define CGAL_USE_BASIC_VIEWER
#include <CGAL/draw_straight_skeleton_2.h>
#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
#include "CGALPY/basic_viewer_types.hpp"
#endif
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/straight_skeleton_2_types.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/stl_nested_forward_iterator.hpp"
#if defined(CGALPY_POLYGON_2_BINDINGS)
#include "CGALPY/polygon_2_types.hpp"
#endif

namespace py = nanobind;

namespace sn2 {

using Shared_straight_skeleton_2 = std::shared_ptr<Straight_skeleton_2>;

Shared_straight_skeleton_2 create_interior_straight_skeleton_2_1(const py::list& points) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto ss = CGAL::create_interior_straight_skeleton_2(points_begin, points_end);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

Shared_straight_skeleton_2 create_interior_straight_skeleton_2_2(const py::list& points, const py::list& holes) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto holes_begin = stl_nested_forward_iterator<Point_2>(holes, true);
  auto holes_end = stl_nested_forward_iterator<Point_2>(holes, false);
  auto ss = CGAL::create_interior_straight_skeleton_2(points_begin, points_end, holes_begin, holes_end);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}


#if defined(CGALPY_POLYGON_2_BINDINGS)
Shared_straight_skeleton_2 create_interior_straight_skeleton_2_3(const pol2::Polygon_2& polygon) {
  auto ss = CGAL::create_interior_straight_skeleton_2(polygon);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

Shared_straight_skeleton_2 create_interior_straight_skeleton_2_4(const pol2::Polygon_with_holes_2& pwh) {
  auto ss = CGAL::create_interior_straight_skeleton_2(pwh);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}
#endif

Shared_straight_skeleton_2 create_exterior_straight_skeleton_2_1(const FT& max_offset, const py::list& points) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto ss = CGAL::create_exterior_straight_skeleton_2(max_offset, points_begin, points_end);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

#if defined(CGALPY_POLYGON_2_BINDINGS)
Shared_straight_skeleton_2 create_exterior_straight_skeleton_2_2(const FT& max_offset, const pol2::Polygon_2& polygon) {
  auto ss = CGAL::create_exterior_straight_skeleton_2(max_offset, polygon);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}
#endif

}

void export_straight_skeleton_2(py::module_& m) {
  using Sn = sn2::Straight_skeleton_2;
  using V = sn2::Vertex;
  using H = sn2::Halfedge;
  using F = sn2::Face;
  if (! add_attr<Sn>(m, "Straight_skeleton_2")) {
    py::class_<Sn> sn2_c(m, "Straight_skeleton_2");
    sn2_c.def(py::init<>());
  }

  // interior
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_1);
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_2);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_3);
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_4);
#endif

  // exterior
  m.def("create_exterior_straight_skeleton_2", &sn2::create_exterior_straight_skeleton_2_1);
  m.def("create_exterior_straight_skeleton_2", &sn2::create_exterior_straight_skeleton_2_2);

  // offset
  // m.def("create_offset_polygons_2", &sn2::create_offset_polygons_2);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  using Pgn = pol2::Polygon_2;
  using Pwh = pol2::Polygon_with_holes_2;
  m.def("create_offset_polygons_2",
        [](const FT& offset, const sn2::Straight_skeleton_2& skeleton)
        { return CGAL::create_offset_polygons_2<Pgn, FT, Sn, Kernel>(offset, skeleton); });

  // interior offset
  m.def("create_interior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pgn); });
  m.def("create_interior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pwh); });
  m.def("create_interior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pgn); });
  m.def("create_interior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pwh); });

  // exterior offset
  m.def("create_exterior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, pgn); });
  m.def("create_exterior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, pwh); });
  m.def("create_exterior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_with_holes_2(offset, pgn); });
  m.def("create_exterior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_with_holes_2(offset, pwh); });
#endif

  // auxiliary
  m.def("print_straight_skeleton", &CGAL::Straight_skeletons_2::IO::print_straight_skeleton<Kernel>);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  using Pc = Point_2_container;
  m.def("print_polygons", &CGAL::Straight_skeletons_2::IO::print_polygons<Kernel, Pc>);
  m.def("print_polygons_with_holes", &CGAL::Straight_skeletons_2::IO::print_polygons_with_holes<Kernel, Pc>);
#endif

#ifdef CGALPY_HAS_VISUAL
  m.def("draw",
        [](const Sn& sn, const char* title) { CGAL::draw(sn, title); });

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  m.def("draw",
        [](const Sn& sn, const bvr::Graphics_scene_options& gso, const char* title) { CGAL::draw(sn, gso, title); });
#endif
#endif
}
