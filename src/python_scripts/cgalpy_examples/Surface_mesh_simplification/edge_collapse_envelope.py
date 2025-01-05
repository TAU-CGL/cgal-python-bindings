import math
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

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Sms = CGALPY.Sms
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/helmet.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    exit(1)

stop = Sms.Edge_count_stop_predicate(0) # go as far as you can while in the envelope

bbox = Pmp.bbox(surface_mesh)

cmin = Ker.Point_3(bbox.xmin(), bbox.ymin(), bbox.zmin())
cmax = Ker.Point_3(bbox.xmax(), bbox.ymax(), bbox.zmax())
diag = math.sqrt(Ker.squared_distance(cmin, cmax))

print(f"eps = {0.01*diag}")
placement = Sms.LindstromTurk_placement()
filter = Sms.Polyhedral_envelope_filter(0.01*diag)
Sms.edge_collapse(surface_mesh, stop, {"get_placement": placement, "filter": filter})

Sm.write_polygon_mesh("out.off", surface_mesh)
