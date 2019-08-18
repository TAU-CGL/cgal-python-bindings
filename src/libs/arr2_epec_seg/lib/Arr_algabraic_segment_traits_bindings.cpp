#include <Common.h>
#ifdef ARR_ALGEBRAIC_SEGMENT_TRAITS

bp::list to_double(TPoint_2& p)
{
  auto lst = bp::list();
  auto pair = p.to_double();
  lst.append(pair.first);
  lst.append(pair.second);
  return lst;
}

void export_Arr_linear_traits()
{
  using namespace boost::python;
  class_<TPoint_2>("TPoint_2")
    .def(init<TPoint_2&>())
    .def("curve", &TPoint_2::curve)
    .def("arcno", &TPoint_2::arcno)
    .def("to_double", &to_double)
    ;


}

#endif
