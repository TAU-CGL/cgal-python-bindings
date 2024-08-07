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
Pol3 = CGALPY.Pol3

filename = sys.argv[i] if len(sys.argv) > i else 'meshes/blobby.off'
try: mesh = Pol3.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

vertex = Pol3.vertices(mesh)[0]
edge = Pol3.edges(mesh)[0]
halfedge = Pol3.halfedges(mesh)[0]
face = Pol3.faces(mesh)[0]

print("num_vertices: ", Pol3.num_vertices(mesh))
print("num_edges: ", Pol3.num_edges(mesh))
print("degree_v: ", Pol3.degree(vertex, mesh))
print("degree_f: ", Pol3.degree(face, mesh))
print("out_degree: ", Pol3.out_degree(vertex, mesh))
print("in_degree: ", Pol3.in_degree(vertex, mesh))
print("source_e: ", Pol3.source(edge, mesh))
print("source_h: ", Pol3.source(halfedge, mesh))
print("target_e: ", Pol3.target(edge, mesh))
print("target_h: ", Pol3.target(halfedge, mesh))
print("vertices: ", Pol3.vertices(mesh))
print("edges: ", Pol3.edges(mesh))
print("in_edges: ", Pol3.in_edges(vertex, mesh))
print("out_edges: ", Pol3.out_edges(vertex, mesh))
print("adjacent_vertices: ", Pol3.adjacent_vertices(vertex, mesh))
print("edge: ", Pol3.edge(vertex, vertex, mesh))
print("next: ", Pol3.next(halfedge, mesh))
print("prev: ", Pol3.prev(halfedge, mesh))
print("opposite: ", Pol3.opposite(halfedge, mesh))
print("edge_h: ", Pol3.edge(halfedge, mesh))
print("halfedge_e: ", Pol3.halfedge(edge, mesh))
print("halfedge_v: ", Pol3.halfedge(vertex, mesh))
print("halfedge_vv: ", Pol3.halfedge(vertex, vertex, mesh))
print("halfedges: ", Pol3.halfedges(mesh))
print("num_halfedges: ", Pol3.num_halfedges(mesh))
print("set_next: ", Pol3.set_next(halfedge, halfedge, mesh))
print("set_target: ", Pol3.set_target(halfedge, vertex, mesh))
print("set_halfedge_vh: ", Pol3.set_halfedge(vertex, halfedge, mesh))
print("collect_garbage: ", Pol3.collect_garbage(mesh))
print("add_edge: ", Pol3.add_edge(mesh))
print("halfedge_f: ", Pol3.halfedge(face, mesh))
print("face_h: ", Pol3.face(halfedge, mesh))
print("set_face: ", Pol3.set_face(halfedge, face, mesh))
print("set_halfedge_fh: ", Pol3.set_halfedge(face, halfedge, mesh))
print("num_faces: ", Pol3.num_faces(mesh))
print("faces: ", Pol3.faces(mesh))
print("add_vertex: ", Pol3.add_vertex(mesh))
print("add_vertex_p: ", Pol3.add_vertex(Ker.Point_3(0, 0, 0), mesh))
print("reserve: ", Pol3.reserve(mesh, 10, 10, 10))
print("remove_vertex: ", Pol3.remove_vertex(vertex, mesh))
print("remove_edge: ", Pol3.remove_edge(edge, mesh))
print("remove_face: ", Pol3.remove_face(face, mesh))
print("remove_all_elements: ", Pol3.remove_all_elements(mesh))
print("add_face: ", Pol3.add_face(mesh))
mesh = Pol3.read_polygon_mesh(filename)
vertex = Pol3.vertices(mesh)[0]
edge = Pol3.edges(mesh)[0]
halfedge = Pol3.halfedges(mesh)[0]
face = Pol3.faces(mesh)[0]
print("is_valid_vertex_descriptor: ", Pol3.is_valid_vertex_descriptor(vertex, mesh))
print("is_valid_halfedge_descriptor: ", Pol3.is_valid_halfedge_descriptor(halfedge, mesh))
print("is_valid_edge_descriptor: ", Pol3.is_valid_edge_descriptor(edge, mesh))
print("is_valid_face_descriptor: ", Pol3.is_valid_face_descriptor(face, mesh))
