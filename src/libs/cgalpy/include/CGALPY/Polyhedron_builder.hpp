// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYHEDRON_BUILDER_HPP
#define CGALPY_POLYHEDRON_BUILDER_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Modifier_base.h>

#include "CGALPY/polyhedron_3_types.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace pol3 {

class Polyhedron_builder : public CGAL::Modifier_base<Halfedge_ds> {
private:
  py::object m_data;
  py::object m_operator = py::none();

public:
  Polyhedron_builder(py::object data) { m_data = data; }
  void set_operator(py::object op) { m_operator = op; }
  void set_data(py::object data) { m_data = data; }

  void operator()(Halfedge_ds& hds)
  { if (! m_operator.is_none()) m_operator(&hds, m_data); }

  py::object data() const { return m_data; }
};

}
} // namespace cgalpy

#endif
