import time
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
Psp = CGALPY.Psp

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

# Generate N deterministic points on a sphere of radius 100.
# This avoids a degenerate all-points-at-origin input and keeps the example
# reproducible without relying on a random point generator binding.
radius = 100.0
golden_angle = math.pi * (3.0 - math.sqrt(5.0))
points = []
for j in range(max(N, 1)):
  z = 1.0 - 2.0 * (j + 0.5) / max(N, 1)
  r = math.sqrt(max(0.0, 1.0 - z * z))
  theta = golden_angle * j
  points.append(
    CGALPY.Ker.Kernel.Point_3(radius * r * math.cos(theta),
                              radius * r * math.sin(theta),
                              radius * z))

# The current Python binding exposes compute_average_spacing() with the
# standard point range and NumPy array inputs. The CGAL callback named
# parameter is not exposed here yet, so keep this example on the supported path.
average_spacing = Psp.compute_average_spacing(points, 6)
print(f"Average spacing: {average_spacing}")
