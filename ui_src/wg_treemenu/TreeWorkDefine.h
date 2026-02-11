/////////////////////////////////////////////////////////////////////////////
// CTreeWorkCtrl window
/////////////////////////////////////////////////////////////////////////////
// Work Tree의 순서는 여기에 나타난 순서대로 나옴 
// 주의 : 각 범위안의 Index를 사용하여 define 해줄 것

#define WORK_ROOT         10

//---------------------------------------------------------------------------
// Analysis Control (100-129)
//---------------------------------------------------------------------------
#define WORK_ANAL_FLD     100
#define WORK_PDEL_FLD     101
#define WORK_BUCK_FLD     102
#define WORK_EIGV_FLD     103
#define WORK_SPCT_FLD     104
#define WORK_HHCT_FLD     105
#define WORK_MVCT_FLD     106
#define WORK_SMCT_FLD     107
#define WORK_NLCT_FLD     108
#define WORK_NLCT_LST     109
#define WORK_STCT_FLD     110
#define WORK_SBCT_FLD     111
#define WORK_POCT_FLD     112
#define WORK_BCCT_FLD     113
#define WORK_BCCT_LST     114
#define WORK_ILDS_FLD     115
#define WORK_IFGS_FLD     116
#define WORK_EQMF_FLD     117
#define WORK_ISDS_FLD     118
#define WORK_IFCT_FLD     119
#define WORK_INMF_FLD     120
#define WORK_AETL_FLD     121

//---------------------------------------------------------------------------
// Structure (130-149)
//---------------------------------------------------------------------------
#define WORK_STRU_FLD     130
#define WORK_NODE_FLD     131
#define WORK_ELEM_FLD     132
#define	WORK_TRUSS_LST    133
#define	WORK_TRTENS_LST   134
#define	WORK_TRCOMP_LST   135
#define	WORK_BEAM_LST     136
#define	WORK_PLATE_LST    137
#define	WORK_PLSTRS_LST   138
#define	WORK_PLSTRN_LST   139
#define	WORK_AXISYM_LST   140
#define	WORK_SOLID_LST    141
#define	WORK_WALL_LST     142
#define	WORK_WALLOPEN_LST 143
#define	WORK_CRBWALL_LST  144
#define	WORK_SPAN_FLD     145
#define	WORK_SPAN_LST     146
#define	WORK_SPAN_COMPOSTIE_FLD     147
#define	WORK_SPAN_COMPOSTIE_LST     148

//---------------------------------------------------------------------------
// Dimension (150)
//---------------------------------------------------------------------------
#define WORK_DIMM_FLD     150
#define WORK_DIMM_LST     151

//---------------------------------------------------------------------------
// Domain (155)
//---------------------------------------------------------------------------
#define WORK_DOMN_FLD     155
#define WORK_MADO_FLD     156
#define WORK_MADO_LST     157
#define WORK_SBDO_FLD     158
#define WORK_SBDO_LST     159

//---------------------------------------------------------------------------
// Group (160-169)
//---------------------------------------------------------------------------
#define WORK_GRUP_FLD     160
#define WORK_GRUP_LST     161

//---------------------------------------------------------------------------
// Named Plane (170-179)
//---------------------------------------------------------------------------
#define WORK_NPLN_FLD     170
#define WORK_NPLN_LST     171

//---------------------------------------------------------------------------
// Property (180-249)
//---------------------------------------------------------------------------
#define WORK_PROP_FLD     180
#define WORK_MATL_FLD     181
#define WORK_MATL_LST     182
#define WORK_TDMT_FLD     183 // Time Dependent Material
#define WORK_TDMT_LST     184
#define WORK_TDMF_FLD     185
#define WORK_TDMF_LST     186
#define WORK_TMAT_FLD     187
#define WORK_TMAT_LST     188
#define WORK_TDME_FLD     189
#define WORK_TDME_LST     190
#define WORK_EDMP_FLD     191
#define WORK_EDMP_LST     192
#define WORK_EPMT_FLD     193 // Plastic Material 
#define WORK_EPMT_LST     194
#define WORK_SECT_FLD     195
#define WORK_SECT_LST     196
#define WORK_SECF_FLD     197
#define WORK_SECF_LST     198
#define WORK_RPSC_FLD     199
#define WORK_RPSC_LST     200
#define WORK_TSGR_FLD     201
#define WORK_TSGR_LST     202
#define WORK_CSCS_FLD     203
#define WORK_CSCS_LST     204
#define WORK_THIK_FLD     205
#define WORK_THIK_LST     206
													
