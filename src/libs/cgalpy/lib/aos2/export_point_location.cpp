// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_naive_point_location.h>
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
#include <CGAL/Arr_walk_along_line_point_location.h>
#endif
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>
#include <CGAL/Arr_observer.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/aos2/arr_point_location_config.hpp"
#include "cgalpy/aos2/arrangement_on_surface_2_types.hpp"
#include "cgalpy/Aos2_docstrings.hpp"
#include "cgalpy/iterators/py_list_forward_iterator.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

namespace cgalpy {
namespace aos2 {

using Face_const_handle = typename aos2::Face_const_handle;
using Halfedge_const_handle = typename aos2::Halfedge_const_handle;
using Vertex_const_handle = typename aos2::Vertex_const_handle;

using Pl_result =
  typename CGAL::Arr_point_location_result<Arrangement_on_surface_2>::Type;
  using Pl_query_result = typename std::pair<Point_2, Pl_result>;

//
class Point_location_result_visitor {
public:
  template<typename T>
  py::object operator()(T& operand) const
  { return py::cast(&(*operand)); }
};

//
py::list locate_batch(const Arrangement_on_surface_2& arr, const py::list& lst)
{
  py::list res;
  auto op = [&] (const Pl_query_result& p) mutable {
      const auto& result =
        std::visit(Point_location_result_visitor(), p.second);
      res.append(py::make_tuple(p.first, result));
    };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin = py_list_forward_iterator<Point_2>(lst);
  auto end = py_list_forward_iterator<Point_2>(lst, false);
  locate(arr, begin, end, it);
  return res;
}

//
template <typename PL>
py::object locate(PL& pl, const Point_2& p) {
  auto result = pl.locate(p);
  return std::visit(Point_location_result_visitor(), result);
}

//
template <typename PL>
py::object ray_shoot_up(PL& pl, const Point_2& p) {
  auto result = pl.ray_shoot_up(p);
  return std::visit(Point_location_result_visitor(), result);
}

//
template <typename PL>
py::object ray_shoot_down(PL& pl, const Point_2& p) {
  auto result = pl.ray_shoot_down(p);
  return std::visit(Point_location_result_visitor(), result);
}

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

  ~Point_location_with_owner() {
    if (m_attached) Base::detach();
  }

  void attach(Aos& arr) {
    // Preserve the previous Python owner while CGAL changes the
    // native attachment, then replace it with the new arrangement.
    Base::attach(arr);
    m_arrangement_owner =
      py::cast(&arr, py::rv_policy::reference);
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
    auto* w =
      py::inst_ptr<Point_location_with_owner>(self);

    Py_VISIT(w->m_arrangement_owner.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    auto* w =
      py::inst_ptr<Point_location_with_owner>(self);

    if (w->m_attached) {
      static_cast<Base&>(*w).detach();
      w->m_attached = false;
    }

    w->m_arrangement_owner = {};

    return 0;
  }

private:
  py::object m_arrangement_owner;
  bool m_attached;
};

template <typename PointLocation>
PyType_Slot* point_location_owner_slots() {
  static PyType_Slot slots[] = {
    {
      Py_tp_traverse,
      (void*) PointLocation::tp_traverse
    },
    {
      Py_tp_clear,
      (void*) PointLocation::tp_clear
    },
    {0, nullptr}
  };

  return slots;
}

// #if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
//   CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
//   CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
// void landmarks_pl_attach(Landmarks_pl& pl, Arrangement_on_surface_2& arr)
// { pl.attach(arr); }
// #endif

}
} // namespace cgalpy

