#include <Common.h>

//Free functions
Vertex& insert_point(Arrangement_2& arr, Point_2& p)
{
  return *(CGAL::insert_point(arr, p));
}
void insert_segment(Arrangement_2& arr, Segment_2& s)
{
  CGAL::insert(arr, s);
}
void insert_segments(Arrangement_2& arr, boost::python::list& lst)
{
  auto v = std::vector< Segment_2 >(boost::python::stl_input_iterator< Segment_2 >(lst),
    boost::python::stl_input_iterator< Segment_2 >());
  CGAL::insert(arr, v.begin(), v.end());
}
Halfedge& insert_non_intersecting_segment(Arrangement_2& arr, Segment_2& s)
{
  return *(CGAL::insert_non_intersecting_curve(arr, s));
}
void insert_non_intersecting_segments(Arrangement_2& arr, boost::python::list& lst)
{
  auto v = std::vector< Segment_2 >(boost::python::stl_input_iterator< Segment_2 >(lst),
    boost::python::stl_input_iterator< Segment_2 >());
  CGAL::insert_non_intersecting_curves(arr, v.begin(), v.end());
}
bool do_intersect(Arrangement_2& arr, Segment_2& s)
{
  return CGAL::do_intersect(arr, s);
}
void overlay(Arrangement_2& arr1, Arrangement_2& arr2, Arrangement_2& arr_res)
{
  CGAL::overlay(arr1, arr2, arr_res);
}
Face& remove_edge_free(Arrangement_2& arr, Halfedge& e)
{
  return *(CGAL::remove_edge(arr, Halfedge_iterator(&e)));
}
bool remove_vertex_free(Arrangement_2& arr, Vertex& v)
{
  return CGAL::remove_vertex(arr, Vertex_iterator(&v));
}

//decompose? zone?

//Arrangement methods
Halfedge& insert_from_left_vertex(Arrangement_2& arr, Segment_2& s, Vertex& v)
{
  return *(arr.insert_from_left_vertex(s,(Vertex_iterator(&v))));
}
Halfedge& insert_from_right_vertex(Arrangement_2& arr, Segment_2& s, Vertex& v)
{
  return *(arr.insert_from_right_vertex(s, (Vertex_iterator(&v))));
}
Halfedge& insert_edge_in_face_interior(Arrangement_2& arr, Segment_2& s, Face& f)
{
  return *(arr.insert_in_face_interior(s, (Face_iterator(&f))));
}
Vertex& insert_vertex_in_face_interior(Arrangement_2& arr, Point_2& p, Face& f)
{
  return *(arr.insert_in_face_interior(p, (Face_iterator(&f))));
}
Halfedge& insert_at_vertices(Arrangement_2& arr, Segment_2& s, Vertex& v1, Vertex& v2)
{
  return *(arr.insert_at_vertices(s, (Vertex_iterator(&v1)), (Vertex_iterator(&v2))));
}
Vertex& modify_vertex(Arrangement_2& arr, Vertex& v, Point_2& p)
{
  return *(arr.modify_vertex(Vertex_iterator(&v), p));
}
Face& remove_isolated_vertex(Arrangement_2& arr, Vertex& v)
{
  return *(arr.remove_isolated_vertex(Vertex_iterator(&v)));
}
Halfedge& modify_edge(Arrangement_2& arr, Halfedge& e, Segment_2& s)
{
  return *(arr.modify_edge(Halfedge_iterator(&e), s));
}
Halfedge& split_edge(Arrangement_2& arr, Halfedge& e, Segment_2& s1, Segment_2& s2)
{
  return *(arr.split_edge(Halfedge_iterator(&e), s1, s2));
}
Halfedge& merge_edge(Arrangement_2& arr, Halfedge& e1, Halfedge& e2, Segment_2& s)
{
  return *(arr.merge_edge(Halfedge_iterator(&e1), Halfedge_iterator(&e2), s));
}
Face& remove_edge(Arrangement_2& arr, Halfedge& e)
{
  return *(arr.remove_edge(Halfedge_iterator(&e)));
}
Vertex_iterator vertices_begin(Arrangement_2& arr) { return arr.vertices_begin(); }
Vertex_iterator vertices_end(Arrangement_2& arr) { return arr.vertices_end(); }
Halfedge_iterator halfedges_begin(Arrangement_2& arr) { return arr.halfedges_begin(); }
Halfedge_iterator halfedges_end(Arrangement_2& arr) { return arr.halfedges_end(); }
Halfedge_iterator edges_begin(Arrangement_2& arr) { return arr.edges_begin(); }
Halfedge_iterator edges_end(Arrangement_2& arr) { return arr.edges_end(); }
Face_iterator faces_begin(Arrangement_2& arr) { return arr.faces_begin(); }
Face_iterator faces_end(Arrangement_2& arr) { return arr.faces_end(); }
Face& unbounded_face(Arrangement_2& arr) { return *(arr.unbounded_face()); }


