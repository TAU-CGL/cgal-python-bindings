#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm

m = Sm.Surface_mesh_3()

u: Sm.Vertex_index
for i in range(5):
  v = m.add_vertex(CGALPY.Ker.Point_3(0,0,i+1))
  if i == 2:
    u = v

m.remove_vertex(u)

print(f"""
After insertion of 5 vertices and removal of the 3. vertex\n
# vertices  / # vertices + # removed vertices =
{m.number_of_vertices()}
 / {m.number_of_vertices() + m.number_of_removed_vertices()}
""")

print("Iterate over vertices")

for vd in Sm.vertices(m): print(m.point(vd))

# The status of being used or removed is stored in a property map
removed = m.property_map_vertex_bool("v:removed")
if not removed:
  print("Property map not found")
  exit(1)

print(f"""
Iterate over vertices and deleted vertices\n
# vertices / # vertices + # removed vertices =
{m.number_of_vertices()}
 / {m.number_of_vertices() + m.number_of_removed_vertices()}
""")
for i in range(m.number_of_vertices() + m.number_of_removed_vertices()):
  vh = Sm.Vertex_index(i)
  assert m.is_removed(vh) == removed[vh]
  print(m.point(vh), "R" if m.is_removed(vh) else "")

m.collect_garbage()

print(f"""
After garbage collection\n
# vertices / # vertices + # removed vertices =
{m.number_of_vertices()}
 / {m.number_of_vertices() + m.number_of_removed_vertices()}
""")

end = m.number_of_vertices() + m.number_of_removed_vertices()
for i in range(end):
  vh = Sm.Vertex_index(i)
  print(m.point(vh), "R" if m.is_removed(vh) else "")