#define WORK_MPHI_FLD     207
#define WORK_MPGB_FLD     208  
#define WORK_MPGB_LST     209
#define WORK_MPHG_FLD     210
#define WORK_MPHG_LST     211
#define WORK_MPST_FLD     212
#define WORK_MPST_LST     213

#define WORK_WSSF_FLD     214
#define WORK_WSSF_LST     215
#define WORK_IEHP_FLD     216
#define WORK_IEHP_LST     217
#define WORK_IEHG_FLD     218
#define WORK_IEHG_LST     219
#define WORK_GRDP_FLD     220
#define WORK_GRDP_LST     221
#define WORK_FIMP_FLD     222
#define WORK_FIMP_LST     223
#define WORK_FIBR_FLD     224
#define WORK_FIBR_LST     225
#define WORK_PSSF_FLD     226
#define WORK_PSSF_LST     227
#define WORK_PREB_FLD     228
#define WORK_PREB_LST     229
#define WORK_PRCS_FLD     230
#define WORK_PRCS_LST     231
#define WORK_DPAN_FLD     232
#define WORK_DPAN_LST     233
#define WORK_CCPT_FLD     234
#define WORK_CCPT_LST     235
#define WORK_INEL_PROP_FLD 236
#define WORK_IEHC_FLD     237
#define WORK_FIBW_FLD     238
#define WORK_FIBW_LST     239
#define WORK_MSIS_FLD     240
#define WORK_MSIS_LST     241
#define	WORK_EFWD_FLD     242
#define	WORK_EFWD_LST     243
#define WORK_MHTP_FLD     244
#define WORK_MHTP_LST     245
#define WORK_MHAT_FLD     246
#define WORK_MHAT_LST     247
#define WORK_SIHO_FLD     248
#define WORK_SIHG_FLD     249

//---------------------------------------------------------------------------
// Boundary (250-349)
//---------------------------------------------------------------------------
#define WORK_BNDR_FLD     250    
#define WORK_CONS_FLD     251
#define WORK_CONS_LST     252
#define WORK_NSPR_FLD     253
#define WORK_NSPR_LIN_FLD 254
#define WORK_NSPR_LIN_LST 255
#define WORK_NSPR_NON_FLD 256
#define WORK_NSPR_NON_LST 257
#define WORK_NSPR_MLT_FLD 258
#define WORK_NSPR_MLT_LST 259
#define WORK_GSTP_FLD     260
#define WORK_GSTP_LST     261
#define WORK_ELNK_FLD     262
#define WORK_ELNK_LST     263
#define WORK_NLLP_FLD     264 // General Link 
#define WORK_NLLP_LST     265
#define WORK_ASGB_FLD     266
#define WORK_ASGB_LST     267
#define WORK_NLNK_FLD     268
#define WORK_NLNK_LST     269
#define WORK_FRLS_FLD     270
#define WORK_FRLS_LST     271
#define WORK_OFFS_FLD     272
#define WORK_OFFS_LST     273
#define WORK_PRLS_FLD     274
#define WORK_PRLS_LST     275
#define WORK_RIGD_FLD     276
#define WORK_RIGD_LST     277
#define WORK_DRLS_FLD     278
#define WORK_PZEF_FLD     279
#define WORK_SKEW_FLD     280
#define WORK_SKEW_LST     281
#define WORK_EWSF_FLD     282
#define WORK_EWSF_LST     283
#define WORK_CGLP_FLD     284
#define WORK_CGLP_LST     285
#define WORK_MCON_FLD     286 // MNET:2790-JHLEE-20070911
#define WORK_MCON_LST     287
#define WORK_SSPS_FLD     288
#define WORK_SSPS_LST     289
#define WORK_SSPS_LIN_FLD 290
#define WORK_SSPS_LIN_LST 291
#define WORK_SSPS_NON_FLD 292
#define WORK_SSPS_NON_LST 293
#define WORK_CLDR_FLD     294
#define WORK_CLDR_LST     295
#define WORK_SSDV_FLD     296
#define WORK_SDVI_FLD     297
#define WORK_SDVI_LST     298
#define WORK_SDVE_FLD     299
#define WORK_SDVE_LST     300
#define WORK_SDST_FLD     301
#define WORK_SDST_LST     302
#define WORK_SDHY_FLD     303
#define WORK_SDHY_LST     304
#define WORK_SDIS_FLD     305
#define WORK_SDIS_LST     306
#define WORK_MLFC_FLD     307
#define WORK_MLFC_LST     308
#define WORK_RLFC_FLD     309
#define WORK_RLFC_LST     310
#define WORK_DMGB_FLD     311
#define WORK_DMGB_LST     312
#define WORK_RFST_FLD     313
#define WORK_RFST_LST     314


