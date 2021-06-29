// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/python.hpp>

#include "CGALPY/common.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/hash_rational_point.hpp"

namespace bp = boost::python;

#if (CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) || \
  (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_LAZY_GMPQ)
typename FT_d::Exact_type& FT_exact(FT_d& ft) { return ft.exact(); }

typename FT_d::Approximate_type& FT_approx(FT_d& ft) { return ft.approx(); }
#endif

double FT_to_double(FT_d& ft) { return CGAL::to_double(ft); }

static Point_d* init_point_d(int d, bp::list& lst) {
  auto begin = bp::stl_input_iterator<FT_d>(lst);
  auto end = bp::stl_input_iterator<FT_d>();
  return new Point_d(d, begin, end);
}

static const FT_d* point_d_cartesian_begin(Point_d& p)
{ return p.cartesian_begin(); }

static const FT_d* point_d_cartesian_end(Point_d& p)
{ return p.cartesian_end(); }

void export_kernel_d() {
#if (CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) || \
  (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_LAZY_GMPQ)
  bp::class_<FT_d>("FT")
    .def(bp::init<double>())
    .def(bp::init<FT_d::Exact_type>())
    .def(bp::init<FT_d>())
    .def("exact", &FT_exact, bp::return_internal_reference<>())
    //.def("approx", &FT_approx, return_internal_reference<>())
    .def("to_double", &FT_to_double)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self * bp::self)
    .def(bp::self *= bp::self)
    .def(bp::self / bp::self)
    .def(bp::self /= bp::self)
    .def(-bp::self)
    ;
#endif

  bp::class_<Point_d>("Point_d")
    .def(bp::init<>())
    .def("__init__", make_constructor(&init_point_d))
    .def("dimension", &Point_d::dimension)
    .def("cartesian", &Point_d::cartesian)
    .def("__getitem__", &Point_d::operator[])
    .def("coordinates", bp::range<>(&point_d_cartesian_begin, &point_d_cartesian_end))
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self > bp::self)
    .def(bp::self < bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self - bp::self)
    .def("__hash__", &hash_rational_point<Point_d>)
    ;
}
