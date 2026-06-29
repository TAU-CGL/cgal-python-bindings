// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Efi Fogel          <efifogel@gmail.com>
//            Utkarsh Khajuria   <utkarshkhajuria55@gmail.com>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>

#include <CGAL/Point_set_3.h>
#include <CGAL/property_map.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/estimate_scale.h>
#include <CGAL/bilateral_smooth_point_set.h>
#include <CGAL/cluster_point_set.h>
// #include <CGAL/compute_average_spacing.h>
#include <CGAL/edge_aware_upsample_point_set.h>
// #include <CGAL/estimate_scale.h>
#include <CGAL/vcm_estimate_normals.h>
// #include <CGAL/Named_function_parameters.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/hierarchy_simplify_point_set.h>
#include <CGAL/jet_estimate_normals.h>
#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/mst_orient_normals.h>
#include <CGAL/pca_estimate_normals.h>
#include <CGAL/random_simplify_point_set.h>
#include <CGAL/remove_outliers.h>
// #include <CGAL/scanline_orient_normals.h>
// #include <CGAL/vcm_estimate_normals.h>
#include <CGAL/vcm_estimate_edges.h>
#include <CGAL/wlop_simplify_and_regularize_point_set.h>
#include <CGAL/IO/write_points.h>

#include "cgalpy/kernel_type.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/ndarray_to_point_3_vector.hpp"
#include "cgalpy/point_set_processing_type.hpp"

namespace py = nanobind;

namespace psp {

/*! A class template that wraps the function template
 * PMP::read_points()
 */
template <typename T, typename... Args>
struct Read_points_wrapper {
  static auto call(T np, Args&&... args) {
    return CGAL::IO::read_points(std::forward<Args>(args)..., std::forward<T>(np));
  }
};

//!
template <typename OutputIterator>
bool read_points_impl(const std::string& fname, OutputIterator oi,
                      const py::dict& params) {
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits op;
  cgalpy::Named_parameter_wrapper<Read_points_wrapper,
                                  const std::string&, OutputIterator>
    wrapper(fname, std::move(oi));
  return cgalpy::named_parameter_applicator(wrapper, np, params, op);
}

//! \todo replace return value with numpy array
auto read_points(const std::string& fname,
                 const py::dict& params = py::dict()) {
  py::list points;
#if 1
  std::vector<Point_3> tmp;
  bool res = read_points_impl(fname, std::back_inserter(tmp), params);
  for (const auto& p : tmp) points.append(p);
#else
  auto op = [&] (const Point_3& p) mutable { points.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  bool res = read_points_impl(fname, it, params);
#endif
  if (! res) throw std::runtime_error("Cannot read points!");
  return points;
}

//! Compute average spacing from a point range.
template <typename Point_3>
auto compute_average_spacing(const std::vector<Point_3>& points,
                             const unsigned int k,
                             const py::dict& params = py::dict()) {
  (void) params;
  return CGAL::compute_average_spacing<CGAL::Sequential_tag>(points, k);
}

//! Compute average spacing from a NumPy-style point array.
template <typename Point_3>
auto compute_average_spacing_np(const py::ndarray<>& points_array,
                                const unsigned int k,
                                const py::dict& params = py::dict()) {
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return compute_average_spacing(points, k, params);
}

//! Compute average spacing from a Point_set_3.
template <typename PointSet_3>
auto compute_average_spacing_point_set(const PointSet_3& points,
                                       const unsigned int k,
                                       const py::dict& params = py::dict()) {
  (void) params;
  return CGAL::compute_average_spacing<CGAL::Sequential_tag>(points, k);
}

//! Estimate global scale in the K nearest neighbors sense from a point range.
template <typename Point_3>
auto estimate_global_k_neighbor_scale(const std::vector<Point_3>& points,
                                      const py::dict& params = py::dict()) {
  (void) params;
  return CGAL::estimate_global_k_neighbor_scale(points);
}

//! Estimate global scale in the K nearest neighbors sense from a NumPy-style point array.
template <typename Point_3>
auto estimate_global_k_neighbor_scale_np(const py::ndarray<>& points_array,
                                         const py::dict& params = py::dict()) {
  (void) params;
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return estimate_global_k_neighbor_scale(points);
}

//! Estimate global scale in the range sense from a point range.
template <typename Point_3>
auto estimate_global_range_scale(const std::vector<Point_3>& points,
                                 const py::dict& params = py::dict()) {
  (void) params;
  return CGAL::estimate_global_range_scale(points);
}

//! Estimate global scale in the range sense from a NumPy-style point array.
template <typename Point_3>
auto estimate_global_range_scale_np(const py::ndarray<>& points_array,
                                    const py::dict& params = py::dict()) {
  (void) params;
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return estimate_global_range_scale(points);
}

//! Estimate local scales in the K nearest neighbors sense for query points.
template <typename Point_3>
auto estimate_local_k_neighbor_scales(const std::vector<Point_3>& points,
                                      const std::vector<Point_3>& queries,
                                      const py::dict& params = py::dict()) {
  (void) params;
  std::vector<std::size_t> output;
  CGAL::estimate_local_k_neighbor_scales(points, queries, std::back_inserter(output));
  return output;
}

//! Estimate local scales in the K nearest neighbors sense from NumPy-style point arrays.
template <typename Point_3>
auto estimate_local_k_neighbor_scales_np(const py::ndarray<>& points_array,
                                         const py::ndarray<>& queries_array,
                                         const py::dict& params = py::dict()) {
  (void) params;
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  const auto queries =
    cgalpy::ndarray_to_point_3_vector<Point_3>(queries_array, "queries");
  return estimate_local_k_neighbor_scales(points, queries);
}

//! Estimate local scales in the range sense for query points.
template <typename Point_3>
auto estimate_local_range_scales(const std::vector<Point_3>& points,
                                 const std::vector<Point_3>& queries,
                                 const py::dict& params = py::dict()) {
  (void) params;
  using FT = typename Kernel::FT;
  std::vector<FT> output;
  CGAL::estimate_local_range_scales(points, queries, std::back_inserter(output));
  return output;
}

//! Estimate local scales in the range sense from NumPy-style point arrays.
template <typename Point_3>
auto estimate_local_range_scales_np(const py::ndarray<>& points_array,
                                    const py::ndarray<>& queries_array,
                                    const py::dict& params = py::dict()) {
  (void) params;
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  const auto queries =
    cgalpy::ndarray_to_point_3_vector<Point_3>(queries_array, "queries");
  return estimate_local_range_scales(points, queries);
}


//! Simplify points by grid clustering.
template <typename Point_3>
auto grid_simplify_point_set(std::vector<Point_3> points,
                             const double epsilon,
                             const py::dict& params = py::dict()) {
  const unsigned int min_points_per_cell =
    params.contains("min_points_per_cell") ?
    py::cast<unsigned int>(params["min_points_per_cell"]) : 1;
  auto it = CGAL::grid_simplify_point_set
    (points, epsilon,
     CGAL::parameters::min_points_per_cell(min_points_per_cell));
  return std::make_pair(points, std::distance(points.begin(), it));
}

//! Simplify NumPy-style points by grid clustering.
template <typename Point_3>
auto grid_simplify_point_set_np(const py::ndarray<>& points_array,
                                const double epsilon,
                                const py::dict& params = py::dict()) {
  auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return grid_simplify_point_set(points, epsilon, params);
}

//! Randomly simplify a point set.
template <typename Point_3>
auto random_simplify_point_set(std::vector<Point_3> points,
                               const double removed_percentage,
                               const py::dict& params = py::dict()) {
  (void) params;
  auto it = CGAL::random_simplify_point_set(points, removed_percentage);
  return std::make_pair(points, std::distance(points.begin(), it));
}

//! Randomly simplify NumPy-style points.
template <typename Point_3>
auto random_simplify_point_set_np(const py::ndarray<>& points_array,
                                  const double removed_percentage,
                                  const py::dict& params = py::dict()) {
  auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return random_simplify_point_set(points, removed_percentage, params);
}

//! Simplify points using hierarchy simplification.
template <typename Point_3>
auto hierarchy_simplify_point_set(std::vector<Point_3> points,
                                  const py::dict& params = py::dict()) {
  const unsigned int size =
    params.contains("size") ? py::cast<unsigned int>(params["size"]) : 10;
  const double maximum_variation =
    params.contains("maximum_variation") ?
    py::cast<double>(params["maximum_variation"]) : 1.0 / 3.0;
  auto it = CGAL::hierarchy_simplify_point_set
    (points,
     CGAL::parameters::size(size)
     .maximum_variation(maximum_variation));
  return std::make_pair(points, std::distance(points.begin(), it));
}

//! Simplify NumPy-style points using hierarchy simplification.
template <typename Point_3>
auto hierarchy_simplify_point_set_np(const py::ndarray<>& points_array,
                                     const py::dict& params = py::dict()) {
  auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return hierarchy_simplify_point_set(points, params);
}

//! Smooth points using jet fitting.
template <typename Point_3>
auto jet_smooth_point_set(std::vector<Point_3> points,
                          const unsigned int k,
                          const py::dict& params = py::dict()) {
  using FT = typename Kernel::FT;
  const FT neighbor_radius =
    params.contains("neighbor_radius") ?
    FT(py::cast<double>(params["neighbor_radius"])) : FT(0);
  const unsigned int degree_fitting =
    params.contains("degree_fitting") ?
    py::cast<unsigned int>(params["degree_fitting"]) : 2;
  const unsigned int degree_monge =
    params.contains("degree_monge") ?
    py::cast<unsigned int>(params["degree_monge"]) : 2;

  CGAL::jet_smooth_point_set<CGAL::Sequential_tag>
    (points, k,
     CGAL::parameters::neighbor_radius(neighbor_radius)
     .degree_fitting(degree_fitting)
     .degree_monge(degree_monge));
  return points;
}

//! Smooth NumPy-style points using jet fitting.
template <typename Point_3>
auto jet_smooth_point_set_np(const py::ndarray<>& points_array,
                             const unsigned int k,
                             const py::dict& params = py::dict()) {
  auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return jet_smooth_point_set(points, k, params);
}

//! Remove outliers from a point set.
template <typename Point_3>
auto remove_outliers(std::vector<Point_3> points,
                     const unsigned int k,
                     const py::dict& params = py::dict()) {
  using FT = typename Kernel::FT;
  const FT neighbor_radius =
    params.contains("neighbor_radius") ?
    FT(py::cast<double>(params["neighbor_radius"])) : FT(0);
  const double threshold_percent =
    params.contains("threshold_percent") ?
    py::cast<double>(params["threshold_percent"]) : 10.0;
  const FT threshold_distance =
    params.contains("threshold_distance") ?
    FT(py::cast<double>(params["threshold_distance"])) : FT(0);

  auto it = CGAL::remove_outliers<CGAL::Sequential_tag>
    (points, k,
     CGAL::parameters::neighbor_radius(neighbor_radius)
     .threshold_percent(threshold_percent)
     .threshold_distance(threshold_distance));
  return std::make_pair(points, std::distance(points.begin(), it));
}

//! Remove outliers from NumPy-style points.
template <typename Point_3>
auto remove_outliers_np(const py::ndarray<>& points_array,
                        const unsigned int k,
                        const py::dict& params = py::dict()) {
  auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return remove_outliers(points, k, params);
}


//! Convert NumPy-style point and normal arrays to point-normal pairs.
template <typename Point_3, typename Vector_3>
std::vector<std::pair<Point_3, Vector_3>>
ndarray_to_point_normal_3_vector(const py::ndarray<>& points_array,
                                 const py::ndarray<>& normals_array) {
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  const auto normals =
    cgalpy::ndarray_to_point_3_vector<Vector_3>(normals_array, "normals");

  if (points.size() != normals.size()) {
    throw std::runtime_error("points and normals must have the same number of rows.");
  }

  std::vector<std::pair<Point_3, Vector_3>> output;
  output.reserve(points.size());

  for (std::size_t i = 0; i < points.size(); ++i) {
    output.emplace_back(points[i], normals[i]);
  }

  return output;
}

//! Read points with normals from a file.
template <typename Point_3, typename Vector_3>
auto read_points_with_normals(const std::string& fname,
                              const py::dict& params = py::dict()) {
  (void) params;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  std::vector<PointNormalPair> output;
  const bool success =
    CGAL::IO::read_points(fname, std::back_inserter(output),
                          CGAL::parameters::point_map(Point_map())
                          .normal_map(Normal_map()));
  if (! success) output.clear();
  return output;
}

//! Compute average spacing for points with normals.
template <typename Point_3, typename Vector_3>
auto compute_average_spacing_with_normals(
  const std::vector<std::pair<Point_3, Vector_3>>& points,
  const unsigned int k,
  const py::dict& params = py::dict()) {
  (void) params;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;

  return CGAL::compute_average_spacing<CGAL::Sequential_tag>
    (points, k, CGAL::parameters::point_map(Point_map()));
}

//!
template <typename Point_3, typename Vector_3>
auto compute_average_spacing_with_normals_np(const py::ndarray<>& points_array,
                                             const py::ndarray<>& normals_array,
                                             const unsigned int k,
                                             const py::dict& params = py::dict()) {
  const auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return compute_average_spacing_with_normals(points, k, params);
}

//!

//! Simplify points with normals by grid clustering.
template <typename Point_3, typename Vector_3>
auto grid_simplify_point_set_with_normals(
  std::vector<std::pair<Point_3, Vector_3>> points,
  const double epsilon,
  const py::dict& params = py::dict()) {
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;

  const unsigned int min_points_per_cell =
    params.contains("min_points_per_cell") ?
    py::cast<unsigned int>(params["min_points_per_cell"]) : 1;

  auto it = CGAL::grid_simplify_point_set
    (points, epsilon,
     CGAL::parameters::min_points_per_cell(min_points_per_cell)
     .point_map(Point_map()));

  return std::make_pair(points, std::distance(points.begin(), it));
}

//!
template <typename Point_3, typename Vector_3>
auto grid_simplify_point_set_with_normals_np(const py::ndarray<>& points_array,
                                             const py::ndarray<>& normals_array,
                                             const double epsilon,
                                             const py::dict& params = py::dict()) {
  auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return grid_simplify_point_set_with_normals(points, epsilon, params);
}

//!

//! Identify clusters in a Point_set_3 and collect cluster adjacencies.
template <typename PointSet_3, typename ClusterMap_3>
auto cluster_point_set(PointSet_3& points,
                       ClusterMap_3& cluster_map,
                       const py::dict& params = py::dict()) {
  const double attraction_factor =
    params.contains("attraction_factor") ?
    py::cast<double>(params["attraction_factor"]) : 2.0;

  std::vector<std::pair<std::size_t, std::size_t>> adjacencies;
  std::size_t number_of_clusters = 0;

  if (params.contains("neighbor_radius")) {
    const double neighbor_radius = py::cast<double>(params["neighbor_radius"]);
    number_of_clusters =
      CGAL::cluster_point_set(points, cluster_map,
                              CGAL::parameters::attraction_factor(attraction_factor)
                              .adjacencies(std::back_inserter(adjacencies))
                              .neighbor_radius(neighbor_radius));
  }
  else {
    number_of_clusters =
      CGAL::cluster_point_set(points, cluster_map,
                              CGAL::parameters::attraction_factor(attraction_factor)
                              .adjacencies(std::back_inserter(adjacencies)));
  }

  return std::make_pair(number_of_clusters, adjacencies);
}

//! Estimate normals using PCA.
template <typename Point_3, typename Vector_3>
auto pca_estimate_normals(std::vector<std::pair<Point_3, Vector_3>> points,
                          const unsigned int k,
                          const py::dict& params = py::dict()) {
  using FT = typename Kernel::FT;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  const FT neighbor_radius =
    params.contains("neighbor_radius") ?
    FT(py::cast<double>(params["neighbor_radius"])) : FT(0);

  CGAL::pca_estimate_normals<CGAL::Sequential_tag>
    (points, k,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map())
     .neighbor_radius(neighbor_radius));
  return points;
}

//! Estimate normals from NumPy-style points using PCA.
template <typename Point_3, typename Vector_3>
auto pca_estimate_normals_np(const py::ndarray<>& points_array,
                             const unsigned int k,
                             const py::dict& params = py::dict()) {
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");

  std::vector<std::pair<Point_3, Vector_3>> point_normals;
  point_normals.reserve(points.size());
  for (const auto& point : points) {
    point_normals.emplace_back(point, Vector_3(0, 0, 0));
  }

  return pca_estimate_normals(point_normals, k, params);
}

//! Estimate normals using jet fitting.
template <typename Point_3, typename Vector_3>
auto jet_estimate_normals(std::vector<std::pair<Point_3, Vector_3>> points,
                          const unsigned int k,
                          const py::dict& params = py::dict()) {
  using FT = typename Kernel::FT;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  const FT neighbor_radius =
    params.contains("neighbor_radius") ?
    FT(py::cast<double>(params["neighbor_radius"])) : FT(0);
  const unsigned int degree_fitting =
    params.contains("degree_fitting") ?
    py::cast<unsigned int>(params["degree_fitting"]) : 2;

  CGAL::jet_estimate_normals<CGAL::Sequential_tag>
    (points, k,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map())
     .neighbor_radius(neighbor_radius)
     .degree_fitting(degree_fitting));
  return points;
}

