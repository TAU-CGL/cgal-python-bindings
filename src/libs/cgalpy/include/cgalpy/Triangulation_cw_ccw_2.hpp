#ifndef CGALPY_TRIANGULATION_CW_CCW_2_HPP
#define CGALPY_TRIANGULATION_CW_CCW_2_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Triangulation_2.h>

namespace py = nanobind;

namespace cgalpy {
namespace tri2 {

template <typename TriangulationType, typename C>
auto export_triangulation_cw_ccw_2(C& c) {
  return c
    .def(py::init<>(),
         "default constructor.")
    .def_static("cw", &TriangulationType::cw,
         py::arg("i"),
         "returns the index of the neighbor or vertex that is next to the neighbor or vertex with index i in counterclockwise order around a face.")
    .def_static("ccw", &TriangulationType::ccw,
         py::arg("i"),
         "returns the index of the neighbor or vertex that is next to the neighbor or vertex with index i in counterclockwise order around a face.")
    ;
}


}
} // namespace cgalpy // namespace tri2

#endif // CGALPY_TRIANGULATION_CW_CCW_2_HPP
