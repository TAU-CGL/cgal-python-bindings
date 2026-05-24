// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/General_polygon_set_2.h>
#include <CGAL/IO/Gps_iostream.h>

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/append_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

#include "cgalpy/Bso2_docstrings.hpp"

// The following might be needed in the future when extraction support is added
// to specific curves types. At that point we can add support for extraction
// of general polygons with holes bounded by curves of the specific types.

// #include <CGAL/Gps_circle_segment_traits_2.h>
// #include <CGAL/CORE_algebraic_number_traits.h>
// #include <CGAL/Arr_conic_traits_2.h>
// #include <CGAL/Arr_Bezier_curve_traits_2.h>

namespace py = nanobind;
namespace bso2_doc = cgalpy::docstrings::Boolean_set_operations_2;

namespace bso2 {

py::list polygons_with_holes(Gps_on_surface_base_2& ps) {
  py::list lst;
  ps.polygons_with_holes(append_iterator(lst));
  return lst;
}

//
void insert1(Gps_on_surface_base_2& ps, py::list& pgn_lst, py::list& pwh_lst) {
  using Pgn = General_polygon_2;
  using Pwh = General_polygon_with_holes_2;
  auto begin0 = stl_forward_iterator<Pgn>(pgn_lst);
  auto end0 = stl_forward_iterator<Pgn>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<Pwh>(pwh_lst);
  auto end1 = stl_forward_iterator<Pwh>(pwh_lst, false);
  ps.insert(begin0, end0, begin1, end1);
}

//
void insert2(Gps_on_surface_base_2& ps, py::list& lst) {
  if (lst.size() == 0) return;
  using Pgn = General_polygon_2;
  if (py::isinstance<Pgn>(lst[0])) {
    auto begin = stl_forward_iterator<Pgn>(lst);
    auto end = stl_forward_iterator<Pgn>(lst, false);
    ps.insert(begin, end);
    return;
  }

  using Pwh = General_polygon_with_holes_2;
  if (py::isinstance<Pwh>(lst[0])) {
    auto begin = stl_forward_iterator<Pwh>(lst);
    auto end = stl_forward_iterator<Pwh>(lst, false);
    ps.insert(begin, end);
    return;
  }

  throw std::runtime_error("Attempting to insert a list of objects of unrecognized type!");
}

//
void intersection1(Gps_on_surface_base_2& ps,
                   py::list& pgn_lst, py::list& pwh_lst) {
  using Pgn = General_polygon_2;
  using Pwh = General_polygon_with_holes_2;
  auto begin0 = stl_forward_iterator<Pgn>(pgn_lst);
  auto end0 = stl_forward_iterator<Pgn>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<Pwh>(pwh_lst);
  auto end1 = stl_forward_iterator<Pwh>(pwh_lst, false);
  ps.intersection(begin0, end0, begin1, end1);
}

//
void intersection2(Gps_on_surface_base_2& ps, py::list& lst) {
  if (lst.size() == 0) return;
  using Pgn = General_polygon_2;
  if (py::isinstance<Pgn>(lst[0])) {
    auto begin = stl_forward_iterator<Pgn>(lst);
    auto end = stl_forward_iterator<Pgn>(lst, false);
    ps.intersection(begin, end);
    return;
  }

  using Pwh = General_polygon_with_holes_2;
  if (py::isinstance<Pwh>(lst[0])) {
    auto begin = stl_forward_iterator<Pwh>(lst);
    auto end = stl_forward_iterator<Pwh>(lst, false);
    ps.intersection(begin, end);
    return;
  }

  throw std::runtime_error("Attempting to intersect a list of objects of unrecognized type!");
}

//
void join1(Gps_on_surface_base_2& ps, py::list& pgn_lst, py::list& pwh_lst) {
  using Pgn = General_polygon_2;
  using Pwh = General_polygon_with_holes_2;
  auto begin0 = stl_forward_iterator<Pgn>(pgn_lst);
  auto end0 = stl_forward_iterator<Pgn>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<Pwh>(pwh_lst);
  auto end1 = stl_forward_iterator<Pwh>(pwh_lst, false);
  ps.join(begin0, end0, begin1, end1);
}

//
void join2(Gps_on_surface_base_2& ps, py::list& lst) {
  if (lst.size() == 0) return;
  using Pgn = General_polygon_2;
  if (py::isinstance<Pgn>(lst[0])) {
    auto begin = stl_forward_iterator<Pgn>(lst);
    auto end = stl_forward_iterator<Pgn>(lst, false);
    ps.join(begin, end);
    return;
  }

  using Pwh = General_polygon_with_holes_2;
  if (py::isinstance<Pwh>(lst[0])) {
    auto begin = stl_forward_iterator<Pwh>(lst);
    auto end = stl_forward_iterator<Pwh>(lst, false);
    ps.join(begin, end);
    return;
  }

  throw std::runtime_error("Attempting to join a list of objects of unrecognized type!");
}

//
void symmetric_difference1(Gps_on_surface_base_2& ps,
                           py::list& pgn_lst, py::list& pwh_lst) {
  using Pgn = General_polygon_2;
  using Pwh = General_polygon_with_holes_2;
  auto begin0 = stl_forward_iterator<Pgn>(pgn_lst);
  auto end0 = stl_forward_iterator<Pgn>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<Pwh>(pwh_lst);
  auto end1 = stl_forward_iterator<Pwh>(pwh_lst, false);
  ps.symmetric_difference(begin0, end0, begin1, end1);
}

//
void symmetric_difference2(Gps_on_surface_base_2& ps, py::list& lst) {
  if (lst.size() == 0) return;
  using Pgn = General_polygon_2;
  if (py::isinstance<Pgn>(lst[0])) {
    auto begin = stl_forward_iterator<Pgn>(lst);
    auto end = stl_forward_iterator<Pgn>(lst, false);
    return ps.symmetric_difference(begin, end);
  }

  using Pwh = General_polygon_with_holes_2;
  if (py::isinstance<Pwh>(lst[0])) {
    auto begin = stl_forward_iterator<Pwh>(lst);
    auto end = stl_forward_iterator<Pwh>(lst, false);
    return ps.symmetric_difference(begin, end);
  }

  throw std::runtime_error("Attempting to compute the symmetric difference of a list of objects of unrecognized type!");
}

//
bool do_intersect1(Gps_on_surface_base_2& ps,
                   py::list& pgn_lst, py::list& pwh_lst) {
  using Pgn = General_polygon_2;
  using Pwh = General_polygon_with_holes_2;
  auto begin0 = stl_forward_iterator<Pgn>(pgn_lst);
  auto end0 = stl_forward_iterator<Pgn>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<Pwh>(pwh_lst);
  auto end1 = stl_forward_iterator<Pwh>(pwh_lst, false);
  return ps.do_intersect(begin0, end0, begin1, end1);
}

//
bool do_intersect2(Gps_on_surface_base_2& ps, py::list& lst) {
  if (lst.size() == 0) return false;
  using Pgn = General_polygon_2;
  if (py::isinstance<Pgn>(lst[0])) {
    auto begin = stl_forward_iterator<Pgn>(lst);
    auto end = stl_forward_iterator<Pgn>(lst, false);
    return ps.do_intersect(begin, end);
  }

  using Pwh = General_polygon_with_holes_2;
  if (py::isinstance<Pwh>(lst[0])) {
    auto begin = stl_forward_iterator<Pwh>(lst);
    auto end = stl_forward_iterator<Pwh>(lst, false);
    return ps.do_intersect(begin, end);
  }

  throw std::runtime_error("Attempting to test for intersection a list of objects of unrecognized type!");
  return false;
}

}

