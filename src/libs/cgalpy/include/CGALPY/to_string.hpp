// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TO_STRING_HPP
#define CGALPY_TO_STRING_HPP

#include <string>
#include <sstream>

template <typename T>
std::string std::to_string(const T& n) {
  std::ostringstream os;
  os << n;
  return os.str();
}

#endif