//---------------------------------------------------------------------------
// Mass (350-369)
//---------------------------------------------------------------------------
#define WORK_MASS_FLD     350
#define WORK_NMAS_FLD     351
#define WORK_NMAS_LST     352
#define WORK_DMAS_FLD     353
#define WORK_DMAS_LST     354
#define WORK_LTOM_FLD     355
#define WORK_LTOM_LST     356

//---------------------------------------------------------------------------
// Static Load (370-449)
//---------------------------------------------------------------------------
#define WORK_STLD_FLD     370
#define WORK_STLD_LST     371
#define WORK_NBOF_FLD     372
#define WORK_NBOF_LST     373
#define WORK_BODF_FLD     374
#define WORK_BODF_LST     375
#define WORK_CNLD_FLD     376
#define WORK_CNLD_LST     377
#define WORK_SDSP_FLD     378
#define WORK_SDSP_LST     379
#define WORK_BMLD_FLD     380
#define WORK_BMLD_LST     381
#define WORK_BMLD_ELEM_FLD  382   
#define WORK_BMLD_ELEM_LST  383
#define WORK_BMLD_FLOR_FLD  384
#define WORK_BMLD_FLOR_LST  385
#define WORK_BMLD_LINE_FLD  386
#define WORK_BMLD_LINE_LST  387
#define WORK_BMLD_TYPC_FLD  388
#define WORK_BMLD_TYPC_LST  389
#define WORK_FBLA_FLD     390
#define WORK_FBLA_LST     391
#define WORK_PNLA_FLD     392 // Plane Load
#define WORK_PNLA_LST     393
#define WORK_FMLD_FLD     394
#define WORK_PRST_FLD     395
#define WORK_PRST_LST     396
#define WORK_PTNS_FLD     397
#define WORK_PTNS_LST     398
#define WORK_EXLD_FLD     399
#define WORK_EXLD_LST     400
#define WORK_TDPL_FLD     401
#define WORK_TDPL_LST     402
#define WORK_PRES_FLD     403
#define WORK_PRES_LST     404
#define WORK_STMP_FLD     405
#define WORK_STMP_LST     406
#define WORK_NTMP_FLD     407
#define WORK_NTMP_LST     408
#define WORK_ETMP_FLD     409
#define WORK_GTMP_FLD     410
#define WORK_GTMP_LST     411
#define WORK_BTMP_FLD     412 
#define WORK_WIND_FLD     413
#define WORK_WIND_LST     414
#define WORK_SEIS_FLD     415
#define WORK_SEIS_LST     416
#define WORK_SSEI_FLD     417
#define WORK_SSEI_LST     418
#define WORK_ETCLD_FLD    419
#define WORK_TMLD_FLD     420
#define WORK_CRPC_FLD     421
#define WORK_WDPR_AREA_FLD  422
#define WORK_WDPR_BEAM_FLD  423
#define WORK_WDPR_NODAL_FLD 424
#define WORK_WDPR_FUNC_FLD  425
#define WORK_CMCS_FLD     426
#define WORK_WVEP_FLD     427
#define WORK_WVEP_LST     428
#define WORK_WDFC_FLD     429
#define WORK_WDFC_LST     430
#define WORK_WIND_WA_WT_FLD 431
#define WORK_ARPR_FLD     432
#define WORK_ARPR_LST     433
#define WORK_STBK_FLD     434
#define WORK_STBK_LST     435
#define WORK_ULCB_FLD     436
#define WORK_ULCB_LST     437
#define WORK_POSL_FLD     438
#define WORK_POSL_LST     439
#define WORK_POSP_FLD     440
#define WORK_POSP_LST     441
#define WORK_EPST_FLD     442
#define WORK_EPSE_FLD     443
#define WORK_STLD_TYPE_FLD	444
#define WORK_CNLD_TYPE_FLD	445
#define WORK_CNLD_TYPE_LST	446
#define WORK_CNLD_SUB_LST	447

