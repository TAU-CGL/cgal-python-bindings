// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_IF__HPP
#define CGALPY_IF__HPP

// General utility: if_<Condition, Then, Else>::type
// Selects 'Then' or 'Else' type based on the value of the 'Condition'
template <bool Condition, typename Then, typename Else = void>
struct if_ { typedef Then type; };
template <typename Then, typename Else>
struct if_<false, Then, Else > { typedef Else type; };

#endif
