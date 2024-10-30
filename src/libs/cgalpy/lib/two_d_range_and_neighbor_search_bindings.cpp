#include <nanobind/nanobind.h>

#include "CGALPY/Triangulation_2.hpp"
#include "CGALPY/kernel_type.hpp"

namespace py = nanobind;


void export_2d_range_and_neighbor_search(py::module_& m) {
  using K = Kernel;

  // tests
  py::class_<CGAL::Triangulation_2<K>> tri_c(m, "Triangulation_2");
  tri2::export_triangulation_2<CGAL::Triangulation_2<K>>(tri_c);

  py::class_<CGAL::Triangulation_cw_ccw_2> cw_ccw_c(m, "Triangulation_cw_ccw_2");
  tri2::export_triangulation_cw_ccw_2<CGAL::Triangulation_2<K>>(cw_ccw_c);
}

