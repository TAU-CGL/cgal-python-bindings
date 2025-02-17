// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_CIRCULATOR_STATE_HPP
#define CGALPY_CIRCULATOR_STATE_HPP

template <typename Iterator>
struct circulator_state {
  Iterator it;
  bool first;
  bool done;
};

#endif
