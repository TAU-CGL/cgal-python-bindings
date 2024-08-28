#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>

#include <CGAL/mesh_segmentation.h>

#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace segment {

template <typename Tm, typename SDFPropertyMap, typename PointPropertyMap = typename boost::property_map<Tm, boost::vertex_point_t>::type>
auto sdf_values(const Tm& pm, const SDFPropertyMap& sdf_values_map, double cone_angle = 2.0 / 3.0 * CGAL_PI,
                std::size_t number_of_rays = 25, bool postprocess = true,
                std::optional<PointPropertyMap> ppmap = std::optional<PointPropertyMap>()) {
    if (ppmap) {
      return CGAL::sdf_values(pm, sdf_values_map, cone_angle, number_of_rays, postprocess, *ppmap);
    }
    else {
      return CGAL::sdf_values(pm, sdf_values_map, cone_angle, number_of_rays, postprocess);
    }
}

template <typename Tm, typename SDFPropertyMap, typename SegmentPropertyMap,
        typename PointPropertyMap = typename boost::property_map<Tm, boost::vertex_point_t>::type>
auto segmentation_from_sdf_values(const Tm& pm, const SDFPropertyMap& sdf_values_map, SegmentPropertyMap segment_ids,
                                  std::size_t number_of_clusters = 5, double smoothing_lambda = 0.26,
                                  bool output_cluster_ids = false,
                                  std::optional<PointPropertyMap> ppmap = std::optional<PointPropertyMap>()) {
    if (ppmap) {
      return CGAL::segmentation_from_sdf_values(pm, sdf_values_map, segment_ids, number_of_clusters, smoothing_lambda,
                                                output_cluster_ids, *ppmap);
    }
    else {
      return CGAL::segmentation_from_sdf_values(pm, sdf_values_map, segment_ids, number_of_clusters, smoothing_lambda,
                                                output_cluster_ids);
    }
}

template <typename Tm, typename SegmentPropertyMap,
        typename PointPropertyMap = typename boost::property_map<Tm, boost::vertex_point_t>::type>
auto segmentation_via_sdf_values(const Tm& pm, SegmentPropertyMap segment_ids,
                                 double cone_angle = 2.0 / 3.0 * CGAL_PI,
                                 std::size_t number_of_rays = 25, std::size_t number_of_clusters = 5,
                                 double smoothing_lambda = 0.26, bool output_cluster_ids = false,
                                 std::optional<PointPropertyMap> ppmap = std::optional<PointPropertyMap>()) {
    if (ppmap) {
      return CGAL::segmentation_via_sdf_values(pm, segment_ids, cone_angle, number_of_rays, number_of_clusters,
                                               smoothing_lambda, output_cluster_ids, *ppmap);
    }
    else {
      return CGAL::segmentation_via_sdf_values(pm, segment_ids, cone_angle, number_of_rays, number_of_clusters,
                                               smoothing_lambda, output_cluster_ids);
    }
}

} // namespace segment

