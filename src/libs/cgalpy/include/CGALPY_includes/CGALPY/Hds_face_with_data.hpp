// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HDS_FACE_WITH_DATA_HPP
#define CGALPY_HDS_FACE_WITH_DATA_HPP

#include <CGAL/HalfedgeDS_face_base.h>

// Vertex with data

template <typename Base_, typename Data_>
class Hds_face_with_data : Base_ {
public:
  using Base = Base_;
  using Data = Data_;
  using Plane_3 = typename Base::Plane_3;

private:
  /*! General purpose data.
   */
  Data m_data;

public:
  /*! Construct default.
   */
  Hds_face_with_data();

  /*! Construct from a plane.
   */
  Hds_face_with_data(Plane_3 const& p);

  /*! Construct from a plane and a face data.
   */
  Hds_face_with_data(Plane_3 const& p, const Data& data);

  /*! Obtain a non-const reference of the face data.
   */
  Data& data();

  /*! Obtain a const reference of the face data.
   */
  const Data& data() const;

  /*! Set the general purpose face data.
   */
  void set_data(const Data& data);
};

//! \brief constructs default
template <typename Base, typename Data>
Hds_face_with_data<Base, Data>::Hds_face_with_data() : m_data() {}

//! \brief constructs from a plane.
template <typename Base, typename Data>
Hds_face_with_data<Base, Data>::Hds_face_with_data(Plane_3 const& p) :
  Base(p),
  m_data()
{}

//! \brief constructs from a plane and a face data.
template <typename Base, typename Data>
Hds_face_with_data<Base, Data>::
Hds_face_with_data(Plane_3 const& p, const Data& data) :
  Base(p),
  m_data(data)
{}

//! \brief obtains a non-const reference of the face data.
template <typename Base, typename Data>
Data& Hds_face_with_data<Base, Data>::data() { return m_data; }

//! \brief obtains a const reference of the face data.
template <typename Base, typename Data>
const Data& Hds_face_with_data<Base, Data>::data() const
{ return m_data; }

//! \brief sets the general purpose face data.
template <typename Base, typename Data>
void Hds_face_with_data<Base, Data>::set_data(const Data& data)
{ m_data = data; }

#endif
