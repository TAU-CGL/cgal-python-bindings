// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/intersections_d.h>

#include "CGALPY/common.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/export_ft.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

extern void export_gmpz(py::module_& m);
extern void export_gmpq(py::module_& m);

void init_point_d(Point_d& pd, int d, py::list& lst) {
  auto begin = stl_input_iterator<FT_d>(lst);
  auto end = stl_input_iterator<FT_d>(lst, false);
#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
  new (&pd) Point_d(d, begin, end);
#else
  // Workaround a bug in CGAL
  std::list<FT_d> tmp(begin, end);
  new (&pd) Point_d(d, tmp.begin(), tmp.end());
#endif
}

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_epec_d_type() {
  return ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||
          (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ));
}

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2)
// are not a valid overload for do_intersect in which case the second version
// (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_d_2T(py::module_& m,
                            decltype(CGAL::do_intersect<Kernel_d>(T1(), T2())))
{
  m.def<bool(const T1&, const T2&)>("do_intersect", &CGAL::do_intersect<Kernel_d>);
}

template<typename, typename>
void bind_do_intersect_d_2T(py::module_& m, ...) {}

template <typename T>
void bind_do_intersect_d_1T(py::module_& m) {
  bind_do_intersect_d_2T<T, Point_d>(m, true);
  bind_do_intersect_d_2T<T, Segment_d>(m, true);
  // bind_do_intersect_d_2T<T, Line_d>(m, true);
  // bind_do_intersect_d_2T<T, Ray_d>(m, true);
  // bind_do_intersect_d_2T<T, Triangle_d>(m, true);
  // bind_do_intersect_d_2T<T, Iso_rectangle_d>(m, true);
  // bind_do_intersect_d_2T<T, Circle_d>(m, true);
}

void bind_do_intersect_d(py::module_& m) {
  bind_do_intersect_d_1T<Point_d>(m);
  bind_do_intersect_d_1T<Segment_d>(m);
  // bind_do_intersect_d_1T<Line_d>(m);
  // bind_do_intersect_d_1T<Ray_d>(m);
  // bind_do_intersect_d_1T<Triangle_d>(m);
  // bind_do_intersect_d_1T<Iso_rectangle_d>(m);
  // bind_do_intersect_d_1T<Circle_d>(m);
}

void export_kernel_d(py::module_& m) {
  using Pnt = Point_d;

#if ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||                     \
     (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ))
  if (! add_attr<CGAL::Gmpz>(m, "Gmpz")) export_gmpz(m);
  if (! add_attr<CGAL::Gmpq>(m, "Gmpq")) export_gmpq(m);
  if (! add_attr<FT_d>(m, "FT")) {
    auto ftc = py::class_<FT_d>(m, "FT");
    export_ft<FT_d>(ftc);
  }
#endif

  py::class_<Pnt> pd_c(m, "Point_d");
  pd_c.def(py::init<>())
    .def("__init__", &init_point_d)
    .def("__hash__", &hash_rational_point<is_epec_d_type(), Pnt>)
    .def("dimension", &Pnt::dimension)
    .def("cartesian", &Pnt::cartesian)
    .def("__getitem__", &Pnt::operator[])
    .def(py::self == py::self)
    .def(py::self != py::self)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
#endif
    ;

  using Cci = Pnt::Cartesian_const_iterator;
  add_iterator<Cci, Cci, const FT_d&>("Cartesian_iterator", pd_c);
  pd_c.def("cartesians",
            [](const Pnt& p)
            { return make_iterator(p.cartesian_begin(), p.cartesian_end()); },
           py::keep_alive<0, 1>());

  add_insertion(pd_c, "__str__");
  add_insertion(pd_c, "__repr__");

  py::class_<Segment_d> sd_co(m, "Segment_d");
  sd_co.def(py::init<Point_d&, Point_d&>())
    .def("source", &Segment_d::source)
    .def("target", &Segment_d::target)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def("opposite", &Segment_d::opposite)
    .def("__getitem__", &Segment_d::operator[])
#endif
#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
    .def("vertex", &Segment_d::vertex)
    .def("point", &Segment_d::point)
    .def("min", &Segment_d::min)
    .def("max", &Segment_d::max)
    .def("supporting_line", &Segment_d::supporting_line)
    .def("squared_length", &Segment_d::squared_length)
    .def("direction", &Segment_d::direction)
    .def("has_on", &Segment_d::has_on)
    .def("is_degenerate", &Segment_d::is_degenerate)
    .def(py::self == py::self)
    .def(py::self != py::self)
#endif
    // .setattr("__hash__", &hash<Segment_d>)
    ;

  add_insertion(sd_co, "__str__");
  add_insertion(sd_co, "__repr__");

  bind_do_intersect_d(m);
}
