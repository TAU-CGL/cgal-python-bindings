#include <nanobind/make_iterator.h>
#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Origin.h>
#include <CGAL/Point_set_3.h>

#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/internal.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

template <typename Point_set, typename Property, typename C, typename Point_set_nb>
auto define_property_map(C& c, Point_set_nb& ptst, const std::string& name) {
  using Property_map = typename Point_set::template Property_map<Property>;

  py::class_<Property_map> pm(c, ("Property_map_" + name).c_str());
  pm.def(py::init<>())
      .def("reset", &Property_map::reset)
      .def("data", &Property_map::data)
      .def(
          "transfer", [](Property_map& pm, const Property_map& other) { pm.transfer(other); }, py::arg("other"))
      .def(
          "transfer",
          [](Property_map& pm, const Property_map& other, std::size_t from, std::size_t to) {
            pm.transfer(other, from, to);
          },
          py::arg("other"), py::arg("from"), py::arg("to"))
      .def(
          "__iter__",
          [](Property_map& pm) {
            return py::make_iterator(py::type<typename Property_map::iterator>(), "Iterator", pm.begin(), pm.end());
          },
          py::keep_alive<0, 1>())
      .def(
          "__getitem__", [](Property_map& pm, std::size_t i) { return pm[i]; }, py::arg("index"))
      .def(
          "__setitem__", [](Property_map& pm, std::size_t i, const Property& p) { pm[i] = p; }, py::arg("index"),
          py::arg("value"))
      .def(
          "__getitem__", [](Property_map& pm, const typename Point_set::iterator& it) { return pm[*it]; },
          py::arg("index"))
      .def(
          "__setitem__",
          [](Property_map& pm, const typename Point_set::iterator& it, const Property& p) { pm[*it] = p; },
          py::arg("index"), py::arg("value"));

  // TODO: remove property map
  ptst.def(("add_property_map_" + name).c_str(), &Point_set::template add_property_map<Property>,
           py::arg("name") = std::string(), py::arg("default_value") = Property())
      .def(("property_map_" + name).c_str(), &Point_set::template property_map<Property>,
           py::arg("name") = std::string())
      .def(("has_property_map_" + name).c_str(), &Point_set::template has_property_map<Property>,
           py::arg("name") = std::string())
      .def("remove_property_map", &Point_set::template remove_property_map<Property>, py::arg("prop"));

  return pm;
}

template <typename Range, typename C>
auto define_range(C& c, const std::string& name) {
  py::class_<Range> r(c, name.c_str());
  r.def("size", &Range::size)
      .def("empty", &Range::empty)
      .def(
          "__iter__",
          [](Range& r) {
            return py::make_iterator(py::type<typename Range::iterator>(), "Iterator", r.begin(), r.end());
          },
          py::keep_alive<0, 1>());
  return r;
}

template <typename Push_property_map, typename C>
auto define_push_property_map(C& c, const std::string& name) {
  py::class_<Push_property_map> pm(c, name.c_str());
  pm.def(py::init<>())
      // TODO: why is get and put not working?
      ;
}

