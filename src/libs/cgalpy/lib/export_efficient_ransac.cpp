#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/trampoline.h>

#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>

#include "CGALPY/kernel_type.hpp"

namespace py = nanobind;
namespace SD = CGAL::Shape_detection;

template <typename Shape, typename Traits>
struct Py_base_shape : Shape {
  NB_TRAMPOLINE(Shape, 2);
  std::string info() const override {
    NB_OVERRIDE(info);
  }
  typename Shape::FT squared_distance(const typename Shape::Point_3 &p) const override {
    NB_OVERRIDE_PURE(squared_distance, p);
  }

  // protected in base
  // void create_shape(const std::vector< std::size_t > &indices) override {
  //   NB_OVERRIDE(create_shape, indices);
  // }
  // std::size_t connected_component(std::vector< std::size_t > &indices, typename Shape::FT cluster_epsilon) override {
  //   NB_OVERRIDE(connected_component, indices, cluster_epsilon);
  // }
  // void squared_distance(const std::vector< std::size_t > &indices, std::vector< typename Shape::FT > &distances) const override {
  //   NB_OVERRIDE(squared_distance, indices, distances);
  // }
  // void cos_to_normal(const std::vector< std::size_t > &indices, std::vector< typename Shape::FT > &angles) const override {
  //   NB_OVERRIDE(cos_to_normal, indices, angles);
  // }
  // std::size_t minimum_sample_size() const override {
  //   NB_OVERRIDE(minimum_sample_size);
  // }
};

template<typename Traits, typename Shape, typename C>
auto export_base_shape(C& c, const std::string& name, const std::string& doc = "") {
  using Base_shape = Py_base_shape<Shape, Traits>;
  return py::class_<Shape, Base_shape>(c, name.c_str(), doc.c_str())
    .def("indices_of_assigned_points", &Base_shape::indices_of_assigned_points,
         "Returns the indices of the points in the input range assigned to this shape.")
    .def("info", &Shape::info,
         "Returns a string containing the shape type and the numerical parameters.")
    .def("squared_distance", [](const Shape& self, const typename Shape::Point_3& p) {
      return self.squared_distance(p);
    },
         "Computes the squared Euclidean distance from the query point p to the shape.")
    ;
}

template<typename RANSAC, typename Shape, typename C>
auto add_shape_factory(C& c, const std::string& name) {
  return c.def(("add_" + name + "_factory").c_str(), [](RANSAC& self)
    { return self.template add_shape_factory<Shape>(); },
    ("Registers the " + name + " shape type in the detection engine.").c_str()
  );
}

// template<typename Traits, typename Kernel_, typename ShapeRange, typename C>
// auto export_point_to_shape_index_map(C& c, const std::string& name) {
//   using Point_to_shape_index_map = SD::Point_to_shape_index_map<Traits>;
//   using PointRange = std::vector<Kernel::Point_3>;
//   c.def(("get_Point_to_" + name + "_index_map").c_str(),
//         [](const PointRange& points, const ShapeRange& shapes) {
//           return Point_to_shape_index_map(points, shapes);
//         },
//         ("Property map that associates a point index to its assigned " + name + " found by the Sd.Efficient_RANSAC algorithm.").c_str()
//         );
//   return py::class_<Point_to_shape_index_map>(c,
//     ("Point_to_" + name + "_index_map").c_str(),
//     ("Property map that associates a point index to its assigned " + name + " found by the Sd.Efficient_RANSAC algorithm.").c_str())
//     .def(py::init<const PointRange&, const ShapeRange&>(), // this doesnt work
//          py::arg("points"), py::arg("shapes"),
//          ("Constructs a property map to map points to " + name + " shapes.\n"
//           "Note\n"
//           "• shapes must be a range of shapes detected using points.").c_str())
//   ;
// }

template <typename Map>
auto export_input_map(py::module_& m, const std::string& name) {
  return py::class_<Map>(m, name.c_str(),
    ("Map that associates a point index to its assigned " + name).c_str())
    .def(py::init<>())
    .def("__getitem__", &Map::operator[])
    .def("__setitem__", &Map::operator[])
    ;
}

