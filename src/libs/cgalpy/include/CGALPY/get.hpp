// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_GET_HPP
#define CGALPY_GET_HPP

#include <CGAL/boost/graph/properties.h>

#include <nanobind/nanobind.h>

namespace bgl {

//! Obtain a property map attached to a graph
template <typename PropertyMapTag, typename Graph>
auto get(const PropertyMapTag& tag, const Graph& g) { return CGAL::get(tag, g); }

}

#endif
