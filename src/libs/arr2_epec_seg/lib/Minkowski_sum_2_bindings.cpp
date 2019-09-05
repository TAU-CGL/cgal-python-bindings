#include <Config.hpp>
#ifdef MINKOWSKI_SUM
#include <Common.hpp>

template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_2(T1& P, T2& Q)
{
  return CGAL::minkowski_sum_2(P, Q);
}

template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_by_full_convolution_2(T1& P, T2& Q)
{
  return CGAL::minkowski_sum_by_full_convolution_2(P, Q);
}

template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_by_reduced_convolution_2(T1& P, T2& Q)
{
  return CGAL::minkowski_sum_by_reduced_convolution_2(P, Q);
}


void export_Minkowski_sum_2()
{
  using namespace boost::python;
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_2, Polygon_2>);
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_2, Polygon_with_holes_2>);
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_with_holes_2, Polygon_2>);
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_with_holes_2, Polygon_with_holes_2>);

  def("minkowski_sum_by_full_convolution_2", &minkowski_sum_by_full_convolution_2<Polygon_2, Polygon_2>);

  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_2, Polygon_2>);
  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_2, Polygon_with_holes_2>);
  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_with_holes_2, Polygon_2>);
  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_with_holes_2, Polygon_with_holes_2>);

}

#endif