import time
import os
import sys
import importlib
if len(sys.argv) < 5:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[4]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

def is_small_hole(h, mesh, max_hole_diam, max_num_hole_edges):
    num_hole_edges = 0
    hole_bbox = Ker.Bbox_3()
    for hc in Pmp.halfedges_around_face(h, mesh):
        p = mesh.point(Sm.target(hc, mesh))
        hole_bbox += p.bbox()
        num_hole_edges += 1
        if num_hole_edges > max_num_hole_edges:
            return False
        if hole_bbox.xmax() - hole_bbox.xmin() > max_hole_diam:
            return False
        if hole_bbox.ymax() - hole_bbox.ymin() > max_hole_diam:
            return False
        if hole_bbox.zmax() - hole_bbox.zmin() > max_hole_diam:
            return False
    return True

class Stop(Exception):
    def __init__(self):
        pass

class Progress():

    def __init__(self, time_limit):
        self.visitor = Pmp.Hole_filling_default_visitor()
        Pmp.set_start_planar_phase(self.visitor, self.start_planar_phase)
        Pmp.set_end_planar_phase(self.visitor, self.end_planar_phase)
        Pmp.set_start_quadratic_phase(self.visitor, self.start_quadratic_phase)
        Pmp.set_quadratic_step(self.visitor, self.quadratic_step)
        Pmp.set_end_quadratic_phase(self.visitor, self.end_quadratic_phase)
        Pmp.set_start_cubic_phase(self.visitor, self.start_cubic_phase)
        Pmp.set_cubic_step(self.visitor, self.cubic_step)
        Pmp.set_end_cubic_phase(self.visitor, self.end_cubic_phase)
        self.time_limit = time_limit
        self.quadratic_n = 0
        self.quadratic_report = 0
        self.cubic_n = 0
        self.cubic_report = 0

    def start_planar_phase(self):
        print("Start planar phase")

    def end_planar_phase(self, success):
        print(f"End planar phase {(success and '(success)' or '(failed)')}")

    def start_quadratic_phase(self, n):
        self.timer = time.time()
        self.quadratic_i = 0
        self.quadratic_n = n
        self.quadratic_report = n // 10
        print(f"Start quadratic phase with estimated {n} steps")

    def quadratic_step(self):
        if self.quadratic_i == self.quadratic_report:
            self.quadratic_i += 1
            print(f"{round(self.quadratic_i / self.quadratic_n * 100, 4)}%")
            self.quadratic_report += int(self.quadratic_n) // 10
        else:
            self.quadratic_i += 1

    def end_quadratic_phase(self, success):
        print(f"End quadratic phase {round(time.time() - self.timer, 8)} sec. {'(success)' if success else '(failed)'}")
        self.timer = time.time()

    def start_cubic_phase(self, n):
        self.timer = time.time()
        self.cubic_i = n
        self.cubic_report = n / 10
        print(f"Start cubic phase with {n} steps")

    def cubic_step(self):
        if time.time() > self.time_limit:
            print("Let's stop here")
            raise Stop()
        self.cubic_i += 1
        if self.cubic_i == self.cubic_report:
            print(f"{round(self.cubic_i / self.cubic_n * 100, 4)}%")
            self.cubic_report += self.cubic_n / 10

    def end_cubic_phase(self):
        print("End cubic phase")

filename = "meshes/mech-holes-shark.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    sys.exit(1)

max_hole_diam = -1.0 if len(sys.argv) < 3 else float(sys.argv[2])
max_num_hole_edges = -1 if len(sys.argv) < 4 else int(sys.argv[3])

nb_holes = 0

border_cycles = Pmp.extract_boundary_cycles(mesh)

for h in border_cycles:
    if max_hole_diam > 0 and max_num_hole_edges > 0 and not is_small_hole(h, mesh, max_hole_diam, max_num_hole_edges):
        continue
    progress = Progress(10.0)
    success = False
    try:
        success = Pmp.triangulate_refine_and_fair_hole(mesh, h, {"visitor": progress.visitor, "use_delaunay_triangulation": True})[0]
    except Stop:
        print("We stopped with a timeout")
    print(f"  Is fairing successful: {1 if success else 0}")
    nb_holes += 1
    del progress.visitor # visitors must be del'd

print()
print(f"{nb_holes} holes have been filled")

Sm.write_polygon_mesh("filled_SM.off", mesh, {"stream_precision": 17})
print("Mesh written to: filled_SM.off")

