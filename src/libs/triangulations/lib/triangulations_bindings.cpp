#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_2.h>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Filtered_kernel.h> 
#include <CGAL/Cartesian.h>
#include <boost/python.hpp>
#include <string>

namespace bp = boost::python;

//typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
//typedef CGAL::Cartesian<int>                             Kernel_base;
//typedef CGAL::Filtered_kernel<Kernel_base>                  Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef Kernel::FT                                              FT;
typedef Kernel::Point_2                                         Point_2;
typedef Kernel::Segment_2                                       Segment_2;
typedef CGAL::Triangulation_2<Kernel>                           Triangulation_2;
typedef CGAL::Delaunay_triangulation_2<Kernel>                  Delaunay_triangulation_2;
typedef Triangulation_2::Vertex                                 Vertex;
typedef Triangulation_2::Vertex_iterator                        Vertex_iterator;
typedef Triangulation_2::All_vertices_iterator                  All_vertices_iterator;
typedef Triangulation_2::Finite_vertices_iterator               Finite_vertices_iterator;
typedef Triangulation_2::Edge                                   Edge;
typedef Triangulation_2::Edge_iterator                          Edges_iterator;
typedef Triangulation_2::All_edges_iterator                     All_edges_iterator;
typedef Triangulation_2::Finite_edges_iterator                  Finite_edges_iterator;
typedef Triangulation_2::Face                                   Face;
typedef Triangulation_2::Face_iterator                          Face_iterator;
typedef Triangulation_2::Face_handle                            Face_handle;
typedef Triangulation_2::All_faces_iterator                     All_faces_iterator;
typedef Triangulation_2::Finite_faces_iterator                  Finite_faces_iterator;
typedef Triangulation_2::Point_iterator                         Point_iterator;


typedef typename bp::return_value_policy<bp::copy_const_reference>       Kernel_return_value_policy;


inline bp::object pass_through(bp::object const& o) { return o; }

template <typename circulator>
class Iterator_from_circulator
{
private:
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
    bp::throw_error_already_set();
    return *m_curr;
  }
};

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

template <typename T>
CopyIterator<All_edges_iterator>* all_edges_iterator(T& t)
{
  return new CopyIterator<All_edges_iterator>(t.all_edges_begin(), t.all_edges_end());
}

template <typename T>
CopyIterator<Finite_edges_iterator>* finite_edges_iterator(T& t)
{
  return new CopyIterator<Finite_edges_iterator>(t.finite_edges_begin(), t.finite_edges_end());
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
template <typename T>
void insert_list(T& t, bp::list& lst)
{
  auto v = std::vector< Point_2 >(bp::stl_input_iterator< Point_2 >(lst),
    bp::stl_input_iterator< Point_2 >());
  t.insert(v.begin(), v.end());
}

template <typename T>
void flip(T& t, Face_handle& f, int i)
{
  auto fh = T::Face_handle(f);
  t.flip(f, i);
}

template <typename T>
typename T::Triangle triangle(T& t, All_faces_iterator& f)
{
  std::cout << "0";
  auto fh = T::Face_handle(f);
  std::cout << "1";
  auto res = t.triangle(fh);
  std::cout << "2";
  return res;
}

template<typename T>
void insert_point(T& t, Point_2& p)
{
  t.insert(p);
}

template <typename T, typename C>
void bind_triangulation(C c)
{
  using namespace boost::python;
  //class_<Triangulation_2>("Triangulation");

  c
    .def(init<>())
    .def("clear", &T::clear)
    .def("insert", &insert_list<T>)
    .def("insert", &insert_point<T>)
    //.def("flip", &flip<T>)
    //.def("triangle", &triangle<T>)
    .def("all_vertices", range<return_internal_reference<>>(&T::all_vertices_begin, &T::all_vertices_end))
    .def("finite_vertices", range<return_internal_reference<>>(&T::finite_vertices_begin, &T::finite_vertices_end))
    .def("all_edges", &all_edges_iterator<T>, return_value_policy<manage_new_object>())
    .def("finite_edges", &finite_edges_iterator<T>, return_value_policy<manage_new_object>())
    .def("all_faces", range<return_internal_reference<>>(&T::all_faces_begin, &T::all_faces_end))
    .def("finite_faces", range<return_internal_reference<>>(&T::finite_faces_begin, &T::finite_faces_end))
    .def("points", range<return_internal_reference<>>(&T::points_begin, &T::points_end))
    .def<Segment_2(T::*)(const Edge&) const>("segment", &T::segment)
    .def<bool (T::*)(const Edge&) const>("is_infinite", &T::is_infinite)

    ;
}

BOOST_PYTHON_MODULE(triangulations)
{
  using namespace boost::python;
  auto c0 = class_<Triangulation_2>("Triangulation_2");
  auto c1 = class_<Delaunay_triangulation_2, bases<Triangulation_2>>("Delaunay_triangulation_2");

  bind_triangulation<Triangulation_2>(c0);
  bind_triangulation<Delaunay_triangulation_2>(c1);

  class_<Vertex>("Triangulation_vertex")
    .def<Point_2& (Vertex::*) ()>("point", &Vertex::point, return_internal_reference<>())
    ;

  class_<Edge>("Triangulation_edge")
    ;
  class_<Face>("Triangulation_face")
    ;

  bind_copy_iterator<CopyIterator<All_edges_iterator>>("Triangulation_all_edges_iterator");
  bind_copy_iterator<CopyIterator<Finite_edges_iterator>>("Triangulation_finite_edges_iterator");
}