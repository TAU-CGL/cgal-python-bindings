def read_objects(module, filename):
  Curve = module.Arrangement_2.Curve_2
  lst = []
  with open(filename, 'r') as f:
    line = next(f)
    n = int(line)
    for i in range(n):
      line = next(f)
      lst.append(Curve(line))
    f.close()
  return lst
