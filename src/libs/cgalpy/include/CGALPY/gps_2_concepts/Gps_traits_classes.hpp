// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_TRAITS_CLASSES_HPP
#define CGALPY_AOS_TRAITS_CLASSES_HPP

template <typename T>
struct Gps_traits_classes {
  typedef typename T::Polygon_2                         Polygon_2;
  typedef typename T::Polygon_with_holes_2              Polygon_with_holes_2;
  typedef typename T::Construct_polygon_2               Construct_polygon_2;
  typedef typename T::Construct_general_polygon_with_holes_2
    Construct_general_polygon_with_holes_2;
  typedef typename T::Construct_outer_boundary          Construct_outer_boundary;
  typedef typename T::Construct_holes                   Construct_holes;
  typedef typename T::Is_unbounded                      Is_unbounded;

  // Constructor
  Gps_traits_classes() :
    m_polygon_2(nullptr),
    m_polygon_with_holes_2(nullptr),
    m_construct_polygon_2(nullptr),
    m_construct_general_polygon_with_holes_2(nullptr),
    m_construct_outer_boundary(nullptr),
    m_construct_holes(nullptr),
    m_is_unbounded(nullptr)
  {}

  // Destructor
  ~Gps_traits_classes() {
    if (m_polygon_2) delete m_polygon_2;
    if (m_polygon_with_holes_2) delete m_polygon_with_holes_2;
    if (m_construct_polygon_2) delete m_construct_polygon_2;
    if (m_construct_general_polygon_with_holes_2)
      delete m_construct_general_polygon_with_holes_2;
    if (m_construct_outer_boundary) delete m_construct_outer_boundary;
    if (m_construct_holes) delete m_construct_holes;
    if (m_is_unbounded) delete m_is_unbounded;
  }

  bp::class_<Polygon_2>* m_polygon_2;
  bp::class_<Polygon_with_holes_2>* m_polygon_with_holes_2;
  bp::class_<Construct_polygon_2>* m_construct_polygon_2;
  bp::class_<Construct_general_polygon_with_holes_2>* m_construct_general_polygon_with_holes_2;
  bp::class_<Construct_outer_boundary>* m_construct_outer_boundary;
  bp::class_<Construct_holes>* m_construct_holes;
  bp::class_<Is_unbounded>* m_is_unbounded;
};

#endif
