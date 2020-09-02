// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

void export_kernel();
void export_triangulation_3();
void export_alpha_shapes_3();

BOOST_PYTHON_MODULE(CGALPY_TRI3_MODULE_NAME)
{
  using namespace boost::python;

  export_kernel();
  export_triangulation_3();
#ifdef CGALPY_ALPHA_SHAPES_3_BINDINGS
  export_alpha_shapes_3();
#endif
}
