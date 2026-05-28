// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_SPATIAL_SORTING_TYPES_HPP
#define CGALPY_SPATIAL_SORTING_TYPES_HPP

#include "CGALPY/spatial_sorting_config.hpp"

namespace cgalpy {
namespace st {

using Concurrency = Concurrency<CGALPY_ST_CONCURRENCY>::type;
using Policy = Policy<CGALPY_ST_POLICY>::type;

}
} // namespace cgalpy

#endif
