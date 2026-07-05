// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <array>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>

#include <CGAL/approximate_convex_decomposition.h>

#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/sm/surface_mesh_decomposition_types.hpp"
#include "cgalpy/Smdec_docstrings.hpp"

namespace py = nanobind;

namespace {

using Surface_mesh_3 = cgalpy::sm::Surface_mesh_3;
using Point_3 = Kernel::Point_3;
using Triangle = std::array<unsigned int, 3>;
using Convex_volume = std::pair<std::vector<Point_3>, std::vector<Triangle>>;
namespace smdec_doc = cgalpy::smdec::docstrings;

struct Named_parameter_maximum_number_of_voxels {
  const std::string m_name = "maximum_number_of_voxels";

  template <typename NamedParameters>
  auto operator()(NamedParameters np, const py::handle& value) const {
    try {
      return np.maximum_number_of_voxels(py::cast<unsigned int>(value));
    }
    catch (const py::cast_error&) {
      throw std::invalid_argument("Named parameter 'maximum_number_of_voxels' must be an unsigned int.");
    }
  }
};

struct Named_parameter_maximum_depth {
  const std::string m_name = "maximum_depth";

  template <typename NamedParameters>
  auto operator()(NamedParameters np, const py::handle& value) const {
    try {
      return np.maximum_depth(py::cast<unsigned int>(value));
    }
    catch (const py::cast_error&) {
      throw std::invalid_argument("Named parameter 'maximum_depth' must be an unsigned int.");
    }
  }
};

struct Named_parameter_refitting {
  const std::string m_name = "refitting";

  template <typename NamedParameters>
  auto operator()(NamedParameters np, const py::handle& value) const {
    try {
      return np.refitting(py::cast<bool>(value));
    }
    catch (const py::cast_error&) {
      throw std::invalid_argument("Named parameter 'refitting' must be a boolean.");
    }
  }
};

struct Named_parameter_maximum_number_of_convex_volumes {
  const std::string m_name = "maximum_number_of_convex_volumes";

  template <typename NamedParameters>
  auto operator()(NamedParameters np, const py::handle& value) const {
    try {
      return np.maximum_number_of_convex_volumes(py::cast<unsigned int>(value));
    }
    catch (const py::cast_error&) {
      throw std::invalid_argument("Named parameter 'maximum_number_of_convex_volumes' must be an unsigned int.");
    }
  }
};

struct Named_parameter_volume_error {
  const std::string m_name = "volume_error";

  template <typename NamedParameters>
  auto operator()(NamedParameters np, const py::handle& value) const {
    try {
      return np.volume_error(py::cast<double>(value));
    }
    catch (const py::cast_error&) {
      throw std::invalid_argument("Named parameter 'volume_error' must be a number.");
    }
  }
};

struct Named_parameter_split_at_concavity {
  const std::string m_name = "split_at_concavity";

  template <typename NamedParameters>
  auto operator()(NamedParameters np, const py::handle& value) const {
    try {
      return np.split_at_concavity(py::cast<bool>(value));
    }
    catch (const py::cast_error&) {
      throw std::invalid_argument("Named parameter 'split_at_concavity' must be a boolean.");
    }
  }
};

std::vector<Convex_volume>
approximate_convex_decomposition(const Surface_mesh_3& tmesh,
                                 const py::dict& np = py::dict()) {
  std::vector<Convex_volume> convex_volumes;

  auto wrapper = [&tmesh, &convex_volumes](const auto& params) {
    return CGAL::approximate_convex_decomposition
      (tmesh, std::back_inserter(convex_volumes), params);
  };

  cgalpy::Named_parameter_vertex_point_map<Surface_mesh_3> vertex_point_map_op;
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  Named_parameter_maximum_number_of_voxels maximum_number_of_voxels_op;
  Named_parameter_maximum_depth maximum_depth_op;
  Named_parameter_refitting refitting_op;
  Named_parameter_maximum_number_of_convex_volumes maximum_number_of_convex_volumes_op;
  Named_parameter_volume_error volume_error_op;
  Named_parameter_split_at_concavity split_at_concavity_op;

  auto params = CGAL::parameters::default_values();
  cgalpy::named_parameter_applicator
    (wrapper, params, np,
     vertex_point_map_op,
     geom_traits_op,
     maximum_number_of_voxels_op,
     maximum_depth_op,
     refitting_op,
     maximum_number_of_convex_volumes_op,
     volume_error_op,
     split_at_concavity_op);

  return convex_volumes;
}

} // namespace

void export_surface_mesh_decomposition(py::module_& m) {
  m.def("approximate_convex_decomposition",
        &approximate_convex_decomposition,
        py::arg("tmesh"),
        py::arg("np") = py::dict(),
        smdec_doc::approximate_convex_decomposition);
}
