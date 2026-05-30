#include <nanobind/nanobind.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/polyhedron_3_types.hpp"

namespace py = nanobind;

/*!
 */
void export_polyhedron_halfedge_ds(py::module_& m) {
  using Hds = cgalpy::pol3::Halfedge_ds;
  if (! add_attr<Hds>(m, "Halfedge_ds")) {
    py::class_<Hds>(m, "Halfedge_ds",
                    "Halfedge data structure used by Polyhedron_3.")
      ;
  }
}
