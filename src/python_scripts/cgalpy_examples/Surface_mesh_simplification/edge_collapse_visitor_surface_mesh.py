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
Ker = CGALPY.Ker
Sms = CGALPY.Sms
Sm = CGALPY.Sm


class Stats:
  def __init__(self):
    self.collected = 0
    self.processed = 0
    self.collapsed = 0
    self.non_collapsable = 0
    self.cost_uncomputable = 0
    self.placement_uncomputable = 0


def OnCollected(profile: Sms.Edge_profile, cost) -> None:
    stats.collected += 1
    sys.stderr.write(f"\rEdges collected: {stats.collected}")

def OnSelected(profile: Sms.Edge_profile, cost, initial: int, current: int) -> None:
    stats.processed += 1
    if cost is None:
        stats.cost_uncomputable += 1
    if current == initial:
        sys.stderr.write(f"\n")
    sys.stderr.write(f"\r{current}")

def OnCollapsing(profile: Sms.Edge_profile, placement: float | None) -> None:
    if placement is None:
        stats.placement_uncomputable += 1

def OnNonCollapsable(profile: Sms.Edge_profile) -> None:
    stats.non_collapsable += 1

def OnCollapsed(profile: Sms.Edge_profile, vertex: Sm.SM_vertex_index | None) -> None:
    stats.collapsed += 1


filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/small_cube.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print("Failed to read input mesh: ", filename)
    sys.exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

stats = Stats()
my_visitor = Sms.Edge_collapse_visitor_base()

Sms.set_OnCollected(my_visitor, OnCollected)
Sms.set_OnSelected(my_visitor, OnSelected)
Sms.set_OnCollapsing(my_visitor, OnCollapsing)
Sms.set_OnNonCollapsable(my_visitor, OnNonCollapsable)
Sms.set_OnCollapsed(my_visitor, OnCollapsed)


ratio = float(sys.argv[i]) if len(sys.argv) > i else 0.1
i += 1

stop = Sms.Edge_count_ratio_stop_predicate(ratio)

r = Sms.edge_collapse(surface_mesh, stop, {"visitor": my_visitor})

print(f"\nEdges collected: {stats.collected}",
      f"\nEdges processed: {stats.processed}",
      f"\nEdges collapsed: {stats.collapsed}",
      f"\n",
      f"\nEdges not collapsed due to topological constraints: {stats.non_collapsable}",
      f"\nEdge not collapsed due to cost computation constraints: {stats.cost_uncomputable}",
      f"\nEdge not collapsed due to placement computation constraints: {stats.placement_uncomputable}",
      f"\n")

print(f"Finished!\n{r} edges removed.\n{surface_mesh.number_of_edges()} final edges.\n")

Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh, {"stream_precision": 17})

del my_visitor



