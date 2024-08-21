#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Kinetic_surface_reconstruction_3.h>
#include <CGAL/KSR_3/Graphcut.h>

#include "CGALPY/internal.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

void export_kinetic_surface_reconstruction(py::module_& m) {
  using Kernel_ = Kernel;
  using Intersection_kernel = CGAL::Exact_predicates_exact_constructions_kernel; // default KSR kernel
  using FT = typename Kernel_::FT;
  using Point_3 = typename Kernel_::Point_3;
  using Plane_3 = typename Kernel_::Plane_3;
  using Point_range = CGAL::Point_set_3<Point_3>;
  using Point_map    = typename Point_range::Point_map;
  using Normal_map   = typename Point_range::Vector_map;
  using KSP = CGAL::Kinetic_space_partition_3<Kernel, Intersection_kernel>;
  using KSR = CGAL::Kinetic_surface_reconstruction_3<Kernel, Point_range, Point_map, Normal_map>;
  using Graphcut = CGAL::KSR_3::Graphcut<KSP>;


  py::class_<KSR> ksr(m, "Kinetic_surface_reconstruction_3",
    "Pipeline for piecewise planar surface reconstruction from a point cloud via inside/outside labeling of a kinetic partition using min-cut.\n");
  ksr.def("__init__", [](KSR& ksr, Point_range& points, const py::dict& np = py::dict()) {
    // return a new instance of KSR
    new (&ksr) KSR(points, internal::parse_named_parameters(np));
  },
    py::arg("points"), py::arg("np") = py::dict(),
    "creates a Kinetic_shape_reconstruction_3 object.\n")
    .def("detect_planar_shapes", [](KSR& ksr, const py::dict& np = py::dict()) {
      return ksr.detect_planar_shapes(internal::parse_named_parameters(np));
    },
      py::arg("np") = py::dict(),
      "detects shapes in the provided point cloud and regularizes them.\n")
    .def("detected_planar_shapes", [](KSR& ksr) {
      return ksr.detected_planar_shapes();
    },
      "returns the support planes of the detected and regularized shapes.\n")
    .def("detected_planar_shape_indices", [](KSR& ksr) {
      return ksr.detected_planar_shape_indices();
    },
      "returns the indices of detected and regularized shapes.\n")
    .def("detection_and_partition", [](KSR& ksr, std::size_t k, const py::dict& np = py::dict()) {
      return ksr.detection_and_partition(k, internal::parse_named_parameters(np));
    },
     py::arg("k"), py::arg("np") = py::dict(),
     "detects and regularizes shapes in the provided point cloud and creates the kinetic space partition.\n"
     "Combines calls of detect_planar_shapes(), initialize_partition() and partition().\n")
    .def("initialize_partition", [](KSR& ksr, const py::dict& np = py::dict()) {
      return ksr.initialize_partition(internal::parse_named_parameters(np));
    },
      py::arg("np") = py::dict(),
      "initializes the kinetic partition.\n")
    // .def("kinetic_partition", [](const KSR& ksr) { // deprecated?
    //   return ksr.kinetic_partition();
    // },
    // "gives access to the kinetic partition.\n")
    .def("partition", [](KSR& ksr, std::size_t k) {
      return ksr.partition(k);
    },
      py::arg("k"),
      "propagates the kinetic polygons in the initialized partition.\n")
    .def("reconstruct", [](KSR& ksr, FT lambda, std::map<KSP::Face_support, bool> external_nodes) {
         std::vector<std::vector<std::size_t>> polys;
         std::vector<Point_3> vertices;
         ksr.reconstruct(lambda, external_nodes, std::back_inserter(vertices), std::back_inserter(polys));
         return std::make_tuple(vertices, polys);
    },
         py::arg("lambda"), py::arg("external_nodes"),
         "uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.\n"
         "The external_nodes parameter allows to indicate the preferred labels for faces on the bounding box.\n")
    .def("reconstruct_with_ground", [](KSR& ksr, FT lambda) {
         std::vector<std::vector<std::size_t>> polys;
         std::vector<Point_3> vertices;
         ksr.reconstruct_with_ground(lambda, std::back_inserter(vertices), std::back_inserter(polys));
         return std::make_tuple(vertices, polys);
    },
      py::arg("lambda"),
      "uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.\n"
      "Estimates a horizontal ground plane within the detected shapes. Cells in the partition below the ground plane receive a weight to be labeled as inside. The z axis is considered as vertical upwards pointing.\n")
    ;
}

