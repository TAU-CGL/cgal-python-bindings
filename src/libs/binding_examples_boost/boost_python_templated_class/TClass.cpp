#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

template<class T>
class A
{
	const T a;
public:
	A(T a) : a(a) {}
	const T getA() const { return a; }
};


BOOST_PYTHON_MODULE(TClass)
{
	using namespace boost::python;
	class_<A<int>>("A", init<int>())
		.def("get", &A<int>::getA);
}