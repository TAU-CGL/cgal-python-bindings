# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
# #include <CGAL/Polygon_mesh_processing/remesh_planar_patches.h>
# #include <CGAL/Polygon_mesh_processing/remesh.h>
# #include <CGAL/Polygon_mesh_processing/detect_features.h>
# #include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <iostream>
# #include <fstream>
#
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
# typedef Kernel::Point_3 Point_3;
# typedef CGAL::Surface_mesh<Kernel::Point_3> Mesh;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
# int main()
# {
#   Mesh sm;
#   CGAL::IO::read_polygon_mesh(CGAL::data_file_path("meshes/cube_quad.off"), sm);
#
#   // triangulate faces;
#   PMP::triangulate_faces(sm);
#   std::cout << "Input mesh has " << faces(sm).size() << " faces" << std::endl;
#   assert(faces(sm).size()==12);
#
#   Mesh::Property_map<Mesh::Edge_index, bool> ecm =
#     sm.add_property_map<Mesh::Edge_index, bool>("ecm",false).first;
#
#   // detect sharp edges of the cube
#   PMP::detect_sharp_edges(sm, 60, ecm);
#
#   // create a remeshed version of the cube with many elements
#   PMP::isotropic_remeshing(faces(sm), 0.1, sm, CGAL::parameters::edge_is_constrained_map(ecm));
#   CGAL::IO::write_polygon_mesh("cube_remeshed.off", sm, CGAL::parameters::stream_precision(17));
#   assert(faces(sm).size()>100);
#
#   // decimate the mesh
#   Mesh out;
#   PMP::remesh_planar_patches(sm, out);
#   CGAL::IO::write_polygon_mesh("cube_decimated.off", out, CGAL::parameters::stream_precision(17));
#
#   // we should be back to 12 faces
#   std::cout << "Output mesh has " << faces(out).size() << " faces" << std::endl;
#   assert(faces(out).size()==12);
#
#   return EXIT_SUCCESS;
# }

import time
import os
import sys
import importlib
if len(sys.argv) < 3:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = 'meshes/cube_quad.off' if len(sys.argv) < 2 else sys.argv[1]

sm = Sm.read_polygon_mesh(filename)

Pmp.triangulate_faces(sm)
print(f"Input mesh has {len(list(sm.faces()))} faces")
assert len(list(sm.faces())) == 12

ecm = sm.add_property_map_edge_bool("ecm", False)[0]

Pmp.detect_sharp_edges(sm, 60, ecm)

Pmp.isotropic_remeshing(list(sm.faces()), 0.1, sm, {"edge_is_constrained_map": ecm})

Sm.write_polygon_mesh("cube_remeshed.off", sm, {"stream_precision": 17})
assert len(list(sm.faces())) > 100

out = Pmp.remesh_planar_patches(sm)

Sm.write_polygon_mesh("cube_decimated.off", out, {"stream_precision": 17})

print(f"Output mesh has {len(list(out.faces()))} faces")
assert len(list(out.faces())) == 12

