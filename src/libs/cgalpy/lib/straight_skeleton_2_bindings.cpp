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

#include <CGAL/compute_outer_frame_margin.h>
#include <CGAL/create_offset_polygons_2.h>
#include <CGAL/create_offset_polygons_from_polygon_with_holes_2.h>
#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/create_weighted_offset_polygons_2.h>
#include <CGAL/create_weighted_offset_polygons_from_polygon_with_holes_2.h>
#include <CGAL/create_weighted_straight_skeleton_2.h>
#include <CGAL/create_weighted_straight_skeleton_from_polygon_with_holes_2.h>
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
#include "generated/cgalpy/Straight_skeleton_2_docstrings.hpp"
#endif

namespace py = nanobind;
namespace doc = cgalpy::docstrings::Straight_skeleton_2;

namespace sn2 {

using Sss2 = std::shared_ptr<Straight_skeleton_2>;

#if defined(CGALPY_POLYGON_2_BINDINGS)
using Pgn = pol2::Polygon_2;
using Shared_pgn = std::shared_ptr<Pgn>;
#endif

/*! creates a straight skeleton in the interior of a polygon defined by its outer boundary.
 */
Sss2 create_interior_straight_skeleton_2_11(const py::list& points, const Kernel& kernel) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto ss = CGAL::create_interior_straight_skeleton_2(points_begin, points_end, kernel);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

/*! creates a straight skeleton in the interior of a polygon defined by its its outer boundary.
 */
Sss2 create_interior_straight_skeleton_2_12(const py::list& points)
{ return create_interior_straight_skeleton_2_11(points, Kernel()); }

/*! creates a straight skeleton in the interior of a polygon with holes defined by its outer boundary and holes.
 */
Sss2 create_interior_straight_skeleton_2_21(const py::list& points, const py::list& holes, const Kernel& kernel) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto holes_begin = stl_nested_forward_iterator<Point_2>(holes, true);
  auto holes_end = stl_nested_forward_iterator<Point_2>(holes, false);
  auto ss = CGAL::create_interior_straight_skeleton_2(points_begin, points_end, holes_begin, holes_end, kernel);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

/*! creates a straight skeleton in the interior of a polygon with holes defined by its outer boundary and holes.
 */
Sss2 create_interior_straight_skeleton_2_22(const py::list& points, const py::list& holes)
{ return create_interior_straight_skeleton_2_21(points, holes, Kernel()); }

/*! creates a straight skeleton in the limited exterior of a polygon defined by its outer boundary.
 */
Sss2 create_exterior_straight_skeleton_2_11(const FT& max_offset, const py::list& points, const Kernel& kernel) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto ss = CGAL::create_exterior_straight_skeleton_2(max_offset, points_begin, points_end, kernel);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

/*! creates a straight skeleton in the limited exterior of a polygon defined by its outer boundary.
 */
Sss2 create_exterior_straight_skeleton_2_12(const FT& max_offset, const py::list& points)
{ return create_exterior_straight_skeleton_2_11(max_offset, points, Kernel()); }

/*! creates a straight skeleton in the interior of a polygon defined by its outer boundary.
 */
Sss2 create_interior_weighted_straight_skeleton_2_11(const py::list& points, const py::list& weights,
                                                     const Kernel& kernel) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto weights_begin = stl_forward_iterator<FT>(weights, true);
  auto weights_end = stl_forward_iterator<FT>(weights, false);
  auto ss = CGAL::create_interior_weighted_straight_skeleton_2(points_begin, points_end,
                                                               weights_begin, weights_end, kernel);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

/*! creates a straight skeleton in the limited interior of a polygon defined by its its outer boundary.
 */
Sss2 create_interior_weighted_straight_skeleton_2_12(const py::list& points, const py::list& weights)
{ return create_interior_weighted_straight_skeleton_2_11(points, weights, Kernel()); }

/*! creates a straight skeleton in the interior of a polygon with holes defined by its outer boundary and holes.
 */
Sss2 create_interior_weighted_straight_skeleton_2_21(const py::list& points, const py::list& holes,
                                                     const py::list& outer_weights, const py::list& hole_weights,
                                                     const Kernel& kernel) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto outer_weights_begin = stl_forward_iterator<FT>(outer_weights, true);
  auto outer_weights_end = stl_forward_iterator<FT>(outer_weights, false);
  auto holes_begin = stl_nested_forward_iterator<Point_2>(holes, true);
  auto holes_end = stl_nested_forward_iterator<Point_2>(holes, false);
  auto hole_weights_begin = stl_nested_forward_iterator<FT>(hole_weights, true);
  auto hole_weights_end = stl_nested_forward_iterator<FT>(hole_weights, false);
  auto ss = CGAL::create_interior_weighted_straight_skeleton_2(points_begin, points_end, holes_begin, holes_end,
                                                               outer_weights_begin, outer_weights_end,
                                                               hole_weights_begin, hole_weights_end,
                                                               kernel);
  if (! ss) throw std::runtime_error("Failed to create straight skeleton");
  return ss;
}

