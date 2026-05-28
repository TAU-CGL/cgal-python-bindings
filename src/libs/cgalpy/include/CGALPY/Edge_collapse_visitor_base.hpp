#ifndef CGALPY_EDGE_COLLAPSE_VISITOR_BASE_HPP
#define CGALPY_EDGE_COLLAPSE_VISITOR_BASE_HPP

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h>

namespace py = nanobind;
namespace SMS = CGAL::Surface_mesh_simplification;

namespace cgalpy {
namespace sms {

template <typename TriangleMesh>
struct My_ec_visitor : SMS::Edge_collapse_visitor_base<TriangleMesh> {
  using Tm = TriangleMesh;
  using Profile = SMS::Edge_profile<Tm>;
  using FT = typename Profile::FT;
  using size_type = typename Profile::edges_size_type;
  using vertex_descriptor = typename Profile::vertex_descriptor;
  using Point = typename Profile::Point;
  
  void OnStarted(Tm& tm) { if (started) started(tm); }
  void OnFinished(Tm& tm) { if (finished) finished(tm); }
  void OnStopConditionReached(const Profile& p) { if (stop_condition_reached) stop_condition_reached(p); }
  void OnCollected(const Profile& p, const std::optional<FT>& f) { if (collected) collected(p, f); }
  void OnSelected(const Profile& p, const std::optional<FT>& f, size_type n, size_type m) { if (selected) selected(p, f, n, m); }
  void OnCollapsing(const Profile& p, const std::optional<Point>& pt) { if (collapsing) collapsing(p, pt); }
  void OnCollapsed(const Profile& p, const vertex_descriptor& v) { if (collapsed) collapsed(p, v); }
  void OnNonCollapsable(const Profile& p) { if (non_collapsable) non_collapsable(p); }

  void set_started(const std::function<void(TriangleMesh&)>& f) { started = f; }
  void set_finished(const std::function<void(TriangleMesh&)>& f) { finished = f; }
  void set_stop_condition_reached(const std::function<void(const Profile&)>& f) { stop_condition_reached = f; }
  void set_collected(const std::function<void(const Profile&, std::optional<FT>)>& f) { collected = f; }
  void set_selected(const std::function<void(const Profile&, std::optional<FT>, size_type, size_type)>& f) { selected = f; }
  void set_collapsing(const std::function<void(const Profile&, std::optional<Point>)>& f) { collapsing = f; }
  void set_collapsed(const std::function<void(const Profile&, typename Profile::vertex_descriptor)>& f) { collapsed = f; }
  void set_non_collapsable(const std::function<void(const Profile&)>& f) { non_collapsable = f; }


 
  My_ec_visitor() :
    started(nullptr),
    finished(nullptr),
    stop_condition_reached(nullptr),
    collected(nullptr),
    selected(nullptr),
    collapsing(nullptr),
    collapsed(nullptr),
    non_collapsable(nullptr)
  {}


private:
  std::function<void(TriangleMesh&)> started;
  std::function<void(TriangleMesh&)> finished;
  std::function<void(const Profile&)> stop_condition_reached;
  std::function<void(const Profile&, std::optional<FT>)> collected;
  std::function<void(const Profile&, std::optional<FT>, size_type, size_type)> selected;
  std::function<void(const Profile&, std::optional<Point>)> collapsing;
  std::function<void(const Profile&, typename Profile::vertex_descriptor)> collapsed;
  std::function<void(const Profile&)> non_collapsable;
};

}
} // namespace cgalpy // namespace sms


#endif // CGALPY_EDGE_COLLAPSE_VISITOR_BASE_HPP
