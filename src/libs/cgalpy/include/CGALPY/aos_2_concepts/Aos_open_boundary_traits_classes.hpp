// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_OPEN_BOUNDARY_TRAITS_CLASSES_HPP
#define CGALPY_AOS_OPEN_BOUNDARY_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Aos_open_boundary_traits_classes {
  typedef typename T::Parameter_space_in_x_2    Parameter_space_in_x_2;
  typedef typename T::Compare_y_near_boundary_2 Compare_y_near_boundary_2;
  typedef typename T::Parameter_space_in_y_2    Parameter_space_in_y_2;
  typedef typename T::Compare_x_at_limit_2      Compare_x_at_limit_2;
  typedef typename T::Compare_x_near_limit_2    Compare_x_near_limit_2;

  // Constructor
  Aos_open_boundary_traits_classes() :
    m_parameter_space_in_x_2(nullptr),
    m_compare_y_near_boundary_2(nullptr),
    m_parameter_space_in_y_2(nullptr),
    m_compare_x_at_limit_2(nullptr),
    m_compare_x_near_limit_2(nullptr)
  {}

  // Destructor
  ~Aos_open_boundary_traits_classes() {
    if (m_parameter_space_in_x_2) delete m_parameter_space_in_x_2;
    if (m_compare_y_near_boundary_2) delete m_compare_y_near_boundary_2;
    if (m_parameter_space_in_y_2) delete m_parameter_space_in_y_2;
    if (m_compare_x_at_limit_2) delete m_compare_x_at_limit_2;
    if (m_compare_x_near_limit_2) delete m_compare_x_near_limit_2;
  }

  // Class objects
  py::class_<Parameter_space_in_x_2>* m_parameter_space_in_x_2;
  py::class_<Compare_y_near_boundary_2>* m_compare_y_near_boundary_2;
  py::class_<Parameter_space_in_y_2>* m_parameter_space_in_y_2;
  py::class_<Compare_x_at_limit_2>* m_compare_x_at_limit_2;
  py::class_<Compare_x_near_limit_2>* m_compare_x_near_limit_2;
};

#endif