// Definition
// template<typename GeomTraits, typename PointRange, typename PointMap, typename NormalMap, typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// class CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >
//
// Pipeline for piecewise planar surface reconstruction from a point cloud via inside/outside labeling of a kinetic partition using min-cut.
//
// Template Parameters
//     GeomTraits	must be a model of KineticShapePartitionTraits_3.
//     PointRange	must be a range of 3D points and corresponding 3D normal vectors whose iterator type is RandomAccessIterator.
//     PointMap	a model of ReadablePropertyMap whose key type is the value type of the PointRange and value type is GeomTraits::Point_3
//     NormalMap	a model of ReadablePropertyMap whose key type is the value type of the PointRange and value type is GeomTraits::Vector_3
//     IntersectionKernel	must be a model of Kernel using exact computations. Defaults to CGAL::Exact_predicates_exact_constructions_kernel. Used for the internal kinetic shape partition.
//
// Examples
//     Kinetic_surface_reconstruction/ksr_basic.cpp, Kinetic_surface_reconstruction/ksr_building.cpp, and Kinetic_surface_reconstruction/ksr_parameters.cpp.
//
// Public Types
// using 	Kernel = GeomTraits
//  
// using 	Intersection_kernel = IntersectionKernel
//  
// using 	FT = typename Kernel::FT
//  
// using 	Point_3 = typename Kernel::Point_3
//  
// using 	Plane_3 = typename Kernel::Plane_3
//  
// using 	Point_range = PointRange
//  
// using 	KSP = Kinetic_space_partition_3< Kernel, Intersection_kernel >
//  
// using 	Point_map = PointMap
//  
// using 	Normal_map = NormalMap
//  
// Public Member Functions
// template<typename NamedParameters = parameters::Default_named_parameters>
//  	Kinetic_surface_reconstruction_3 (Point_range &points, const NamedParameters &np=CGAL::parameters::default_values())
//  	creates a Kinetic_shape_reconstruction_3 object.
//  
// template<typename NamedParameters = CGAL::parameters::Default_named_parameters>
// std::size_t 	detect_planar_shapes (const NamedParameters &np=parameters::default_values())
//  	detects shapes in the provided point cloud and regularizes them.
//  
// const std::vector< Plane_3 > & 	detected_planar_shapes ()
//  	returns the support planes of the detected and regularized shapes.
//  
// const std::vector< std::vector< std::size_t > > & 	detected_planar_shape_indices ()
//  	returns the indices of detected and regularized shapes.
//  
// template<typename NamedParameters = CGAL::parameters::Default_named_parameters>
// void 	detection_and_partition (std::size_t k, const NamedParameters &np=parameters::default_values())
//  	detects and regularizes shapes in the provided point cloud and creates the kinetic space partition.
//  
// template<typename NamedParameters = CGAL::parameters::Default_named_parameters>
// void 	initialize_partition (const NamedParameters &np=parameters::default_values())
//  	initializes the kinetic partition.
//  
// void 	partition (std::size_t k)
//  	propagates the kinetic polygons in the initialized partition.
//  
// const KSP & 	kinetic_partition () const
//  	gives access to the kinetic partition.
//  
// template<class OutputPointIterator , class OutputPolygonIterator >
// void 	reconstruct_with_ground (FT lambda, OutputPointIterator pit, OutputPolygonIterator polyit)
//  	uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.
//  
// template<class OutputPointIterator , class OutputPolygonIterator >
// void 	reconstruct (FT lambda, std::map< typename KSP::Face_support, bool > external_nodes, OutputPointIterator pit, OutputPolygonIterator polyit)
//  	uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.
//  
// Constructor & Destructor Documentation
// ◆ Kinetic_surface_reconstruction_3()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// template<typename NamedParameters = parameters::Default_named_parameters>
// CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::Kinetic_surface_reconstruction_3 	( 	Point_range &  	points,
// 		const NamedParameters &  	np = CGAL::parameters::default_values() 
// 	) 		
//
// creates a Kinetic_shape_reconstruction_3 object.
//
// Parameters
//     points	an instance of PointRange with 3D points and corresponding 3D normal vectors.
//     np	a sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is GeomTraits::Point_3
//         Default: PointMap()
//
//     	
//
// Member Function Documentation
// ◆ detect_planar_shapes()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// template<typename NamedParameters = CGAL::parameters::Default_named_parameters>
// std::size_t CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::detect_planar_shapes 	( 	const NamedParameters &  	np = parameters::default_values()	) 	
//
// detects shapes in the provided point cloud and regularizes them.
//
// Template Parameters
//     NamedParameters	a sequence of Named Parameters
//
// Parameters
//     np	an instance of NamedParameters.
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is GeomTraits::Point_3
//         Default: PointMap()
//
//     	
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is GeomTraits::Vector_3
//         Default: NormalMap()
//
//     k_neighbors
//
//         Shape detection: the number of neighbors for each point considered during region growing
//         Type: std::size_t
//         Default: 12
//
//     	
//     maximum_distance
//
//         Shape detection: the maximum distance from a point to a plane
//         Type: GeomTraits::FT
//         Default: 2% of bounding box diagonal
//
//     	
//     maximum_angle
//
//         Shape detection: maximum angle in degrees between the normal of a point and the plane normal
//         Type: GeomTraits::FT
//         Default: 15 degrees
//
//     	
//     minimum_region_size
//
//         Shape detection: minimum number of 3D points a region must have
//         Type: std::size_t
//         Default: 1% of input points
//
//     	
//     angle_tolerance
//
//         Shape regularization: maximum allowed angle in degrees between plane normals used for parallelism, orthogonality, and axis symmetry
//         Type: GeomTraits::FT
//         Default: 5 degrees
//
//     	
//     maximum_offset
//
//         Shape regularization: maximum allowed orthogonal distance between two parallel planes such that they are considered to be coplanar
//         Type: GeomTraits::FT
//         Default: 0.5% of bounding box diagonal
//
//     	
//     regularize_parallelism
//
//         Shape regularization: indicates whether parallelism should be regularized or not
//         Type: boolean
//         Default: false
//
//     	
//     regularize_orthogonality
//
//         Shape regularization: indicates whether orthogonality should be regularized or not
//         Type: boolean
//         Default: false
//
//     	
//     regularize_coplanarity
//
//         Shape regularization: indicates whether coplanarity should be regularized or not
//         Type: boolean
//         Default: true
//
//     	
//     regularize_axis_symmetry
//
//         Shape regularization: indicates whether axis symmetry should be regularized or not
//         Type: boolean
//         Default: false
//
//     	
//     symmetry_direction
//
//         Shape regularization: an axis for symmetry regularization
//         Type: GeomTraits::Vector_3
//         Default: Z axis that is GeomTraits::Vector_3(0, 0, 1)
//
//     	
//
// ◆ detected_planar_shape_indices()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// const std::vector< std::vector< std::size_t > > & CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::detected_planar_shape_indices 	( 		) 	
//
// returns the indices of detected and regularized shapes.
//
// Returns
//     indices into points for each detected planar shape.
//
// Precondition
//     shape detection performed 
//
// ◆ detected_planar_shapes()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// const std::vector< Plane_3 > & CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::detected_planar_shapes 	( 		) 	
//
// returns the support planes of the detected and regularized shapes.
//
// Returns
//     vector with a Plane_3 for each detected planar shape.
//
// Precondition
//     shape detection performed 
//
// ◆ detection_and_partition()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// template<typename NamedParameters = CGAL::parameters::Default_named_parameters>
// void CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::detection_and_partition 	( 	std::size_t  	k,
// 		const NamedParameters &  	np = parameters::default_values() 
// 	) 		
//
// detects and regularizes shapes in the provided point cloud and creates the kinetic space partition.
//
// Combines calls of detect_planar_shapes(), initialize_partition() and partition().
//
// Template Parameters
//     NamedParameters	a sequence of Named Parameters
//
// Parameters
//     k	maximum number of allowed intersections for each input polygon before its expansion stops.
//     np	an instance of NamedParameters.
//
// Optional Named Parameters
//     point_map
//
//         a property map associating points to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is GeomTraits::Point_3
//         Default: PointMap()
//
//     	
//     normal_map
//
//         a property map associating normals to the elements of the point set points
//         Type: a model of ReadablePropertyMap whose key type is the value type of the iterator of PointRange and whose value type is GeomTraits::Vector_3
//         Default: NormalMap()
//
//     k_neighbors
//
//         Shape detection: the number of neighbors for each point considered during region growing
//         Type: std::size_t
//         Default: 12
//
//     	
//     maximum_distance
//
//         Shape detection: the maximum distance from a point to a plane
//         Type: GeomTraits::FT
//         Default: 2% of bounding box diagonal
//
//     	
//     maximum_angle
//
//         Shape detection: maximum angle in degrees between the normal of a point and the plane normal
//         Type: GeomTraits::FT
//         Default: 15 degrees
//
//     	
//     minimum_region_size
//
//         Shape detection: minimum number of 3D points a region must have
//         Type: std::size_t
//         Default: 1% of input points
//
//     	
//     angle_tolerance
//
//         Shape regularization: maximum allowed angle in degrees between plane normals used for parallelism, orthogonality, and axis symmetry
//         Type: GeomTraits::FT
//         Default: 5 degrees
//
//     	
//     maximum_offset
//
//         Shape regularization: maximum allowed orthogonal distance between two parallel planes such that they are considered to be coplanar
//         Type: GeomTraits::FT
//         Default: 0.5% of bounding box diagonal
//
//     	
//     regularize_parallelism
//
//         Shape regularization: indicates whether parallelism should be regularized or not
//         Type: boolean
//         Default: false
//
//     	
//     regularize_orthogonality
//
//         Shape regularization: indicates whether orthogonality should be regularized or not
//         Type: boolean
//         Default: false
//
//     	
//     regularize_coplanarity
//
//         Shape regularization: indicates whether coplanarity should be regularized or not
//         Type: boolean
//         Default: true
//
//     	
//     regularize_axis_symmetry
//
//         Shape regularization: indicates whether axis symmetry should be regularized or not
//         Type: boolean
//         Default: false
//
//     	
//     symmetry_direction
//
//         Shape regularization: an axis for symmetry regularization
//         Type: GeomTraits::Vector_3
//         Default: Z axis that is GeomTraits::Vector_3(0, 0, 1)
//
//     	
//     reorient_bbox
//
//         Use the oriented bounding box instead of the axis-aligned bounding box.
//         Type: bool
//         Default: false
//
//     	
//     bbox_dilation_ratio
//
//         Factor for extension of the bounding box of the input data to be used for the partition.
//         Type: FT
//         Default: 1.1
//
//     	
//
// ◆ initialize_partition()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// template<typename NamedParameters = CGAL::parameters::Default_named_parameters>
// void CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::initialize_partition 	( 	const NamedParameters &  	np = parameters::default_values()	) 	
//
// initializes the kinetic partition.
//
// Parameters
//     np	a sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     reorient_bbox
//
//         Use the oriented bounding box instead of the axis-aligned bounding box.
//         Type: bool
//         Default: false
//
//     	
//     bbox_dilation_ratio
//
//         Factor for extension of the bounding box of the input data to be used for the partition.
//         Type: FT
//         Default: 1.1
//
//     	
//
// Precondition
//     shape detection performed 
//
// ◆ kinetic_partition()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// const KSP & CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::kinetic_partition 	( 		) 	const
//
// gives access to the kinetic partition.
//
// Returns
//     created kinetic partition data structure
//
// Precondition
//     partition created 
//
// ◆ partition()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// void CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::partition 	( 	std::size_t  	k	) 	
//
// propagates the kinetic polygons in the initialized partition.
//
// Parameters
//     k	maximum number of allowed intersections for each input polygon before its expansion stops.
//
// Precondition
//     partition initialized 
//
// ◆ reconstruct()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// template<class OutputPointIterator , class OutputPolygonIterator >
// void CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::reconstruct 	( 	FT  	lambda,
// 		std::map< typename KSP::Face_support, bool >  	external_nodes,
// 		OutputPointIterator  	pit,
// 		OutputPolygonIterator  	polyit 
// 	) 		
//
// uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.
//
// The external_nodes parameter allows to indicate the preferred labels for faces on the bounding box.
//
// Template Parameters
//     OutputPointIterator	an output iterator taking Point_3.
//     OutputPolygonIterator	an output iterator taking polygon indices std::vector<std::size_t>.
//
// Parameters
//     lambda	trades data faithfulness of the reconstruction for low complexity. Must be in the range [0, 1).
//     external_nodes	adds label preference for the faces on the bounding box. Bounding box sides without preset label are chosen by the min-cut. Setting external_nodes[ZMIN] = true sets the inside label as the preferred label for the ZMIN side of the bounding box.
//     pit	output iterator to receive the vertices of the reconstructed surface.
//     polyit	output iterator to store all polygonal faces of the reconstructed surface.
//
// Precondition
//     partition created 
//
// ◆ reconstruct_with_ground()
// template<typename GeomTraits , typename PointRange , typename PointMap , typename NormalMap , typename IntersectionKernel = CGAL::Exact_predicates_exact_constructions_kernel>
// template<class OutputPointIterator , class OutputPolygonIterator >
// void CGAL::Kinetic_surface_reconstruction_3< GeomTraits, PointRange, PointMap, NormalMap, IntersectionKernel >::reconstruct_with_ground 	( 	FT  	lambda,
// 		OutputPointIterator  	pit,
// 		OutputPolygonIterator  	polyit 
// 	) 		
//
// uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.
//
// Estimates a horizontal ground plane within the detected shapes. Cells in the partition below the ground plane receive a weight to be labeled as inside. The z axis is considered as vertical upwards pointing.
//
// Template Parameters
//     OutputPointIterator	an output iterator taking Point_3.
//     OutputPolygonIterator	an output iterator taking polygon indices std::vector<std::size_t>.
//
// Parameters
//     lambda	trades data faithfulness of the reconstruction for low complexity. Must be in the range [0, 1).
//     pit	output iterator to receive the vertices of the reconstructed surface.
//     polyit	output iterator to store all polygonal faces of the reconstructed surface.
//
// Precondition
//     partition created 
//
