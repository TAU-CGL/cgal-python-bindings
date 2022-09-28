// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HDS_VERTEX_WITH_DATA_HPP
#define CGALPY_HDS_VERTEX_WITH_DATA_HPP

// Vertex with data

template <typename Refs_, typename Point_, typename Data_>
class Hds_vertex_with_data :
  public CGAL::HalfedgeDS_vertex_base<Refs_, CGAL::Tag_true, Point_>
{
public:
  using Refs = Refs_;
  using Data = Data_;
  using Point = Point_;
  using Base = CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point>;

private:
  /*! General purpose data.
   */
  Data m_data;

public:
  /*! Construct default.
   */
  Hds_vertex_with_data();

  /*! Construct from a point.
   */
  Hds_vertex_with_data(Point const& p);

  /*! Construct from a point and a vertex data.
   */
  Hds_vertex_with_data(Point const& p, const Data& data);

  /*! Obtain a non-const reference of the vertex data.
   */
  Data& data() { return m_data; }

  /*! Obtain a const reference of the vertex data.
   */
  const Data& data() const { return m_data; }

  /*! Set the general purpose vertex data.
   */
  void set_data(const Data& data) { m_data = data; }
};

//! \brief constructs default
template <typename Refs, typename Point, typename Data>
Hds_vertex_with_data<Refs, Point, Data>::Hds_vertex_with_data() : m_data() {}

//! \brief constructs from a point.
template <typename Refs, typename Point, typename Data>
Hds_vertex_with_data<Refs, Point, Data>::Hds_vertex_with_data(Point const& p) :
  Base(p),
  m_data()
{}

//! \brief constructs from a point and a vertex data.
template <typename Refs, typename Point, typename Data>
Hds_vertex_with_data<Refs, Point, Data>::
Hds_vertex_with_data(Point const& p,const Data& data) :
  Base(p),
  m_data(data)
{}

//! \brief obtains a non-const reference of the vertex data.
template <typename Refs, typename Point, typename Data>
Data& Hds_vertex_with_data<Refs, Point, Data>::data() { return m_data; }

//! \brief obtains a const reference of the vertex data.
template <typename Refs, typename Point, typename Data>
const Data& Hds_vertex_with_data<Refs, Point, Data>::data() const
{ return m_data; }

//! \brief sets the general purpose vertex data.
template <typename Refs, typename Point, typename Data>
void Hds_vertex_with_data<Refs, Point, Data>::set_data(const Data& data)
{ m_data = data; }

#endif
