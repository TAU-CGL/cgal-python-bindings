def read_objects(Type, filename):
  lst = []
  with open(filename, 'r') as f:
    line = next(f)
    n = int(line)
    for i in range(n):
      line = next(f)
      lst.append(Type(line))
    f.close()
  return lst
