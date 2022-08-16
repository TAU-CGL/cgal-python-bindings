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

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/append_iterator.hpp"

namespace py = nanobind;

namespace bso2 {

py::list polygons_with_holes(Gps_on_surface_base_2& ps) {
  py::list lst;
  ps.polygons_with_holes(append_iterator(lst));
  return lst;
}

// void insert_range0(Gps_on_surface_base_2& ps, py::list& polygon_lst,
//                    py::list& pwh_lst) {
//   auto begin0 = py::stl_input_iterator<General_polygon_2>(polygon_lst);
//   auto end0 = py::stl_input_iterator<General_polygon_2>();
//   auto begin1 = py::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
//   auto end1 = py::stl_input_iterator<General_polygon_with_holes_2>();
//   auto v0 = std::vector<General_polygon_2>(begin0, end0);
//   auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);

//   ps.insert(v0.begin(), v0.end(), v1.begin(), v1.end());
// }

// template <typename T>
// void insert_range(Gps_on_surface_base_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.insert(v.begin(), v.end());
// }

// void intersection_range0(Gps_on_surface_base_2& ps, py::list& polygon_lst,
//                          py::list& pwh_lst) {
//   auto begin0 = py::stl_input_iterator<General_polygon_2>(polygon_lst);
//   auto end0 = py::stl_input_iterator<General_polygon_2>();
//   auto begin1 = py::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
//   auto end1 = py::stl_input_iterator<General_polygon_with_holes_2>();
//   auto v0 = std::vector<General_polygon_2>(begin0, end0);
//   auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
//   ps.intersection(v0.begin(), v0.end(), v1.begin(), v1.end());
// }

// template <typename T>
// void intersection_range(Gps_on_surface_base_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.intersection(v.begin(), v.end());
// }

// void join_range0(Gps_on_surface_base_2& ps, py::list& polygon_lst,
//                  py::list& pwh_lst) {
//   auto begin0 = py::stl_input_iterator<General_polygon_2>(polygon_lst);
//   auto end0 = py::stl_input_iterator<General_polygon_2>();
//   auto begin1 = py::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
//   auto end1 = py::stl_input_iterator<General_polygon_with_holes_2>();
//   auto v0 = std::vector<General_polygon_2>(begin0, end0);
//   auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
//   ps.join(v0.begin(), v0.end(), v1.begin(), v1.end());
// }

// template <typename T>
// void join_range(Gps_on_surface_base_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.join(v.begin(), v.end());
// }

// void symmetric_difference_range0(Gps_on_surface_base_2& ps,
//                                  py::list& polygon_lst, py::list& pwh_lst) {
//   auto begin0 = py::stl_input_iterator<General_polygon_2>(polygon_lst);
//   auto end0 = py::stl_input_iterator<General_polygon_2>();
//   auto begin1 = py::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
//   auto end1 = py::stl_input_iterator<General_polygon_with_holes_2>();
//   auto v0 = std::vector<General_polygon_2>(begin0, end0);
//   auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
//   ps.symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end());
// }

// template <typename T>
// void symmetric_difference_range(Gps_on_surface_base_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.symmetric_difference(v.begin(), v.end());
// }

// bool do_intersect_range0(Gps_on_surface_base_2& ps,
//                          py::list& polygon_lst, py::list& pwh_lst) {
//   auto begin0 = py::stl_input_iterator<General_polygon_2>(polygon_lst);
//   auto end0 = py::stl_input_iterator<General_polygon_2>();
//   auto begin1 = py::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
//   auto end1 = py::stl_input_iterator<General_polygon_with_holes_2>();
//   auto v0 = std::vector<General_polygon_2>(begin0, end0);
//   auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
//   return ps.do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
// }

// template <typename T>
// void do_intersect_range(Gps_on_surface_base_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.do_intersect(v.begin(), v.end());
// }

template<typename T>
CGAL::Oriented_side oriented_side(Gps_on_surface_base_2& ps, const T& other)
{ return ps.oriented_side(other); }

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

  py::class_<Gpsb2> gpsb2_co(m, "Gps_on_surface_base_2");
  gpsb2_co.def(py::init<>())
    .def(py::init<const Gpsb2&>())
    .def(py::init<const GT&>())
    .def("simplify", &Gpsb2::simplify)

    // Complement
    .def("complement", py::overload_cast<>(&Gpsb2::complement))
    .def("complement", py::overload_cast<const Gpsb2&>(&Gpsb2::complement))

