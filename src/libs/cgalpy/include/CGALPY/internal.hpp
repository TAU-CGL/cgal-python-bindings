#ifndef CGALPY_PARSE_NAMED_PARAMS_HPP
#define CGALPY_PARSE_NAMED_PARAMS_HPP

#include <CGAL/Named_function_parameters.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>


namespace py = nanobind;

namespace internal {


CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> parse_named_parameters(const py::dict& params) {
  // iterate throught all params and add them to the NamedParameter
  CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> cgal_parameters = CGAL::parameters::all_default();

  for (const auto& item : params) {
    // cast key to C++ string
    std::string key = py::cast<std::string>(item.first);
    if (key == "number_of_points_per_area_unit") {
      // second item is int
      int value = py::cast<int>(item.second);
      cgal_parameters = cgal_parameters.number_of_points_per_area_unit(value);
    }
    else if (key == "number_of_iterations") {
      // second item is int
      int value = py::cast<int>(item.second);
      cgal_parameters = cgal_parameters.number_of_iterations(value);
    }
    else {
      // log unknown parameter
      std::cerr << "Unknown parameter: " << key << "\n";
    }
  }
  return cgal_parameters;
}

} // namespace parse_params

#endif //CGALPY_PARSE_PARAMS_HPP
