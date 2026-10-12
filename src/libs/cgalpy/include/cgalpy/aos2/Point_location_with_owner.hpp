// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_AOS2_POINT_LOCATION_WITH_OWNER_HPP
#define CGALPY_AOS2_POINT_LOCATION_WITH_OWNER_HPP

#include <Python.h>
#include <nanobind/nanobind.h>

namespace cgalpy {
namespace aos2 {

namespace py = nanobind;

template <typename Base, typename Aos>
class Point_location_with_owner : public Base {
public:
  Point_location_with_owner() :
    Base(),
    m_arrangement_owner(py::none()),
    m_attached(false)
  {}

  explicit Point_location_with_owner(Aos& arr) :
    Base(arr),
    m_arrangement_owner(
      py::cast(&arr, py::rv_policy::reference)),
    m_attached(true)
  {}

  explicit Point_location_with_owner(bool with_guarantees) :
    Base(with_guarantees),
    m_arrangement_owner(py::none()),
    m_attached(false)
  {}

  Point_location_with_owner(Aos& arr, bool with_guarantees) :
    Base(arr, with_guarantees),
    m_arrangement_owner(
      py::cast(&arr, py::rv_policy::reference)),
    m_attached(true)
  {}

  ~Point_location_with_owner() { if (m_attached) Base::detach(); }

  void attach(Aos& arr) {
    // Preserve the previous Python owner while CGAL changes the
    // native attachment, then replace it with the new arrangement.
    Base::attach(arr);
    m_arrangement_owner = py::cast(&arr, py::rv_policy::reference);
    m_attached = true;
  }

  void detach() {
    if (! m_attached) return;

    // Remove the native relationship before releasing its Python owner.
    Base::detach();
    m_attached = false;
    m_arrangement_owner = py::none();
  }

  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    bool traversed_base = false;

    py::handle bound_type = py::type<Point_location_with_owner>();

    auto* py_type = reinterpret_cast<PyTypeObject*>(bound_type.ptr());

    if (py_type->tp_base != nullptr) {
      py::handle base_type(reinterpret_cast<PyObject*>(py_type->tp_base));

      auto base_traverse = reinterpret_cast<traverseproc>(py::type_get_slot(base_type, Py_tp_traverse));

      if (base_traverse != nullptr) {
        const int result = base_traverse(self, visit, arg);
        if (result != 0) return result;

        traversed_base = true;
      }
    }

    if (! traversed_base) Py_VISIT(Py_TYPE(self));

    if (! py::inst_ready(self)) return 0;

    auto* w = py::inst_ptr<Point_location_with_owner>(self);

    Py_VISIT(w->m_arrangement_owner.ptr());

    return 0;
  }

  static int tp_clear(PyObject* self) {
    inquiry base_clear = nullptr;

    py::handle bound_type = py::type<Point_location_with_owner>();

    auto* py_type = reinterpret_cast<PyTypeObject*>(bound_type.ptr());

    if (py_type->tp_base != nullptr) {
      py::handle base_type(reinterpret_cast<PyObject*>(py_type->tp_base));

      base_clear = reinterpret_cast<inquiry>(py::type_get_slot(base_type, Py_tp_clear));
    }

    if (! py::inst_ready(self)) return (base_clear != nullptr) ? base_clear(self) : 0;

    auto* w = py::inst_ptr<Point_location_with_owner>(self);
    if (w->m_attached) {
      static_cast<Base&>(*w).detach();
      w->m_attached = false;
    }

    w->m_arrangement_owner = {};

    if (base_clear != nullptr) {
      const int result = base_clear(self);
      if (result != 0) return result;
    }

    return 0;
  }

private:
  py::object m_arrangement_owner;
  bool m_attached;
};

template <typename PointLocation>
PyType_Slot* point_location_owner_slots() {
  static PyType_Slot slots[] = {
    { Py_tp_traverse, (void*) PointLocation::tp_traverse },
    { Py_tp_clear, (void*) PointLocation::tp_clear },
    {0, nullptr}
  };

  return slots;
}

} // namespace aos2
} // namespace cgalpy

#endif // CGALPY_AOS2_POINT_LOCATION_WITH_OWNER_HPP