void export_efficient_ransac(py::module_& m) {
  using Kernel_ = Kernel;
  using Gt = Kernel_;
  using Point_with_normal = std::pair<Kernel::Point_3, Kernel::Vector_3>;
  using Input_range = std::vector<Point_with_normal>;
  using InputPointMap = CGAL::First_of_pair_property_map<Point_with_normal>;
  using InputNormalMap = CGAL::Second_of_pair_property_map<Point_with_normal>;
  using RANSAC_traits = SD::Efficient_RANSAC_traits<Gt, Input_range, InputPointMap, InputNormalMap>;
  using RANSAC = SD::Efficient_RANSAC<RANSAC_traits>;
  using Point_to_shape_index_map = SD::Point_to_shape_index_map<RANSAC>;
  using Plane_map = SD::Plane_map<RANSAC>;

  export_input_map<InputPointMap>(m, "Input_point_map");
  export_input_map<InputNormalMap>(m, "Input_normal_map");

  // Shapes
  using Cone = SD::Cone<RANSAC_traits>;
  using Cylinder = SD::Cylinder<RANSAC_traits>;
  using Plane = SD::Plane<RANSAC_traits>;
  using Shape_base = SD::Shape_base<RANSAC_traits>;
  using Sphere = SD::Sphere<RANSAC_traits>;
  using Torus = SD::Torus<RANSAC_traits>;

  auto cone = export_base_shape<RANSAC_traits, Cone>(m, "Cone",
    "Cone implements Shape_base.\n"
    "The cone is represented by its apex, the axis, and the opening angle. This representation models an open infinite single-cone.\n"
    "Examples\n"
    "Shape_detection/efficient_RANSAC_with_parameters.py.")
    .def("angle", &Cone::angle,
         "Opening angle between the axis and the surface of the cone.")
    .def("apex", &Cone::apex,
         "Apex of the cone.")
    .def("axis", &Cone::axis,
         "Axis points from the apex into the cone.")
    ;
  auto cylinder = export_base_shape<RANSAC_traits, Cylinder>(m, "Cylinder",
    "Cylinder implements Shape_base.\n"
    "The cylinder is represented by the axis, that is a point and direction, and the radius. The cylinder is unbounded, thus caps are not modeled.\n"
    "Examples\n"
    "Shape_detection/efficient_RANSAC_with_parameters.py.")
    .def("axis", &Cylinder::axis,
         "Axis of the cylinder.")
    .def("radius", &Cylinder::radius,
         "Radius of the cylinder.")
    ;
  auto plane = export_base_shape<RANSAC_traits, Plane>(m, "Plane",
    "Plane implements Shape_base.\n"
    "The plane is represented by the normal vector and the distance to the origin.\n"
    "Examples\n"
    "Shape_detection/efficient_RANSAC_basic.py, Shape_detection/efficient_RANSAC_with_callback.py, Shape_detection/efficient_RANSAC_with_parameters.py, and Shape_detection/efficient_RANSAC_with_point_access.py.")
    .def("get_plane", &Plane::operator Kernel::Plane_3,
         "Conversion operator to Plane_3 type.")
    .def("get_normal", &Plane::plane_normal,
         "Normal vector of the plane.")
    .def("get_d", &Plane::d,
         "Signed distance from the origin.")
    ;
  auto shape_base = export_base_shape<RANSAC_traits, Shape_base>(m, "Shape_base",
    "Base class for shape types that defines an interface to construct a shape from a set of points and to compute the point distance and normal deviation from the surface normal.\n"
    "It is used during detection to identify the inliers from the input data and to extract the largest connected component in the inlier points.\n"
    "Examples\n"
    "Shape_detection/include/efficient_RANSAC_with_custom_shape.h.");
  auto sphere = export_base_shape<RANSAC_traits, Sphere>(m, "Sphere",
    "Sphere implements Shape_base.\n"
    "The sphere is represented by its center and the radius.\n"
    "Examples\n"
    "Shape_detection/efficient_RANSAC_with_parameters.py.")
    .def("get_sphere", &Sphere::operator Kernel::Sphere_3,
         "Conversion operator to Sphere_3 type.")
    .def("center", &Sphere::center,
         "Access to the center.")
    .def("radius", &Sphere::radius,
         "Access to the radius of the sphere.")
    ;
  auto torus = export_base_shape<RANSAC_traits, Torus>(m, "Torus",
    "The torus is represented by the symmetry axis, its center on the axis, and the major and minor radii.\n"
    "Examples\n"
    "Shape_detection/efficient_RANSAC_with_parameters.py.")
    .def("axis", &Torus::axis,
         "Direction of symmetry axis.")
    .def("center", &Torus::center,
         "Center point on symmetry axis.")
    .def("major_radius", &Torus::major_radius,
         "Major radius of the torus.")
    .def("minor_radius", &Torus::minor_radius,
         "Minor radius of the torus.")
    ;


  // Property Maps
  py::class_<RANSAC_traits>(m, "Efficient_RANSAC_traits",
    "Default traits class for the CGAL::Shape_detection::Efficient_RANSAC.")
    .def(py::init<const Gt &>(),
         py::arg("gt") = Gt())
    .def("construct_point_3_object", &RANSAC_traits::construct_point_3_object)
    .def("construct_vector_3_object", &RANSAC_traits::construct_vector_3_object)
    .def("construct_point_2_object", &RANSAC_traits::construct_point_2_object)
    .def("construct_vector_2_object", &RANSAC_traits::construct_vector_2_object)
    .def("construct_sphere_3_object", &RANSAC_traits::construct_sphere_3_object)
    .def("construct_line_3_object", &RANSAC_traits::construct_line_3_object)
    .def("construct_circle_2_object", &RANSAC_traits::construct_circle_2_object)
    .def("construct_point_on_3_object", &RANSAC_traits::construct_point_on_3_object)
    .def("compute_x_2_object", &RANSAC_traits::compute_x_2_object)
    .def("compute_y_2_object", &RANSAC_traits::compute_y_2_object)
    .def("compute_x_3_object", &RANSAC_traits::compute_x_3_object)
    .def("compute_y_3_object", &RANSAC_traits::compute_y_3_object)
    .def("compute_z_3_object", &RANSAC_traits::compute_z_3_object)
    .def("compute_squared_length_3_object", &RANSAC_traits::compute_squared_length_3_object)
    .def("compute_squared_length_2_object", &RANSAC_traits::compute_squared_length_2_object)
    .def("construct_scaled_vector_3_object", &RANSAC_traits::construct_scaled_vector_3_object)
    .def("construct_sum_of_vectors_3_object", &RANSAC_traits::construct_sum_of_vectors_3_object)
    .def("construct_translated_point_3_object", &RANSAC_traits::construct_translated_point_3_object)
    .def("compute_scalar_product_3_object", &RANSAC_traits::compute_scalar_product_3_object)
    .def("construct_cross_product_vector_3_object", &RANSAC_traits::construct_cross_product_vector_3_object)
    .def("construct_center_3_object", &RANSAC_traits::construct_center_3_object)
    .def("construct_center_2_object", &RANSAC_traits::construct_center_2_object)
    .def("compute_squared_radius_3_object", &RANSAC_traits::compute_squared_radius_3_object)
    .def("compute_squared_radius_2_object", &RANSAC_traits::compute_squared_radius_2_object)
    .def("collinear_2_object", &RANSAC_traits::collinear_2_object)
    // .def("intersection_3_object", &RANSAC_traits::intersection_3_object) // this one doesn't compile?
    ;

  py::class_<RANSAC> eff_ransac(m, "Efficient_RANSAC",
    "Shape detection algorithm based on the RANSAC method.\n"
    "Given a point set in 3D space with unoriented normals, sampled on surfaces, this class enables to detect subsets of connected points lying on the surface of primitive shapes. Each input point is assigned to either none or at most one detected primitive shape. The implementation follows [2].\n\n"
    "Examples\n"
    "Shape_detection/efficient_RANSAC_basic.py, Shape_detection/efficient_RANSAC_with_callback.py, Shape_detection/efficient_RANSAC_with_custom_shape.py, Shape_detection/efficient_RANSAC_with_parameters.py, and Shape_detection/efficient_RANSAC_with_point_access.py.");
  eff_ransac
    // Initialization
    .def(py::init<RANSAC_traits>(),
         py::arg("traits") = RANSAC_traits(),
         "Constructs an empty shape detection object.")
    .def("traits", &RANSAC::traits,
         "Retrieves the traits class.")
    .def("point_map", &RANSAC::point_map,
         "Retrieves the point property map.")
    .def("normal", &RANSAC::normal,
         "Retrieves the normal property map.")
    // .def("input_iterator_first", &RANSAC::input_iterator_first)
    // .def("input_iterator_beyond", &RANSAC::input_iterator_beyond)
    .def("set_input", &RANSAC::set_input,
         py::arg("input_range"),
         py::arg("point_map") = InputPointMap(),
         py::arg("normal_map") = InputNormalMap(),
         "Sets the input data.\n\n"
         "The range must stay valid until the detection has been performed and the access to the results is no longer required. The data in the input is reordered by the methods detect() and preprocess(). This function first calls clear().\n\n"
         "Parameters\n"
         "• input_range:	Range of input data.\n"
         "• point_map:	Property map to access the position of an input point.\n"
         "• normal_map:	Property map to access the normal of an input point.\n"
         "Examples\n"
         "    Shape_detection/efficient_RANSAC_with_custom_shape.py.")
    ;

  add_shape_factory<RANSAC, Cone>(eff_ransac, "cone");
  add_shape_factory<RANSAC, Cylinder>(eff_ransac, "cylinder");
  add_shape_factory<RANSAC, Plane>(eff_ransac, "plane");
  // add_shape_factory<RANSAC, Shape_base>(eff_ransac, "shape_base"); // needs to be not pure
  add_shape_factory<RANSAC, Sphere>(eff_ransac, "sphere");
  add_shape_factory<RANSAC, Torus>(eff_ransac, "torus");

  // Property Maps (weird)
  using Point = typename Kernel::Point_3;
  // export_point_to_shape_index_map<RANSAC_traits, RANSAC::Plane_range, Cone>(m, "cone");

}
