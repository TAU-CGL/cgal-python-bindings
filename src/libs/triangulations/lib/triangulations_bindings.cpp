#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_2.h>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Filtered_kernel.h> 
#include <CGAL/Cartesian.h>
#include <boost/python.hpp>

namespace bp = boost::python;

//typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
//typedef CGAL::Cartesian<int>                             Kernel_base;
//typedef CGAL::Filtered_kernel<Kernel_base>                  Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef Kernel::FT                                              FT;
typedef Kernel::Point_2                                         Point_2;
typedef Kernel::Segment_2                                       Segment_2;
typedef CGAL::Delaunay_triangulation_2<Kernel>                  Triangulation;
typedef Triangulation::Vertex                                   Vertex;
typedef Triangulation::Vertex_iterator                          Vertex_iterator;
typedef Triangulation::Edge                                     Edge;
typedef Triangulation::All_edges_iterator                       Edge_iterator;
//typedef Triangulation::Constrained_edges_iterator               CE_iterator;

typedef typename bp::return_value_policy<bp::copy_const_reference>       Kernel_return_value_policy;


inline bp::object pass_through(bp::object const& o) { return o; }

template <typename iterator>
class CopyIterator
{
private:
  iterator m_curr;
  iterator m_end;
public:
  CopyIterator(iterator begin, iterator end) : m_curr(begin), m_end(end) {}
  typename iterator::value_type next()
  {
    if (m_curr != m_end)
    {
      return *(m_curr++);
    }
    PyErr_SetString(PyExc_StopIteration, "No more data.");
    bp::throw_error_already_set();
    return *m_curr;
  }
};

CopyIterator<Edge_iterator>* edges_iterator(Triangulation& t)
{
  return new CopyIterator<Edge_iterator>(t.all_edges_begin(), t.all_edges_end());
}

template<typename iterator>
void bind_copy_iterator(const char* python_name)
{
  using namespace boost::python;
  class_<iterator>(python_name, no_init)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next)
    ;
}

void insert_list(Triangulation& t, bp::list& lst)
{
  auto v = std::vector< Point_2 >(bp::stl_input_iterator< Point_2 >(lst),
    bp::stl_input_iterator< Point_2 >());
  t.insert(v.begin(), v.end());
}

BOOST_PYTHON_MODULE(triangulations)
{
  using namespace boost::python;

  class_<Edge>("Edge")
    ;

  class_<Vertex>("Vertex")
    .def<Point_2& (Vertex::*) ()>("point", &Vertex::point, return_internal_reference<>())
    ;

  class_<Triangulation>("Triangulation")
    .def(init<>())
    .def("insert", &insert_list)
    .def("edges", &edges_iterator, return_value_policy<manage_new_object>())
    .def("vertices", range<return_internal_reference<>>(&Triangulation::vertices_begin, &Triangulation::vertices_end))
    .def<Segment_2(Triangulation::*)(const Edge&) const>("segment", &Triangulation::segment)
    .def<bool (Triangulation::*)(const Edge&) const>("is_infinite", &Triangulation::is_infinite)
    ;


  bind_copy_iterator<CopyIterator<Edge_iterator>>("Triangulation_edges_iterator");
}