#ifndef CGALPY_TRIANGULATION_2_HPP
#define CGALPY_TRIANGULATION_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

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
  using Edge_iterator = typename Tds::Edge_iterator;
  using Locate_type = typename TriangulationType::Locate_type;

  // Related Functions
  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
  c
    .def(py::init<const TriangulationType&>(),
         py::arg("tr"),
         "Copy constructor.")
    .def("all_face_handles", [](const TriangulationType& tr) { return tr.all_face_handles(); },
         "returns a range of iterators over all faces.")
    .def("all_vertex_handles", [](const TriangulationType& tr) { return tr.all_vertex_handles(); },
         "returns a range of iterators over all vertices.")
    // ccw in base class
    .def("circumcenter", [](const TriangulationType& tr, const Face_handle& f) { return tr.circumcenter(f); },
         py::arg("f"),
         "Compute the circumcenter of the face pointed to by f.\n"
         "This function is available only if the corresponding function is provided in the geometric traits.")
    // cw in base class
    .def("finite_face_handles", [](const TriangulationType& tr) { return tr.finite_face_handles(); },
         "returns a range of iterators over finite faces.\n\n"
         "Note\n"
         "• While the value type of Finite_faces_iterator is Face, the value type of Finite_face_handles::iterator is Face_handle.")
    .def("finite_vertex_handles", [](const TriangulationType& tr) { return tr.finite_vertex_handles(); },
         "returns a range of iterators over finite vertices.\n\n"
         "Note\n"
         "• While the value type of Finite_vertices_iterator is Vertex, the value type of Finite_vertex_handles::iterator is Vertex_handle.")
    .def("flip", [](TriangulationType& tr, const Face_handle& f, int i) { return tr.flip(f, i); },
         py::arg("f"), py::arg("i"),
         "Exchanges the edge incident to f and f->neighbor(i) with the other diagonal of the quadrilateral formed by f and f->neighbor(i).\n"
         "Precondition\n"
         "• The faces f and f->neighbor(i) are finite faces and their union form a convex quadrilateral.")
    .def("incident_edges", [](const TriangulationType& tr, const Vertex_handle& v, const Face_handle& f) { return tr.incident_edges(v, f); },
         py::arg("v"), py::arg("f"),
         "Starts at the first edge of f incident to v, in counterclockwise order around v.\n\n"
         "Precondition\n"
         "• Face f is incident to vertex v.")
    .def("incident_faces", [](const TriangulationType& tr, const Vertex_handle& v, const Face_handle& f) { return tr.incident_faces(v, f); },
         py::arg("v"), py::arg("f"),
         "Starts at face f.\n\n"
         "Precondition\n"
         "• Face f is incident to vertex v.")
    .def("incident_vertices", [](const TriangulationType& tr, const Vertex_handle& v, const Face_handle& f) { return tr.incident_vertices(v, f); },
         py::arg("v"), py::arg("f"),
         "Starts at the first vertex of f adjacent to v in counterclockwise order around v.\n\n"
         "Precondition\n"
         "• Face f is incident to vertex v.")
    .def("includes_edge", [](const TriangulationType& tr, const Vertex_handle& va, const Vertex_handle& vb, Vertex_handle& vbb, Face_handle& fr, int i) { return tr.includes_edge(va, vb, vbb, fr, i); },
         py::arg("va"), py::arg("vb"), py::arg("vbb"), py::arg("fr"), py::arg("i"),
         "true if the line segment from va to vb includes an edge e incident to va.\n"
         "If true, vbb becomes the other vertex of e, e is the edge (fr,i) where fr is a handle to the face incident to e and on the right side e oriented from va to vb.")
    .def("inexact_locate", [](const TriangulationType& tr, const Point& query, Face_handle start) { return tr.inexact_locate(query, start); },
         py::arg("query"), py::arg("start"),
         "Same as locate() but uses inexact predicates.\n"
         "This function returns a handle on a face that is a good approximation of the exact location of query, while being faster. Note that it may return a handle on a face whose interior does not contain query. When the triangulation has dimension smaller than 2, start is returned.")
    .def("insert", [](TriangulationType& tr, const Point& p, const Face_handle& f) { return tr.insert(p, f); },
         py::arg("p"), py::arg("f"),
         "Inserts point p in the triangulation and returns the corresponding vertex.\n"
         "If point p coincides with an already existing vertex, this vertex is returned and the triangulation remains unchanged.\n"
         "If point p is on an edge, the two incident faces are split in two.\n"
         "If point p is strictly inside a face of the triangulation, the face is split in three.\n"
         "If point p is strictly outside the convex hull, p is linked to all visible points on the convex hull to form the new triangulation.\n"
         "At last, if p is outside the affine hull (in case of degenerate 1-dimensional or 0-dimensional triangulations), p is linked all the other vertices to form a triangulation whose dimension is increased by one. The last argument f is an indication to the underlying locate algorithm of where to start.\n\n"
         "Examples\n"
         "• Triangulation_2/adding_handles.py, Triangulation_2/colored_face.py, and Triangulation_2/for_loop_2.py.")
    .def("insert", [](TriangulationType& tr, const std::vector<Point>& points) { return tr.insert(points.begin(), points.end()); },
         py::arg("points"),
         "Inserts the points in the given order, and returns the number of inserted points.\n")
    .def("insert_in_edge", [](TriangulationType& tr, const Point& p, const Face_handle& f, int i) { return tr.insert_in_edge(p, f, i); },
         py::arg("p"), py::arg("f"), py::arg("i"),
         "Inserts vertex v in edge i of f.\n\n"
         "Precondition\n"
         "• The point in vertex v lies on the edge opposite to the vertex i of face f.")
    .def("insert_in_face", [](TriangulationType& tr, const Point& p, const Face_handle& f) { return tr.insert_in_face(p, f); },
         py::arg("p"), py::arg("f"),
         "Inserts vertex v in face f.\n"
         "Face f is modified, two new faces are created.\n\n"
         "Precondition\n"
         "• The point in vertex v lies inside face f.")
    .def("insert_outside_convex_hull", [](TriangulationType& tr, const Point& p, const Face_handle& f) { return tr.insert_outside_convex_hull(p, f); },
         py::arg("p"), py::arg("f"),
         "Inserts a point which is outside the convex hull but in the affine hull.\n\n"
         "Precondition\n"
         "• The handle f points to a face which is a proof of the location ofp, see the description of the locate method above.")
    .def("is_valid", [](const TriangulationType& tr, bool verbose, int level) { return tr.is_valid(verbose, level); },
         py::arg("verbose") = false, py::arg("level") = 0,
         "Checks the combinatorial validity of the triangulation and also the validity of its geometric embedding.\n\n"
         "This method is mainly a debugging help for the users of advanced features.")
    .def("line_walk", [](const TriangulationType& tr, const Point& p, const Point& q, Face_handle f) { return tr.line_walk(p, q, f); },
         py::arg("p"), py::arg("q"), py::arg("f"),
         "This function returns a circulator that allows to visit the faces intersected by the line pq.\n"
         "If there is no such face the circulator has a singular value.\n"
         "The starting point of the circulator is the face f, or the first finite face traversed by l , if f is omitted.\n"
         "The circulator wraps around the infinite vertex: after the last traversed finite face, it steps through the infinite face adjacent to this face then through the infinite face adjacent to the first traversed finite face then through the first finite traversed face again.\n\n"
         "Precondition\n"
         "• The triangulation must have dimension 2.\n"
         "• Points p and q must be different points.\n"
         "• If f != nullptr, it must point to a finite face and the point p must be inside or on the boundary of f.")
    .def("locate", [](const TriangulationType& tr, const Point& query, Face_handle f) { return tr.locate(query, f); },
         py::arg("query"), py::arg("f"),
         "If the point query lies inside the convex hull of the points, a face that contains the query in its interior or on its boundary is returned.\n"
         "If the point query lies outside the convex hull of the triangulation but in the affine hull, the returned face is an infinite face which is a proof of the point's location:\n"
         "• for a two dimensional triangulation, it is a face (∞,p,q) such that query lies to the left of the oriented line pq (the rest of the triangulation lying to the right of this line).\n"
         "• for a degenerate one dimensional triangulation it is the (degenerate one dimensional) face (∞,p,nullptr) such that query and the triangulation lie on either side of p.\n"
         "If the point query lies outside the affine hull, the returned Face_handle is nullptr.\n"
         "The optional Face_handle argument, if provided, is used as a hint of where the locate process has to start its search.\n")
    .def("locate", [](const TriangulationType& tr, const Point& query, Locate_type& lt, int& li, Face_handle f) { return tr.locate(query, lt, li, f); },
         py::arg("query"), py::arg("lt"), py::arg("li"), py::arg("f"),
         "If the point query lies inside the convex hull of the points, a face that contains the query in its interior or on its boundary is returned.\n"
         "If the point query lies outside the convex hull of the triangulation but in the affine hull, the returned face is an infinite face which is a proof of the point's location:\n"
         "• for a two dimensional triangulation, it is a face (∞,p,q) such that query lies to the left of the oriented line pq (the rest of the triangulation lying to the right of this line).\n"
         "• for a degenerate one dimensional triangulation it is the (degenerate one dimensional) face (∞,p,nullptr) such that query and the triangulation lie on either side of p.\n"
         "If the point query lies outside the affine hull, the returned Face_handle is nullptr.\n"
         "The optional Face_handle argument, if provided, is used as a hint of where the locate process has to start its search.\n"
         "Additionally, the parameters lt and li describe where the query point is located. The variable lt is set to the locate type of the query. If lt==VERTEX the variable li is set to the index of the vertex, and if lt==EDGE li is set to the index of the vertex opposite to the edge. Be careful that li has no meaning when the query type is FACE, OUTSIDE_CONVEX_HULL, or OUTSIDE_AFFINE_HULL or when the triangulation is 0-dimensional.\n\n"
         "Precondition\n"
         "• The triangulation must have dimension 2.\n"
         "• Points p and q must be different points.")
    .def("mirror_edge", [](TriangulationType& tr, const Edge& e) { return tr.mirror_edge(e); },
         py::arg("e"),
         "returns the same edge seen from the other adjacent face.\n\n"
         "Precondition\n"
         "• 0≤i≤2.")
    .def("mirror_index", [](const TriangulationType& tr, const Face_handle& f, int i) { return tr.mirror_index(f, i); },
         py::arg("f"), py::arg("i"),
         "returns the index of f in its ith neighbor.\n\n"
         "Precondition\n"
         "• 0≤i≤2.")
    .def("mirror_vertex", [](TriangulationType& tr, const Face_handle& f, int i) { return tr.mirror_vertex(f, i); },
         py::arg("f"), py::arg("i"),
         "returns the vertex of the ith neighbor of f that is opposite to f.\n\n"
         "Precondition\n"
         "• 0≤i≤2.")
    .def("move", [](TriangulationType& tr, const Vertex_handle& v, const Point& p) { return tr.move(v, p); },
         py::arg("v"), py::arg("p"),
         "If there is no collision during the move, this function is the same as move_if_no_collision .\n"
         "Otherwise, v is removed and the vertex at point p is returned.\n\n"
         "Precondition\n"
         "• Vertex v must be finite.")
    .def("move_if_no_collision", [](TriangulationType& tr, const Vertex_handle& v, const Point& p) { return tr.move_if_no_collision(v, p); },
         py::arg("v"), py::arg("p"),
         "If there is not already another vertex placed on p, the triangulation is modified such that the new position of vertex v is p, and v is returned.\n\n"
         "Otherwise, the triangulation is not modified and the vertex at point p is returned.\n\n"
         "Precondition\n"
         "• Vertex v must be finite.")
    // operator= skipped
    .def("oriented_side", [](const TriangulationType& tr, const Face_handle& f, const Point& p) { return tr.oriented_side(f, p); },
         py::arg("f"), py::arg("p"),
         "Returns on which side of the oriented boundary of f lies the point p.\n\n"
         "Precondition\n"
         "• f is finite.")
    .def("remove", [](TriangulationType& tr, const Vertex_handle& v) { return tr.remove(v); },
         py::arg("v"),
         "Removes the vertex from the triangulation.\n"
         "The created hole is re-triangulated.\n\n"
         "Precondition\n"
         "• Vertex v must be finite.")
    .def("remove_degree_3", [](TriangulationType& tr, const Vertex_handle& v) { return tr.remove_degree_3(v); },
         py::arg("v"),
         "Removes a vertex of degree three.\n"
         "Two of the incident faces are destroyed, the third one is modified.\n\n"
         "Precondition\n"
         "• Vertex v is a finite vertex with degree three.")
    .def("segment", [](const TriangulationType& tr, const Edge& e) { return tr.segment(e); },
         py::arg("e"),
         "Returns the line segment corresponding to edge e.\n\n"
         "Precondition\n"
         "• e is a finite edge.")
    .def("segment", [](const TriangulationType& tr, const Edge_circulator& ec) { return tr.segment(ec); },
         py::arg("ec"),
         "Returns the line segment corresponding to edge *ec.\n\n"
         "Precondition\n"
         "• *ec is a finite edge.")
    .def("segment", [](const TriangulationType& tr, const Edge_iterator& ei) { return tr.segment(ei); },
         py::arg("ei"),
         "Returns the line segment corresponding to edge *ei.\n\n"
         "Precondition\n"
         "• *ei is a finite edge.")
    .def("segment", [](const TriangulationType& tr, const Face_handle& f, int i) { return tr.segment(f, i); },
         py::arg("f"), py::arg("i"),
         "Returns the line segment formed by the vertices ccw(i) and cw(i) of face f.\n\n"
         "Precondition\n"
         "• 0≤i≤2. The vertices ccw(i) and cw(i) of f are finite.")
    .def("set_infinite_vertex", [](TriangulationType& tr, const Vertex_handle& v) { return tr.set_infinite_vertex(v); },
         py::arg("v"),
         "This is an advanced function.\n\n"
         "Advanced\n"
         "This method is meant to be used only if you have done a low-level operation on the underlying tds that invalidated the infinite vertex. Sets the infinite vertex.")
    .def("side_of_oriented_circle", [](const TriangulationType& tr, const Face_handle& f, const Point& p) { return tr.side_of_oriented_circle(f, p); },
         py::arg("f"), py::arg("p"),
         "Returns on which side of the circumcircle of face f lies the point p.\n"
         "The circle is assumed to be counterclockwise oriented, so its positive side correspond to its bounded side. This predicate is available only if the corresponding predicates on points is provided in the geometric traits class.")
    .def("star_hole", [](TriangulationType& tr, const Point& p, const std::vector<Edge>& edges) { return tr.star_hole(p, edges.begin(), edges.end()); },
         py::arg("p"), py::arg("edges"),
         "creates a new vertex v and use it to star the hole whose boundary is described by the sequence of edges.\n"
         "Returns a handle to the new vertex.\n"
         "This function is intended to be used in conjunction with the find_conflicts() member functions of Delaunay and constrained Delaunay triangulations to perform insertions.")
    .def("star_hole", [](TriangulationType& tr, const Point& p, const std::vector<Edge>& edges, const std::vector<Face_handle>& faces) { return tr.star_hole(p, edges.begin(), edges.end(), faces.begin(), faces.end()); },
         py::arg("p"), py::arg("edges"), py::arg("faces"),
         "same as star_hole() with edges, except that the algorithm first recycles faces in the sequence and create new ones only when the sequence is exhausted.\n"
         "This function is intended to be used in conjunction with the find_conflicts() member functions of Delaunay and constrained Delaunay triangulations to perform insertions.")
    .def("swap", &TriangulationType::swap,
         py::arg("tr"),
         "The triangulations tr and *this are swapped.\n"
         "This method should be used instead of assignment of copy constructor. if tr is deleted after that.")
    .def("triangle", [](const TriangulationType& tr, const Face_handle& f) { return tr.triangle(f); },
         py::arg("f"),
         "Returns the triangle formed by the three vertices of f.\n\n"
         "Precondition\n"
         "• The face is finite.")
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
    .def("is_face", [](const TriangulationType& tr, const Vertex_handle& v1, const Vertex_handle& v2, const Vertex_handle& v3) { return tr.is_face(v1, v2, v3); },
         py::arg("v1"), py::arg("v2"), py::arg("v3"),
         "true if there is a face having v1, v2 and v3 as vertices.")
    .def("is_face", [](const TriangulationType& tr, const Vertex_handle& v1, const Vertex_handle& v2, const Vertex_handle& v3, Face_handle& fr) { return tr.is_face(v1, v2, v3, fr); },
         py::arg("v1"), py::arg("v2"), py::arg("v3"), py::arg("fr"),
         "true if there is a face having v1, v2 and v3 as vertices.\n"
         "In addition, if true is returned, fr is a handle to the face with v1, v2 and v3 as vertices.")
    
    ;

  // inherits from Triangulation_cw_ccw_2
  tri2::export_triangulation_cw_ccw_2<TriangulationType>(c);
  return c;
}

} // namespace tri2

