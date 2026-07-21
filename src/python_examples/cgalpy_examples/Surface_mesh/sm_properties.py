#!/usr/bin/python

import os
import sys
import importlib

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm

m = Sm.Surface_mesh_3()
v0 = m.add_vertex(CGALPY.Ker.Point_3(0,2,0))
v1 = m.add_vertex(CGALPY.Ker.Point_3(2,2,0))
v2 = m.add_vertex(CGALPY.Ker.Point_3(0,0,0))
v3 = m.add_vertex(CGALPY.Ker.Point_3(2,0,0))
v4 = m.add_vertex(CGALPY.Ker.Point_3(1,1,0))
m.add_face(v3, v1, v4)
m.add_face(v0, v4, v1)
m.add_face(v0, v2, v4)
m.add_face(v2, v3, v4)

# give each vertex a name, the default is empty
name, created = m.add_property_map_vertex_string("v:name","m1")
assert created
# add some names to the vertices
name[v0] = "hello"
name[v2] = "world"

# You get an existing property, and created will be false
name, created = m.add_property_map_vertex_string("v:name", "")
assert not created

# You can't get a property that does not exist
gnus = m.property_map_face_string("v:gnus")
assert not gnus

# retrieve the point property for which exists a convenience function
location = m.points()
for vd in Sm.vertices(m):
    print(name[vd], " @ ", location[vd])

m2 = Sm.Surface_mesh_3()
Sm.make_triangle(CGALPY.Ker.Point_3(0,0,1), CGALPY.Ker.Point_3(1,0,1),CGALPY.Ker.Point_3(0,1,1), m2)

m2.add_property_map_vertex_string("v:name","m2")
index = m2.add_property_map_vertex_int("v:index",-1)[0]
i = 0
for v in Sm.vertices(m2):
    index[v] = i
    i += 1

print("properties of m1:")
props = m.properties_vertex()
for p in props:
    print(p)

m.join(m2)
print("properties of m1 after join:")
for p in m.properties_vertex():
    print(p)

for v in Sm.vertices(m):
    print(name[v])
# delete the string property again
m.remove_property_map(name)