/*! creates a straight skeleton in the interior of a polygon with holes defined by its outer boundary and holes.
 */
Sss2 create_interior_weighted_straight_skeleton_2_22(const py::list& points, const py::list& holes,
                                                     const py::list& outer_weights, const py::list& hole_weights)
{ return create_interior_weighted_straight_skeleton_2_21(points, holes, outer_weights, hole_weights, Kernel()); }

/*! creates a straight skeleton in the limited exterior of a polygon defined by its outer boundary.
 */
Sss2 create_exterior_weighted_straight_skeleton_2_11(const FT& max_offset, const py::list& points,
                                                     const py::list& weights, const Kernel& kernel) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto weights_begin = stl_forward_iterator<FT>(weights, true);
  auto weights_end = stl_forward_iterator<FT>(weights, false);
  auto ss = CGAL::create_exterior_weighted_straight_skeleton_2(max_offset, points_begin, points_end,
                                                               weights_begin, weights_end, kernel);
  if (! ss) throw std::runtime_error("Failed to create weighted straight skeleton");
  return ss;
}

/*! creates a straight skeleton in the limited exterior of a polygon defined by its outer boundary.
 */
Sss2 create_exterior_weighted_straight_skeleton_2_12(const FT& max_offset, const py::list& points,
                                                     const py::list& weights)
{ return create_exterior_weighted_straight_skeleton_2_11(max_offset, points, weights, Kernel()); }

#if defined(CGALPY_POLYGON_2_BINDINGS)
/*!
 */
std::vector<Shared_pgn>
create_interior_skeleton_and_offset_polygons_2_11(const FT& offset, const Pgn& pgn, const py::list& holes,
                                                  const Kernel& kernel) {
  auto holes_begin = stl_nested_forward_iterator<Point_2>(holes, true);
  auto holes_end = stl_nested_forward_iterator<Point_2>(holes, false);
  return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pgn, holes_begin, holes_end, kernel, kernel);
}

/*!
 */
std::vector<Shared_pgn>
create_interior_skeleton_and_offset_polygons_2_12(const FT& offset, const Pgn& pgn, const py::list& holes)
{ return create_interior_skeleton_and_offset_polygons_2_11(offset, pgn, holes, Kernel()); }

#endif

#if defined(CGALPY_POLYGON_2_BINDINGS)
/*!
 */
std::vector<Shared_pgn>
create_interior_weighted_skeleton_and_offset_polygons_2_11(const FT& offset, const Pgn& pgn, const py::list& holes,
                                                           const std::vector<FT>& outer_weights,
                                                           const py::list& hole_weights,
                                                           const Kernel& kernel) {
  auto holes_begin = stl_nested_forward_iterator<Point_2>(holes, true);
  auto holes_end = stl_nested_forward_iterator<Point_2>(holes, false);
  auto hole_weights_begin = stl_nested_forward_iterator<FT>(hole_weights, true);
  auto hole_weights_end = stl_nested_forward_iterator<FT>(hole_weights, false);
  return CGAL::create_interior_weighted_skeleton_and_offset_polygons_2(offset, pgn, holes_begin, holes_end,
                                                                       outer_weights,
                                                                       hole_weights_begin, hole_weights_end,
                                                                       kernel, kernel);
}

