text = """
aos2_alg_epec'       'kerEpecInt_aos2AlgPl')";                  
aos2_bezier_epec'    'kerEpecInt_aos2BezPl')";                  
aos2_conic_epec'     'kerEpecInt_aos2ConicPl')";                
aos2_ccd_seg_epec'   'kerEpecInt_aos2SegCdCcdPl')";             
aos2_cd_pos_epec'    'kerEpecInt_aos2PosCdPl')";                
aos2_cs_epec'        'kerEpecInt_aos2CsPl')";                   
aos2_cs_ve_epec'     'kerEpecInt_aos2CsVe')";                   
aos2_epic'           'kerEpic_aos2SegPl')";                     
aos2_linear_epec'    'kerEpecInt_aos2LinearPl')";               
aos2_ncseg_epic'     'kerEpic_aos2NcsegPl')";                   
aos2_pos_epec'       'kerEpecInt_aos2PosPl')";                  
aos2_rational_epec'  'kerEpecInt_aos2RatPl')";                  
aos2_seg_ae_epec'    'kerEpecInt_aos2SegVeHeFePl')";            
aos2_seg_epec'       'kerEpecInt_aos2SegPl')";                  
aos2_seg_epic'       'kerEpic_aos2SegPl')";                     
as2_del_epic'        'kerEpic_as2_tri2DelNci')";                
as3_del_fast_epic'   'kerEpic_as3Plain_tri3DelFasSeq')";        
bso2_bezier_ccr'     'kerCcrInt_aos2Bez_bso2')";                
bso2_cs_epec'        'kerEpecInt_aos2Cs_bso2')";                
bso2_seg_epec'       'kerEpecInt_aos2Seg_bso2_pol2')";          
cdlg'                'kerdCdlgDynamic')";                       
ch2_epec'            'kerEpecInt_ch2')";                        
ch2_epic'            'kerEpicInt_ch2')";                        
env2_linear_cd_epec' 'kerEpecInt_aos2LinearCdPl_env2')";        
env2_rat_cd_ccr'     'kerCcrInt_aos2RatCdPl_env2')";            
env2_seg_cd_epec'    'kerEpecInt_aos2SegCdPl_env2')";           
env3_plane_epec'     'kerEpecInt_aos2LinearPl_env3Plane')";     
env3_plane_sd_epec'  'kerEpecInt_aos2LinearPl_env3PlaneSd')";   
env3_sphere_ccr'     'kerCcrInt_aos2ConicPl_env3Sphere')";      
env3_tri_sd_epec'    'kerEpecInt_aos2SegPl_env3TriSd')";        
epecd'               'kerdEpecdDynamic')";                      
epec'                'kerEpecInt')";                            
epicd'               'kerdEpicdDynamic')";                      
epic'                'kerEpic')";                               
ms2_conic_ccr'       'kerCcrInt_aos2Conic_pol2_ms2')";          
ms2_cs_epec'         'kerEpecInt_aos2Cs_pol2_ms2')";            
ms2_seg_epec'        'kerEpecInt_aos2SegPl_bso2_pol2_ms2_pp')"; 
pol3_bso2_epec'      'kerEpecInt_bso2_pol3Wn_pol2')";           
pol3_ch3_epec'       'kerEpec_ch3Pol3_pol3Ker')";               
pol3_ch3_epic'       'kerEpic_ch3Pol3_pol3Ker')";               
sm_pmp_epic'         'kerEpic_sm_pmpSm')";                      
ss_2_clg'            'kerdCdlgDynamic_ss2')";                   
ss2_seg_epec'        'kerEpecInt_aos2SegPl_ss2')";              
tri2_ct_wi_epic'     'kerEpic_tri2CdelNciWiWi')";               
tri2_wi_hi_epic'     'kerEpic_tri2PlainNciWiWiHi')";            
tri3_del_epic'       'kerEpic_tri3DelComSeq')";                 
vis2_epic'           'kerEpic_aos2SegPl_vis2')";                

"""

# delete 's, )s, ;s and ;s form the text
text = text.replace("'", "")
text = text.replace(")", "")
text = text.replace(";", "")
text = text.replace('"', "")

# split the text into lines, and add _release to the end of the first word, combine all again and print
lines = text.split("\n")
lines_split = [line.split() for line in lines]
# delete empty lines
lines_split = [line for line in lines_split if line]

# add _release to the end of the first word
# lines_split = [[line[0] + "_release.cmake", line[1]] for line in lines_split]
# reverse the order of the words
lines_split = [[line[1], line[0] + "_release.cmake"] for line in lines_split]

# combine all again
lines = [" ".join(line) for line in lines_split]
lines = "\n".join(lines)
print(lines)

