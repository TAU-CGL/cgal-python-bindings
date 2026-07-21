# Safety and precondition examples

This file records real Python-side safety/precondition examples collected before changing wrappers. The goal is to discuss the desired exception mapping first, then make the smallest wrapper changes only after mentor agreement.

## Collected cases

### Pol2: invalid `Polygon_2` index access segfaults

Module/build used locally:

- `CGALPY_kerEpecInt_aos2Seg_bso2_pol2`
- build directory: `~/build/cgalpy/full_matrix_arm64_patched_20260530_180116/bso2_seg_epec_release`

Operations tested on a triangle polygon of size 3:

- `poly.vertex(99)`
- `poly[99]`

Observed behavior:

- Both terminate the Python process with segmentation fault.
- Reproduction logs:
  - `~/build/cgalpy/safety_precondition_probes/pol2_vertex_out_of_range_detail/`
  - `~/build/cgalpy/safety_precondition_probes/pol2_getitem_out_of_range_detail/`

Relevant binding location:

- `src/libs/cgalpy/lib/polygon_2_bindings.cpp`
- `Polygon_2.vertex(size_t)`
- `Polygon_2.__getitem__(size_t)`

Question for Efi:

- Should invalid polygon vertex/index access raise Python `IndexError` before calling CGAL?

### Kerd: invalid `Segment_d` index silently returns endpoint 0

Module/build used locally:

- `CGALPY_kerdCdlgDyn_ss2`
- build directory: `~/build/cgalpy/full_matrix_arm64_patched_20260530_180116/ss_2_clg_release`

Operations tested on a 4D segment:

- `s[2]`
- `s.vertex(2)`
- `s.point(2)`

Observed behavior:

- Return code is 0.
- No exception is raised.
- All three invalid accesses return endpoint 0.

Reproduction log:

- `~/build/cgalpy/safety_precondition_probes/kerd_segment_invalid_index_returns/`

Relevant binding location:

- `src/libs/cgalpy/lib/export_segment_d.cpp`
- `Segment_d.__getitem__(i)`
- `Segment_d.vertex(i)`
- `Segment_d.point(i)`

Question for Efi:

- Should invalid segment endpoint indices raise Python `IndexError`?

### Kerd/Trid: dimension-handling behavior needs clarification

Modules/builds used locally:

- `CGALPY_kerdCdlgDyn_ss2`
- `CGALPY_kerEpic_kerdEpicd7_tridDelDef7`

Observed behavior:

- `Kerd.Point_d(4, [0, 1])` constructs `4 0 1 0 0`, apparently padding missing coordinates.
- `Kerd.Segment_d(Point_d(4,...), Point_d(3,...))` constructs a segment with endpoints of different dimensions.
- `Trid.Delaunay_triangulation(7, kernel).insert(Point_d(4,...))` inserts a 4D point into a 7D triangulation without raising.

Reproduction log:

- `~/build/cgalpy/safety_precondition_probes/trid_kerd_dimension_detail/`

Questions for Efi:

- Are these acceptable CGAL behaviors that should be left unchanged?
- Or should Python wrappers enforce dimension consistency and raise `ValueError` for dimension mismatches?

## Current policy before wrapper changes

Do not add broad Python-facing prechecks blindly. For each case:

1. Reproduce current behavior.
2. Record exact module, operation, invalid input, return code, stdout/stderr, and binding location.
3. Ask Efi about desired exception mapping.
4. Patch only the agreed wrappers with the smallest local change.
5. Rebuild the smallest affected target first, then broaden validation.
