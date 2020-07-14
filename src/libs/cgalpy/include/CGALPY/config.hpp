// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_CONFIG_HPP
#define CGALPY_CONFIG_HPP

#include <CGALPY/kernel_config.hpp>
#include <CGALPY/arrangement_2_config.hpp>
#include <CGALPY/spatial_searching_config.hpp>
#include <CGALPY/convex_hull_2_config.hpp>

#define CGAL_HEADER_ONLY 1
#define BOOST_PYTHON_STATIC_LIB 1
#define CGAL_DO_NOT_USE_BOOST_MP 1
#define STR(s) #s
#define XSTR(s) STR(s)


#endif //CONFIG_HPP
