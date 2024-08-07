#!/usr/bin/python
import os
import sys
import importlib
lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm

filename = sys.argv[i] if len(sys.argv) > i else "meshes/blobby.off"
try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

vertex = Sm.vertices(mesh)[0]
edge = Sm.edges(mesh)[0]
halfedge = Sm.halfedges(mesh)[0]
face = Sm.faces(mesh)[0]

print("num_vertices: ", Sm.num_vertices(mesh))
print("num_edges: ", Sm.num_edges(mesh))
print("degree_v: ", Sm.degree(vertex, mesh))
print("degree_f: ", Sm.degree(face, mesh))
print("out_degree: ", Sm.out_degree(vertex, mesh))
print("in_degree: ", Sm.in_degree(vertex, mesh))
print("source_e: ", Sm.source(edge, mesh))
print("source_h: ", Sm.source(halfedge, mesh))
print("target_e: ", Sm.target(edge, mesh))
print("target_h: ", Sm.target(halfedge, mesh))
print("vertices: ", Sm.vertices(mesh))
print("edges: ", Sm.edges(mesh))
print("in_edges: ", Sm.in_edges(vertex, mesh))
print("out_edges: ", Sm.out_edges(vertex, mesh))
print("adjacent_vertices: ", Sm.adjacent_vertices(vertex, mesh))
print("edge: ", Sm.edge(vertex, vertex, mesh))
print("next: ", Sm.next(halfedge, mesh))
print("prev: ", Sm.prev(halfedge, mesh))
print("opposite: ", Sm.opposite(halfedge, mesh))
print("edge_h: ", Sm.edge(halfedge, mesh))
print("halfedge_e: ", Sm.halfedge(edge, mesh))
print("halfedge_v: ", Sm.halfedge(vertex, mesh))
print("halfedge_vv: ", Sm.halfedge(vertex, vertex, mesh))
print("halfedges: ", Sm.halfedges(mesh))
print("num_halfedges: ", Sm.num_halfedges(mesh))
print("set_next: ", Sm.set_next(halfedge, halfedge, mesh))
print("set_target: ", Sm.set_target(halfedge, vertex, mesh))
print("set_halfedge_vh: ", Sm.set_halfedge(vertex, halfedge, mesh))
print("collect_garbage: ", Sm.collect_garbage(mesh))
print("add_edge: ", Sm.add_edge(mesh))
print("halfedge_f: ", Sm.halfedge(face, mesh))
print("face_h: ", Sm.face(halfedge, mesh))
print("set_face: ", Sm.set_face(halfedge, face, mesh))
print("set_halfedge_fh: ", Sm.set_halfedge(face, halfedge, mesh))
print("num_faces: ", Sm.num_faces(mesh))
print("faces: ", Sm.faces(mesh))
print("add_vertex: ", Sm.add_vertex(mesh))
print("add_vertex_p: ", Sm.add_vertex(Ker.Point_3(0, 0, 0), mesh))
print("reserve: ", Sm.reserve(mesh, 10, 10, 10))
print("remove_vertex: ", Sm.remove_vertex(vertex, mesh))
print("remove_edge: ", Sm.remove_edge(edge, mesh))
print("remove_face: ", Sm.remove_face(face, mesh))
print("remove_all_elements: ", Sm.remove_all_elements(mesh))
print("add_face: ", Sm.add_face(mesh))
mesh = Sm.read_polygon_mesh(filename)
vertex = Sm.vertices(mesh)[0]
edge = Sm.edges(mesh)[0]
halfedge = Sm.halfedges(mesh)[0]
face = Sm.faces(mesh)[0]
print("is_valid_vertex_descriptor: ", Sm.is_valid_vertex_descriptor(vertex, mesh))
print("is_valid_halfedge_descriptor: ", Sm.is_valid_halfedge_descriptor(halfedge, mesh))
print("is_valid_edge_descriptor: ", Sm.is_valid_edge_descriptor(edge, mesh))
print("is_valid_face_descriptor: ", Sm.is_valid_face_descriptor(face, mesh))
