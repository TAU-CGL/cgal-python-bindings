#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/trampoline.h>

#include <CGAL/Shape_detection/Efficient_RANSAC.h>

#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;
namespace SD = CGAL::Shape_detection;

template <typename Shape, typename Traits>
struct Py_base_shape : Shape {
  // NB_TRAMPOLINE(Shape, 7);
  NB_TRAMPOLINE(Shape, 1);
  std::string info() const override {
    NB_OVERRIDE(info);
  }

};

template<typename Traits, typename Shape, typename C>
auto export_base_shape(C& c, const std::string& name) {
  using Base_shape = Py_base_shape<Shape, Traits>;
  return py::class_<Shape, Base_shape>(c, name.c_str())
    .def("indices_of_assigned_points", &Base_shape::indices_of_assigned_points)
    .def("info", &Shape::info)
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

  // Shapes
  using Cone = SD::Cone<RANSAC_traits>;
  using Cylinder = SD::Cylinder<RANSAC_traits>;
  using Plane = SD::Plane<RANSAC_traits>;
  using Shape_base = SD::Shape_base<RANSAC_traits>;
  using Sphere = SD::Sphere<RANSAC_traits>;
  using Torus = SD::Torus<RANSAC_traits>;

  auto shape = export_base_shape<RANSAC_traits, Shape_base>(m, "Shape_base");

  // Property Maps
  py::class_<RANSAC_traits>(m, "Efficient_RANSAC_traits")
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

  py::class_<RANSAC> eff_ransac(m, "Efficient_RANSAC");
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
    .def("input_iterator_first", &RANSAC::input_iterator_first)
    .def("input_iterator_beyond", &RANSAC::input_iterator_beyond)
    // .def("set_input", &RANSAC::set_input,
    //      py::arg("input_range"),
    //      py::arg("point_map") = InputPointMap(),
    //      py::arg("normal_map") = InputNormalMap(),
    //      "Sets the input data.\n\n"
    //      "The range must stay valid until the detection has been performed and the access to the results is no longer required. The data in the input is reordered by the methods detect() and preprocess(). This function first calls clear().\n\n"
    //      "Parameters\n"
    //      "• input_range:	Range of input data.\n"
    //      "• point_map:	Property map to access the position of an input point.\n"
    //      "• normal_map:	Property map to access the normal of an input point.\n"
    //      "Examples\n"
    //      "    Shape_detection/efficient_RANSAC_with_custom_shape.cpp.")
    ;



}
