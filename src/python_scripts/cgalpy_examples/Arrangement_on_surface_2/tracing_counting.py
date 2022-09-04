#!/usr/bin/python3
# export PYTHONPATH=...

import os
import sys
import importlib
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Geom_traits = Arrangement.Geometry_traits_2

#include <CGAL/Arr_tracing_traits_2.h>
#include <CGAL/Arr_counting_traits_2.h>

# typedef CGAL::Arr_tracing_traits_2<Traits>              Tracing_traits;
# typedef CGAL::Arr_counting_traits_2<Tracing_traits>     Geom_traits;
# typedef CGAL::Arrangement_2<Geom_traits>                My_arrangement;

s1 = Segment(Point(0, 0), Point(2, 2))
s2 = Segment(Point(2, 0), Point(0, 2))
segments = [s1, s2]

traits = Geom_traits()
traits.disable_all_traces()
traits.enable_trace(Tracing_traits::INTERSECT_OP)

# Construct an arrangement using aggregated insertion:
arr1 = Arrangement(traits)
Aos2.insert(arr1, segments)
print(traits)
traits.clear_counters()

# Construct an arrangement using incremental insertion:
arr2 = Arrangement(traits)
Aos2.insert(arr2, s1)
Aos2.insert(arr2, s2)
print(traits)