void export_3d_point_set(py::module_& m) {
  using Kernel_ = Kernel;
  using Pt_set = CGAL::Point_set_3<Kernel_::Point_3, Kernel_::Vector_3>;
  using Pnt_3 = Kernel_::Point_3;
  using Vec_3 = Kernel_::Vector_3;
  using FT = Kernel_::FT;
  using Point_set_3_index = CGAL::internal::Point_set_3_index<Pnt_3, Vec_3>;
  using Point_push_map = Pt_set::Point_push_map;
  using Point_map = Pt_set::template Property_map<Point_3>;
  using Vector_map = Pt_set::template Property_map<Vector_3>;

  py::class_<Pt_set> ptst(
      m, "Point_set_3",
      "A collection of points with dynamically associated properties.\n"
      "An instance of this class stores a set of indices of type Index, each representing a point. Properties can be "
      "associated to each point and can be retrieved using the index of the point. There are two particular properties "
      "that are hard coded by this class: the coordinates of the points and the normal vectors.\n"
      "The coordinates of a point can be accessed using the index of the point and the member function point(). This "
      "property is always present. The normal vector of a point can be accessed using the index of the point and the "
      "normal() method. This property must be explicitly created.\n"
      "All properties can be accessed as a range using the methods points(), normals(), and range() for point "
      "coordinates, normal vectors, and other properties respectively.\n"
      "Removing a point with properties is achieved by moving its index at the end of the container and keeping track "
      "of the number of removed elements. A garbage collection method must be called to really remove it from memory.\n"
      "For convenience, all functions of the package Point Set Processing automatically create the right named "
      "parameters if called with a CGAL::Point_set_3 object as argument.\n");

  // Construction, Destruction, Assignment
  ptst.def(py::init<bool>(), py::arg("with_normal_map") = false,
           "creates an empty point set with no additional property."
           "\n"
           "Parameters\n"
           "with_normal_map  True if the normal map should be added. If false (default value), the normal map can "
           "still be added later on (see add_normal_map()).")

      // Related Functions
      .def(
          "__iadd__", [](Pt_set& ps, Pt_set& other) { return ps += other; }, py::arg("other"),
          "Append other at the end of ps.\n"
          "Shifts the indices of points of other by ps.number_of_points() + other.number_of_points().\n"
          "Copies entries of all property maps which have the same name in ps and other. Property maps which are only "
          "in other are ignored.\n"
          "\n"
          "Note\n"
          "Garbage is collected in both point sets when calling this method.",
          py::is_operator())
      // extraction
      // insertion
      // insertion

      // Member Functions
      .def(
          "add_normal_map",
          [](Pt_set& ps, const Vec_3& default_value = Vector_3(0, 0, 0)) { ps.add_normal_map(default_value); },
          py::arg("default_value") = Vector_3(0, 0, 0),
          "Convenience method that adds a normal property.\n"
          "This method adds a property of type Vector and named normal.\n"
          "\n"
          "Returns\n"
          "Returns a pair containing the normal map and a Boolean that is True if the property was added and false if "
          "it already exists (and was therefore not added but only returned). ")
      .def("cancel_removals", &Pt_set::cancel_removals,
           "restores all removed points.\n"
           "After removing one or several points, calling this method restores the point set to its initial state: "
           "points that were removed (and their associated properties) are restored.\n"
           "\n"
           "Note\n"
           "This method is only guaranteed to work if no point was inserted after the removal: otherwise, some points "
           "might not be restored.\n"
           "If collect_garbage() was called after removal, the points are irremediably lost and nothing will be "
           "restored. ")
      .def("clear", &Pt_set::clear,
           "Clears the point set properties and content.\n"
           "After calling this method, the object is the same as a newly constructed object. The additional properties "
           "(such as normal vectors) are also removed and must thus be re-added if needed.")
      .def("clear_properties", &Pt_set::clear_properties,
           "Clears all properties created.\n"
           "After calling this method, all properties are removed. The points are left unchanged.")
      .def("copy_properties", &Pt_set::copy_properties, py::arg("other"),
           "Copies the properties from another point set.\n"
           "All properties from other that do not already exist in this point set are added and filled to their "
           "default values. Properties that exist in both point sets are left unchanged.")
      .def("garbage_size", &Pt_set::garbage_size, "Number of removed points.\n")
      .def("has_garbage", &Pt_set::has_garbage,
           "returns True if there are elements marked as removed, False otherwise.")
      .def("collect_garbage", &Pt_set::collect_garbage, "erases from memory the elements marked as removed.")
      .def("has_normal_map", &Pt_set::has_normal_map,
           "Convenience method that tests whether the point set has normals.\n"
           "This method tests whether a property of type Vector and named normal exists.")
      .def(
          "insert", [](Pt_set& ps) { return ps.insert(); },
          "inserts a new element with default property values.\n"
          "\n"
          "Note\n"
          "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. "
          "Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are "
          "guaranteed to keep referring to the same elements they were referring to before the call.",
          py::keep_alive<1, 0>())
      .def(
          "insert", [](Pt_set& ps, const Pnt_3& p) { return ps.insert(p); }, py::arg("p"),
          "inserts new point with default property values.\n"
          "\n"
          "Parameters\n"
          "p  Point to insert\n"
          "\n"
          "Note\n"
          "Properties of the added point are initialized to their default value.\n"
          "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. "
          "Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are "
          "guaranteed to keep referring to the same elements they were referring to before the call.",
          py::keep_alive<1, 0>())
      .def(
          "insert", [](Pt_set& ps, const Pnt_3& p, const Vec_3& n) { return ps.insert(p, n); }, py::arg("p"),
          py::arg("n"),
          "Convenience method to add a point with a normal vector.\n"
          "\n"
          "Parameters\n"
          "p  Point to insert\n"
          "n  Associated normal vector\n"
          "\n"
          "Note\n"
          "Properties of the added point other than its normal vector are initialized to their default value.\n"
          "If not already added, a normal property is automatically added to the point set when using this method. The "
          "default value for normal vectors is CGAL::NULL_VECTOR.\n"
          "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. "
          "Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are "
          "guaranteed to keep referring to the same elements they were referring to before the call.\n")
      .def(
          "insert", [](Pt_set& ps, const Pt_set& other, const std::size_t idx) { return ps.insert(other, idx); },
          py::arg("other"), py::arg("idx"),
          "Convenience method to copy a point with all its properties from another point set.\n"
          "In the case where two point sets have the same properties, this method allows the user to easily copy one "
          "point (along with the values of all its properties) from one point set to another.\n"
          "\n"
          "Parameters\n"
          "other  Point set to which the point to copy belongs\n"
          "idx  Index of the point to copy in other\n"
          "\n"
          "Warning\n"
          "This point set and other must have the exact same properties, with the exact same names and types in the "
          "exact same order.\n"
          "\n"
          "Note\n"
          "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. "
          "Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are "
          "guaranteed to keep referring to the same elements they were referring to before the call.\n",
          py::keep_alive<1, 0>())
      .def(
          "insert", [](Pt_set& pw, const Pt_set& other, const Point_set_3_index& idx) { return pw.insert(other, idx); },
          py::arg("other"), py::arg("idx"),
          "Convenience method to copy a point with all its properties from another point set.\n"
          "In the case where two point sets have the same properties, this method allows the user to easily copy one "
          "point (along with the values of all its properties) from one point set to another.\n"
          "\n"
          "Parameters\n"
          "other  Point set to which the point to copy belongs\n"
          "idx  Index of the point to copy in other\n"
          "\n"
          "Warning\n"
          "This point set and other must have the exact same properties, with the exact same names and types in the "
          "exact same order.\n"
          "\n"
          "Note\n"
          "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. "
          "Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are "
          "guaranteed to keep referring to the same elements they were referring to before the call.\n",
          py::keep_alive<1, 0>())
      .def("is_empty", &Pt_set::is_empty,
           "returns True if the number of elements not marked as removed is 0, False otherwise.\n"
           "\n"
           "Note\n"
           "This does not count the removed elements.\n"
           "The method empty() is also available (see Range) and does the same thing.")
      .def(
          "is_removed", [](Pt_set& ps, const Pt_set::iterator it) { return ps.is_removed(it); }, py::arg("it"),
          "returns True if the element is marked as removed, False otherwise.\n"
          "\n"
          "Note\n"
          "When iterating between begin() and end(), no element marked as removed can be found.")
      .def(
          "is_removed", [](Pt_set& ps, const std::size_t idx) { return ps.is_removed(idx); }, py::arg("index"),
          "returns True if the element is marked as removed, False otherwise.\n"
          "\n"
          "Note\n"
          "When iterating between begin() and end(), no element marked as removed can be found.")
      .def(
          "is_removed", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.is_removed(idx); }, py::arg("index"),
          "returns True if the element is marked as removed, False otherwise.\n"
          "\n"
          "Note\n"
          "When iterating between begin() and end(), no element marked as removed can be found.")
      .def("join", &Pt_set::join, py::arg("other"),
           "merges other in the point set.\n"
           "Shifts the indices of points of other by number_of_points() + other.number_of_points().\n"
           "Copies entries of all property maps which have the same name in the point set and other. Property maps "
           "which are only in other are ignored.\n"
           "\n"
           "Note\n"
           "If copy_properties() with other as argument is called before calling this method, then all the content of "
           "other will be copied and no property will be lost in the process.\n"
           "Garbage is collected in both point sets when calling this method.")
      .def(
          "begin", [](Pt_set& ps) { return ps.begin(); }, "returns the begin iterator.\n", py::keep_alive<1, 0>())
      .def(
          "end", [](Pt_set& ps) { return ps.end(); }, "returns the past-the-end iterator.\n", py::keep_alive<1, 0>())
      .def(
          "point", [](Pt_set& ps, const std::size_t idx) { return ps.point(idx); }, py::arg("index"),
          "returns a reference to the point corresponding to index.\n")
      .def(
          "point", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.point(idx); }, py::arg("index"),
          "returns a reference to the point corresponding to index.\n")
      .def(
          "set_point", [](Pt_set& ps, const std::size_t idx, const Point_3& p) { ps.point(idx) = p; }, py::arg("index"),
          py::arg("p"), "sets the point corresponding to index to p.\n")
      .def(
          "set_point", [](Pt_set& pw, const Point_set_3_index& idx, const Point_3& p) { pw.point(idx) = p; },
          py::arg("index"), py::arg("p"), "sets the point corresponding to index to p.\n")
      .def(
          "set_point", [](Pt_set& pw, const Pt_set::iterator it, const Point_3& p) { pw.point(*it) = p; },
          py::arg("it"), py::arg("p"), "sets the point corresponding to index to p.\n")
      .def(
          "normal", [](Pt_set& ps, const std::size_t idx) { return ps.normal(idx); }, py::arg("index"),
          "returns a reference to the normal corresponding to index.\n"
          "\n"
          "Note\n"
          "If not already added, a normal property is automatically added to the point set (see add_normal_map()). ")
      .def(
          "normal", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.normal(idx); }, py::arg("index"),
          "returns a reference to the normal corresponding to index.\n"
          "\n"
          "Note\n"
          "If not already added, a normal property is automatically added to the point set (see add_normal_map()). ")
      .def(
          "set_normal", [](Pt_set& ps, const std::size_t idx, const Vector_3& n) { ps.normal(idx) = n; },
          py::arg("index"), py::arg("n"), "sets the normal corresponding to index to n.\n")
      .def(
          "set_normal", [](Pt_set& pw, const Point_set_3_index& idx, const Vector_3& n) { pw.normal(idx) = n; },
          py::arg("index"), py::arg("n"), "sets the normal corresponding to index to n.\n")
      .def(
          "set_normal", [](Pt_set& pw, const Pt_set::iterator it, const Vector_3& n) { pw.normal(*it) = n; },
          py::arg("it"), py::arg("n"), "sets the normal corresponding to index to n.\n")
      .def(
          "normal_map", [](Pt_set& ps) { return ps.normal_map(); },
          "returns the property map of the normal property.\n"
          "\n"
          "Note\n"
          "If the normal property has not been added yet to the point set before calling this method, the property map "
          "is automatically added with add_normal_map(). ")
      // TODO: normal push map?
      .def("number_of_points", &Pt_set::number_of_points,
           "returns the number of elements in the point set."
           "\n"
           "Note\n"
           "See number_of_removed_points() for getting the number of elements marked as removed."
           "The method size() is also available (see Range) and does the same thing. ")
      .def("number_of_removed_points", &Pt_set::number_of_removed_points,
           "returns the number of elements marked as removed."
           "\n"
           "See also\n"
           "garbage_size()")
      .def(
          "remove", [](Pt_set& ps, Pt_set::iterator first, Pt_set::iterator last) { return ps.remove(first, last); },
          py::arg("first"), py::arg("last"),
          "marks all elements between first and last as removed.\n"
          "\n"
          "Note\n"
          "The elements are just marked as removed and are not erased from the memory. collect_garbage() should be "
          "called if the memory needs to be disallocated. Elements can be recovered with cancel_removals().\n"
          "All iterators, pointers and references related to the container are invalidated. ")
      .def(
          "remove", [](Pt_set& ps, Pt_set::iterator it) { return ps.remove(it); }, py::arg("index"),
          "marks the element as removed.\n"
          "\n"
          "Note\n"
          "The element is not actually removed from the point set, but only marked as removed. The element can be "
          "restored with cancel_removals(). ")
      .def(
          "remove", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.remove(idx); }, py::arg("index"),
          "marks the element as removed.\n"
          "\n"
          "Note\n"
          "The element is not actually removed from the point set, but only marked as removed. The element can be "
          "restored with cancel_removals(). ")
      .def("remove_normal_map", &Pt_set::remove_normal_map,
           "removes the normal property.\n"
           "Returns\n"
           "True if the property was removed and False if it was not found.")
      .def(
          "point_map", [](Pt_set& ps) { return ps.point_map(); }, "returns the property map of the point property.")
      .def("properties", &Pt_set::properties, "returns a list with all strings that describe properties")
      .def("properties_and_types", &Pt_set::properties_and_types,
           "returns a list of pairs that describe properties and associated types.")
      .def("reserve", &Pt_set::reserve, py::arg("index"),
           "restores the element marked as removed.\n"
           "\n"
           "Parameters\n"
           "s Index of the element to restore\n"
           "\n"
           "Note\n"
           "This method does not change the content of the point set and is only used for optimization.")
      .def("resize", &Pt_set::resize, py::arg("index"),
           "changes size of the point set.\n"
           "\n"
           "Parameters\n"
           "s Target size of the point set\n"
           "\n"
           "Note\n"
           "If the given size is larger than the current size, the capacity of the internal container is extended. If "
           "there are element marked as removed, they may be overwritten. If the given size is smaller than the "
           "current size, garbage is collected and the container is resized.")

      // Ranges
      .def("points", &Pt_set::points, "returns a constant range of points.")
      .def("normals", &Pt_set::normals, "returns a constant range of normals.")

      // Push Property Maps and Inserters (Advanced)
      .def("point_push_map", &Pt_set::point_push_map, "returns the push property map of the point property.")
      .def("normal_push_map", &Pt_set::normal_push_map, "returns the push property map of the vector property.")

      // Undocumented
      .def("size", &Pt_set::size)
      .def("empty", &Pt_set::empty)
      .def("info", &Pt_set::info)
      .def(
          "__iter__",
          [](Pt_set& ps) {
            return py::make_iterator(py::type<typename Pt_set::iterator>(), "Iterator", ps.begin(), ps.end());
          },
          py::keep_alive<0, 1>());

  add_insertion(ptst, "__str__");
  add_insertion(ptst, "__repr__");
  add_extraction(ptst);

  define_property_map<Pt_set, Vec_3>(m, ptst, "vector");
  define_property_map<Pt_set, CGAL::IO::Color>(m, ptst, "color");
  define_property_map<Pt_set, FT>(m, ptst, "FT");
  define_property_map<Pt_set, Pnt_3>(m, ptst, "point");
  define_property_map<Pt_set, std::size_t>(m, ptst, "index");
  define_range<typename Pt_set::Point_range>(m, "Point_set_point_range");
  define_range<typename Pt_set::Vector_range>(m, "Point_set_vector_range");

  m.def(
      "read_point_set",
      [](const std::string& fname, Pt_set& ps, const py::dict& np = py::dict()) {
        return CGAL::IO::read_point_set<Pnt_3, Vec_3>(fname, ps, internal::parse_named_parameters(np));
      },
      py::arg("fname"), py::arg("ps"), py::arg("np") = py::dict(),
      "reads the point set from an input file.\n"
      "Supported file formats are the following:\n"
      "\n"
      "Object File Format (OFF) (.off)\n"
      "Polygon File Format (PLY) (.ply)\n"
      "LAS (Lidar) File Format (.las)\n"
      "XYZ File Format (.xyz)\n"
      "\n"
      "The format is detected from the filename extension (letter case is not important). If the file contains normal "
      "vectors, the normal map is added to the point set. For PLY input, all point properties found in the header are "
      "added.\n"
      "\n"
      "Parameters\n"
      "fname	name of the input file\n"
      "ps	the point set\n"
      "np	an optional sequence of Named Parameters among the ones listed below\n"
      "\n"
      "Optional Named Parameters\n"
      "use_binary_mode (bool) = True\n"
      "\n"
      "Returns\n"
      "True if the reading was successful, False otherwise.");
  m.def(
      "write_point_set",
      [](const std::string& fname, Pt_set& ps, const py::dict& np = py::dict()) {
        return CGAL::IO::write_point_set<Pnt_3, Vec_3>(fname, ps, internal::parse_named_parameters(np));
      },
      py::arg("fname"), py::arg("ps"), py::arg("np") = py::dict(),
      "writes the point set in an output file.\n"
      "Supported file formats are the following:\n"
      "\n"
      "Object File Format (OFF) (.off)\n"
      "Polygon File Format (PLY) (.ply)\n"
      "LAS (Lidar) File Format (.las)\n"
      "XYZ File Format (.xyz)\n"
      "\n"
      "The format is detected from the filename extension (letter case is not important).\n"
      "\n"
      "Parameters\n"
      "fname	name of the output file\n"
      "ps	the point set\n"
      "np	an optional sequence of Named Parameters among the ones listed below\n"
      "\n"
      "Optional Named Parameters\n"
      "use_binary_mode (bool) = True\n"
      "\n"
      "Returns True if the writing was successful, False otherwise.");

  py::class_<Point_set_3_index> ptst_idx(m, "Point_set_3_index");
  ptst_idx.def(py::init<const Point_set_3_index&>())
      .def(py::init_implicit<const std::size_t&>())
      .def(py::init<>())
      .def("__int__", [](Point_set_3_index& index) { return static_cast<std::size_t>(index); })
      .def(py::self == py::self, py::sig("def __eq__(self, arg: object, /) -> bool"))
      .def(py::self != py::self, py::sig("def __ne__(self, arg: object, /) -> bool"))
      .def(py::self < py::self)
      .def("next", [](Point_set_3_index& index) { return ++index; })
      .def("prev", [](Point_set_3_index& index) { return --index; })
      // operator with an integer
      .def(
          "__add__", [](Point_set_3_index& index, const int& value) { return index + value; }, py::is_operator())
      .def("__sub__", [](Point_set_3_index& index, const int& value) { return index - value; }, py::is_operator());

  add_insertion(ptst_idx, "__str__");
  add_insertion(ptst_idx, "__repr__");

  py::class_<Pt_set::iterator> ptst_it(m, "Point_set_3_iterator");
  ptst_it.def(py::init<>())
      .def(
          "__add__", [](Pt_set::iterator& it, const int& value) { return it + value; }, py::is_operator())
      .def("__sub__", [](Pt_set::iterator& it, const int& value) { return it - value; }, py::is_operator());

  define_push_property_map<Pt_set::Push_property_map<Point_map>>(m, "Point_push_map");
  define_push_property_map<Pt_set::Push_property_map<Vector_map>>(m, "Vector_push_map");
}