void export_point_location(py::module_& m) {
  using Aos = cgalpy::aos2::Arrangement_on_surface_2;
  using Aob = CGAL::Arr_observer<Aos>;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Supported only by some of the traits
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  using Landmarks_base =
    CGAL::Arr_landmarks_point_location<Aos>;
  using Landmarks_pl =
    cgalpy::aos2::Point_location_with_owner<
      Landmarks_base, Aos>;
  if (! add_attr<Landmarks_pl>(m, "Arr_landmarks_point_location")) {
    py::class_<Landmarks_pl>(
      m,
      "Arr_landmarks_point_location",
      py::type_slots(
        cgalpy::aos2::point_location_owner_slots<
          Landmarks_pl>()),
      aos2_doc::Arr_landmarks_point_location_class)
      .def(py::init<>(),
           "Construct a detached landmarks point-location object.")
      .def(py::init<Aos&>(), py::arg("arr"),
           aos2_doc::AosPointLocation_2_AosPointLocation_2_1)
      .def("attach", &Landmarks_pl::attach,
           py::arg("arr"),
           aos2_doc::AosPointLocation_2_attach)
      .def("detach", &Landmarks_pl::detach,
           aos2_doc::AosPointLocation_2_detach)
      .def("locate", &cgalpy::aos2::locate<Landmarks_pl>, ri,
           py::arg("point"),
           aos2_doc::AosPointLocation_2_locate)
      // .def("ray_shoot_up", &cgalpy::aos2::ray_shoot_up<Landmarks_pl>, ri)
      // .def("ray_shoot_down", &cgalpy::aos2::ray_shoot_down<Landmarks_pl>, ri)
      ;
  }
#endif

  // Compile in only if we use CGAL version >= 5.6.0; see PR #6810
#if CGAL_VERSION_NR >= 1050600900
  using Trapezoid_base =
    CGAL::Arr_trapezoid_ric_point_location<Aos>;
  using Trapezoid_pl =
    cgalpy::aos2::Point_location_with_owner<
      Trapezoid_base, Aos>;
  if (! add_attr<Trapezoid_pl>(m, "Arr_trapezoid_ric_point_location")) {
    py::class_<Trapezoid_pl, Aob>(
      m,
      "Arr_trapezoid_ric_point_location",
      py::type_slots(
        cgalpy::aos2::point_location_owner_slots<
          Trapezoid_pl>()),
      aos2_doc::Arr_trapezoid_ric_point_location_class)
      .def(py::init<bool>(), py::arg("with_guarantees") = true,
           aos2_doc::Arr_trapezoid_ric_point_location_Arr_trapezoid_ric_point_location)
      .def(py::init<Aos&, bool>(), py::arg("arr"), py::arg("with_guarantees") = true,
           aos2_doc::Arr_trapezoid_ric_point_location_Arr_trapezoid_ric_point_location_1)
      .def("attach", &Trapezoid_pl::attach, py::arg("arr"),
           aos2_doc::AosPointLocation_2_attach)
      .def("detach", &Trapezoid_pl::detach,
           aos2_doc::AosPointLocation_2_detach)
      .def("depth", &Trapezoid_pl::depth,
           "Return the depth of the trapezoidal search structure.")
      .def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length,
           "Return the longest query path length in the search structure.")
      .def("with_guarantees", &Trapezoid_pl::with_guarantees,
           aos2_doc::Arr_trapezoid_ric_point_location_with_guarantees)
      .def("locate", &cgalpy::aos2::locate<Trapezoid_pl>, ri,
           py::arg("point"),
           aos2_doc::AosPointLocation_2_locate)
      .def("ray_shoot_up", &cgalpy::aos2::ray_shoot_up<Trapezoid_pl>, ri,
           py::arg("point"),
           aos2_doc::AosVerticalRayShoot_2_ray_shoot_up)
      .def("ray_shoot_down", &cgalpy::aos2::ray_shoot_down<Trapezoid_pl>, ri,
           py::arg("point"),
           aos2_doc::AosVerticalRayShoot_2_ray_shoot_down)
      ;
  }
#endif

#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  using Walk_base =
    CGAL::Arr_walk_along_line_point_location<Aos>;
  using Walk_pl =
    cgalpy::aos2::Point_location_with_owner<
      Walk_base, Aos>;
  if (! add_attr<Walk_pl>(m, "Arr_walk_along_line_point_location")) {
    py::class_<Walk_pl>(
      m,
      "Arr_walk_along_line_point_location",
      py::type_slots(
        cgalpy::aos2::point_location_owner_slots<
          Walk_pl>()),
      aos2_doc::Arr_walk_along_line_point_location_class)
      .def(py::init<>(),
           "Construct a detached walk-along-line point-location object.")
      .def(py::init<Aos&>(), py::arg("arr"),
           aos2_doc::AosPointLocation_2_AosPointLocation_2_1)
      .def("attach", &Walk_pl::attach, py::arg("arr"),
           aos2_doc::AosPointLocation_2_attach)
      .def("detach", &Walk_pl::detach,
           aos2_doc::AosPointLocation_2_detach)
      .def("locate", &cgalpy::aos2::locate<Walk_pl>, ri,
           py::arg("point"),
           aos2_doc::AosPointLocation_2_locate)
      .def("ray_shoot_up", &cgalpy::aos2::ray_shoot_up<Walk_pl>, ri,
           py::arg("point"),
           aos2_doc::AosVerticalRayShoot_2_ray_shoot_up)
      .def("ray_shoot_down", &cgalpy::aos2::ray_shoot_down<Walk_pl>, ri,
           py::arg("point"),
           aos2_doc::AosVerticalRayShoot_2_ray_shoot_down)
      ;
  }
#endif

  using Naive_base =
    CGAL::Arr_naive_point_location<Aos>;
  using Naive_pl =
    cgalpy::aos2::Point_location_with_owner<
      Naive_base, Aos>;
  if (! add_attr<Naive_pl>(m, "Arr_naive_point_location")) {
    py::class_<Naive_pl>(
      m,
      "Arr_naive_point_location",
      py::type_slots(
        cgalpy::aos2::point_location_owner_slots<
          Naive_pl>()),
      aos2_doc::Arr_naive_point_location_class)
      .def(py::init<>(),
           "Construct a detached naive point-location object.")
      .def(py::init<Aos&>(), py::arg("arr"),
           aos2_doc::AosPointLocation_2_AosPointLocation_2_1)
      .def("attach", &Naive_pl::attach, py::arg("arr"),
           aos2_doc::AosPointLocation_2_attach)
      .def("detach", &Naive_pl::detach,
           aos2_doc::AosPointLocation_2_detach)
      .def("locate", &cgalpy::aos2::locate<Naive_pl>, ri,
           py::arg("point"),
           aos2_doc::AosPointLocation_2_locate)
      // .def("ray_shoot_up", &cgalpy::aos2::ray_shoot_up<Naive_pl>, ri)
      // .def("ray_shoot_down", &cgalpy::aos2::ray_shoot_down<Naive_pl>, ri)
      ;
  }

  m.def("locate", &cgalpy::aos2::locate_batch, ri, py::keep_alive<1, 0>(),
        py::arg("arr"), py::arg("points"),
        aos2_doc::locate);
}
