# Pretty-print a polygon
def print_polygon(pgn):
  print('[{}  vertices:'.format(pgn.size()), end='')
  for v in pgn.vertices():
    print(' ({})'.format(v), end='')
  print(' ]')

# Pretty-print a polygon with holes
def print_polygon_with_holes(pwh):
  if not pwh.is_unbounded():
    print('{ Outer boundary = ', end='')
    print_polygon(pwh.outer_boundary())
  else:
    print('{ Unbounded polygon.')

  k = 1;

  print('  {} holes:'.format(pwh.number_of_holes()))
  for h in pwh.holes():
    print('    Hole #{} = '.format(k), end='')
    print_polygon(h)
  print(' }')
