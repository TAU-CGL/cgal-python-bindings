// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "common.hpp"
#include "CGAL/Kd_tree.h"
#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/Search_traits_3.h>
#include <CGAL/K_neighbor_search.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Euclidean_distance.h>
#include <CGAL/Fuzzy_sphere.h>
#include "General_distance_python.hpp"


typedef CGAL::Search_traits_3<Kernel> Search_traits_3;
//typedef CGAL::Orthogonal_incremental_neighbor_search<Search_traits_3> Orthogonal_incremental_neighbor_search;
//typedef Orthogonal_incremental_neighbor_search::iterator NN_iterator;
//typedef Orthogonal_incremental_neighbor_search::Tree Orthogonal_incremental_neighbor_search_tree;
typedef CGAL::Kd_tree<Search_traits_3> Kd_tree;
typedef CGAL::Sliding_midpoint<Search_traits_3> Splitter;
typedef CGAL::Fuzzy_iso_box<Search_traits_3> Fuzzy_iso_box;
typedef CGAL::Kd_tree_rectangle<FT, CGAL::Dimension_tag<3>> Kd_tree_rectangle;
typedef CGAL::K_neighbor_search<Search_traits_3> K_neighbor_search;
typedef General_distance_python<CGAL::Dimension_tag<3>, FT, Point_3, Point_3> Distance_python;
typedef CGAL::K_neighbor_search<Search_traits_3, Distance_python> K_neighbor_search_python;
typedef CGAL::Euclidean_distance<Search_traits_3> Euclidean_distance;

// performance test
/*
#define _USE_MATH_DEFINES
#include <math.h>
bool cd(Polygon_set_2& ps, Point_3& p_3, FT l, FT epsilon)
{
  auto x = p_3.x();
  auto y = p_3.y();
  auto a = p_3.z();
  auto p_2 = Point_2(x, y);
  auto r0 = Vector_2(-epsilon, epsilon);
  auto r1 = Vector_2(-epsilon, -epsilon);
  auto r2 = Vector_2(l + epsilon, -epsilon);
  auto r3 = Vector_2(l + epsilon, epsilon);
  auto p = Point_2(x, y);
  auto at = Aff_Transformation_2(Rotation(), FT(sin(to_double(a))), FT(cos(to_double(a))), FT(1));
  auto p0 = p_2 + at.transform(r0);
  auto p1 = p_2 + at.transform(r1);
  auto p2 = p_2 + at.transform(r2);
  auto p3 = p_2 + at.transform(r3);

  std::vector<Point_2> v = { p0, p1, p2, p3 };

  auto rectangle = Polygon_2(v.begin(), v.end());
  if (ps.do_intersect(rectangle)) return false;
  return true;
}

bool lp(Polygon_set_2& ps, Point_3& start, Point_3& end, FT length, FT epsilon, bool clockwise)
{
  auto x1 = start.x().exact();
  auto y1 = start.y().exact();
  auto a1 = start.z().exact();

  auto x2 = end.x().exact();
  auto y2 = end.y().exact();
  auto a2 = end.z().exact();

  if (!clockwise && a2 < a1) a1 = a1 - Gmpq(2 * M_PI);
  if (clockwise && a2 > a1) a1 = a1 + Gmpq(2 * M_PI);

  auto dx = x2 - x1;
  auto dy = y2 - y1;
  auto dz = abs((a2 - a1).to_double());

  auto sample_count = int(
    (
    sqrt(pow(dx.to_double(), 2) + pow(dy.to_double(), 2))
    + dz * (to_double(length) + to_double(epsilon))
    )
    / ((int(2)) * int(to_double(epsilon)))
    )
    + 1;

  Gmpq x, y, a;
  for (int i = 0; i < sample_count + 1; i++)
  {
    x = Gmpq(sample_count - i, sample_count) * x1 + Gmpq(i, sample_count) * x2;
    y = Gmpq(sample_count - i, sample_count) * y1 + Gmpq(i, sample_count) * y2;
    a = Gmpq(sample_count - i, sample_count) * a1 + Gmpq(i, sample_count) * a2;
    auto p = Point_3(FT(x), FT(y), FT(a));
    if (!cd(ps, p, length, epsilon)) return false;
  }
  return true;
}

*/



template <typename T>
static T* init_tree()
{
  return new T();
}

template <typename T>
static T* init_tree_from_list(bp::list& lst)
{
  auto begin = boost::python::stl_input_iterator<typename T::Point_d >(lst);
  auto end = boost::python::stl_input_iterator<typename T::Point_d >();
  return new T(begin, end);
}

template <typename T>
void tree_insert(T& tree, bp::list& lst)
{
  //copying into a vector because of an apparent bug with boost::python::stl_input_iterator
  auto begin = boost::python::stl_input_iterator<typename T::Point_d >(lst);
  auto end = boost::python::stl_input_iterator<typename T::Point_d >();
  auto v = std::vector<typename T::Point_d>(begin, end);
  tree.insert(v.begin(), v.end());
}

