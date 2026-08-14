// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Utkarsh Khajuria <utkarshkhajuria55@gmail.com>
//            Efi Fogel         <efifogel@gmail.com>

#include <stdexcept>

#include <nanobind/nanobind.h>

#include "cgalpy/visibility_2_types.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/aos2/arrangement_on_surface_2_types.hpp"

#include "cgalpy/Vis2_docstrings.hpp"

namespace py = nanobind;
namespace vis2_doc = cgalpy::vis2::docstrings;

namespace cgalpy {
namespace vis2 {

template <typename Visibility>
class Visibility_with_owner : public Visibility {
public:
  using Base = Visibility;
  using Arrangement_2 = typename Base::Arrangement_2;

  Visibility_with_owner() :
    Base(),
    m_arrangement_owner(py::none())
  {}

  explicit Visibility_with_owner(const Arrangement_2& arr) :
    Base(arr),
    m_arrangement_owner(py::cast(&arr, py::rv_policy::reference))
  {}

  ~Visibility_with_owner() {
    if (Base::is_attached()) Base::detach();
  }

  bool is_attached() const
  { return Base::is_attached(); }

  void attach(const Arrangement_2& arr) {
    // Keep the previous Python owner alive while CGAL performs the
    // transition, then replace it with the newly attached arrangement.
    Base::attach(arr);
    m_arrangement_owner =
      py::cast(&arr, py::rv_policy::reference);
  }

  void detach() {
    // Detach the native object before releasing the Python owner.
    Base::detach();
    m_arrangement_owner = py::none();
  }

  const Arrangement_2& arrangement_2() const {
    if (! Base::is_attached())
      throw std::runtime_error(
        "visibility object is not attached to an arrangement");
    return Base::arrangement_2();
  }

  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    auto* w = py::inst_ptr<Visibility_with_owner>(self);

    Py_VISIT(w->m_arrangement_owner.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    auto* w = py::inst_ptr<Visibility_with_owner>(self);

    auto& base = static_cast<Base&>(*w);
    if (base.is_attached()) base.detach();

    w->m_arrangement_owner = {};

    return 0;
  }

private:
  py::object m_arrangement_owner;
};

template <typename Visibility>
const Face& compute_visibility1(Visibility& vis,
                                const Point_2& q, const Face& f,
                                Arrangement_2& arr)
{ return *(vis.compute_visibility(q, Face_const_handle(&f), arr)); }

template <typename Visibility>
const Face& compute_visibility2(Visibility& vis,
                                const Point_2& q, const Halfedge& h,
                                Arrangement_2& arr)
{ return *(vis.compute_visibility(q, Halfedge_const_handle(&h), arr)); }

}
} // namespace cgalpy

void export_visibility_2(py::module_& m) {
  using Spv_base =
    cgalpy::vis2::Simple_polygon_visibility_2;
  using Spv =
    cgalpy::vis2::Visibility_with_owner<Spv_base>;
  using Arr = cgalpy::vis2::Arrangement_2;
  using Point = cgalpy::vis2::Point_2;
  using Face = cgalpy::vis2::Face;

  if (! add_attr<Spv>(m, "Simple_polygon_visibility_2")) {
    static PyType_Slot spv_slots[] = {
      {Py_tp_traverse, (void*) Spv::tp_traverse},
      {Py_tp_clear, (void*) Spv::tp_clear},
      {0, nullptr}
    };

    py::class_<Spv> spv_c(
      m,
      "Simple_polygon_visibility_2",
      py::type_slots(spv_slots),
      vis2_doc::Simple_polygon_visibility_2_class);
    spv_c.def(py::init<>(), vis2_doc::Visibility_2_Visibility_2)
      .def(py::init<const Arr&>(), py::arg("arr"),
           vis2_doc::Visibility_2_Visibility_2_1)
      .def("is_attached", &Spv::is_attached,
           vis2_doc::Visibility_2_is_attached)
      .def("attach", &Spv::attach, py::arg("arr"),
           vis2_doc::Simple_polygon_visibility_2_attach)
      .def("detach", &Spv::detach,
           vis2_doc::Visibility_2_detach)
      .def("arrangement_2", &Spv::arrangement_2,
           py::rv_policy::reference,
           vis2_doc::Visibility_2_arrangement_2)
      .def("compute_visibility", cgalpy::vis2::compute_visibility1<Spv>,
           py::arg("q"), py::arg("face"), py::arg("out_arr"),
           py::rv_policy::reference, py::keep_alive<0, 4>(),
           vis2_doc::Visibility_2_compute_visibility)
      .def("compute_visibility", cgalpy::vis2::compute_visibility2<Spv>,
           py::arg("q"), py::arg("halfedge"), py::arg("out_arr"),
           py::rv_policy::reference, py::keep_alive<0, 4>(),
           vis2_doc::Visibility_2_compute_visibility_1)
      ;
  }

  using Tev_base =
    cgalpy::vis2::Triangular_expansion_visibility_2;
  using Tev =
    cgalpy::vis2::Visibility_with_owner<Tev_base>;
  if (! add_attr<Tev>(m, "Triangular_expansion_visibility_2")) {
    static PyType_Slot tev_slots[] = {
      {Py_tp_traverse, (void*) Tev::tp_traverse},
      {Py_tp_clear, (void*) Tev::tp_clear},
      {0, nullptr}
    };

    py::class_<Tev> tev_c(
      m,
      "Triangular_expansion_visibility_2",
      py::type_slots(tev_slots),
      vis2_doc::Triangular_expansion_visibility_2_class);
    tev_c.def(py::init<>(), vis2_doc::Visibility_2_Visibility_2)
      .def(py::init<const Arr&>(), py::arg("arr"),
           vis2_doc::Visibility_2_Visibility_2_1)
      .def("is_attached", &Tev::is_attached,
           vis2_doc::Visibility_2_is_attached)
      .def("attach", &Tev::attach, py::arg("arr"),
           vis2_doc::Triangular_expansion_visibility_2_attach)
      .def("detach", &Tev::detach,
           vis2_doc::Visibility_2_detach)
      .def("arrangement_2", &Tev::arrangement_2,
           py::rv_policy::reference,
           vis2_doc::Visibility_2_arrangement_2)
      .def("compute_visibility", cgalpy::vis2::compute_visibility1<Tev>,
           py::arg("q"), py::arg("face"), py::arg("out_arr"),
           py::rv_policy::reference, py::keep_alive<0, 4>(),
           vis2_doc::Visibility_2_compute_visibility)
      .def("compute_visibility", cgalpy::vis2::compute_visibility2<Tev>,
           py::arg("q"), py::arg("halfedge"), py::arg("out_arr"),
           py::rv_policy::reference, py::keep_alive<0, 4>(),
           vis2_doc::Visibility_2_compute_visibility_1)
      ;
  }
}
