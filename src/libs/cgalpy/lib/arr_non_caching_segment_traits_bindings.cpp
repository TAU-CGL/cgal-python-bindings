// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/export_point_2.hpp"
#include "CGALPY/export_segment_2.hpp"

namespace bp = boost::python;

void export_arr_non_caching_segment_traits() {
  bp::scope traits_scope = bp::class_<Traits>("Traits")
    .def(bp::init<>())
    ;

  const bp::type_info pt_info = bp::type_id<TPoint_2>();
  const bp::converter::registration* pt_reg =
    bp::converter::registry::query(pt_info);
  if (pt_reg == nullptr || (*pt_reg).m_to_python == nullptr)
    export_point_2<TPoint_2>();
  else traits_scope.attr("Point_2") = bp::handle<>(pt_reg->m_class_object);

  const bp::type_info cv_info = bp::type_id<Curve_2>();
  const bp::converter::registration* cv_reg =
    bp::converter::registry::query(cv_info);
  if (cv_reg == nullptr || (*cv_reg).m_to_python == nullptr)
    export_segment_2<Curve_2>();
  else traits_scope.attr("Curve_2") = bp::handle<>(cv_reg->m_class_object);
}