//! Estimate normals from NumPy-style points using jet fitting.
template <typename Point_3, typename Vector_3>
auto jet_estimate_normals_np(const py::ndarray<>& points_array,
                             const unsigned int k,
                             const py::dict& params = py::dict()) {
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");

  std::vector<std::pair<Point_3, Vector_3>> point_normals;
  point_normals.reserve(points.size());
  for (const auto& point : points) {
    point_normals.emplace_back(point, Vector_3(0, 0, 0));
  }

  return jet_estimate_normals(point_normals, k, params);
}

//! Orient normals using MST propagation.
template <typename Point_3, typename Vector_3>
auto mst_orient_normals(std::vector<std::pair<Point_3, Vector_3>> points,
                        const unsigned int k,
                        const py::dict& params = py::dict()) {
  using FT = typename Kernel::FT;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  const FT neighbor_radius =
    params.contains("neighbor_radius") ?
    FT(py::cast<double>(params["neighbor_radius"])) : FT(0);

  auto it = CGAL::mst_orient_normals
    (points, k,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map())
     .neighbor_radius(neighbor_radius));

  return std::make_pair(points, std::distance(points.begin(), it));
}

//! Orient normals from NumPy-style points and normals using MST propagation.
template <typename Point_3, typename Vector_3>
auto mst_orient_normals_np(const py::ndarray<>& points_array,
                           const py::ndarray<>& normals_array,
                           const unsigned int k,
                           const py::dict& params = py::dict()) {
  auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return mst_orient_normals(points, k, params);
}

//! Compute Voronoi covariance matrices for a point range.
template <typename Point_3>
auto compute_vcm(const std::vector<Point_3>& points,
                 const double offset_radius,
                 const double convolution_radius,
                 const py::dict& params = py::dict()) {
  (void) params;
  using FT = typename Kernel::FT;
  using Covariance = std::array<FT, 6>;

  std::vector<Covariance> output(points.size());
  CGAL::compute_vcm(points, output, offset_radius, convolution_radius);
  return output;
}

//! Compute Voronoi covariance matrices for a NumPy-style point array.
template <typename Point_3>
auto compute_vcm_np(const py::ndarray<>& points_array,
                    const double offset_radius,
                    const double convolution_radius,
                    const py::dict& params = py::dict()) {
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return compute_vcm(points, offset_radius, convolution_radius, params);
}

//! Compute Voronoi covariance matrices for points with normals.
template <typename Point_3, typename Vector_3>
auto compute_vcm_with_normals(
  const std::vector<std::pair<Point_3, Vector_3>>& points,
  const double offset_radius,
  const double convolution_radius,
  const py::dict& params = py::dict()) {
  (void) params;
  using FT = typename Kernel::FT;
  using Covariance = std::array<FT, 6>;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;

  std::vector<Covariance> output(points.size());
  CGAL::compute_vcm(points, output, offset_radius, convolution_radius,
                    CGAL::parameters::point_map(Point_map()));
  return output;
}

//!
template <typename Point_3, typename Vector_3>
auto compute_vcm_with_normals_np(const py::ndarray<>& points_array,
                                 const py::ndarray<>& normals_array,
                                 const double offset_radius,
                                 const double convolution_radius,
                                 const py::dict& params = py::dict()) {
  const auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return compute_vcm_with_normals(points, offset_radius, convolution_radius,
                                  params);
}

//!

//! Estimate normals using Voronoi covariance matrices.
template <typename Point_3, typename Vector_3>
auto vcm_estimate_normals(std::vector<std::pair<Point_3, Vector_3>> points,
                          const double offset_radius,
                          const double convolution_radius,
                          const py::dict& params = py::dict()) {
  (void) params;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  CGAL::vcm_estimate_normals
    (points, offset_radius, convolution_radius,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map()));
  return points;
}

//!
template <typename Point_3, typename Vector_3>
auto vcm_estimate_normals_np(const py::ndarray<>& points_array,
                             const py::ndarray<>& normals_array,
                             const double offset_radius,
                             const double convolution_radius,
                             const py::dict& params = py::dict()) {
  auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return vcm_estimate_normals(points, offset_radius, convolution_radius,
                              params);
}

//!

//! Estimate normals using Voronoi covariance matrices and neighbor count.
template <typename Point_3, typename Vector_3>
auto vcm_estimate_normals_neighbors(
  std::vector<std::pair<Point_3, Vector_3>> points,
  const double offset_radius,
  const unsigned int k,
  const py::dict& params = py::dict()) {
  (void) params;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  CGAL::vcm_estimate_normals
    (points, offset_radius, k,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map()));
  return points;
}

//!
template <typename Point_3, typename Vector_3>
auto vcm_estimate_normals_neighbors_np(const py::ndarray<>& points_array,
                                       const py::ndarray<>& normals_array,
                                       const double offset_radius,
                                       const unsigned int k,
                                       const py::dict& params = py::dict()) {
  auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return vcm_estimate_normals_neighbors(points, offset_radius, k, params);
}

//!

//! Test whether a Voronoi covariance matrix marks a feature edge.
template <typename FT>
bool vcm_is_on_feature_edge(std::array<FT, 6> cov,
                            const double threshold) {
  return CGAL::vcm_is_on_feature_edge(cov, threshold);
}

//! Write a point range to a file.
template <typename Point_3>
bool write_points(const std::string& fname,
                  const std::vector<Point_3>& points,
                  const py::dict& params = py::dict()) {
  (void) params;
  return CGAL::IO::write_points(fname, points,
                                CGAL::parameters::stream_precision(17));
}

//!
template <typename Point_3>
bool write_points_np(const std::string& fname,
                     const py::ndarray<>& points_array,
                     const py::dict& params = py::dict()) {
  const auto points =
    cgalpy::ndarray_to_point_3_vector<Point_3>(points_array, "points");
  return write_points(fname, points, params);
}

//! Write points with normals to a file.
template <typename Point_3, typename Vector_3>
bool write_points_with_normals(
  const std::string& fname,
  const std::vector<std::pair<Point_3, Vector_3>>& points,
  const py::dict& params = py::dict()) {
  (void) params;
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  return CGAL::IO::write_points
    (fname, points,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map())
     .stream_precision(17));
}