/*!
 */
std::vector<Shared_pgn>
create_interior_weighted_skeleton_and_offset_polygons_2_12(const FT& offset, const Pgn& pgn, const py::list& holes,
                                                           const std::vector<FT>& outer_weights,
                                                           const py::list& hole_weights) {
  return create_interior_weighted_skeleton_and_offset_polygons_2_11(offset, pgn, holes, outer_weights, hole_weights,
                                                                    Kernel());
}

#endif

/*!
 */
FT compute_outer_frame_margin(const py::list& points, const FT& offset) {
  auto points_begin = stl_forward_iterator<Point_2>(points, true);
  auto points_end = stl_forward_iterator<Point_2>(points, false);
  auto res = CGAL::compute_outer_frame_margin(points_begin, points_end, offset);
  if (! res) std::overflow_error("Cannot approximate");
  return *res;
}

}

void export_straight_skeleton_2(py::module_& m) {
  using Sn = sn2::Straight_skeleton_2;
  using V = sn2::Vertex;
  using H = sn2::Halfedge;
  using F = sn2::Face;

#if defined(CGALPY_POLYGON_2_BINDINGS)
  using Pgn = pol2::Polygon_2;
  using Pwh = pol2::Polygon_with_holes_2;
#endif

  if (! add_attr<Sn>(m, "Straight_skeleton_2")) {
    py::class_<Sn> sn2_c(m, "Straight_skeleton_2", doc::CGAL_Straight_skeleton_2__class__);
    sn2_c.def(py::init<>());
  }

  // Interior
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_11,
        py::arg("points"), py::arg("kernel"),
        doc::create_interior_straight_skeleton_2);
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_12,
        py::arg("points"),
        doc::create_interior_straight_skeleton_2_1);
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_21,
        py::arg("points"), py::arg("holes"), py::arg("kernel"),
        doc::create_interior_straight_skeleton_2_2);
  m.def("create_interior_straight_skeleton_2", &sn2::create_interior_straight_skeleton_2_22,
        py::arg("points"), py::arg("holes"),
        doc::create_interior_straight_skeleton_2_3);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_interior_straight_skeleton_2",
        [](const Pgn& png, const Kernel& kernel) { return CGAL::create_interior_straight_skeleton_2(png, kernel); },
        py::arg("polygon"), py::arg("kernel"),
        doc::create_interior_straight_skeleton_2_4);
  m.def("create_interior_straight_skeleton_2",
        [](const Pgn& png) { return CGAL::create_interior_straight_skeleton_2(png); },
        py::arg("polygon"),
        doc::create_interior_straight_skeleton_2_5);
  m.def("create_interior_straight_skeleton_2",
        [](const pol2::Polygon_with_holes_2& pwh, const Kernel& kernel)
        { return CGAL::create_interior_straight_skeleton_2(pwh, kernel); },
        py::arg("polygon_with_holes"), py::arg("kernel"),
        doc::create_interior_straight_skeleton_2_6);
  m.def("create_interior_straight_skeleton_2",
        [](const pol2::Polygon_with_holes_2& pwh) { return CGAL::create_interior_straight_skeleton_2(pwh); },
        py::arg("polygon_with_holes"),
        doc::create_interior_straight_skeleton_2_7);