#endif // CGALPY_TRIANGULATION_2_HPP
// Member Function Documentation
// ◆ all_face_handles()
// template<typename Traits , typename Tds >
// All_face_handles CGAL::Triangulation_2< Traits, Tds >::all_face_handles 	( 		) 	const
//
// returns a range of iterators over all faces.
//
// Note
//     While the value type of All_faces_iterator is Face, the value type of All_face_handles::iterator is Face_handle. 
//
// ◆ all_vertex_handles()
// template<typename Traits , typename Tds >
// All_vertex_handles CGAL::Triangulation_2< Traits, Tds >::all_vertex_handles 	( 		) 	const
//
// returns a range of iterators over all vertices.
//
// Note
//     While the value type of All_vertices_iterator is Vertex, the value type of All_vertex_handles::iterator is Vertex_handle. 
//
// ◆ ccw()
// template<typename Traits , typename Tds >
// int CGAL::Triangulation_2< Traits, Tds >::ccw 	( 	int  	i	) 	const
//
// Returns i+1 modulo 3.
//
// Precondition
//     0≤i≤2. 
//
// ◆ circumcenter()
// template<typename Traits , typename Tds >
// Point CGAL::Triangulation_2< Traits, Tds >::circumcenter 	( 	Face_handle  	f	) 	const
//
// Compute the circumcenter of the face pointed to by f.
//
// This function is available only if the corresponding function is provided in the geometric traits.
// ◆ cw()
// template<typename Traits , typename Tds >
// int CGAL::Triangulation_2< Traits, Tds >::cw 	( 	int  	i	) 	const
//
// Returns i+2 modulo 3.
//
// Precondition
//     0≤i≤2. 
//
// ◆ finite_face_handles()
// template<typename Traits , typename Tds >
// Finite_face_handles CGAL::Triangulation_2< Traits, Tds >::finite_face_handles 	( 		) 	const
//
// returns a range of iterators over finite faces.
//
// Note
//     While the value type of Finite_faces_iterator is Face, the value type of Finite_face_handles::iterator is Face_handle. 
//
// ◆ finite_vertex_handles()
// template<typename Traits , typename Tds >
// Finite_vertex_handles CGAL::Triangulation_2< Traits, Tds >::finite_vertex_handles 	( 		) 	const
//
// returns a range of iterators over finite vertices.
//
// Note
//     While the value type of Finite_vertices_iterator is Vertex, the value type of Finite_vertex_handles::iterator is Vertex_handle. 
//
// ◆ flip()
// template<typename Traits , typename Tds >
// void CGAL::Triangulation_2< Traits, Tds >::flip 	( 	Face_handle  	f,
// 		int  	i 
// 	) 		
//
// Exchanges the edge incident to f and f->neighbor(i) with the other diagonal of the quadrilateral formed by f and f->neighbor(i).
//
// Precondition
//     The faces f and f->neighbor(i) are finite faces and their union form a convex quadrilateral. 
//
// ◆ incident_edges()
// template<typename Traits , typename Tds >
// Edge_circulator CGAL::Triangulation_2< Traits, Tds >::incident_edges 	( 	Vertex_handle  	v,
// 		Face_handle  	f 
// 	) 		const
//
// Starts at the first edge of f incident to v, in counterclockwise order around v.
//
// Precondition
//     Face f is incident to vertex v. 
//
// ◆ incident_faces()
// template<typename Traits , typename Tds >
// Face_circulator CGAL::Triangulation_2< Traits, Tds >::incident_faces 	( 	Vertex_handle  	v,
// 		Face_handle  	f 
// 	) 		const
//
// Starts at face f.
//
// Precondition
//     Face f is incident to vertex v. 
//
// ◆ incident_vertices()
// template<typename Traits , typename Tds >
// Vertex_circulator CGAL::Triangulation_2< Traits, Tds >::incident_vertices 	( 	Vertex_handle  	v,
// 		Face_handle  	f 
// 	) 		
//
// Starts at the first vertex of f adjacent to v in counterclockwise order around v.
//
// Precondition
//     Face f is incident to vertex v. 
//
// ◆ includes_edge()
// template<typename Traits , typename Tds >
// bool CGAL::Triangulation_2< Traits, Tds >::includes_edge 	( 	Vertex_handle  	va,
// 		Vertex_handle  	vb,
// 		Vertex_handle &  	vbb,
// 		Face_handle &  	fr,
// 		int &  	i 
// 	) 		
//
// true if the line segment from va to vb includes an edge e incident to va.
//
// If true, vbb becomes the other vertex of e, e is the edge (fr,i) where fr is a handle to the face incident to e and on the right side e oriented from va to vb.
// ◆ inexact_locate()
// template<typename Traits , typename Tds >
// Face_handle CGAL::Triangulation_2< Traits, Tds >::inexact_locate 	( 	const Point &  	query,
// 		Face_handle  	start = Face_handle() 
// 	) 		const
//
// Same as locate() but uses inexact predicates.
//
// This function returns a handle on a face that is a good approximation of the exact location of query, while being faster. Note that it may return a handle on a face whose interior does not contain query. When the triangulation has dimension smaller than 2, start is returned.
// ◆ insert() [1/3]
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::insert 	( 	const Point &  	p,
// 		Face_handle  	f = Face_handle() 
// 	) 		
//
// Inserts point p in the triangulation and returns the corresponding vertex.
//
// If point p coincides with an already existing vertex, this vertex is returned and the triangulation remains unchanged.
//
// If point p is on an edge, the two incident faces are split in two.
//
// If point p is strictly inside a face of the triangulation, the face is split in three.
//
// If point p is strictly outside the convex hull, p is linked to all visible points on the convex hull to form the new triangulation.
//
// At last, if p is outside the affine hull (in case of degenerate 1-dimensional or 0-dimensional triangulations), p is linked all the other vertices to form a triangulation whose dimension is increased by one. The last argument f is an indication to the underlying locate algorithm of where to start.
//
// Examples
//     Triangulation_2/adding_handles.cpp, Triangulation_2/colored_face.cpp, and Triangulation_2/for_loop_2.cpp.
//
// ◆ insert() [2/3]
// template<typename Traits , typename Tds >
// template<class PointInputIterator >
// std::ptrdiff_t CGAL::Triangulation_2< Traits, Tds >::insert 	( 	PointInputIterator  	first,
// 		PointInputIterator  	last 
// 	) 		
//
// Inserts the points in the range [first,last) in the given order, and returns the number of inserted points.
//
// Template Parameters
//     PointInputIterator	must be an input iterator with value type Point.
//
// ◆ insert() [3/3]
// template<typename Traits , typename Tds >
// template<class PointWithInfoInputIterator >
// std::ptrdiff_t CGAL::Triangulation_2< Traits, Tds >::insert 	( 	PointWithInfoInputIterator  	first,
// 		PointWithInfoInputIterator  	last 
// 	) 		
//
// inserts the points in the iterator range [first,last) in the given order, and returns the number of inserted points.
//
// Given a pair (p,i), the vertex v storing p also stores i, that is v.point() == p and v.info() == i. If several pairs have the same point, only one vertex is created, and one of the objects of type Vertex::Info will be stored in the vertex.
//
// Precondition
//     Vertex must be model of the concept TriangulationVertexBaseWithInfo_2.
//
// Template Parameters
//     PointWithInfoInputIterator	must be an input iterator with the value type std::pair<Point,Vertex::Info>.
//
// ◆ insert_in_edge()
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::insert_in_edge 	( 	const Point &  	p,
// 		Face_handle  	f,
// 		int  	i 
// 	) 		
//
// Inserts vertex v in edge i of f.
//
// Precondition
//     The point in vertex v lies on the edge opposite to the vertex i of face f. 
//
// ◆ insert_in_face()
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::insert_in_face 	( 	const Point &  	p,
// 		Face_handle  	f 
// 	) 		
//
// Inserts vertex v in face f.
//
// Face f is modified, two new faces are created.
//
// Precondition
//     The point in vertex v lies inside face f. 
//
// ◆ insert_outside_convex_hull()
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::insert_outside_convex_hull 	( 	const Point &  	p,
// 		Face_handle  	f 
// 	) 		
//
// Inserts a point which is outside the convex hull but in the affine hull.
//
// Precondition
//     The handle f points to a face which is a proof of the location ofp, see the description of the locate method above. 
//
// ◆ is_edge()
// template<typename Traits , typename Tds >
// bool CGAL::Triangulation_2< Traits, Tds >::is_edge 	( 	Vertex_handle  	va,
// 		Vertex_handle  	vb,
// 		Face_handle &  	fr,
// 		int &  	i 
// 	) 		
//
// as above.
//
// In addition, if true is returned, the edge with vertices va and vb is the edge e=(fr,i) where fr is a handle to the face incident to e and on the right side of e oriented from va to vb.
// ◆ is_face()
// template<typename Traits , typename Tds >
// bool CGAL::Triangulation_2< Traits, Tds >::is_face 	( 	Vertex_handle  	v1,
// 		Vertex_handle  	v2,
// 		Vertex_handle  	v3,
// 		Face_handle &  	fr 
// 	) 		
//
// as above.
//
// In addition, if true is returned, fr is a handle to the face with v1, v2 and v3 as vertices.
// ◆ is_valid()
// template<typename Traits , typename Tds >
// bool CGAL::Triangulation_2< Traits, Tds >::is_valid 	( 	bool  	verbose = false,
// 		int  	level = 0 
// 	) 		const
//
// Checks the combinatorial validity of the triangulation and also the validity of its geometric embedding.
//
// This method is mainly a debugging help for the users of advanced features.
// ◆ line_walk()
// template<typename Traits , typename Tds >
// Line_face_circulator CGAL::Triangulation_2< Traits, Tds >::line_walk 	( 	const Point &  	p,
// 		const Point &  	q,
// 		Face_handle  	f = Face_handle() 
// 	) 		const
//
// This function returns a circulator that allows to visit the faces intersected by the line pq.
//
// If there is no such face the circulator has a singular value.
//
// The starting point of the circulator is the face f, or the first finite face traversed by l , if f is omitted.
//
// The circulator wraps around the infinite vertex: after the last traversed finite face, it steps through the infinite face adjacent to this face then through the infinite face adjacent to the first traversed finite face then through the first finite traversed face again.
//
// Precondition
//     The triangulation must have dimension 2. 
//     Points p and q must be different points. 
//     If f != nullptr, it must point to a finite face and the point p must be inside or on the boundary of f. 
//
// ◆ locate() [1/2]
// template<typename Traits , typename Tds >
// Face_handle CGAL::Triangulation_2< Traits, Tds >::locate 	( 	const Point &  	query,
// 		Face_handle  	f = Face_handle() 
// 	) 		const
//
// If the point query lies inside the convex hull of the points, a face that contains the query in its interior or on its boundary is returned.
//
// If the point query lies outside the convex hull of the triangulation but in the affine hull, the returned face is an infinite face which is a proof of the point's location:
//
//     for a two dimensional triangulation, it is a face (∞,p,q) such that query lies to the left of the oriented line pq (the rest of the triangulation lying to the right of this line).
//     for a degenerate one dimensional triangulation it is the (degenerate one dimensional) face (∞,p,nullptr) such that query and the triangulation lie on either side of p.
//
// If the point query lies outside the affine hull, the returned Face_handle is nullptr.
//
// The optional Face_handle argument, if provided, is used as a hint of where the locate process has to start its search.
// ◆ locate() [2/2]
// template<typename Traits , typename Tds >
// Face_handle CGAL::Triangulation_2< Traits, Tds >::locate 	( 	const Point &  	query,
// 		Locate_type &  	lt,
// 		int &  	li,
// 		Face_handle  	h = Face_handle() 
// 	) 		const
//
// Same as above.
//
// Additionally, the parameters lt and li describe where the query point is located. The variable lt is set to the locate type of the query. If lt==VERTEX the variable li is set to the index of the vertex, and if lt==EDGE li is set to the index of the vertex opposite to the edge. Be careful that li has no meaning when the query type is FACE, OUTSIDE_CONVEX_HULL, or OUTSIDE_AFFINE_HULL or when the triangulation is 0-dimensional.
// ◆ mirror_edge()
// template<typename Traits , typename Tds >
// Edge CGAL::Triangulation_2< Traits, Tds >::mirror_edge 	( 	Edge  	e	) 	const
//
// returns the same edge seen from the other adjacent face.
//
// Precondition
//     0≤i≤2. 
//
// ◆ mirror_index()
// template<typename Traits , typename Tds >
// int CGAL::Triangulation_2< Traits, Tds >::mirror_index 	( 	Face_handle  	f,
// 		int  	i 
// 	) 		const
//
// returns the index of f in its ith neighbor.
//
// Precondition
//     0≤i≤2. 
//
// ◆ mirror_vertex()
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::mirror_vertex 	( 	Face_handle  	f,
// 		int  	i 
// 	) 		const
//
// returns the vertex of the ith neighbor of f that is opposite to f.
//
// Precondition
//     0≤i≤2. 
//
// ◆ move()
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::move 	( 	Vertex_handle  	v,
// 		const Point &  	p 
// 	) 		
//
// If there is no collision during the move, this function is the same as move_if_no_collision .
//
// Otherwise, v is removed and the vertex at point p is returned.
//
// Precondition
//     Vertex v must be finite. 
//
// ◆ move_if_no_collision()
// template<typename Traits , typename Tds >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::move_if_no_collision 	( 	Vertex_handle  	v,
// 		const Point &  	p 
// 	) 		
//
// If there is not already another vertex placed on p, the triangulation is modified such that the new position of vertex v is p, and v is returned.
//
// Otherwise, the triangulation is not modified and the vertex at point p is returned.
//
// Precondition
//     Vertex v must be finite. 
//
// ◆ operator=()
// template<typename Traits , typename Tds >
// Triangulation_2 CGAL::Triangulation_2< Traits, Tds >::operator= 	( 	const Triangulation_2< Traits, Tds > &  	tr	) 	
//
// Assignment.
//
// All the vertices and faces are duplicated. After the assignment, *this and tr refer to different triangulations: if tr is modified, *this is not.
// ◆ oriented_side()
// template<typename Traits , typename Tds >
// Oriented_side CGAL::Triangulation_2< Traits, Tds >::oriented_side 	( 	Face_handle  	f,
// 		const Point &  	p 
// 	) 		const
//
// Returns on which side of the oriented boundary of f lies the point p.
//
// Precondition
//     f is finite. 
//
// ◆ remove()
// template<typename Traits , typename Tds >
// void CGAL::Triangulation_2< Traits, Tds >::remove 	( 	Vertex_handle  	v	) 	
//
// Removes the vertex from the triangulation.
//
// The created hole is re-triangulated.
//
// Precondition
//     Vertex v must be finite. 
//
// ◆ remove_degree_3()
// template<typename Traits , typename Tds >
// void CGAL::Triangulation_2< Traits, Tds >::remove_degree_3 	( 	Vertex_handle  	v	) 	
//
// Removes a vertex of degree three.
//
// Two of the incident faces are destroyed, the third one is modified.
//
// Precondition
//     Vertex v is a finite vertex with degree three. 
//
// ◆ segment() [1/4]
// template<typename Traits , typename Tds >
// Segment CGAL::Triangulation_2< Traits, Tds >::segment 	( 	const Edge &  	e	) 	const
//
// Returns the line segment corresponding to edge e.
//
// Precondition
//     e is a finite edge. 
//
// ◆ segment() [2/4]
// template<typename Traits , typename Tds >
// Segment CGAL::Triangulation_2< Traits, Tds >::segment 	( 	const Edge_circulator &  	ec	) 	const
//
// Returns the line segment corresponding to edge *ec.
//
// Precondition
//     *ec is a finite edge. 
//
// ◆ segment() [3/4]
// template<typename Traits , typename Tds >
// Segment CGAL::Triangulation_2< Traits, Tds >::segment 	( 	const Edge_iterator &  	ei	) 	const
//
// Returns the line segment corresponding to edge *ei.
//
// Precondition
//     *ei is a finite edge. 
//
// ◆ segment() [4/4]
// template<typename Traits , typename Tds >
// Segment CGAL::Triangulation_2< Traits, Tds >::segment 	( 	Face_handle  	f,
// 		int  	i 
// 	) 		const
//
// Returns the line segment formed by the vertices ccw(i) and cw(i) of face f.
//
// Precondition
//     0≤i≤2. The vertices ccw(i) and cw(i) of f are finite. 
//
// ◆ set_infinite_vertex()
// template<typename Traits , typename Tds >
// void CGAL::Triangulation_2< Traits, Tds >::set_infinite_vertex 	( 	const Vertex_handle &  	v	) 	
//
// This is an advanced function.
// Advanced
//
// This method is meant to be used only if you have done a low-level operation on the underlying tds that invalidated the infinite vertex. Sets the infinite vertex.
// ◆ side_of_oriented_circle()
// template<typename Traits , typename Tds >
// Oriented_side CGAL::Triangulation_2< Traits, Tds >::side_of_oriented_circle 	( 	Face_handle  	f,
// 		const Point &  	p 
// 	) 		
//
// Returns on which side of the circumcircle of face f lies the point p.
//
// The circle is assumed to be counterclockwise oriented, so its positive side correspond to its bounded side. This predicate is available only if the corresponding predicates on points is provided in the geometric traits class.
// ◆ star_hole() [1/2]
// template<typename Traits , typename Tds >
// template<class EdgeIt >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::star_hole 	( 	Point  	p,
// 		EdgeIt  	edge_begin,
// 		EdgeIt  	edge_end 
// 	) 		
//
// creates a new vertex v and use it to star the hole whose boundary is described by the sequence of edges [edge_begin, edge_end).
//
// Returns a handle to the new vertex.
//
// This function is intended to be used in conjunction with the find_conflicts() member functions of Delaunay and constrained Delaunay triangulations to perform insertions.
// ◆ star_hole() [2/2]
// template<typename Traits , typename Tds >
// template<class EdgeIt , class FaceIt >
// Vertex_handle CGAL::Triangulation_2< Traits, Tds >::star_hole 	( 	Point  	p,
// 		EdgeIt  	edge_begin,
// 		EdgeIt  	edge_end,
// 		FaceIt  	face_begin,
// 		FaceIt  	face_end 
// 	) 		
//
// same as above, except that the algorithm first recycles faces in the sequence [face_begin, face_end) and create new ones only when the sequence is exhausted.
//
// This function is intended to be used in conjunction with the find_conflicts() member functions of Delaunay and constrained Delaunay triangulations to perform insertions.
// ◆ swap()
// template<typename Traits , typename Tds >
// void CGAL::Triangulation_2< Traits, Tds >::swap 	( 	Triangulation_2< Traits, Tds > &  	tr	) 	
//
// The triangulations tr and *this are swapped.
//
// This method should be used instead of assignment of copy constructor. if tr is deleted after that.
// ◆ triangle()
// template<typename Traits , typename Tds >
// Triangle CGAL::Triangulation_2< Traits, Tds >::triangle 	( 	Face_handle  	f	) 	const
//
// Returns the triangle formed by the three vertices of f.
//
// Precondition
//     The face is finite. 
//
