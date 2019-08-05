#include <Common.h>

inline boost::python::object pass_through(boost::python::object const& o) { return o; }

template <class circulator>
class Iterator_from_circulator
{
  bool first = true;
  circulator m_first;
  circulator m_curr;
public:
  Iterator_from_circulator(circulator first) : m_first(first), m_curr(first) {}
  typename circulator::value_type& next()
  {
    if (m_curr != 0)
    {
      if (first || m_curr != m_first)
      {
        first = false;
        return *(m_curr++);
      }
    }
    PyErr_SetString(PyExc_StopIteration, "No more data.");
    boost::python::throw_error_already_set();
    return *m_curr;
  }
};

template <class iterator>
class Iterator_of_circulators
{
  typedef typename Iterator_from_circulator<typename iterator::value_type> modified_circulator;
  iterator m_curr;
  iterator m_end;
public:
  Iterator_of_circulators(iterator begin, iterator end) : m_curr(begin), m_end(end) {}
  modified_circulator* next()
  {
    if (m_curr != m_end)
      return new modified_circulator(modified_circulator(*(m_curr++)));
    PyErr_SetString(PyExc_StopIteration, "No more data.");
    boost::python::throw_error_already_set();
    return new modified_circulator(modified_circulator(*m_curr));
  }
};

template<typename iterator>
void bind_iterator_of_circulators(const char* python_name)
{
  using namespace boost::python;
  class_<iterator>(python_name, no_init)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next, return_value_policy<manage_new_object>())
    ;
}

template<typename iterator>
void bind_iterator(const char* python_name)
{
  using namespace boost::python;
  class_<iterator>(python_name, no_init)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next, return_value_policy<reference_existing_object>())
    ;
}

namespace Kernel_bindings
{
  Kernel::Equal_2 kernel_equal_2(Kernel& k)
  {
    return (Kernel::Equal_2)(k.equal_2_object());
  }
}

namespace Vertex_bindings
{
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
}

namespace Halfedge_bindings
{
  Vertex& source(Halfedge& h) { return (*(h.source())); }
  Vertex& target(Halfedge& h) { return (*(h.target())); }
  Halfedge& next(Halfedge& h) { return (*(h.next())); }
  Halfedge& twin(Halfedge& h) { return (*(h.twin())); }
  Face& face(Halfedge& h) { return (*(h.face())); }
}

namespace Face_bindings
{
  Iterator_from_circulator<Ccb_halfedge_circulator>* outer_ccb(Face& f)
  {
    return new Iterator_from_circulator<Ccb_halfedge_circulator>(f.outer_ccb());
  }
  Iterator_of_circulators<Inner_ccb_iterator>* inner_ccbs(Face& f)
  {
    return new Iterator_of_circulators<Inner_ccb_iterator>(f.inner_ccbs_begin(), f.inner_ccbs_end());
  }
}

void export_Arrangement_2();
void export_Intersections_2();

