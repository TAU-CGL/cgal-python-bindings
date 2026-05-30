// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_FT_HPP
#define CGALPY_EXPORT_FT_HPP

#include "cgalpy/Kernel/export_rt.hpp"

namespace py = nanobind;

//
template <typename C>
void export_ft(C& c) { export_rt(c); }

#endif