#endif

  // Exterior
  m.def("create_exterior_straight_skeleton_2", &sn2::create_exterior_straight_skeleton_2_11,
        py::arg("max_offset"), py::arg("points"), py::arg("kernel"),
        doc::create_exterior_straight_skeleton_2);
  m.def("create_exterior_straight_skeleton_2", &sn2::create_exterior_straight_skeleton_2_12,
        py::arg("max_offset"), py::arg("points"),
        doc::create_exterior_straight_skeleton_2_1);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_exterior_straight_skeleton_2",
        [](const FT& max_offset, const Pgn& pgn, const Kernel& kernel)
        { return CGAL::create_exterior_straight_skeleton_2(max_offset, pgn, kernel); },
        py::arg("max_offset"), py::arg("polygon"), py::arg("kernel"),
        doc::create_exterior_straight_skeleton_2_2);
  m.def("create_exterior_straight_skeleton_2",
        [](const FT& max_offset, const Pgn& pgn)
        { return CGAL::create_exterior_straight_skeleton_2(max_offset, pgn); },
        py::arg("max_offset"), py::arg("polygon"),
        doc::create_exterior_straight_skeleton_2_3);
  m.def("create_exterior_straight_skeleton_2",
        [](const FT& max_offset, const Pwh& pwh, const Kernel& kernel)
        { return CGAL::create_exterior_straight_skeleton_2(max_offset, pwh, kernel); },
        py::arg("max_offset"), py::arg("polygon_with_holes"), py::arg("kernel"),
        doc::create_exterior_straight_skeleton_2_4);
  m.def("create_exterior_straight_skeleton_2",
        [](const FT& max_offset, const Pwh& pwh) { return CGAL::create_exterior_straight_skeleton_2(max_offset, pwh); },
        py::arg("max_offset"), py::arg("polygon_with_holes"),
        doc::create_exterior_straight_skeleton_2_5);
#endif

  // Weighted

  // Interior weigheed
  m.def("create_interior_weighted_straight_skeleton_2", &sn2::create_interior_weighted_straight_skeleton_2_11,
        py::arg("points"), py::arg("weights"), py::arg("kernel"),
        doc::create_interior_weighted_straight_skeleton_2);
  m.def("create_interior_weighted_straight_skeleton_2", &sn2::create_interior_weighted_straight_skeleton_2_12,
        py::arg("points"), py::arg("weights"),
        doc::create_interior_weighted_straight_skeleton_2_1);
  m.def("create_interior_weighted_straight_skeleton_2", &sn2::create_interior_weighted_straight_skeleton_2_21,
        py::arg("points"), py::arg("holes"), py::arg("outer_weights"), py::arg("hole_weights"), py::arg("kernel"),
        doc::create_interior_weighted_straight_skeleton_2_2);
  m.def("create_interior_weighted_straight_skeleton_2", &sn2::create_interior_weighted_straight_skeleton_2_22,
        py::arg("points"), py::arg("holes"), py::arg("outer_weights"), py::arg("hole_weights"),
        doc::create_interior_weighted_straight_skeleton_2_3);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_interior_weighted_straight_skeleton_2",
        [](const Pgn& png,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights,
           const Kernel& kernel)
        { return CGAL::create_interior_weighted_straight_skeleton_2(png, weights, kernel); },
        py::arg("polygon"), py::arg("weights"), py::arg("kernel"),
        doc::create_interior_weighted_straight_skeleton_2_4);
  m.def("create_interior_weighted_straight_skeleton_2",
        [](const Pgn& png,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_interior_weighted_straight_skeleton_2(png, weights); },
        py::arg("polygon"), py::arg("weights"),
        doc::create_interior_weighted_straight_skeleton_2_5);
  m.def("create_interior_weighted_straight_skeleton_2",
        [](const pol2::Polygon_with_holes_2& pwh, const std::vector<std::vector<FT>>& weights, const Kernel& kernel)
        { return CGAL::create_interior_weighted_straight_skeleton_2(pwh, weights, kernel); },
        py::arg("polygon_with_holes"), py::arg("weights"), py::arg("kernel"),
        doc::create_interior_weighted_straight_skeleton_2_6);
  m.def("create_interior_weighted_straight_skeleton_2",
        [](const pol2::Polygon_with_holes_2& pwh, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_interior_weighted_straight_skeleton_2(pwh, weights); },
        py::arg("polygon_with_holes"), py::arg("weights"),
        doc::create_interior_weighted_straight_skeleton_2_7);
