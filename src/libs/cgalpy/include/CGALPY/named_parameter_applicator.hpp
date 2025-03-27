// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_APPLICATOR_HPP
#define CGALPY_NAMED_PARAMETER_APPLICATOR_HPP

namespace CGALPY {

//!
template <typename Wrapper, typename NamedParameter>
auto named_parameter_applicator(Wrapper& wrapper, NamedParameter& np,
                                const py::dict& params)
{ return wrapper(np); }

//!
template <typename Wrapper, typename NamedParameter,
          typename NamedParameterOp, typename... NamedParameterOps>
auto named_parameter_applicator(Wrapper& wrapper, NamedParameter& np,
                                const py::dict& params, NamedParameterOp op,
                                NamedParameterOps... ops) {
  for (const auto& item : params) {
    const std::string& key = py::cast<std::string>(item.first);
    if (key == op.m_name) {
      auto np_new = op(np, item.second);
      return named_parameter_applicator(wrapper, np_new, params, ops...);
    }
  }
  return named_parameter_applicator(wrapper, np, params, ops...);
}

}

#endif
