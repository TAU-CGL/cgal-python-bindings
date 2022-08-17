// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <nanobind/nanobind.h>

#include <CGAL/intersections_d.h>

#include "CGALPY/common.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/export_ft.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

extern void export_gmpz();
extern void export_gmpq();

Point_d* init_point_d(int d, py::list& lst) {
  auto begin = py::stl_input_iterator<FT_d>(lst);
  auto end = py::stl_input_iterator<FT_d>();
#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
  return new Point_d(d, begin, end);
#else
  // Workaround a bug in CGAL
  std::list<FT_d> tmp(begin, end);
  return new Point_d(d, tmp.begin(), tmp.end());
#endif
}

// const FT_d* point_d_cartesian_begin(Point_d& p)
// { return p.cartesian_begin(); }

// const FT_d* point_d_cartesian_end(Point_d& p)
// { return p.cartesian_end(); }

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_epec_d_type() {
  return ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||
          (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ));
}

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2) are not a valid overload for do_intersect
// in which case the second version (which does nothing) will be used instead (SFINAE)
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
#if ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||                     \
     (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ))
  const py::type_info info_gmpz = py::type_id<CGAL::Gmpz>();
  const auto* reg_gmpz = py::converter::registry::query(info_gmpz);
  if ((reg_gmpz == nullptr) || ((*reg_gmpz).m_to_python == nullptr))
    export_gmpz();
  else py::scope().attr("Gmpz") = py::handle<>(reg_gmpz->m_class_object);

  const py::type_info info_gmpq = py::type_id<CGAL::Gmpq>();
  const auto* reg_gmpq = py::converter::registry::query(info_gmpq);
  if ((reg_gmpq == nullptr) || ((*reg_gmpq).m_to_python == nullptr))
    export_gmpq();
  else py::scope().attr("Gmpq") = py::handle<>(reg_gmpq->m_class_object);

  const py::type_info info_ftd = py::type_id<FT_d>();
  const auto* reg_ftd = py::converter::registry::query(info_ftd);
  if ((reg_ftd == nullptr) || ((*reg_ftd).m_to_python == nullptr)) {
    auto ftc = py::class_<FT_d>("FT");
    export_ft<FT_d>(ftc);
  }
  else py::scope().attr("FT") = py::handle<>(reg_ftd->m_class_object);
#endif

  py::class_<Point_d> pd_co(m, "Point_d");
  pd_co.def(py::init<>())
    .def("__init__", py::make_constructor(&init_point_d))
    .def("dimension", &Point_d::dimension)
    .def("cartesian", &Point_d::cartesian, Kernel_d_return_value_policy())
    .def("__getitem__", &Point_d::operator[], Kernel_d_return_value_policy())
    .def("coordinates", py::range<>(&Point_d::cartesian_begin, &Point_d::cartesian_end))
    .def(py::self == py::self)
    .def(py::self != py::self)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
#endif
    .setattr("__hash__", &hash_rational_point<is_epec_d_type(), Point_d>)
    ;

  add_insertion(pd_co, "__str__");
  add_insertion(pd_co, "__repr__");

  py::class_<Segment_d> sd_co(m, "Segment_d");
  sd_co.def(py::init<Point_d&, Point_d&>())
    .def("source", &Segment_d::source, Kernel_d_return_value_policy())
    .def("target", &Segment_d::target, Kernel_d_return_value_policy())
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def("opposite", &Segment_d::opposite)
    .def("__getitem__", &Segment_d::operator[], Kernel_d_return_value_policy())
#endif
#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
    .def("vertex", &Segment_d::vertex, Kernel_d_return_value_policy())
    .def("point", &Segment_d::point, Kernel_d_return_value_policy())
    .def("min", &Segment_d::min, Kernel_d_return_value_policy())
    .def("max", &Segment_d::max, Kernel_d_return_value_policy())
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
