#!/usr/bin/python3.7
# export PYTHONPATH=...

import sys
import importlib

if len(sys.argv) < 2:
  sys.exit('Library name missing')

Arr2 = importlib.import_module(sys.argv[1]+".Arr2")
Ker = importlib.import_module(sys.argv[1]+".Ker")

Point_2 = Ker.Point_2
Segment_2 = Ker.Segment_2

s1 = Segment_2(Point_2(0, 1), Point_2(1,0))
s2 = Segment_2(Point_2(0, 0), Point_2(1,1))
ix = Ker.intersection(s1, s2)
if not ix.is_point():
  sys.exit('Wrong intersection')
p = Point_2()
ix.get_point(p)
print(p)
