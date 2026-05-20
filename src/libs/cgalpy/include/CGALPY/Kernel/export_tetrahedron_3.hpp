#ifndef CGALPY_EXPORT_TETRAHEDRON_3_HPP
#define CGALPY_EXPORT_TETRAHEDRON_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Tetrahedron_3.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

template <typename Kernel_, typename C>
void export_tetrahedron_3(C &c) {
  using Ker = Kernel_;
  using Pnt = typename Ker::Point_3;
  using Tet = typename Ker::Tetrahedron_3;
  using FT = typename Ker::FT;

  c.def(py::init<const Pnt&, const Pnt&, const Pnt&, const Pnt&>(),
        py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("p3"), "introduces a tetrahedron t with vertices p0, p1, p2 and p3.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"), "Test for equality: two tetrahedra t and t2 are equal, iff t and t2 have the same orientation and their sets (not sequences) of vertices are equal.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"), "Test for inequality.")
    .def("vertex", &Tet::vertex, py::arg("i"), doc::Tetrahedron_3_vertex)    .def("__getitem__", &Tet::operator[], py::arg("i"), doc::Tetrahedron_3_operator)    .def("is_degenerate", &Tet::is_degenerate, doc::Tetrahedron_3_is_degenerate)    .def("orientation", &Tet::orientation)
    .def("oriented_side", &Tet::oriented_side, py::arg("p"), doc::Tetrahedron_3_oriented_side)    .def("bounded_side", &Tet::bounded_side, py::arg("p"), doc::Tetrahedron_3_bounded_side)    .def("has_on_positive_side", &Tet::has_on_positive_side, py::arg("p"))
    .def("has_on_negative_side", &Tet::has_on_negative_side, py::arg("p"))
    .def("has_on_boundary", &Tet::has_on_boundary, py::arg("p"))
    .def("has_on_bounded_side", &Tet::has_on_bounded_side, py::arg("p"))
    .def("has_on_unbounded_side", &Tet::has_on_unbounded_side, py::arg("p"))
    .def("volume", &Tet::volume, doc::Tetrahedron_3_volume)    .def("bbox", &Tet::bbox, doc::Tetrahedron_3_bbox)    .def("transform", &Tet::transform, py::arg("at"), doc::Tetrahedron_3_transform)    .def("__str__", &to_string<Tet>)
    .def("__repr__", &to_string<Tet>)
    ;

  add_extraction(c);
}



#endif
