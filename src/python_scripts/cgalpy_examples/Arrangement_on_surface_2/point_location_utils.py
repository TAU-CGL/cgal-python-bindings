def print_point_location(arr, point, obj):
  print('The point {} is located '.format(point), end = '')

  if type(obj) == arr.Vertex:
    if obj.is_isolated():
      print('on an isolated vertex: ', obj.point())
      return
    print('on a vertex: ', obj.point())
    return

  if type(obj) == arr.Halfedge:
    print('on an edge:', obj.curve())
    return

  if obj.is_unbounded():
    print('inside the unbounded face')
    return

  print('inside a bounded face')
  return

def locate_point(arr, pl, q):
  obj = pl.locate(q)			# perform the point-location query.
  print_point_location(arr, q, obj)	# print the result.

#  Perform a vertical ray-shooting query and print the result.
def shoot_vertical_ray(arr, vrs, q):
  # Perform the point-location query.
  obj = vrs.ray_shoot_up(q)

  # Print the result.
  print('Shooting up from ({}) : hit '.format(q), end='')
  if type(obj) == arr.Vertex:
    if obj.is_isolated():
      print('an isolated vertex: {}'.format(obj.point()))
    else: print('a vertex: {}'.format(obj.point()))
  elif type(obj) == arr.Halfedge:
    print('an edge: {}'.format(obj.curve()))
  elif type(obj) == arr.Face:
    assert(obj.is_unbounded())
    print("nothing.")
  else: raise Exception("Invalid object.")
