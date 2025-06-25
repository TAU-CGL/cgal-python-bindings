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

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/config.hpp"
#include "CGALPY/Kernel/export_ft.hpp"
#include "CGALPY/kernel_d_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

extern void export_gmpz(py::module_& m);
extern void export_gmpq(py::module_& m);
extern void export_point_d(py::class_<Point_d>& pntd_c);
extern void export_segment_d(py::class_<Segment_d>& segd_c);
extern void export_linear_algebra_cd(py::class_<Linear_algebra_cartesian_d>& lacd_c);
extern void export_vector_d(py::class_<Vector_d>& vecd_c);

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2)
// are not a valid overload for do_intersect in which case the second version
// (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_d_2T(py::module_& m,
                            decltype(CGAL::do_intersect<Kernel_d>(T1(), T2())))
{
  m.def("do_intersect",
        static_cast<bool(*)(const T1&, const T2&)>(&CGAL::do_intersect<Kernel_d>));
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
  using Kerd = Kernel_d;
  using Pntd = Point_d;
  using Segd = Segment_d;
  using Vecd = Vector_d;
  using Lacd = Linear_algebra_cartesian_d;

  // Point
  if (! add_attr<Pntd>(m, "Point_d")) {
    py::class_<Pntd> pntd_c(m, "Point_d");
    export_point_d(pntd_c);
  }

  // Segment
  if (! add_attr<Segd>(m, "Segment_d")) {
    py::class_<Segd> segd_c(m, "Segment_d");
    export_segment_d(segd_c);
  }

  // Vector
  if (! add_attr<Vecd>(m, "Vecotr_d")) {
    py::class_<Vecd> vecd_c(m, "Vector_d");
    export_vector_d(vecd_c);
  }

  // Kernel
  if (! add_attr<Kerd>(m, "Kernel_d")) {
    using Ctr_vec_d = Kerd::Construct_vector_d;

    py::class_<Kerd> kerd_c(m, "Kernel_d");
    kerd_c.def(py::init<>())
      .def("construct_vector_d_object", [](const Kerd& k)->Ctr_vec_d{ return k.construct_vector_d_object(); })
      ;

    // Construct_vector_d
    if (! add_attr<Ctr_vec_d>(kerd_c, "Construct_vector_d")) {
      using Ctr1 = Vecd(Ctr_vec_d::*)(const Pntd&, const Pntd&) const;
      py::class_<Ctr_vec_d>(kerd_c, "Construct_vector_d")
        //! \todo the following fails for clang
        // .def("__call__", static_cast<Ctr1>(&Ctr_vec_d::operator()))
        .def("__call__", [](Ctr_vec_d& ctr, const Pntd& p1, const Pntd& p2)->Vecd{ return ctr(p1, p2); })
        ;
    }
  }

#if ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||                     \
     (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ))
  if (! add_attr<CGAL::Gmpz>(m, "Gmpz")) export_gmpz(m);
  if (! add_attr<CGAL::Gmpq>(m, "Gmpq")) export_gmpq(m);
  if (! add_attr<FT_d>(m, "FT")) {
    auto ftc = py::class_<FT_d>(m, "FT");
    export_ft(ftc);
  }
#endif

  // Linear_algebraCd
  if (! add_attr<Lacd>(m, "Linear_algebraCd")) {
    py::class_<Lacd> lacd_c(m, "Linear_algebraCd");
    export_linear_algebra_cd(lacd_c);
  }

  bind_do_intersect_d(m);
}
