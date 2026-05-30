#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/trampoline.h>

#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>

#include "cgalpy/kernel_type.hpp"
#include "cgalpy/Sd_docstrings.hpp"

namespace py = nanobind;
namespace SD = CGAL::Shape_detection;
namespace sd_doc = cgalpy::sd::docstrings;

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
auto export_base_shape(C& c, const std::string& name, const char* doc = "") {
  using Base_shape = Py_base_shape<Shape, Traits>;
  return py::class_<Shape, Base_shape>(c, name.c_str(), doc)
    .def("indices_of_assigned_points", &Base_shape::indices_of_assigned_points,
         sd_doc::Shape_detection_Shape_base_indices_of_assigned_points)
    .def("info", &Shape::info,
         sd_doc::Shape_detection_Shape_base_info)
    .def("squared_distance", [](const Shape& self, const typename Shape::Point_3& p) {
      return self.squared_distance(p);
    },
         py::arg("p"),
         sd_doc::Shape_detection_Shape_base_squared_distance)
    ;
}

template<typename RANSAC, typename Shape, typename C>
auto add_shape_factory(C& c, const std::string& name) {
  return c.def(("add_" + name + "_factory").c_str(), [](RANSAC& self)
    { return self.template add_shape_factory<Shape>(); },
    sd_doc::Shape_detection_Efficient_RANSAC_add_shape_factory
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

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  auto cone = export_base_shape<RANSAC_traits, Cone>(m, "Cone",
    sd_doc::Shape_detection_Cone_class)
    .def("angle", &Cone::angle,
         sd_doc::Shape_detection_Cone_angle)
    .def("apex", &Cone::apex,
         sd_doc::Shape_detection_Cone_apex)
    .def("axis", &Cone::axis,
         sd_doc::Shape_detection_Cone_axis)
    ;
  auto cylinder = export_base_shape<RANSAC_traits, Cylinder>(m, "Cylinder",
    sd_doc::Shape_detection_Cylinder_class)
    .def("axis", &Cylinder::axis,
         sd_doc::Shape_detection_Cylinder_axis)
    .def("radius", &Cylinder::radius,
         sd_doc::Shape_detection_Cylinder_radius)
    ;
  auto plane = export_base_shape<RANSAC_traits, Plane>(m, "Plane",
    sd_doc::Shape_detection_Plane_class)
    .def("get_plane", &Plane::operator Kernel::Plane_3,
         sd_doc::Shape_detection_Plane_operator_Plane_3)
    .def("get_normal", &Plane::plane_normal,
         sd_doc::Shape_detection_Plane_plane_normal)
    .def("get_d", &Plane::d,
         sd_doc::Shape_detection_Plane_d)
    ;
  auto shape_base = export_base_shape<RANSAC_traits, Shape_base>(m, "Shape_base",
    sd_doc::Shape_detection_Shape_base_class);
  auto sphere = export_base_shape<RANSAC_traits, Sphere>(m, "Sphere",
    sd_doc::Shape_detection_Sphere_class)
    .def("get_sphere", &Sphere::operator Kernel::Sphere_3,
         sd_doc::Shape_detection_Sphere_operator_Sphere_3)
    .def("center", &Sphere::center,
         sd_doc::Shape_detection_Sphere_center)
    .def("radius", &Sphere::radius,
         sd_doc::Shape_detection_Sphere_radius)
    ;
  auto torus = export_base_shape<RANSAC_traits, Torus>(m, "Torus",
    sd_doc::Shape_detection_Torus_class)
    .def("axis", &Torus::axis,
         sd_doc::Shape_detection_Torus_axis)
    .def("center", &Torus::center,
         sd_doc::Shape_detection_Torus_center)
    .def("major_radius", &Torus::major_radius,
         sd_doc::Shape_detection_Torus_major_radius)
    .def("minor_radius", &Torus::minor_radius,
         sd_doc::Shape_detection_Torus_minor_radius)
    ;
#endif

  // Property Maps
  py::class_<RANSAC_traits>(m, "Efficient_RANSAC_traits",
    sd_doc::Shape_detection_Efficient_RANSAC_traits_class)
    .def(py::init<const Gt &>(),
         py::arg("gt") = Gt(),
         sd_doc::Shape_detection_Efficient_RANSAC_traits_Efficient_RANSAC_traits)
    .def("construct_point_3_object", &RANSAC_traits::construct_point_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_point_3_object)
    .def("construct_vector_3_object", &RANSAC_traits::construct_vector_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_vector_3_object)
    .def("construct_point_2_object", &RANSAC_traits::construct_point_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_point_2_object)
    .def("construct_vector_2_object", &RANSAC_traits::construct_vector_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_vector_2_object)
    .def("construct_sphere_3_object", &RANSAC_traits::construct_sphere_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_sphere_3_object)
    .def("construct_line_3_object", &RANSAC_traits::construct_line_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_line_3_object)
    .def("construct_circle_2_object", &RANSAC_traits::construct_circle_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_circle_2_object)
    .def("construct_point_on_3_object", &RANSAC_traits::construct_point_on_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_point_on_3_object)
    .def("compute_x_2_object", &RANSAC_traits::compute_x_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_x_2_object)
    .def("compute_y_2_object", &RANSAC_traits::compute_y_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_y_2_object)
    .def("compute_x_3_object", &RANSAC_traits::compute_x_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_x_3_object)
    .def("compute_y_3_object", &RANSAC_traits::compute_y_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_y_3_object)
    .def("compute_z_3_object", &RANSAC_traits::compute_z_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_z_3_object)
    .def("compute_squared_length_3_object", &RANSAC_traits::compute_squared_length_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_squared_length_3_object)
    .def("compute_squared_length_2_object", &RANSAC_traits::compute_squared_length_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_squared_length_2_object)
    .def("construct_scaled_vector_3_object", &RANSAC_traits::construct_scaled_vector_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_scaled_vector_3_object)
    .def("construct_sum_of_vectors_3_object", &RANSAC_traits::construct_sum_of_vectors_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_sum_of_vectors_3_object)
    .def("construct_translated_point_3_object", &RANSAC_traits::construct_translated_point_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_translated_point_3_object)
    .def("compute_scalar_product_3_object", &RANSAC_traits::compute_scalar_product_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_scalar_product_3_object)
    .def("construct_cross_product_vector_3_object", &RANSAC_traits::construct_cross_product_vector_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_cross_product_vector_3_object)
    .def("construct_center_3_object", &RANSAC_traits::construct_center_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_center_3_object)
    .def("construct_center_2_object", &RANSAC_traits::construct_center_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_construct_center_2_object)
    .def("compute_squared_radius_3_object", &RANSAC_traits::compute_squared_radius_3_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_squared_radius_3_object)
    .def("compute_squared_radius_2_object", &RANSAC_traits::compute_squared_radius_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_compute_squared_radius_2_object)
    .def("collinear_2_object", &RANSAC_traits::collinear_2_object,
         sd_doc::Shape_detection_Efficient_RANSAC_traits_collinear_2_object)
    // .def("intersection_3_object", &RANSAC_traits::intersection_3_object) // this one doesn't compile?
    ;

  py::class_<RANSAC> eff_ransac(
    m, "Efficient_RANSAC",
    sd_doc::Shape_detection_Efficient_RANSAC_class);
  eff_ransac
    // Initialization
    .def(py::init<RANSAC_traits>(),
         py::arg("traits") = RANSAC_traits(),
         sd_doc::Shape_detection_Efficient_RANSAC_Efficient_RANSAC)
    .def("traits", &RANSAC::traits,
         sd_doc::Shape_detection_Efficient_RANSAC_traits)
    .def("point_map", &RANSAC::point_map,
         sd_doc::Shape_detection_Efficient_RANSAC_point_map)
    .def("normal", &RANSAC::normal,
         sd_doc::Shape_detection_Efficient_RANSAC_normal)
    // .def("input_iterator_first", &RANSAC::input_iterator_first)
    // .def("input_iterator_beyond", &RANSAC::input_iterator_beyond)
    .def("set_input", &RANSAC::set_input,
         py::arg("input_range"),
         py::arg("point_map") = InputPointMap(),
         py::arg("normal_map") = InputNormalMap(),
         sd_doc::Shape_detection_Efficient_RANSAC_set_input)
    ;

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))

  add_shape_factory<RANSAC, Cone>(eff_ransac, "cone");
  add_shape_factory<RANSAC, Cylinder>(eff_ransac, "cylinder");
  add_shape_factory<RANSAC, Plane>(eff_ransac, "plane");
  // add_shape_factory<RANSAC, Shape_base>(eff_ransac, "shape_base"); // needs to be not pure
  add_shape_factory<RANSAC, Sphere>(eff_ransac, "sphere");
  add_shape_factory<RANSAC, Torus>(eff_ransac, "torus");

#endif

  // Property Maps (weird)
  using Point = typename Kernel::Point_3;
  // export_point_to_shape_index_map<RANSAC_traits, RANSAC::Plane_range, Cone>(m, "cone");

}
