%module swig_sp
%include <std_shared_ptr.i>
%shared_ptr(MyClass)
%{
#include "swig_sp.h"
%}

%include "swig_sp.h"