//!
template <typename Point_3, typename Vector_3>
bool write_points_with_normals_np(const std::string& fname,
                                  const py::ndarray<>& points_array,
                                  const py::ndarray<>& normals_array,
                                  const py::dict& params = py::dict()) {
  const auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return write_points_with_normals(fname, points, params);
}

//!

//! Smooth points with normals using bilateral projection.
template <typename Point_3, typename Vector_3>
auto bilateral_smooth_point_set(
  std::vector<std::pair<Point_3, Vector_3>> points,
  const unsigned int k,
  const py::dict& params = py::dict()) {
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;
  using FT = typename Kernel::FT;

  const double sharpness_angle =
    params.contains("sharpness_angle") ?
    py::cast<double>(params["sharpness_angle"]) : 30.0;

  const FT neighbor_radius =
    params.contains("neighbor_radius") ?
    FT(py::cast<double>(params["neighbor_radius"])) : FT(0);

  const double error = CGAL::bilateral_smooth_point_set<CGAL::Sequential_tag>
    (points, k,
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map())
     .sharpness_angle(sharpness_angle)
     .neighbor_radius(neighbor_radius));

  return std::make_pair(error, points);
}

//!
template <typename Point_3, typename Vector_3>
auto bilateral_smooth_point_set_np(const py::ndarray<>& points_array,
                                   const py::ndarray<>& normals_array,
                                   const unsigned int k,
                                   const py::dict& params = py::dict()) {
  auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return bilateral_smooth_point_set(points, k, params);
}

//!

//! Upsample points with normals while preserving sharp features.
template <typename Point_3, typename Vector_3>
std::vector<std::pair<Point_3, Vector_3>> edge_aware_upsample_point_set(
  std::vector<std::pair<Point_3, Vector_3>> points,
  const py::dict& params = py::dict()) {
  using PointNormalPair = std::pair<Point_3, Vector_3>;
  using Point_map = CGAL::First_of_pair_property_map<PointNormalPair>;
  using Normal_map = CGAL::Second_of_pair_property_map<PointNormalPair>;

  const double sharpness_angle =
    params.contains("sharpness_angle") ?
    py::cast<double>(params["sharpness_angle"]) : 30.0;

  const double edge_sensitivity =
    params.contains("edge_sensitivity") ?
    py::cast<double>(params["edge_sensitivity"]) : 1.0;

  const double neighbor_radius =
    params.contains("neighbor_radius") ?
    py::cast<double>(params["neighbor_radius"]) : -1.0;

  const std::size_t number_of_output_points =
    params.contains("number_of_output_points") ?
    py::cast<std::size_t>(params["number_of_output_points"]) :
    points.size() * 4;

  CGAL::edge_aware_upsample_point_set<CGAL::Sequential_tag>
    (points, std::back_inserter(points),
     CGAL::parameters::point_map(Point_map())
     .normal_map(Normal_map())
     .sharpness_angle(sharpness_angle)
     .edge_sensitivity(edge_sensitivity)
     .neighbor_radius(neighbor_radius)
     .number_of_output_points(number_of_output_points));

  return points;
}

//!
template <typename Point_3, typename Vector_3>
std::vector<std::pair<Point_3, Vector_3>>
edge_aware_upsample_point_set_np(const py::ndarray<>& points_array,
                                 const py::ndarray<>& normals_array,
                                 const py::dict& params = py::dict()) {
  auto points =
    ndarray_to_point_normal_3_vector<Point_3, Vector_3>(points_array,
                                                        normals_array);
  return edge_aware_upsample_point_set(points, params);
}

//!

//! Simplify and regularize a point set using WLOP.
template <typename Point_3>
std::vector<Point_3> wlop_simplify_and_regularize_point_set(
  std::vector<Point_3> points,
  const py::dict& params = py::dict()) {
  std::vector<Point_3> output;

  const double select_percentage =
    params.contains("select_percentage") ?
    py::cast<double>(params["select_percentage"]) : 5.0;

  const double neighbor_radius =
    params.contains("neighbor_radius") ?
    py::cast<double>(params["neighbor_radius"]) : -1.0;

  const unsigned int number_of_iterations =
    params.contains("number_of_iterations") ?
    py::cast<unsigned int>(params["number_of_iterations"]) : 35;

  const bool require_uniform_sampling =
    params.contains("require_uniform_sampling") ?
    py::cast<bool>(params["require_uniform_sampling"]) : false;

  CGAL::wlop_simplify_and_regularize_point_set<CGAL::Sequential_tag>
    (points, std::back_inserter(output),
     CGAL::parameters::select_percentage(select_percentage)
     .neighbor_radius(neighbor_radius)
     .number_of_iterations(number_of_iterations)
     .require_uniform_sampling(require_uniform_sampling));

  return output;
}

//! Simplify and regularize a NumPy-style point array using WLOP.
template <typename Point_3>
std::vector<Point_3> wlop_simplify_and_regularize_point_set_np(
  const py::ndarray<>& points,
  const py::dict& params = py::dict()) {
  return wlop_simplify_and_regularize_point_set<Point_3>
    (cgalpy::ndarray_to_point_3_vector<Point_3>(points, "points"), params);
}

}

#if 0
//!
struct dummy_callback {
  dummy_callback(const std::function<bool(double)>& callback) : callback(callback) {}
  bool operator()(double d) const { return callback(d); }
private:
  std::function<bool(double)> callback;
};

//!
template <typename PointRange, typename Point, typename Vector, typename C>
void export_functions_without_normals(C& c) {
  using K = Kernel;
  using Tag = CGAL::Sequential_tag;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))

  c.def("bilateral_smooth_point_set",
        [](PointRange& points, unsigned int k, const py::kwargs& np = py::kwargs()) {
          double r = CGAL::bilateral_smooth_point_set<Tag>(points, k);
          return std::make_pair(r, points);
        },
        py::arg("points"), py::arg("k"), py::arg("np"),
        "This function smooths an input point set by iteratively projecting each point onto the implicit surface patch fitted over its nearest neighbors. \n"
        "Bilateral projection preserves sharp features according to the normal (gradient) information. Both point positions and normals will be modified. For more details, please see section 4 in [5].\n\n"
        "Precondition\n"
        "• Normals must be unit vectors \n"
        "• k >= 2"
        "Parameters\n"
        "• points: input point range\n"
        "• k: size of the neighborhood for the implicit surface patch fitting. The larger the value is, the smoother the result will be.\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "• neighbor_radius\n"
        "• sharpness_angle\n"
        "Returns\n"
        "a tuple of the average point movement error. It's a convergence criterium for the algorithm. This value can help the user to decide how many iterations are sufficient, and the resulting points"
        );

  c.def("edge_aware_upsample_point_set",
        [](const PointRange& points, const py::kwargs& np = py::kwargs()) {
          std::vector<std::pair<Point, Vector>> output;
          CGAL::edge_aware_upsample_point_set<Tag>(points, std::back_inserter(output));
          return output;
        },
        py::arg("points"), py::arg("np"),
        "This method progressively upsamples the point set while approaching the edge singularities (detected by normal variation), which generates a denser point set from an input point set.\n"
        "This has applications in point-based rendering, hole filling, and sparse surface reconstruction. Normals of points are required as input. For more details, please refer to [5].\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "• sharpness_angle\n"
        "• edge_sensitivity\n"
        "• number_of_output_points\n"
        "• neighbor_radius"
        );

  c.def("scanline_orient_normals",
        [](PointRange& points, const py::kwargs& np = py::kwargs()) {
        // TODO: handle scan_angle_map and scanline_id_map
          CGAL::scanline_orient_normals(points);
          return points;
        },
        py::arg("points"), py::arg("np"),
        "orients the normals of the range of points by estimating a line of sight and checking its consistency with the current normal orientation.\n\n"
        "Warning\n"
        "This function requires the input points to be ordered along scanlines aligned on the XY-plane. It is typically designed for 2.5D urban datasets acquired through, for example, airborne LIDAR devices.\n\n"
        "First, scanlines are estimated as subranges of points by iterating on points:\n"
        "• if the named parameter scanline_id_map is provided, the range is cut every time the id changes.\n"
        "• if no scanline ID map is provided, a fallback method simply cuts the range every time 3 consecutive points form an acute angle on the projected XY-plane. This fallback method gives suboptimal results.\n\n"
        "Then, the line of sight (estimated vector between a point and the position of the scanner at its time of acquisition) is estimated:\n"
        "• if scan_angle is provided, the line of sight can be directly computed as a combination of the estimated scanline and of the scan angle.\n"
        "• if no scan angle map is provided, then for each scanline, the position of the scanner is estimated as being above of the barycenter of the points of the scanline projected on the XY-plane. This fallback method gives suboptimal results.\n\n"
        "Once the line of sight is estimated for each point, the normals are oriented by checking, for each of them, if the line of sight and the normal vector give a positive scalar product. If they don't, then the normal vector is inverted.\n\n"
        "Note\n"
        "This method gives optimal results when scanline_id_map and scan_angle are provided. Correct results may still be produced in the absence of either one or both of these properties, as long as the point set is ordered in 2.5D scanlines.\n\n"
        "Parameters\n"
        "• points: input point range.\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n\n"
        "Returns\n"
        "the modified point set.\n\n"
        "Examples\n"
        "• Point_set_processing_3/orient_scanlines_example.py."
        );


  c.def("vcm_estimate_normals",
        [](PointRange& points, double offset_radius, double convolution_radius,
           const py::kwargs& np = py::kwargs()) {
          CGAL::vcm_estimate_normals(points, offset_radius, convolution_radius);
          return points;
        },
        py::arg("points"), py::arg("offset_radius"), py::arg("convolution_radius"), py::arg("np"),
        "Estimates normal directions of the range of points using the Voronoi Covariance Measure with a radius for the convolution.\n"
        "The output normals are randomly oriented.\n"
        "See compute_vcm() for a detailed description of the parameters offset_radius and convolution_radius and of the Voronoi Covariance Measure.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• offset_radius: offset_radius.\n"
        "• convolution_radius: convolution_radius.\n\n"
        "Returns\n"
        "the modified point set.\n\n"
        );

  c.def("vcm_estimate_normals_neighbors",
        [](PointRange& points, double offset_radius, unsigned int k,
           const py::kwargs& np = py::kwargs()) {
          CGAL::vcm_estimate_normals(points, offset_radius, k);
          return points;
        },
        py::arg("points"), py::arg("offset_radius"), py::arg("k"), py::arg("np"),
        "Estimates normal directions of the range of points using the Voronoi Covariance Measure with a number of neighbors for the convolution.\n"
        "The output normals are randomly oriented.\n"
        "See compute_vcm() for a detailed description of the parameter offset_radius and of the Voronoi Covariance Measure.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• offset_radius: offset_radius.\n"
        "• k: number of neighbor points used for convolution.\n\n"
        "Returns\n"
        "the modified point set.\n\n"
        );

  c.def("wlop_simplify_and_regularize_point_set",
        [](PointRange& points,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          std::vector<Kernel::Point_3> output;
          auto cb_class = dummy_callback(callback);
          double sp = np.contains("select_percentage") ? py::cast<double>(np["select_percentage"]) : 5;
          unsigned int noi = np.contains("number_of_iterations") ? py::cast<unsigned int>(np["number_of_iterations"]) : 35;
          bool rus = np.contains("require_uniform_sampling") ? py::cast<bool>(np["require_uniform_sampling"]) : false;
          if (np.contains("neighbor_radius")) {
            auto nr = py::cast<double>(np["neighbor_radius"]);
            CGAL::wlop_simplify_and_regularize_point_set<Tag>(points, std::back_inserter(output),
                                                              CGAL::parameters::select_percentage(sp)
                                                              .number_of_iterations(noi)
                                                              .require_uniform_sampling(rus)
                                                              .neighbor_radius(nr)
                                                              // .callback(cb_class)
                                                              .geom_traits(K())
                                                              );
          }
          else {
            CGAL::wlop_simplify_and_regularize_point_set<Tag>(points, std::back_inserter(output),
                                                              CGAL::parameters::select_percentage(sp)
                                                              .number_of_iterations(noi)
                                                              .require_uniform_sampling(rus)
                                                              // .callback(cb_class)
                                                              .geom_traits(K())
                                                              );
          }
          return std::make_pair(output, points);
        },
        py::arg("points"), py::arg("callback") = std::function<bool(double)>(),
        py::arg("np"),
        "This is an implementation of the Weighted Locally Optimal Projection (WLOP) simplification algorithm.\n"
        "The WLOP simplification algorithm can produce a set of denoised, outlier-free and evenly distributed particles over the original dense point cloud. The core of the algorithm is a Weighted Locally Optimal Projection operator with a density uniformization term. For more details, please refer to [4].\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
        "• select_percentage\n: percentage of points to retain (default: 5)\n"
        "• neighbor_radius\n: the spherical neighborhood radius (default: 8 times the average spacing of the point set)\n"
        "• number_of_iterations: number of iterations to solve the optimsation problem (default: 35)\n"
        "• require_uniform_sampling: If true, an optional preprocessing is applied, which will give better results if the distribution of the input points is highly non-uniform. (default: false)\n"
        "Returns\n"
        "a tuple of the modified point set and the output points.\n\n"
        );