template <typename T, typename FQI>
void tree_search(T& tree, FQI& q, bp::list& lst)
{
  auto v = std::vector<typename T::Point_d>();
  tree.search(std::back_inserter(v), q);
  for (auto p : v)
    lst.append(p); 
}

template<typename T>
void points(T& tree, bp::list& lst)
{
  for (auto p : tree)
    lst.append(p);
}

template <typename T>
void bind_kd_tree(const char* python_name)
{
  using namespace bp;
  class_<T, boost::noncopyable>(python_name)
    //.def("__init__", make_constructor(&init_tree<T>))
    .def(init<>())
    .def("__init__", make_constructor(&init_tree_from_list<T>))
    .def("insert", static_cast<void (T::*) (const typename T::Point_d&)>(&T::insert))
    .def("insert", &tree_insert<T>)
    //.def("remove", static_cast<void (T::*) (const typename T::Point_d&) > (&T::remove)) // returning address of local variable or temporary warning
    .def("build", &T::build)
    .def("points", &points<T>)
    .def("search", &tree_search<T, Fuzzy_iso_box>)
    .def("size", &T::size)
    .def("capacity", &T::capacity)
    ;
}

template <typename T>
void k_neighbors(T& neighbor_search, bp::list& lst)
{
  for (auto it = neighbor_search.begin(); it != neighbor_search.end(); ++it)
  {
    lst.append(bp::make_tuple(it->first, it->second));
  }
}

template <typename T>
void bind_neighbor_search(const char* python_name)
{
  using namespace bp;
  class_<T>(python_name, init<const typename T::Tree&, typename T::Query_item,
   unsigned int, FT, bool, typename T::Distance, bool>())
    .def("k_neighbors", &k_neighbors<T>)
    ;
}

void export_Spatial_searching()
{
  using namespace bp;
  class_< Search_traits_3>("Search_traits_3")
    .def(init<>())
    ;

  class_<Fuzzy_iso_box>("Fuzzy_iso_box_3")
    .def(init <Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d>())
    .def(init<Fuzzy_iso_box::Point_d, Fuzzy_iso_box::Point_d, FT>())
    .def("contains", &Fuzzy_iso_box::contains)
    .def("inner_range_intersects", &Fuzzy_iso_box::inner_range_intersects)
    .def("outer_range_contains", &Fuzzy_iso_box::outer_range_contains)
    ;

  class_<Kd_tree_rectangle>("Kd_tree_rectangle")
    .def(init<int>())
    .def("min_coord", &Kd_tree_rectangle::min_coord)
    .def("max_coord", &Kd_tree_rectangle::max_coord)
    .def("set_upper_bound", &Kd_tree_rectangle::set_upper_bound)
    .def("set_lower_bound", &Kd_tree_rectangle::set_lower_bound)
    .def("max_span_coord", &Kd_tree_rectangle::max_span_coord)
    .def("max_span", &Kd_tree_rectangle::max_span)
    .def("dimension", &Kd_tree_rectangle::dimension)
    .def("split", &Kd_tree_rectangle::split)
    ;

  bind_kd_tree<Kd_tree>("Kd_tree");

  class_<Distance_python>("Distance_python")
    .def(init<bp::object, bp::object, bp::object, bp::object, bp::object>())
    .def<FT (Distance_python::*) (const Distance_python::Query_item&, const Distance_python::Point_d&)>("transformed_distance", &Distance_python::transformed_distance)
    .def("min_distance_to_rectangle", &Distance_python::min_distance_to_rectangle)
    .def("max_distance_to_rectangle", &Distance_python::max_distance_to_rectangle)
    .def<FT(Distance_python::*) (const FT&)>("transformed_distance", &Distance_python::transformed_distance)
    .def("inverse_of_transformed_distance", &Distance_python::inverse_of_transformed_distance)
    ;

  class_<Euclidean_distance>("Euclidean_distance")
    .def(init<>())
    //.def<FT (Euclidean_distance::*) (const Euclidean_distance::Query_item&, const Euclidean_distance::Point_d&) const>("transformed_distance", &Euclidean_distance::transformed_distance)
    /*.def("min_distance_to_rectangle", &Euclidean_distance::min_distance_to_rectangle)
    .def("max_distance_to_rectangle", &Euclidean_distance::max_distance_to_rectangle)
    .def("transformed_distance", &Euclidean_distance::transformed_distance)
    .def("inverse_of_transformed_distance", &Euclidean_distance::inverse_of_transformed_distance)*/
    ;

  bind_neighbor_search<K_neighbor_search_python>("K_neighbor_search_python");
  
  bind_neighbor_search<K_neighbor_search>("K_neighbor_search");

  //def("cd", &cd);
  //def("lp", &lp);
}