BOOST_PYTHON_MODULE(arr2_epec_seg)
{
  using namespace boost::python;

  class_<CGAL::Gmpq>("Gmpq")
    //.def(init<>())
    .def(init<int, int>())
    .def(init<const std::string&>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  class_<FT>("FT")
    .def(init<FT::Exact_type>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  enum_<CGAL::Sign>("Sign")
    ;

  class_<Kernel>("Kernel")
    .def(init<>())
    .def("equal_2_object", &Kernel_bindings::kernel_equal_2)
    ;

  class_<Traits>("Traits")
    .def(init<>())
    .def("equal_2_object", &Traits::equal_2_object)
    .def("compare_xy_2_object", &Traits::compare_xy_2_object)
    ;

  class_<Traits::Compare_xy_2>("Traits_compare_xy_2", no_init)
    .def<CGAL::Sign(Traits::Compare_xy_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Compare_xy_2::operator())
    ;

  class_<Traits::Equal_2>("Traits_equal_2_object", no_init)
    .def<bool (Traits::Equal_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Equal_2::operator())
    ;

  class_<Kernel::Equal_2>("Kernel_equal_2_object", no_init)
    //.def<bool (Kernel::Equal_2::*)(const Rational_point&, const Rational_point&) const>("__call__", &Kernel::Equal_2::operator())
    ;

  class_<Point_2>("Point_2")
    .def(init<>())
    .def(init<double, double>())
    .def(init<FT, FT>())
    .def("x", &Point_2::x)
    .def("y", &Point_2::y)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  class_<Segment_2>("Segment_2")
    .def(init<Point_2, Point_2>())
    .def("source", &Segment_2::source)
    .def("target", &Segment_2::target)
    .def("supporting_line", &Segment_2::supporting_line)
    .def("squared_length", &Segment_2::squared_length)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  class_<Curve>("Curve")
    .def("source", &Curve::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve::target, return_value_policy<copy_const_reference>())
    .def("line", &Curve::line, return_value_policy<reference_existing_object>())
    .def("is_vertical", &Curve::is_vertical)
    .def(self_ns::str(self_ns::self))
    //.def(self == self)
    ;

  class_<Line_2>("Line_2")
    .def("a", &Line_2::a)
    .def("b", &Line_2::b)
    .def("c", &Line_2::c)
    .def("is_degenerate", &Line_2::is_degenerate)
    .def("is_horizontal", &Line_2::is_horizontal)
    .def("is_vertical", &Line_2::is_horizontal)
    .def("has_on", &Line_2::has_on)
    .def("has_on_boundary", &Line_2::has_on_boundary)
    .def("has_on_negative_side", &Line_2::has_on_negative_side)
    .def("has_on_positive_side", &Line_2::has_on_positive_side)
    .def(self_ns::str(self_ns::self))
    .def(self == self)
    ;

  export_Arrangement_2();

  class_<Vertex>("Vertex")
    .def<Point_2& (Vertex::*)()>("point", &Vertex::point, return_internal_reference<>())
    .def("is_isolated", &Vertex::is_isolated)
    .def("degree", &Vertex::degree)
    .def("incident_halfedges", &Vertex_bindings::halfedge_around_vertex_iterator, return_value_policy<manage_new_object>())
    //.def<Vertex::Data& (Vertex::*)()>("data", &Vertex::data, return_value_policy<copy_non_const_reference>()) //elementary type
    //.def("set_data", &Vertex::set_data)
    ;
  bind_iterator<Iterator_from_circulator<Halfedge_around_vertex_circulator>>("Halfedge_around_vertex_iterator");

  class_<Halfedge>("Halfedge")
    .def("source", &Halfedge_bindings::source, return_value_policy<reference_existing_object>())
    .def("target", &Halfedge_bindings::target, return_value_policy<reference_existing_object>())
    .def("twin", &Halfedge_bindings::twin, return_value_policy<reference_existing_object>())
    .def("face", &Halfedge_bindings::face, return_value_policy<reference_existing_object>())
    .def("next", &Halfedge_bindings::next, return_value_policy<reference_existing_object>())
    .def<Halfedge::X_monotone_curve& (Halfedge::*)()>("curve", &Halfedge::curve, return_value_policy<reference_existing_object>())
    //.def("set_data", &Halfedge::set_data)
    //.def<Halfedge::Data& (Halfedge::*)()>("data", &Halfedge::data, return_value_policy<copy_non_const_reference>()) //elementary type
    ;

  class_<Face>("Face")
    .def("is_unbounded", &Face::is_unbounded)
    .def("outer_ccb", &Face_bindings::outer_ccb, return_value_policy<manage_new_object>())
    .def("inner_ccbs", &Face_bindings::inner_ccbs, return_value_policy<manage_new_object>())
    //.def("set_data", &Face::set_data)
    //.def<Face::Data& (Face::*)()>("data", &Face::data, return_value_policy<copy_non_const_reference>()) //elementary type
    ;
  bind_iterator<Iterator_from_circulator<Ccb_halfedge_circulator>>("Ccb_halfedge_iterator");
  bind_iterator_of_circulators<Iterator_of_circulators<Inner_ccb_iterator>>("Inner_ccbs_iterator");

  export_Intersections_2();

  def<Point_2(const Point_2&, const Point_2&)>("midpoint", &CGAL::midpoint);
  def<FT(const Point_2&, const Point_2&)>("squared_distance", &CGAL::squared_distance);
  def< FT(const Point_2&, const Segment_2&)>("squared_distance", &CGAL::squared_distance);

}