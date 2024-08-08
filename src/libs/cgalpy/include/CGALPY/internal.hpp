#ifndef CGALPY_INTERNAL_HPP
#define CGALPY_INTERNAL_HPP

#include <CGAL/Named_function_parameters.h>
#include <CGAL/exceptions.h>
#include <csignal>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace py = nanobind;

namespace internal {

// string hash function from: https://gist.github.com/EvanMcBroom/2a9bed888c2755153a9616aa7ae1f79a
template <typename _T>
unsigned int constexpr Hash(_T const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * Hash(input + 1) : 5381;
}


using Named_params = CGAL::Named_function_parameters<bool, CGAL::internal_np::verbose_t>;
Named_params parse_named_parameters(const py::dict& params, Named_params cgal_parameters = CGAL::parameters::verbose(false));

std::vector<std::vector<size_t>> polylist2polyvec_i(const py::list& polylist);
py::list polyvec2polylist_i(const std::vector<std::vector<std::size_t>> polyvec);

} // namespace internal

#endif //CGALPY_INTERNAL_HPP
//
