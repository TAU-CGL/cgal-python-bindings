import os
import sys
import importlib
# Python output:
# 5 vertices: (3 3) - Isolated (1 3) - degree 2 (3 5) - degree 2 (5 3) - degree 2 (3 1) - degree 2 4 edges: [1 3 3 5] [3 5 5 3] [5 3 3 1] [3 1 1 3] 2 faces: Unbounded face. Hole #1: (1 3) [1 3 3 5] (3 5) [3 5 5 3] (5 3) [5 3 3 1] (3 1) [3 1 1 3] (1 3) Outer boundary: (1 3) [3 1 1 3] (3 1) [5 3 3 1] (5 3) [3 5 5 3] (3 5) [1 3 3 5] (1 3) Isolated vertex #1: (3 3)
# C++ output:
# 5 vertices: (3 3) - Isolated. (1 3) - degree 2 (3 5) - degree 2 (5 3) - degree 2 (3 1) - degree 2 4 edges: [1 3 3 5] [3 5 5 3] [5 3 3 1] [3 1 1 3] 2 faces: Unbounded face. Hole #1: (1 3) [1 3 3 5] (3 5) [3 5 5 3] (5 3) [5 3 3 1] (3 1) [3 1 1 3] (1 3) Outer boundary: (1 3) [3 1 1 3] (3 1) [5 3 3 1] (5 3) [3 5 5 3] (3 5) [1 3 3 5] (1 3) Isolated vertex #1: (3 3)
# Difference:
# 1c1
# < 5 vertices: (3 3) - Isolated (1 3) - degree 2 (3 5) - degree 2 (5 3) - degree 2 (3 1) - degree 2 4 edges: [1 3 3 5] [3 5 5 3] [5 3 3 1] [3 1 1 3] 2 faces: Unbounded face. Hole #1: (1 3) [1 3 3 5] (3 5) [3 5 5 3] (5 3) [5 3 3 1] (3 1) [3 1 1 3] (1 3) Outer boundary: (1 3) [3 1 1 3] (3 1) [5 3 3 1] (5 3) [3 5 5 3] (3 5) [1 3 3 5] (1 3) Isolated vertex #1: (3 3)
# ---
# > 5 vertices: (3 3) - Isolated. (1 3) - degree 2 (3 5) - degree 2 (5 3) - degree 2 (3 1) - degree 2 4 edges: [1 3 3 5] [3 5 5 3] [5 3 3 1] [3 1 1 3] 2 faces: Unbounded face. Hole #1: (1 3) [1 3 3 5] (3 5) [3 5 5 3] (5 3) [5 3 3 1] (3 1) [3 1 1 3] (1 3) Outer boundary: (1 3) [3 1 1 3] (3 1) [5 3 3 1] (5 3) [3 5 5 3] (3 5) [1 3 3 5] (1 3) Isolated vertex #1: (3 3)
# fix this file

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2

#
def print_ccb(ccb):
  e = next(ccb)
  print('({})'.format(e.source().point()), end='')
  print(' [{}] ({})'.format(e.curve(), e.target().point()), end='')
  for e in ccb:
    print(' [{}] ({})'.format(e.curve(), e.target().point()), end='')
  print()

#
def print_face(f):
  # Print the outer boundary.
  if f.is_unbounded(): print('Unbounded face.')
  else:
    print('Outer boundary: ', end='')
    print_ccb(f.outer_ccb())

  # Print the boundary of each of the holes.
  ind = 1
  for hole in f.holes():
    print('    Hole #{}: '.format(ind), end='')
    print_ccb(hole)
    ind = ind + 1

  # Print the isolated vertices.
  ind = 1
  iv: f.Vertex
  for iv in f.isolated_vertices():
    print('    Isolated vertex #{}: ({})'.format(ind, iv.point()))
    ind = ind + 1

# Print an arrangement object
def print_arrangement(arr):
  assert(arr.is_valid())

  # Print the arrangement vertices.
  print('{} vertices:'.format(arr.number_of_vertices()))
  v: Arrangement.Vertex
  for v in arr.vertices():
    if v.is_isolated(): print('({})  - Isolated.'.format(v.point()))
    else: print('({})  - degree {}'.format(v.point(), v.degree()))

  # Print the arrangement edges.
  print('{} edges:'.format(arr.number_of_edges()))
  e: Arrangement.Halfedge
  for e in arr.edges(): print('[{}]'.format(e.curve()))

  # Print the arrangement faces.
  print('{} faces:'.format(arr.number_of_faces()))
  f: Arrangement.Face
  for f in arr.faces(): print_face(f)

# Print the number of cells of a given arrangement.
def print_arrangement_size(arr):
  print('The arrangement size:')
  print('   |V| =  {}, |E| = {}, |F| = {}'.format(arr.number_of_vertices(),
                                                  arr.number_of_edges(),
                                                  arr.number_of_faces()))

def print_unbounded_arrangement_size(arr):
  print("The arrangement size:")
  print('   |V| = {} (plus {} at infinity), |E| = {}, |F| = {} ({} unbounded)'.
        format(arr.number_of_vertices(),
               arr.number_of_vertices_at_infinity(),
               arr.number_of_edges(),
               arr.number_of_faces(),
               arr.number_of_unbounded_faces()))
