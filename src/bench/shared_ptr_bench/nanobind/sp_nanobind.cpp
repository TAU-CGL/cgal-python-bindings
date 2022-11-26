#include <random>
#include <memory>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>

namespace py = nanobind;

using namespace std;

default_random_engine generator;
uniform_int_distribution<int> distribution(1, 10);

class MyClass {
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

shared_ptr<MyClass> getAnotherReference(shared_ptr<MyClass> p)
{ return p; }

int getReferenceCount(shared_ptr<MyClass> p) { return p.use_count(); }

//PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
NB_MODULE(sp_nanobind, m)
{
  py::class_<MyClass>(m, "MyClass")
    .def(py::init<>())
    .def("get", &MyClass::get)
    .def("set", &MyClass::set);
  m.def("getSharedPointerToClass", &getSharedPointerToClass);
  m.def("getAnotherReference", &getAnotherReference);
  m.def("getReferenceCount", &getReferenceCount);
}
