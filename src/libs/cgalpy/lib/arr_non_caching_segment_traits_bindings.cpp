// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <boost/assert.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/export_point_2.hpp"
#include "CGALPY/export_segment_2.hpp"

namespace py = nanobind;

py::class_<aos2::Geometry_traits_2>
export_arr_non_caching_segment_traits(py::module_& m) {
  typedef aos2::Geometry_traits_2       GT;
  py::scope traits_scope = py::class_<GT>(m, "Traits")
    .def(py::init<>())
    ;

  const py::type_info pt_info = py::type_id<Point_2>();
  const py::converter::registration* pt_reg =
    py::converter::registry::query(pt_info);
  BOOST_ASSERT((pt_reg != nullptr) || ((*pt_reg).m_to_python != nullptr));
  traits_scope.attr("Point_2") = py::handle<>(pt_reg->m_class_object);

  const py::type_info cv_info = py::type_id<Curve_2>();
  const py::converter::registration* cv_reg =
    py::converter::registry::query(cv_info);
  BOOST_ASSERT((cv_reg != nullptr) || ((*cv_reg).m_to_python != nullptr));
  traits_scope.attr("Curve_2") = py::handle<>(cv_reg->m_class_object);

  const py::type_info cv_info = py::type_id<X_monotone_curve_2>();
  const py::converter::registration* cv_reg =
    py::converter::registry::query(cv_info);
  BOOST_ASSERT((cv_reg != nullptr) || ((*cv_reg).m_to_python != nullptr));
  traits_scope.attr("X_monotone_curve_2") = py::handle<>(cv_reg->m_class_object);
}
