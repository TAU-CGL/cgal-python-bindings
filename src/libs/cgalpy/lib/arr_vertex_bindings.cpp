// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include  <cstddef>

#include <nanobind/nanobind.h>
#include <nanobind/stl/unique_ptr.h>

#include <CGAL/Envelope_3/Envelope_base.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace aos2 {

//! Wrap the function that obtains the real circulator
auto incident_halfedges_circulator(const Vertex& v) { return v.incident_halfedges(); }

//! Wrap the iterator
py::object incident_halfedges_iterator(const Vertex& v)
{ return make_iterator_from_circulator(v.incident_halfedges()); }

//!
#ifdef CGALPY_ENVELOPE_3_BINDINGS
py::object surfaces(const Vertex& v)
{ return make_iterator(v.surfaces_begin(), v.surfaces_end()); }
#endif

//!
static int vertex_tp_traverse(PyObject* self, visitproc visit, void* arg) {
  V* v = py::inst_ptr<V>(self);

#if 1
  py::object& data = *(reinterpret_cast<py::object*>(v));
#else
  py::handle data = py::find(v->m_data);
#endif
  Py_VISIT(data.ptr());

  // On Python 3.9+, we must traverse the implicit dependency
  // of an object on its associated type object.
#if PY_VERSION_HEX >= 0x03090000
  Py_VISIT(Py_TYPE(self));
#endif

  return 0;
}

/*! We need to access the private m_data field (which stores an object of type
 * py::object). We use a (very) dirty hack, and we base it on the assumption that m_data
 * is the first field in V (a.k.a. Arr_extended_vertex).
 *
 * Nevertheless, the cleanup doesn't work, and I have no idea why.
 * I've tried to temporary eliminate the hack by changing the V class and definiting
 * m_data as public to no avail, so the leak(s) are still reported.
 * \todo Fix the leak(s) caused by having py::objects in V (and also in H and F).
 *
 * In the meanwhile one can explicitly clean the m_data field of all vertices from within
 * the python script:
 *   for v in arr.vertices(): v.set_data({})
 * and similarly for H and F:
 *   for h in arr.halfedges(): h.set_data({})
 *   for f in arr.faces(): f.set_data({})
 */
static int vertex_tp_clear(PyObject* self) {
  V* v = py::inst_ptr<V>(self);
#if 1
  py::object& data = *(reinterpret_cast<py::object*>(v));
#else
  py::handle data = py::find(v->m_data);
#endif
  data = {};
  return 0;
}

//!
static PyType_Slot aos_vertex_slots[] = {
  {Py_tp_traverse, (void*) vertex_tp_traverse},
  {Py_tp_clear, (void*) vertex_tp_clear},
  {0, nullptr}
};

}

//!
void export_vertex(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Vertex = Aos::Vertex;
  using Face = Aos::Face;
  using Point = Aos::Point_2;
  constexpr auto ri(py::rv_policy::reference_internal);

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Env_data = Vertex::Vertex_data;
  using Dd = CGAL::Dac_decision;
#endif

#ifdef CGALPY_AOS2_VERTEX_EXTENDED
  if (! add_attr<aos2::V>(c, "Arr_extended_vertex")) {
    py::class_<aos2::V> (c, "Arr_extended_vertex", py::type_slots(aos2::aos_vertex_slots))
      .def("set_data", &aos2::V::set_data)
      .def("data", py::overload_cast<>(&aos2::V::data, py::const_))
      ;
  }
#endif

  if (add_attr<Vertex>(c, "Vertex")) return;

#ifdef CGALPY_AOS2_VERTEX_EXTENDED
  py::class_<Vertex, aos2::V> vertex_c(c, "Vertex");
#else
  py::class_<Vertex> vertex_c(c, "Vertex");
#endif

  vertex_c.def(py::init<>())

    // As a convention, add the suffix `_mutable` to the mutable version.
    // Wrap the mutable method with the `reference_internal` call policy.
    // An unsafe point that is referenced counted will most likely die when the
    // Aos data structure that holds it dies, as the reference counter will
    // vanish. Not all points are referenced counted (perhaps they should...).
    .def("point_unsafe_mutable", [](Vertex& v)->Point& { return v.point(); }, ri)
    .def("point_unsafe", [](const Vertex& v)->const Point& { return v.point(); }, ri)
    .def("point",
         [](const Vertex& v)->std::unique_ptr<Point>
         { return std::make_unique<Point>(v.point()); }, ri)
    .def("is_isolated", [](const Vertex& v)->bool { return v.is_isolated(); })

    // Immediate members
    .def("is_at_open_boundary", &Vertex::is_at_open_boundary)
    .def("parameter_space_in_x",
         [](const Vertex& v)->CGAL::Arr_parameter_space
         { return v.parameter_space_in_x(); })
    .def("parameter_space_in_y",
         [](const Vertex& v)->CGAL::Arr_parameter_space
         { return v.parameter_space_in_y(); })
    .def("degree", &Vertex::degree)
    .def("face", [](const Vertex& v)->const Face& { return *(v.face()); }, ri)
    .def("incident_halfedges",
         &aos2::incident_halfedges_iterator, py::keep_alive<0, 1>())

    // Wrap also the function that obtains the circulator
    .def("incident_halfedges_circulator", &aos2::incident_halfedges_circulator)

// #ifdef CGALPY_AOS2_VERTEX_EXTENDED
//     // The member functions set_data() and data() are defined in a base class of
//     // Face. Therefore, we cannot directly refere to any of them, e.g.,
//     // `Face::set_data`. Instead, we introduce lambda functions that call the
//     // appropriate member functions.
//     .def("set_data", [](Vertex& v, py::object obj) { v.set_data(obj); })
//     .def("data", [](const Vertex& v)->py::object { return v.data(); })
// #endif

#ifdef CGALPY_ENVELOPE_3_BINDINGS
    .def("number_of_surfaces", [](Vertex& v) { return v.number_of_surfaces(); })
    .def("surfaces", &aos2::surfaces, py::keep_alive<0, 1>())
#endif
    ;

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Si = Vertex::Data_const_iterator;
  add_iterator<Si, Si>("Surface_iterator", vertex_c);
#endif

  using Havcc = aos2::Halfedge_around_vertex_const_circulator;
  add_iterator_from_circulator<Havcc>("Halfedge_around_vertex_iterator", vertex_c);

  // Wrap also the real circulator
  export_circulator<Havcc>(vertex_c, "Halfedge_around_vertex_circulator");
}
