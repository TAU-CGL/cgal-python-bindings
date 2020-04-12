// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "config.hpp"
#ifdef CGALPY_ALPHA_SHAPES_3_BINDINGS
#include "common.hpp"

template <typename A, typename C>
void export_alpha_shapes_3(C c)
{
  using namespace boost::python;

  c
    .def(init<>())
    .def(init<A&>())
#if 0
    // Modifiers
    .def("make_alpha_shape", &A:make_alpha_shape)
    .def("clear", &A:clear)
    .def("set_alpha", &A:set_alpha)
    .def("set_mode", &A:set_mode)
    // Query Functions
    .def("get_mode", &A:get_mode)
    .def("get_alpha", &A:get_alpha)
    .def("get_nth_alpha", &A:get_nth_alpha)
    .def("number_of_alphas", &A:number_of_alphas)
    .def("classify", &A:classify)
    .def("classify", &A:classify)
    .def("classify", &A:classify)
    .def("classify", &A:classify)
    .def("classify", &A:classify)
    .def("classify", &A:classify)
    .def("get_alpha_status", &A:get_alpha_status)
    .def("get_alpha_status", &A:get_alpha_status)
    .def("get_alpha_shape_cells", &A:get_alpha_shape_cells)
    .def("get_alpha_shape_facets", &A:get_alpha_shape_facets)
    .def("get_alpha_shape_edges", &A:get_alpha_shape_edges)
    .def("get_alpha_shape_vertices", &A:get_alpha_shape_vertices)
    .def("filtration", &A:filtration)
    .def("filtration_with_alpha_values", &A:filtration_with_alpha_values)
    // Traversal of the alpha-Values
    .def("alpha_begin", &A:alpha_begin)
    .def("alpha_end", &A:alpha_end)
    .def("alpha_find", &A:alpha_find)
    .def("alpha_lower_bound", &A:alpha_lower_bound)
    .def("alpha_upper_bound", &A:alpha_upper_bound)
    // Operations
    .def("number_of_solid_components", &A:number_of_solid_components)
    .def("find_optimal_alpha", &A:find_optimal_alpha)
#endif
    ;
}

#endif
