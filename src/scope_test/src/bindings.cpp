#define BOOST_PYTHON_STATIC_LIB 1
#include <boost/python.hpp>

int foo() { return 0; }

#define SET_SCOPE(x)  \
std::string module_name = std::string("scopeTest")+std::string(".")+std::string(x); \
bp::object module(bp::handle<>(bp::borrowed(PyImport_AddModule(module_name.c_str())))); \
bp::scope().attr(x) = module; \
bp::scope module_scope = module;

BOOST_PYTHON_MODULE(scopeTest)
{
  namespace bp = boost::python;
  // http://isolation-nation.blogspot.com/2008/09/packages-in-python-extension-modules.html
  bp::object package = bp::scope();
  package.attr("__path__") = "scopeTest";
  {
    SET_SCOPE("Ker")
      bp::def("foo", &foo);
  }
}