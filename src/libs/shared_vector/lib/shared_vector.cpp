#define BOOST_PYTHON_STATIC_LIB

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>

namespace bp = boost::python;

class Element {
private:
  std::string m_str;

public:
  const std::string& str() const { return m_str; }
  void set_str(const std::string& str) { m_str = str; }
};

typedef boost::shared_ptr<Element>              Shared_element;
typedef std::vector<Shared_element>             Element_vector;
typedef boost::shared_ptr<Element_vector>       Shared_element_vector;

// class Data {
// private:
//   Shared_element_vector m_data;

//   Data() { m_data = Shared_element_vector(new Element_vector); }

//   Shared_element_vector data() const { return m_data; }

//   void add_data(Shared_element element) { m_data->push_back(element); }
// };

Shared_element init_element()
{
  auto e = Shared_element(new Element);
  e->set_str("new element");
  return e;
}

Shared_element_vector initd()
{
  auto data = Shared_element_vector(new Element_vector);

  auto e1 = Shared_element(new Element);
  e1->set_str("hello");
  auto e2 = Shared_element(new Element);
  e2->set_str("world");

  data->push_back(e1);
  data->push_back(e2);

  return data;
}

Shared_element ev_get(Element_vector& ev, int i)
{
  return ev[i];
}

void ev_set(Element_vector& ev, int i, Shared_element& e)
{
  ev[i] = e;
}

Element_vector::iterator ev_begin(Element_vector& ev)
{
  return ev.begin();
}

Element_vector::iterator ev_end(Element_vector& ev)
{
  return ev.end();
}


BOOST_PYTHON_MODULE(shared_vector)
{
  using namespace boost::python;

  //bp::register_ptr_to_python<Shared_element_vector>();
  //bp::register_ptr_to_python<Shared_element>();

  bp::class_<Element, Shared_element, boost::noncopyable>("Element", bp::no_init)
    .def("str", &Element::str,
         bp::return_value_policy<bp::copy_const_reference>())
    .def("set_str", &Element::set_str);
    ;

  bp::class_<Element_vector, Shared_element_vector>("Element_vector", bp::no_init)
    // "ev[i]"
    .def<Shared_element& (Element_vector::*)(const Element_vector::size_type)>
    ("__getitem__", &Element_vector::operator[], return_value_policy<copy_non_const_reference>())
    // less clustered alternative
    //.def("__getitem__", &ev_get)

    // "ev[i] = ..."
    .def("__setitem__", &ev_set)

    // "for element in ev.element():"
    .def("elements", bp::range<return_value_policy<return_by_value>>(&ev_begin, &ev_end))
  ;

  

  bp::def("initd", initd);
  bp::def("init_element", init_element);
}