#endif
}

//
template <typename PointRange, typename PointMap, typename NormalMap,
          typename Point, typename Vector, typename C>
void export_functions_with_normals(C& c) {
  using K = Kernel;
  using Tag = CGAL::Sequential_tag;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) &&           \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("bilateral_smooth_point_set",
        [](PointRange& points, unsigned int k, const py::kwargs& np = py::kwargs()) {
          double r = CGAL::bilateral_smooth_point_set<Tag>(points, k);
          return std::make_pair(r, points);
        },
        py::arg("points"), py::arg("k"), py::arg("np"),
        "This function smooths an input point set by iteratively projecting each point onto the implicit surface patch fitted over its nearest neighbors. \n"
        "Bilateral projection preserves sharp features according to the normal (gradient) information. Both point positions and normals will be modified. For more details, please see section 4 in [5].\n\n"
        "Precondition\n"
        "• Normals must be unit vectors \n"
        "• k >= 2"
        "Parameters\n"
        "• points: input point range\n"
        "• k: size of the neighborhood for the implicit surface patch fitting. The larger the value is, the smoother the result will be.\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "• neighbor_radius\n"
        "• sharpness_angle\n"
        "Returns\n"
        "a tuple of the average point movement error. It's a convergence criterium for the algorithm. This value can help the user to decide how many iterations are sufficient, and the resulting points"
        );
#endif

  c.def("jet_estimate_normals",
        [](PointRange& points, unsigned int k, const py::kwargs& np = py::kwargs()) {
          CGAL::jet_estimate_normals<Tag>(points, k);
          return points;
        },
        py::arg("points"), py::arg("k"), py::arg("np"),
        "Estimates normal directions of the range of points using jet fitting on the nearest neighbors.\n"
        "The output normals are randomly oriented.\n\n"
        "Precondition\n"
        "â�¢ k >= 2\n\n"
        "Parameters\n"
        "â�¢ points: input point range\n"
        "â�¢ k: number of neighbors\n"
        "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
        "â�¢ neighbor_radius\n"
        "â�¢ degree_fitting\n"
        "Returns\n"
        "â�¢ the modified point set.\n\n"
        "Examples\n"
        "â�¢ Point_set_processing_3/edges_example.py."
        );

  c.def("compute_average_spacing_with_normals",
        [](const PointRange& points, const unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          return CGAL::compute_average_spacing<Tag>(points, k);
        },
        py::arg("points"), py::arg("k"),
        py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Computes average spacing from k nearest neighbors.\n"
        "Precondition\n"
        "• k >= 2.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• k: number of neighbors.\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Returns\n"
        "average spacing (scalar). The return type FT is a number type."
        );

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) &&           \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("edge_aware_upsample_point_set",
        [](const PointRange& points, const py::kwargs& np = py::kwargs()) {
          std::vector<std::pair<Point, Vector>> output;
          CGAL::edge_aware_upsample_point_set<Tag>(points, std::back_inserter(output));
          return output;
        },
        py::arg("points"), py::arg("np"),
        "This method progressively upsamples the point set while approaching the edge singularities (detected by normal variation), which generates a denser point set from an input point set.\n"
        "This has applications in point-based rendering, hole filling, and sparse surface reconstruction. Normals of points are required as input. For more details, please refer to [5].\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "• sharpness_angle\n"
        "• edge_sensitivity\n"
        "• number_of_output_points\n"
        "• neighbor_radius"
        );

  c.def("scanline_orient_normals",
        [](PointRange& points, const py::kwargs& np = py::kwargs()) {
          // TODO: handle scan_angle_map and scanline_id_map
          CGAL::scanline_orient_normals(points);
          return points;
        },
        py::arg("points"), py::arg("np"),
        "orients the normals of the range of points by estimating a line of sight and checking its consistency with the current normal orientation.\n\n"
        "Warning\n"
        "This function requires the input points to be ordered along scanlines aligned on the XY-plane. It is typically designed for 2.5D urban datasets acquired through, for example, airborne LIDAR devices.\n\n"
        "First, scanlines are estimated as subranges of points by iterating on points:\n"
        "• if the named parameter scanline_id_map is provided, the range is cut every time the id changes.\n"
        "• if no scanline ID map is provided, a fallback method simply cuts the range every time 3 consecutive points form an acute angle on the projected XY-plane. This fallback method gives suboptimal results.\n\n"
        "Then, the line of sight (estimated vector between a point and the position of the scanner at its time of acquisition) is estimated:\n"
        "• if scan_angle is provided, the line of sight can be directly computed as a combination of the estimated scanline and of the scan angle.\n"
        "• if no scan angle map is provided, then for each scanline, the position of the scanner is estimated as being above of the barycenter of the points of the scanline projected on the XY-plane. This fallback method gives suboptimal results.\n\n"
        "Once the line of sight is estimated for each point, the normals are oriented by checking, for each of them, if the line of sight and the normal vector give a positive scalar product. If they don't, then the normal vector is inverted.\n\n"
        "Note\n"
        "This method gives optimal results when scanline_id_map and scan_angle are provided. Correct results may still be produced in the absence of either one or both of these properties, as long as the point set is ordered in 2.5D scanlines.\n\n"
      "Parameters\n"
      "• points: input point range.\n"
      "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
      "Optional Named Parameters\n\n"
      "Returns\n"
      "the modified point set.\n\n"
      "Examples\n"
        "• Point_set_processing_3/orient_scanlines_example.py."
        );
#endif

  c.def("pca_estimate_normals",
        [](PointRange& points, unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          double nr = np.contains("neighbor_radius") ? py::cast<double>(np["neighbor_radius"]) : 0;
          CGAL::pca_estimate_normals<Tag>(points, k);
          return points;
        },
        py::arg("points"), py::arg("k"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Estimates normal directions of the range of points by linear least squares fitting of a plane over the nearest neighbors.\n"
        "The output normals are randomly oriented.\n\n"
        "Precondition\n"
        "â�¢ k >= 2\n\n"
        "Parameters\n"
        "â�¢ points: input point range\n"
        "â�¢ k: number of neighbors\n"
        "â�¢ callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
      "â�¢ neighbor_radius:  the spherical neighborhood radius (default: 0 (no limit))\n"
        "Returns\n"
        "â�¢ the modified point set."
        );

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("mst_orient_normals",
        [](PointRange& points, unsigned int k, const py::kwargs& np = py::kwargs()) {
          if (np.contains("point_is_constrained_map")) {
            // TODO: handle constrained points
            auto it = CGAL::mst_orient_normals(points, k);
            return std::make_pair(points, std::distance(points.begin(), it));
          }
          else {
            auto it = CGAL::mst_orient_normals(points, k);
            return std::make_pair(points, std::distance(points.begin(), it));
          }
        },
        py::arg("points"), py::arg("k"), py::arg("np"),
        "Orients the normals of the range of points using the propagation of a seed orientation through a minimum spanning tree of the Riemannian graph.\n"
        "This method modifies the order of input points so as to pack all successfully oriented points first, and returns an index over the first point with an unoriented normal (see erase-remove idiom). For this reason it should not be called on sorted containers. It is based on [3].\n\n"
        "Precondition\n"
        "â�¢ Normals must be unit vectors \n"
        "â�¢ k >= 2\n\n"
        "Parameters\n"
        "â�¢ points: input point range\n"
        "â�¢ k: number of neighbors.\n"
        "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
        "â�¢ neighbor_radius:  the spherical neighborhood radius (default: 0 (no limit))\n"
        "Returns\n"
        "â�¢ a tuple of the modified point set and the first index of a point with an unoriented normal.\n\n"
        "Examples\n"
        "â�¢ Point_set_processing_3/normals_example.py."
        );

  c.def("vcm_estimate_normals",
        [](PointRange& points, double offset_radius, double convolution_radius, const py::kwargs& np = py::kwargs()) {
          CGAL::vcm_estimate_normals(points, offset_radius, convolution_radius);
          return points;
        },
        py::arg("points"), py::arg("offset_radius"), py::arg("convolution_radius"), py::arg("np"),
        "Estimates normal directions of the range of points using the Voronoi Covariance Measure with a radius for the convolution.\n"
        "The output normals are randomly oriented.\n"
        "See compute_vcm() for a detailed description of the parameters offset_radius and convolution_radius and of the Voronoi Covariance Measure.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• offset_radius: offset_radius.\n"
        "• convolution_radius: convolution_radius.\n\n"
        "Returns\n"
        "the modified point set.\n\n"
        );

  c.def("vcm_estimate_normals_neighbors",
        [](PointRange& points, double offset_radius, unsigned int k, const py::kwargs& np = py::kwargs()) {
          CGAL::vcm_estimate_normals(points, offset_radius, k);
          return points;
        },
        py::arg("points"), py::arg("offset_radius"), py::arg("k"), py::arg("np"),
        "Estimates normal directions of the range of points using the Voronoi Covariance Measure with a number of neighbors for the convolution.\n"
        "The output normals are randomly oriented.\n"
        "See compute_vcm() for a detailed description of the parameter offset_radius and of the Voronoi Covariance Measure.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• offset_radius: offset_radius.\n"
        "• k: number of neighbor points used for convolution.\n\n"
        "Returns\n"
        "the modified point set.\n\n"
        );

  c.def("wlop_simplify_and_regularize_point_set_with_normals",
        [](PointRange& points,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          std::vector<Kernel::Point_3> output;
          auto cb_class = dummy_callback(callback);
          double sp = np.contains("select_percentage") ? py::cast<double>(np["select_percentage"]) : 5;
          unsigned int noi = np.contains("number_of_iterations") ? py::cast<unsigned int>(np["number_of_iterations"]) : 35;
          bool rus = np.contains("require_uniform_sampling") ? py::cast<bool>(np["require_uniform_sampling"]) : false;
          if (np.contains("neighbor_radius")) {
            auto nr = py::cast<double>(np["neighbor_radius"]);
            CGAL::wlop_simplify_and_regularize_point_set<Tag>(points, std::back_inserter(output),
                                                              CGAL::parameters::select_percentage(sp)
                                                              .point_map(PointMap())
                                                              .normal_map(NormalMap())
                                                              .number_of_iterations(noi)
                                                              .require_uniform_sampling(rus)
                                                              .neighbor_radius(nr)
                                                              // .callback(cb_class)
                                                              .geom_traits(K()));
          }
          else {
            CGAL::wlop_simplify_and_regularize_point_set<Tag>(points, std::back_inserter(output),
                                                              CGAL::parameters::select_percentage(sp)
                                                              .point_map(PointMap())
                                                              .normal_map(NormalMap())
                                                              .number_of_iterations(noi)
                                                              .require_uniform_sampling(rus)
                                                              // .callback(cb_class)
                                                              .geom_traits(K()));
          }
          return std::make_pair(points, output);
        },
        py::arg("points"), py::arg("callback") = std::function<bool(double)>(),
        py::arg("np"),
        "This is an implementation of the Weighted Locally Optimal Projection (WLOP) simplification algorithm.\n"
        "The WLOP simplification algorithm can produce a set of denoised, outlier-free and evenly distributed particles over the original dense point cloud. The core of the algorithm is a Weighted Locally Optimal Projection operator with a density uniformization term. For more details, please refer to [4].\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
        "• select_percentage\n: percentage of points to retain (default: 5)\n"
        "• neighbor_radius\n: the spherical neighborhood radius (default: 8 times the average spacing of the point set)\n"
        "• number_of_iterations: number of iterations to solve the optimsation problem (default: 35)\n"
        "• require_uniform_sampling: If true, an optional preprocessing is applied, which will give better results if the distribution of the input points is highly non-uniform. (default: false)\n"
        "Returns\n"
        "a tuple of the modified point set and the output points.\n\n"
        );
#endif
}

