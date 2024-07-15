// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HDS_HALFEDGE_WITH_DATA_HPP
#define CGALPY_HDS_HALFEDGE_WITH_DATA_HPP

#include <CGAL/HalfedgeDS_halfedge_base.h>

// Halfedge with data

template <typename Refs_, typename Data_>
class Hds_halfedge_with_data :
  public CGAL::HalfedgeDS_halfedge_base<Refs_, CGAL::Tag_true, CGAL::Tag_true,
                                        CGAL::Tag_true>
{
public:
  using Refs = Refs_;
  using Data = Data_;
  using Base = CGAL::HalfedgeDS_halfedge_base<Refs, CGAL::Tag_true,
                                              CGAL::Tag_true, CGAL::Tag_true>;

private:
  /*! General purpose data.
   */
  Data m_data;

public:
  /*! Construct default.
   */
  Hds_halfedge_with_data();

  /*! Construct from a halfedge data.
   */
  Hds_halfedge_with_data(const Data& data);

  /*! Obtain a non-const reference of the halfedge data.
   */
  Data& data();

  /*! Obtain a const reference of the halfedge data.
   */
  const Data& data() const;

  /*! Set the general purpose halfedge data.
   */
  void set_data(const Data& data);
};

//! \brief constructs default
template <typename Refs, typename Data>
Hds_halfedge_with_data<Refs, Data>::Hds_halfedge_with_data() : m_data() {}

//! \brief constructs from a halfedge data.
template <typename Refs, typename Data>
Hds_halfedge_with_data<Refs, Data>::
Hds_halfedge_with_data(const Data& data) : m_data(data) {}

//! \brief obtains a non-const reference of the halfedge data.
template <typename Refs, typename Data>
Data& Hds_halfedge_with_data<Refs, Data>::data() { return m_data; }

//! \brief obtains a const reference of the halfedge data.
template <typename Refs, typename Data>
const Data& Hds_halfedge_with_data<Refs, Data>::data() const
{ return m_data; }

//! \brief sets the general purpose halfedge data.
template <typename Refs, typename Data>
void Hds_halfedge_with_data<Refs, Data>::set_data(const Data& data)
{ m_data = data; }

#endif
