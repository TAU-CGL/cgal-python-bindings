#define BOOST_PYTHON_STATIC_LIB
#include <memory>
#include <random>
#include <boost/shared_ptr.hpp>
#include <boost/python.hpp>
using namespace std;

default_random_engine generator;
uniform_int_distribution <int> distribution(1, 10);

class MyClass
{
private:
	int val = distribution(generator);
public:
	int get() const { return val; }
	void set(int i) { val = i; }
};


boost::shared_ptr<MyClass> getSharedPointerToClass() {
	boost::shared_ptr<MyClass> sp = boost::shared_ptr<MyClass>(new MyClass);
	return sp;
}

boost::shared_ptr<MyClass> getAnotherReference(boost::shared_ptr<MyClass> p) {
	return p;
}

int getReferenceCount(boost::shared_ptr<MyClass> p)
{
	return p.use_count();
}

BOOST_PYTHON_MODULE(boost_sp)
{
	using namespace boost::python;
	def("getAnotherReference", &getAnotherReference);
	def("getReferenceCount", &getReferenceCount);
	def("getSharedPointerToClass", &getSharedPointerToClass);
	class_<MyClass, boost::shared_ptr<MyClass>>("MyClass")
		.def("get", &MyClass::get)
		.def("set", &MyClass::set);
}