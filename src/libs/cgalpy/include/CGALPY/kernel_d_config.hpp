// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_D_CONFIG_HPP
#define CGALPY_KERNEL_D_CONFIG_HPP

#define CGALPY_KERNEL_D_EPIC_D                  0
#define CGALPY_KERNEL_D_EPEC_D                  1
#define CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE      3
#define CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ   4

#ifndef CGALPY_KERNEL_D
#define CGALPY_KERNEL_D                         CGALPY_KERNEL_D_EPIC_D
#endif

#define CGALPY_KERNEL_D_DIMENSION_TAG_DYNAMIC   0
#define CGALPY_KERNEL_D_DIMENSION_TAG_STATIC    1

#ifndef CGALPY_KERNEL_D_DIMENSION_TAG
#define CGALPY_KERNEL_D_DIMENSION_TAG           CGALPY_KERNEL_D_DIMENSION_TAG_DYNAMIC
#endif

#ifndef CGALPY_KERNEL_D_DIMENSION
#define CGALPY_KERNEL_D_DIMENSION               2
#endif

#endif
