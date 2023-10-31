// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ENVELOPETRAITS_3_HPP
#define CGALPY_EXPORT_ENVELOPETRAITS_3_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

//
template <typename T>
py::list mk_xy_monotone_3_call_operator(const typename T::Make_xy_monotone_3& m,
                                        const typename T::Surface_3& s,
                                        bool is_lower) {
  using Pnt = typename T::Point_2;
  using Multiplicity = typename T::Multiplicity;
  using Xcv = typename T::X_monotone_curve_2;
  using Xcv_res = std::pair<Xcv, Multiplicity>;
  using Result = std::variant<Xcv_res, Pnt>;

  py::list res;
  // auto op =
  //   [&] (const Result& o) mutable {
  //     if (auto* xcv_res = std::get_if<Xcv_res>(&o)) {
  //       py::list xcv_lst;
  //       xcv_lst.append(xcv_res->first);
  //       xcv_lst.append(xcv_res->second);
  //       res.append(xcv_lst);
  //     }
  //     else if (auto* p = std::get_if<Pnt>(&o)) res.append(*p);
  //   };
  // // The argument type of boost::function_output_iterator (UnaryFunction) must
  // // be Assignable and Copy Constructible; hence the application of std::ref().
  // auto it = boost::make_function_output_iterator(std::ref(op));
  // m(s, is_lower, it);
  return res;
}

//
template <typename T>
py::list
cpb_2_call_operator(const typename T::Construct_projected_boundary_2& m,
                    const typename T::Xy_monotone_surface_3& s) {
  py::list res;
  return res;
}

//
template <typename T>
py::list
cpi_2_call_operator(const typename T::Construct_projected_intersections_2& m,
                    const typename T::Xy_monotone_surface_3& s1,
                    const typename T::Xy_monotone_surface_3& s2) {
  py::list res;
  return res;
}

//! Export the 3D envelope traits function objects
template <typename T, typename C, typename Concepts>
void export_EnvelopeTraits_3(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  using Pnt = typename T::Point_2;
  using Xcv = typename T::X_monotone_curve_2;

  using Srf = typename T::Surface_3;
  using Xy_monotone_srf_3 = typename T::Xy_monotone_surface_3;
  using Make_xy_monotone_3 = typename T::Make_xy_monotone_3;
  using Cpb_2 = typename T::Construct_projected_boundary_2;
  using Cpi_2 = typename T::Construct_projected_intersections_2;
  using Cmp_z_at_xy_3 = typename T::Compare_z_at_xy_3;
  using Cmp_z_at_xy_above_3 = typename T::Compare_z_at_xy_above_3;
  using Cmp_z_at_xy_below_3 = typename T::Compare_z_at_xy_below_3;

  auto& classes = concepts.m_env_traits_3_classes;

  // Surface_3
  if (! add_attr<Srf>(c, "Surface_3")) {
    classes.m_surface_3 = new py::class_<Srf>(c, "Surface_3");
    classes.m_surface_3->def(py::init<>());
    classes.m_surface_3->def(py::init<const Srf&>());
  }

  // Xy_monotone_surface_3
  if (! add_attr<Xy_monotone_srf_3>(c, "Xy_monotone_surface_3")) {
    classes.m_xy_monotone_surface_3 =
      new py::class_<Xy_monotone_srf_3>(c, "Xy_monotone_surface_3");
    classes.m_xy_monotone_surface_3->def(py::init<>());
    classes.m_xy_monotone_surface_3->def(py::init<const Xy_monotone_srf_3&>());
  }

  // Make_xy_monotone_3
  if (! add_attr<Make_xy_monotone_3>(c, "Make_xy_monotone_3")) {
    classes.m_make_xy_monotone_3 =
      new py::class_<Make_xy_monotone_3>(c, "Make_xy_monotone_3");
    classes.m_make_xy_monotone_3->def("__call__",
                                      &mk_xy_monotone_3_call_operator<T>);
  }

  // Construct_projected_boundary_2
  if (! add_attr<Cpb_2>(c, "Construct_projected_boundary_2")) {
    classes.m_cpb_2 = new py::class_<Cpb_2>(c, "Construct_projected_boundary_2");
    classes.m_cpb_2->def("__call__", &cpb_2_call_operator<T>);
  }

  // Construct_projected_intersections_2
  if (! add_attr<Cpi_2>(c, "Construct_projected_intersections_2")) {
    classes.m_cpi_2 =
      new py::class_<Cpi_2>(c, "Construct_projected_intersections_2");
    classes.m_cpi_2->def("__call__", &cpi_2_call_operator<T>);
  }

  // Compare_z_at_xy_3
  if (! add_attr<Cmp_z_at_xy_3>(c, "Compare_z_at_xy_3")) {
    classes.m_cmp_z_at_xy_3 =
      new py::class_<Cmp_z_at_xy_3>(c, "Compare_z_at_xy_3");
    classes.m_cmp_z_at_xy_3->def("__call__",
                                 py::overload_cast<const Pnt&,
                                                   const Xy_monotone_srf_3&,
                                                   const Xy_monotone_srf_3&>
                                 (&Cmp_z_at_xy_3::operator(), py::const_));
    classes.m_cmp_z_at_xy_3->def("__call__",
                                 py::overload_cast<const Xcv&,
                                                   const Xy_monotone_srf_3&,
                                                   const Xy_monotone_srf_3&>
                                 (&Cmp_z_at_xy_3::operator(), py::const_));
  }

  // Compare_z_at_xy_above_3
  if (! add_attr<Cmp_z_at_xy_above_3>(c, "Compare_z_at_xy_above_3")) {
    classes.m_cmp_z_at_xy_above_3 =
      new py::class_<Cmp_z_at_xy_above_3>(c, "Compare_z_at_xy_above_3");
    classes.m_cmp_z_at_xy_above_3->def("__call__",
                                       &Cmp_z_at_xy_above_3::operator());
  }

  // Compare_z_at_xy_below_3
  if (! add_attr<Cmp_z_at_xy_below_3>(c, "Compare_z_at_xy_below_3")) {
    classes.m_cmp_z_at_xy_below_3 =
      new py::class_<Cmp_z_at_xy_below_3>(c, "Compare_z_at_xy_below_3");
    classes.m_cmp_z_at_xy_below_3->def("__call__",
                                       &Cmp_z_at_xy_below_3::operator());
  }

  c.def("make_xy_monotone_3_object", &T::make_xy_monotone_3_object);
  c.def("construct_projected_boundary_2_object",
        &T::construct_projected_boundary_2_object);
  c.def("construct_projected_intersections_2_object",
        &T::construct_projected_intersections_2_object);
  c.def("compare_z_at_xy_3_object", &T::compare_z_at_xy_3_object);
  c.def("compare_z_at_xy_above_3_object", &T::compare_z_at_xy_above_3_object);
  c.def("compare_z_at_xy_below_3_object", &T::compare_z_at_xy_below_3_object);
}

#endif
