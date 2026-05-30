#ifndef CGALPY_MESH_CONSTANT_DOMAIN_FIELD_3_HPP
#define CGALPY_MESH_CONSTANT_DOMAIN_FIELD_3_HPP

#include <nanobind/nanobind.h>

#include "cgalpy/kernel_types.hpp"

namespace py = nanobind;

template <typename Kernel, typename Df, typename C>
void export_mesh_constant_domain_field_3(C& c) {
  c.def(py::init<FT>())
    .def("set_size", &Df::set_size)
    ;
}


#endif // CGALPY_MESH_CONSTANT_DOMAIN_FIELD_3_HPP

