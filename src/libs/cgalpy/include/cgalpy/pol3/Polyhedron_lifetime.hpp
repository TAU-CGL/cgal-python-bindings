// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_POL3_POLYHEDRON_LIFETIME_HPP
#define CGALPY_POL3_POLYHEDRON_LIFETIME_HPP

#include <nanobind/nanobind.h>
#include <Python.h>

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace cgalpy::pol3::lifetime {
namespace py = nanobind;

#ifndef CGALPY_POL3_LIFETIME_POLICIES
#define CGALPY_POL3_LIFETIME_POLICIES 1
#endif

#if CGALPY_POL3_LIFETIME_POLICIES
#define CGALPY_POL3_LIFETIME_POLICY(Policy) , nanobind::call_policy<Policy>()
#else
#define CGALPY_POL3_LIFETIME_POLICY(Policy)
#endif

struct Owner_state { PyObject* owner = nullptr; std::size_t active = 0; };

inline auto& owner_states() {
  static auto* states = new std::unordered_map<PyObject*, Owner_state*>();
  return *states;
}
inline PyTypeObject*& owner_python_type() { static PyTypeObject* type = nullptr; return type; }
inline auto& dependent_python_types() { static auto* types = new std::vector<PyTypeObject*>(); return *types; }
inline void register_owner_python_type(PyObject* type) noexcept { owner_python_type() = reinterpret_cast<PyTypeObject*>(type); }
inline void register_dependent_python_type(PyObject* type) {
  auto* py_type = reinterpret_cast<PyTypeObject*>(type);
  for (auto* current : dependent_python_types()) if (current == py_type) return;
  dependent_python_types().push_back(py_type);
}
inline bool& lifetime_checks_enabled_flag() { static bool enabled = true; return enabled; }
inline bool& lifetime_checks_ever_disabled_flag() { static bool disabled = false; return disabled; }
inline bool lifetime_checks_enabled() noexcept {
#if CGALPY_POL3_LIFETIME_POLICIES
  return lifetime_checks_enabled_flag();
#else
  return false;
#endif
}
inline std::size_t total_active_lease_count() noexcept {
  std::size_t total = 0;
  for (const auto& entry : owner_states()) if (entry.second != nullptr) total += entry.second->active;
  return total;
}
inline void disable_lifetime_checks() {
#if CGALPY_POL3_LIFETIME_POLICIES
  if (lifetime_checks_ever_disabled_flag()) return;
  if (total_active_lease_count() != 0)
    throw std::runtime_error("Polyhedron_3 lifetime checks can only be disabled after all tracked dependent handles are released.");
  lifetime_checks_enabled_flag() = false;
  lifetime_checks_ever_disabled_flag() = true;
#endif
}
inline void release_owner_state(void* payload) noexcept {
  auto* state = static_cast<Owner_state*>(payload);
  owner_states().erase(state->owner);
  delete state;
}
inline Owner_state& ensure_owner_state(PyObject* owner) {
  auto& states = owner_states();
  const auto found = states.find(owner);
  if (found != states.end()) return *found->second;
  auto* state = new Owner_state{owner, 0};
  states.emplace(owner, state);
  py::detail::keep_alive(owner, state, release_owner_state);
  return *state;
}
inline Owner_state* find_owner_state(PyObject* owner) noexcept {
  auto& states = owner_states(); const auto found = states.find(owner);
  return found == states.end() ? nullptr : found->second;
}
inline std::size_t active_lease_count(PyObject* owner) noexcept {
  const auto* state = find_owner_state(owner); return state == nullptr ? 0 : state->active;
}
inline void release_dependent_lease(void* payload) noexcept {
  auto* state = static_cast<Owner_state*>(payload); if (state == nullptr) return;
  if (state->active != 0) --state->active;
  PyObject* owner = state->owner; Py_DECREF(owner);
}
inline void attach_lease(PyObject* owner, PyObject* dependent) {
  if (!lifetime_checks_enabled() || owner == nullptr || dependent == nullptr || dependent == Py_None) return;
  auto& state = ensure_owner_state(owner); ++state.active; Py_INCREF(owner);
  py::detail::keep_alive(dependent, &state, release_dependent_lease);
}
inline void guard_no_active_leases(PyObject* owner) {
  if (!lifetime_checks_enabled()) return;
  if (active_lease_count(owner) != 0)
    throw std::runtime_error("Polyhedron_3 topology mutation is blocked while dependent Python handles are alive.");
}
inline PyObject* find_owner_argument(PyObject** args, std::size_t nargs) noexcept {
  auto* type = owner_python_type(); if (type == nullptr) return nullptr;
  for (std::size_t i = 0; i < nargs; ++i)
    if (args[i] != nullptr && PyObject_TypeCheck(args[i], type)) return args[i];
  return nullptr;
}
inline bool is_registered_dependent(PyObject* object) noexcept {
  if (object == nullptr) return false;
  for (auto* type : dependent_python_types())
    if (type != nullptr && PyObject_TypeCheck(object, type)) return true;
  return false;
}
inline void attach_registered_dependents(PyObject* owner, PyObject* object,
                                         std::unordered_set<PyObject*>& seen) {
  if (object == nullptr || object == Py_None || !seen.insert(object).second) return;
  if (is_registered_dependent(object)) { attach_lease(owner, object); return; }
  if (PyTuple_Check(object)) {
    const auto n = PyTuple_GET_SIZE(object);
    for (Py_ssize_t i = 0; i < n; ++i) attach_registered_dependents(owner, PyTuple_GET_ITEM(object, i), seen);
  }
  else if (PyList_Check(object)) {
    const auto n = PyList_GET_SIZE(object);
    for (Py_ssize_t i = 0; i < n; ++i) attach_registered_dependents(owner, PyList_GET_ITEM(object, i), seen);
  }
}

template <std::size_t I> struct Register_result_lease_from {
  static void precall(PyObject**, std::size_t, py::detail::cleanup_list*) {}
  template <std::size_t N> static void postcall(PyObject** args, std::integral_constant<std::size_t,N>, py::handle result) {
    static_assert(I > 0 && I <= N, "Owner argument index is outside the function argument range.");
    if (result.ptr() != nullptr && result.ptr() != Py_None) attach_lease(args[I-1], result.ptr());
  }
};
struct Register_result_lease_from_owner_argument {
  static void precall(PyObject**, std::size_t, py::detail::cleanup_list*) {}
  template <std::size_t N> static void postcall(PyObject** args, std::integral_constant<std::size_t,N>, py::handle result) {
    if (!lifetime_checks_enabled() || result.ptr() == nullptr || result.ptr() == Py_None) return;
    if (auto* owner = find_owner_argument(args,N)) attach_lease(owner,result.ptr());
  }
};
struct Register_dependent_result_tree_from_owner_argument {
  static void precall(PyObject**, std::size_t, py::detail::cleanup_list*) {}
  template <std::size_t N> static void postcall(PyObject** args, std::integral_constant<std::size_t,N>, py::handle result) {
    if (!lifetime_checks_enabled() || result.ptr() == nullptr || result.ptr() == Py_None) return;
    auto* owner = find_owner_argument(args,N); if (owner == nullptr) return;
    std::unordered_set<PyObject*> seen; attach_registered_dependents(owner,result.ptr(),seen);
  }
};
template <std::size_t I> struct Guard_no_active_leases_from {
  static void precall(PyObject** args, std::size_t nargs, py::detail::cleanup_list*) {
    if (I == 0 || I > nargs) throw std::runtime_error("Owner argument index is outside the function argument range.");
    guard_no_active_leases(args[I-1]);
  }
  template <std::size_t N> static void postcall(PyObject**, std::integral_constant<std::size_t,N>, py::handle) {}
};
struct Guard_no_active_leases_any_owner_argument {
  static void precall(PyObject** args, std::size_t nargs, py::detail::cleanup_list*) {
    if (!lifetime_checks_enabled()) return;
    if (auto* owner = find_owner_argument(args,nargs)) guard_no_active_leases(owner);
  }
  template <std::size_t N> static void postcall(PyObject**, std::integral_constant<std::size_t,N>, py::handle) {}
};
struct Require_lifetime_checks_disabled {
  static void precall(PyObject**, std::size_t, py::detail::cleanup_list*) {
    if (lifetime_checks_enabled())
      throw std::runtime_error("This Polyhedron_3 topology mutation requires lifetime checks to be disabled first.");
  }
  template <std::size_t N> static void postcall(PyObject**, std::integral_constant<std::size_t,N>, py::handle) {}
};
template <std::size_t I> struct Register_self_lease_from_owner_attr_at {
  static void precall(PyObject**, std::size_t, py::detail::cleanup_list*) {}
  template <std::size_t N> static void postcall(PyObject** args, std::integral_constant<std::size_t,N>, py::handle) {
    static_assert(I > 0 && I <= N, "Owner-carrier argument index is outside the function argument range.");
    if (N == 0 || args[0] == nullptr || args[I-1] == nullptr) return;
    PyObject* owner = PyObject_GetAttrString(args[I-1], "_cgalpy_polyhedron_owner");
    if (owner == nullptr) { PyErr_Clear(); return; }
    if (owner != Py_None) attach_lease(owner,args[0]); Py_DECREF(owner);
  }
};
template <typename Polyhedron> auto checked_halfedge_handle(Polyhedron& polyhedron,
  typename Polyhedron::Halfedge& halfedge) -> typename Polyhedron::Halfedge_handle {
  for (auto it=polyhedron.halfedges_begin(); it!=polyhedron.halfedges_end(); ++it)
    if (std::addressof(*it) == std::addressof(halfedge)) return it;
  throw std::invalid_argument("Halfedge does not belong to this Polyhedron_3.");
}

} // namespace cgalpy::pol3::lifetime
#endif
