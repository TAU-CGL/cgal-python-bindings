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

template <typename Base_, typename Data_>
class Hds_halfedge_with_data : public Base_ {
public:
  using Base = Base_;
  using Data = Data_;

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
template <typename Base, typename Data>
Hds_halfedge_with_data<Base, Data>::Hds_halfedge_with_data() : m_data() {}

//! \brief constructs from a halfedge data.
template <typename Base, typename Data>
Hds_halfedge_with_data<Base, Data>::
Hds_halfedge_with_data(const Data& data) : m_data(data) {}

//! \brief obtains a non-const reference of the halfedge data.
template <typename Base, typename Data>
Data& Hds_halfedge_with_data<Base, Data>::data() { return m_data; }

//! \brief obtains a const reference of the halfedge data.
template <typename Base, typename Data>
const Data& Hds_halfedge_with_data<Base, Data>::data() const
{ return m_data; }

//! \brief sets the general purpose halfedge data.
template <typename Base, typename Data>
void Hds_halfedge_with_data<Base, Data>::set_data(const Data& data)
{ m_data = data; }

#endif
