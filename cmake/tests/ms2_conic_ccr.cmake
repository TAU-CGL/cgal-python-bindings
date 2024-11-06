set(CGALPY_USE_SHARED_LIBS on cache bool "" force)
set(CGALPY_KERNEL_NAME "cartesianCoreRational" cache string "use kernel" force)
set(CGALPY_KERNEL_INTERSECTION_BINDINGS on cache bool "with intersections" force)
set(CGALPY_POLYGON_2_BINDINGS on cache bool "" force)
# Until the Partitiontraits_2 is fixed, suppress the polygon partition module
set(CGALPY_POLYGON_PARTITIONING_BINDINGS OFF cache BOOL "" force)
set(CGALPY_ARRANGEMENT_ON_SURFACE_2_BINDINGS on cache bool "" force)
set(CGALPY_AOS2_GEOMETRY_TRAITS_NAME "conic" cache string "use conics" force)
set(CGALPY_MINKOWSKI_SUM_2_BINDINGS on cache bool "" force)
