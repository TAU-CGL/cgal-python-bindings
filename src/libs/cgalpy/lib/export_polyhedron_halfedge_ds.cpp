#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

namespace py = nanobind;

/*!
 */
void export_polyhedron_halfedge_ds(py::module_& m) {
  using Hds = pol3::Halfedge_ds;
  if (! add_attr<Hds>(m, "Halfedge_ds")) {
    py::class_<Hds>(m, "Halfedge_ds")
      ;
  }
}