//---------------------------------------------------------------------------
// Earth Pressure Parameter (448)
//---------------------------------------------------------------------------
#define WORK_ERPR_FLD 448

//Plate Thickness Temperature
#define WORK_PTMP_FLD     449

//---------------------------------------------------------------------------
// Response Spectrum (450-469)
//---------------------------------------------------------------------------
#define WORK_SPEC_FLD     450
#define WORK_SPEC_LST     451
#define WORK_SPFC_FLD     452
#define WORK_SPFC_LST     453
#define WORK_SPLC_FLD     454
#define WORK_SPLC_LST     455
#define WORK_SPFC_ND_FLD  456
#define WORK_SPFC_ND_LST  457

//---------------------------------------------------------------------------
// Tendon (470-499)
//---------------------------------------------------------------------------
#define WORK_PSTD_FLD     470 // Prestressing Tendon
#define WORK_TDNT_FLD     471
#define WORK_TDNT_LST     472
#define WORK_TDNA_FLD     473
#define WORK_TDNA_LST     474
#define WORK_TDCS_FLD     475
#define WORK_TDCS_LST     476
#define WORK_TDBM_FLD     477 //PT beam
#define WORK_TDBM_LST     478
// #define WORK_TDSL_FLD     479 //PT slab
// #define WORK_TDSL_LST     480
#define WORK_PTTN_FLD     479 //PT slab
#define WORK_PTTN_LST     480

//---------------------------------------------------------------------------
// Time History Load (500-549)
//---------------------------------------------------------------------------
#define WORK_THISANL_FLD  500
#define WORK_THISANL_LST  501
#define WORK_THIS_FLD     502
#define WORK_THIS_LST     503
#define WORK_THFC_FLD     504
#define WORK_THFC_LST     505
#define WORK_THGA_FLD     506
#define WORK_THGA_LST     507
#define WORK_THNL_FLD     508
#define WORK_THNL_LST     509
#define WORK_THSL_FLD     510
#define WORK_THSL_LST     511
#define WORK_THMS_FLD     512
#define WORK_THMS_LST     513
#define WORK_THRD_FLD     514
#define WORK_THRD_LST     515
#define WORK_THAT_FLD     516
#define WORK_THAT_LST     517
#define WORK_THRG_FLD     518
#define WORK_THRG_LST     519
#define WORK_THRI_FLD     520
#define WORK_THRI_LST     521
#define WORK_THRS_FLD     522
#define WORK_THRS_LST     523
#define WORK_THGC_FLD     524
#define WORK_THGC_LST     525
#define WORK_THSG_FLD     526
#define WORK_THEF_FLD     527
#define WORK_THEF_LST     528
#define WORK_THSF_FLD     529
#define WORK_THSF_LST     530
#define WORK_THCR_FLD     531
#define WORK_THCR_LST     532

