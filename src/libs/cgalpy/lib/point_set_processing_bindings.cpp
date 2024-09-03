#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/bilateral_smooth_point_set.h>
#include <CGAL/cluster_point_set.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/edge_aware_upsample_point_set.h>
#include <CGAL/estimate_scale.h>
#include <CGAL/vcm_estimate_normals.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Named_function_parameters.h>

#include "CGALPY/internal.hpp"
#include "CGALPY/kernel_type.hpp"

namespace py = nanobind;

template <typename PointRange, typename Point, typename Vector, typename C>
void export_multiple_dimension_functions(C& c) {
  using K = Kernel;

  c.def("estimate_global_k_neighbor_scale", [](const PointRange& points, const py::kwargs& np = py::kwargs())
        { return CGAL::estimate_global_k_neighbor_scale(points,
                  internal::parse_named_parameters(np)
                         .geom_traits(K())
                          ); },
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

  c.def("estimate_global_range_scale", [](PointRange& points, const py::kwargs& np = py::kwargs())
        { CGAL::estimate_global_range_scale(points,
                  internal::parse_named_parameters(np)
                         .geom_traits(K())
                          ); },
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


}

template <typename PointRange, typename Point, typename Vector, typename C>
void export_functions_with_point_range(C& c) {
  using K = Kernel;
  using Tag = CGAL::Sequential_tag;

  c.def("compute_vcm", [](const PointRange& points,
                        double offset_radius,
                        double convolution_radius,
                        const py::kwargs& np = py::kwargs())
      { std::vector<std::array<double, 6>> ccov;
        return CGAL::compute_vcm(points, ccov, offset_radius, convolution_radius,
                internal::parse_named_parameters(np)
                         .geom_traits(K())
                                      );
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
      "average spacing (scalar). The return type FT is a number type."
      "Examples\n"
      "Point_set_processing_3/scale_estimation_example.py."
      );

  c.def("edge_aware_upsample_point_set", [](const PointRange& points, const py::kwargs& np = py::kwargs())
        { std::vector<std::pair<Point, Vector>> output;
          CGAL::edge_aware_upsample_point_set<Tag>(points, std::back_inserter(output),
                  internal::parse_named_parameters(np)
                         .geom_traits(K())
                                      );
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

}


template <typename Points, typename C>
void export_functions_with_point_range(C& c) {
  using K = Kernel;
  c.def("estimate_local_k_neighbor_scales", [](const Points& points,
                                               const Points& queries,
                                               const py::kwargs& np = py::kwargs())
        { std::vector<std::size_t> output;
          return CGAL::estimate_local_k_neighbor_scales(points, queries, std::back_inserter(output),
                  internal::parse_named_parameters(np)
                         .geom_traits(K())
                          );
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
}


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

  export_functions_with_point_range<PointVector_3, Point_3, Vector_3>(m);
  export_functions_with_point_range<PointSet_3, Point_3, Vector_3>(m);


  // export_multiple_dimension_functions<PointVector_2, Point_2, Vector_2>(m);
  // export_multiple_dimension_functions<PointSet_2, Point_2, Vector_2>(m);
  export_multiple_dimension_functions<PointVector_3, Point_3, Vector_3>(m);
  export_multiple_dimension_functions<PointSet_3, Point_3, Vector_3>(m);

  export_functions_with_point_range<PointVector_3>(m);

  // Algorithms
  m.def("bilateral_smooth_point_set", [](PointRange_3& points, unsigned int k, const py::kwargs& np = py::kwargs())
        { return CGAL::bilateral_smooth_point_set<Tag>(points, k,
                  internal::parse_named_parameters(np)
                           .point_map(CGAL::First_of_pair_property_map<PointVectorPair_3>())
                           .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair_3>())
                           .geom_traits(K())); },
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
        );
  m.def("cluster_point_set", [](PointSet_3& points, ClusterMap_3& cluster_map,
                                const std::function<bool(double)>& callback = std::function<bool(double)>(),
                                const py::kwargs& np = py::kwargs()) {
        std::vector<std::pair<std::size_t, std::size_t>> adj;
        auto res = CGAL::cluster_point_set(points, cluster_map
                                           ,
                internal::parse_named_parameters(np)
                         .callback(callback)
                         .adjacencies(std::back_inserter(adj))
                         // .geom_traits(K())
                         );
        return std::make_tuple(res, adj);
        }
        ,
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
        { return CGAL::compute_average_spacing<Tag>(points, k,
                  internal::parse_named_parameters(np)
                         .callback(callback)
                         .geom_traits(K())
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
        "Optional Named Parameters\n"
        "• callback\n"
        "• geom_traits\n\n"
        "Returns\n"
        "average spacing (scalar). The return type FT is a number type."
        );

  // TODO: compute_registration_transformation() needs OpenGR
  // TODO: compute_registration_transformation() needs PointMatcher


}
// Function Documentation
// ◆ bilateral_smooth_point_set()
// template<typename ConcurrencyTag , typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// double CGAL::bilateral_smooth_point_set 	( 	PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/bilateral_smooth_point_set.h>
//
// This function smooths an input point set by iteratively projecting each point onto the implicit surface patch fitted over its nearest neighbors.
//
// Bilateral projection preserves sharp features according to the normal (gradient) information. Both point positions and normals will be modified. For more details, please see section 4 in [5].
//
// A parallel version of this function is provided and requires the executable to be linked against the Intel TBB library. To control the number of threads used, the user may use the tbb::task_scheduler_init class. See the TBB documentation for more details.
//
// Precondition
//     Normals must be unit vectors 
//     k >= 2
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	size of the neighborhood for the implicit surface patch fitting. The larger the value is, the smoother the result will be.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//         Default: Normals are computed and stored internally.
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     sharpness_angle
//
//         controls the sharpness of the result
//         Type: floating scalar value
//         Default: 30
//         Extra: The larger the value is, the smoother the result will be. The range of possible value is [0, 90]
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped, all points are left unchanged and the function return NaN.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//         Extra: When CGAL::Parallel_tag is used, the callback mechanism is called asynchronously on a separate thread and shouldn't access or modify the variables that are parameters of the algorithm.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     Average point movement error. It's a convergence criterium for the algorithm. This value can help the user to decide how many iterations are sufficient. 
//
// ◆ cluster_point_set()
// template<typename PointRange , typename ClusterMap , typename NamedParameters = parameters::Default_named_parameters>
// std::size_t CGAL::cluster_point_set 	( 	PointRange &  	points,
// 		ClusterMap  	cluster_map,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/cluster_point_set.h>
//
// Identifies connected components on a nearest neighbor graph built using a query sphere of fixed radius centered on each point.
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//     ClusterMap	is a model of ReadWritePropertyMap with value type std::size_t.
//
// Parameters
//     points	input point range
//     cluster_map	maps each point to the index of the cluster it belongs to.
//     np	optional sequence of Named Parameters among the ones listed below.
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped and the number of already computed clusters is returned.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 1 percent of the bounding box diagonal
//
//     	
//
//     attraction_factor
//
//         used to compute adjacencies between clusters. Adjacencies are computed using a nearest neighbor graph built similarly to the one used for clustering, using attraction_factor * neighbor_radius as parameter.
//         Type: floating scalar value
//         Default: 2
//
//     	
//
//     adjacencies
//
//         an output iterator used to output pairs containing the indices of two adjacent clusters.
//         Type: a model of OutputIterator that accepts objects of type std::pair<std::size_t, std::size_t>
//         Default: CGAL::Emptyset_iterator
//         Extra: If this parameter is not used, adjacencies are not computed at all.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     the number of clusters identified. 
//
// Examples
//     Point_set_processing_3/clustering_example.cpp.
//
// ◆ compute_average_spacing()
// template<typename ConcurrencyTag , typename PointRange , typename NamedParameters = CGAL::parameters::Default_named_parameters>
// FT CGAL::compute_average_spacing 	( 	const PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/compute_average_spacing.h>
//
// Computes average spacing from k nearest neighbors.
//
// Precondition
//     k >= 2.
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	number of neighbors.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped, the average spacing value estimated on the processed subset is returned.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//         Extra: When CGAL::Parallel_tag is used, the callback mechanism is called asynchronously on a separate thread and shouldn't access or modify the variables that are parameters of the algorithm.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     average spacing (scalar). The return type FT is a number type. It is either deduced from the geom_traits Named Parameters if provided, or the geometric traits class deduced from the point property map of points. 
//
// ◆ compute_registration_transformation() [1/2]
// template<class PointRange1 , class PointRange2 , class NamedParameters1 = parameters::Default_named_parameters, class NamedParameters2 = parameters::Default_named_parameters>
// std::pair< geom_traits::Aff_transformation_3, double > CGAL::OpenGR::compute_registration_transformation 	( 	const PointRange1 &  	point_set_1,
// 		const PointRange2 &  	point_set_2,
// 		const NamedParameters1 &  	np1 = parameters::default_values(),
// 		const NamedParameters2 &  	np2 = parameters::default_values() 
// 	) 		
//
// #include <CGAL/OpenGR/compute_registration_transformation.h>
//
// Computes the registration of point_set_2 with respect to point_set_1 and returns the corresponding affine transformation along with the registration score.
//
// Registration is computed using the Super4PCS algorithm [8].
//
// Note
//     This function requires the OpenGR library.
//
// Warning
//     Although this may seem counter-intuitive, if one of the two point set matches only a small section of the other one, it is advised to use the small point set as reference instead of the big one. The reason is that the reference point set is used to construct a base that is sought after in the other point set: if the big point set is used as reference, chances are the constructed base will not be present in the small point set.
//
// Template Parameters
//     PointRange1	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters1.
//     PointRange2	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters2.
//
// Parameters
//     point_set_1	input point range used as reference.
//     point_set_2	input point range whose registration w.r.t. point_set_1 will be computed.
//     np1	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Vector_3
//         Default: Normals are computed and stored internally.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
//     number_of_samples
//
//         size of the subset of input points used to compute registration
//         Type: unsigned int
//         Default: 200
//         Extra: Input clouds are sub-sampled prior exploration, to ensure fast computations. Super4PCS has a linear complexity w.r.t. the number of input samples, allowing to use larger values than 4PCS. Simple geometry with large overlap can be matched with only 200 samples. However, with Super4PCS, smaller details can be used during the process by using up to thousands of points. There is no theoretical limit to this parameter, however using too large values leads to very a large congruent set, which requires more time and memory to be explored. Using a large number of samples is recommended when: geometrical details are required to perform the matching, for instance to disambiguate between several similar configurations; the clouds have a very low overlap: using a too sparse sampling can prevent to have samples in the overlapping area, causing the algorithm to fail; the clouds are very noisy, and require a dense sampling. Note that Super4PCS is a global registration algorithm, which finds a good approximate of the rigid transformation aligning too clouds. Increasing the number of samples in order to get a fine registration is not optimal: it is usually faster to use less samples, and refine the transformation using a local algorithm, like the ICP, or its variant SparseICP.
//
//     	
//
//     maximum_normal_deviation
//
//         angle threshold (in degrees) used to filter pairs of points according to their normal consistency
//         Type: floating scalar value
//         Default: 90.00
//         Extra: Small values decrease computation time but may also decrease the quality if pairs of points that should match have a normal deviation higher than the threshold.
//
//     	
//
//     accuracy
//
//         registration accuracy (delta in the paper)
//         Type: floating scalar value
//         Default: 5.00
//         Extra: Setting a small value means that the two clouds needs to be very close to be considered as well aligned. It is expressed in scene units. A simple way to understand its impact is to consider the computation of the Largest Common Pointset (LCP), the metric used to verify how much the clouds are aligned. For each transformation matrix produced by Super4PCS, we compute the LCP measure by considering a shell around the reference cloud, and count the percentage of points of the target cloud lying in the shell. The thickness of the shell is defined by the parameter delta.
//
//     	
//
//     overlap
//
//         ratio of expected overlap between the two point sets: it is ranging between 0 (no overlap) to 1 (100% overlap)
//         Type: floating scalar value
//         Default: 0.20
//         Extra: The overlap parameter controls the size of the basis used for registration. Usually, the larger the overlap, the faster the algorithm. When the overlap is unknown, a simple way to set this parameter is to start from 100% overlap, and decrease the value until obtaining a good result. Using too small values will slow down the algorithm, and reduce the accuracy of the result.
//
//     	
//
//     maximum_running_time
//
//         maximum number of seconds after which the algorithm terminates.
//         Type: floating scalar value
//         Default: 1000
//         Extra: Super4PCS explores the transformation space to align the two input clouds. Since the exploration is performed randomly, it is recommended to use a large time value to explore the whole space.
//
//     	
//
// Parameters
//     np2	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Vector_3
//         Default: Normals are computed and stored internally.
//
//     	
//
// Returns
//     a pair containing the affine transformation that should be applied to point_set_2 to make it registered w.r.t. point_set_1 and the registration score. 
//
// Examples
//     Point_set_processing_3/registration_with_OpenGR.cpp, and Point_set_processing_3/registration_with_opengr_pointmatcher_pipeline.cpp.
//
// ◆ compute_registration_transformation() [2/2]
// template<class PointRange1 , class PointRange2 , class NamedParameters1 = parameters::Default_named_parameters, class NamedParameters2 = parameters::Default_named_parameters>
// std::pair< geom_traits::Aff_transformation_3, bool > CGAL::pointmatcher::compute_registration_transformation 	( 	const PointRange1 &  	point_set_1,
// 		const PointRange2 &  	point_set_2,
// 		const NamedParameters1 &  	np1 = parameters::default_values(),
// 		const NamedParameters2 &  	np2 = parameters::default_values() 
// 	) 		
//
// #include <CGAL/pointmatcher/compute_registration_transformation.h>
//
// Computes the registration of point_set_2 with respect to point_set_1 and returns the corresponding affine transformation.
//
// Registration is computed using the Iterative Closest Point (ICP) algorithm.
//
// Note
//     This function requires the PointMatcher library.
//
// Template Parameters
//     PointRange1	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters1.
//     PointRange2	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters2.
//
// Parameters
//     point_set_1	input point range used as reference.
//     point_set_2	input point range whose registration w.r.t. point_set_1 will be computed.
//     np1	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Vector_3
//
//     	
//
//     scalar_map
//
//         a property map associating 1D values - scalars to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::FT
//         Default: CGAL::Constant_property_map with the value = 1 for all scalars
//         Extra: These scalars, sometimes called weights, can be provided through the GenericDescriptorOutlierFilter of PointMatcher library, where the descName = weights. See outlier_filters below and registration_with_pointmatcher.cpp for more details.
//
//     	
//
//     point_set_filters
//
//         a chain of filters to be applied to the point set
//         Type: a class model of Range. The value type of its iterator must be ICP_config.
//         Default: RandomSamplingDataPointsFilter
//
//         Extra: The chain of filters to be applied to the reference point cloud. The reference point cloud is processed into an intermediate point cloud with the given chain of filters to be used in the alignment procedure. The chain is organized with the forward traversal order of the point set filters range.
//
//         The chain of point set filters are applied only once at the beginning of the ICP procedure, i.e., before the first iteration of the ICP algorithm.
//
//         The filters can have several purposes, including but are not limited to i) removal of noisy points which render alignment of point clouds difficult, ii) removal of redundant points so as to speed up alignment, iii) addition of descriptive information to the points such as a surface normal vector, or the direction from the point to the sensor.
//         Extra: Corresponds to referenceDataPointsFilters configuration module of PointMatcher library. The filters should be chosen and set from possible components of the referenceDataPointsFilters configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     matcher
//
//         a method used for matching (linking) the points from point_set_2, to the points in the reference cloud, point_set_1
//         Type: a class model of ICP_config
//         Default: KDTreeMatcher
//         Extra: Corresponds to the matcher configuration module of PointMatcher library. The matcher should be chosen and set from possible components of the matcher configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     outlier_filters
//
//         a chain of filters to be applied to the matched (linked) point clouds after each processing iteration of the ICP algorithm to remove the links which do not correspond to true point correspondences
//         Type: a model of Range. The value type of its iterator must be ICP_config.
//         Default: TrimmedDistOutlierFilter
//         Extra: The outliers are rejected. Points with no link are ignored in the subsequent error minimization step. The chain is organized with the forward traversal order of the outlier filters range.
//         Extra: Corresponds to the outlierFilters configuration module of PointMatcher library. The filters should be chosen and set from possible components of the outlierFilters configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     error_minimizer
//
//         an error minimizer that computes a transformation matrix such as to minimize the error between the point sets
//         Type: a class model of ICP_config
//         Default: PointToPlaneErrorMinimizer
//         Extra: Corresponds to the errorMinimizer configuration module of PointMatcher library. The error minimizer should be chosen and set from possible components of the errorMinimizer configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     transformation_checkers
//
//         a chain of transformation checkers
//         Type: a class model of Range. The value type of its iterator must be ICP_config.
//         Default: CounterTransformationChecker and DifferentialTransformationChecker
//         Extra: The chain is organized with the forward traversal order of the transformation checkers range.
//         Extra: A transformation checker can stop the iteration depending on the conditions it defines.
//         Extra: Corresponds to the transformationCheckers configuration module of PointMatcher library. The transformation checkers should be chosen and set from possible components of the transformationCheckers configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     inspector
//
//         an inspector that enables logging data at different steps for analysis.
//         Type: a class model of ICP_config
//         Default: NullInspector
//         Extra: Inspectors typically provide deeper scrutiny than the logger.
//         Extra: Corresponds to the inspector configuration module of PointMatcher library. The inspector should be chosen and set from possible components of the inspector configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     logger
//
//         a method for logging information regarding the registration process outputted by PointMatcher library
//         Type: a class model of ICP_config
//         Default: NullLogger
//         Extra: The logs generated by CGAL library does not get effected by this configuration.
//         Extra: Corresponds to the logger configuration module of PointMatcher library. The logger should be chosen and set from possible components of the logger configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Parameters
//     np2	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Vector_3
//
//     	
//
//     scalar_map
//
//         a property map associating 1D values - scalars to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::FT
//         Default: CGAL::Constant_property_map with the value = 1 for all scalars
//         Extra: These scalars, sometimes called weights, can be provided through the GenericDescriptorOutlierFilter of PointMatcher library, where the descName = weights.
//
//     	
//
//     point_set_filters
//
//         a chain of filters to be applied to the point set
//         Type: a class model of Range. The value type of its iterator must be ICP_config.
//         Default: SamplingSurfaceNormalDataPointsFilter
//
//         Extra: The chain of filters to be applied to the point cloud point_set_2. The point cloud is processed into an intermediate point cloud with the given chain of filters to be used in the alignment procedure. The chain is organized with the forward traversal order of the point set filters range.
//
//         The chain of point set filters are applied only once at the beginning of the ICP procedure, i.e., before the first iteration of the ICP algorithm.
//
//         The filters can have several purposes, including but are not limited to i) removal of noisy points which render alignment of point clouds difficult, ii) removal of redundant points so as to speed up alignment, iii) addition of descriptive information to the points such as a surface normal vector, or the direction from the point to the sensor.
//         Extra: Corresponds to the readingDataPointsFilters configuration module of PointMatcher library. The filters should be chosen and set from possible components of the readingDataPointsFilters configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     transformation
//
//         an affine transformation that is used as the initial transformation for point_set_2
//         Type: CGAL::Aff_transformation_3
//         Default: the identity transformation
//
//     	
//
// Returns
//     a pair containing the affine transformation that should be applied to point_set_2 to make it registered w.r.t. point_set_1 and the boolean value indicating if the registration converged. The second of the pair is true if converged, false otherwise. A log why it failed to converge is written to std::cerr if the registration cannot converge. 
//
// Examples
//     Point_set_processing_3/registration_with_pointmatcher.cpp.
//
// ◆ compute_vcm()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::compute_vcm 	( 	const PointRange &  	points,
// 		std::vector< std::array< double, 6 > > &  	ccov,
// 		double  	offset_radius,
// 		double  	convolution_radius,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/vcm_estimate_normals.h>
//
// computes the Voronoi Covariance Measure (VCM) of a point cloud, a construction that can be used for normal estimation and sharp feature detection.
//
// The VCM associates to each point the covariance matrix of its Voronoi cell intersected with the ball of radius offset_radius. In addition, if the second radius convolution_radius is positive, the covariance matrices are smoothed via a convolution process. More specifically, each covariance matrix is replaced by the average of the matrices of the points located at a distance at most convolution_radius. The choice for parameter offset_radius should refer to the geometry of the underlying surface while the choice for parameter convolution_radius should refer to the noise level in the point cloud. For example, if the point cloud is a uniform and noise-free sampling of a smooth surface, offset_radius should be set to the minimum local feature size of the surface, while convolution_radius can be set to zero.
//
// The Voronoi covariance matrix of each vertex is stored in an array a of length 6 and is as follow:
// ⎡⎣⎢a[0]a[1]a[2]a[1]a[3]a[4]a[2]a[4]a[5]⎤⎦⎥
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     ccov	output range of covariance matrices.
//     offset_radius	offset_radius.
//     convolution_radius	convolution_radius.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// See also
//     CGAL::vcm_is_on_feature_edge() 
//     CGAL::vcm_estimate_normals() 
//
// Examples
//     Point_set_processing_3/edges_example.cpp.
//
// ◆ edge_aware_upsample_point_set()
// template<typename ConcurrencyTag , typename PointRange , typename OutputIterator , typename NamedParameters = parameters::Default_named_parameters>
// OutputIterator CGAL::edge_aware_upsample_point_set 	( 	const PointRange &  	points,
// 		OutputIterator  	output,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/edge_aware_upsample_point_set.h>
//
// This method progressively upsamples the point set while approaching the edge singularities (detected by normal variation), which generates a denser point set from an input point set.
//
// This has applications in point-based rendering, hole filling, and sparse surface reconstruction. Normals of points are required as input. For more details, please refer to [5].
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel versions of compute_average_spacing() (called internally). Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//     OutputIterator	Type of the output iterator. The type of the objects put in it is std::pair<geom_traits::Point_3, geom_traits::Vector_3>. Note that the user may use a function_output_iterator to match specific needs.
//
// Parameters
//     points	input point range
//     output	iterator where output points and normals are put.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     sharpness_angle
//
//         controls the sharpness of the result
//         Type: floating scalar value
//         Default: 30.00
//         Extra: The larger the value is, the smoother the result will be. The range of possible value is [0, 90]
//
//     	
//
//     edge_sensitivity
//
//         controls the priority of points inserted along sharp features
//         Type: floating scalar value
//         Default: 1
//         Extra: Larger values of edge-sensitivity give higher priority to inserting points along sharp features. The range of possible values is [0, 1]. See section Parameter: edge_sensitivity for an example
//
//     	
//
//     number_of_output_points
//
//         the number of output points to generate
//         Type: unsigned int
//         Default: 1000
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// ◆ estimate_global_k_neighbor_scale()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// std::size_t CGAL::estimate_global_k_neighbor_scale 	( 	const PointRange &  	points,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/estimate_scale.h>
//
// Estimates the global scale in a K nearest neighbors sense.
//
// The computed scale corresponds to the smallest scale such that the K subsets of points have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).
//
// Template Parameters
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3 (or geom_traits::Point_2)
//         Default: CGAL::Identity_property_map<geom_traits::Point_3> (or CGAL::Identity_property_map<geom_traits::Point_2>)
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Note
//     This function accepts both 2D and 3D points.
//
// Returns
//     The estimated scale in the K nearest neighbors sense. 
//
// Examples
//     Point_set_processing_3/scale_estimation_example.cpp.
//
// ◆ estimate_global_range_scale()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// FT CGAL::estimate_global_range_scale 	( 	const PointRange &  	points,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/estimate_scale.h>
//
// Estimates the global scale in a range sense.
//
// The computed scale corresponds to the smallest scale such that the subsets of points inside the sphere range have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).
//
// Template Parameters
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3 (or geom_traits::Point_2)
//         Default: CGAL::Identity_property_map<geom_traits::Point_3> (or CGAL::Identity_property_map<geom_traits::Point_2>)
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Note
//     This function accepts both 2D and 3D points.
//
// Returns
//     The estimated scale in the range sense. The return type FT is a number type. It is either deduced from the geom_traits Named Parameters if provided, or the geometric traits class deduced from the point property map of points. 
//
// Examples
//     Point_set_processing_3/scale_estimation_example.cpp.
//
// ◆ estimate_local_k_neighbor_scales()
// template<typename PointRange , typename QueryPointRange , typename OutputIterator , typename NamedParameters = parameters::Default_named_parameters>
// OutputIterator CGAL::estimate_local_k_neighbor_scales 	( 	const PointRange &  	points,
// 		const QueryPointRange &  	queries,
// 		OutputIterator  	output,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/estimate_scale.h>
//
// Estimates the local scale in a K nearest neighbors sense on a set of user-defined query points.
//
// The computed scales correspond to the smallest scales such that the K subsets of points have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).
//
// Template Parameters
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//     QueryPointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter query_point_map.
//     OutputIterator	is used to store the computed scales. It accepts values of type std::size_t.
//
// Parameters
//     points	input point range
//     queries	range of locations where scale must be estimated
//     output	iterator to store the computed scales
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3 (or geom_traits::Point_2)
//         Default: CGAL::Identity_property_map<geom_traits::Point_3> (or CGAL::Identity_property_map<geom_traits::Point_2>)
//
//     	
//
//     query_point_map
//
//         the property map containing the points associated to the elements of the point range queries
//         Type: a model of ReadablePropertyMap with value type geom_traits::Point_3 (or geom_traits::Point_2)
//         Default: CGAL::Identity_property_map<geom_traits::Point_3> (or CGAL::Identity_property_map<geom_traits::Point_2>)
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Note
//     This function accepts both 2D and 3D points, but sample points and query must have the same dimension. 
//
// Examples
//     Point_set_processing_3/scale_estimation_2d_example.cpp.
//
// ◆ estimate_local_range_scales()
// template<typename PointRange , typename QueryPointRange , typename OutputIterator , typename NamedParameters = parameters::Default_named_parameters>
// OutputIterator CGAL::estimate_local_range_scales 	( 	const PointRange &  	points,
// 		const QueryPointRange &  	queries,
// 		OutputIterator  	output,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/estimate_scale.h>
//
// Estimates the local scale in a range sense on a set of user-defined query points.
//
// The computed scales correspond to the smallest scales such that the subsets of points included in the sphere range have the appearance of a surface in 3D or the appearance of a curve in 2D (see Automatic Scale Estimation).
//
// Template Parameters
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//     QueryPointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter query_point_map.
//     OutputIterator	is used to store the computed scales. It accepts values of type geom_traits::FT.
//
// Parameters
//     points	input point range
//     queries	range of locations where scale must be estimated
//     output	iterator to store the computed scales
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3 (or geom_traits::Point_2)
//         Default: CGAL::Identity_property_map<geom_traits::Point_3> (or CGAL::Identity_property_map<geom_traits::Point_2>)
//
//     	
//
//     query_point_map
//
//         the property map containing the points associated to the elements of the point range queries
//         Type: a model of ReadablePropertyMap with value type geom_traits::Point_3 (or geom_traits::Point_2)
//         Default: CGAL::Identity_property_map<geom_traits::Point_3> (or CGAL::Identity_property_map<geom_traits::Point_2>)
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Note
//     This function accepts both 2D and 3D points, but sample points and query must have the same dimension. 
//
// ◆ grid_simplify_point_set()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// PointRange::iterator CGAL::grid_simplify_point_set 	( 	PointRange &  	points,
// 		double  	epsilon,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/grid_simplify_point_set.h>
//
// Merges points which belong to the same cell of a grid of cell size = epsilon.
//
// This method modifies the order of input points so as to pack all remaining points first, and returns an iterator over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.
//
// Precondition
//     epsilon > 0
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     epsilon	tolerance value when merging 3D points.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     min_points_per_cell
//
//         minimum number of points in a cell such that a point in this cell is kept after simplification
//         Type: unsigned int
//         Default: 1
//         Extra: If a value greater than 1 is used, the algorithm also acts as an outlier filtering algorithm, by removing low-density areas.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     iterator over the first point to remove. 
//
// Examples
//     Point_set_processing_3/callback_example.cpp, Point_set_processing_3/grid_simplification_example.cpp, Point_set_processing_3/grid_simplify_indices.cpp, and Point_set_processing_3/scale_estimation_example.cpp.
//
// ◆ hierarchy_simplify_point_set()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// PointRange::iterator CGAL::hierarchy_simplify_point_set 	( 	PointRange &  	points,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/hierarchy_simplify_point_set.h>
//
// Recursively split the point set in smaller clusters until the clusters have fewer than size elements and until their variation factor is below var_max.
//
// This method modifies the order of input points so as to pack all remaining points first, and returns an iterator over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.
//
// Precondition
//     0 < maximum_variation <= 1/3 
//     size > 0
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped and simplification stops with no guarantee on the output.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//
//     	
//
//     size
//
//         a value for cluster size
//         Type: unsigned int
//         Default: 10
//
//     	
//
//     maximum_variation
//
//         a value for maximum cluster variation
//         Type: floating scalar value
//         Default: 1/3
//
//     	
//
//     diagonalize_traits
//
//         the solver used for diagonalizing covariance matrices
//         Type: a model of DiagonalizeTraits
//         Default: If Eigen 3 (or greater) is available and CGAL_EIGEN3_ENABLED is defined then an overload using Eigen_diagonalize_traits is provided. Otherwise, the internal implementation CGAL::Diagonalize_traits is used.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     iterator over the first point to remove. 
//
// Examples
//     Point_set_processing_3/hierarchy_simplification_example.cpp.
//
// ◆ jet_estimate_normals()
// template<typename ConcurrencyTag , typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::jet_estimate_normals 	( 	PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/jet_estimate_normals.h>
//
// Estimates normal directions of the range of points using jet fitting on the nearest neighbors.
//
// The output normals are randomly oriented.
//
// Precondition
//     k >= 2
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	number of neighbors
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of WritablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     degree_fitting
//
//         the degree of fitting
//         Type: unsigned int
//         Default: 2
//         Extra: see CGAL::Monge_via_jet_fitting
//
//     	
//
//     svd_traits
//
//         the linear algebra algorithm used in the class CGAL::Monge_via_jet_fitting
//         Type: a class fitting the requirements of CGAL::Monge_via_jet_fitting
//         Default: If Eigen 3.2 (or greater) is available and CGAL_EIGEN3_ENABLED is defined, then CGAL::Eigen_svd is used.
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped and the remaining normals are left unchanged.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//         Extra: When CGAL::Parallel_tag is used, the callback mechanism is called asynchronously on a separate thread and shouldn't access or modify the variables that are parameters of the algorithm.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// ◆ jet_smooth_point_set()
// template<typename ConcurrencyTag , typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::jet_smooth_point_set 	( 	PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/jet_smooth_point_set.h>
//
// Smoothes the range of points using jet fitting on the nearest neighbors and reprojection onto the jet.
//
// As this method relocates the points, it should not be called on containers sorted w.r.t. point locations.
//
// Precondition
//     k >= 2
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	number of neighbors
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     degree_fitting
//
//         the degree of fitting
//         Type: unsigned int
//         Default: 2
//         Extra: see CGAL::Monge_via_jet_fitting
//
//     	
//
//     degree_monge
//
//         the Monge degree
//         Type: unsigned int
//         Default: 2
//         Extra: see CGAL::Monge_via_jet_fitting
//
//     	
//
//     svd_traits
//
//         the linear algebra algorithm used in the class CGAL::Monge_via_jet_fitting
//         Type: a class fitting the requirements of CGAL::Monge_via_jet_fitting
//         Default: If Eigen 3.2 (or greater) is available and CGAL_EIGEN3_ENABLED is defined, then CGAL::Eigen_svd is used.
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped and the remaining points are left unchanged.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//         Extra: When CGAL::Parallel_tag is used, the callback mechanism is called asynchronously on a separate thread and shouldn't access or modify the variables that are parameters of the algorithm.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// ◆ mst_orient_normals()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// PointRange::iterator CGAL::mst_orient_normals 	( 	PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/mst_orient_normals.h>
//
// Orients the normals of the range of points using the propagation of a seed orientation through a minimum spanning tree of the Riemannian graph.
//
// This method modifies the order of input points so as to pack all successfully oriented points first, and returns an iterator over the first point with an unoriented normal (see erase-remove idiom). For this reason it should not be called on sorted containers. It is based on [3].
//
// Precondition
//     Normals must be unit vectors 
//     k >= 2
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	number of neighbors.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of WritablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     point_is_constrained_map
//
//         a property map containing information about points being constrained or not
//         Type: a class model of ReadablePropertyMap with PointRange::iterator::value_type as key type and bool as value type
//         Default: If this parameter is omitted, the highest point (highest Z coordinate) will be used as the unique seed with an upward oriented.
//         Extra: Points with a true value will be used as seed points: their normal will be considered as already oriented, it won't be altered and it will be propagated to its neighbors.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     iterator over the first point with an unoriented normal. 
//
// Examples
//     Point_set_processing_3/normals_example.cpp.
//
// ◆ pca_estimate_normals()
// template<typename ConcurrencyTag , typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::pca_estimate_normals 	( 	PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/pca_estimate_normals.h>
//
// Estimates normal directions of the range of points by linear least squares fitting of a plane over the nearest neighbors.
//
// The output normals are randomly oriented.
//
// Precondition
//     k >= 2
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	number of neighbors
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of WritablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped and the remaining normals are left unchanged.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//         Extra: When CGAL::Parallel_tag is used, the callback mechanism is called asynchronously on a separate thread and shouldn't access or modify the variables that are parameters of the algorithm.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// ◆ random_simplify_point_set()
// template<typename PointRange >
// PointRange::iterator CGAL::random_simplify_point_set 	( 	PointRange &  	points,
// 		double  	removed_percentage 
// 	) 		
//
// #include <CGAL/random_simplify_point_set.h>
//
// Randomly deletes a user-specified fraction of the input points.
//
// This method modifies the order of input points so as to pack all remaining points first, and returns an iterator over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.
//
// Template Parameters
//     PointRange	is a model of Range.
//
// Parameters
//     points	input point range
//     removed_percentage	percentage of points to remove.
//
// Returns
//     iterator over the first point to remove. 
//
// ◆ register_point_sets() [1/2]
// template<class PointRange1 , class PointRange2 , class NamedParameters1 = parameters::Default_named_parameters, class NamedParameters2 = parameters::Default_named_parameters>
// double CGAL::OpenGR::register_point_sets 	( 	const PointRange1 &  	point_set_1,
// 		PointRange2 &  	point_set_2,
// 		const NamedParameters1 &  	np1 = parameters::default_values(),
// 		const NamedParameters2 &  	np2 = parameters::default_values() 
// 	) 		
//
// #include <CGAL/OpenGR/register_point_sets.h>
//
// Computes the registration of point_set_2 with respect to point_set_1 and applies it.
//
// Registration is computed using the Super4PCS algorithm [8]. Parameters documentation is copy-pasted from the official documentation of OpenGR. For more details on this method, please refer to it.
//
// Note
//     This function requires the OpenGR library.
//
// Warning
//     Although this may seem counter-intuitive, if one of the two point set matches only a small section of the other one, it is advised to use the small point set as reference instead of the big one. The reason is that the reference point set is used to construct a base that is sought after in the other point set: if the big point set is used as reference, chances are the constructed base will not be present in the small point set.
//
// Template Parameters
//     PointRange1	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters1.
//     PointRange2	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters2.
//
// Parameters
//     point_set_1	input point range used as reference.
//     point_set_2	input point range whose registration w.r.t. point_set_1 will be computed.
//     np1	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Vector_3
//         Default: Normals are computed and stored internally.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
//     number_of_samples
//
//         size of the subset of input points used to compute registration
//         Type: unsigned int
//         Default: 200
//         Extra: Input clouds are sub-sampled prior exploration, to ensure fast computations. Super4PCS has a linear complexity w.r.t. the number of input samples, allowing to use larger values than 4PCS. Simple geometry with large overlap can be matched with only 200 samples. However, with Super4PCS, smaller details can be used during the process by using up to thousands of points. There is no theoretical limit to this parameter, however using too large values leads to very a large congruent set, which requires more time and memory to be explored. Using a large number of samples is recommended when: geometrical details are required to perform the matching, for instance to disambiguate between several similar configurations; the clouds have a very low overlap: using a too sparse sampling can prevent to have samples in the overlapping area, causing the algorithm to fail; the clouds are very noisy, and require a dense sampling. Note that Super4PCS is a global registration algorithm, which finds a good approximate of the rigid transformation aligning too clouds. Increasing the number of samples in order to get a fine registration is not optimal: it is usually faster to use less samples, and refine the transformation using a local algorithm, like the ICP, or its variant SparseICP.
//
//     	
//
//     maximum_normal_deviation
//
//         angle threshold (in degrees) used to filter pairs of points according to their normal consistency
//         Type: floating scalar value
//         Default: 90.00
//         Extra: Small values decrease computation time but may also decrease the quality if pairs of points that should match have a normal deviation higher than the threshold.
//
//     	
//
//     accuracy
//
//         registration accuracy (delta in the paper)
//         Type: floating scalar value
//         Default: 5.00
//         Extra: Setting a small value means that the two clouds needs to be very close to be considered as well aligned. It is expressed in scene units. A simple way to understand its impact is to consider the computation of the Largest Common Pointset (LCP), the metric used to verify how much the clouds are aligned. For each transformation matrix produced by Super4PCS, we compute the LCP measure by considering a shell around the reference cloud, and count the percentage of points of the target cloud lying in the shell. The thickness of the shell is defined by the parameter delta.
//
//     	
//
//     overlap
//
//         ratio of expected overlap between the two point sets: it is ranging between 0 (no overlap) to 1 (100% overlap)
//         Type: floating scalar value
//         Default: 0.20
//         Extra: The overlap parameter controls the size of the basis used for registration. Usually, the larger the overlap, the faster the algorithm. When the overlap is unknown, a simple way to set this parameter is to start from 100% overlap, and decrease the value until obtaining a good result. Using too small values will slow down the algorithm, and reduce the accuracy of the result.
//
//     	
//
//     maximum_running_time
//
//         maximum number of seconds after which the algorithm terminates.
//         Type: floating scalar value
//         Default: 1000
//         Extra: Super4PCS explores the transformation space to align the two input clouds. Since the exploration is performed randomly, it is recommended to use a large time value to explore the whole space.
//
//     	
//
// Parameters
//     np2	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Vector_3
//         Default: Normals are computed and stored internally.
//
//     	
//
// Returns
//     the registration score. 
//
// Examples
//     Point_set_processing_3/registration_with_OpenGR.cpp.
//
// ◆ register_point_sets() [2/2]
// template<class PointRange1 , class PointRange2 , class NamedParameters1 = parameters::Default_named_parameters, class NamedParameters2 = parameters::Default_named_parameters>
// bool CGAL::pointmatcher::register_point_sets 	( 	const PointRange1 &  	point_set_1,
// 		PointRange2 &  	point_set_2,
// 		const NamedParameters1 &  	np1 = parameters::default_values(),
// 		const NamedParameters2 &  	np2 = parameters::default_values() 
// 	) 		
//
// #include <CGAL/pointmatcher/register_point_sets.h>
//
// Computes the registration of point_set_2 with respect to point_set_1 and applies it.
//
// Registration is computed using the Iterative Closest Point (ICP) algorithm.
//
// Note
//     This function requires the PointMatcher library.
//
// Template Parameters
//     PointRange1	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters1.
//     PointRange2	is a model of Range. The value type of its iterator is the key type of the named parameter point_map in NamedParameters2.
//
// Parameters
//     point_set_1	input point range used as reference.
//     point_set_2	input point range whose registration w.r.t. point_set_1 will be computed.
//     np1	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_1
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange1 and whose value type is geom_traits::Vector_3
//
//     	
//
//     point_set_filters
//
//         a chain of filters to be applied to the point set
//         Type: a class model of Range. The value type of its iterator must be ICP_config.
//         Default: RandomSamplingDataPointsFilter
//
//         Extra: The chain of filters to be applied to the reference point cloud. The reference point cloud is processed into an intermediate point cloud with the given chain of filters to be used in the alignment procedure. The chain is organized with the forward traversal order of the point set filters range.
//
//         The chain of point set filters are applied only once at the beginning of the ICP procedure, i.e., before the first iteration of the ICP algorithm.
//
//         The filters can have several purposes, including but are not limited to i) removal of noisy points which render alignment of point clouds difficult, ii) removal of redundant points so as to speed up alignment, iii) addition of descriptive information to the points such as a surface normal vector, or the direction from the point to the sensor.
//         Extra: Corresponds to referenceDataPointsFilters configuration module of PointMatcher library. The filters should be chosen and set from possible components of the referenceDataPointsFilters configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     matcher
//
//         a method used for matching (linking) the points from point_set_2, to the points in the reference cloud, point_set_1
//         Type: a class model of ICP_config
//         Default: KDTreeMatcher
//         Extra: Corresponds to the matcher configuration module of PointMatcher library. The matcher should be chosen and set from possible components of the matcher configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     outlier_filters
//
//         a chain of filters to be applied to the matched (linked) point clouds after each processing iteration of the ICP algorithm to remove the links which do not correspond to true point correspondences
//         Type: a model of Range. The value type of its iterator must be ICP_config.
//         Default: TrimmedDistOutlierFilter
//         Extra: The outliers are rejected. Points with no link are ignored in the subsequent error minimization step. The chain is organized with the forward traversal order of the outlier filters range.
//         Extra: Corresponds to the outlierFilters configuration module of PointMatcher library. The filters should be chosen and set from possible components of the outlierFilters configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     error_minimizer
//
//         an error minimizer that computes a transformation matrix such as to minimize the error between the point sets
//         Type: a class model of ICP_config
//         Default: PointToPlaneErrorMinimizer
//         Extra: Corresponds to the errorMinimizer configuration module of PointMatcher library. The error minimizer should be chosen and set from possible components of the errorMinimizer configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     transformation_checkers
//
//         a chain of transformation checkers
//         Type: a class model of Range. The value type of its iterator must be ICP_config.
//         Default: CounterTransformationChecker and DifferentialTransformationChecker
//         Extra: The chain is organized with the forward traversal order of the transformation checkers range.
//         Extra: A transformation checker can stop the iteration depending on the conditions it defines.
//         Extra: Corresponds to the transformationCheckers configuration module of PointMatcher library. The transformation checkers should be chosen and set from possible components of the transformationCheckers configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     inspector
//
//         an inspector that enables logging data at different steps for analysis.
//         Type: a class model of ICP_config
//         Default: NullInspector
//         Extra: Inspectors typically provide deeper scrutiny than the logger.
//         Extra: Corresponds to the inspector configuration module of PointMatcher library. The inspector should be chosen and set from possible components of the inspector configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     logger
//
//         a method for logging information regarding the registration process outputted by PointMatcher library
//         Type: a class model of ICP_config
//         Default: NullLogger
//         Extra: The logs generated by CGAL library does not get effected by this configuration.
//         Extra: Corresponds to the logger configuration module of PointMatcher library. The logger should be chosen and set from possible components of the logger configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Parameters
//     np2	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set point_set_2
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange2 and whose value type is geom_traits::Vector_3
//
//     	
//
//     point_set_filters
//
//         a chain of filters to be applied to the point set
//         Type: a class model of Range. The value type of its iterator must be ICP_config.
//         Default: SamplingSurfaceNormalDataPointsFilter
//
//         Extra: The chain of filters to be applied to the point cloud point_set_2. The point cloud is processed into an intermediate point cloud with the given chain of filters to be used in the alignment procedure. The chain is organized with the forward traversal order of the point set filters range.
//
//         The chain of point set filters are applied only once at the beginning of the ICP procedure, i.e., before the first iteration of the ICP algorithm.
//
//         The filters can have several purposes, including but are not limited to i) removal of noisy points which render alignment of point clouds difficult, ii) removal of redundant points so as to speed up alignment, iii) addition of descriptive information to the points such as a surface normal vector, or the direction from the point to the sensor.
//         Extra: Corresponds to the readingDataPointsFilters configuration module of PointMatcher library. The filters should be chosen and set from possible components of the readingDataPointsFilters configuration module. See libpointmatcher documentation for possible configurations.
//
//     	
//
//     transformation
//
//         an affine transformation that is used as the initial transformation for point_set_2
//         Type: CGAL::Aff_transformation_3
//         Default: the identity transformation
//
//     	
//
// Returns
//     true if registration is converged, false otherwise. A log why it failed to converge is written to std::cerr if the registration cannot converge. 
//
// Examples
//     Point_set_processing_3/registration_with_opengr_pointmatcher_pipeline.cpp, and Point_set_processing_3/registration_with_pointmatcher.cpp.
//
// ◆ remove_outliers()
// template<typename ConcurrencyTag , typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// PointRange::iterator CGAL::remove_outliers 	( 	PointRange &  	points,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/remove_outliers.h>
//
// Removes outliers:
//
//     computes average squared distance to the nearest neighbors,
//     and partitions the points either using a threshold on the of average distance or selecting a fixed percentage of points with the highest average distances
//
// This method modifies the order of input points so as to pack all remaining points first, and returns an iterator over the first point to remove (see erase-remove idiom). For this reason it should not be called on sorted containers.
//
// Precondition
//     k >= 2
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     k	number of neighbors
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 0 (no limit)
//         Extra: If provided, the neighborhood of a query point is computed with a fixed spherical radius instead of a fixed number of neighbors. In that case, the parameter k is used as a limit on the number of points returned by each spherical query (to avoid overly large number of points in high density areas).
//
//     	
//
//     threshold_percent
//
//         the maximum percentage of points to remove
//         Type: double
//         Default: 10
//
//     	
//
//     threshold_distance
//
//         the minimum distance for a point to be considered as outlier
//         Type: double
//         Default: 0
//         Extra: Distance here is the square root of the average squared distance to K-nearest neighbors
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped, all points are left unchanged and the function return points.size().
//         Extra: The callback will be copied and therefore needs to be lightweight.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Returns
//     iterator over the first point to remove.
//
// Note
//     There are two thresholds that can be used: threshold_percent and threshold_distance. This function returns the smallest number of outliers such that at least one of these threshold is fulfilled. This means that if threshold_percent=100, only threshold_distance is taken into account; if threshold_distance=0 only threshold_percent is taken into account. 
//
// ◆ scanline_orient_normals()
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::scanline_orient_normals 	( 	PointRange &  	points,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/scanline_orient_normals.h>
//
// orients the normals of the range of points by estimating a line of sight and checking its consistency with the current normal orientation.
//
// Warning
//     This function requires the input points to be ordered along scanlines aligned on the XY-plane. It is typically designed for 2.5D urban datasets acquired through, for example, airborne LIDAR devices.
//
// First, scanlines are estimated as subranges of points by iterating on points:
//
//     if the named parameter scanline_id_map is provided, the range is cut every time the id changes.
//     if no scanline ID map is provided, a fallback method simply cuts the range every time 3 consecutive points form an acute angle on the projected XY-plane. This fallback method gives suboptimal results.
//
// Then, the line of sight (estimated vector between a point and the position of the scanner at its time of acquisition) is estimated:
//
//     if scan_angle is provided, the line of sight can be directly computed as a combination of the estimated scanline and of the scan angle.
//     if no scan angle map is provided, then for each scanline, the position of the scanner is estimated as being above of the barycenter of the points of the scanline projected on the XY-plane. This fallback method gives suboptimal results.
//
// Once the line of sight is estimated for each point, the normals are oriented by checking, for each of them, if the line of sight and the normal vector give a positive scalar product. If they don't, then the normal vector is inverted.
//
// Note
//     This method gives optimal results when scanline_id_map and scan_angle are provided. Correct results may still be produced in the absence of either one or both of these properties, as long as the point set is ordered in 2.5D scanlines.
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of WritablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     scan_angle_map
//
//         a property map associating the angle of acquisition (in degrees) to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is convertible to double
//
//     	
//
//     scanline_id_map
//
//         a property map associating a scanline ID to the elements of the point set points. A scanline is detected as a consecutive subrange of items in the input range point whose ID are identical. IDs do not need to be unique, they just need to be different for two consecutive scanlines. The LAS property scan_direction_flag (whose values are either 0 or 1 depending on the direction of the scanner) can be used.
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is a model of EqualityComparable
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Examples
//     Point_set_processing_3/orient_scanlines_example.cpp.
//
// ◆ structure_point_set()
// template<typename PointRange , typename PlaneRange , typename OutputIterator , typename NamedParameters >
// OutputIterator CGAL::structure_point_set 	( 	const PointRange &  	points,
// 		const PlaneRange &  	planes,
// 		OutputIterator  	output,
// 		double  	epsilon,
// 		const NamedParameters &  	np 
// 	) 		
//
// #include <CGAL/structure_point_set.h>
//
// This is an implementation of the Point Set Structuring algorithm.
//
// This algorithm takes advantage of a set of detected planes: it detects adjacency relationships between planes and resamples the detected planes, edges and corners to produce a structured point set.
//
// The size parameter epsilon is used both for detecting adjacencies and for setting the sampling density of the structured point set.
//
// For more details, please refer to [6].
//
// Template Parameters
//     PointRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter point_map.
//     PlaneRange	is a model of ConstRange. The value type of its iterator is the key type of the named parameter plane_map.
//     OutputIterator	Type of the output iterator. The type of the objects put in it is std::pair<Kernel::Point_3, Kernel::Vector_3>. Note that the user may use a function_output_iterator to match specific needs.
//
// Parameters
//     points	input point range
//     planes	input plane range.
//     output	output iterator where output points are written
//     epsilon	size parameter.
//     np	a sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     plane_index_map
//
//         a property map associating the index of a point in the input range to the index of plane (-1 if the point is not assigned to a plane)
//         Type: a class model of ReadablePropertyMap with std::size_t as key type and int as value type
//         Default: There is no default, this parameters is mandatory.
//
//     	
//
//     plane_map
//
//         a property map containing the planes associated to the elements of the plane range planes
//         Type: a class model of ReadablePropertyMap with PlaneRange::iterator::value_type as key type and geom_traits::Plane_3 as value type
//         Default: CGAL::Identity_property_map<Kernel::Plane_3>
//
//     	
//
//     attraction_factor
//
//         multiple of a tolerance epsilon used to connect simplices
//         Type: floating scalar value
//         Default: 3
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// Examples
//     Point_set_processing_3/structuring_example.cpp.
//
// ◆ vcm_estimate_normals() [1/2]
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::vcm_estimate_normals 	( 	PointRange &  	points,
// 		double  	offset_radius,
// 		double  	convolution_radius,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/vcm_estimate_normals.h>
//
// Estimates normal directions of the range of points using the Voronoi Covariance Measure with a radius for the convolution.
//
// The output normals are randomly oriented.
//
// See compute_vcm() for a detailed description of the parameters offset_radius and convolution_radius and of the Voronoi Covariance Measure.
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     offset_radius	offset_radius.
//     convolution_radius	convolution_radius.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     diagonalize_traits
//
//         the solver used for diagonalizing covariance matrices
//         Type: a class model of DiagonalizeTraits
//         Default: If Eigen 3 (or greater) is available and CGAL_EIGEN3_ENABLED is defined then an overload using Eigen_diagonalize_traits is provided. Otherwise, the internal implementation CGAL::Diagonalize_traits is used
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// ◆ vcm_estimate_normals() [2/2]
// template<typename PointRange , typename NamedParameters = parameters::Default_named_parameters>
// void CGAL::vcm_estimate_normals 	( 	PointRange &  	points,
// 		double  	offset_radius,
// 		unsigned int  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/vcm_estimate_normals.h>
//
// Estimates normal directions of the range of points using the Voronoi Covariance Measure with a number of neighbors for the convolution.
//
// The output normals are randomly oriented.
//
// See compute_vcm() for a detailed description of the parameter offset_radius and of the Voronoi Covariance Measure.
//
// Template Parameters
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//
// Parameters
//     points	input point range
//     offset_radius	offset_radius.
//     k	number of neighbor points used for convolution.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     diagonalize_traits
//
//         the solver used for diagonalizing covariance matrices
//         Type: a class model of DiagonalizeTraits
//         Default: If Eigen 3 (or greater) is available and CGAL_EIGEN3_ENABLED is defined then an overload using Eigen_diagonalize_traits is provided. Otherwise, the internal implementation CGAL::Diagonalize_traits is used
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
// ◆ vcm_is_on_feature_edge()
// template<class FT , class VCMTraits >
// bool CGAL::vcm_is_on_feature_edge 	( 	std::array< FT, 6 > &  	cov,
// 		double  	threshold,
// 		VCMTraits  	 
// 	) 		
//
// #include <CGAL/vcm_estimate_edges.h>
//
// determines if a point is on a sharp feature edge from a point set for which the Voronoi covariance Measures have been computed.
//
// The sharpness of the edge, specified by parameter threshold, is used to filtered points according to the external angle around a sharp feature.
//
// A point is considered to be on a sharp feature if the external angle alpha at the edge is such that alpha >= 2 / sqrt(3) * sqrt(threshold). In particular this means that if the input contains sharp features with different external angles, the one with the smallest external angle should be considered, which however would result in selecting more points on sharper regions. More details are provided in [9].
//
// Template Parameters
//     VCMTraits	is a model of DiagonalizeTraits. It can be omitted: if Eigen 3 (or greater) is available and CGAL_EIGEN3_ENABLED is defined then an overload using Eigen_diagonalize_traits is provided. Otherwise, the internal implementation Diagonalize_traits is used.
//
// See also
//     CGAL::compute_vcm() 
//
// Examples
//     Point_set_processing_3/edges_example.cpp.
//
// ◆ wlop_simplify_and_regularize_point_set()
// template<typename ConcurrencyTag , typename PointRange , typename OutputIterator , typename NamedParameters = parameters::Default_named_parameters>
// OutputIterator CGAL::wlop_simplify_and_regularize_point_set 	( 	PointRange &  	points,
// 		OutputIterator  	output,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// #include <CGAL/wlop_simplify_and_regularize_point_set.h>
//
// This is an implementation of the Weighted Locally Optimal Projection (WLOP) simplification algorithm.
//
// The WLOP simplification algorithm can produce a set of denoised, outlier-free and evenly distributed particles over the original dense point cloud. The core of the algorithm is a Weighted Locally Optimal Projection operator with a density uniformization term. For more details, please refer to [4].
//
// A parallel version of WLOP is provided and requires the executable to be linked against the Intel TBB library. To control the number of threads used, the user may use the tbb::task_scheduler_init class. See the TBB documentation for more details.
//
// Template Parameters
//     ConcurrencyTag	enables sequential versus parallel algorithm. Possible values are Sequential_tag, Parallel_tag, and Parallel_if_available_tag.
//     PointRange	is a model of Range. The value type of its iterator is the key type of the named parameter point_map.
//     OutputIterator	Type of the output iterator. It must accept objects of type geom_traits::Point_3.
//
// Parameters
//     points	input point range
//     output	iterator where output points are put.
//     np	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Point_3
//         Default: CGAL::Identity_property_map<geom_traits::Point_3>
//
//     	
//
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadWritePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is geom_traits::Vector_3
//
//     	
//
//     select_percentage
//
//         percentage of points to retain
//         Type: floating scalar value
//         Default: 5
//
//     	
//
//     neighbor_radius
//
//         the spherical neighborhood radius
//         Type: floating scalar value
//         Default: 8 times the average spacing of the point set
//         Extra: This is a key parameter that needs to be finely tuned. The result will be irregular if too small, but a larger value will impact the runtime. In practice, choosing a radius such that the neighborhood of each sample point includes at least two rings of neighboring sample points gives satisfactory result.
//
//     	
//
//     number_of_iterations
//
//         number of iterations to solve the optimsation problem
//         Type: unsigned int
//         Default: 35
//         Extra: More iterations give a more regular result but increase the runtime
//
//     	
//
//     require_uniform_sampling
//
//         If true, an optional preprocessing is applied, which will give better results if the distribution of the input points is highly non-uniform.
//         Type: Boolean
//         Default: false
//
//     	
//
//     callback
//
//         a mechanism to get feedback on the advancement of the algorithm while it's running and to interrupt it if needed
//         Type: an instance of std::function<bool(double)>.
//         Default: unused
//         Extra: It is called regularly when the algorithm is running: the current advancement (between 0. and 1.) is passed as parameter. If it returns true, then the algorithm continues its execution normally; if it returns false, the algorithm is stopped, no output points are generated.
//         Extra: The callback will be copied and therefore needs to be lightweight.
//         Extra: When CGAL::Parallel_tag is used, the callback mechanism is called asynchronously on a separate thread and shouldn't access or modify the variables that are parameters of the algorithm.
//
//     	
//
//     geom_traits
//
//         an instance of a geometric traits class
//         Type: a model of Kernel
//         Default: a CGAL Kernel deduced from the point type, using CGAL::Kernel_traits
//
//     	
//
