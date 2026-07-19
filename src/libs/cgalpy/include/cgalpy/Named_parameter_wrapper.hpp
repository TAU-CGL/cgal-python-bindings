// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_WRAPPER_HPP
#define CGALPY_NAMED_PARAMETER_WRAPPER_HPP

#include <tuple>
#include <utility>

namespace cgalpy {

// Stores the non-named-parameter arguments of a wrapped function template.
// The wrapper is single-use: invoking operator() consumes the stored tuple.
template <template <typename...> class FuncTemplate, typename... StoredArgs>
struct Named_parameter_wrapper {
  std::tuple<StoredArgs...> data;

  Named_parameter_wrapper(StoredArgs&&... args) :
    data(std::forward<StoredArgs>(args)...) {}

  template <typename... NPTypes>
  auto operator()(NPTypes&&... nps) && {
    return std::apply(
      [&](StoredArgs&&... tuple_args) {
        return FuncTemplate<NPTypes..., StoredArgs...>::
          call(std::forward<NPTypes>(nps)...,
               std::forward<StoredArgs>(tuple_args)...);
      },
      std::move(data));
  }
};

} // namespace cgalpy

#endif
