// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_CARTESIAN_PRODUCT_HPP
#define CGALPY_CARTESIAN_PRODUCT_HPP

namespace CGALPY {

template <typename... Types> class Type_list { };

template <typename... Types> class Call_args { };

template <template <typename ...> class Wrapper, typename Arg,
          typename... Types>
void cartesian_product_recursive(Arg& arg, Call_args<Types...>)
{ Wrapper<Arg, Types...>()(arg); }

template <template <typename ...> class Wrapper, typename Arg,
          typename... CallTypes, typename... Types, typename ... TypeLists>
void cartesian_product_recursive(Arg& arg, Call_args<CallTypes...>,
                                 Type_list<Types...>,
                                 TypeLists... type_lists) {
  // fold-expression on Types
  (cartesian_product_recursive<Wrapper>(arg, Call_args<CallTypes..., Types>(),
                                        type_lists...), ...);
}

template <template <typename ...> class Wrapper, typename Arg,
          typename... TypeLists>
void cartesian_product(Arg& arg, TypeLists... type_lists) {
  cartesian_product_recursive<Wrapper>(arg, Call_args<>(), type_lists...);
}

}

#endif
