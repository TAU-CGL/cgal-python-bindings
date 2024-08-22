#ifndef CGALPY_POLYLINE_VISITOR_HPP
#define CGALPY_POLYLINE_VISITOR_HPP

#include <functional>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace boost_utils {

//
template <typename VertexDescriptor>
struct PolylineVisitor {
  PolylineVisitor() : snp(), an(), ep() {}

  void start_new_polyline() const { snp(); }
  void add_node(const VertexDescriptor& v) const { an(v); }
  void end_polyline() const { ep(); }

  void set_start_new_polyline(const std::function<void()>& fn) { snp = fn; }
  void set_add_node(const std::function<void(VertexDescriptor)>& fn) {
    an = fn;
  }
  void set_end_polyline(const std::function<void()>& fn) { ep = fn; }

 private:
  std::function<void()> snp;
  std::function<void(VertexDescriptor)> an;
  std::function<void()> ep;
};

template <typename C, typename VertexDescriptor>
void export_polyline_visitor(C& c) {
  py::class_<PolylineVisitor<VertexDescriptor>>(c, "Polyline_visitor", "Visitor used in split_graph_into_polylines.")
    .def(py::init<>())
    .def("set_start_new_polyline", &PolylineVisitor<VertexDescriptor>::set_start_new_polyline,
         py::arg("fn"),
         "Sets the function to be called when starting the description of a polyline.")
    .def("set_add_node", &PolylineVisitor<VertexDescriptor>::set_add_node,
         py::arg("fn"),
         "Sets the function to be called for each vertex v of the polyline currently described. If the polyline is closed this function will be called twice for the first vertex of the cycle picked (once after calling start_new_polyline() and once before the call to end_polyline().")
    .def("set_end_polyline", &PolylineVisitor<VertexDescriptor>::set_end_polyline,
         py::arg("fn"),
         "Sets the function to be called when the description of a polyline is finished.")
    ;
}

}  // namespace boost_utils

#endif  // CGALPY_POLYLINE_VISITOR_HPP
