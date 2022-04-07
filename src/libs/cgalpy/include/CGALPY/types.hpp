// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TYPES_HPP
#define CGALPY_TYPES_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/config.hpp"

namespace py = nanobind;

#if 0
typedef py::return_value_policy<py::copy_const_reference> Copy_const_reference;
typedef py::return_value_policy<py::return_by_value>      Return_by_value;
typedef py::return_value_policy<py::manage_new_object>    Manage_new_object;
typedef py::return_value_policy<py::reference_existing_object>
  Reference_existing_object;
typedef py::return_value_policy<py::copy_non_const_reference>
  Copy_non_const_reference;
#endif

constexpr bool kernel_bindings()
{ return DETECT_EXIST(CGALPY_KERNEL_BINDINGS); }

constexpr bool kernel_d_bindings()
{ return DETECT_EXIST(CGALPY_KERNEL_D_BINDINGS); }

constexpr bool arrangement_on_surface_2_bindings()
{ return DETECT_EXIST(CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS); }

constexpr bool alpha_shape_2_bindings()
{ return DETECT_EXIST(CGALPY_ALPHA_SHAPE_2_BINDINGS); }

constexpr bool alpha_shape_3_bindings()
{ return DETECT_EXIST(CGALPY_ALPHA_SHAPE_3_BINDINGS); }

constexpr bool boolean_set_operations_2_bindings()
{ return DETECT_EXIST(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS); }

constexpr bool bounding_volumes_bindings()
{ return DETECT_EXIST(CGALPY_BOUNDING_VOLUMES_BINDINGS); }

constexpr bool convex_hull_2_bindings()
{ return DETECT_EXIST(CGALPY_CONVEX_HULL_2_BINDINGS); }

constexpr bool convex_hull_3_bindings()
{ return DETECT_EXIST(CGALPY_CONVEX_HULL_3_BINDINGS); }

constexpr bool polygon_2_bindings()
{ return DETECT_EXIST(CGALPY_POLYGON_2_BINDINGS); }

constexpr bool polygon_partitioning_bindings()
{ return DETECT_EXIST(CGALPY_POLYGON_PARTITIONING_BINDINGS); }

constexpr bool minkowski_sum_2_bindings()
{ return DETECT_EXIST(CGALPY_MINKOWSKI_SUM_2_BINDINGS); }

constexpr bool spatial_searching_bindings()
{ return DETECT_EXIST(CGALPY_SPATIAL_SEARCHING_BINDINGS); }

constexpr bool triangulation_2_bindings()
{ return DETECT_EXIST(CGALPY_TRIANGULATION_2_BINDINGS); }

constexpr bool triangulation_3_bindings()
{ return DETECT_EXIST(CGALPY_TRIANGULATION_3_BINDINGS); }

#endif