//---------------------------------------------------------------------------
// Moving Load (550-599)
//---------------------------------------------------------------------------
#define WORK_MVLDANL_FLD  550
#define WORK_MVLDANL_LST  551
#define WORK_MVCD_FLD     552
#define WORK_LLAN_FLD     553
#define WORK_LLAN_LST     554
#define WORK_SLAN_FLD     555
#define WORK_SLAN_LST     556
#define WORK_SINF_FLD     557
#define WORK_SINF_LST     558
#define WORK_MLSP_FLD     559
#define WORK_MLSR_FLD     560
#define WORK_MVHL_FLD     561
#define WORK_MVHL_LST     562
#define WORK_MVHC_FLD     563
#define WORK_MVHC_LST     564
#define WORK_MVLD_FLD     565
#define WORK_MVLD_LST     566
#define WORK_IMPF_FLD     567  // Additional Impact Factor
#define WORK_IMPF_LST     568
#define WORK_DYNF_FLD     569 
#define WORK_DYNF_LST     570
#define WORK_DYFG_FLD     571 
#define WORK_DYFG_LST     572
#define WORK_DYLA_FLD     573 
#define WORK_DYLA_LST     574
#define WORK_LLANop_FLD   575
#define WORK_LLANop_LST   576
#define WORK_SLANop_FLD   577
#define WORK_SLANop_LST   578
#define WORK_MLTT_FLD     579

//---------------------------------------------------------------------------
// Moving Load Japan (580-619)
//---------------------------------------------------------------------------
#define WORK_MVLDANLjp_FLD  580
#define WORK_LLANjp_FLD     581
#define WORK_LLANjp_LST     582
#define WORK_MVLDjp_FLD     583
#define WORK_MVLDjp_LST     584
#define WORK_JIMP_FLD       585
#define WORK_JIMP_LST       586

//---------------------------------------------------------------------------
// Moving Load China (580-619)  - vehicle & vehicle class는 국내버전과 같이 사용 
//---------------------------------------------------------------------------
#define WORK_MVLDANLch_FLD  590
#define WORK_LLANch_FLD     591
#define WORK_LLANch_LST     592
#define WORK_SLANch_FLD     593
#define WORK_SLANch_LST     594
#define WORK_SINFch_FLD     595
#define WORK_MVLDch_FLD     596
#define WORK_MVLDch_LST     597

//---------------------------------------------------------------------------
// Moving Load India (600-610)  - vehicle은  국내버전과 같이 사용 
//---------------------------------------------------------------------------
#define WORK_MVLDANLid_FLD  600
#define WORK_LLANid_FLD     601
#define WORK_LLANid_LST     602
#define WORK_MVLDid_FLD     603
#define WORK_MVLDid_LST     604

//---------------------------------------------------------------------------
// Moving Load India (610-619)  - 나머지는 국내버전과 같이 사용
//---------------------------------------------------------------------------
#define WORK_MVLDbs_FLD     610
#define WORK_MVLDbs_LST     611

//---------------------------------------------------------------------------
// Settlement (620-649)
//---------------------------------------------------------------------------
#define WORK_STTL_FLD     620
#define WORK_STTL_LST     621
#define WORK_SMPT_FLD     622
#define WORK_SMPT_LST     623
#define WORK_SMLC_FLD     624
#define WORK_SMLC_LST     625

//---------------------------------------------------------------------------
// transverse moving load(600-610)
//---------------------------------------------------------------------------
#define WORK_MVLDANLtr_FLD  631
#define WORK_LLANtr_FLD     632
#define WORK_LLANtr_LST     633
#define WORK_MVHLtr_FLD     634
#define WORK_MVHLtr_LST     635
#define WORK_MVLDtr_FLD     636
#define WORK_MVLDtr_LST     637

//---------------------------------------------------------------------------
// crane load(640-649)
//---------------------------------------------------------------------------
#define WORK_CRANELOAD_FLD  640
#define WORK_CRAN_FLD		641
#define WORK_CRAN_LST		642
#define WORK_NCRN_FLD		643
#define WORK_NCRN_LST		644

//---------------------------------------------------------------------------
// Erection Sequence - 기능삭제 (650-659)
//---------------------------------------------------------------------------
#define WORK_ESEQ_FLD     650
#define WORK_ESEQ_LST     651

