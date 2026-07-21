#!/usr/bin/python

import importlib
import sys

if len(sys.argv) < 2:
    lib = "CGALPY"
else:
    lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point = Ker.Point_2
Segment = Ker.Segment_2

p = Point(1, 1)
q = Point(10, 10)

print(f"p = {p}")
print(f"q = {q.x()} {q.y()}")
print(f"sqdist(p,q) = {Ker.squared_distance(p, q)}")

s = Segment(p, q)
m = Point(5, 9)

print(f"m = {m}")
print(f"sqdist(Segment_2(p,q), m) = {Ker.squared_distance(s, m)}")

if Ker.collinear(p, q, m):
    turn = "are collinear"
elif Ker.left_turn(p, q, m):
    turn = "make a left turn"
else:
    turn = "make a right turn"

print(f"p, q, and m {turn}")
print(f" midpoint(p,q) = {Ker.midpoint(p, q)}")
