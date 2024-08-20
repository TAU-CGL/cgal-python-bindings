#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>

#include <CGAL/Point_set_3.h>
#include <CGAL/Origin.h>

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
    .def("transfer", [](Property_map& pm, const Property_map& other) { pm.transfer(other); },
         py::arg("other"))
    .def("transfer", [](Property_map& pm, const Property_map& other, std::size_t from, std::size_t to) { pm.transfer(other, from, to); },
         py::arg("other"), py::arg("from"), py::arg("to"))
    .def("__iter__", [](Property_map& pm) { return py::make_iterator(py::type<typename Property_map::iterator>(), "Iterator", pm.begin(), pm.end()); },
         py::keep_alive<0, 1>())
    ;

  // TODO: remove property map
  ptst.def(("add_property_map_" + name).c_str(), &Point_set::template add_property_map<Property>,
           py::arg("name") = std::string(), py::arg("default_value") = Property())
    .def(("property_map_" + name).c_str(), &Point_set::template property_map<Property>,
         py::arg("name") = std::string())
    .def(("has_property_map_" + name).c_str(), &Point_set::template has_property_map<Property>,
         py::arg("name") = std::string())
    .def("remove_property_map", &Point_set::template remove_property_map<Property>,
         py::arg("prop"))
    ;

  return pm;
}

template <typename Range, typename C>
auto define_range(C& c, const std::string& name) {
  py::class_<Range> r(c, name.c_str());
   r.def("size", &Range::size)
    .def("empty", &Range::empty)
    .def("__iter__", [](Range& r) { return py::make_iterator(py::type<typename Range::iterator>(), "Iterator", r.begin(), r.end()); },
         py::keep_alive<0, 1>())
    ;
  return r;
}

