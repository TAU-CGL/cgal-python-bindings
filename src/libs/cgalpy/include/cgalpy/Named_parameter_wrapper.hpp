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
#include <utility>  // for std::apply

namespace cgalpy {

// Define Named_parameter_wrapper, which takes a class template as a template
// template parameter
template <template <typename...> class FuncTemplate, typename... StoredArgs>
struct Named_parameter_wrapper {
  std::tuple<StoredArgs...> data;  // Store variadic arguments in a tuple

  // Constructor to initialize the tuple with arguments
  Named_parameter_wrapper(StoredArgs&&... args) :
    data(std::forward<StoredArgs>(args)...) {}

  // Function to call the stored function template with an additional parameter
  // np
  template <typename NPType>
  auto operator()(NPType&& np) {
    return std::apply([&np](StoredArgs&&... tupleArgs) {
                      return FuncTemplate<NPType, StoredArgs...>::
                        call(std::forward<NPType>(np),
                             std::forward<StoredArgs>(tupleArgs)...);
                      },
      std::move(data));  // move the tuple to enable perfect forwarding
  }
};

}

#endif
