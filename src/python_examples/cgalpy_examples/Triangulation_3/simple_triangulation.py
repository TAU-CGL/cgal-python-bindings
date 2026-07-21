#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Tri3 = CGALPY.Tri3
Point = Ker.Point_3

T = Tri3.Triangulation_3([Point(0,0,0), Point(1,0,0), Point(0,1,0)])
n = T.number_of_vertices()
V = [Point(0,0,1), Point(1,1,1), Point(2,2,2)]
n = n + T.insert(V)
assert(n == 6)       # 6 points have been inserted
assert(T.is_valid()) # checking validity of T
p = Point(0,0,0)
lt, c, li = T.locate_face(p)
# p is the vertex of c of index li :
assert(lt == Tri3.Triangulation_3.VERTEX)
assert(c.vertex(li).point() == p)

v = c.vertex((li + 1) & 3)
# v is another vertex of c
nc = c.neighbor(li)
# nc = neighbor of c opposite to the vertex associated with p
# nc must have vertex v :
res, nli = nc.has_vertex(v)
assert(res)
# nli is the index of v in nc

# std::ofstream oFileT("output",std::ios::out);
# # writing file output;
fout = open("output.txt", "w")
fout.write(str(T))
fout.close()
fin = open("output.txt", "r")
T1 = Tri3.Triangulation_3(fin.read())
fin.close()

assert(T1.is_valid())
assert(T1.number_of_vertices() == T.number_of_vertices())
assert(T1.number_of_cells() == T.number_of_cells())
