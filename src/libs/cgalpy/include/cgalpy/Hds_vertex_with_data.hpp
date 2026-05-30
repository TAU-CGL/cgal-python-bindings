// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HDS_VERTEX_WITH_DATA_HPP
#define CGALPY_HDS_VERTEX_WITH_DATA_HPP

#include <CGAL/HalfedgeDS_vertex_base.h>

// Vertex with data

template <typename Base_, typename Data_>
class Hds_vertex_with_data : public Base_ {
public:
  using Base = Base_;
  using Data = Data_;
  using Point_3 = typename Base::Point;

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
  Hds_vertex_with_data(Point_3 const& p);

  /*! Construct from a point and a vertex data.
   */
  Hds_vertex_with_data(Point_3 const& p, const Data& data);

  /*! Obtain a non-const reference of the vertex data.
   */
  Data& data();

  /*! Obtain a const reference of the vertex data.
   */
  const Data& data() const;

  /*! Set the general purpose vertex data.
   */
  void set_data(const Data& data);
};

//! \brief constructs default
template <typename Base, typename Data>
Hds_vertex_with_data<Base, Data>::Hds_vertex_with_data() : m_data() {}

//! \brief constructs from a point.
template <typename Base, typename Data>
Hds_vertex_with_data<Base, Data>::Hds_vertex_with_data(Point_3 const& p) :
  Base(p),
  m_data()
{}

//! \brief constructs from a point and a vertex data.
template <typename Base, typename Data>
Hds_vertex_with_data<Base, Data>::
Hds_vertex_with_data(Point_3 const& p,const Data& data) :
  Base(p),
  m_data(data)
{}

//! \brief obtains a non-const reference of the vertex data.
template <typename Base, typename Data>
Data& Hds_vertex_with_data<Base, Data>::data() { return m_data; }

//! \brief obtains a const reference of the vertex data.
template <typename Base, typename Data>
const Data& Hds_vertex_with_data<Base, Data>::data() const
{ return m_data; }

//! \brief sets the general purpose vertex data.
template <typename Base, typename Data>
void Hds_vertex_with_data<Base, Data>::set_data(const Data& data)
{ m_data = data; }

#endif
