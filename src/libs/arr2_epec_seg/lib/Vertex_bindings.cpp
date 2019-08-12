#include <Common.h>


Halfedge_around_vertex_circulator Halfedge_around_vertex_circulator_begin(Vertex& v)
{
  return v.incident_halfedges();
}

Halfedge_around_vertex_circulator Halfedge_around_vertex_circulator_end(Vertex& v)
{
  return v.incident_halfedges();
}

Iterator_from_circulator<Halfedge_around_vertex_circulator>* halfedge_around_vertex_iterator(Vertex& v)
{
  return new Iterator_from_circulator<Halfedge_around_vertex_circulator>(v.incident_halfedges());
}

void export_Vertex()
{
  using namespace boost::python;
  class_<Vertex>("Vertex")
    .def(init<>())
    .def<Point_2& (Vertex::*)()>("point", &Vertex::point, return_internal_reference<>())
    .def("is_isolated", &Vertex::is_isolated)
    .def("degree", &Vertex::degree)
    .def("incident_halfedges", &halfedge_around_vertex_iterator, return_value_policy<manage_new_object>())
#ifdef EXTENDED_DCEL
    .def<Vertex::Data& (Vertex::*)()>("data", &Vertex::data, return_value_policy<copy_non_const_reference>()) //elementary type
    .def("set_data", &Vertex::set_data)
#endif // EXTENDED_DCEL
    ;
  bind_iterator<Iterator_from_circulator<Halfedge_around_vertex_circulator>>("Halfedge_around_vertex_iterator");
}