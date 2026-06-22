// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_TRIANGULATE_FACES_VISITOR_HPP
#define CGALPY_TRIANGULATE_FACES_VISITOR_HPP

#include <functional>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>

namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
namespace pmp {

//! Visitor wrapper for CGAL::Polygon_mesh_processing::triangulate_faces().
template <typename PolygonMesh>
struct Triangulate_faces_visitor :
  public PMP::Triangulate_faces::Default_visitor<PolygonMesh>
{
  using Base = PMP::Triangulate_faces::Default_visitor<PolygonMesh>;
  using Gt = boost::graph_traits<PolygonMesh>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;

  Triangulate_faces_visitor() = default;

  void before_subface_creations(Fd f)
  {
    if (before_subface_creations_fn) before_subface_creations_fn(f);
  }

  void after_subface_creations()
  {
    if (after_subface_creations_fn) after_subface_creations_fn();
  }

  void after_subface_created(Fd f)
  {
    if (after_subface_created_fn) after_subface_created_fn(f);
  }

  bool accept_face(Fd f, Vd v0, Vd v1, Vd v2) const
  {
    return accept_face_fn ? accept_face_fn(f, v0, v1, v2) :
      Base::accept_face(f, v0, v1, v2);
  }

  void set_before_subface_creations(const std::function<void(Fd)>& fn)
  { before_subface_creations_fn = fn; }

  void set_after_subface_creations(const std::function<void()>& fn)
  { after_subface_creations_fn = fn; }

  void set_after_subface_created(const std::function<void(Fd)>& fn)
  { after_subface_created_fn = fn; }

  void set_accept_face(const std::function<bool(Fd, Vd, Vd, Vd)>& fn)
  { accept_face_fn = fn; }

private:
  std::function<void(Fd)> before_subface_creations_fn;
  std::function<void()> after_subface_creations_fn;
  std::function<void(Fd)> after_subface_created_fn;
  std::function<bool(Fd, Vd, Vd, Vd)> accept_face_fn;
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_TRIANGULATE_FACES_VISITOR_HPP