void export_3d_point_set(py::module_& m) {
  using Kernel_ = Kernel;
  using Pt_set = CGAL::Point_set_3<Kernel_::Point_3, Kernel_::Vector_3>;
  using Pnt_3 = Kernel_::Point_3;
  using Vec_3 = Kernel_::Vector_3;
  using FT = Kernel_::FT;
  using Point_set_3_index = CGAL::internal::Point_set_3_index<Pnt_3, Vec_3>;

  py::class_<Pt_set> ptst(m, "Point_set_3",
    "A collection of points with dynamically associated properties.\n"
    "An instance of this class stores a set of indices of type Index, each representing a point. Properties can be associated to each point and can be retrieved using the index of the point. There are two particular properties that are hard coded by this class: the coordinates of the points and the normal vectors.\n"
    "The coordinates of a point can be accessed using the index of the point and the member function point(). This property is always present. The normal vector of a point can be accessed using the index of the point and the normal() method. This property must be explicitly created.\n"
    "All properties can be accessed as a range using the methods points(), normals(), and range() for point coordinates, normal vectors, and other properties respectively.\n"
    "Removing a point with properties is achieved by moving its index at the end of the container and keeping track of the number of removed elements. A garbage collection method must be called to really remove it from memory.\n"
    "For convenience, all functions of the package Point Set Processing automatically create the right named parameters if called with a CGAL::Point_set_3 object as argument.\n");

    // Construction, Destruction, Assignment
  ptst.def(py::init<bool>(),
           py::arg("with_normal_map")=false,
      "creates an empty point set with no additional property."
      "\n"
      "Parameters\n"
      "with_normal_map  True if the normal map should be added. If false (default value), the normal map can still be added later on (see add_normal_map()).")

    // Related Functions
    .def("__iadd__", [](Pt_set& ps, Pt_set& other) { return ps += other; },
       py::arg("other"),
      "Append other at the end of ps.\n"
      "Shifts the indices of points of other by ps.number_of_points() + other.number_of_points().\n"
      "Copies entries of all property maps which have the same name in ps and other. Property maps which are only in other are ignored.\n"
      "\n"
      "Note\n"
      "Garbage is collected in both point sets when calling this method.",
       py::is_operator())
    // extraction
    // insertion
    // insertion
    
    // Member Functions
    // NULL_VECTOR breaks this:
    // .def("add_normal_map", [](Pt_set& ps, const Vec_3& default_value=CGAL::Null_vector()) { ps.add_normal_map(default_value); },
    //      py::arg("default_value")=CGAL::Null_vector(),
    //   "Convenience method that adds a normal property.\n"
    //   "This method adds a property of type Vector and named normal.\n"
    //   "\n"
    //   "Returns\n"
    //   "Returns a pair containing the normal map and a Boolean that is True if the property was added and false if it already exists (and was therefore not added but only returned). ")
    .def("cancel_removals", &Pt_set::cancel_removals,
      "restores all removed points.\n"
      "After removing one or several points, calling this method restores the point set to its initial state: points that were removed (and their associated properties) are restored.\n"
      "\n"
      "Note\n"
      "This method is only guaranteed to work if no point was inserted after the removal: otherwise, some points might not be restored.\n"
      "If collect_garbage() was called after removal, the points are irremediably lost and nothing will be restored. ")
    .def("clear", &Pt_set::clear,
      "Clears the point set properties and content.\n"
      "After calling this method, the object is the same as a newly constructed object. The additional properties (such as normal vectors) are also removed and must thus be re-added if needed.")
    .def("clear_properties", &Pt_set::clear_properties,
      "Clears all properties created.\n"
      "After calling this method, all properties are removed. The points are left unchanged.")
    .def("copy_properties", &Pt_set::copy_properties,
      py::arg("other"),
      "Copies the properties from another point set.\n"
      "All properties from other that do not already exist in this point set are added and filled to their default values. Properties that exist in both point sets are left unchanged.")
    .def("garbage_size", &Pt_set::garbage_size,
      "Number of removed points.\n")
    .def("has_garbage", &Pt_set::has_garbage,
      "returns True if there are elements marked as removed, False otherwise.")
    .def("collect_garbage", &Pt_set::collect_garbage,
         "erases from memory the elements marked as removed.")
    .def("has_normal_map", &Pt_set::has_normal_map,
      "Convenience method that tests whether the point set has normals.\n"
      "This method tests whether a property of type Vector and named normal exists.")
    .def("insert", [](Pt_set& ps) { ps.insert(); },
      "inserts a new element with default property values.\n"
      "\n"
      "Note\n"
      "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call. ")
    .def("insert", [](Pt_set& ps, const Pnt_3& p) { ps.insert(p); },
      py::arg("p"),
      "inserts new point with default property values.\n"
      "\n"
      "Parameters\n"
      "p  Point to insert\n"
      "\n"
      "Note\n"
      "Properties of the added point are initialized to their default value.\n"
      "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.\n")
    .def("insert", [](Pt_set& ps, const Pnt_3& p, const Vec_3& n) { ps.insert(p, n); },
      py::arg("p"), py::arg("n"),
      "Convenience method to add a point with a normal vector.\n"
      "\n"
      "Parameters\n"
      "p  Point to insert\n"
      "n  Associated normal vector\n"
      "\n"
      "Note\n"
      "Properties of the added point other than its normal vector are initialized to their default value.\n"
      "If not already added, a normal property is automatically added to the point set when using this method. The default value for normal vectors is CGAL::NULL_VECTOR.\n"
      "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.\n")
    .def("insert", [](Pt_set& ps, const Pt_set& other, const std::size_t idx) { ps.insert(other, idx); },
      py::arg("other"), py::arg("idx"),
      "Convenience method to copy a point with all its properties from another point set.\n"
      "In the case where two point sets have the same properties, this method allows the user to easily copy one point (along with the values of all its properties) from one point set to another.\n"
      "\n"
      "Parameters\n"
      "other  Point set to which the point to copy belongs\n"
      "idx  Index of the point to copy in other\n"
      "\n"
      "Warning\n"
      "This point set and other must have the exact same properties, with the exact same names and types in the exact same order.\n"
      "\n"
      "Note\n"
      "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.\n")
    .def("insert", [](Pt_set& pw, const Pt_set& other, const Point_set_3_index& idx) { return pw.insert(other, idx); },
         py::arg("other"), py::arg("idx"),
      "Convenience method to copy a point with all its properties from another point set.\n"
      "In the case where two point sets have the same properties, this method allows the user to easily copy one point (along with the values of all its properties) from one point set to another.\n"
      "\n"
      "Parameters\n"
      "other  Point set to which the point to copy belongs\n"
      "idx  Index of the point to copy in other\n"
      "\n"
      "Warning\n"
      "This point set and other must have the exact same properties, with the exact same names and types in the exact same order.\n"
      "\n"
      "Note\n"
      "If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.\n")
    .def("is_empty", &Pt_set::is_empty,
      "returns True if the number of elements not marked as removed is 0, False otherwise.\n"
      "\n"
      "Note\n"
      "This does not count the removed elements.\n"
      "The method empty() is also available (see Range) and does the same thing. ")
    .def("is_removed", [](Pt_set& ps, const std::size_t idx) { return ps.is_removed(idx); },
      py::arg("index"),
      "returns True if the element is marked as removed, False otherwise.\n"
      "\n"
      "Note\n"
      "When iterating between begin() and end(), no element marked as removed can be found. ")
    .def("is_removed", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.is_removed(idx); },
      py::arg("index"),
      "returns True if the element is marked as removed, False otherwise.\n"
      "\n"
      "Note\n"
      "When iterating between begin() and end(), no element marked as removed can be found. ")
    .def("join", &Pt_set::join,
      py::arg("other"),
      "merges other in the point set.\n"
      "Shifts the indices of points of other by number_of_points() + other.number_of_points().\n"
      "Copies entries of all property maps which have the same name in the point set and other. Property maps which are only in other are ignored.\n"
      "\n"
      "Note\n"
      "If copy_properties() with other as argument is called before calling this method, then all the content of other will be copied and no property will be lost in the process.\n"
      "Garbage is collected in both point sets when calling this method. ")
    .def("point", [](Pt_set& ps, const std::size_t idx) { return ps.point(idx); },
         py::arg("index"),
         "returns a reference to the point corresponding to index.\n")
    .def("point", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.point(idx); },
         py::arg("index"),
         "returns a reference to the point corresponding to index.\n")
    .def("normal", [](Pt_set& ps, const std::size_t idx) { return ps.normal(idx); },
      py::arg("index"),
      "returns a reference to the normal corresponding to index.\n"
      "\n"
      "Note\n"
      "If not already added, a normal property is automatically added to the point set (see add_normal_map()). ")
    .def("normal", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.normal(idx); },
         py::arg("index"),
         "returns a reference to the normal corresponding to index.\n"
         "\n"
         "Note\n"
         "If not already added, a normal property is automatically added to the point set (see add_normal_map()). ")
    .def("normal_map", [](Pt_set& ps) { return ps.normal_map(); },
      "returns the property map of the normal property.\n"
      "\n"
      "Note\n"
      "If the normal property has not been added yet to the point set before calling this method, the property map is automatically added with add_normal_map(). ")
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
    .def("remove", [](Pt_set& ps, const std::size_t idx) { return ps.remove(idx); },
      py::arg("index"),
      "marks the element as removed.\n"
      "\n"
      "Note\n"
      "The element is not actually removed from the point set, but only marked as removed. The element can be restored with cancel_removals(). ")
    .def("remove", [](Pt_set& pw, const Point_set_3_index& idx) { return pw.remove(idx); },
      py::arg("index"),
      "marks the element as removed.\n"
      "\n"
      "Note\n"
      "The element is not actually removed from the point set, but only marked as removed. The element can be restored with cancel_removals(). ")
    .def("remove_normal_map", &Pt_set::remove_normal_map,
      "removes the normal property.\n"
      "Returns\n"
      "True if the property was removed and False if it was not found.")
    .def("point_map", [](Pt_set& ps) { return ps.point_map(); },
      "returns the property map of the point property.")
    .def("properties", &Pt_set::properties,
      "returns a list with all strings that describe properties")
    .def("properties_and_types", &Pt_set::properties_and_types,
      "returns a list of pairs that describe properties and associated types.")
    .def("reserve", &Pt_set::reserve,
      py::arg("index"),
      "restores the element marked as removed.\n"
      "\n"
      "Parameters\n"
      "s Index of the element to restore\n"
      "\n"
      "Note\n"
      "This method does not change the content of the point set and is only used for optimization.")
    .def("resize", &Pt_set::resize,
      py::arg("index"),
      "changes size of the point set.\n"
      "\n"
      "Parameters\n"
      "s Target size of the point set\n"
      "\n"
      "Note\n"
      "If the given size is larger than the current size, the capacity of the internal container is extended. If there are element marked as removed, they may be overwritten. If the given size is smaller than the current size, garbage is collected and the container is resized.")

    // Ranges
    .def("points", &Pt_set::points,
      "returns a constant range of points.")
    .def("normals", &Pt_set::normals,
      "returns a constant range of normals.")

    // Undocumented
    .def("size", &Pt_set::size)
    .def("empty", &Pt_set::empty)
    .def("info", &Pt_set::info)
    .def("__iter__", [](Pt_set& ps) { return py::make_iterator(py::type<typename Pt_set::iterator>(), "Iterator", ps.begin(), ps.end()); },
         py::keep_alive<0, 1>())
    ;

  define_property_map<Pt_set, Vec_3>(m, ptst, "vector");
  define_property_map<Pt_set, CGAL::IO::Color>(m, ptst, "color");
  define_property_map<Pt_set, FT>(m, ptst, "FT");
  define_range<typename Pt_set::Point_range>(m, "Point_set_point_range");
  define_range<typename Pt_set::Vector_range>(m, "Point_set_vector_range");

  m.def("read_point_set", [](const std::string& fname, Pt_set& ps, const py::dict& np = py::dict()) {
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
        "The format is detected from the filename extension (letter case is not important). If the file contains normal vectors, the normal map is added to the point set. For PLY input, all point properties found in the header are added.\n"
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
        "True if the reading was successful, False otherwise."
        );
  m.def("write_point_set", [](const std::string& fname, Pt_set& ps, const py::dict& np = py::dict()) {
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
        "Returns True if the writing was successful, False otherwise."
        );

  add_insertion(ptst, "__str__");
  add_insertion(ptst, "__repr__");
  add_extraction(ptst);

  py::class_<Point_set_3_index> ptst_idx(m, "Point_set_3_index");
  ptst_idx
    .def(py::init<const Point_set_3_index&>())
    .def(py::init_implicit<const std::size_t&>())
    .def(py::init<>())
    .def("__int__", [](Point_set_3_index& index) { return static_cast<std::size_t>(index); })
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def("next", [](Point_set_3_index& index) { return ++index; })
    .def("prev", [](Point_set_3_index& index) { return --index; })
    ;

  add_insertion(ptst_idx, "__str__");
  add_insertion(ptst_idx, "__repr__");
}

// Definition
// template<typename Point, typename Vector>
// class CGAL::Point_set_3< Point, Vector >
//
// A collection of points with dynamically associated properties.
//
// An instance of this class stores a set of indices of type Index, each representing a point. Properties can be associated to each point and can be retrieved using the index of the point. There are two particular properties that are hard coded by this class: the coordinates of the points and the normal vectors.
//
// The coordinates of a point can be accessed using the index of the point and the member function point(). This property is always present. The normal vector of a point can be accessed using the index of the point and the normal() method. This property must be explicitly created.
//
// All properties can be accessed as a range using the methods points(), normals(), and range() for point coordinates, normal vectors, and other properties respectively.
//
// Removing a point with properties is achieved by moving its index at the end of the container and keeping track of the number of removed elements. A garbage collection method must be called to really remove it from memory.
//
// For convenience, all functions of the package Point Set Processing automatically create the right named parameters if called with a CGAL::Point_set_3 object as argument.
//
// Template Parameters
//     Point	Point type
//     Vector	Normal vector type
//
// Is model of
//     Range 
//
// Examples
//     Point_set_3/draw_point_set_3.cpp, Point_set_3/point_set.cpp, Point_set_3/point_set_advanced.cpp, Point_set_3/point_set_algo.cpp, Point_set_3/point_set_property.cpp, Point_set_3/point_set_read_ply.cpp, and Point_set_3/point_set_read_xyz.cpp.
//
// Public Types
// typedef Point 	Point_3
//  	The point type.
//  
// typedef Vector 	Vector_3
//  	The vector type.
//  
// typedef unspecified_type 	iterator
//  	Iterator type of the point set with value type Index is model of RandomAccessIterator
//  
// typedef unspecified_type 	const_iterator
//  	Constant iterator type of the point set with value type Index is model of RandomA.ccessIterator
//  
// typedef Property_map< Point > 	Point_map
//  	Property map of points.
//  
// typedef Property_map< Vector > 	Vector_map
//  	Property map of vectors.
//  
// typedef Property_range< Point > 	Point_range
//  	Constant range of points.
//  
// typedef Property_range< Vector > 	Vector_range
//  	Constant range of vectors.
//  
// Related Functions
//
// (Note that these are not member functions.)
// template<typename Point , typename Vector >
// Point_set_3< Point, Vector > & 	operator+= (Point_set_3< Point, Vector > &ps, Point_set_3< Point, Vector > &other)
//  	Append other at the end of ps.
//  
// template<typename Point , typename Vector >
// std::istream & 	operator>> (std::istream &is, CGAL::Point_set_3< Point, Vector > &ps)
//  	reads the point set from an input stream.
//  
// template<typename Point , typename Vector >
// std::ostream & 	operator<< (std::ostream &os, const CGAL::Point_set_3< Point, Vector > &ps)
//  	writes the point set in an output stream in the Polygon File Format (PLY).
//  
// Property Handling
//
// A property Property_map<Type> allows to associate properties of type Type to a point.
//
// Properties can be added, looked up with a string and removed at runtime.
// template<class Type >
// using 	Property_map = unspecified_type
//  	Model of LvaluePropertyMap with Index as a key type and Type as value type.
//  
// template<typename T >
// bool 	has_property_map (const std::string &name) const
//  	tests whether property name of type T already exists.
//  
// template<class T >
// std::pair< Property_map< T >, bool > 	add_property_map (const std::string &name, const T t=T())
//  	adds a new property name of type T with given default value.
//  
// template<class T >
// std::optional< Property_map< T > > 	property_map (const std::string &name) const
//  	returns the property name of type T.
//  
// template<class T >
// bool 	remove_property_map (Property_map< T > &prop)
//  	removes the specified property.
//  
// bool 	has_normal_map () const
//  	Convenience method that tests whether the point set has normals.
//  
// std::pair< Vector_map, bool > 	add_normal_map (const Vector &default_value=CGAL::NULL_VECTOR)
//  	Convenience method that adds a normal property.
//  
// Vector_map 	normal_map ()
//  	returns the property map of the normal property.
//  
// const Vector_map 	normal_map () const
//  	returns the property map of the normal property (constant version).
//  
// bool 	remove_normal_map ()
//  	Convenience method that removes the normal property.
//  
// Point_map 	point_map ()
//  	returns the property map of the point property.
//  
// const Point_map 	point_map () const
//  	returns the property map of the point property (constant version).
//  
// void 	copy_properties (const Point_set_3 &other)
//  	Copies the properties from another point set.
//  
// std::vector< std::string > 	properties () const
//  	returns a vector with all strings that describe properties.
//  
// std::vector< std::pair< std::string, std::type_index > > 	properties_and_types () const
//  	returns a vector of pairs that describe properties and associated types.
//  
// unspecified_type 	parameters () const
//  	returns a sequence of Named Parameters to be used in Point Set Processing algorithms.
//  
// Ranges
// template<class Type >
// using 	Property_range = unspecified_type
//  	Model of ConstRange that handles constant ranges for property maps with value type Type.
//  
// template<class T >
// Property_range< T > 	range (const Property_map< T > &pmap) const
//  	returns a property as a range.
//  
// Point_range 	points () const
//  	returns a constant range of points.
//  
// Vector_range 	normals () const
//  	returns a constant range of normals.
//  
// Push Property Maps and Inserters (Advanced)
// Advanced
//
// The following method are specifically designed to make CGAL::Point_set_3 usable with CGAL input/output functions.
// template<class Property >
// using 	Property_back_inserter = unspecified_type
//  	This is an advanced type.
//  
// template<class Property >
// using 	Push_property_map = unspecified_type
//  	This is an advanced type.
//  
// typedef Property_back_inserter< Index_map > 	Index_back_inserter
//  	This is an advanced type.
//  
// typedef Property_back_inserter< Point_map > 	Point_back_inserter
//  	This is an advanced type.
//  
// typedef Push_property_map< Point_map > 	Point_push_map
//  	This is an advanced type.
//  
// typedef Push_property_map< Vector_map > 	Vector_push_map
//  	This is an advanced type.
//  
// template<class T >
// Push_property_map< Property_map< T > > 	push_property_map (Property_map< T > &prop)
//  	This is an advanced function.
//  
// Point_push_map 	point_push_map ()
//  	This is an advanced function.
//  
// Vector_push_map 	normal_push_map ()
//  	This is an advanced function.
//  
// Index_back_inserter 	index_back_inserter ()
//  	This is an advanced function.
//  
// Point_back_inserter 	point_back_inserter ()
//  	This is an advanced function.
//  
// Construction, Destruction, Assignment
//  	Point_set_3 (bool with_normal_map=false)
//  	creates an empty point set with no additional property.
//  
// Point_set_3 & 	operator= (const Point_set_3 &ps)
//  	Assignment operator, all properties with their content are copied.
//  
// Memory Management
// bool 	is_empty () const
//  	returns true if the number of elements not marked as removed is 0, false otherwise.
//  
// std::size_t 	number_of_points () const
//  	returns the number of elements (not counting elements marked as removed).
//  
// bool 	join (Point_set_3 &other)
//  	merges other in the point set.
//  
// void 	clear ()
//  	Clears the point set properties and content.
//  
// void 	clear_properties ()
//  	Clears all properties created.
//  
// void 	reserve (std::size_t s)
//  	increases the capacity of internal containers to be able to efficiently accommodate at least s elements
//  
// void 	resize (std::size_t s)
//  	changes size of the point set.
//  
// Adding Points and Normals
// iterator 	insert ()
//  	inserts a new element with default property values.
//  
// iterator 	insert (const Point &p)
//  	inserts new point with default property values.
//  
// iterator 	insert (const Point &p, const Vector &n)
//  	Convenience method to add a point with a normal vector.
//  
// iterator 	insert (const Point_set_3 &other, const Index &idx)
//  	Convenience method to copy a point with all its properties from another point set.
//  
// Accessors and Iterators
// iterator 	begin ()
//  	returns the begin iterator.
//  
// iterator 	end ()
//  	returns the past-the-end iterator.
//  
// const_iterator 	begin () const
//  	returns the begin constant iterator.
//  
// const_iterator 	end () const
//  	returns the past-the-end constant iterator.
//  
// Point & 	point (const Index &index)
//  	returns a reference to the point corresponding to index.
//  
// const Point & 	point (const Index &index) const
//  	returns a constant reference to the point corresponding to index.
//  
// Vector & 	normal (const Index &index)
//  	returns a reference to the normal corresponding to index.
//  
// const Vector & 	normal (const Index &index) const
//  	returns a constant reference to the normal corresponding to index.
//  
// Removal Methods
// void 	remove (iterator first, iterator last)
//  	marks all elements between first and last as removed.
//  
// void 	remove (iterator it)
//  	marks element specified by iterator as removed.
//  
// void 	remove (const Index &index)
//  	marks element specified by Index as removed.
//  
// Garbage Management
// bool 	is_removed (const_iterator it) const
//  	returns true if the element is marked as removed, false otherwise.
//  
// bool 	is_removed (const Index &index) const
//  	returns true if the element is marked as removed, false otherwise.
//  
// const_iterator 	garbage_begin () const
//  	returns the constant iterator to the first element marked as removed (equal to garbage_end() if no elements are marked as removed.
//  
// const_iterator 	garbage_end () const
//  	returns the past-the-end constant iterator of the elements marked as removed.
//  
// std::size_t 	number_of_removed_points () const
//  	Number of removed points.
//  
// std::size_t 	garbage_size () const
//  	Number of removed points.
//  
// bool 	has_garbage () const
//  	returns true if there are elements marked as removed, false otherwise.
//  
// void 	collect_garbage ()
//  	erases from memory the elements marked as removed.
//  
// void 	cancel_removals ()
//  	restores all removed points.
//  
// Member Typedef Documentation
// ◆ Index_back_inserter
// template<typename Point , typename Vector >
// typedef Property_back_inserter<Index_map> CGAL::Point_set_3< Point, Vector >::Index_back_inserter
//
// This is an advanced type.
// Advanced
//
// Back inserter on indices
// ◆ Point_back_inserter
// template<typename Point , typename Vector >
// typedef Property_back_inserter<Point_map> CGAL::Point_set_3< Point, Vector >::Point_back_inserter
//
// This is an advanced type.
// Advanced
//
// Back inserter on points
// ◆ Point_push_map
// template<typename Point , typename Vector >
// typedef Push_property_map<Point_map> CGAL::Point_set_3< Point, Vector >::Point_push_map
//
// This is an advanced type.
// Advanced
//
// Property map for pushing new points
// ◆ Property_back_inserter
// template<typename Point , typename Vector >
// template<class Property >
// using CGAL::Point_set_3< Point, Vector >::Property_back_inserter = unspecified_type
//
// This is an advanced type.
// Advanced
//
// Model of OutputIterator used to insert elements by defining the value of the property Property.
// ◆ Push_property_map
// template<typename Point , typename Vector >
// template<class Property >
// using CGAL::Point_set_3< Point, Vector >::Push_property_map = unspecified_type
//
// This is an advanced type.
// Advanced
//
// Model of WritablePropertyMap based on Property and that is allowed to push new items to the point set if needed.
// ◆ Vector_push_map
// template<typename Point , typename Vector >
// typedef Push_property_map<Vector_map> CGAL::Point_set_3< Point, Vector >::Vector_push_map
//
// This is an advanced type.
// Advanced
//
// Property map for pushing new vectors
// Constructor & Destructor Documentation
// ◆ Point_set_3()
// template<typename Point , typename Vector >
// CGAL::Point_set_3< Point, Vector >::Point_set_3 	( 	bool  	with_normal_map = false	) 	
//
// creates an empty point set with no additional property.
//
// Parameters
//     with_normal_map	true if the normal map should be added. If false (default value), the normal map can still be added later on (see add_normal_map()).
//
// Member Function Documentation
// ◆ add_normal_map()
// template<typename Point , typename Vector >
// std::pair< Vector_map, bool > CGAL::Point_set_3< Point, Vector >::add_normal_map 	( 	const Vector &  	default_value = CGAL::NULL_VECTOR	) 	
//
// Convenience method that adds a normal property.
//
// This method adds a property of type Vector and named normal.
//
// Returns
//     Returns a pair containing the normal map and a Boolean that is true if the property was added and false if it already exists (and was therefore not added but only returned). 
//
// ◆ add_property_map()
// template<typename Point , typename Vector >
// template<class T >
// std::pair< Property_map< T >, bool > CGAL::Point_set_3< Point, Vector >::add_property_map 	( 	const std::string &  	name,
// 		const T  	t = T() 
// 	) 		
//
// adds a new property name of type T with given default value.
//
// Template Parameters
//     T	type of the property.
//
// Parameters
//     name	Name of the property.
//     t	Value taken by the property on already created elements.
//
// Returns
//     Returns a pair containing the property map and a Boolean that is true if the property was added and false if it already exists (and was therefore not added but only returned). 
//
// ◆ cancel_removals()
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::cancel_removals 	( 		) 	
//
// restores all removed points.
//
// After removing one or several points, calling this method restores the point set to its initial state: points that were removed (and their associated properties) are restored.
//
// Note
//     This method is only guaranteed to work if no point was inserted after the removal: otherwise, some points might not be restored.
//     If collect_garbage() was called after removal, the points are irremediably lost and nothing will be restored. 
//
// ◆ clear()
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::clear 	( 		) 	
//
// Clears the point set properties and content.
//
// After calling this method, the object is the same as a newly constructed object. The additional properties (such as normal vectors) are also removed and must thus be re-added if needed.
// ◆ clear_properties()
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::clear_properties 	( 		) 	
//
// Clears all properties created.
//
// After calling this method, all properties are removed. The points are left unchanged.
// ◆ copy_properties()
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::copy_properties 	( 	const Point_set_3< Point, Vector > &  	other	) 	
//
// Copies the properties from another point set.
//
// All properties from other that do not already exist in this point set are added and filled to their default values. Properties that exist in both point sets are left unchanged.
// ◆ end() [1/2]
// template<typename Point , typename Vector >
// iterator CGAL::Point_set_3< Point, Vector >::end 	( 		) 	
//
// returns the past-the-end iterator.
//
// Note
//     The returned value is the same as garbage_begin(). 
//
// ◆ end() [2/2]
// template<typename Point , typename Vector >
// const_iterator CGAL::Point_set_3< Point, Vector >::end 	( 		) 	const
//
// returns the past-the-end constant iterator.
//
// Note
//     The returned value is the same as garbage_begin(). 
//
// ◆ garbage_size()
// template<typename Point , typename Vector >
// std::size_t CGAL::Point_set_3< Point, Vector >::garbage_size 	( 		) 	const
//
// Number of removed points.
//
// See also
//     number_of_removed_points() 
//
// ◆ has_normal_map()
// template<typename Point , typename Vector >
// bool CGAL::Point_set_3< Point, Vector >::has_normal_map 	( 		) 	const
//
// Convenience method that tests whether the point set has normals.
//
// This method tests whether a property of type Vector and named normal exists.
// ◆ has_property_map()
// template<typename Point , typename Vector >
// template<typename T >
// bool CGAL::Point_set_3< Point, Vector >::has_property_map 	( 	const std::string &  	name	) 	const
//
// tests whether property name of type T already exists.
//
// Template Parameters
//     T	type of the property.
//
// Parameters
//     name	Name of the property.
//
// ◆ index_back_inserter()
// template<typename Point , typename Vector >
// Index_back_inserter CGAL::Point_set_3< Point, Vector >::index_back_inserter 	( 		) 	
//
// This is an advanced function.
// Advanced
//
// returns the back inserter on the index property.
// ◆ insert() [1/4]
// template<typename Point , typename Vector >
// iterator CGAL::Point_set_3< Point, Vector >::insert 	( 		) 	
//
// inserts a new element with default property values.
//
// Returns
//     The iterator on the newly added element.
//
// Note
//     If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call. 
//
// ◆ insert() [2/4]
// template<typename Point , typename Vector >
// iterator CGAL::Point_set_3< Point, Vector >::insert 	( 	const Point &  	p	) 	
//
// inserts new point with default property values.
//
// Parameters
//     p	Point to insert
//
// Note
//     Properties of the added point are initialized to their default value.
//     If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.
//
// Returns
//     The iterator on the newly added element. 
//
// ◆ insert() [3/4]
// template<typename Point , typename Vector >
// iterator CGAL::Point_set_3< Point, Vector >::insert 	( 	const Point &  	p,
// 		const Vector &  	n 
// 	) 		
//
// Convenience method to add a point with a normal vector.
//
// Parameters
//     p	Point to insert
//     n	Associated normal vector
//
// Note
//     Properties of the added point other than its normal vector are initialized to their default value.
//     If not already added, a normal property is automatically added to the point set when using this method. The default value for normal vectors is CGAL::NULL_VECTOR.
//     If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.
//
// Returns
//     The iterator on the newly added element. 
//
// ◆ insert() [4/4]
// template<typename Point , typename Vector >
// iterator CGAL::Point_set_3< Point, Vector >::insert 	( 	const Point_set_3< Point, Vector > &  	other,
// 		const Index &  	idx 
// 	) 		
//
// Convenience method to copy a point with all its properties from another point set.
//
// In the case where two point sets have the same properties, this method allows the user to easily copy one point (along with the values of all its properties) from one point set to another.
//
// Parameters
//     other	Point set to which the point to copy belongs
//     idx	Index of the point to copy in other
//
// Warning
//     This point set and other must have the exact same properties, with the exact same names and types in the exact same order.
//
// Note
//     If a reallocation happens, all iterators, pointers and references related to the container are invalidated. Otherwise, only the end iterator is invalidated, and all iterators, pointers and references to elements are guaranteed to keep referring to the same elements they were referring to before the call.
//
// Returns
//     The iterator on the newly added element. 
//
// ◆ is_empty()
// template<typename Point , typename Vector >
// bool CGAL::Point_set_3< Point, Vector >::is_empty 	( 		) 	const
//
// returns true if the number of elements not marked as removed is 0, false otherwise.
//
// Note
//     This does not count the removed elements.
//     The method empty() is also available (see Range) and does the same thing. 
//
// ◆ is_removed() [1/2]
// template<typename Point , typename Vector >
// bool CGAL::Point_set_3< Point, Vector >::is_removed 	( 	const Index &  	index	) 	const
//
// returns true if the element is marked as removed, false otherwise.
//
// Note
//     When iterating between begin() and end(), no element marked as removed can be found. 
//
// ◆ is_removed() [2/2]
// template<typename Point , typename Vector >
// bool CGAL::Point_set_3< Point, Vector >::is_removed 	( 	const_iterator  	it	) 	const
//
// returns true if the element is marked as removed, false otherwise.
//
// Note
//     When iterating between begin() and end(), no element marked as removed can be found. 
//
// ◆ join()
// template<typename Point , typename Vector >
// bool CGAL::Point_set_3< Point, Vector >::join 	( 	Point_set_3< Point, Vector > &  	other	) 	
//
// merges other in the point set.
//
// Shifts the indices of points of other by number_of_points() + other.number_of_points().
//
// Copies entries of all property maps which have the same name in the point set and other. Property maps which are only in other are ignored.
//
// Note
//     If copy_properties() with other as argument is called before calling this method, then all the content of other will be copied and no property will be lost in the process.
//     Garbage is collected in both point sets when calling this method. 
//
// ◆ normal() [1/2]
// template<typename Point , typename Vector >
// Vector & CGAL::Point_set_3< Point, Vector >::normal 	( 	const Index &  	index	) 	
//
// returns a reference to the normal corresponding to index.
//
// Note
//     If not already added, a normal property is automatically added to the point set (see add_normal_map()). 
//
// ◆ normal() [2/2]
// template<typename Point , typename Vector >
// const Vector & CGAL::Point_set_3< Point, Vector >::normal 	( 	const Index &  	index	) 	const
//
// returns a constant reference to the normal corresponding to index.
//
// Note
//     If not already added, a normal property is automatically added to the point set (see add_normal_map()). 
//
// ◆ normal_map() [1/2]
// template<typename Point , typename Vector >
// Vector_map CGAL::Point_set_3< Point, Vector >::normal_map 	( 		) 	
//
// returns the property map of the normal property.
//
// Note
//     If the normal property has not been added yet to the point set before calling this method, the property map is automatically added with add_normal_map(). 
//
// ◆ normal_map() [2/2]
// template<typename Point , typename Vector >
// const Vector_map CGAL::Point_set_3< Point, Vector >::normal_map 	( 		) 	const
//
// returns the property map of the normal property (constant version).
//
// Note
//     The normal property must have been added to the point set before calling this method (see add_normal_map()). 
//
// ◆ normal_push_map()
// template<typename Point , typename Vector >
// Vector_push_map CGAL::Point_set_3< Point, Vector >::normal_push_map 	( 		) 	
//
// This is an advanced function.
// Advanced
//
// returns the push property map of the normal property.
//
// Note
//     The normal property must have been added to the point set before calling this method (see add_normal_map()).
//
// ◆ number_of_points()
// template<typename Point , typename Vector >
// std::size_t CGAL::Point_set_3< Point, Vector >::number_of_points 	( 		) 	const
//
// returns the number of elements (not counting elements marked as removed).
//
// Note
//     See number_of_removed_points() for getting the number of elements marked as removed.
//     The method size() is also available (see Range) and does the same thing. 
//
// ◆ number_of_removed_points()
// template<typename Point , typename Vector >
// std::size_t CGAL::Point_set_3< Point, Vector >::number_of_removed_points 	( 		) 	const
//
// Number of removed points.
//
// See also
//     garbage_size() 
//
// ◆ parameters()
// template<typename Point , typename Vector >
// unspecified_type CGAL::Point_set_3< Point, Vector >::parameters 	( 		) 	const
//
// returns a sequence of Named Parameters to be used in Point Set Processing algorithms.
//
// The following named parameters are used:
//
//     point_map: contains the point property map (see point_map())
//     normal_map: contains the normal map (see normal_map())
//     geom_traits: contains the kernel typename Kernel_traits<Point>::Kernel
//
// Warning
//     this method does not check if the normal map was instantiated or not. The normal map named parameter should not be used if this property was not instantiated first. 
//
// ◆ point_back_inserter()
// template<typename Point , typename Vector >
// Point_back_inserter CGAL::Point_set_3< Point, Vector >::point_back_inserter 	( 		) 	
//
// This is an advanced function.
// Advanced
//
// returns the back inserter on the point property.
// ◆ point_push_map()
// template<typename Point , typename Vector >
// Point_push_map CGAL::Point_set_3< Point, Vector >::point_push_map 	( 		) 	
//
// This is an advanced function.
// Advanced
//
// returns the push property map of the point property.
// ◆ property_map()
// template<typename Point , typename Vector >
// template<class T >
// std::optional< Property_map< T > > CGAL::Point_set_3< Point, Vector >::property_map 	( 	const std::string &  	name	) 	const
//
// returns the property name of type T.
//
// Template Parameters
//     T	type of the property.
//
// Parameters
//     name	Name of the property.
//
// Returns
//     Returns an optional property map (empty if the property was not found). 
//
// Examples
//     Point_set_3/point_set_property.cpp.
//
// ◆ push_property_map()
// template<typename Point , typename Vector >
// template<class T >
// Push_property_map< Property_map< T > > CGAL::Point_set_3< Point, Vector >::push_property_map 	( 	Property_map< T > &  	prop	) 	
//
// This is an advanced function.
// Advanced
//
// returns the push property map of the given property.
//
// Template Parameters
//     T	type of the property.
//
// Parameters
//     prop	The property map.
//
// Returns
//     Returns a pair containing: the specified property map and a Boolean set to true or an empty property map and a Boolean set to false (if the property was not found).
//
// ◆ remove() [1/3]
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::remove 	( 	const Index &  	index	) 	
//
// marks element specified by Index as removed.
//
// Note
//     The element is just marked as removed and is not erased from the memory. collect_garbage() should be called if the memory needs to be freed. The element can be recovered with cancel_removals().
//     The end() iterator is invalidated, it dereferences to a different element. 
//
// ◆ remove() [2/3]
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::remove 	( 	iterator  	first,
// 		iterator  	last 
// 	) 		
//
// marks all elements between first and last as removed.
//
// Note
//     The elements are just marked as removed and are not erased from the memory. collect_garbage() should be called if the memory needs to be disallocated. Elements can be recovered with cancel_removals().
//     All iterators, pointers and references related to the container are invalidated. 
//
// ◆ remove() [3/3]
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::remove 	( 	iterator  	it	) 	
//
// marks element specified by iterator as removed.
//
// Note
//     The element is just marked as removed and is not erased from the memory. collect_garbage() should be called if the memory needs to be freed. The element can be recovered with cancel_removals().
//     The end() iterator is invalidated, it dereferences to a different element. 
//
// ◆ remove_normal_map()
// template<typename Point , typename Vector >
// bool CGAL::Point_set_3< Point, Vector >::remove_normal_map 	( 		) 	
//
// Convenience method that removes the normal property.
//
// Returns
//     Returns true if the property was removed and false if the property was not found. 
//
// ◆ remove_property_map()
// template<typename Point , typename Vector >
// template<class T >
// bool CGAL::Point_set_3< Point, Vector >::remove_property_map 	( 	Property_map< T > &  	prop	) 	
//
// removes the specified property.
//
// Template Parameters
//     T	type of the property.
//
// Parameters
//     prop	The property.
//
// Returns
//     Returns true if the property was removed and false if the property was not found. 
//
// ◆ reserve()
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::reserve 	( 	std::size_t  	s	) 	
//
// increases the capacity of internal containers to be able to efficiently accommodate at least s elements
//
// Parameters
//     s	Expected final number of elements.
//
// Note
//     This method does not change the content of the point set and is only used for optimization. 
//
// ◆ resize()
// template<typename Point , typename Vector >
// void CGAL::Point_set_3< Point, Vector >::resize 	( 	std::size_t  	s	) 	
//
// changes size of the point set.
//
// Parameters
//     s	Target size of the point set
//
// Note
//     If the given size is larger than the current size, the capacity of the internal container is extended. If there are element marked as removed, they may be overwritten. If the given size is smaller than the current size, garbage is collected and the container is resized. 
//
// Friends And Related Function Documentation
// ◆ operator+=()
// template<typename Point , typename Vector >
// Point_set_3< Point, Vector > & operator+= 	( 	Point_set_3< Point, Vector > &  	ps,
// 		Point_set_3< Point, Vector > &  	other 
// 	) 		
// 	related
//
// Append other at the end of ps.
//
// Shifts the indices of points of other by ps.number_of_points() + other.number_of_points().
//
// Copies entries of all property maps which have the same name in ps and other. Property maps which are only in other are ignored.
//
// Note
//     Garbage is collected in both point sets when calling this method. 
//