//
template <typename PointRange, typename Point, typename Vector, typename C>
void export_multiple_dimension_functions(C& c) {
  using K = Kernel;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("estimate_global_k_neighbor_scale",
        [](const PointRange& points, const py::kwargs& np = py::kwargs()) {
          return CGAL::estimate_global_k_neighbor_scale(points);
        },
        py::arg("points"), py::arg("np"),
        "Estimates the global scale in a K nearest neighbors sense.\n"
        "The computed scale corresponds to the smallest scale such that the K subsets of points have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).\n"
        "Parameters\n"
        "• points:	input point range\n"
        "• np:	an optional sequence of Named Parameters among the ones listed below\n"
        "Note\n"
        "• This function accepts both 2D and 3D points.\n\n"
        "Returns\n"
        "• The estimated scale in the K nearest neighbors sense.\n\n"
        "Examples\n"
        "• Point_set_processing_3/scale_estimation.py."
        );

  c.def("estimate_global_range_scale",
        [](const PointRange& points, const py::kwargs& np = py::kwargs()) {
          return CGAL::estimate_global_range_scale(points);
        },
        py::arg("points"), py::arg("np"),
        "Estimates the global scale in a range sense.\n"
        "The computed scale corresponds to the smallest scale such that the subsets of points inside the sphere range have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).\n\n"
        "Parameters\n"
        "• points:	input point range\n"
        "• np:	an optional sequence of Named Parameters among the ones listed below\n\n"
        "Note\n"
        "• This function accepts both 2D and 3D points.\n\n"
        "Returns\n"
        "• The estimated scale in the range sense. The return type FT is a number type. It is either deduced from the geom_traits Named Parameters if provided, or the geometric traits class deduced from the point property map of points.\n\n"
        "Examples\n"
        "• Point_set_processing_3/scale_estimation.py."
        );

#endif
}

//
template <typename PointRange, typename Point, typename Vector,
          typename PointMap, typename C>
