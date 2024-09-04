import time
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
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY

class Progress_to_std_cerr_callback:
    def __init__(self, name):
        self.nb = 0
        self.timer = time.perf_counter_ns()
        self.t_start = self.timer
        self.t_latest = self.t_start
        self.name = name

progress = Progress_to_std_cerr_callback("Computing average spacing")

def callback(advancement):
    # Avoid calling time() at every single iteration, which could
    # impact performances very badly
    progress.nb += 1
    if advancement != 1 and progress.nb % 100 != 0:
        return True
    
    t = time.perf_counter_ns()
    if advancement == 1 or (t - progress.t_latest)*1e-9 > 0.1: # Update every 1/10th of second
        sys.stderr.write(f"\r{progress.name}: {int(advancement * 100)}%")
    
        if advancement == 1:
            sys.stderr.write("\n")
        progress.t_latest = t
    
    return True

N = int(sys.argv[i]) if len(sys.argv) > i else 1000
i += 1

# Generate N points on a sphere of radius 100.
points = [CGALPY.Ker.Kernel.Point_3(0, 0, 0) for _ in range(N)] # TODO: use Random_points_on_sphere_3

average_spacing = CGALPY.compute_average_spacing(points, 6, callback)
