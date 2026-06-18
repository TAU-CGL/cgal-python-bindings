// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_SPATIAL_SEARCHING_TYPES_HPP
#define CGALPY_SPATIAL_SEARCHING_TYPES_HPP

#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/Search_traits_d.h>
#include <CGAL/K_neighbor_search.h>
#include <CGAL/Euclidean_distance.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Fuzzy_sphere.h>
#include <CGAL/Kd_tree.h>

#include "cgalpy/spatial_searching_config.hpp"
#include "cgalpy/kernel_d_types.hpp"
#include "cgalpy/General_distance_python.hpp"

namespace cgalpy {
namespace ss {

using Search_traits_d = CGAL::Search_traits_d<Kernel_d, Dimension_tag>;
// using Orthogonal_incremental_neighbor_search = CGAL::Orthogonal_incremental_neighbor_search<Search_traits_d>;
// using NN_iterator = Orthogonal_incremental_neighbor_search::iterator;
// using Orthogonal_incremental_neighbor_search_tree = Orthogonal_incremental_neighbor_search::Tree;
using Kd_tree = CGAL::Kd_tree<Search_traits_d>;
using Splitter = CGAL::Sliding_midpoint<Search_traits_d>;
using Fuzzy_iso_box = CGAL::Fuzzy_iso_box<Search_traits_d>;
using Fuzzy_sphere = CGAL::Fuzzy_sphere<Search_traits_d>;
using Kd_tree_rectangle = CGAL::Kd_tree_rectangle<FT_d, Dimension_tag>;
using K_neighbor_search = CGAL::K_neighbor_search<Search_traits_d>;
using Distance_python = General_distance_python<Dimension_tag, FT_d, Point_d, Point_d>;
using K_neighbor_search_python = CGAL::K_neighbor_search<Search_traits_d, Distance_python>;
using Euclidean_distance = CGAL::Euclidean_distance<Search_traits_d>;

}
}

#endif