void export_functions_with_point_range_normals(C& c) {
  using K = Kernel;
  using Tag = CGAL::Sequential_tag;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("compute_vcm_with_normals",
        [](const PointRange& points, double offset_radius, double convolution_radius,
           const py::kwargs& np = py::kwargs()) {
          std::vector<std::array<double, 6>> ccov;
          CGAL::compute_vcm(points, ccov, offset_radius, convolution_radius);
          return ccov;
        },
        py::arg("points"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("np"),
        "Computes the Voronoi Covariance Measure (VCM) of a point cloud, a construction that can be used for normal estimation and sharp feature detection.\n"
        "The VCM associates to each point the covariance matrix of its Voronoi cell intersected with the ball of radius offset_radius. In addition, if the second radius convolution_radius is positive, the covariance matrices are smoothed via a convolution process. More specifically, each covariance matrix is replaced by the average of the matrices of the points located at a distance at most convolution_radius. The choice for parameter offset_radius should refer to the geometry of the underlying surface while the choice for parameter convolution_radius should refer to the noise level in the point cloud. For example, if the point cloud is a uniform and noise-free sampling of a smooth surface, offset_radius should be set to the minimum local feature size of the surface, while convolution_radius can be set to zero.\n"
        "The Voronoi covariance matrix of each vertex is stored in an array a of length 6 and is as follow:\n"
        "â�¡â�£â�¢a[0]a[1]a[2]a[1]a[3]a[4]a[2]a[4]a[5]â�¤â�¦â�¥\n\n"
        "Parameters\n"
        "â�¢ points: input point range\n"
        "â�¢ offset_radius: offset_radius.\n"
        "â�¢ convolution_radius: convolution_radius.\n\n"
        "Returns\n"
        "â�¢ average spacing (scalar). The return type FT is a number type.\n\n"
        "Examples\n"
        "â�¢ Point_set_processing_3/scale_estimation_example.py."
        );

  c.def("grid_simplify_point_set_with_normals",
        [](PointRange& points, double epsilon,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          unsigned int min_points_per_cell = np.contains("min_points_per_cell") ? py::cast<unsigned int>(np["min_points_per_cell"]) : 1;
          auto el = CGAL::grid_simplify_point_set(points, epsilon, CGAL::parameters::min_points_per_cell(min_points_per_cell)
                                                  .point_map(PointMap())
                                                  .callback(cb_class));
          return std::make_pair(points, std::distance(points.begin(), el));
        },
        py::arg("points"), py::arg("epsilon"),
        py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "merges points which belong to the same cell of a grid of cell size = epsilon.\n"
        "This method modifies the order of input points so as to pack all remaining points first, and returns the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
        "Precondition\n"
        "â�¢ epsilon > 0\n\n"
        "Parameters\n"
        "â�¢ points: input point range\n"
        "â�¢ epsilon: tolerance value when merging 3D points.\n"
        "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "â�¢ min_points_per_cell:  minimum number of points in a cell such that a point in this cell is kept after simplification (default: 1)\n\n"
        "Returns\n"
        "â�¢ a tuple of the modified point set and the index of the first point to remove.\n\n"
        "Examples\n"
        "â�¢ Point_set_processing_3/callback_example.py, Point_set_processing_3/grid_simplification_example.py, Point_set_processing_3/grid_simplify_indices.py, and Point_set_processing_3/scale_estimation_example.py."
        );

  c.def("hierarchy_simplify_point_set_with_normals",
        [](PointRange& points,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          unsigned int size = np.contains("size") ? py::cast<unsigned int>(np["size"]) : 10;
          double var_max = np.contains("maximum_variation") ? py::cast<double>(np["maximum_variation"]) : 1/3.0f;
          auto el = CGAL::hierarchy_simplify_point_set(points, CGAL::parameters::size(size)
                                                       .maximum_variation(var_max)
                                                       .point_map(PointMap())
                                                       // .callback(cb_class)
                                                       );
          return std::make_pair(points, std::distance(points.begin(), el));
        },
        py::arg("points"),
        py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Recursively split the point set in smaller clusters until the clusters have fewer than size elements and until their variation factor is below var_max.\n"
        "This method modifies the order of input points so as to pack all remaining points first, and returns the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
        "Precondition\n"
        "â�¢ 0 < maximum_variation <= 1/3 \n"
        "â�¢ size > 0\n\n"
        "Parameters\n"
        "â�¢ points: input point range\n"
        "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "â�¢ size\n"
        "â�¢ maximum_variation\n"
        "Returns\n"
        "â�¢ a tuple of the modified point set and the index first point to remove.\n\n"
        "Examples\n"
        "â�¢ Point_set_processing_3/hierarchy_simplification_example.py."
        );
#endif

  c.def("jet_smooth_point_set_with_normals",
        [](PointRange& points, unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          double nr = np.contains("neighbor_radius") ? py::cast<double>(np["neighbor_radius"]) : 0;
          double df = np.contains("degree_fitting") ? py::cast<double>(np["degree_fitting"]) : 2;
          double dm = np.contains("degree_monge") ? py::cast<double>(np["degree_monge"]) : 2;
          CGAL::jet_smooth_point_set<Tag>(points, k, CGAL::parameters::neighbor_radius(nr)
                                          .degree_fitting(df)
                                          .degree_monge(dm)
                                          .callback(cb_class)
                                          .point_map(PointMap())
                                          );
          return points;
        },
        py::arg("points"), py::arg("k"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
      "Smoothes the range of points using jet fitting on the nearest neighbors and reprojection onto the jet.\n"
      "As this method relocates the points, it should not be called on containers sorted w.r.t. point locations.\n\n"
      "Precondition\n"
      "â�¢ k >= 2\n\n"
      "Parameters\n"
      "â�¢ points: input point range\n"
      "â�¢ k: number of neighbors\n"
      "â�¢ callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
      "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n\n"
      "Optional Named Parameters\n"
      "â�¢ neighbor_radius:  the spherical neighborhood radius (default: 0 (no limit))\n"
      "â�¢ degree_fitting:  the degree of fitting (default: 2)\n"
      "â�¢ degree_monge:  the Monge degree (default: 2)\n"
      "Returns\n"
      "â�¢ the modified point set.\n\n"
      "Examples\n"
      "â�¢ Point_set_processing_3/edges_example.py."
      );

  c.def("remove_outliers",
        [](PointRange& points, unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          double nr = np.contains("neighbor_radius") ? py::cast<double>(np["neighbor_radius"]) : 0;
          double tp = np.contains("threshold_percent") ? py::cast<double>(np["threshold_percent"]) : 10;
          double td = np.contains("threshold_distance") ? py::cast<double>(np["threshold_distance"]) : 0;
          auto it = CGAL::remove_outliers<Tag>(points, k, CGAL::parameters::neighbor_radius(nr)
                                               .threshold_percent(tp)
                                               .threshold_distance(td)
                                               .point_map(PointMap())
                                               // .callback(cb_class)
                                               );
          return std::make_pair(points, std::distance(points.begin(), it));
        },
        py::arg("points"), py::arg("k"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
      "Removes outliers:\n"
      "â�¢ computes average squared distance to the nearest neighbors,\n"
      "â�¢ and partitions the points either using a threshold on the of average distance or selecting a fixed percentage of points with the highest average distances\n"
      "This method modifies the order of input points so as to pack all remaining points first, and returns an index over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
      "Precondition\n"
      "â�¢ k >= 2\n\n"
      "Parameters\n"
      "â�¢ points: input point range\n"
      "â�¢ k: number of neighbors\n"
      "â�¢ callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
      "â�¢ np: an optional sequence of Named Parameters among the ones listed below\n\n"
      "Optional Named Parameters\n"
      "â�¢ threshold_percent\n"
      "â�¢ threshold_distance\n"
      "Returns\n"
      "a tuple of the modified point set and the index of the first point to remove.\n\n"
      "Note\n"
      "There are two thresholds that can be used: threshold_percent and threshold_distance. This function returns the smallest number of outliers such that at least one of these threshold is fulfilled. This means that if threshold_percent=100, only threshold_distance is taken into account; if threshold_distance=0 only threshold_percent is taken into account. \n"
      );
}

//
template <typename PointRange, typename Point, typename Vector, typename C>
void export_functions_with_point_range(C& c) {
  using K = Kernel;
  using Tag = CGAL::Sequential_tag;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("compute_vcm",
        [](const PointRange& points, double offset_radius, double convolution_radius,
           const py::kwargs& np = py::kwargs()) {
          std::vector<std::array<double, 6>> ccov;
          CGAL::compute_vcm(points, ccov, offset_radius, convolution_radius);
          return ccov;
        },
        py::arg("points"), py::arg("offset_radius"), py::arg("convolution_radius"), py::arg("np"),
        "Computes the Voronoi Covariance Measure (VCM) of a point cloud, a construction that can be used for normal estimation and sharp feature detection.\n"
        "The VCM associates to each point the covariance matrix of its Voronoi cell intersected with the ball of radius offset_radius. In addition, if the second radius convolution_radius is positive, the covariance matrices are smoothed via a convolution process. More specifically, each covariance matrix is replaced by the average of the matrices of the points located at a distance at most convolution_radius. The choice for parameter offset_radius should refer to the geometry of the underlying surface while the choice for parameter convolution_radius should refer to the noise level in the point cloud. For example, if the point cloud is a uniform and noise-free sampling of a smooth surface, offset_radius should be set to the minimum local feature size of the surface, while convolution_radius can be set to zero.\n"
        "The Voronoi covariance matrix of each vertex is stored in an array a of length 6 and is as follow:\n"
        "⎡⎣⎢a[0]a[1]a[2]a[1]a[3]a[4]a[2]a[4]a[5]⎤⎦⎥\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• offset_radius: offset_radius.\n"
        "• convolution_radius: convolution_radius.\n\n"
        "Returns\n"
        "• average spacing (scalar). The return type FT is a number type.\n\n"
        "Examples\n"
        "• Point_set_processing_3/scale_estimation_example.py."
        );

  c.def("grid_simplify_point_set",
        [](PointRange& points, double epsilon,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          unsigned int min_points_per_cell = np.contains("min_points_per_cell") ? py::cast<unsigned int>(np["min_points_per_cell"]) : 1;
          auto el = CGAL::grid_simplify_point_set(points, epsilon,
                                                  CGAL::parameters::min_points_per_cell(min_points_per_cell)
                                                  .callback(cb_class));
          return std::make_pair(points, std::distance(points.begin(), el));
        },
        py::arg("points"), py::arg("epsilon"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "merges points which belong to the same cell of a grid of cell size = epsilon.\n"
        "This method modifies the order of input points so as to pack all remaining points first, and returns the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
        "Precondition\n"
        "• epsilon > 0\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• epsilon: tolerance value when merging 3D points.\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "• min_points_per_cell:  minimum number of points in a cell such that a point in this cell is kept after simplification (default: 1)\n\n"
        "Returns\n"
        "• a tuple of the modified point set and the index of the first point to remove.\n\n"
        "Examples\n"
        "• Point_set_processing_3/callback_example.py, Point_set_processing_3/grid_simplification_example.py, Point_set_processing_3/grid_simplify_indices.py, and Point_set_processing_3/scale_estimation_example.py."
        );

  c.def("hierarchy_simplify_point_set",
        [](PointRange& points,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          unsigned int size = np.contains("size") ? py::cast<unsigned int>(np["size"]) : 10;
          double var_max = np.contains("maximum_variation") ? py::cast<double>(np["maximum_variation"]) : 1/3.0f;
          auto el = CGAL::hierarchy_simplify_point_set(points,
                                                       CGAL::parameters::size(size)
                                                       .maximum_variation(var_max)
                                                       // .callback(cb_class) // this doesnt work
                                                       );
          return std::make_pair(points, std::distance(points.begin(), el));
        },
        py::arg("points"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Recursively split the point set in smaller clusters until the clusters have fewer than size elements and until their variation factor is below var_max.\n"
        "This method modifies the order of input points so as to pack all remaining points first, and returns the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
        "Precondition\n"
        "• 0 < maximum_variation <= 1/3 \n"
        "• size > 0\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Optional Named Parameters\n"
        "• size\n"
        "• maximum_variation\n"
        "Returns\n"
        "• a tuple of the modified point set and the index of the first point to remove.\n\n"
        "Examples\n"
        "• Point_set_processing_3/hierarchy_simplification_example.py."
        );
#endif

  c.def("jet_smooth_point_set",
        [](PointRange& points, unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          double nr = np.contains("neighbor_radius") ? py::cast<double>(np["neighbor_radius"]) : 0;
          double df = np.contains("degree_fitting") ? py::cast<double>(np["degree_fitting"]) : 2;
          double dm = np.contains("degree_monge") ? py::cast<double>(np["degree_monge"]) : 2;
          CGAL::jet_smooth_point_set<Tag>(points, k, CGAL::parameters::neighbor_radius(nr)
                                          .degree_fitting(df)
                                          .degree_monge(dm)
                                          // .callback(cb_class)
                                          );
          return points;
        },
        py::arg("points"), py::arg("k"),
        py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Smoothes the range of points using jet fitting on the nearest neighbors and reprojection onto the jet.\n"
        "As this method relocates the points, it should not be called on containers sorted w.r.t. point locations.\n\n"
        "Precondition\n"
        "• k >= 2\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• k: number of neighbors\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
        "• neighbor_radius:  the spherical neighborhood radius (default: 0 (no limit))\n"
        "• degree_fitting:  the degree of fitting (default: 2)\n"
        "• degree_monge:  the Monge degree (default: 2)\n"
        "Returns\n"
        "• the modified point set.\n\n"
        "Examples\n"
        "• Point_set_processing_3/edges_example.py."
        );

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("mst_orient_normals",
        [](PointRange& points, unsigned int k,
           const py::kwargs& np = py::kwargs()) {
          if (np.contains("point_is_constrained_map")) {
            // TODO: handle constrained points
            auto it = CGAL::mst_orient_normals(points, k);
            return std::make_pair(points, std::distance(points.begin(), it));
          }
          else {
            auto it = CGAL::mst_orient_normals(points, k);
            return std::make_pair(points, std::distance(points.begin(), it));
          }
        },
        py::arg("points"), py::arg("k"), py::arg("np"),
        "Orients the normals of the range of points using the propagation of a seed orientation through a minimum spanning tree of the Riemannian graph.\n"
        "This method modifies the order of input points so as to pack all successfully oriented points first, and returns an index over the first point with an unoriented normal (see erase-remove idiom). For this reason it should not be called on sorted containers. It is based on [3].\n\n"
        "Precondition\n"
        "• Normals must be unit vectors \n"
        "• k >= 2\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• k: number of neighbors.\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Optional Named Parameters\n"
        "• neighbor_radius:  the spherical neighborhood radius (default: 0 (no limit))\n"
        "Returns\n"
        "• a tuple of the modified point set and the index of the first point with an unoriented normal.\n\n"
        "Examples\n"
        "• Point_set_processing_3/normals_example.py."
        );
#endif

  c.def("random_simplify_point_set",
        [](PointRange& points, double removed_percentage) {
          auto it = CGAL::random_simplify_point_set(points, removed_percentage);
          return std::make_pair(points, std::distance(points.begin(), it));
        },
        py::arg("points"), py::arg("removed_percentage"),
        "Randomly deletes a user-specified fraction of the input points.\n"
        "This method modifies the order of input points so as to pack all remaining points first, and returns an index over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• removed_percentage: percentage of points to remove\n"
        "Returns\n"
        "• a tuple of the modified point set and the index of the first point to remove.\n\n"
        "Examples\n"
        "• Point_set_processing_3/random_simplification_example.py."
        );

  c.def("remove_outliers",
        [](PointRange& points, unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          double nr = np.contains("neighbor_radius") ? py::cast<double>(np["neighbor_radius"]) : 0;
          double tp = np.contains("threshold_percent") ? py::cast<double>(np["threshold_percent"]) : 10;
          double td = np.contains("threshold_distance") ? py::cast<double>(np["threshold_distance"]) : 0;
          auto it = CGAL::remove_outliers<Tag>(points, k, CGAL::parameters::neighbor_radius(nr)
                                               .threshold_percent(tp)
                                               .threshold_distance(td)
                                               // .callback(cb_class)
                                               );
          return std::make_pair(points, std::distance(points.begin(), it));
        },
        py::arg("points"), py::arg("k"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
      "Removes outliers:\n"
      "• computes average squared distance to the nearest neighbors,\n"
      "• and partitions the points either using a threshold on the of average distance or selecting a fixed percentage of points with the highest average distances\n"
      "This method modifies the order of input points so as to pack all remaining points first, and returns an index over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.\n\n"
      "Precondition\n"
      "• k >= 2\n\n"
      "Parameters\n"
      "• points: input point range\n"
      "• k: number of neighbors\n"
      "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
      "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
      "Optional Named Parameters\n"
      "• threshold_percent\n"
      "• threshold_distance\n"
      "Returns\n"
      "a tuple of the modified point set and the index of the first point to remove.\n\n"
      "Note\n"
      "There are two thresholds that can be used: threshold_percent and threshold_distance. This function returns the smallest number of outliers such that at least one of these threshold is fulfilled. This means that if threshold_percent=100, only threshold_distance is taken into account; if threshold_distance=0 only threshold_percent is taken into account. \n"
      );

  // TODO: structure_point_set() needs RANSAC planes too
}

//
template <typename Points, typename C>
void export_functions_with_point_vec(C& c) {
  using K = Kernel;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  c.def("estimate_local_k_neighbor_scales",
        [](const Points& points, const Points& queries, const py::kwargs& np = py::kwargs()) {
          std::vector<std::size_t> output;
          return CGAL::estimate_local_k_neighbor_scales(points, queries, std::back_inserter(output));
        },
        py::arg("points"), py::arg("queries"), py::arg("np"),
        "Estimates the local scale in a K nearest neighbors sense on a set of user-defined query points.\n"
        "The computed scales correspond to the smallest scales such that the K subsets of points have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).\n\n"
        "Parameters\n\n"
        "• points: input point range\n"
        "• queries:	range of locations where scale must be estimated\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Note\n"
        "• This function accepts both 2D and 3D points, but sample points and query must have the same dimension.\n\n"
        "Returns\n"
        "• The estimated scales in the K nearest neighbors sense. The return type is a vector of FTs. It is either deduced from the geom_traits Named Parameters if provided, or the geometric traits class deduced from the point property map of points.\n\n"
        "Examples\n"
        "• Point_set_processing_3/scale_estimation_2d_example.py."
        );

  c.def("estimate_local_range_scales",
        [](const Points& points, const Points& queries, const py::kwargs& np = py::kwargs()) {
          std::vector<std::size_t> output;
          return CGAL::estimate_local_range_scales(points, queries, std::back_inserter(output));
        },
        py::arg("points"), py::arg("queries"), py::arg("np"),
        "Estimates the local scale in a range sense on a set of user-defined query points.\n"
        "The computed scales correspond to the smallest scales such that the subsets of points included in the sphere range have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• queries:	range of locations where scale must be estimated\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n\n"
        "Note\n"
        "• This function accepts both 2D and 3D points, but sample points and query must have the same dimension."
        );
#endif
}

#endif

//
void export_point_set_processing(py::module_& m) {
  using Tag = CGAL::Sequential_tag;
  using K = Kernel;
  using Point_2 = typename K::Point_2;
  using Point_3 = typename K::Point_3;
  using Vector_2 = typename K::Vector_2;
  using Vector_3 = typename K::Vector_3;
  using PointVector_2 = std::vector<Point_2>;
  using PointVector_3 = std::vector<Point_3>;
  using PointVectorPair_2 = std::pair<Point_2, Vector_2>;
  using PointVectorPair_3 = std::pair<Point_3, Vector_3>;
  using PointRange_2 = std::vector<std::pair<Point_2, Vector_2>>;
  using PointRange_3 = std::vector<std::pair<Point_3, Vector_3>>;
  using PointSet_2 = CGAL::Point_set_3<Point_2>;
  using PointSet_3 = CGAL::Point_set_3<Point_3>;
  using ClusterMap_2 = PointSet_2::Property_map<int>;
  using ClusterMap_3 = PointSet_3::Property_map<int>;

  using Fopopm = CGAL::First_of_pair_property_map<PointVectorPair_3>;
  using Sopopm = CGAL::Second_of_pair_property_map<PointVectorPair_3>;

#if 0
  export_functions_with_point_range<PointVector_3, Point_3, Vector_3>(m);
  export_functions_with_point_range<PointSet_3, Point_3, Vector_3>(m);
  export_functions_with_point_range_normals<PointRange_3, Point_3, Vector_3, Fopopm>(m);

  // export_multiple_dimension_functions<PointVector_2, Point_2, Vector_2>(m);
  // export_multiple_dimension_functions<PointSet_2, Point_2, Vector_2>(m);
  export_multiple_dimension_functions<PointVector_3, Point_3, Vector_3>(m);
  export_multiple_dimension_functions<PointSet_3, Point_3, Vector_3>(m);

  export_functions_with_point_vec<PointVector_3>(m);
  // export_functions_with_point_vec<PointVector_2>(m); // why does this not work?

  export_functions_with_normals<PointRange_3, Fopopm, Sopopm, Point_3, Vector_3>(m);
  export_functions_without_normals<PointVector_3, Point_3, Vector_3>(m);

  // Algorithms
  m.def("cluster_point_set",
        [](PointSet_3& points, ClusterMap_3& cluster_map,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          // auto cb_class = dummy_callback(callback);
          double af = np.contains("attraction_factor") ? py::cast<double>(np["attraction_factor"]) : 0;
          std::vector<std::pair<std::size_t, std::size_t>> adj;
          std::size_t res;
          if (np.contains("neighbor_radius")) {
            double nr = py::cast<double>(np["neighbor_radius"]);
            res = CGAL::cluster_point_set(points, cluster_map,
                                          CGAL::parameters::attraction_factor(af)
                                          // .callback(callback) // this crashes for some reason
                                          .adjacencies(std::back_inserter(adj))
                                          .neighbor_radius(nr));
          }
          else {
            res = CGAL::cluster_point_set(points, cluster_map,
                                          CGAL::parameters::attraction_factor(af)
                                          .adjacencies(std::back_inserter(adj))
                                          // .callback(callback)
                                          );
          }
          return std::make_pair(res, adj);
        },
        py::arg("points"), py::arg("cluster_map"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Identifies connected components on a nearest neighbor graph built using a query sphere of fixed radius centered on each point.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• cluster_map: maps each point to the index of the cluster it belongs to.\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: optional sequence of Named Parameters among the ones listed below.\n"
        "Optional Named Parameters\n"
        "• neighbor_radius\n"
        "• attraction_factor\n\n"
        "Returns\n"
        "A tuple of the number of clusters identified and adjacencies.\n\n"
        "Examples\n"
        "Point_set_processing_3/clustering_example.py."
        );

  m.def("compute_average_spacing", [](const PointSet_3& points, const unsigned int k,
                                      const std::function<bool(double)>& callback = std::function<bool(double)>(),
                                      const py::kwargs& np = py::kwargs())
        { /* auto cb_class = dummy_callback(callback); */
        return CGAL::compute_average_spacing<Tag>(points, k
                         //                          ,
                         // CGAL::parameters::callback(callback)
                          ); },
        py::arg("points"), py::arg("k"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Computes average spacing from k nearest neighbors.\n"
        "Precondition\n"
        "• k >= 2.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• k: number of neighbors.\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Returns\n"
        "average spacing (scalar). The return type FT is a number type."
        );

  m.def("compute_average_spacing",
        [](const PointVector_3& points, const unsigned int k,
           const std::function<bool(double)>& callback = std::function<bool(double)>(),
           const py::kwargs& np = py::kwargs()) {
          auto cb_class = dummy_callback(callback);
          return CGAL::compute_average_spacing<Tag>(points, k
                                                    // CGAL::parameters::callback(cb_class)
                                                    );
        },
        py::arg("points"), py::arg("k"), py::arg("callback") = std::function<bool(double)>(), py::arg("np"),
        "Computes average spacing from k nearest neighbors.\n"
        "Precondition\n"
        "• k >= 2.\n\n"
        "Parameters\n"
        "• points: input point range\n"
        "• k: number of neighbors.\n"
        "• callback: a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed\n"
        "• np: an optional sequence of Named Parameters among the ones listed below\n"
        "Returns\n"
        "average spacing (scalar). The return type FT is a number type."
        );

  // TODO: compute_registration_transformation() needs OpenGR
  // TODO: compute_registration_transformation() needs PointMatcher
  // TODO: register_point_sets() needs OpenGR
  // TODO: register_point_sets() needs PointMatcher
#endif

  m.def("read_points", &psp::read_points,
        py::arg("fname"), py::arg("params") = py::dict(),
        "reads a point set from an input file.\n"
        "Supported file formats are the following:\n"
        "• Object File Format (OFF) (.off)\n"
        "• Polygon File Format (PLY) (.ply)\n"
        "• LAS (Lidar) File Format (.las)\n"
        "• XYZ File Format (.xyz)\n"
    );

  m.def("compute_average_spacing", &psp::compute_average_spacing<Point_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Computes average spacing from k nearest neighbors for a point range.\n"
        "Precondition: k >= 2.");

  m.def("compute_average_spacing",
        &psp::compute_average_spacing_point_set<PointSet_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Computes average spacing from k nearest neighbors for a Point_set_3.\n"
        "Precondition: k >= 2.");

  m.def("compute_average_spacing", &psp::compute_average_spacing_np<Point_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Computes average spacing from k nearest neighbors for a NumPy-style "
        "float64 point array with shape (N, 3).\n"
        "The input array is copied into a CGAL point range.\n"
        "Precondition: k >= 2.");

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) &&                                \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) &&                      \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  m.def("estimate_global_k_neighbor_scale",
        &psp::estimate_global_k_neighbor_scale<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Estimates the global scale in a K nearest neighbors sense for a point range.");

  m.def("estimate_global_k_neighbor_scale",
        &psp::estimate_global_k_neighbor_scale_np<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Estimates the global scale in a K nearest neighbors sense for a NumPy-style "
        "float64 point array with shape (N, 3).");

  m.def("estimate_global_range_scale",
        &psp::estimate_global_range_scale<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Estimates the global scale in a range sense for a point range.");

  m.def("estimate_global_range_scale",
        &psp::estimate_global_range_scale_np<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Estimates the global scale in a range sense for a NumPy-style "
        "float64 point array with shape (N, 3).");

  m.def("estimate_local_k_neighbor_scales",
        &psp::estimate_local_k_neighbor_scales<Point_3>,
        py::arg("points"), py::arg("queries"), py::arg("params") = py::dict(),
        "Estimates local scales in a K nearest neighbors sense for query points.");

  m.def("estimate_local_k_neighbor_scales",
        &psp::estimate_local_k_neighbor_scales_np<Point_3>,
        py::arg("points"), py::arg("queries"), py::arg("params") = py::dict(),
        "Estimates local scales in a K nearest neighbors sense for NumPy-style "
        "float64 point arrays with shape (N, 3).");

  m.def("estimate_local_range_scales",
        &psp::estimate_local_range_scales<Point_3>,
        py::arg("points"), py::arg("queries"), py::arg("params") = py::dict(),
        "Estimates raw local range-scale values for query points. "
        "These are the values used by CGAL before estimate_global_range_scale "
        "returns sqrt(median(values)).");

  m.def("estimate_local_range_scales",
        &psp::estimate_local_range_scales_np<Point_3>,
        py::arg("points"), py::arg("queries"), py::arg("params") = py::dict(),
        "Estimates raw local range-scale values for NumPy-style float64 point arrays "
        "with shape (N, 3). These are the values used by CGAL before "
        "estimate_global_range_scale returns sqrt(median(values)).");

  m.def("grid_simplify_point_set",
        &psp::grid_simplify_point_set<Point_3>,
        py::arg("points"), py::arg("epsilon"), py::arg("params") = py::dict(),
        "Simplifies a point set by keeping one representative point per grid cell. "
        "Returns (points, first_index_to_remove).");

  m.def("grid_simplify_point_set",
        &psp::grid_simplify_point_set_np<Point_3>,
        py::arg("points"), py::arg("epsilon"), py::arg("params") = py::dict(),
        "Simplifies a NumPy-style float64 point array with shape (N, 3) by grid clustering. "
        "Returns (points, first_index_to_remove).");

  m.def("random_simplify_point_set",
        &psp::random_simplify_point_set<Point_3>,
        py::arg("points"), py::arg("removed_percentage"),
        py::arg("params") = py::dict(),
        "Randomly removes a percentage of points. "
        "Returns (points, first_index_to_remove).");

  m.def("random_simplify_point_set",
        &psp::random_simplify_point_set_np<Point_3>,
        py::arg("points"), py::arg("removed_percentage"),
        py::arg("params") = py::dict(),
        "Randomly removes a percentage of points from a NumPy-style float64 point array "
        "with shape (N, 3). Returns (points, first_index_to_remove).");

  m.def("hierarchy_simplify_point_set",
        &psp::hierarchy_simplify_point_set<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Simplifies a point set using hierarchy simplification. "
        "Returns (points, first_index_to_remove).");

  m.def("hierarchy_simplify_point_set",
        &psp::hierarchy_simplify_point_set_np<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Simplifies a NumPy-style float64 point array with shape (N, 3) using hierarchy "
        "simplification. Returns (points, first_index_to_remove).");

  m.def("jet_smooth_point_set",
        &psp::jet_smooth_point_set<Point_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Smooths a point set using jet fitting.");

  m.def("jet_smooth_point_set",
        &psp::jet_smooth_point_set_np<Point_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Smooths a NumPy-style float64 point array with shape (N, 3) using jet fitting.");

  m.def("remove_outliers",
        &psp::remove_outliers<Point_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Removes outliers from a point set. Returns (points, first_index_to_remove).");

  m.def("remove_outliers",
        &psp::remove_outliers_np<Point_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Removes outliers from a NumPy-style float64 point array with shape (N, 3). "
        "Returns (points, first_index_to_remove).");


  m.def("read_points_with_normals",
        &psp::read_points_with_normals<Point_3, Vector_3>,
        py::arg("fname"), py::arg("params") = py::dict(),
        "Reads points with normals from a point-set file.");

  m.def("cluster_point_set",
        &psp::cluster_point_set<PointSet_3, ClusterMap_3>,
        py::arg("points"), py::arg("cluster_map"), py::arg("params") = py::dict(),
        "Identifies connected components on a nearest-neighbor graph. "
        "Returns (number_of_clusters, adjacencies).");

  m.def("compute_average_spacing_with_normals",
        &psp::compute_average_spacing_with_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Computes average spacing from k nearest neighbors for points with normals.");
  m.def("compute_average_spacing_with_normals",
        &psp::compute_average_spacing_with_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("k"),
        py::arg("params") = py::dict(),
        "Computes average spacing from k nearest neighbors for NumPy-style "
        "float64 point and normal arrays with shape (N, 3).");

  m.def("grid_simplify_point_set_with_normals",
        &psp::grid_simplify_point_set_with_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("epsilon"), py::arg("params") = py::dict(),
        "Simplifies points with normals by grid clustering. "
        "Returns (points, first_index_to_remove).");
  m.def("grid_simplify_point_set_with_normals",
        &psp::grid_simplify_point_set_with_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("epsilon"),
        py::arg("params") = py::dict(),
        "Simplifies NumPy-style float64 point and normal arrays with shape (N, 3) "
        "by grid clustering. Returns (points, first_index_to_remove).");

  m.def("pca_estimate_normals",
        &psp::pca_estimate_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Estimates normal directions using PCA for points with normals.");

  m.def("pca_estimate_normals",
        &psp::pca_estimate_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Estimates normal directions using PCA for NumPy-style float64 point arrays "
        "with shape (N, 3).");

  m.def("jet_estimate_normals",
        &psp::jet_estimate_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Estimates normal directions using jet fitting for points with normals.");

  m.def("jet_estimate_normals",
        &psp::jet_estimate_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Estimates normal directions using jet fitting for NumPy-style float64 point arrays "
        "with shape (N, 3).");

  m.def("mst_orient_normals",
        &psp::mst_orient_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Orients normals using MST propagation. Returns (points, first_unoriented_index).");

  m.def("mst_orient_normals",
        &psp::mst_orient_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("k"),
        py::arg("params") = py::dict(),
        "Orients normals using MST propagation for NumPy-style float64 point and normal "
        "arrays with shape (N, 3). Returns (points, first_unoriented_index).");


  m.def("compute_vcm",
        &psp::compute_vcm<Point_3>,
        py::arg("points"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("params") = py::dict(),
        "Computes Voronoi covariance matrices for a point range.");

  m.def("compute_vcm",
        &psp::compute_vcm_np<Point_3>,
        py::arg("points"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("params") = py::dict(),
        "Computes Voronoi covariance matrices for a NumPy-style float64 point array "
        "with shape (N, 3).");

  m.def("compute_vcm_with_normals",
        &psp::compute_vcm_with_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("params") = py::dict(),
        "Computes Voronoi covariance matrices for points with normals.");
  m.def("compute_vcm_with_normals",
        &psp::compute_vcm_with_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("params") = py::dict(),
        "Computes Voronoi covariance matrices for NumPy-style float64 point "
        "and normal arrays with shape (N, 3).");

  m.def("vcm_estimate_normals",
        &psp::vcm_estimate_normals<Point_3, Vector_3>,
        py::arg("points"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("params") = py::dict(),
        "Estimates normals using Voronoi covariance matrices.");
  m.def("vcm_estimate_normals",
        &psp::vcm_estimate_normals_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("offset_radius"),
        py::arg("convolution_radius"), py::arg("params") = py::dict(),
        "Estimates normals using Voronoi covariance matrices for NumPy-style "
        "float64 point and normal arrays with shape (N, 3).");

  m.def("vcm_estimate_normals_neighbors",
        &psp::vcm_estimate_normals_neighbors<Point_3, Vector_3>,
        py::arg("points"), py::arg("offset_radius"), py::arg("k"),
        py::arg("params") = py::dict(),
        "Estimates normals using Voronoi covariance matrices and a neighbor count.");
  m.def("vcm_estimate_normals_neighbors",
        &psp::vcm_estimate_normals_neighbors_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("offset_radius"),
        py::arg("k"), py::arg("params") = py::dict(),
        "Estimates normals using Voronoi covariance matrices and a neighbor count "
        "for NumPy-style float64 point and normal arrays with shape (N, 3).");

  m.def("vcm_is_on_feature_edge",
        &psp::vcm_is_on_feature_edge<typename Kernel::FT>,
        py::arg("cov"), py::arg("threshold"),
        "Tests whether a Voronoi covariance matrix marks a feature edge.");


  m.def("write_points",
        &psp::write_points<Point_3>,
        py::arg("fname"), py::arg("points"), py::arg("params") = py::dict(),
        "Writes a point range to a file.");

  m.def("write_points",
        &psp::write_points_np<Point_3>,
        py::arg("fname"), py::arg("points"), py::arg("params") = py::dict(),
        "Writes a NumPy-style float64 point array with shape (N, 3) "
        "to a point-set file.");

  m.def("write_points_with_normals",
        &psp::write_points_with_normals<Point_3, Vector_3>,
        py::arg("fname"), py::arg("points"), py::arg("params") = py::dict(),
        "Writes points with normals to a file.");
  m.def("write_points_with_normals",
        &psp::write_points_with_normals_np<Point_3, Vector_3>,
        py::arg("fname"), py::arg("points"), py::arg("normals"),
        py::arg("params") = py::dict(),
        "Writes NumPy-style float64 point and normal arrays with shape (N, 3) "
        "to a point-set file.");


  m.def("bilateral_smooth_point_set",
        &psp::bilateral_smooth_point_set<Point_3, Vector_3>,
        py::arg("points"), py::arg("k"), py::arg("params") = py::dict(),
        "Smooths points with normals using bilateral projection. "
        "Returns (average_movement_error, points).");
  m.def("bilateral_smooth_point_set",
        &psp::bilateral_smooth_point_set_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("k"),
        py::arg("params") = py::dict(),
        "Smooths NumPy-style float64 point and normal arrays with shape (N, 3) "
        "using bilateral projection. Returns (average_movement_error, points).");


  m.def("edge_aware_upsample_point_set",
        &psp::edge_aware_upsample_point_set<Point_3, Vector_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Upsamples points with normals while preserving sharp features.");
  m.def("edge_aware_upsample_point_set",
        &psp::edge_aware_upsample_point_set_np<Point_3, Vector_3>,
        py::arg("points"), py::arg("normals"), py::arg("params") = py::dict(),
        "Upsamples NumPy-style float64 point and normal arrays with shape (N, 3) "
        "while preserving sharp features.");


  m.def("wlop_simplify_and_regularize_point_set",
        &psp::wlop_simplify_and_regularize_point_set<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Simplifies and regularizes a point set using WLOP.");

  m.def("wlop_simplify_and_regularize_point_set",
        &psp::wlop_simplify_and_regularize_point_set_np<Point_3>,
        py::arg("points"), py::arg("params") = py::dict(),
        "Simplifies and regularizes a NumPy-style float64 point array "
        "with shape (N, 3) using WLOP.");

#endif

#if 0
  m.def("read_points_with_normals",
        [](const std::string& fname, const py::kwargs& np = py::kwargs()) {
          using PointVectorPair = std::pair<Kernel::Point_3, Kernel::Vector_3>;
          using Point_map = CGAL::First_of_pair_property_map<PointVectorPair>;
          using Normal_map = CGAL::Second_of_pair_property_map<PointVectorPair>;

          std::vector<PointVectorPair> output;
          bool success = CGAL::IO::read_points(fname, std::back_inserter(output));
          return std::make_pair(success, output);
        },
        py::arg("fname"), py::arg("np"),
        "reads the point set from an input file.\n"
        "Supported file formats are the following:\n"
        "• Object File Format (OFF) (.off)\n"
        "• Polygon File Format (PLY) (.ply)\n"
        "• LAS (Lidar) File Format (.las)\n"
        "• XYZ File Format (.xyz)\n"
        "The format is detected from the filename extension (letter case is not important).\n\n"
        "Parameters\n"
        "• fname: the name of the input file.\n"
        "• np: optional sequence of Named Parameters among the ones listed below.\n\n"
        "Optional Named Parameters\n"
        "• use_binary_mode: indicates whether data should be read in binary (true) or in ASCII (false) Default: true\n\n"
        "Returns\n"
        "a tuple of a boolean indicating if reading was successful and the output points.\n\n"
        "Examples\n"
        "Point_set_processing_3/average_spacing_example.py, Point_set_processing_3/bilateral_smooth_point_set_example.py, Point_set_processing_3/edge_aware_upsample_point_set_example.py, Point_set_processing_3/edges_example.py, Point_set_processing_3/grid_simplification_example.py, Point_set_processing_3/hierarchy_simplification_example.py, Point_set_processing_3/normals_example.py, Point_set_processing_3/registration_with_OpenGR.py, Point_set_processing_3/registration_with_opengr_pointmatcher_pipeline.py, Point_set_processing_3/registration_with_pointmatcher.py, Point_set_processing_3/remove_outliers_example.py, Point_set_processing_3/scale_estimation_example.py, Point_set_processing_3/structuring_example.py, and Point_set_processing_3/wlop_simplify_and_regularize_point_set_example.py."
        );

  m.def("write_points",
        [](const std::string& fname, const PointVector_3& points, const py::kwargs& np = py::kwargs()) {
          return CGAL::IO::write_points(fname, points);
        },
        py::arg("fname"), py::arg("points"), py::arg("np"),
        "writes the range of points with properties to a file.\n"
        "Supported file formats are the following:\n"
        "• Object File Format (OFF) (.off)\n"
        "• Polygon File Format (PLY) (.ply)\n"
        "• LAS (Lidar) File Format (.las)\n"
        "• XYZ File Format (.xyz)\n"
        "The format is detected from the filename extension (letter case is not important).\n\n"
        "Parameters\n"
        "• fname: the name of the output file.\n"
        "• points: the range of points that will be written.\n"
        "• np: optional sequence of Named Parameters among the ones listed below.\n\n"
        "Optional Named Parameters\n"
        "• use_binary_mode: indicates whether data should be written in binary (true) or in ASCII (false) (default: true)\n\n"
        "• stream_precision: a parameter used to set the precision (i.e. how many digits are generated) of the output stream (default: 6)\n\n"
        "Returns\n"
        "true if writing was successful, false otherwise. \n\n"
        "Examples\n"
        "Point_set_processing_3/edge_aware_upsample_point_set_example.py, Point_set_processing_3/hierarchy_simplification_example.py, Point_set_processing_3/registration_with_OpenGR.py, Point_set_processing_3/registration_with_opengr_pointmatcher_pipeline.py, Point_set_processing_3/registration_with_pointmatcher.py, Point_set_processing_3/structuring_example.py, and Point_set_processing_3/wlop_simplify_and_regularize_point_set_example.py."
        );

  m.def("write_points_with_normals",
        [](const std::string& fname, const std::vector<std::pair<Point_3, Vector_3>>& points,
           const py::kwargs& np = py::kwargs()) {
          return CGAL::IO::write_points(fname, points);
        },
        py::arg("fname"), py::arg("points"), py::arg("np"),
        "writes the range of points with properties to a file.\n"
        "Supported file formats are the following:\n"
        "• Object File Format (OFF) (.off)\n"
        "• Polygon File Format (PLY) (.ply)\n"
        "• LAS (Lidar) File Format (.las)\n"
        "• XYZ File Format (.xyz)\n"
        "The format is detected from the filename extension (letter case is not important).\n\n"
        "Parameters\n"
        "• fname: the name of the output file.\n"
        "• points: the range of points that will be written.\n"
        "• np: optional sequence of Named Parameters among the ones listed below.\n\n"
        "Optional Named Parameters\n"
        "• use_binary_mode: indicates whether data should be written in binary (true) or in ASCII (false) (default: true)\n\n"
        "• stream_precision: a parameter used to set the precision (i.e. how many digits are generated) of the output stream (default: 6)\n\n"
        "Returns\n"
        "true if writing was successful, false otherwise. \n\n"
        "Examples\n"
        "Point_set_processing_3/edge_aware_upsample_point_set_example.py, Point_set_processing_3/hierarchy_simplification_example.py, Point_set_processing_3/registration_with_OpenGR.py, Point_set_processing_3/registration_with_opengr_pointmatcher_pipeline.py, Point_set_processing_3/registration_with_pointmatcher.py, Point_set_processing_3/structuring_example.py, and Point_set_processing_3/wlop_simplify_and_regularize_point_set_example.py."
        );

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  m.def("vcm_is_on_feature_edge", &CGAL::vcm_is_on_feature_edge<Kernel::FT>,
        py::arg("cov"), py::arg("threshold"),
        "determines if a point is on a sharp feature edge from a point set for which the Voronoi covariance Measures have been computed.\n"
        "The sharpness of the edge, specified by parameter threshold, is used to filtered points according to the external angle around a sharp feature.\n"
      "A point is considered to be on a sharp feature if the external angle alpha at the edge is such that alpha >= 2 / sqrt(3) * sqrt(threshold). In particular this means that if the input contains sharp features with different external angles, the one with the smallest external angle should be considered, which however would result in selecting more points on sharper regions. More details are provided in [9].\n\n"
        "See also\n"
        "• CGALPY.compute_vcm()\n\n"
        "Examples\n"
        "Point_set_processing_3/edges_example.py."
        );
#endif
#endif
}
