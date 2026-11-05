// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_APPLICATOR_HPP
#define CGALPY_NAMED_PARAMETER_APPLICATOR_HPP

/*! \file named_parameter_applicator.hpp
 * \brief Machinery that converts a Python dictionary of named parameters
 *        into a CGAL `Named_function_parameters` object and invokes a wrapped
 *        CGAL function with it.
 *
 * CGAL named parameters are a compile-time construct: every call such as
 * `CGAL::parameters::vertex_point_map(vpm).geom_traits(gt)` produces an
 * object of a *different* type. A Python dictionary, on the other hand, is
 * only known at run time. Therefore, the applicator cannot build the final
 * named-parameter object in a variable and then call the function. Instead,
 * it uses continuation-passing style: for every supported parameter (an
 * *op*), it checks whether the dictionary contains the corresponding key; if
 * so, it extends the named-parameter object and recurses with the new
 * (differently typed) object, otherwise it recurses with the unchanged
 * object. When all ops have been consumed, the continuation (and eventually
 * the wrapper) is invoked with the final object.
 *
 * As a result, the compiler instantiates one code path for every subset of
 * the supported ops (2^N paths for N ops), and the run-time dictionary
 * selects which path is taken.
 *
 * \section np_op_concept The NamedParameterOp concept
 *
 * Every op passed to the applicator must model the following concept:
 * - A static data member `m_name`, comparable with `std::string`
 *   (e.g., `static constexpr const char* m_name = "vertex_point_map";`),
 *   holding the Python key that selects this op.
 * - A const call operator `auto operator()(const NP& np, py::handle value) const`
 *   that returns `np` extended with the parameter converted from `value`,
 *   e.g., `return np.vertex_point_map(py::cast<Vpm>(value));`.
 *   It must accept any `CGAL::Named_function_parameters` type (i.e., be a
 *   template).
 *
 * \section np_wrapper_concept The Wrapper concept
 *
 * The wrapper is a callable that receives the final named-parameter
 * object(s) and invokes the wrapped CGAL function (see, e.g.,
 * `Named_parameter_wrapper`). The applicators invoke it **as an rvalue**
 * (`std::move(wrapper)(...)`), exactly once; callers must not rely on the
 * state of the wrapper after the call.
 *
 * \section np_return Return values
 *
 * All entry points return `decltype(auto)`, that is, exactly what the
 * wrapper returns. If the wrapped CGAL function is `void`, the applicator
 * returns `void` as well. In that case the result is typically produced
 * through an output argument bound in the wrapper, and the binding function
 * must return that argument explicitly:
 * \code
 * Pm ch;
 * Named_parameter_wrapper<Convex_hull_3_wrapper, const Pm&, Pm&> wrapper(pm, ch);
 * named_parameter_applicator(wrapper, np, params, vpm_op, gt_op);
 * return ch;   // do not omit; the applicator returns void here
 * \endcode
 */

#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <nanobind/nanobind.h>

#include <CGAL/Named_function_parameters.h>

namespace py = nanobind;