//---------------------------------------------------------------------------
// Composite Bridge (660-669)
//---------------------------------------------------------------------------
#define WORK_PLCB_FLD     660
#define WORK_PLCB_LST     661

//---------------------------------------------------------------------------
// Hydration (700-749)
//---------------------------------------------------------------------------
#define WORK_HYDR_FLD     700 // Hydration
//#define WORK_HHCT_FLD     701
//#define WORK_HHCT_LST     702
#define WORK_HMTP_FLD     703
#define WORK_HMTP_LST     704
#define WORK_HMAT_FLD     705
#define WORK_HMAT_LST     706
#define WORK_ETFC_FLD     707
#define WORK_ETFC_LST     708
#define WORK_CCFC_FLD     709
#define WORK_CCFC_LST     710
#define WORK_HECB_FLD     711
#define WORK_HECB_LST     712
#define WORK_HSPT_FLD     713
#define WORK_HSPT_LST     714
#define WORK_HSFC_FLD     715
#define WORK_HSFC_LST     716
#define WORK_HAHS_FLD     717
#define WORK_HAHS_LST     718
#define WORK_HPCE_FLD     719
#define WORK_HPCE_LST     720
#define WORK_HCRP_FLD     721
#define WORK_HCRP_LST     722


//---------------------------------------------------------------------------
// Construction Stage (750-799)
//---------------------------------------------------------------------------
#define WORK_STAG_FLD     750 // Construction Stage
#define WORK_STAG_LST     751
#define WORK_STGR_FLD     752
#define WORK_BNGR_FLD     753
#define WORK_LDGR_FLD     754
#define WORK_TNDN_FLD     755
#define WORK_ACST_FLD     756
#define WORK_DEST_FLD     757
#define WORK_ACBN_FLD     758
#define WORK_DEBN_FLD     759
#define WORK_ACLD_FLD     760
#define WORK_DELD_FLD     761
#define WORK_ACTD_FLD     762
#define WORK_DETD_FLD     763
#define WORK_ACST_LST     764
#define WORK_DEST_LST     765
#define WORK_ACBN_LST     766
#define WORK_DEBN_LST     767
#define WORK_ACLD_LST     768
#define WORK_DELD_LST     769
#define WORK_ACTD_LST     770
#define WORK_DETD_LST     771
#define WORK_TSTP_FLD     772

//---------------------------------------------------------------------------
// Grid Analysis Model (800-899)
//---------------------------------------------------------------------------
#define WORK_GRID_FLD     800
#define WORK_MGDR_FLD     801
#define WORK_MGDR_LST     802
#define WORK_CBEM_FLD     803
#define WORK_CBEM_LST     804
#define WORK_LDLN_FLD     805
#define WORK_LDLN_LST     806
#define WORK_GILC_FLD     807
#define WORK_GILC_LST     808
#define WORK_GILD_FLD     809
#define WORK_GILD_LST     810
#define WORK_BRGT_FLD     811
#define WORK_BRGT_LST     812
#define WORK_BSPN_FLD     813
#define WORK_BSPN_LST     814
#define WORK_GSTY_FLD     815
#define WORK_GSTY_LST     816
#define WORK_GIMP_FLD     817
#define WORK_GIMP_LST     818
#define WORK_CREF_FLD     819
#define WORK_CREF_LST     820
#define WORK_P1LT_FLD     821
#define WORK_P1LT_LST     822
#define WORK_P1LA_FLD     823
#define WORK_P1LA_LST     824
#define WORK_LNKF_FLD     825
#define WORK_LNKF_LST     826
#define WORK_GMAS_FLD     827
#define WORK_GMAS_LST     828
#define WORK_GACD_FLD     829
#define WORK_GACD_LST     830
//---------------------------------------------------------------------------
// Pushover (900-1000)
//---------------------------------------------------------------------------
#define WORK_PUSH_FLD     900
#define WORK_POGD_FLD     901
#define WORK_POLD_FLD     902 // v730부터 POLC로 변경
#define WORK_POLD_LST     903 // v730부터 POLC로 변경
#define WORK_HNGT_FLD     904 // v730부터 PHGT로 변경
#define WORK_HNGT_LST     905 // v730부터 PHGT로 변경
#define WORK_HNGE_FLD     906 // v730부터 PHGE로 변경
#define WORK_HNGE_LST     907 // v730부터 PHGE로 변경
#define WORK_PORD_FLD     908
#define WORK_PORD_LST     909
#define WORK_POFC_FLD     910
#define WORK_POFC_LST     911
#define WORK_IMPORT_FIMP_PO_FLD  912
#define WORK_FIMP_PO_FLD  913
#define WORK_FIMP_PO_LST  914
#define WORK_FIBR_PO_FLD  915
#define WORK_FIBR_PO_LST  916
#define WORK_FIBW_PO_FLD  917
#define WORK_FIBW_PO_LST  918
#define WORK_POEF_FLD     919
#define WORK_POEF_LST     920
#define WORK_PORG_FLD     921
#define WORK_PORG_LST     922
#define WORK_PORE_FLD     923
#define WORK_PORE_LST     924
#define WORK_PORI_FLD     925
#define WORK_PORI_LST     926
#define WORK_PO_MEMBASSIG_FLD 927
#define WORK_PUSH_FLD_END 1000

