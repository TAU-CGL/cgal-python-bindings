The examples accept as an argument the name of the compiled bindings library to be used.
Several precompiled libraries are provided for Windows 10 64 bit, Python 3.9, under "src/python_scripts/precompiled". If no argument is provided, these libraries will be used by default in all examples.

## Curated C++/Python example-comparison notes

The GitHub smoke workflow contains curated deterministic C++/Python example comparisons. Some packages are intentionally not included in stdout comparison yet:

- `As2`: deferred after `find_optimal_alpha` / iterator-conversion issues; mentor guidance was to move on for now.
- `Bv` and `Bvr`: visual/Qt/OpenGL-oriented modules; defer until visual CI uses offscreen rendering and pixel-buffer comparison.
- `Nef3`: skipped for now because the currently exposed Python API is too limited for a faithful direct C++ example translation.
- `Sms`: skipped for now because `edge_collapse`/named-parameter support is not ready for the current bindings.
- `Sd`: TODO; avoid RANSAC-based stdout comparisons for now because Efficient RANSAC output was observed to be nondeterministic.
- `Pp2`: skipped for now because no usable Point_set_processing_2 config/example pair was found in the current checked tree.

When adding a new pair to `compare_examples.py`, prefer deterministic stdout examples. For visual examples, use the future offscreen-rendering plus pixel-buffer comparison path instead of stdout comparison.

## Stdout comparison coverage status

The curated stdout comparison set accounts for all 32 binding modules currently tracked in the GSoC 2026 example-validation pass.

Covered by deterministic C++/Python stdout comparisons:

- `Aos2`, `Pol2`, `Bso2`, `Ms2`, `Ss2`, `Env2`, `Env3`, `Vis2`
- `Ker`, `Sm`, `Pol3`, `Pmp`, `Tri2`, `Tri3`, `Trid`, `Kerd`
- `As3`, `Bgl`, `Ch2`, `Ch3`, `Smsk`, `Ss`, `St`, `Sn2`, `Gog`

Intentionally deferred from stdout comparison:

- `As2`: deferred after `find_optimal_alpha` / iterator-conversion issues; mentor guidance was to move on.
- `Bv` and `Bvr`: visual/Qt/OpenGL-oriented modules; validate with visual CI/offscreen pixel-buffer comparison instead of stdout.
- `Nef3`: current Python API is too limited for a faithful direct C++ example translation.
- `Sms`: `edge_collapse` / named-parameter support is not ready for the current bindings.
- `Sd`: avoid RANSAC-based stdout checks because Efficient RANSAC output was observed to be nondeterministic.
- `Pp2`: no usable Point_set_processing_2 config/example pair was found in the current checked tree.

Audit result: no unaccounted modules remain for the stdout-comparison task.
