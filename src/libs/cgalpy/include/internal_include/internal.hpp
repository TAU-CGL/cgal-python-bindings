#ifndef CGALPY_INTERNAL_HPP
#define CGALPY_INTERNAL_HPP

#include <CGAL/Named_function_parameters.h>
#include <CGAL/exceptions.h>
#include <csignal>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
// #include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/polygon_mesh_processing_config.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#endif // CGALPY_POLYGON_MESH_PROCESSING_BINDINGS_HPP

namespace py = nanobind;

namespace internal {

typedef CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> Named_params;

// string hash function from: https://gist.github.com/EvanMcBroom/2a9bed888c2755153a9616aa7ae1f79a
template <typename _T>
unsigned int constexpr Hash(_T const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * Hash(input + 1) : 5381;
}

Named_params parse_named_parameters(const py::dict& params);

} // namespace parse_params

#endif //CGALPY_INTERNAL_HPP
