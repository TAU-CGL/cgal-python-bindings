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

def locate_point(arr, pl, point):
  obj = pl.locate(point)		# perform the point-location query.
  print_point_location(arr, point, obj)	# print the result.