namespace cgalpy {

/*! \brief Verifies that every key in a Python dictionary is supported by one
 *         of the given ops.
 *
 * The ops are used only for their types (their static `m_name` members);
 * the arguments themselves are unused and serve only to deduce the pack.
 *
 * \tparam NamedParameterOps the op types; each must model the
 *         \ref np_op_concept "NamedParameterOp" concept.
 * \param params the Python dictionary of named parameters.
 * \throw std::runtime_error if `params` contains a key that does not match
 *        the `m_name` of any op. Without this check, misspelled parameters
 *        would be silently ignored.
 */
template <typename... NamedParameterOps>
void validate_params(const py::dict& params, const NamedParameterOps&...) {
  const std::set<std::string> known = { NamedParameterOps::m_name... };
  for (const auto& item : params) {
    const std::string key = py::str(item.first).c_str();
    if (known.count(key) == 0) {
      throw std::runtime_error("Unknown named parameter: '" + key + "'");
    }
  }
}

/*! \brief A group of ops together with the Python dictionary they consume.
 *
 * A group produces a single `CGAL::Named_function_parameters` object. Use
 * several groups with `multi_np_applicator()` when the wrapped CGAL function
 * accepts more than one named-parameter argument (e.g., one for each input
 * mesh).
 *
 * \tparam Ops the op types; each must model the
 *         \ref np_op_concept "NamedParameterOp" concept.
 *
 * \note `params` is held by value. A `py::dict` is a reference-counted
 *       handle, so the copy is cheap, and the group remains valid even if it
 *       was constructed from a temporary dictionary.
 */
template <typename... Ops>
struct Np_group {
  py::dict params;        //!< the Python dictionary of named parameters
  std::tuple<Ops...> ops; //!< the ops that may consume entries of `params`
};

/*! \brief Constructs an `Np_group`, deducing the op types.
 *
 * \param params the Python dictionary of named parameters.
 * \param ops the ops; they are copied or moved into the group.
 * \return the group.
 */
template <typename... Ops>
auto make_np_group(const py::dict& params, Ops&&... ops) {
  return Np_group<std::decay_t<Ops>...>{ params, std::make_tuple(std::forward<Ops>(ops)...) };
}

namespace detail {

/*! \brief Recursion terminator of `apply_group()`: all ops consumed.
 *
 * \param continuation the callable that receives the final named-parameter object.
 * \param np the final named-parameter object.
 * \return whatever `continuation(np)` returns (possibly `void`).
 */
template <typename Continuation, typename NamedParameter>
decltype(auto) apply_group(Continuation& continuation, NamedParameter& np, const py::dict&)
{ return continuation(np); }

/*! \brief Applies the first op (if its key is present) and recurses on the
 *         remaining ops.
 *
 * If `params` contains the key `NamedParameterOp::m_name`, `op` is invoked to
 * extend `np` with the associated value, and the recursion continues with
 * the extended object (whose type differs from that of `np`). Otherwise, the
 * recursion continues with `np` unchanged. Both branches are instantiated;
 * the dictionary selects one at run time. Both branches deduce the same
 * return type, because they eventually call the same continuation.
 *
 * \tparam Continuation a callable accepting any named-parameter object.
 * \tparam NamedParameter the type of the current named-parameter object.
 * \tparam NamedParameterOp the type of the op handled at this level.
 * \tparam NamedParameterOps the types of the remaining ops.
 * \param continuation the callable that receives the final named-parameter object.
 * \param np the current named-parameter object.
 * \param params the Python dictionary of named parameters.
 * \param op the op handled at this level.
 * \param ops the remaining ops.
 * \return whatever the continuation returns (possibly `void`).
 */
template <typename Continuation, typename NamedParameter, typename NamedParameterOp, typename... NamedParameterOps>
decltype(auto) apply_group(Continuation& continuation, NamedParameter& np, const py::dict& params,
                           const NamedParameterOp& op, const NamedParameterOps&... ops) {
  for (const auto& item : params) {
    const std::string key = py::str(item.first).c_str();
    if (key == NamedParameterOp::m_name) {
      auto np_new = op(np, item.second);
      return apply_group(continuation, np_new, params, ops...);
    }
  }
  return apply_group(continuation, np, params, ops...);
}

/*! \brief Recursion terminator of `multi_np_applicator_impl()`: all groups
 *         consumed; invokes the wrapper.
 *
 * \tparam Wrapper the wrapper type; see the \ref np_wrapper_concept "Wrapper" concept.
 * \tparam NamedParametersTuple a tuple of references to the final
 *         named-parameter objects, one per group, in group order.
 * \param wrapper the wrapper; invoked as an rvalue.
 * \param named_parameters the final named-parameter objects.
 * \return whatever the wrapper returns (possibly `void`).
 */
template <typename Wrapper, typename NamedParametersTuple>
decltype(auto)
multi_np_applicator_impl(Wrapper& wrapper, NamedParametersTuple& named_parameters)
{ return std::apply([&](auto&... nps) -> decltype(auto) { return std::move(wrapper)(nps...); }, named_parameters); }

/*! \brief Builds the named-parameter object of the first group and recurses
 *         on the remaining groups.
 *
 * The dictionary of the group is validated first. Then `apply_group()` builds
 * the group's named-parameter object, starting from
 * `CGAL::parameters::default_values()`. Its continuation appends a reference
 * to the result to `named_parameters` and recurses on the remaining groups.
 * All named-parameter objects live on the stack of the recursion until the
 * wrapper returns, so storing references to them is safe.
 *
 * \tparam Wrapper the wrapper type; see the \ref np_wrapper_concept "Wrapper" concept.
 * \tparam NamedParametersTuple a tuple of references to the named-parameter
 *         objects built so far.
 * \tparam Ops the op types of the current group.
 * \tparam Groups the types of the remaining groups.
 * \param wrapper the wrapper.
 * \param named_parameters the named-parameter objects built so far.
 * \param group the current group.
 * \param groups the remaining groups.
 * \return whatever the wrapper returns (possibly `void`).
 * \throw std::runtime_error if a dictionary contains an unknown key.
 */
template <typename Wrapper, typename NamedParametersTuple, typename... Ops, typename... Groups>
decltype(auto) multi_np_applicator_impl(Wrapper& wrapper, NamedParametersTuple& named_parameters,
                                        Np_group<Ops...>& group, Groups&... groups) {
  auto np = CGAL::parameters::default_values();
  return std::apply([&](const auto&... ops) -> decltype(auto) {
       validate_params(group.params, ops...);
       auto continuation = [&](auto& np_final) -> decltype(auto) {
         auto next_named_parameters = std::tuple_cat(named_parameters, std::forward_as_tuple(np_final));
         return multi_np_applicator_impl(wrapper, next_named_parameters, groups...);
       };

       return apply_group(continuation, np, group.params, ops...);
     },
     group.ops);
}

} // namespace detail

/*! \brief Converts a Python dictionary into a CGAL named-parameter object
 *         and invokes the wrapper with it.
 *
 * Use this entry point when the wrapped CGAL function accepts a single
 * named-parameter argument.
 *
 * Example:
 * \code
 * auto np = CGAL::parameters::default_values();
 * Named_parameter_vertex_point_map<Pm> vpm_op;
 * Named_parameter_geom_traits gt_op;
 * Named_parameter_wrapper<Convex_hull_3_wrapper, const Pm&, Pm&> wrapper(pm, ch);
 * named_parameter_applicator(wrapper, np, params, vpm_op, gt_op);
 * \endcode
 *
 * \tparam Wrapper the wrapper type; see the \ref np_wrapper_concept "Wrapper" concept.
 * \tparam NamedParameter the type of the initial named-parameter object,
 *         typically that of `CGAL::parameters::default_values()`.
 * \tparam NamedParameterOps the op types; each must model the
 *         \ref np_op_concept "NamedParameterOp" concept.
 * \param wrapper the wrapper; invoked once, as an rvalue.
 * \param np the initial named-parameter object; extended (in copies) by the ops.
 * \param params the Python dictionary of named parameters.
 * \param ops the supported ops. The order of the ops determines the order in
 *        which parameters are appended to the named-parameter object.
 * \return whatever the wrapper returns. If the wrapped function is `void`,
 *         this function returns `void`, and the caller must return any
 *         output argument itself (see \ref np_return "Return values").
 * \throw std::runtime_error if `params` contains an unknown key.
 */
template <typename Wrapper, typename NamedParameter, typename... NamedParameterOps>
decltype(auto) named_parameter_applicator(Wrapper& wrapper, NamedParameter& np, const py::dict& params,
                                          const NamedParameterOps&... ops) {
  validate_params(params, ops...);
  auto continuation = [&](auto& np_final) -> decltype(auto) { return std::move(wrapper)(np_final); };
  return detail::apply_group(continuation, np, params, ops...);
}

/*! \brief Converts several Python dictionaries into CGAL named-parameter
 *         objects and invokes the wrapper with all of them.
 *
 * Use this entry point when the wrapped CGAL function accepts more than one
 * named-parameter argument. Each group yields one named-parameter object;
 * the wrapper receives them in group order.
 *
 * Example:
 * \code
 * multi_np_applicator(wrapper,
 *                     make_np_group(np1, Named_parameter_vertex_point_map<Pm>()),
 *                     make_np_group(np2, Named_parameter_vertex_point_map<Pm>()));
 * \endcode
 *
 * \tparam Wrapper the wrapper type; see the \ref np_wrapper_concept "Wrapper" concept.
 * \tparam Groups the group types; each must be an instance of `Np_group`.
 * \param wrapper the wrapper; invoked once, as an rvalue, with one
 *        named-parameter object per group.
 * \param groups the groups, typically created with `make_np_group()`.
 * \return whatever the wrapper returns (possibly `void`; see
 *         \ref np_return "Return values").
 * \throw std::runtime_error if any dictionary contains an unknown key.
 */
template <typename Wrapper, typename... Groups>
decltype(auto)
multi_np_applicator(Wrapper& wrapper, Groups&&... groups) {
  auto named_parameters = std::tuple<>();
  return detail::multi_np_applicator_impl(wrapper, named_parameters, groups...);
}

} // namespace cgalpy

#endif
