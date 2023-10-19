// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ITERATOR_STATE_HPP
#define CGALPY_ITERATOR_STATE_HPP

template <typename Iterator, typename Sentinel>
struct iterator_state {
  Iterator it;
  Sentinel end;
  bool first_or_done;
};

#endif
