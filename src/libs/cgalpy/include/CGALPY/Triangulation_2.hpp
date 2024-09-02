#ifndef CGALPY_TRIANGULATION_2_HPP
#define CGALPY_TRIANGULATION_2_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Triangulation_2.h>

#include "CGALPY/Triangulation_cw_ccw_2.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

namespace tri2 {

template <typename TriangulationType, typename C>
auto export_triangulation_2(C& c) {

  using Vertex_handle = typename TriangulationType::Vertex_handle;
  using Face_handle = typename TriangulationType::Face_handle;
  using All_faces_iterator = typename TriangulationType::All_faces_iterator;
  using All_edges_iterator = typename TriangulationType::All_edges_iterator;
  using All_vertices_iterator = typename TriangulationType::All_vertices_iterator;
  using Geom_traits = typename TriangulationType::Geom_traits;
  using Face_circulator = typename TriangulationType::Face_circulator;
  using Edge_circulator = typename TriangulationType::Edge_circulator;
  using Vertex_circulator = typename TriangulationType::Vertex_circulator;
  using Tds = typename TriangulationType::Triangulation_data_structure;
  using Point = typename Geom_traits::Point_2;
  using Segment = typename Geom_traits::Segment_2;
  using Triangle = typename Geom_traits::Triangle_2;
  using Vertex = typename Tds::Vertex;
  using Face = typename Tds::Face;
  using Edge = typename Tds::Edge;
  using Locate_type = typename TriangulationType::Locate_type;

  // Related Functions
  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
  return c
    .def(py::init<const TriangulationType&>(),
         py::arg("tr"),
         "Copy constructor.")
    .def("swap", &TriangulationType::swap,
         py::arg("tr"),
         "The triangulations tr and *this are swapped.\n"
         "This method should be used instead of assignment of copy constructor. if tr is deleted after that.")
    .def("clear", &TriangulationType::clear,
         "Deletes all faces and finite vertices resulting in an empty triangulation.")
    .def("dimension", &TriangulationType::dimension,
         "Returns the dimension of the triangulation.")
    .def("number_of_vertices", &TriangulationType::number_of_vertices,
         "Returns the number of vertices in the triangulation.")
    .def("number_of_faces", &TriangulationType::number_of_faces,
         "Returns the number of faces in the triangulation.")
    .def("infinite_face", &TriangulationType::infinite_face,
         "Returns the infinite face.")
    .def("infinite_vertex", &TriangulationType::infinite_vertex,
         "Returns the infinite vertex.")
    .def("finite_vertex", &TriangulationType::finite_vertex,
         "Returns the finite vertex.")
    .def("geom_traits", &TriangulationType::geom_traits,
         "Returns a const reference to the triangulation traits object.")
    .def("tds", [](const TriangulationType& tr) { return tr.tds(); },
         "Returns a const reference to the triangulation data structure.")
    .def("non_const_tds", [](TriangulationType& tr) { return tr.tds(); },
         "Returns a reference to the triangulation data structure.\n\n"
         "Attention\n"
         "• The responsibility of keeping a valid triangulation belongs to the user when using advanced operations allowing a direct manipulation of the tds. This method is mainly a help for users implementing their own triangulation algorithms.")

    // Predicates
    .def("is_infinite", [](const TriangulationType& tr, const Vertex_handle& v) { return tr.is_infinite(v); },
         py::arg("v"),
         "true iff v is the infinite vertex.")
    .def("is_infinite", [](const TriangulationType& tr, const Face_handle& f) { return tr.is_infinite(f); },
         py::arg("f"),
         "true iff face f is infinite.")
    .def("is_infinite", [](const TriangulationType& tr, const Face_handle& f, const int i) { return tr.is_infinite(f, i); },
         py::arg("f"), py::arg("i"),
         "true iff edge (f,i) is infinite.")
    .def("is_infinite", [](const TriangulationType& tr, const Edge& e) { return tr.is_infinite(e); },
         py::arg("e"),
         "true iff edge e is infinite.")
    .def("is_infinite", [](const TriangulationType& tr, const Edge_circulator& ec) { return tr.is_infinite(ec); },
         py::arg("ec"),
         "true iff edge *ec is infinite.")
    .def("is_infinite", [](const TriangulationType& tr, const All_edges_iterator& ei) { return tr.is_infinite(ei); },
         py::arg("ei"),
         "true iff edge *ei is infinite.")
    .def("is_edge", [](const TriangulationType& tr, const Vertex_handle& va, const Vertex_handle& vb) { return tr.is_edge(va, vb); },
         py::arg("va"), py::arg("vb"),
         "true if there is an edge having va and vb as vertices.")
    .def("is_edge", [](const TriangulationType& tr, const Vertex_handle& va, const Vertex_handle& vb, Face_handle& fr, int i) { return tr.is_edge(va, vb, fr, i); },
         py::arg("va"), py::arg("vb"), py::arg("fr"), py::arg("i"),
         "true if there is an edge having va and vb as vertices.\n"
         "In addition, if true is returned, the edge with vertices va and vb is the edge e=(fr,i) where fr is a handle to the face incident to e and on the right side of e oriented from va to vb.")
    .def("includes_edge", &TriangulationType::includes_edge,
         py::arg("va"), py::arg("vb"), py::arg("vbb"), py::arg("fr"), py::arg("i"),
         "true if the line segment from va to vb includes an edge e incident to va.\n"
         "If true, vbb becomes the other vertex of e, e is the edge (fr,i) where fr is a handle to the face incident to e and on the right side e oriented from va to vb.")
    .def("is_face", [](const TriangulationType& tr, const Vertex_handle& v1, const Vertex_handle& v2, const Vertex_handle& v3) { return tr.is_face(v1, v2, v3); },
         py::arg("v1"), py::arg("v2"), py::arg("v3"),
         "true if there is a face having v1, v2 and v3 as vertices.")
    .def("is_face", [](const TriangulationType& tr, const Vertex_handle& v1, const Vertex_handle& v2, const Vertex_handle& v3, Face_handle& fr) { return tr.is_face(v1, v2, v3, fr); },
         py::arg("v1"), py::arg("v2"), py::arg("v3"), py::arg("fr"),
         "true if there is a face having v1, v2 and v3 as vertices.\n"
         "In addition, if true is returned, fr is a handle to the face with v1, v2 and v3 as vertices.")

    // Queries
    .def("locate", [](const TriangulationType& tr, const Point& query, Face_handle f) { return tr.locate(query, f); },
         py::arg("query"), py::arg("f"),
         "if the point query lies inside the convex hull of the points, a face that contains the query in its interior or on its boundary is returned.\n"
         "if the point query lies outside the convex hull of the triangulation but in the affine hull, the returned face is an infinite face which is a proof of the point's location:\n"
         "• for a two dimensional triangulation, it is a face (∞,p,q) such that query lies to the left of the oriented line pq (the rest of the triangulation lying to the right of this line).\n"
         "• for a degenerate one dimensional triangulation it is the (degenerate one dimensional) face (∞,p,nullptr) such that query and the triangulation lie on either side of p.\n"
         "if the point query lies outside the affine hull, the returned face_handle is nullptr.\n"
         "the optional face_handle argument, if provided, is used as a hint of where the locate process has to start its search.")
    .def("inexact_locate", [](const TriangulationType& tr, const Point& query, Face_handle start) { return tr.inexact_locate(query, start); },
         py::arg("query"), py::arg("start"),
         "Same as locate() but uses inexact predicates.\n"
         "This function returns a handle on a face that is a good approximation of the exact location of query, while being faster. Note that it may return a handle on a face whose interior does not contain query. When the triangulation has dimension smaller than 2, start is returned.")
    .def("locate", [](const TriangulationType& tr, const Point& query, Locate_type& lt, int& li, Face_handle f) { return tr.locate(query, lt, li, f); },
         py::arg("query"), py::arg("lt"), py::arg("li"), py::arg("f"),
         "Additionally, the parameters lt and li describe where the query point is located. The variable lt is set to the locate type of the query. If lt==VERTEX the variable li is set to the index of the vertex, and if lt==EDGE li is set to the index of the vertex opposite to the edge. Be careful that li has no meaning when the query type is FACE, OUTSIDE_CONVEX_HULL, or OUTSIDE_AFFINE_HULL or when the triangulation is 0-dimensional.\n"
         "The optional Locate_type argument, if provided, is used as a hint of where the locate process has to start its search.\n"
         "Additionally, the parameters lt and li describe where the query point is located. The variable lt is set to the locate type of the query. If lt==VERTEX the variable li is set to the index of the vertex, and if lt==EDGE li is set to the index of the vertex opposite to the edge. Be careful that li has no meaning when the query type is FACE, OUTSIDE_CONVEX_HULL, or OUTSIDE_AFFINE_HULL or when the triangulation is 0-dimensional.")

    
    ;

  // inherits from Triangulation_cw_ccw_2
  tri2::export_triangulation_cw_ccw_2<TriangulationType>(c);
}

} // namespace tri2

#endif // CGALPY_TRIANGULATION_2_HPP
