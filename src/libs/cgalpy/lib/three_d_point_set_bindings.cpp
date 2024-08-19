#include <nanobind/nanobind.h>

#include <CGAL/Point_set_3.h>

#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

void export_3d_point_set(py::module_& m) {
  using Kernel_ = Kernel;
  using Pt_set = CGAL::Point_set_3<Kernel_::Point_3, Kernel_::Vector_3>;

  py::class_<Pt_set>(m, "Point_set_3")

    // Construction, Destruction, Assignment
    .def(py::init<bool>(), py::arg("with_normal_map")=false,
      "creates an empty point set with no additional property.")

    // Related Functions


    ;


}


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