#endif

  // Exterior weigheed
  m.def("create_exterior_weighted_straight_skeleton_2", &sn2::create_exterior_weighted_straight_skeleton_2_11,
        py::arg("max_offset"), py::arg("points"), py::arg("weights"), py::arg("kernel"),
        doc::create_exterior_weighted_straight_skeleton_2);
  m.def("create_exterior_weighted_straight_skeleton_2", &sn2::create_exterior_weighted_straight_skeleton_2_12,
        py::arg("max_offset"), py::arg("points"), py::arg("weights"),
        doc::create_exterior_weighted_straight_skeleton_2_1);
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_exterior_weighted_straight_skeleton_2",
        [](const FT& max_offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights,
           const Kernel& kernel)
        { return CGAL::create_exterior_weighted_straight_skeleton_2(max_offset, pgn, weights, kernel); },
        py::arg("max_offset"), py::arg("polygon"), py::arg("weights"), py::arg("kernel"));
  m.def("create_exterior_weighted_straight_skeleton_2",
        [](const FT& max_offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_exterior_weighted_straight_skeleton_2(max_offset, pgn, weights); },
        py::arg("max_offset"), py::arg("polygon"), py::arg("weights"));
  m.def("create_exterior_weighted_straight_skeleton_2",
        [](const FT& max_offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights, const Kernel& kernel)
        { return CGAL::create_exterior_weighted_straight_skeleton_2(max_offset, pwh, weights, kernel); },
        py::arg("max_offset"), py::arg("polygon_with_holes"), py::arg("weights"), py::arg("kernel"));
  m.def("create_exterior_weighted_straight_skeleton_2",
        [](const FT& max_offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_exterior_weighted_straight_skeleton_2(max_offset, pwh, weights); },
        py::arg("max_offset"), py::arg("polygon_with_holes"), py::arg("weights"));
#endif

  // Offset
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_offset_polygons_2",
        [](const FT& offset, const sn2::Straight_skeleton_2& skeleton, const Kernel& kernel)
        { return CGAL::create_offset_polygons_2<Pgn, FT, Sn, Kernel>(offset, skeleton, kernel); },
        py::arg("offset"), py::arg("skeleton"), py::arg("kernel"),
        doc::create_offset_polygons_2);
  m.def("create_offset_polygons_2",
        [](const FT& offset, const sn2::Straight_skeleton_2& skeleton)
        { return CGAL::create_offset_polygons_2<Pgn, FT, Sn, Kernel>(offset, skeleton); },
        py::arg("offset"), py::arg("skeleton"),
        doc::create_offset_polygons_2_1);