void export_Arrangement_2()
{
  using namespace boost::python;
  class_<Arrangement_2>("Arrangement_2")
    .def(init<>())
    .def("halfedges", range<return_value_policy<reference_existing_object>>(&halfedges_begin, &halfedges_end))
    .def("vertices", range<return_value_policy<reference_existing_object>>(&vertices_begin, &vertices_end))
    .def("faces", range<return_value_policy<reference_existing_object>>(&faces_begin, &faces_end))
    .def("edges", range<return_value_policy<reference_existing_object>>(&edges_begin, &edges_end))
    .def("unbounded_face", &unbounded_face, return_value_policy<reference_existing_object>())
    .def("insert_from_left_vertex", &insert_from_left_vertex, return_value_policy<reference_existing_object>())
    .def("insert_from_right_vertex", &insert_from_right_vertex, return_value_policy<reference_existing_object>())
    .def("insert_in_face_interior", &insert_edge_in_face_interior, return_value_policy<reference_existing_object>())
    .def("insert_in_face_interior", &insert_vertex_in_face_interior, return_value_policy<reference_existing_object>())
    .def("insert_at_vertices", &insert_at_vertices, return_value_policy<reference_existing_object>())
    .def("modify_vertex", &modify_vertex, return_value_policy<reference_existing_object>())
    .def("remove_isolated_vertex", &remove_isolated_vertex, return_value_policy<reference_existing_object>())
    .def("modify_edge", &modify_edge, return_value_policy<reference_existing_object>())
    .def("split_edge", &split_edge, return_value_policy<reference_existing_object>())
    .def("merge_edge", &merge_edge, return_value_policy<reference_existing_object>())
    .def("remove_edge", &remove_edge, return_value_policy<reference_existing_object>())
    .def("is_empty", &Arrangement_2::is_empty)
    .def("is_valid", &Arrangement_2::is_valid)
    .def("number_of_edges", &Arrangement_2::number_of_edges)
    .def("number_of_faces", &Arrangement_2::number_of_faces)
    .def("number_of_halfedges", &Arrangement_2::number_of_halfedges)
    .def("number_of_isolated_vertices", &Arrangement_2::number_of_isolated_vertices)
    .def("number_of_unbounded_faces", &Arrangement_2::number_of_unbounded_faces)
    .def("number_of_vertices", &Arrangement_2::number_of_vertices)
    .def("number_of_vertices_at_infinity", &Arrangement_2::number_of_vertices_at_infinity)
    .def("assign", &Arrangement_2::assign)
    .def("clear", &Arrangement_2::clear)
    .def(self_ns::str(self_ns::self))
    ;

  def("insert_point", &insert_point, return_value_policy<reference_existing_object>());
  def("insert", &insert_segment);
  def("insert", &insert_segments);
  def("insert_non_intersecting_curve", &insert_non_intersecting_segment, return_value_policy<reference_existing_object>());
  def("insert_non_intersecting_curves", &insert_non_intersecting_segments);
  def("overlay", &overlay);
  def("do_intersect", &do_intersect);
  def("remove_edge", &remove_edge_free, return_value_policy<reference_existing_object>());
  def("remove_vertex", &remove_vertex_free);
}