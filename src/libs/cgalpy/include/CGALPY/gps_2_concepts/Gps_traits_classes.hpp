// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_TRAITS_CLASSES_HPP
#define CGALPY_AOS_TRAITS_CLASSES_HPP

template <typename T>
struct Gps_traits_classes {
  using Polygon_2 = typename T::Polygon_2;
  using Polygon_with_holes_2 = typename T::Polygon_with_holes_2;
  using Construct_polygon_2 = typename T::Construct_polygon_2;
  using Construct_polygon_with_holes_2 =
    typename T::Construct_polygon_with_holes_2;
  using Construct_outer_boundary = typename T::Construct_outer_boundary;
  using Construct_holes = typename T::Construct_holes;
  using Is_unbounded = typename T::Is_unbounded;

  // Constructor
  Gps_traits_classes() :
    m_polygon_2(nullptr),
    m_polygon_with_holes_2(nullptr),
    m_construct_polygon_2(nullptr),
    m_construct_polygon_with_holes_2(nullptr),
    m_construct_outer_boundary(nullptr),
    m_construct_holes(nullptr),
    m_is_unbounded(nullptr)
  {}

  // Destructor
  ~Gps_traits_classes() {
    if (m_polygon_2) delete m_polygon_2;
    if (m_polygon_with_holes_2) delete m_polygon_with_holes_2;
    if (m_construct_polygon_2) delete m_construct_polygon_2;
    if (m_construct_polygon_with_holes_2)
      delete m_construct_polygon_with_holes_2;
    if (m_construct_outer_boundary) delete m_construct_outer_boundary;
    if (m_construct_holes) delete m_construct_holes;
    if (m_is_unbounded) delete m_is_unbounded;
  }

  py::class_<Polygon_2>* m_polygon_2;
  py::class_<Polygon_with_holes_2>* m_polygon_with_holes_2;
  py::class_<Construct_polygon_2>* m_construct_polygon_2;
  py::class_<Construct_polygon_with_holes_2>* m_construct_polygon_with_holes_2;
  py::class_<Construct_outer_boundary>* m_construct_outer_boundary;
  py::class_<Construct_holes>* m_construct_holes;
  py::class_<Is_unbounded>* m_is_unbounded;
};

#endif
