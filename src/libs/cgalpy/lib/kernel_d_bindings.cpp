// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/python.hpp>

#include <CGAL/intersections_d.h>

#include "CGALPY/common.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/export_ft.hpp"

namespace bp = boost::python;

extern void export_gmpz();
extern void export_gmpq();

Point_d* init_point_d(int d, bp::list& lst) {
  auto begin = bp::stl_input_iterator<FT_d>(lst);
  auto end = bp::stl_input_iterator<FT_d>();
  return new Point_d(d, begin, end);
}

#if ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||                     \
     (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ))
const FT_d* point_d_cartesian_begin(Point_d& p)
{ return p.cartesian_begin(); }

const FT_d* point_d_cartesian_end(Point_d& p)
{ return p.cartesian_end(); }
#endif

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_epec_d_type() {
  return ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||
          (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ));
}

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2) are not a valid overload for do_intersect
// in which case the second version (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_d_2T(decltype(CGAL::do_intersect<Kernel_d>(T1(), T2())))
{
  bp::def<bool(const T1&, const T2&)>("do_intersect", &CGAL::do_intersect<Kernel_d>);
}

template<typename, typename>
void bind_do_intersect_d_2T(...) {}

template <typename T>
void bind_do_intersect_d_1T() {
  bind_do_intersect_d_2T<T, Point_d>(true);
  bind_do_intersect_d_2T<T, Segment_d>(true);
  // bind_do_intersect_d_2T<T, Line_d>(true);
  // bind_do_intersect_d_2T<T, Ray_d>(true);
  // bind_do_intersect_d_2T<T, Triangle_d>(true);
  // bind_do_intersect_d_2T<T, Iso_rectangle_d>(true);
  // bind_do_intersect_d_2T<T, Circle_d>(true);
}

void bind_do_intersect_d() {
  bind_do_intersect_d_1T<Point_d>();
  bind_do_intersect_d_1T<Segment_d>();
  // bind_do_intersect_d_1T<Line_d>();
  // bind_do_intersect_d_1T<Ray_d>();
  // bind_do_intersect_d_1T<Triangle_d>();
  // bind_do_intersect_d_1T<Iso_rectangle_d>();
  // bind_do_intersect_d_1T<Circle_d>();
}

void export_kernel_d() {
  const bp::type_info info_gmpz = bp::type_id<CGAL::Gmpz>();
  const auto* reg_gmpz = bp::converter::registry::query(info_gmpz);
  if ((reg_gmpz == nullptr) || ((*reg_gmpz).m_to_python == nullptr))
    export_gmpz();
  else bp::scope().attr("Gmpz") = bp::handle<>(reg_gmpz->m_class_object);

  const bp::type_info info_gmpq = bp::type_id<CGAL::Gmpq>();
  const auto* reg_gmpq = bp::converter::registry::query(info_gmpq);
  if ((reg_gmpq == nullptr) || ((*reg_gmpq).m_to_python == nullptr))
    export_gmpq();
  else bp::scope().attr("Gmpq") = bp::handle<>(reg_gmpq->m_class_object);

#if ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||                     \
     (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ))
  const bp::type_info info_ftd = bp::type_id<FT_d>();
  const auto* reg_ftd = bp::converter::registry::query(info_ftd);
  if ((reg_ftd == nullptr) || ((*reg_ftd).m_to_python == nullptr)) {
    auto ftc = bp::class_<FT_d>("FT");
    export_ft<FT_d>(ftc);
  }
  else bp::scope().attr("FT") = bp::handle<>(reg_ftd->m_class_object);
#endif

  bp::class_<Point_d>("Point_d")
    .def(bp::init<>())
    .def("__init__", bp::make_constructor(&init_point_d))
    .def("dimension", &Point_d::dimension)
    .def("cartesian", &Point_d::cartesian, Kernel_d_return_value_policy())
    .def("__getitem__", &Point_d::operator[], Kernel_d_return_value_policy())
#if ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||                     \
     (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ))
    .def("coordinates", bp::range<>(&point_d_cartesian_begin, &point_d_cartesian_end))
#else
    .def("coordinates", bp::range<>(&Point_d::cartesian_begin, &Point_d::cartesian_end))
#endif
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .setattr("__hash__", &Hash_rational_point<is_epec_d_type()>::operator()<Point_d>)
    ;

  bp::class_<Segment_d>("Segment_d")
    .def(bp::init<Point_d&, Point_d&>())
    .def("source", &Segment_d::source, Kernel_d_return_value_policy())
    .def("target", &Segment_d::target, Kernel_d_return_value_policy())
    .def("vertex", &Segment_d::vertex, Kernel_d_return_value_policy())
    .def("point", &Segment_d::point, Kernel_d_return_value_policy())
    .def("__getitem__", &Segment_d::operator[], Kernel_d_return_value_policy())
    .def("min", &Segment_d::min, Kernel_d_return_value_policy())
    .def("max", &Segment_d::max, Kernel_d_return_value_policy())
    .def("opposite", &Segment_d::opposite)
    .def("supporting_line", &Segment_d::supporting_line)
    .def("squared_length", &Segment_d::squared_length)
    .def("direction", &Segment_d::direction)
    .def("has_on", &Segment_d::has_on)
    .def("is_degenerate", &Segment_d::is_degenerate)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    // .setattr("__hash__", &hash<Segment_d>)
    ;

  bind_do_intersect_d();
}
