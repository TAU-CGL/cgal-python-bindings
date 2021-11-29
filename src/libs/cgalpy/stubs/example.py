import CGALPY

traits = CGALPY.Aos2.Geometry_traits_2()
p = traits.Point_2()
p.x()
equal_object = traits.equal_2_object()
equal_object()

p = CGALPY.Ker.Point_2()
s = CGALPY.Ker.Segment_2(p, p)
