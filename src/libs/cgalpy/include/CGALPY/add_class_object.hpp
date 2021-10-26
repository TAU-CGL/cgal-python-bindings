// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ADD_CLASS_OBJECT_HPP
#define CGALPY_ADD_CLASS_OBJECT_HPP

template <typename Type, const char* Name, bool no_init>
struct New_class_object {};

template<typename Type, const char* Name>
struct New_class_object<Type, Name, false> {
  bp::class_<Type>* operator()() {
    return new bp::class_<Type>(Name);
  }
};

template<typename Type, const char* Name>
struct New_class_object<Type, Name, true> {
  bp::class_<Type>* operator()() {
    return new bp::class_<Type>(Name, bp::no_init);
  }
};

// Introduce a new class object
// Return true iff the class object was not already registered
template <typename Type, const char* Name, bool no_init=false>
bool add_class_object(bp::scope& my_scope, bp::class_<Type>*& co) {
  bp::handle<> tco(bp::objects::registered_class_object(bp::type_id<Type>()));
  if (tco.get() != 0) {
    my_scope.attr(Name) = tco;
    return false;
  } else {
    co = New_class_object<Type, Name, no_init>()();
  }
  return true;
}

#endif // CGALPY_ADD_CLASS_OBJECT_HPP