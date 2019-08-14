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

BOOST_PYTHON_MODULE(shared_vector)
{
  using namespace boost::python;

  bp::register_ptr_to_python<Shared_element_vector>();

  bp::class_<Element, Shared_element, boost::noncopyable>("Element", bp::no_init)
    .def("str", &Element::str,
         bp::return_value_policy<bp::copy_const_reference>())
    .def("set_str", &Element::set_str);
    ;

  bp::def("initd", initd);
}
