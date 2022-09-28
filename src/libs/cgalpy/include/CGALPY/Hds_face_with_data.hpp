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

template <typename Refs_, typename Plane_, typename Data_>
class Hds_face_with_data :
  public CGAL::HalfedgeDS_face_base<Refs_, CGAL::Tag_true, Plane_>
{
public:
  using Refs = Refs_;
  using Data = Data_;
  using Plane = Plane_;
  using Base = CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane>;

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
  Hds_face_with_data(Plane const& p);

  /*! Construct from a plane and a face data.
   */
  Hds_face_with_data(Plane const& p, const Data& data);

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
template <typename Refs, typename Plane, typename Data>
Hds_face_with_data<Refs, Plane, Data>::Hds_face_with_data() : m_data() {}

//! \brief constructs from a plane.
template <typename Refs, typename Plane, typename Data>
Hds_face_with_data<Refs, Plane, Data>::Hds_face_with_data(Plane const& p) :
  Base(p),
  m_data()
{}

//! \brief constructs from a plane and a face data.
template <typename Refs, typename Plane, typename Data>
Hds_face_with_data<Refs, Plane, Data>::
Hds_face_with_data(Plane const& p,const Data& data) :
  Base(p),
  m_data(data)
{}

//! \brief obtains a non-const reference of the face data.
template <typename Refs, typename Plane, typename Data>
Data& Hds_face_with_data<Refs, Plane, Data>::data() { return m_data; }

//! \brief obtains a const reference of the face data.
template <typename Refs, typename Plane, typename Data>
const Data& Hds_face_with_data<Refs, Plane, Data>::data() const
{ return m_data; }

//! \brief sets the general purpose face data.
template <typename Refs, typename Plane, typename Data>
void Hds_face_with_data<Refs, Plane, Data>::set_data(const Data& data)
{ m_data = data; }

#endif
