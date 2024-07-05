# reverse a dict:
# for keys with multiple values, write them separated by a comma
# sequence.py bso2_seg_epec_release.cmake
# approx_offset.py ms2_cs_epec_release.cmake
# exact_offset.py ms2_conic_ccr_release.cmake
# approx_inset.py ms2_cs_epec_release.cmake
# batched_point_location.py aos2_seg_epec_release.cmake
# conic_multiplicities.py aos2_conic_epec_release.cmake
# conics.py aos2_conic_epec_release.cmake
# dcel_extension.py aos2_seg_ae_epec_release.cmake
# dcel_extension_io.py aos2_seg_ae_epec_release.cmake
# face_extension.py aos2_seg_ae_epec_release.cmake
# face_extension_overlay.py aos2_seg_ae_epec_release.cmake
# incremental_insertion.py aos2_seg_epec_release.cmake
# io_unbounded.py aos2_linear_epec_release.cmake
# overlay_color.py aos2_seg_ae_epec_release.cmake
# point_location.py aos2_seg_epec_release.cmake
# algebraic_segments.py aos2_alg_epec_release.cmake
# algebraic_curves.py aos2_alg_epec_release.cmake

dict_path = "example2cmake.txt"
with open(dict_path, "r") as f:
    text = f.read()

# reverse
lines = text.split("\n")
lines_split = [line.split() for line in lines]
# delete empty lines
lines_split = [line for line in lines_split if line]
lines_dict = {line[0]: line[1] for line in lines_split}
reverse_dict = {}
for key, value in lines_dict.items():
    if value in reverse_dict:
        reverse_dict[value] += "," + key
    else:
        reverse_dict[value] = key
reverse_dict = {key: value for key, value in reverse_dict.items()}

# now write the reverse dict to a file
reverse_dict_path = "cmake2example.txt"
with open(reverse_dict_path, "w") as f:
    for key, value in reverse_dict.items():
        f.write(f"{key} {value}\n")