/*! Wrap General_polygon_set and its base classes.
 * Observe that overload_cast() cannot be used when at least one overload is a
 * template member function.
 */
void export_general_polygon_set_2(py::module_& m) {
  using Gpsb2 = bso2::Gps_on_surface_base_2;
  using Gpsos2 = bso2::General_polygon_set_on_surface_2;
  using Gps2 = bso2::General_polygon_set_2;
  using GT = bso2::Geometry_traits_2;
  using Pgn = bso2::General_polygon_2;
  using Pwh = bso2::General_polygon_with_holes_2;
  using Arrangement_2 = bso2::Arrangement_2;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Gpsb2> gpsb2_c(m, "Gps_on_surface_base_2");
  gpsb2_c.def(py::init<>())
    .def(py::init<const Gpsb2&>())
    .def(py::init<const GT&>())
    .def("simplify", &Gpsb2::simplify)

    // Complement
    .def("complement", py::overload_cast<>(&Gpsb2::complement))
    .def("complement", py::overload_cast<const Gpsb2&>(&Gpsb2::complement))

    // insert
    .def("insert", static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::insert))
    .def("insert", static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::insert))
    .def("insert", &bso2::insert1)
    .def("insert", &bso2::insert2)

    // do_intersect
    .def("do_intersect",
         static_cast<bool(Gpsb2::*)(const Pgn&)const>(&Gpsb2::do_intersect))
    .def("do_intersect",
         static_cast<bool(Gpsb2::*)(const Pwh&)const>(&Gpsb2::do_intersect))
    .def("do_intersect",
         static_cast<bool(Gpsb2::*)(const Gpsb2&)const>(&Gpsb2::do_intersect))
    .def("do_intersect", &bso2::do_intersect1)
    .def("do_intersect", &bso2::do_intersect2)

    // intersection
    .def("intersection",
         static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::intersection))
    .def("intersection",
         static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::intersection))
    .def("intersection",
         static_cast<void(Gpsb2::*)(const Gpsb2&)>(&Gpsb2::intersection))
    .def("intersection",
         static_cast<void(Gpsb2::*)(const Gpsb2&, const Gpsb2&)>(&Gpsb2::intersection))
    .def("intersection", &bso2::intersection1)
    .def("intersection", &bso2::intersection2)

    // join
    .def("join", static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::join))
    .def("join", static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::join))
    .def("join", static_cast<void(Gpsb2::*)(const Gpsb2&)>(&Gpsb2::join))
    .def("join", static_cast<void(Gpsb2::*)(const Gpsb2&, const Gpsb2&)>(&Gpsb2::join))
    .def("join", &bso2::join1)
    .def("join", &bso2::join2)

    // difference
    .def("difference", py::overload_cast<const Pgn&>(&Gpsb2::difference))
    .def("difference", py::overload_cast<const Pwh&>(&Gpsb2::difference))
    .def("difference", py::overload_cast<const Gpsb2&>(&Gpsb2::difference))
    .def("difference", py::overload_cast<const Gpsb2&, const Gpsb2&>(&Gpsb2::difference))

    // symmetric_difference
    .def("symmetric_difference",
         static_cast<void(Gpsb2::*)(const Gpsb2&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference",
         static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference",
         static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference",
         static_cast<void(Gpsb2::*)(const Gpsb2&, const Gpsb2&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference", &bso2::symmetric_difference1)
    .def("symmetric_difference", &bso2::symmetric_difference2)

    .def("number_of_polygons_with_holes",
         &Gpsb2::number_of_polygons_with_holes,
         bso2_doc::General_polygon_set_2_number_of_polygons_with_holes)
    .def("is_empty", &Gpsb2::is_empty,
         bso2_doc::General_polygon_set_2_is_empty)
    .def("is_plane", &Gpsb2::is_plane,
         bso2_doc::General_polygon_set_2_is_plane)
    .def("clear", &Gpsb2::clear,
         bso2_doc::General_polygon_set_2_clear)

    // oriented_side
    .def("oriented_side",
         py::overload_cast<const GT::Point_2&>(&Gpsb2::oriented_side, py::const_))
    .def("oriented_side",
         py::overload_cast<const Pgn&>(&Gpsb2::oriented_side, py::const_))
    .def("oriented_side",
         py::overload_cast<const Pwh&>(&Gpsb2::oriented_side, py::const_))
    .def("oriented_side",
         py::overload_cast<const Gpsb2&>(&Gpsb2::oriented_side, py::const_))

    .def("locate", &Gpsb2::locate)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("arrangement_mutable", py::overload_cast<>(&Gpsb2::arrangement), ri)
    .def("arrangement", py::overload_cast<>(&Gpsb2::arrangement, py::const_), ri,
         bso2_doc::General_polygon_set_2_arrangement)

    .def("is_valid", &Gpsb2::is_valid,
         bso2_doc::General_polygon_set_2_is_valid)
    .def("polygons_with_holes", &bso2::polygons_with_holes,
         bso2_doc::General_polygon_set_2_polygons_with_holes)
    ;

  py::class_<Gpsos2, Gpsb2> gpsos2_c(m, "General_polygon_set_on_surface_2");
  gpsos2_c.def(py::init<>())
    .def(py::init<const Gpsos2&>())
    .def(py::init<const GT&>())
    ;

  py::class_<Gps2, Gpsos2> gps2_c(
    m, "General_polygon_set_2", bso2_doc::General_polygon_set_2_class);
  gps2_c.def(py::init<>(),
      bso2_doc::General_polygon_set_2_General_polygon_set_2)
    .def(py::init<const Gps2&>(), py::arg("other"),
         bso2_doc::General_polygon_set_2_General_polygon_set_2_1)
    .def(py::init<const GT&>(), py::arg("traits"),
         bso2_doc::General_polygon_set_2_General_polygon_set_2_2)
    .def(py::init<const Pgn&>(), py::arg("polygon"),
         bso2_doc::General_polygon_set_2_General_polygon_set_2_3)
    .def(py::init<const Pwh&>(), py::arg("polygon_with_holes"),
         bso2_doc::General_polygon_set_2_General_polygon_set_2_4)
    .def(py::init<const Pgn&, const GT&>(),
         py::arg("polygon"), py::arg("traits"),
         bso2_doc::General_polygon_set_2_General_polygon_set_2_5)
    .def(py::init<const Pwh&, const GT&>(),
         py::arg("polygon_with_holes"), py::arg("traits"),
         bso2_doc::General_polygon_set_2_General_polygon_set_2_6)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("arrangement_mutable", py::overload_cast<>(&Gps2::arrangement), ri)
    .def("arrangement", py::overload_cast<>(&Gps2::arrangement, py::const_),
         bso2_doc::General_polygon_set_2_arrangement)
    ;

  // Types that have been registered already:
  if (! add_attr<GT>(gps2_c, "Geometry_traits_2"))
    std::cerr << "bso2::Geometry_traits_2 not registered!\n";
  if (! add_attr<GT>(gps2_c, "Traits_2"))
    std::cerr << "bso2::Traits_2 not registered!\n";
  if (! add_attr<Pgn>(gps2_c, "Polygon_2"))
    std::cerr << "bso2::General_polygon_2 not registered!\n";
  if (! add_attr<Pwh>(gps2_c, "Polygon_with_holes_2"))
    std::cerr << "bso2::General_polygon_with_holes_2 not registered!\n";
  if (! add_attr<bso2::Arrangement_2>(gps2_c, "Arrangement_2"))
    std::cerr << "bso2::Arrangement_2 not registered!\n";

  add_insertion(gps2_c, "__str__");
  add_insertion(gps2_c, "__repr__");

  // Support extraction only for geometry traits that do support extraction:
  // using Cs_pgn = CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_2;
  // using Nt_traits = CGAL::CORE_algebraic_number_traits;
  // using Rational = Nt_traits::Rational;
  // using Algebraic = Nt_traits::Algebraic;
  // using Rat_kernel = CGAL::Cartesian<Rational>;
  // using Alg_kernel = CGAL::Cartesian<Algebraic>;
  // using Conic_traits = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  // using Conic_pgn = CGAL::Gps_traits_2<Conic_traits>::Polygon_2;
  // using Bezier_traits = CGAL::Arr_Bezier_curve_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  // using Bezier_pgn = CGAL::Gps_traits_2<Bezier_traits>::Polygon_2;
  // None of the traits above support extraction....
  // if constexpr ((std::is_same<Gpgn, Cs_pgn>::value) ||
  //               (std::is_same<Gpgn, Conic_pgn>::value)
  //               (std::is_same<Gpgn, Bezier_pgn>::value))
  //   add_extraction(gps2_c);
}
