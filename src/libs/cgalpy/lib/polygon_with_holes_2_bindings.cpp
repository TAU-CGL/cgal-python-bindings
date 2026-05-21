// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"
#include "CGALPY/add_extraction.hpp"
#ifdef CGALPY_HAS_VISUAL
#define CGAL_USE_BASIC_VIEWER
#include <CGAL/draw_polygon_with_holes_2.h>
#endif

namespace py = nanobind;

namespace pol2 {

// Initialize a polygon with holes from an outer boundary and a list of holes.
void init_polygon_with_holes_2(Polygon_with_holes_2* pwh, Polygon_2& p,
                               py::list& lst) {
  auto begin = stl_forward_iterator<Polygon_2>(lst);
  auto end = stl_forward_iterator<Polygon_2>(lst, false);
  new (pwh) Polygon_with_holes_2(p, begin, end);
}

}

/*! Export `CGAL::Polygon_with_holes_2<>`, which derives from
 * `CGAL::General_polygon_with_holes_2<>`
 */
void export_polygon_with_holes_2(py::module_& m) {
  using Pgn = pol2::Polygon_2;
  using Pwh = pol2::Polygon_with_holes_2;
  using Gpwh = pol2::General_polygon_with_holes_2;

  if (! add_attr<Gpwh>(m, "General_polygon_with_holes_2")) {
    py::class_<Gpwh> gpwh_c(
      m,
      "General_polygon_with_holes_2",
      "Polygon with holes represented by an outer boundary and zero or more holes."
    );
    export_general_polygon_with_holes_2(gpwh_c);
  }

  if (! add_attr<Pwh>(m, "Polygon_with_holes_2")) {
    py::class_<Pwh, Gpwh> pwh_c(
      m,
      "Polygon_with_holes_2",
      "Polygon with holes represented by a linear outer boundary and zero or more polygonal holes."
    );
    pwh_c
      .def(py::init<>(),
           "Construct an empty polygon with holes.")
      .def(py::init<Pgn&>(),
           py::arg("outer_boundary"),
           "Construct a polygon with holes from an outer boundary and no holes.")
      .def("__init__", &pol2::init_polygon_with_holes_2,
           py::arg("outer_boundary"),
           py::arg("holes"),
           "Initialize a polygon with holes from an outer boundary and a list of hole polygons.")

      .def("bbox", &Pwh::bbox,
           "Return the bounding box of the polygon with holes.")
      .def(py::self == py::self,
           py::arg("other"),
           "Return whether two polygons with holes are equal.")
      .def(py::self != py::self,
           py::arg("other"),
           "Return whether two polygons with holes are not equal.")
      ;

    add_insertion(pwh_c, "__str__");
    add_insertion(pwh_c, "__repr__");
    add_extraction(pwh_c);
  }

#ifdef CGALPY_HAS_VISUAL
  using Draw = void(*)(const Pwh&, const char*);
  m.def("draw", static_cast<Draw>(CGAL::draw),
        py::arg("pwh"), py::arg("title") = "",
        "Draw a polygon with holes in a viewer window.");
#endif
}
