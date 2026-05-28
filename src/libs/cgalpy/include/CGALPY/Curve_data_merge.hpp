// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_CURVE_DATA_MERGE_HPP
#define CGALPY_CURVE_DATA_MERGE_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {
namespace aos2 {

/*! \class Curve_data_merge
 * When two curves overlap, the following actions are performed immediately
 * one after the other:
 * 1. An object of this type is constructed.
 * 2. The operator() is invoked.
 * 3. The object is destructed.
 * Therefore, the constructor and destructor should remain empty!
 */
class Curve_data_merge {
public:
  /*! Construct
   */
  Curve_data_merge() {}

  /*! Destruct
   */
  ~Curve_data_merge() {}

  /*! Set the callback function.
   */
  static void set_func(py::object func) {
    s_func = func;
    s_initialized = true;
  }

  /*! Reset the callback function.
   */
  static void reset_func() {
    s_func = py::none();
    s_initialized = false;
  }

  /*! Obtain the callback function.
   */
  static py::object func() { return s_func; }

  /*! Apply the callback function
   */
  py::object operator()(py::object a, py::object b) const
  { return (! s_initialized) ? a : s_func(a, b); }

private:
  //! The callback function to apply
  inline static py::object s_func;

  inline static bool s_initialized = false;
};

}
} // namespace cgalpy

#endif
