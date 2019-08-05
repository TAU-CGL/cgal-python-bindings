#include <Common.h>

namespace Arrangement_2_bindings
{
  void insert_segment(Arrangement_2& arr, Segment_2& s)
  {
    CGAL::insert(arr, s);
  }
  void insert_point(Arrangement_2& arr, Point_2& p)
  {
    CGAL::insert_point(arr, p);
  }
  void insert_segments(Arrangement_2& arr, boost::python::list& lst)
  {
    auto v = std::vector< Segment_2 >(boost::python::stl_input_iterator< Segment_2 >(lst),
      boost::python::stl_input_iterator< Segment_2 >());
    CGAL::insert(arr, v.begin(), v.end());
  }
  Vertex_iterator vertices_begin(Arrangement_2& arr) { return arr.vertices_begin(); }
  Vertex_iterator vertices_end(Arrangement_2& arr) { return arr.vertices_end(); }
  Halfedge_iterator halfedges_begin(Arrangement_2& arr) { return arr.halfedges_begin(); }
  Halfedge_iterator halfedges_end(Arrangement_2& arr) { return arr.halfedges_end(); }
  Face_iterator faces_begin(Arrangement_2& arr) { return arr.faces_begin(); }
  Face_iterator faces_end(Arrangement_2& arr) { return arr.faces_end(); }
  Face& unbounded_face(Arrangement_2& arr) { return *(arr.unbounded_face()); }
  Face& remove_edge(Arrangement_2& arr, Halfedge& e)
  {
    return *(arr.remove_edge(Halfedge_iterator(&e)));
  }
}

void export_Arrangement_2()
{
  using namespace boost::python;
  class_<Arrangement_2>("Arrangement_2")
    .def(init<>())
    .def("halfedges", range<return_value_policy<reference_existing_object>>(&Arrangement_2_bindings::halfedges_begin, &Arrangement_2_bindings::halfedges_end))
    .def("vertices", range<return_value_policy<reference_existing_object>>(&Arrangement_2_bindings::vertices_begin, &Arrangement_2_bindings::vertices_end))
    .def("faces", range<return_value_policy<reference_existing_object>>(&Arrangement_2_bindings::faces_begin, &Arrangement_2_bindings::faces_end))
    .def("unbounded_face", &Arrangement_2_bindings::unbounded_face, return_value_policy<reference_existing_object>())
    //.def("insert_from_left_vertex", &Arrangement_2_bindings::insert_from_left_vertex)
    //.def("insert_from_right_vertex", &Arrangement_2_bindings::insert_from_right_vertex)
    .def("remove_edge", &Arrangement_2_bindings::remove_edge, return_value_policy<reference_existing_object>())
    .def("is_empty", &Arrangement_2::is_empty)
    .def("is_valid", &Arrangement_2::is_valid)
    .def("number_of_edges", &Arrangement_2::number_of_edges)
    .def("number_of_faces", &Arrangement_2::number_of_faces)
    .def("number_of_halfedges", &Arrangement_2::number_of_halfedges)
    .def("number_of_isolated_vertices", &Arrangement_2::number_of_isolated_vertices)
    .def("number_of_unbounded_faces", &Arrangement_2::number_of_unbounded_faces)
    .def("number_of_vertices", &Arrangement_2::number_of_vertices)
    .def("number_of_vertices_at_infinity", &Arrangement_2::number_of_vertices_at_infinity)
    .def(self_ns::str(self_ns::self))
    ;

  def("insert_point", &Arrangement_2_bindings::insert_point);
  def("insert", &Arrangement_2_bindings::insert_segment);
  def("insert", &Arrangement_2_bindings::insert_segments);
}