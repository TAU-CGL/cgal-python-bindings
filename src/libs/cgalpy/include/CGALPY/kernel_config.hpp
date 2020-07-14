// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_CONFIG_HPP
#define CGALPY_KERNEL_CONFIG_HPP

#define CGAL_HEADER_ONLY 1
#define BOOST_PYTHON_STATIC_LIB 1
#define CGAL_DO_NOT_USE_BOOST_MP 1

#define CGALPY_KERNEL_EPEC 0
#define CGALPY_KERNEL_EPIC 1

#ifndef CGALPY_KERNEL
#define CGALPY_KERNEL 0
#endif

#endif //CGALPY_KERNEL_CONFIG_HPP
