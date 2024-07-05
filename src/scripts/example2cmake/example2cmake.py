text = """
kerEpecInt_aos2AlgPl aos2_alg_epec_release.cmake
kerEpecInt_aos2BezPl aos2_bezier_epec_release.cmake
kerEpecInt_aos2ConicPl aos2_conic_epec_release.cmake
kerEpecInt_aos2SegCdCcdPl aos2_ccd_seg_epec_release.cmake
kerEpecInt_aos2PosCdPl aos2_cd_pos_epec_release.cmake
kerEpecInt_aos2CsPl aos2_cs_epec_release.cmake
kerEpecInt_aos2CsVe aos2_cs_ve_epec_release.cmake
kerEpic_aos2SegPl aos2_epic_release.cmake
kerEpecInt_aos2LinearPl aos2_linear_epec_release.cmake
kerEpic_aos2NcsegPl aos2_ncseg_epic_release.cmake
kerEpecInt_aos2PosPl aos2_pos_epec_release.cmake
kerEpecInt_aos2RatPl aos2_rational_epec_release.cmake
kerEpecInt_aos2SegVeHeFePl aos2_seg_ae_epec_release.cmake
kerEpecInt_aos2SegPl aos2_seg_epec_release.cmake
kerEpic_aos2SegPl aos2_seg_epic_release.cmake
kerEpic_as2_tri2DelNci as2_del_epic_release.cmake
kerEpic_as3Plain_tri3DelFasSeq as3_del_fast_epic_release.cmake
kerCcrInt_aos2Bez_bso2 bso2_bezier_ccr_release.cmake
kerEpecInt_aos2Cs_bso2 bso2_cs_epec_release.cmake
kerEpecInt_aos2Seg_bso2_pol2 bso2_seg_epec_release.cmake
kerdCdlgDynamic cdlg_release.cmake
kerEpecInt_ch2 ch2_epec_release.cmake
kerEpicInt_ch2 ch2_epic_release.cmake
kerEpecInt_aos2LinearCdPl_env2 env2_linear_cd_epec_release.cmake
kerCcrInt_aos2RatCdPl_env2 env2_rat_cd_ccr_release.cmake
kerEpecInt_aos2SegCdPl_env2 env2_seg_cd_epec_release.cmake
kerEpecInt_aos2LinearPl_env3Plane env3_plane_epec_release.cmake
kerEpecInt_aos2LinearPl_env3PlaneSd env3_plane_sd_epec_release.cmake
kerCcrInt_aos2ConicPl_env3Sphere env3_sphere_ccr_release.cmake
kerEpecInt_aos2SegPl_env3TriSd env3_tri_sd_epec_release.cmake
kerdEpecdDynamic epecd_release.cmake
kerEpecInt epec_release.cmake
kerdEpicdDynamic epicd_release.cmake
kerEpic epic_release.cmake
kerCcrInt_aos2Conic_pol2_ms2 ms2_conic_ccr_release.cmake
kerEpecInt_aos2Cs_pol2_ms2 ms2_cs_epec_release.cmake
kerEpecInt_aos2SegPl_bso2_pol2_ms2_pp ms2_seg_epec_release.cmake
kerEpecInt_bso2_pol3Wn_pol2 pol3_bso2_epec_release.cmake
kerEpec_ch3Pol3_pol3Ker pol3_ch3_epec_release.cmake
kerEpic_ch3Pol3_pol3Ker pol3_ch3_epic_release.cmake
kerEpic_sm_pmpSm sm_pmp_epic_release.cmake
kerdCdlgDynamic_ss2 ss_2_clg_release.cmake
kerEpecInt_aos2SegPl_ss2 ss2_seg_epec_release.cmake
kerEpic_tri2CdelNciWiWi tri2_ct_wi_epic_release.cmake
kerEpic_tri2PlainNciWiWiHi tri2_wi_hi_epic_release.cmake
kerEpic_tri3DelComSeq tri3_del_epic_release.cmake
kerEpic_aos2SegPl_vis2 vis2_epic_release.cmake
"""

# simple_join_intersect.py CGALPY_kerEpecInt_aos2Seg_bso2 didnt fit
# sum_by_decomposition.py CGALPY_kerEpecInt_aos2SegPl_bso
examples = """
sequence.py CGALPY_kerEpecInt_aos2Seg_bso2_pol2
approx_offset.py CGALPY_kerEpecInt_aos2Cs_pol2_ms2
exact_offset.py CGALPY_kerCcrInt_aos2Conic_pol2_ms2
approx_inset.py CGALPY_kerEpecInt_aos2Cs_pol2_ms2
batched_point_location.py CGALPY_kerEpecInt_aos2SegPl
conic_multiplicities.py CGALPY_kerEpecInt_aos2ConicPl
conics.py CGALPY_kerEpecInt_aos2ConicPl
dcel_extension.py CGALPY_kerEpecInt_aos2SegVeHeFePl
dcel_extension_io.py CGALPY_kerEpecInt_aos2SegVeHeFePl
face_extension.py CGALPY_kerEpecInt_aos2SegVeHeFePl
face_extension_overlay.py CGALPY_kerEpecInt_aos2SegVeHeFePl
incremental_insertion.py CGALPY_kerEpecInt_aos2SegPl
io_unbounded.py CGALPY_kerEpecInt_aos2LinearPl
overlay_color.py CGALPY_kerEpecInt_aos2SegVeHeFePl
point_location.py CGALPY_kerEpecInt_aos2SegPl
algebraic_segments.py CGALPY_kerEpecInt_aos2AlgPl
algebraic_curves.py CGALPY_kerEpecInt_aos2AlgPl
"""

# delete all CGALPY_ from examples
examples = examples.replace("CGALPY_", "")

# replace the CGALPY_kerEpecInt_aos2AlgPls with the corresponding cmake file names
# create a dictionary from the text
lines = text.split("\n")
lines_split = [line.split() for line in lines]
# delete empty lines
lines_split = [line for line in lines_split if line]
lines_dict = {line[0]: line[1] for line in lines_split}

# replace the examples with the corresponding cmake file names
examples_split = [example.split() for example in examples.split("\n")]
# delete empty lines
examples_split = [example for example in examples_split if example]

examples_split = [[example[0], lines_dict[example[1]]] for example in examples_split]

# combine all again
examples = [" ".join(example) for example in examples_split]
examples = "\n".join(examples)
print(examples)