//------------------------------------------------------------------------
// Analysis Control(1001)
//------------------------------------------------------------------------
#define WORK_ACOP_FLD     1001

//------------------------------------------------------------------------
// Dynamic Report Figure
//------------------------------------------------------------------------
#define WORK_DYST_FLD     1002
#define WORK_DYST_LST     1003
#define WORK_DYDB_FLD     1004
#define WORK_DYDB_LST     1005
#define WORK_DYIM_FLD     1006
#define WORK_UFIG_FLD     1007
#define WORK_UFIG_LST     1008
#define WORK_UIMG_FLD     1009
#define WORK_UIMG_LST     1010
#define WORK_DYTB_FLD     1011
#define WORK_UTBL_FLD     1012
#define WORK_UTBL_LST     1013
#define WORK_USTB_FLD     1014
#define WORK_USSS_FLD     1015
#define WORK_USSS_LST     1016
#define WORK_USRS_FLD     1017
#define WORK_USRS_LST     1018
#define WORK_USCS_FLD     1019
#define WORK_USCS_LST     1020
#define WORK_UTPL_FLD     1021
#define WORK_UTPL_LST     1022
#define WORK_UCHT_FLD     1023
#define WORK_UCHT_LST     1024
#define WORK_DYTT_FLD     1025
#define WORK_UTXT_FLD     1026
#define WORK_UTXT_LST     1027
#define WORK_USUM_FLD     1028
#define WORK_USUM_LST     1029
#define WORK_UFTR_FLD     1030
#define WORK_UFTR_LST     1031

//------------------------------------------------------------------------
// Rebar Data
//------------------------------------------------------------------------
#define WORK_REBD_FLD     1041
#define WORK_REBB_FLD     1042
#define WORK_REBB_LST     1043
#define WORK_REBC_FLD     1044
#define WORK_REBC_LST     1045
#define WORK_REBR_FLD     1046
#define WORK_REBR_LST     1047
#define WORK_REBW_FLD     1048
#define WORK_REBW_LST     1049
#define WORK_RBMS_FLD     1050
#define WORK_RBMS_LST     1051
#define WORK_RBMW_FLD     1052
#define WORK_RBMW_LST     1053
#define WORK_RBMS_T1_FLD  1054
#define WORK_RBMS_B1_FLD  1055
#define WORK_RBMS_T2_FLD  1056
#define WORK_RBMS_B2_FLD  1057
#define WORK_RBST_FLD     1058
#define WORK_RBST_LST     1059
#define WORK_RBST_T_FLD   1060
#define WORK_RBST_B_FLD   1061
#define WORK_RIPB_FLD     1062
#define WORK_RIPB_LST     1063
#define WORK_RIPC_FLD     1064
#define WORK_RIPC_LST     1065
#define WORK_RBSL_FLD     1066
#define WORK_RBSL_LST     1067
#define WORK_RBSL_T1_FLD  1068
#define WORK_RBSL_B1_FLD  1069
#define WORK_RBSL_T2_FLD  1070
#define WORK_RBSL_B2_FLD  1071

