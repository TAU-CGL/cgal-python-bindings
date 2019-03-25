from max_cover import *
import time

def breadth_first_Search(arr):
  ubf = arr.unbounded_face()
  ubf.val().set_data(0)
  grays = []
  it = ubf.val().inner_ccbs_begin()
  while it != ubf.val().inner_ccbs_end():
    ccb = it.val()
    curr = it.val()
    while True:
      face = curr.val().twin().val().face()
      if -1 == face.val().data():
        face.val().set_data(1)
        #print(face.val().data())
        grays.append(face)
      curr.inc()
      if ccb == curr: break
    it.inc()

  while len(grays) > 0:
    face = grays.pop(0)
    depth = face.val().data()

    ccb = face.val().outer_ccb()
    curr = ccb.copy()
    cond = True
    while cond:
      new_face = curr.val().twin().val().face()
      if -1 == new_face.val().data():
        if curr.val().data(): diff = -1
        else: diff = 1
        new_face.val().set_data(depth + diff)
        #print("face data", new_face.val().data())
        grays.append(new_face)
      curr.inc()
      cond = curr != ccb

def main(r ,filename):
  print("Executing...")
  t1 = time.time()
  curves = initialize(r, filename)
  kernel = Kernel()
  traits = Traits()
  arr = Arrangement(traits)
  for i in range(len(curves)):
    insert(arr, curves[i])
  t2 = time.time()

  is_equal = traits.equal_2_object()
  it = arr.edges_begin()
  while it != arr.edges_end():
    # face = it.val().face() #not needed?
    trg_curve = it.val().curve().target()
    trg_halfedge = it.val().target().val().point()
    inner = is_equal(trg_curve, trg_halfedge)
    it.val().set_data(inner)
    # print("source: ")
    # pp2(it.val().source().val().point())
    # print("target: ")
    # pp2(it.val().target().val().point())
    # print("edge data:", it.val().data())
    it.val().twin().val().set_data(not inner)
    it.inc()
  it = arr.faces_begin()
  while it != arr.faces_end():
    it.val().set_data(-1)
    it.inc()
  breadth_first_Search(arr)
  it = arr.vertices_begin()
  while it != arr.vertices_end():
    it.val().set_data(0)
    it.inc()
  is_ker_equal = kernel.equal_2_object()
  it = arr.vertices_begin()
  while it != arr.vertices_end():
    if it.val().degree() < 4:
      he = it.val().incident_halfedges()
      if he.val().data(): num = he.val().face().val().data()
      else: num = he.val().twin().val().face().val().data() #line 368
      it.val().set_data(num)
      #print(it.val().data())
      it.inc()
      continue
    curr = it.val().incident_halfedges()
    while not curr.val().data(): curr.inc()
    first = curr.copy()
    center = first.val().curve().supporting_circle().center()
    num = 0
    cond = True
    while cond:
      curr.inc()
      if not curr.val().data():
        num += 1
      cond = not is_ker_equal(center, curr.val().curve().supporting_circle().center())
    #print("num =", num)
    it.val().set_data(first.val().face().val().data() + it.val().degree()//2 - num)
    #print(it.val().data())
    it.inc()

  num_covered = 0
  v = arr.vertices_end()
  cmp = traits.compare_xy_2_object()
  it = arr.vertices_begin()
  while it != arr.vertices_end():
    if it.val().data() > num_covered or (it.val().data() == num_covered and (cmp(it.val().point(), v.val().point()) == -1)):
      num_covered = it.val().data()
      v = it.copy()
    it.inc()
  print("The maximum covering disc is centered at ")
  pp2(v.val().point())
  print("The maximum covering disc covers:", num_covered, "points")
  t3 = time.time()
  print("Arrangement Initialization time:", t2-t1, "\nRemaining Execution time:", t3-t2, "\nTotal execution time:", t3-t1 )

#main(0.5, "points_circle_ap_256.txt")




