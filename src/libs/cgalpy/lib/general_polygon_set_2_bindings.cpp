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

py::list polygons_with_holes(General_polygon_set_2& ps) {
  py::list lst;
  ps.polygons_with_holes(append_iterator(lst));
  return lst;
}

void insert0(General_polygon_set_2& ps, General_polygon_2& pgn)
{ ps.insert(pgn); }

void insert1(General_polygon_set_2& ps, General_polygon_with_holes_2& pwh)
{ ps.insert(pwh); }

// void insert_range0(General_polygon_set_2& ps, py::list& polygon_lst,
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
// void insert_range(General_polygon_set_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.insert(v.begin(), v.end());
// }

void complement0(General_polygon_set_2& ps0, General_polygon_set_2& ps1)
{ ps0.complement(ps1); }

template <typename T>
void intersection(General_polygon_set_2& ps, T& other)
{ ps.intersection(other); }

void intersection(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
                  General_polygon_set_2& ps2)
{ ps0.intersection(ps1, ps2); }

// void intersection_range0(General_polygon_set_2& ps, py::list& polygon_lst,
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
// void intersection_range(General_polygon_set_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.intersection(v.begin(), v.end());
// }

template <typename T>
void join(General_polygon_set_2& ps, T& other) { ps.join(other); }

void join(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
          General_polygon_set_2& ps2)
{ ps0.join(ps1, ps2); }

// void join_range0(General_polygon_set_2& ps, py::list& polygon_lst,
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
// void join_range(General_polygon_set_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.join(v.begin(), v.end());
// }

template <typename T>
void difference(General_polygon_set_2& ps, T& other) { ps.difference(other); }

void difference(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
                General_polygon_set_2& ps2)
{ ps0.difference(ps1, ps2); }

template <typename T>
void symmetric_difference(General_polygon_set_2& ps, T& other)
{ ps.symmetric_difference(other); }

void symmetric_difference(General_polygon_set_2& ps0,
                          General_polygon_set_2& ps1,
                          General_polygon_set_2& ps2)
{ ps0.symmetric_difference(ps1, ps2); }

// void symmetric_difference_range0(General_polygon_set_2& ps,
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
// void symmetric_difference_range(General_polygon_set_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.symmetric_difference(v.begin(), v.end());
// }

template <typename T>
bool do_intersect(General_polygon_set_2& ps, T& other)
{ return ps.do_intersect(other); }

// bool do_intersect_range0(General_polygon_set_2& ps,
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
// void do_intersect_range(General_polygon_set_2& ps, py::list& lst) {
//   auto begin = py::stl_input_iterator<T>(lst);
//   auto end = py::stl_input_iterator<T>();
//   auto v = std::vector<T>(begin, end);
//   ps.do_intersect(v.begin(), v.end());
// }

template<typename T>
CGAL::Oriented_side oriented_side(General_polygon_set_2& ps, T& other)
{ return ps.oriented_side(other); }

}

void export_general_polygon_set_2(py::module_& m) {
  typedef bso2::General_polygon_set_2           Gps2;
  typedef bso2::Geometry_traits_2               GT;
  typedef bso2::General_polygon_2               Pgn;
  typedef bso2::General_polygon_with_holes_2    Pwh;
  typedef bso2::Arrangement_2                   Arrangement_2;

  py::class_<Gps2> gps2_co(m, "General_polygon_set_2");
  gps2_co.def(py::init<>())
    .def(py::init<const Pgn&>())
    .def(py::init<const Pwh&>())
    .def(py::init<const Gps2&>())
    .def("is_empty", &Gps2::is_empty)
    .def("is_plane", &Gps2::is_plane)
    .def("number_of_polygons_with_holes", &Gps2::number_of_polygons_with_holes)
    .def("polygons_with_holes", &bso2::polygons_with_holes)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("arrangement_mutable", py::overload_cast<>(&Gps2::arrangement),
         py::rv_policy::reference_internal)
    .def("arrangement", py::overload_cast<>(&Gps2::arrangement, py::const_))

    .def("clear", &Gps2::clear)
    .def("is_valid", &Gps2::is_valid)
    .def("insert", &bso2::insert0)
    .def("insert", &bso2::insert1)
    // .def("insert", &bso2::insert_range0)
    // .def("insert_polygons", &bso2::insert_range<Pgn>)
    // .def("insert_polygons_with_holes", &bso2::insert_range<Pwh>)
    .def<void(Gps2::*)()>("complement", &Gps2::complement)
    .def("complement", &bso2::complement0)
    .def("intersection", &bso2::intersection<Gps2>)
    .def("intersection", &bso2::intersection<Pgn>)
    .def("intersection", &bso2::intersection<Pwh>)
    .def<void (Gps2&, Gps2&, Gps2&)>("intersection", &bso2::intersection)
    // .def("intersection", &bso2::intersection_range0)
    // .def("intersection_polygons", &bso2::intersection_range<Pgn>)
    // .def("intersection_polygons_with_holes", &bso2::intersection_range<Pwh>)
    .def("join", &bso2::join<Gps2>)
    .def("join", &bso2::join<Pgn>)
    .def("join", &bso2::join<Pwh>)
    .def<void (Gps2&, Gps2&, Gps2&)>("join", &bso2::join)
    // .def("join", &bso2::join_range0)
    // .def("join_polygons", &bso2::join_range<Pgn>)
    // .def("join_polygons_with_holes", &bso2::join_range<Pwh>)
    .def("difference", &bso2::difference<Gps2>)
    .def("difference", &bso2::difference<Pgn>)
    .def("difference", &bso2::difference<Pwh>)
    .def<void (Gps2&, Gps2&, Gps2&)>("difference", &bso2::difference)
    .def("symmetric_difference", &bso2::symmetric_difference<Gps2>)
    .def("symmetric_difference", &bso2::symmetric_difference<Pgn>)
    .def("symmetric_difference", &bso2::symmetric_difference<Pwh>)
    .def<void (Gps2&, Gps2&, Gps2&)>("symmetric_difference", &bso2::symmetric_difference)
    // .def("symmetric_difference", &bso2::symmetric_difference_range0)
    // .def("symmetric_difference_polygons", &bso2::symmetric_difference_range<Pgn>)
    // .def("symmetric_difference_polygons_with_holes", &bso2::symmetric_difference_range<Pwh>)
    .def("do_intersect", &bso2::do_intersect<Gps2>)
    .def("do_intersect", &bso2::do_intersect<Pgn>)
    .def("do_intersect", &bso2::do_intersect<Pwh>)
    // .def("do_intersect", &bso2::do_intersect_range0)
    // .def("do_intersect_polygons", &bso2::do_intersect_range<Pgn>)
    // .def("do_intersect_polygons_with_holes", &bso2::do_intersect_range<Pwh>)
    .def("locate", &Gps2::locate)
    .def("oriented_side", &bso2::oriented_side<GT::Point_2>)
    .def("oriented_side", &bso2::oriented_side<Gps2>)
    .def("oriented_side", &bso2::oriented_side<Pgn>)
    .def("oriented_side", &bso2::oriented_side<Pwh>)
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
