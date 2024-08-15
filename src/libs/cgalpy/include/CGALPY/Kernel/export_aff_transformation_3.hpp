#ifndef CGALPY_EXPORT_AFF_TRANSFORMATION_3_HPP
#define CGALPY_EXPORT_AFF_TRANSFORMATION_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Aff_transformation_3.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;
//
template <typename Kernel_>
typename Kernel_::Point_3
transform_point(const typename Kernel_::Aff_transformation_3& t,
                const typename Kernel_::Point_3& p)
{ return t.transform(p); }

template <typename Kernel_>
typename Kernel_::Vector_3
transform_vector(const typename Kernel_::Aff_transformation_3& t,
                 const typename Kernel_::Vector_3& v)
{ return t.transform(v); }

template <typename Kernel_>
typename Kernel_::Direction_3
transform_direction(const typename Kernel_::Aff_transformation_3& t,
                    const typename Kernel_::Direction_3& d)
{ return t.transform(d); }

template <typename Kernel_>
typename Kernel_::Line_3
transform_line(const typename Kernel_::Aff_transformation_3& t,
               const typename Kernel_::Line_3& l)
{ return t.transform(l); }

template <typename Kernel_>
typename Kernel_::Point_3
op_ptr(const typename Kernel_::Aff_transformation_3& t,
       const typename Kernel_::Point_3& p)
{ return t(p); }

template <typename Kernel_>
typename Kernel_::Vector_3
op_vtr(const typename Kernel_::Aff_transformation_3& t,
       const typename Kernel_::Vector_3& v)
{ return t(v); }

template <typename Kernel_>
typename Kernel_::Direction_3
op_dir(const typename Kernel_::Aff_transformation_3& t,
       const typename Kernel_::Direction_3& d)
{ return t(d); }

template <typename Kernel_>
typename Kernel_::Plane_3
op_ln(const typename Kernel_::Aff_transformation_3& t,
      const typename Kernel_::Plane_3& l)
{ return t(l); }


template <typename Kernel_, typename C>
void export_aff_transformation_3(C& c) {
  using Ker = Kernel_;
  using Rt = typename Ker::RT;
  using Vec_3 = typename Ker::Vector_3;
  using Dir_3 = typename Ker::Direction_3;
  using Aff_3 = typename Ker::Aff_transformation_3;
  using Pnt_3 = typename Ker::Point_3;
  using Pln_3 = typename Ker::Plane_3;
  c.def(py::init<const CGAL::Identity_transformation&>(),
        py::arg("IDENTITY"), "introduces an identity transformation.")
    .def(py::init<const CGAL::Translation, const Vec_3&>(),
         py::arg("Translation"), py::arg("v"), "introduces a translation by a vector v.")
    .def(py::init<const CGAL::Scaling, const Rt&, const Rt&>(),
         py::arg("Scaling"), py::arg("s"), py::arg("hw") = Rt(1),
         "introduces a scaling by a scale factor s/hw.")
          	// Aff_transformation_3 (const Kernel::RT &m00, const Kernel::RT &m01, const Kernel::RT &m02, const Kernel::RT &m03, const Kernel::RT &m10, const Kernel::RT &m11, const Kernel::RT &m12, const Kernel::RT &m13, const Kernel::RT &m20, const Kernel::RT &m21, const Kernel::RT &m22, const Kernel::RT &m23, const Kernel::RT &hw=RT(1))
    .def(py::init<const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&>(),
         py::arg("m00"), py::arg("m01"), py::arg("m02"), py::arg("m03"), py::arg("m10"), py::arg("m11"), py::arg("m12"), py::arg("m13"), py::arg("m20"), py::arg("m21"), py::arg("m22"), py::arg("m23"), py::arg("hw") = Rt(1),
         "introduces a general affine transformation of the matrix form ⎛⎝⎜⎜⎜⎜m00m10m200m01m11m210m02m12m220m03m13m23hw⎞⎠⎟⎟⎟⎟.")
    .def("transform", &Aff_3::transform_point,
         py::arg("p"))
    .def("transform", &Aff_3::transform_vector,
         py::arg("p"))
    .def("transform", &Aff_3::transform_direction,
         py::arg("p"))
    .def("transform", &Aff_3::transform_line,
         py::arg("p"))
    .def("__call__", &op_ptr<Ker>,
         py::arg("p"))
    .def("__call__", &op_vtr<Ker>,
         py::arg("p"))
    .def("__call__", &op_dir<Ker>,
         py::arg("p"))
    .def("__call__", &op_ln<Ker>,
         py::arg("p"))
    .def(py::self * py::self,
         py::sig("def __mul__(self, s: Aff_transformation_3, /) -> Aff_transformation_3"))
    .def("inverse", &Aff_3::inverse, "gives the inverse transformation.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, s: Aff_transformation_3, /) -> bool"))
    .def("is_even", &Aff_3::is_even, "returns true, if the transformation is not reflecting, i.e. the determinant of the involved linear transformation is non-negative.")
    .def("is_odd", &Aff_3::is_odd, "returns true, if the transformation is reflecting.")
    .def("is_scaling", &Aff_3::is_scaling, "returns true, if the object was constructed using the tag CGAL::Scaling, or is the result of the composition of only such scaling transformation objects.")
    .def("is_translation", &Aff_3::is_translation, "returns true, if the object was constructed using the tag CGAL::Translation, or is the result of the composition of only such translation transformation objects.")
    .def("cartesian", &Aff_3::cartesian, py::arg("i"), py::arg("j"))
    .def("m", &Aff_3::m, py::arg("i"), py::arg("j"), "returns entry mij in a matrix representation in which m33 is 1.")
    .def("homogeneous", &Aff_3::homogeneous, py::arg("i"), py::arg("j"))
    .def("hm", &Aff_3::hm, py::arg("i"), py::arg("j"), "returns entry mij in a matrix representation in which m33 is 1.")
    .def("__str__", to_string<Aff_3>)
    .def("__repr__", to_string<Aff_3>)
    ;

  add_extraction(c);
}




#endif
