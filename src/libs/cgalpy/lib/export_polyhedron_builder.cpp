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
    py::class_<Mb>(m, "Modifier_base",
                   "Base class for polyhedron modifiers.")
      ;
  }

  using Pb = cgalpy::pol3::Polyhedron_builder;
  if (! add_attr<Pb>(m, "Polyhedron_builder")) {
    py::class_<Pb, Mb>(m, "Polyhedron_builder",
                        "Helper object for building a Polyhedron_3.")
      .def(py::init<py::object>(), py::arg("data"),
           "Constructs a polyhedron builder with user data.")
      .def("data", &Pb::data,
           "Returns the user data attached to the builder.")
      .def("set_data", &Pb::set_data, py::arg("data"),
           "Sets the user data attached to the builder.")
      .def("set_operator", &Pb::set_operator, py::arg("operator"),
           "Sets the Python callable used by the builder.")
      .def("__call__", &Pb::operator(), py::arg("hds"),
           "Applies the builder to a halfedge data structure.")
      ;
  }
}
