// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef CGALPY_POLYLINE_VISITOR_HPP
#define CGALPY_POLYLINE_VISITOR_HPP

#include <functional>
#include <boost/graph/graph_traits.hpp>

namespace boost_utils {

template <typename Graph>
class Polyline_visitor {
  using Gt = boost::graph_traits<Graph>;
  using Vd = typename Gt::vertex_descriptor;

public:
  Polyline_visitor() :
    an(),
    snp(),
    ep()
  {}

  void set_start_new_polyline(const std::function<void()>& fn) { snp = fn; }
  void set_add_node(const std::function<void(Vd)>& fn) { an = fn; }
  void set_end_polyline(const std::function<void()>& fn) { ep = fn; }

  void start_new_polyline() { snp(); }
  void add_node(Vd v) { an(v); }
  void end_polyline() { ep(); }

  /* void start_new_polyline() called when starting the description of a
   * polyline.
   *   `void add_node(typename boost::graph_traits<Graph>::vertex_descriptor v)`
   * called for each vertex v of the polyline currently described.
   * If the polyline is closed this function will be called twice for the first
   * vertex of the cycle picked (once after calling `start_new_polyline()` and
   * once before the call to `end_polyline()`.
   * `void end_polyline()` called when the description of a polyline is
   * finished.
   */

private:
  std::function<void()> snp;
  std::function<void(Vd)> an;
  std::function<void()> ep;
};


} // namespace boost_utils

#endif
