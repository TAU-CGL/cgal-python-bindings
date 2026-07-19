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

#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <CGAL/Named_function_parameters.h>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

template <typename... NamedParameterOps>
void validate_params(const py::dict& params,
                     const NamedParameterOps&... ops) {
  const std::set<std::string> known = {
    NamedParameterOps::m_name...
  };

  for (const auto& item : params) {
    const std::string key = py::str(item.first).c_str();

    if (known.count(key) == 0) {
      throw std::runtime_error(
        "Unknown named parameter: '" + key + "'");
    }
  }
}

template <typename... Ops>
struct Np_group {
  const py::dict& params;
  std::tuple<Ops...> ops;
};

template <typename... Ops>
auto make_np_group(const py::dict& params, Ops&&... ops) {
  return Np_group<std::decay_t<Ops>...>{
    params,
    std::make_tuple(std::forward<Ops>(ops)...)
  };
}

namespace detail {

template <typename Continuation, typename NamedParameter>
decltype(auto)
apply_group(Continuation& continuation,
            NamedParameter& np,
            const py::dict&) {
  return continuation(np);
}

template <typename Continuation, typename NamedParameter,
          typename NamedParameterOp, typename... NamedParameterOps>
decltype(auto)
apply_group(Continuation& continuation,
            NamedParameter& np,
            const py::dict& params,
            const NamedParameterOp& op,
            const NamedParameterOps&... ops) {
  for (const auto& item : params) {
    const std::string key = py::str(item.first).c_str();

    if (key == NamedParameterOp::m_name) {
      auto np_new = op(np, item.second);

      return apply_group(continuation,
                         np_new,
                         params,
                         ops...);
    }
  }

  return apply_group(continuation,
                     np,
                     params,
                     ops...);
}

template <typename Wrapper, typename NamedParametersTuple>
decltype(auto)
multi_np_applicator_impl(Wrapper& wrapper,
                         NamedParametersTuple& named_parameters) {
  return std::apply(
    [&](auto&... nps) -> decltype(auto) {
      return std::move(wrapper)(nps...);
    },
    named_parameters);
}

template <typename Wrapper, typename NamedParametersTuple,
          typename... Ops, typename... Groups>
decltype(auto)
multi_np_applicator_impl(Wrapper& wrapper,
                         NamedParametersTuple& named_parameters,
                         Np_group<Ops...>& group,
                         Groups&... groups) {
  auto np = CGAL::parameters::default_values();

  return std::apply(
    [&](const auto&... ops) -> decltype(auto) {
      validate_params(group.params, ops...);

      auto continuation =
        [&](auto& np_final) -> decltype(auto) {
          auto next_named_parameters =
            std::tuple_cat(named_parameters,
                           std::forward_as_tuple(np_final));

          return multi_np_applicator_impl(
            wrapper,
            next_named_parameters,
            groups...);
        };

      return apply_group(continuation,
                         np,
                         group.params,
                         ops...);
    },
    group.ops);
}

} // namespace detail

template <typename Wrapper, typename NamedParameter,
          typename... NamedParameterOps>
decltype(auto)
named_parameter_applicator(Wrapper& wrapper,
                           NamedParameter& np,
                           const py::dict& params,
                           const NamedParameterOps&... ops) {
  validate_params(params, ops...);

  auto continuation =
    [&](auto& np_final) -> decltype(auto) {
      return std::move(wrapper)(np_final);
    };

  return detail::apply_group(continuation,
                             np,
                             params,
                             ops...);
}

template <typename Wrapper, typename... Groups>
decltype(auto)
multi_np_applicator(Wrapper& wrapper, Groups&&... groups) {
  auto named_parameters = std::tuple<>();

  return detail::multi_np_applicator_impl(
    wrapper,
    named_parameters,
    groups...);
}

} // namespace cgalpy

#endif
