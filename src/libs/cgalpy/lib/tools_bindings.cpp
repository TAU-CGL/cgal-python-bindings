#include <nanobind/nanobind.h>

#include <CGAL/Memory_sizer.h>

namespace py = nanobind;

void export_tools(py::module_& m) {
  py::class_<CGAL::Memory_sizer>(m, "Memory_sizer")
    .def(py::init<>())
    .def("virtual_size", &CGAL::Memory_sizer::virtual_size,
         "Returns the virtual memory size in bytes.")
    .def("resident_size", &CGAL::Memory_sizer::resident_size,
         "Returns the resident memory size in bytes.")
    ;
}