    // insert
    .def("insert", static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::insert))
    .def("insert", static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::insert))
    // .def("insert", &bso2::insert_range0)
    // .def("insert_polygons", &bso2::insert_range<Pgn>)
    // .def("insert_polygons_with_holes", &bso2::insert_range<Pwh>)

    // do_intersect
    .def("do_intersect", static_cast<bool(Gpsb2::*)(const Pgn&)const>(&Gpsb2::do_intersect))
    .def("do_intersect", static_cast<bool(Gpsb2::*)(const Pwh&)const>(&Gpsb2::do_intersect))
    .def("do_intersect", static_cast<bool(Gpsb2::*)(const Gpsb2&)const>(&Gpsb2::do_intersect))
    // .def("do_intersect", &bso2::do_intersect_range0)
    // .def("do_intersect_polygons", &bso2::do_intersect_range<Pgn>)
    // .def("do_intersect_polygons_with_holes", &bso2::do_intersect_range<Pwh>)

    // intersection
    .def("intersection", static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::intersection))
    .def("intersection", static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::intersection))
    .def("intersection", static_cast<void(Gpsb2::*)(const Gpsb2&)>(&Gpsb2::intersection))
    .def("intersection", static_cast<void(Gpsb2::*)(const Gpsb2&, const Gpsb2&)>(&Gpsb2::intersection))
    // .def("intersection", &bso2::intersection_range0)
    // .def("intersection_polygons", &bso2::intersection_range<Pgn>)
    // .def("intersection_polygons_with_holes", &bso2::intersection_range<Pwh>)

    // join
    .def("join", static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::join))
    .def("join", static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::join))
    .def("join", static_cast<void(Gpsb2::*)(const Gpsb2&)>(&Gpsb2::join))
    .def("join", static_cast<void(Gpsb2::*)(const Gpsb2&, const Gpsb2&)>(&Gpsb2::join))
    // .def("join", &bso2::join_range0)
    // .def("join_polygons", &bso2::join_range<Pgn>)
    // .def("join_polygons_with_holes", &bso2::join_range<Pwh>)

    // difference
    .def("difference", py::overload_cast<const Pgn&>(&Gpsb2::difference))
    .def("difference", py::overload_cast<const Pwh&>(&Gpsb2::difference))
    .def("difference", py::overload_cast<const Gpsb2&>(&Gpsb2::difference))
    .def("difference", py::overload_cast<const Gpsb2&, const Gpsb2&>(&Gpsb2::difference))

    // symmetric_difference
    .def("symmetric_difference", static_cast<void(Gpsb2::*)(const Gpsb2&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference", static_cast<void(Gpsb2::*)(const Pgn&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference", static_cast<void(Gpsb2::*)(const Pwh&)>(&Gpsb2::symmetric_difference))
    .def("symmetric_difference", static_cast<void(Gpsb2::*)(const Gpsb2&, const Gpsb2&)>(&Gpsb2::symmetric_difference))
    // .def("symmetric_difference", &bso2::symmetric_difference_range0)
    // .def("symmetric_difference_polygons", &bso2::symmetric_difference_range<Pgn>)
    // .def("symmetric_difference_polygons_with_holes", &bso2::symmetric_difference_range<Pwh>)

    .def("number_of_polygons_with_holes", &Gpsb2::number_of_polygons_with_holes)
    .def("is_empty", &Gpsb2::is_empty)
    .def("is_plane", &Gpsb2::is_plane)
    .def("clear", &Gpsb2::clear)

    // oriented_side
    .def("oriented_side", &bso2::oriented_side<const GT::Point_2&>)
    .def("oriented_side", &bso2::oriented_side<const Gpsb2&>)
    .def("oriented_side", &bso2::oriented_side<const Pgn&>)
    .def("oriented_side", &bso2::oriented_side<const Pwh&>)

    .def("locate", &Gpsb2::locate)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("arrangement_mutable", py::overload_cast<>(&Gpsb2::arrangement),
         py::rv_policy::reference_internal)
    .def("arrangement", py::overload_cast<>(&Gpsb2::arrangement, py::const_))

    .def("is_valid", &Gpsb2::is_valid)
    .def("polygons_with_holes", &bso2::polygons_with_holes)
    ;

  py::class_<Gpsos2, Gpsb2> gpsos2_co(m, "General_polygon_set_on_surface_2");
  gpsos2_co.def(py::init<>())
    .def(py::init<const Gpsos2&>())
    .def(py::init<const GT&>())
    ;

  py::class_<Gps2, Gpsos2> gps2_co(m, "General_polygon_set_2");
  gps2_co.def(py::init<>())
    .def(py::init<const Gps2&>())
    .def(py::init<const GT&>())
    .def(py::init<const Pgn&>())
    .def(py::init<const Pwh&>())
    .def(py::init<const Pgn&, const GT&>())
    .def(py::init<const Pwh&, const GT&>())

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("arrangement_mutable", py::overload_cast<>(&Gps2::arrangement),
         py::rv_policy::reference_internal)
    .def("arrangement", py::overload_cast<>(&Gps2::arrangement, py::const_))
    ;

  // Types that have been registered already:
  if (! add_attr<GT>("Geometry_traits_2", gps2_co))
    std::cerr << "bso2::Geometry_traits_2 not registered!\n";
  if (! add_attr<GT>("Traits_2", gps2_co))
    std::cerr << "bso2::Traits_2 not registered!\n";
  if (! add_attr<Pgn>("Polygon_2", gps2_co))
    std::cerr << "bso2::General_polygon_2 not registered!\n";
  if (! add_attr<Pwh>("Polygon_with_holes_2", gps2_co))
    std::cerr << "bso2::General_polygon_with_holes_2 not registered!\n";
  if (! add_attr<bso2::Arrangement_2>("Arrangement_2", gps2_co))
    std::cerr << "bso2::Arrangement_2 not registered!\n";
}
