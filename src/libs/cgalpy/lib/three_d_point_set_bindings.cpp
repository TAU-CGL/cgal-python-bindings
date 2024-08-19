#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/string.h>

#include <CGAL/Point_set_3.h>
#include <CGAL/Origin.h>

#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

void export_3d_point_set(py::module_& m) {
  using Kernel_ = Kernel;
  using Pt_set = CGAL::Point_set_3<Kernel_::Point_3, Kernel_::Vector_3>;
  using Pnt_3 = Kernel_::Point_3;
  using Vec_3 = Kernel_::Vector_3;

  py::class_<Pt_set> ptst(m, "Point_set_3");

    // Construction, Destruction, Assignment
  ptst.def(py::init<bool>(),
           py::arg("with_normal_map")=false,
      "creates an empty point set with no additional property."
      "\n"
      "Parameters\n"
      "with_normal_map  true if the normal map should be added. If false (default value), the normal map can still be added later on (see add_normal_map()).")

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
    //   "Returns a pair containing the normal map and a Boolean that is true if the property was added and false if it already exists (and was therefore not added but only returned). ")
    // TODO: add_property_map
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
    .def("has_normal_map", &Pt_set::has_normal_map,
      "Convenience method that tests whether the point set has normals.\n"
      "This method tests whether a property of type Vector and named normal exists.")
    // TODO: has_property_map
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
    .def("is_empty", &Pt_set::is_empty,
      "returns true if the number of elements not marked as removed is 0, false otherwise.\n"
      "\n"
      "Note\n"
      "This does not count the removed elements.\n"
      "The method empty() is also available (see Range) and does the same thing. ")
    .def("is_removed", [](Pt_set& ps, const std::size_t idx) { return ps.is_removed(idx); },
      py::arg("index"),
      "returns true if the element is marked as removed, false otherwise.\n"
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
    .def("normal", [](Pt_set& ps, const std::size_t idx) { return ps.normal(idx); },
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
    // TODO: property map
    .def("remove", [](Pt_set& ps, const std::size_t idx) { return ps.remove(idx); },
      py::arg("index"),
      "marks the element as removed.\n"
      "\n"
      "Note\n"
      "The element is not actually removed from the point set, but only marked as removed. The element can be restored with cancel_removals(). ")
    .def("remove_normal_map", &Pt_set::remove_normal_map,
      "removes the normal property.\n"
      "Returns\n"
      "Returns true if the property was removed and false if it was not found.")
    // TODO: remove property map
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
    // TODO: types for these
    .def("points", &Pt_set::points,
      "returns a constant range of points.")
    .def("normals", &Pt_set::normals,
      "returns a constant range of normals.")



    ;

  add_insertion(ptst, "__str__");
  add_insertion(ptst, "__repr__");
  add_extraction(ptst);

}