void export_triangulated_surface_mesh_segmentation(py::module_& m) {
  using Tm = pmp::Polygonal_mesh;
  using Pm = Tm;
  using Kernel_ = Kernel;
  using Point_3 = typename Kernel_::Point_3;
  using Gt = boost::graph_traits<Pm>;
  using Vd = typename Gt::vertex_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Fd = typename Gt::face_descriptor;
  using Ed = typename Gt::edge_descriptor;
  using faces_size_type = typename Gt::faces_size_type;

#if CGALPY_PMP_POLYGONAL_MESH == 1
  using edge_bool_map = Pm::Property_map<Ed, bool>;
  using VertexPointMap = Pm::Property_map<Vd, Point_3>;
  using FaceDoubleMap = Pm::Property_map<Fd, double>;
  using FaceSizeTMap = Pm::Property_map<Fd, std::size_t>;
#endif
#if CGALPY_PMP_POLYGONAL_MESH == 0
  using edge_bool_map = boost::property_map<Pm, CGAL::dynamic_edge_property_t<bool>>::type;
  using VertexPointMap = boost::property_map<Pm, CGAL::dynamic_vertex_property_t<Point_3>>::type;
  using FaceDoubleMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<double>>::type;
  using FaceSizeTMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>::type;
#endif
  using SDFPropertyMap = FaceDoubleMap;
  using PointPropertyMap = VertexPointMap;
  using SegmentPropertyMap = FaceSizeTMap;

  m.def("sdf_values", &segment::sdf_values<Pm, SDFPropertyMap, PointPropertyMap>,
        py::arg("triangle_mesh"), py::arg("sdf_values_map"), py::arg("cone_angle") = 2.0 / 3.0 * CGAL_PI,
        py::arg("number_of_rays") = 25, py::arg("postprocess") = true,
        py::arg("ppmap") = std::optional<PointPropertyMap>(),
        "Function computing the Shape Diameter Function over a surface mesh.\n"
        "This function implements the Shape Diameter Function (SDF) as described in [4]. It is possible to compute raw SDF values (without post-processing). In such a case, -1 is used to indicate when no SDF value could be computed for a facet.\n"
        "\n"
        "Precondition\n"
        "• is_triangle_mesh(triangle_mesh)\n"
        "\n"
        "Parameters\n"
        "• triangle_mesh: surface mesh on which SDF values are computed\n"
        "• sdf_values_map: the SDF value of each facet\n"
        "• cone_angle: opening angle in radians for the cone of each facet\n"
        "• number_of_rays: number of rays picked in the cone of each facet. In our experiments, we observe that increasing the number of rays beyond the default has little effect on the quality of the segmentation result\n"
        "• postprocess: if true, CGALPY.sdf_values_postprocessing() is called on raw SDF value computed.\n"
        "• ppmap: point property map.\n"
        "Returns\n"
        "• minimum and maximum raw SDF values if postprocess is true, otherwise minimum and maximum SDF values (before linear normalization) \n"
        "Examples\n"
        "Surface_mesh_segmentation/extract_segmentation_into_mesh_example.py, Surface_mesh_segmentation/sdf_values_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_SM_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_example.py, and Surface_mesh_segmentation/segmentation_with_facet_ids_example.py.\n");

  m.def("sdf_values_postprocessing", &CGAL::sdf_values_postprocessing<Pm, SDFPropertyMap>,
        py::arg("triangle_mesh"), py::arg("sdf_values_map"),
        "Function post-processing raw SDF values computed per facet.\n"
        "Post-processing steps applied :\n"
        "\n"
        "• Facets with -1 SDF values are assigned the average SDF value of their edge-adjacent neighbors. If there is still a facet having -1 SDF value, the minimum valid SDF value assigned to it. Note that this step is not inherited from the paper. The main reason for not assigning 0 to facets with no SDF values (i.e. -1) is that it can obstruct log-normalization process which takes place at the beginning of CGAL::segmentation_from_sdf_values().\n"
        "• SDF values are smoothed with bilateral filtering.\n"
        "• SDF values are linearly normalized between [0,1].\n"
        "\n"
        "See the section Post-processing of Raw SDF Values for more details.\n"
        "\n"
        "Precondition\n"
        "• is_triangle_mesh(triangle_mesh) \n"
        "• Raw values should be greater or equal to 0. -1 indicates when no value could be computed\n"
        "\n"
        "Parameters\n"
        "• triangle_mesh: surface mesh on which SDF values are computed\n"
        "• sdf_values_map: the SDF value of each facet\n"
        "\n"
        "Returns\n"
        "• minimum and maximum SDF values before linear normalization \n"
        "Examples\n"
        "Surface_mesh_segmentation/extract_segmentation_into_mesh_example.py, Surface_mesh_segmentation/sdf_values_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_SM_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_example.py, and Surface_mesh_segmentation/segmentation_with_facet_ids_example.py.\n");

  m.def("segmentation_from_sdf_values", &segment::segmentation_from_sdf_values<Tm, SDFPropertyMap, SegmentPropertyMap, PointPropertyMap>,
        py::arg("triangle_mesh"), py::arg("sdf_values_map"), py::arg("segment_ids"),
        py::arg("number_of_clusters") = 5, py::arg("smoothing_lambda") = 0.26,
        py::arg("output_cluster_ids") = false, py::arg("ppmap") = std::optional<PointPropertyMap>(),
        "Function computing the segmentation of a surface mesh given an SDF value per facet.\n"
        "This function fills a property map which associates a segment-id (in [0, number of segments -1]) or a cluster-id (in [0, number_of_clusters -1]) to each facet. A segment is a set of connected facets which are placed under the same cluster (see Figure 73.5).\n"
        "\n"
        "Note\n"
        "• Log-normalization is applied on sdf_values_map before segmentation. As described in the original paper [4], this normalization is done to preserve thin parts of the mesh by increasing the distance between smaller SDF values and reducing it between larger ones. \n"
        "• There is no direct relation between the parameter number_of_clusters and the final number of segments after segmentation. However, setting a large number of clusters will result in a detailed segmentation of the mesh with a large number of segments.\n"
        "\n"
        "Precondition\n"
        "• is_triangle_mesh(triangle_mesh) \n"
        "• number_of_clusters > 0\n"
        "Parameters\n"
        "• triangle_mesh: surface mesh corresponding to the SDF values\n"
        "• sdf_values_map: the SDF value of each facet between [0-1]\n"
        "• segment_ids: the segment or cluster id of each facet\n"
        "• number_of_clusters: number of clusters for the soft clustering\n"
        "• smoothing_lambda: factor which indicates the importance of the surface features for the energy minimization. It is recommended to choose a value in the interval [0,1]. See the section Hard clustering for more details.\n"
        "• output_cluster_ids: if false fill segment_ids with segment-ids, and with cluster-ids otherwise (see Figure 73.5)\n"
        "• ppmap: point property map.\n"
        "Returns\n"
        "• number of segments if output_cluster_ids is set to false and number_of_clusters otherwise \n"
        "Examples\n"
        "Surface_mesh_segmentation/extract_segmentation_into_mesh_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_SM_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_example.py, and Surface_mesh_segmentation/segmentation_with_facet_ids_example.py.\n");

  m.def("segmentation_via_sdf_values", &segment::segmentation_via_sdf_values<Pm, SegmentPropertyMap, PointPropertyMap>,
        py::arg("triangle_mesh"), py::arg("segment_ids"),
        py::arg("cone_angle") = 2.0 / 3.0 * CGAL_PI, py::arg("number_of_rays") = 25,
        py::arg("number_of_clusters") = 5, py::arg("smoothing_lambda") = 0.26,
        py::arg("output_cluster_ids") = false, py::arg("ppmap") = std::optional<PointPropertyMap>(),
        "Function computing the segmentation of a surface mesh\n"
        "This function is equivalent to calling the functions CGALPY.sdf_values() and CGALPY.segmentation_from_sdf_values() with the same parameters.\n"
        "\n"
        "Note\n"
        "• There is no direct relation between the parameter number_of_clusters and the final number of segments after segmentation. However, setting a large number of clusters will result in a detailed segmentation of the mesh with a large number of segments. \n"
        "• For computing segmentations of the mesh with different parameters (i.e. number of levels, and smoothing lambda), it is more efficient to first compute the SDF values using CGALPY.sdf_values() and use them in different calls to CGALPY.segmentation_from_sdf_values().\n"
        "\n"
        "Precondition\n"
        "• is_triangle_mesh(triangle_mesh) \n"
        "• number_of_clusters > 0\n"
        "\n"
        "Parameters\n"
        "• triangle_mesh: surface mesh corresponding to the SDF values\n"
        "• segment_ids: the segment or cluster id of each facet\n"
        "• cone_angle: opening angle in radians for the cone of each facet\n"
        "• number_of_rays: number of rays picked in the cone of each facet. In our experiments, we observe that increasing the number of rays beyond the default has a little effect on the quality of the segmentation result\n"
        "• number_of_clusters: number of clusters for the soft clustering\n"
        "• smoothing_lambda: factor which indicates the importance of the surface features for the energy minimization. It is recommended to choose a value in the interval [0,1]. See the section Hard clustering for more details.\n"
        "• output_cluster_ids: if false fill segment_ids with segment-ids, and with cluster-ids otherwise (see Figure 73.5)\n"
        "• ppmap: point property map.\n"
        "\n"
        "Returns\n"
        "• number of segments if output_cluster_ids is set to false and number_of_clusters otherwise\n"
        "\n"
        "Examples\n"
        "Surface_mesh_segmentation/segmentation_via_sdf_values_example.py.\n");

}

