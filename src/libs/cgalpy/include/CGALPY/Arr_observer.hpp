// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARR_OBSERVER_HPP
#define CGALPY_ARR_OBSERVER_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename Aos, typename Data>
class Arr_observer : public CGAL::Arr_observer<Aos> {
public:
  typedef typename Aos::Vertex_handle   Vertex_handle;
  typedef typename Aos::Halfedge_handle Halfedge_handle;
  typedef typename Aos::Face_handle     Face_handle;

  // Construct default
  Arr_observer() {}

  // Construct
  Arr_observer(Aos& arr) : CGAL::Arr_observer<Aos>(arr) {}

  // Destruct
  ~Arr_observer() {}

  /// Setters
  /// @{

  void set_after_split_face(py::object fnc) { m_after_split_face = fnc; }

  /// @}

  /// Operations
  /// @{

  virtual void after_split_face(Face_handle f, Face_handle new_f, bool is_hole)
  {
    // typename Aos::Face& tmp = *new_f;
    // m_after_split_face(*f, tmp, is_hole);
    // new_f->set_data(py::cast(10));
  }

  /// @}

private:
  py::object m_after_split_face;
};

#endif
