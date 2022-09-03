// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_REFERENCE_WRAPPER_CASTER_HPP
#define CGALPY_REFERENCE_WRAPPER_CASTER_HPP

#include <nanobind/nanobind.h>

namespace nanobind {
namespace detail {

template <typename T>
struct type_caster<std::reference_wrapper<T>> {
  using Value = std::reference_wrapper<T>;
  using Caster = make_caster<T>;
  static constexpr auto Name = Caster::Name;
  static constexpr bool IsClass = true;

  template <typename T_> using Cast = movable_cast_t<T_>;

  Value value;

  bool from_python(handle src, uint8_t flags, cleanup_list* cleanup) noexcept {
    Caster caster;
    if (! caster.from_python(src, flags, cleanup)) return false;

    value = Value(caster.operator T *());
    return true;
  }

  static handle from_cpp(const Value& value, rv_policy policy,
                         cleanup_list* cleanup) noexcept {
    if ((policy == rv_policy::take_ownership) ||
        (policy == rv_policy::automatic))
      policy = rv_policy::automatic_reference;
    return Caster::from_cpp(&value.get(), policy, cleanup);
  }

  explicit operator Value *() { return &value; }
  explicit operator Value &() { return value; }
  explicit operator Value &&() && { return (Value &&) value; }
};

}
}

#endif
