// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_SPHERICAL_BOUNDARY_TRAITS_CLASSES_HPP
#define CGALPY_AOS_SPHERICAL_BOUNDARY_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Aos_spherical_boundary_traits_classes {
  using Parameter_space_in_x_2 = typename T::Parameter_space_in_x_2;
  using Compare_y_near_boundary_2 = typename T::Compare_y_near_boundary_2;
  using Parameter_space_in_y_2 = typename T::Parameter_space_in_y_2;
  using Compare_x_on_boundary_2 = typename T::Compare_x_on_boundary_2;
  using Compare_x_near_boundary_2 = typename T::Compare_x_near_boundary_2;
  using Is_on_y_identification_2 = typename T::Is_on_y_identification_2;

  // Constructor
  Aos_spherical_boundary_traits_classes() :
    m_parameter_space_in_x_2(nullptr),
    m_compare_y_near_boundary_2(nullptr),
    m_parameter_space_in_y_2(nullptr),
    m_compare_x_on_boundary_2(nullptr),
    m_compare_x_near_boundary_2(nullptr),
    m_is_on_y_identification_2(nullptr)
  {}

  // Destructor
  ~Aos_spherical_boundary_traits_classes() {
    if (m_parameter_space_in_x_2) delete m_parameter_space_in_x_2;
    if (m_compare_y_near_boundary_2) delete m_compare_y_near_boundary_2;
    if (m_parameter_space_in_y_2) delete m_parameter_space_in_y_2;
    if (m_compare_x_on_boundary_2) delete m_compare_x_on_boundary_2;
    if (m_compare_x_near_boundary_2) delete m_compare_x_near_boundary_2;
    if (m_is_on_y_identification_2) delete m_is_on_y_identification_2;
  }

  // Class objects
  py::class_<Parameter_space_in_x_2>* m_parameter_space_in_x_2;
  py::class_<Compare_y_near_boundary_2>* m_compare_y_near_boundary_2;
  py::class_<Parameter_space_in_y_2>* m_parameter_space_in_y_2;
  py::class_<Compare_x_on_boundary_2>* m_compare_x_on_boundary_2;
  py::class_<Compare_x_near_boundary_2>* m_compare_x_near_boundary_2;
  py::class_<Is_on_y_identification_2>* m_is_on_y_identification_2;
};

#endif
