// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/gps_2_concepts/export_GpsTraits_2.hpp"
#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

py::object export_arr_circle_segment_traits(py::module_&);

namespace bso2 {

// Initialize a polygon from a list of circle/segment x-monotone curves.
template <typename T>
void init_polygon_2(typename T::Polygon_2& pgn, py::list& lst) {
  using Xcv = typename T::X_monotone_curve_2;
  auto begin = stl_input_iterator<Xcv>(lst);
  auto end = stl_input_iterator<Xcv>(lst, false);
  new (&pgn) typename T::Polygon_2(begin, end);
}

}

// Wrap the instance Gps_circle_segment_traits_2 traits <Kernel>.
py::object export_gps_circle_segment_traits(py::module_& m) {
  using Agt = aos2::Arr_geometry_traits_2;
  using Xcv = Agt::X_monotone_curve_2;
  using Ggt = CGAL::Gps_circle_segment_traits_2<Kernel>;
  using Pgn = Ggt::Polygon_2;

  export_arr_circle_segment_traits(m);

  py::class_<Ggt, Agt> traits_c(m, "Gps_circle_segment_traits_2");
  traits_c.def(py::init<>());
  struct Concepts {
    Gps_traits_classes<Ggt> m_traits_classes;
  } concepts;
  export_GpsTraits_2<Ggt>(traits_c, concepts);
  BOOST_ASSERT(concepts.m_traits_classes.m_polygon_2);
  auto& pgn_c = *(concepts.m_traits_classes.m_polygon_2);
  pgn_c.def("__init__", &bso2::init_polygon_2<Ggt>);

  using Cci = Pgn::Curve_const_iterator;
  add_iterator<Cci, Cci, const Xcv&>("Curve_iterator", pgn_c);
  pgn_c.def("curves",
            [](const Pgn& pgn)
            { return make_iterator(pgn.curves_begin(), pgn.curves_end()); },
            py::keep_alive<0, 1>());

  return traits_c;
}
