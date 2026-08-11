<!-- Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com> -->

The examples accept an optional argument naming the compiled bindings library to use. If no argument is provided, they use `CGALPY` by default.

## Curated C++/Python example-comparison notes

The GitHub smoke workflow contains curated deterministic C++/Python example comparisons. Some packages are intentionally not included in stdout comparison yet:

- `As2`: deferred after `find_optimal_alpha` / iterator-conversion issues; mentor guidance was to move on for now.
- `Bv`: interactive viewer behavior remains outside deterministic stdout comparison.
- `Bvr`: intentionally outside stdout comparison; representative visual behavior is validated through deterministic `Graphics_scene` buffer rasterization described below.
- `Nef3`: skipped for now because the currently exposed Python API is too limited for a faithful direct C++ example translation.
- `Smsi`: skipped for now because `edge_collapse`/named-parameter support is not ready for the current bindings.
- `Sd`: TODO; avoid RANSAC-based stdout comparisons for now because Efficient RANSAC output was observed to be nondeterministic.
- `Pp2`: skipped for now because no usable Point_set_processing_2 config/example pair was found in the current checked tree.

When adding a new pair to `compare_examples.py`, use the comparison mode that matches the observable output. Keep deterministic stdout examples on exact stdout comparison. For visual examples, prefer direct scene/frame buffers or deterministic saved rasters over operating-system screenshots.

## Deterministic visual/raster comparison

`compare_examples.py` supports raster pairs in addition to the existing exact-stdout pairs. Raster producers write binary P6 PPM files into pair-local C++ and Python output directories. The comparator records a machine-readable `comparison.json` and, when the image shapes match, writes diagnostic RGB views, a difference heatmap, and a side-by-side image under `raster_diagnostics/`. The heatmap uses the same transparent-RGB normalization policy as the comparison, and alpha-channel differences remain visible in RGBA diagnostics.

The current representative visual pairs are:

- `aos2_visual_buffer_raster`: builds an `Arrangement_2`, asks `Bvr.add_to_graphics_scene()` / `CGAL::add_to_graphics_scene()` to populate the graphics scene, and rasterizes `POS_SEGMENTS`.
- `pol3_visual_buffer_raster`: builds a tetrahedral `Polyhedron_3`, populates a Basic Viewer graphics scene, projects the scene's `POS_SEGMENTS` through a fixed deterministic 3D-to-2D transform, and rasterizes those segments.

Both pairs deliberately compare scene-buffer geometry instead of taking a desktop/window screenshot. Their rendering controls are fixed in the producers: image dimensions, viewport/projection, margins, background, foreground color, and integer line rasterization. They do not depend on fonts, antialiasing, animation timing, window-manager state, OpenGL framebuffer readback, or random default face colors.

These representative pairs currently use the strictest policy:

- absolute per-channel tolerance: `0`
- allowed fraction of pixels with any channel beyond tolerance: `0.0`
- required result: byte-identical raster values

`bad_fraction` is pixel-based: a pixel is outside tolerance when any of its channels exceeds the configured absolute threshold. Per-channel counts are still recorded separately in the JSON diagnostics. Do not relax those thresholds merely to make a regression pass. If a future producer has documented renderer-dependent nondeterminism, first make capture deterministic where possible, then justify the smallest necessary metric/tolerance policy in the pair definition and tests.

The Linux smoke workflow runs `aos2_visual_buffer_raster` in the existing `aos2_seg_bvr_epec` visual configuration and uploads its comparison JSON, raw PPM outputs, diff heatmap, side-by-side image, and producer logs as a short-lived CI artifact. `pol3_visual_buffer_raster` provides additional cross-package local coverage without adding a second expensive Basic Viewer matrix job.

A local raster comparison uses the same runner as stdout pairs. For example:

```bash
python src/python_examples/cgalpy_examples/compare_examples.py \
  --pair aos2_visual_buffer_raster \
  --cgal-source "$HOME/trees/cgal" \
  --cgal-dir /path/to/current/cgal-build \
  --python-build /path/to/compatible/cgalpy-build \
  --library <compiled-binding-module> \
  --python-executable "$(command -v python)" \
  --work-dir "$HOME/build/cgalpy/example_compare_curated" \
  --build-type Release
```

Use a detached CGAL build matching the checked-out CGAL source and a binding build whose configuration actually enables the data structure and Basic Viewer bindings required by the selected pair.


## Stdout comparison coverage status

The curated stdout comparison set accounts for all 32 binding modules currently tracked in the GSoC 2026 example-validation pass.

Covered by deterministic C++/Python stdout comparisons:

- `Aos2`, `Pol2`, `Bso2`, `Ms2`, `Ss2`, `Env2`, `Env3`, `Vis2`
- `Ker`, `Sm`, `Pol3`, `Pmp`, `Tri2`, `Tri3`, `Trid`, `Kerd`
- `As3`, `Bgl`, `Ch2`, `Ch3`, `Smsk`, `Ss`, `St`, `Sn2`, `Gog`

Intentionally deferred from stdout comparison:

- `As2`: deferred after `find_optimal_alpha` / iterator-conversion issues; mentor guidance was to move on.
- `Bv`: interactive viewer behavior remains outside deterministic stdout comparison.
- `Bvr`: kept outside stdout comparison; representative `Graphics_scene` output is covered by the deterministic raster pairs above.
- `Nef3`: current Python API is too limited for a faithful direct C++ example translation.
- `Smsi`: `edge_collapse` / named-parameter support is not ready for the current bindings.
- `Sd`: avoid RANSAC-based stdout checks because Efficient RANSAC output was observed to be nondeterministic.
- `Pp2`: no usable Point_set_processing_2 config/example pair was found in the current checked tree.

Audit result: no unaccounted modules remain for the stdout-comparison task.