// Functions
// template<class TriangleMesh , class SDFPropertyMap , class PointPropertyMap = typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type, class GeomTraits = typename Kernel_traits<typename boost::property_traits<PointPropertyMap>::value_type>::Kernel>
// std::pair< double, double > 	CGAL::sdf_values (const TriangleMesh &triangle_mesh, SDFPropertyMap sdf_values_map, double cone_angle=2.0/3.0 *CGAL_PI, std::size_t number_of_rays=25, bool postprocess=true, PointPropertyMap ppmap=PointPropertyMap(), GeomTraits traits=GeomTraits())
//  	Function computing the Shape Diameter Function over a surface mesh.
//  
// template<class TriangleMesh , class SDFPropertyMap >
// std::pair< double, double > 	CGAL::sdf_values_postprocessing (const TriangleMesh &triangle_mesh, SDFPropertyMap sdf_values_map)
//  	Function post-processing raw SDF values computed per facet.
//  
// template<class TriangleMesh , class SDFPropertyMap , class SegmentPropertyMap , class PointPropertyMap = typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type, class GeomTraits = typename Kernel_traits<typename boost::property_traits<PointPropertyMap>::value_type>::Kernel>
// std::size_t 	CGAL::segmentation_from_sdf_values (const TriangleMesh &triangle_mesh, SDFPropertyMap sdf_values_map, SegmentPropertyMap segment_ids, std::size_t number_of_clusters=5, double smoothing_lambda=0.26, bool output_cluster_ids=false, PointPropertyMap ppmap=PointPropertyMap(), GeomTraits traits=GeomTraits())
//  	Function computing the segmentation of a surface mesh given an SDF value per facet.
//  
// template<class TriangleMesh , class SegmentPropertyMap , class PointPropertyMap = typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type, class GeomTraits = typename Kernel_traits<typename boost::property_traits<PointPropertyMap>::value_type>::Kernel>
// std::size_t 	CGAL::segmentation_via_sdf_values (const TriangleMesh &triangle_mesh, SegmentPropertyMap segment_ids, double cone_angle=2.0/3.0 *CGAL_PI, std::size_t number_of_rays=25, std::size_t number_of_clusters=5, double smoothing_lambda=0.26, bool output_cluster_ids=false, PointPropertyMap ppmap=PointPropertyMap(), GeomTraits traits=GeomTraits())
//  	Function computing the segmentation of a surface mesh.
//  
// Function Documentation
// ◆ sdf_values()
// template<class TriangleMesh , class SDFPropertyMap , class PointPropertyMap = typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type, class GeomTraits = typename Kernel_traits<typename boost::property_traits<PointPropertyMap>::value_type>::Kernel>
// std::pair< double, double > CGAL::sdf_values 	( 	const TriangleMesh &  	triangle_mesh,
// 		SDFPropertyMap  	sdf_values_map,
// 		double  	cone_angle = 2.0 / 3.0 * CGAL_PI,
// 		std::size_t  	number_of_rays = 25,
// 		bool  	postprocess = true,
// 		PointPropertyMap  	ppmap = PointPropertyMap(),
// 		GeomTraits  	traits = GeomTraits() 
// 	) 		
//
// #include <CGAL/mesh_segmentation.h>
//
// Function computing the Shape Diameter Function over a surface mesh.
//
// This function implements the Shape Diameter Function (SDF) as described in [4]. It is possible to compute raw SDF values (without post-processing). In such a case, -1 is used to indicate when no SDF value could be computed for a facet.
//
// Precondition
//     is_triangle_mesh(triangle_mesh)
//
// Template Parameters
//     TriangleMesh	a model of FaceListGraph
//     SDFPropertyMap	a ReadWritePropertyMap with boost::graph_traits<TriangleMesh>::face_descriptor as key and double as value type
//     GeomTraits	a model of SegmentationGeomTraits
//     PointPropertyMap	a ReadablePropertyMap with boost::graph_traits<TriangleMesh>::vertex_descriptor as key and GeomTraits::Point_3 as value type.
//
// Parameters
//     	triangle_mesh	surface mesh on which SDF values are computed
//     [out]	sdf_values_map	the SDF value of each facet
//     	cone_angle	opening angle in radians for the cone of each facet
//     	number_of_rays	number of rays picked in the cone of each facet. In our experiments, we observe that increasing the number of rays beyond the default has little effect on the quality of the segmentation result
//     	postprocess	if true, CGAL::sdf_values_postprocessing() is called on raw SDF value computed.
//     	traits	traits class
//     	ppmap	point property map. An overload is provided with get(boost::vertex_point,triangle_mesh) as default.
//
// Returns
//     minimum and maximum raw SDF values if postprocess is true, otherwise minimum and maximum SDF values (before linear normalization) 
//
// Examples
//     Surface_mesh_segmentation/extract_segmentation_into_mesh_example.py, Surface_mesh_segmentation/sdf_values_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_SM_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_example.py, and Surface_mesh_segmentation/segmentation_with_facet_ids_example.py.
//
// ◆ sdf_values_postprocessing()
// template<class TriangleMesh , class SDFPropertyMap >
// std::pair< double, double > CGAL::sdf_values_postprocessing 	( 	const TriangleMesh &  	triangle_mesh,
// 		SDFPropertyMap  	sdf_values_map 
// 	) 		
//
// #include <CGAL/mesh_segmentation.h>
//
// Function post-processing raw SDF values computed per facet.
//
// Post-processing steps applied :
//
//     Facets with -1 SDF values are assigned the average SDF value of their edge-adjacent neighbors. If there is still a facet having -1 SDF value, the minimum valid SDF value assigned to it. Note that this step is not inherited from the paper. The main reason for not assigning 0 to facets with no SDF values (i.e. -1) is that it can obstruct log-normalization process which takes place at the beginning of CGAL::segmentation_from_sdf_values().
//     SDF values are smoothed with bilateral filtering.
//     SDF values are linearly normalized between [0,1].
//
// See the section Post-processing of Raw SDF Values for more details.
//
// Precondition
//     is_triangle_mesh(triangle_mesh) 
//     Raw values should be greater or equal to 0. -1 indicates when no value could be computed
//
// Template Parameters
//     TriangleMesh	a model of FaceListGraph
//     SDFPropertyMap	a ReadWritePropertyMap with boost::graph_traits<TriangleMesh>::face_descriptor as key and double as value type
//
// Parameters
//     	triangle_mesh	surface mesh on which SDF values are computed
//     [in,out]	sdf_values_map	the SDF value of each facet
//
// Returns
//     minimum and maximum SDF values before linear normalization 
//
// ◆ segmentation_from_sdf_values()
// template<class TriangleMesh , class SDFPropertyMap , class SegmentPropertyMap , class PointPropertyMap = typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type, class GeomTraits = typename Kernel_traits<typename boost::property_traits<PointPropertyMap>::value_type>::Kernel>
// std::size_t CGAL::segmentation_from_sdf_values 	( 	const TriangleMesh &  	triangle_mesh,
// 		SDFPropertyMap  	sdf_values_map,
// 		SegmentPropertyMap  	segment_ids,
// 		std::size_t  	number_of_clusters = 5,
// 		double  	smoothing_lambda = 0.26,
// 		bool  	output_cluster_ids = false,
// 		PointPropertyMap  	ppmap = PointPropertyMap(),
// 		GeomTraits  	traits = GeomTraits() 
// 	) 		
//
// #include <CGAL/mesh_segmentation.h>
//
// Function computing the segmentation of a surface mesh given an SDF value per facet.
//
// This function fills a property map which associates a segment-id (in [0, number of segments -1]) or a cluster-id (in [0, number_of_clusters -1]) to each facet. A segment is a set of connected facets which are placed under the same cluster (see Figure 73.5).
//
// Note
//     Log-normalization is applied on sdf_values_map before segmentation. As described in the original paper [4], this normalization is done to preserve thin parts of the mesh by increasing the distance between smaller SDF values and reducing it between larger ones. 
//     There is no direct relation between the parameter number_of_clusters and the final number of segments after segmentation. However, setting a large number of clusters will result in a detailed segmentation of the mesh with a large number of segments.
//
// Precondition
//     is_triangle_mesh(triangle_mesh) 
//     number_of_clusters > 0
//
// Template Parameters
//     TriangleMesh	a model of FaceListGraph
//     SDFPropertyMap	a ReadablePropertyMap with boost::graph_traits<TriangleMesh>::face_descriptor as key and double as value type
//     SegmentPropertyMap	a ReadWritePropertyMap with boost::graph_traits<TriangleMesh>::face_descriptor as key and std::size_t as value type
//     GeomTraits	a model of SegmentationGeomTraits
//     PointPropertyMap	a ReadablePropertyMap with boost::graph_traits<TriangleMesh>::vertex_descriptor as key and GeomTraits::Point_3 as value type.
//
// Parameters
//     	triangle_mesh	surface mesh corresponding to the SDF values
//     	sdf_values_map	the SDF value of each facet between [0-1]
//     [out]	segment_ids	the segment or cluster id of each facet
//     	number_of_clusters	number of clusters for the soft clustering
//     	smoothing_lambda	factor which indicates the importance of the surface features for the energy minimization. It is recommended to choose a value in the interval [0,1]. See the section Hard clustering for more details.
//     	output_cluster_ids	if false fill segment_ids with segment-ids, and with cluster-ids otherwise (see Figure 73.5)
//     	traits	traits class
//     	ppmap	point property map. An overload is provided with get(boost::vertex_point,triangle_mesh) as default.
//
// Returns
//     number of segments if output_cluster_ids is set to false and number_of_clusters otherwise 
//
// Examples
//     Surface_mesh_segmentation/extract_segmentation_into_mesh_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_SM_example.py, Surface_mesh_segmentation/segmentation_from_sdf_values_example.py, and Surface_mesh_segmentation/segmentation_with_facet_ids_example.py.
//
// ◆ segmentation_via_sdf_values()
// template<class TriangleMesh , class SegmentPropertyMap , class PointPropertyMap = typename boost::property_map<TriangleMesh, boost::vertex_point_t>::type, class GeomTraits = typename Kernel_traits<typename boost::property_traits<PointPropertyMap>::value_type>::Kernel>
// std::size_t CGAL::segmentation_via_sdf_values 	( 	const TriangleMesh &  	triangle_mesh,
// 		SegmentPropertyMap  	segment_ids,
// 		double  	cone_angle = 2.0 / 3.0 * CGAL_PI,
// 		std::size_t  	number_of_rays = 25,
// 		std::size_t  	number_of_clusters = 5,
// 		double  	smoothing_lambda = 0.26,
// 		bool  	output_cluster_ids = false,
// 		PointPropertyMap  	ppmap = PointPropertyMap(),
// 		GeomTraits  	traits = GeomTraits() 
// 	) 		
//
// #include <CGAL/mesh_segmentation.h>
//
// Function computing the segmentation of a surface mesh.
//
// This function is equivalent to calling the functions CGAL::sdf_values() and CGAL::segmentation_from_sdf_values() with the same parameters.
//
// Note
//     There is no direct relation between the parameter number_of_clusters and the final number of segments after segmentation. However, setting a large number of clusters will result in a detailed segmentation of the mesh with a large number of segments. 
//     For computing segmentations of the mesh with different parameters (i.e. number of levels, and smoothing lambda), it is more efficient to first compute the SDF values using CGAL::sdf_values() and use them in different calls to CGAL::segmentation_from_sdf_values().
//
// Precondition
//     is_triangle_mesh(triangle_mesh) 
//     number_of_clusters > 0
//
// Template Parameters
//     TriangleMesh	a model of FaceListGraph
//     SegmentPropertyMap	a ReadWritePropertyMap with boost::graph_traits<TriangleMesh>::face_descriptor as key and std::size_t as value type
//     GeomTraits	a model of SegmentationGeomTraits
//     PointPropertyMap	a ReadablePropertyMap with boost::graph_traits<TriangleMesh>::vertex_descriptor as key and GeomTraits::Point_3 as value type.
//
// Parameters
//     	triangle_mesh	surface mesh on which SDF values are computed
//     [out]	segment_ids	the segment or cluster id of each facet
//     	cone_angle	opening angle in radians for the cone of each facet
//     	number_of_rays	number of rays picked in the cone of each facet. In our experiments, we observe that increasing the number of rays beyond the default has a little effect on the quality of the segmentation result
//     	number_of_clusters	number of clusters for the soft clustering
//     	smoothing_lambda	factor which indicates the importance of the surface features for the energy minimization. It is recommended to choose a value in the interval [0,1]. See the section Hard clustering for more details.
//     	output_cluster_ids	if false fill segment_ids with segment-ids, and with cluster-ids otherwise (see Figure 73.5)
//     	traits	traits class
//     	ppmap	point property map. An overload is provided with get(boost::vertex_point,triangle_mesh) as default.
//
// Returns
//     number of segments if output_cluster_ids is set to false and number_of_clusters otherwise 
//
// Examples
//     Surface_mesh_segmentation/segmentation_via_sdf_values_example.py.
//