#endif

  // Interior offset
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_interior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn, const Kernel& kernel)
        { return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pgn, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("kernel"));
  m.def("create_interior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pgn); },
        py::arg("offset"), py::arg("polygon"));
  m.def("create_interior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh, const Kernel& kernel)
        { return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pwh, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("kernel"));
  m.def("create_interior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_interior_skeleton_and_offset_polygons_2(offset, pwh); },
        py::arg("offset"), py::arg("polygon_with_holes"));
  m.def("create_interior_skeleton_and_offset_polygons_2_11", &sn2::create_interior_skeleton_and_offset_polygons_2_11,
        py::arg("offset"), py::arg("polygon"), py::arg("holes"), py::arg("kernel"));
  m.def("create_interior_skeleton_and_offset_polygons_2_12", &sn2::create_interior_skeleton_and_offset_polygons_2_12,
        py::arg("offset"), py::arg("polygon"), py::arg("holes"));
  m.def("create_interior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn, const Kernel& kernel)
        { return CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pgn, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("kernel"));
  m.def("create_interior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pgn); },
        py::arg("offset"), py::arg("polygon"));
  m.def("create_interior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh, const Kernel& kernel)
        { return CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pwh, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("kernel"));
  m.def("create_interior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_interior_skeleton_and_offset_polygons_with_holes_2(offset, pwh); },
        py::arg("offset"), py::arg("polygon_with_holes"));
#endif

  // Exterior offset
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_exterior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn, const Kernel& kernel)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, pgn, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("kernel"));
  m.def("create_exterior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, pgn); },
        py::arg("offset"), py::arg("polygon"));
  m.def("create_exterior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh, const Kernel& kernel)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, pwh, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("kernel"));
  m.def("create_exterior_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_2(offset, pwh); },
        py::arg("offset"), py::arg("polygon_with_holes"));
  m.def("create_exterior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn, const Kernel& kernel)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_with_holes_2(offset, pgn, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("kernel"));
  m.def("create_exterior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_with_holes_2(offset, pgn); },
        py::arg("offset"), py::arg("polygon"));
  m.def("create_exterior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh, const Kernel& kernel)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_with_holes_2(offset, pwh, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("kernel"));
  m.def("create_exterior_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh)
        { return CGAL::create_exterior_skeleton_and_offset_polygons_with_holes_2(offset, pwh); },
        py::arg("offset"), py::arg("polygon_with_holes"));
#endif

  // Weighted offset

  // Interior weigheed offset
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_interior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights,
           const Kernel& kernel)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_2(offset, pgn, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"), py::arg("kernel"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_2(offset, pgn, weights); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights, const Kernel& kernel)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_2(offset, pwh, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"), py::arg("kernel"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_2(offset, pwh, weights); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_2_11",
        &sn2::create_interior_weighted_skeleton_and_offset_polygons_2_11,
        py::arg("offset"), py::arg("polygon"), py::arg("holes"), py::arg("outer_weights"), py::arg("hole_weights"),
        py::arg("kernel"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_2_12",
        &sn2::create_interior_weighted_skeleton_and_offset_polygons_2_12,
        py::arg("offset"), py::arg("polygon"), py::arg("holes"), py::arg("outer_weights"), py::arg("hole_weights"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights,
           const Kernel& kernel)
  { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pgn, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"), py::arg("kernel"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pgn, weights); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights, const Kernel& kernel)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pwh, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"), py::arg("kernel"));
  m.def("create_interior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_interior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pwh, weights); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"));
#endif

  // Exterior weigheed offset
#if defined(CGALPY_POLYGON_2_BINDINGS)
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights,
           const Kernel& kernel)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_2(offset, pgn, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"), py::arg("kernel"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_2(offset, pgn, weights); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights, const Kernel& kernel)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_2(offset, pwh, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"), py::arg("kernel"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_2(offset, pwh, weights); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn,
           // The following is a bug in CGAL. It should be just one level of range
           const std::vector<std::vector<FT>>& weights,
           const Kernel& kernel)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pgn, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"), py::arg("kernel"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pgn& pgn, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pgn, weights); },
        py::arg("offset"), py::arg("polygon"), py::arg("weights"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights, const Kernel& kernel)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pwh, weights, kernel, kernel); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"), py::arg("kernel"));
  m.def("create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2",
        [](const FT& offset, const Pwh& pwh, const std::vector<std::vector<FT>>& weights)
        { return CGAL::create_exterior_weighted_skeleton_and_offset_polygons_with_holes_2(offset, pwh, weights); },
        py::arg("offset"), py::arg("polygon_with_holes"), py::arg("weights"));
#endif

  // Auxiliary
  // arrange_offset_polygons_2

  m.def("compute_outer_frame_margin", sn2::compute_outer_frame_margin,
        py::arg("points"), py::arg("ft"),
        doc::compute_outer_frame_margin);

  // convert_straight_skeleton_2

  // extrude_skeleton

  m.def("print_straight_skeleton", &CGAL::Straight_skeletons_2::IO::print_straight_skeleton<Kernel>,
        py::arg("skeleton"));
#if defined(CGALPY_POLYGON_2_BINDINGS)
  using Pc = Point_2_container;
  m.def("print_polygons", &CGAL::Straight_skeletons_2::IO::print_polygons<Kernel, Pc>,
        py::arg("polygons"));
  m.def("print_polygons_with_holes", &CGAL::Straight_skeletons_2::IO::print_polygons_with_holes<Kernel, Pc>,
        py::arg("polygons_with_holes"));
#endif

#ifdef CGALPY_HAS_VISUAL
  m.def("draw",
        [](const Sn& sn, const char* title) { CGAL::draw(sn, title); },
        py::arg("skeleton"), py::arg("title"));

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  m.def("draw",
        [](const Sn& sn, const bvr::Graphics_scene_options& gso, const char* title) { CGAL::draw(sn, gso, title); },
        py::arg("skeleton"), py::arg("options"), py::arg("title"));
#endif
#endif
}
