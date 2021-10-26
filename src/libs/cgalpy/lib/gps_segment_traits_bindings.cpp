// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <CGAL/Gps_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/gps_2_concepts/export_GpsTraits_2.hpp"
#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"

namespace bp = boost::python;

bp::object export_gps_segment_traits() {
  typedef bso2::Traits_2       GT;
  auto traits = bp::class_<GT>("Traits_2");
  bp::scope traits_scope(traits);
  struct Concepts {
    Gps_traits_classes<GT> m_traits_classes;
  };
  Concepts concepts;
  export_GpsTraits_2<GT>(traits, concepts);
  return traits;
}
