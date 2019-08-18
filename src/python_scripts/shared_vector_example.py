from shared_vector import *
ev = initd()
print(ev)
first = ev[0]
print(first)

print(first.str())

for element in ev.elements():
  print(element.str())
  element.set_str("foo")

for element in ev.elements():
  print(element.str())

print(first.str())

e = init_element()
ev[1] = e

print(ev[1].str())