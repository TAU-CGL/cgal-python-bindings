// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>
#include <boost/assert.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/export_point_2.hpp"
#include "CGALPY/export_segment_2.hpp"

namespace bp = boost::python;

bp::class_<aos2::Geometry_traits_2> export_arr_non_caching_segment_traits() {
  typedef aos2::Geometry_traits_2       GT;
  bp::scope traits_scope = bp::class_<GT>("Traits")
    .def(bp::init<>())
    ;

  const bp::type_info pt_info = bp::type_id<Point_2>();
  const bp::converter::registration* pt_reg =
    bp::converter::registry::query(pt_info);
  BOOST_ASSERT((pt_reg != nullptr) || ((*pt_reg).m_to_python != nullptr));
  traits_scope.attr("Point_2") = bp::handle<>(pt_reg->m_class_object);

  const bp::type_info cv_info = bp::type_id<Curve_2>();
  const bp::converter::registration* cv_reg =
    bp::converter::registry::query(cv_info);
  BOOST_ASSERT((cv_reg != nullptr) || ((*cv_reg).m_to_python != nullptr));
  traits_scope.attr("Curve_2") = bp::handle<>(cv_reg->m_class_object);

  const bp::type_info cv_info = bp::type_id<X_monotone_curve_2>();
  const bp::converter::registration* cv_reg =
    bp::converter::registry::query(cv_info);
  BOOST_ASSERT((cv_reg != nullptr) || ((*cv_reg).m_to_python != nullptr));
  traits_scope.attr("X_monotone_curve_2") = bp::handle<>(cv_reg->m_class_object);
}
