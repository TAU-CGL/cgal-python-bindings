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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename1 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby.off')
i += 1
filename2 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off')

mesh1 = Sm.read_polygon_mesh(filename1)
mesh2 = Sm.read_polygon_mesh(filename2)

visitor = Pmp.Non_manifold_output_visitor(mesh1, mesh2)

# try:
valid_inter = Pmp.corefine_and_compute_intersection(mesh1, mesh2, {'visitor': visitor})
print("Intersection was successfully computed as a manifold mesh")
Sm.write_polygon_mesh("inter.off", valid_inter)
# except:
# print("Intersection was successfully computed but is non-manifold, exporting a triangle soup")
# points, polygons = visitor.extract_intersection()
# CGALPY.IO.write_polygon_soup("inter_soup.off", points, polygons)
# Pmp.orient_polygon_soup(points, polygons)
# out = Pmp.polygon_soup_to_polygon_mesh(points, polygons)
# Sm.write_polygon_mesh("inter.off", out)


# visitor = Pmp.Non_manifold_output_visitor(mesh1, mesh2) # Takes references to the input meshes
# out = Pmp.corefine_and_compute_intersection(mesh1, mesh2, {'visitor': visitor})
# # Something gets deleted here, not sure what
# points, polygons = visitor.extract_intersection() # Crash
# Sm.write_polygon_mesh("inter.off", out)
# Sm.write_polygon_soup("inter2.off", mesh1)
# Sm.write_polygon_soup("inter3.off", mesh2)
