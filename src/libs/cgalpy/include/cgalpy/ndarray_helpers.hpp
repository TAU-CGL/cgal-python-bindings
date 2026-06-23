// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NDARRAY_HELPERS_HPP
#define CGALPY_NDARRAY_HELPERS_HPP

#include <stdexcept>
#include <string>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>

namespace py = nanobind;

namespace cgalpy {

//! Convert a 2D NumPy-style array with shape (N, 3) to std::vector<Point_3>.
template <typename Point_3>
std::vector<Point_3> ndarray_to_point_3_vector(const py::ndarray<>& array,
                                               const char* argument_name) {
  if (! array.is_valid()) {
    throw std::runtime_error(std::string(argument_name) + " must be a valid NumPy array.");
  }

  if (array.ndim() != 2) {
    throw std::runtime_error(std::string(argument_name) + " must be a 2D NumPy array with shape (N, 3).");
  }

  if (array.shape(1) != 3) {
    throw std::runtime_error(std::string(argument_name) + " must have shape (N, 3).");
  }

  const auto dtype = array.dtype();

  if (dtype.code != static_cast<uint8_t>(py::dlpack::dtype_code::Float) || dtype.bits != 64) {
    throw std::runtime_error(std::string(argument_name) + " must have dtype float64.");
  }

  const auto* data = static_cast<const double*>(array.data());
  const auto stride0 = array.stride(0);
  const auto stride1 = array.stride(1);
  const std::size_t n = array.shape(0);

  std::vector<Point_3> points;
  points.reserve(n);

  for (std::size_t i = 0; i < n; ++i) {
    const double x = *(data + i * stride0 + 0 * stride1);
    const double y = *(data + i * stride0 + 1 * stride1);
    const double z = *(data + i * stride0 + 2 * stride1);
    points.emplace_back(x, y, z);
  }

  return points;
}

} // namespace cgalpy

#endif
