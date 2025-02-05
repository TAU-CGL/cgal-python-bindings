// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_FACE_PLANE_3_MAP_HPP
#define CGALPY_FACE_PLANE_3_MAP_HPP

namespace pol3 {

/*! An internal propery map from faces of a polyhedron of type `Polyhedron` to
 * objects of type `Polyhedron::Plane_3`. It is suitable for instances of
 * `Polyhedron_3` only. (`Surface mesh` does not have internal maps.)
 * It can be used when the Face(t) type of the polyhedron type is extended
 * with a field that represents the normal of the facet or the plane that
 * contains the facet.
 */
template <typename Polyhedron_>
class Internal_face_plane_3_map {
  using Polyhedron = Polyhedron_;
  using Plane_3 = typename Polyhedron::Plane_3;
  using Facet_handle = typename Polyhedron::Facet_handle;

public:
  using key_type = Facet_handle;
  using value_type = Plane_3;
  using reference = Plane_3;
  using category = boost::read_write_property_map_tag;

  Internal_face_plane_3_map() {}

  friend value_type get(const Internal_face_plane_3_map&, Facet_handle f)
  { return f->plane(); }

  friend void put(const Internal_face_plane_3_map&, key_type f,
                  const value_type& val)
  { f->plane() = val; }
};

} // namespace pol3

#endif
