#include "CGALPY/Python_functor.hpp"

#include <CGAL/Kd_tree_rectangle.h>

template <typename D_, typename FT_, typename Point_d_, typename Query_item_>
class General_distance_python
{

public:
  typedef D_ D;
  typedef FT_ FT;
  typedef Point_d_ Point_d;
  typedef Query_item_ Query_item;
  typedef CGAL::Kd_tree_rectangle<FT, D> Kd_tree_rectangle;

private:

  Python_functor_2_ref<Query_item, Point_d, FT> f0;
  Python_functor_2_ref<Query_item, Kd_tree_rectangle, FT> f1;
  Python_functor_2_ref<Query_item, Kd_tree_rectangle, FT> f2;
  Python_functor_1<FT, FT> f3;
  Python_functor_1<FT, FT> f4;

public:
  General_distance_python() {}

  General_distance_python(bp::object py_functor0, bp::object py_functor1,
    bp::object py_functor2, bp::object py_functor3,
    bp::object py_functor4)
  {
    f0 = Python_functor_2_ref<Query_item, Point_d, FT>(py_functor0);
    f1 = Python_functor_2_ref<Query_item, Kd_tree_rectangle, FT>(py_functor1);
    f2 = Python_functor_2_ref<Query_item, Kd_tree_rectangle, FT>(py_functor2);
    f3 = Python_functor_1<FT, FT>(py_functor3);
    f4 = Python_functor_1<FT, FT>(py_functor4);
  }

  FT transformed_distance(const Query_item& q, const Point_d& r) const
  {
    return f0(q, r);
  }

  FT min_distance_to_rectangle(const Query_item& q, const Kd_tree_rectangle& r) const
  {
    return f1(q, r);
  }

  FT max_distance_to_rectangle(const Query_item& q, const Kd_tree_rectangle& r) const
  {
    return f2(q, r);
  }

  FT transformed_distance(const FT& d) const
  {
    return f3(d);
  }

  FT inverse_of_transformed_distance(const FT& d) const
  {
    return f4(d);
  }
};