#define WORK_CHRP_UBSI_LST  1101
#define WORK_CHRP_UBSI_FLD  1102

#define WORK_CHRP_UMLD_LST  1103
#define WORK_CHRP_UMLD_FLD  1104

#define WORK_CHRP_UBCL_LST  1105
#define WORK_CHRP_UBCL_FLD  1106

#define WORK_CHRP_UNUL_LST  1107
#define WORK_CHRP_UNUL_FLD  1108

#define WORK_CHRP_ULSC_LST  1109 
#define WORK_CHRP_ULSC_FLD  1110 

#define WORK_CHRP_USSC_LST  1111 
#define WORK_CHRP_USSC_FLD  1112

#define WORK_CHRP_FCMB_LST  1113 
#define WORK_CHRP_FCMB_FLD  1114

#define WORK_STOR_FLD     1201
#define	WORK_STOR_LST     1202


#define WORK_DYUN_FLD		  1203

#define	WORK_SPAN_STEELGIRDER_FLD     1204
#define	WORK_SPAN_STEELGIRDER_LST     1205

//---------------------------------------------------------------------------
// Fire Steel Load
//---------------------------------------------------------------------------
#define WORK_FIRE_FLD  1206
#define WORK_FLUE_FLD  1207
#define WORK_FLUE_LST  1208
#define WORK_STTF_FLD  1209
#define WORK_STTF_LST  1210
#define WORK_FIPA_FLD  1211
#define WORK_FIPA_LST  1212
#define WORK_FSSF_FLD  1213

//---------------------------------------------------------------------------
// Concurrent Reaction
//---------------------------------------------------------------------------
#define WORK_CRGR_FLD     1214
#define WORK_CRGR_LST     1215
//---------------------------------------------------------------------------
// Concurrent Joint Force
//---------------------------------------------------------------------------
#define WORK_CJFG_FLD     1216
#define WORK_CJFG_LST     1217

//---------------------------------------------------------------------------
// Moving Load France (580-619)  - vehicle & vehicle class는 국내버전과 같이 사용 
//---------------------------------------------------------------------------
#define WORK_MVLDANLfr_FLD  1218
#define WORK_LLANfr_FLD     1219
#define WORK_LLANfr_LST     1220
#define WORK_SLANfr_FLD     1221
#define WORK_SLANfr_LST     1222
#define WORK_MVLDfr_FLD     1223
#define WORK_MVLDfr_LST     1224
//---------------------------------------------------------------------------
// Property 추가 (1500~2000)
//---------------------------------------------------------------------------
#define WORK_ESSF_FLD     1500
#define WORK_ESSF_LST     1501
#define WORK_SIHP_FLD     1502
#define WORK_VBEM_FLD     1503
#define	WORK_VBEM_LST     1504
#define WORK_SECV_FLD     1505
#define	WORK_SECV_LST     1506
#define	WORK_SPAN_COMPOSTIE_VB_FLD     1507
#define	WORK_SPAN_COMPOSTIE_VB_LST     1508

//---------------------------------------------------------------------------
// Tracing 추가 (2001~2100)
//
#define WORK_TDAS_FLD     2001
#define	WORK_TDAS_LST     2002

//---------------------------------------------------------------------------
// Moving Load New Zealand (2101~2200)
//---------------------------------------------------------------------------
#define WORK_DYNZ_FLD     2101 
#define WORK_DYNZ_LST     2102


//---------------------------------------------------------------------------
// Moving Load BS (2201~2300)
//---------------------------------------------------------------------------
#define WORK_DYBS_FLD     2201 
#define WORK_DYBS_LST     2202
#define WORK_DSFC_FLD     2211 
#define WORK_DSFC_LST     2212

//---------------------------------------------------------------------------
// Moving Load Brazil (2301~2400)
//---------------------------------------------------------------------------
#define WORK_IFBZ_FLD     2301 
#define WORK_IFBZ_LST     2302