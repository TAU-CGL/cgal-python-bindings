#include <nanobind/nanobind.h>

#include <CGAL/Modifier_base.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polyhedron_3_types.hpp"
#include "CGALPY/Polyhedron_builder.hpp"

namespace py = nanobind;

void export_polyhedron_builder(py::module_& m) {
  using Mb = CGAL::Modifier_base<cgalpy::pol3::Halfedge_ds>;
  if (! add_attr<Mb>(m, "Modifier_base")) {
    py::class_<Mb>(m, "Modifier_base")
      ;
  }

  using Pb = cgalpy::pol3::Polyhedron_builder;
  if (! add_attr<Pb>(m, "Polyhedron_builder")) {
    py::class_<Pb, Mb>(m, "Polyhedron_builder")
      .def(py::init<py::object>())
      .def("data", &Pb::data)
      .def("set_data", &Pb::set_data)
      .def("set_operator", &Pb::set_operator)
      .def("__call__", &Pb::operator())
      ;
  }
}
