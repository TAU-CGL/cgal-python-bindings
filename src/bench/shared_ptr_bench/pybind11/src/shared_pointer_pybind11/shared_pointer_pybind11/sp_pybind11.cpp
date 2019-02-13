#include <pybind11/pybind11.h>
#include <random>
namespace py = pybind11;
using namespace std;

default_random_engine generator;
uniform_int_distribution<int> distribution(1, 10);

class MyClass
{
private:
	int val = distribution(generator);
public:
	int get() const { return val; }
	void set(int i) { val = i; }
};


shared_ptr<MyClass> getSharedPointerToClass() {
	shared_ptr<MyClass> sp = make_shared<MyClass>();
	return sp;
}

shared_ptr<MyClass> getAnotherReference(shared_ptr<MyClass> p) {
	return p;
}

int getReferenceCount(shared_ptr<MyClass> p)
{
	return p.use_count();
}


//PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
PYBIND11_MODULE(sp_pybind11, m) {
	py::class_<MyClass, std::shared_ptr<MyClass>>(m, "MyClass")
		.def(py::init<>()).def("get", &MyClass::get)
		.def("set", &MyClass::set);
	m.def("getSharedPointerToClass", &getSharedPointerToClass, "get a shared pointer to a new MyClass object");
	m.def("getAnotherReference", &getAnotherReference, "get another shared pointer to the same object");
	m.def("getReferenceCount", &getReferenceCount, "get the referenced count of the shared_pointer");

}

