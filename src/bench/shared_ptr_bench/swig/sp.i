%module swig_sp
%include <std_shared_ptr.i>
%shared_ptr(MyClass)
%{#include "sp.h"%}
%include "sp.h"
