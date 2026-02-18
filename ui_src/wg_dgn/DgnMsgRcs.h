// DgnMsgRcs.h: interface for the CDgnMsgRcs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNMSGRCS_H__32DAA753_762C_470A_BFE2_A30BBD24CC42__INCLUDED_)
#define AFX_DGNMSGRCS_H__32DAA753_762C_470A_BFE2_A30BBD24CC42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\MIT_Lib\rptostream.h"
#include "DgnMsgDgn.h"
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnMsgRcs : public CDgnMsgDgn  
{
public:
	CDgnMsgRcs();
	virtual ~CDgnMsgRcs();

	// Add by Hong,jiseon 2002.07.09
	public:
	LPCTSTR GetMsg(const CString& strMsg);
	LPCTSTR GetMsg(const LPCTSTR& strMsg);

	LPCTSTR Msg_rcs_001();
	LPCTSTR Msg_rcs_002();
	LPCTSTR Msg_rcs_003();
	LPCTSTR Msg_rcs_004();
	LPCTSTR Msg_rcs_004_1();
	LPCTSTR Msg_rcs_005();
	LPCTSTR Msg_rcs_006();
	LPCTSTR Msg_rcs_006_1();
	LPCTSTR Msg_rcs_007();
	LPCTSTR Msg_rcs_008();
	LPCTSTR Msg_rcs_009();
	LPCTSTR Msg_rcs_010();
	LPCTSTR Msg_rcs_012(BOOL bNextLine=TRUE);  
	LPCTSTR Msg_rcs_013(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_014();
	LPCTSTR Msg_rcs_015(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_016();
	LPCTSTR Msg_rcs_017();
	LPCTSTR Msg_rcs_018();
	LPCTSTR Msg_rcs_018_0();
	LPCTSTR Msg_rcs_019();
	LPCTSTR Msg_rcs_019_0();
	LPCTSTR Msg_rcs_020();
	LPCTSTR Msg_rcs_021();
	LPCTSTR Msg_rcs_022();
	LPCTSTR Msg_rcs_023();
	LPCTSTR Msg_rcs_024();
	LPCTSTR Msg_rcs_024_1();
	LPCTSTR Msg_rcs_024_2();
	LPCTSTR Msg_rcs_025();
	LPCTSTR Msg_rcs_025_1();
	LPCTSTR Msg_rcs_025_2();
	LPCTSTR Msg_rcs_025_3();
	LPCTSTR Msg_rcs_025_4();
	LPCTSTR Msg_rcs_026();
	LPCTSTR Msg_rcs_026_1();
	LPCTSTR Msg_rcs_027();
	LPCTSTR Msg_rcs_027_1(); // EC2:04 Neutral Axis x/d 항목 추가
	LPCTSTR Msg_rcs_028();
	LPCTSTR Msg_rcs_029();
	LPCTSTR Msg_rcs_029_1();
	LPCTSTR Msg_rcs_030();
	LPCTSTR Msg_rcs_030_1();
	LPCTSTR Msg_rcs_031();
	LPCTSTR Msg_rcs_031_0();
	LPCTSTR Msg_rcs_031_1();
	LPCTSTR Msg_rcs_032();
	LPCTSTR Msg_rcs_032_1();
	LPCTSTR Msg_rcs_032_2();
	LPCTSTR Msg_rcs_032_3();   // In AIJ_SRC01
	LPCTSTR Msg_rcs_032_4();   // In AIJ_SRC01
	LPCTSTR Msg_rcs_033();
	LPCTSTR Msg_rcs_033_1();

	//Add by Unsang :: EC에서 Strength -> Resistance로 변경함.
	LPCTSTR Msg_rcs_033_EC();
	LPCTSTR Msg_rcs_033_1_EC();

	LPCTSTR Msg_rcs_033_2();
	LPCTSTR Msg_rcs_033_3();
	LPCTSTR Msg_rcs_033_4();
	LPCTSTR Msg_rcs_034();
	LPCTSTR Msg_rcs_034_1();
	LPCTSTR Msg_rcs_035();
	LPCTSTR Msg_rcs_035_1();
	LPCTSTR Msg_rcs_036();
	LPCTSTR Msg_rcs_036_1();
	LPCTSTR Msg_rcs_036_2();
	LPCTSTR Msg_rcs_036_3();
	LPCTSTR Msg_rcs_037();
	LPCTSTR Msg_rcs_038();
	LPCTSTR Msg_rcs_039();
	LPCTSTR Msg_rcs_039_0();
	LPCTSTR Msg_rcs_040();
	LPCTSTR Msg_rcs_040_1();
	LPCTSTR Msg_rcs_040_2();
	LPCTSTR Msg_rcs_041();
	LPCTSTR Msg_rcs_042();
	LPCTSTR Msg_rcs_043();
	LPCTSTR Msg_rcs_044();
	LPCTSTR Msg_rcs_044_1();
	LPCTSTR Msg_rcs_045();
	LPCTSTR Msg_rcs_046();
	LPCTSTR Msg_rcs_047();
	//Add by Unsang :: EC에서 Capacity -> Resistance로 변경함.
	LPCTSTR Msg_rcs_047_EC();

	LPCTSTR Msg_rcs_047_1();//KJH20020826, JTJ023-85, _T("3.1 About Y Axis")
	LPCTSTR Msg_rcs_047_2();//KJH20020827, JTJ023-85, _T("3.2 About Z Axis")
	LPCTSTR Msg_CONCENTRIC_MAX_AXIAL_LOAD();
	LPCTSTR Msg_NORMALIZED_AXIAL_RATIO();// PMS:xxxx-Seungjun-20100514 for EC2:04 capacity design.  
	LPCTSTR Msg_rcs_049();
	LPCTSTR Msg_rcs_049_F();//KJH20020826, JTJ023-85, _T("Factored Axial Load Strength")
	LPCTSTR Msg_rcs_050();
	LPCTSTR Msg_rcs_051();
	LPCTSTR Msg_rcs_052();
	LPCTSTR Msg_rcs_053();
	LPCTSTR Msg_rcs_053_RUS();
	LPCTSTR Msg_rcs_053_1();//KJH20020824, JTJ023-85, _T("Need Not Check")
	LPCTSTR Msg_rcs_054();
	LPCTSTR Msg_rcs_054_F();//KJH20020824, JTJ023-85, _T("Factored Moment Strength")
	LPCTSTR Msg_rcs_055();
	LPCTSTR Msg_rcs_055_1();
	LPCTSTR Msg_rcs_056();
	LPCTSTR Msg_rcs_057();
	LPCTSTR Msg_rcs_058();
	LPCTSTR Msg_rcs_059();
	LPCTSTR Msg_rcs_060();
	LPCTSTR Msg_rcs_061();
	LPCTSTR Msg_P_M_INTERACTION_DIAGRAM();
	LPCTSTR Msg_rcs_061_1();//RSH 2003.01.17
	LPCTSTR Msg_rcs_062();
	LPCTSTR Msg_M_N_INTERACTION_DIGGRAM();
	LPCTSTR Msg_rcs_063();
	LPCTSTR Msg_rcs_063_BS();
	LPCTSTR Msg_rcs_063_EC(); //Add by Unsang :: EC에서 Capacity -> Resistance로 변경함.
	LPCTSTR Msg_SLS_STRESS_CHK();// PMS:4490-DANAKIM-20120613 Serviceability 항목 추가
    LPCTSTR Msg_SLS_CRACK_CHK();
	LPCTSTR Msg_rcs_063(BOOL bMiddle);
	LPCTSTR Msg_rcs_063_EC(BOOL bMiddle);
	LPCTSTR Msg_rcs_063_1(BOOL bMiddle);
	LPCTSTR Msg_rcs_064();
	LPCTSTR Msg_rcs_064(BOOL bMiddle);
	LPCTSTR Msg_rcs_065();
	LPCTSTR Msg_rcs_065_AREMA();
	LPCTSTR Msg_rcs_065_EC();

	LPCTSTR Msg_rcs_066();
	LPCTSTR Msg_rcs_067();
	LPCTSTR Msg_rcs_068();
	LPCTSTR Msg_rcs_069();
	LPCTSTR Msg_rcs_069_1();// PMS:XXXX-DANAKIM-20120622  Shear Ratio max 값 추가
	LPCTSTR Msg_rcs_069_2();// PMS:DNKIM-20121031 Shear Ratio by Conc.
    LPCTSTR Msg_rcs_069_3();// interaction of shear
	LPCTSTR Msg_rcs_070();
	LPCTSTR Msg_rcs_071();
	LPCTSTR Msg_rcs_072();
	LPCTSTR Msg_rcs_073();
	LPCTSTR Msg_rcs_074();
	LPCTSTR Msg_rcs_075();
	LPCTSTR Msg_rcs_076();
	LPCTSTR Msg_rcs_077();
	LPCTSTR Msg_rcs_078();
	LPCTSTR Msg_rcs_079();
	LPCTSTR Msg_rcs_080();
	LPCTSTR Msg_rcs_081();
	LPCTSTR Msg_rcs_082();
	LPCTSTR Msg_rcs_083();
	LPCTSTR Msg_rcs_084();
	LPCTSTR Msg_rcs_085();
	LPCTSTR Msg_rcs_086();
	LPCTSTR Msg_rcs_087();
	LPCTSTR Msg_rcs_088();
	LPCTSTR Msg_rcs_089();
	LPCTSTR Msg_rcs_090();
	LPCTSTR Msg_rcs_091();
	LPCTSTR Msg_rcs_091_1();
	LPCTSTR Msg_rcs_092();
	LPCTSTR Msg_rcs_092_1();
	LPCTSTR Msg_rcs_093();
	LPCTSTR Msg_rcs_094();
	LPCTSTR Msg_rcs_095();
	LPCTSTR Msg_rcs_096();
	LPCTSTR Msg_rcs_097();
	LPCTSTR Msg_rcs_097_1();

	//Add by Unsang :: EC에서 Strength -> Resistance로 변경함.
	LPCTSTR Msg_rcs_097_EC();
	LPCTSTR Msg_rcs_097_1_EC();

	LPCTSTR Msg_rcs_098();
	LPCTSTR Msg_rcs_099();
	LPCTSTR Msg_rcs_100();
	LPCTSTR Msg_rcs_101();
	LPCTSTR Msg_rcs_102();
	LPCTSTR Msg_rcs_103();
	LPCTSTR Msg_rcs_104();
	LPCTSTR Msg_rcs_106_1(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_106_1_1(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_106_1_2(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_106_1_3(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_106_2();
	LPCTSTR Msg_rcs_106_3();
	LPCTSTR Msg_rcs_106_4(); 
	LPCTSTR Msg_rcs_107();
	LPCTSTR Msg_rcs_108();
	LPCTSTR Msg_rcs_112();
	LPCTSTR Msg_rcs_113(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_114(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_114_1(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_115(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_115_1(BOOL bExistCrane=FALSE);  // Add by GAY. MNET:3780. ('08.11.04). For Torsion Design.
	LPCTSTR Msg_rcs_115_2(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_115_3(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_119_1(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_119_2(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_119_3(BOOL bExistCrane=FALSE, BOOL bExistCorner=FALSE);  // Add by GAY. MNET:2683. ('07.05.07).
	LPCTSTR Msg_rcs_119();
	LPCTSTR Msg_rcs_120();
	LPCTSTR Msg_rcs_123();
	LPCTSTR Msg_rcs_126(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_126_1();
	LPCTSTR Msg_rcs_126_2(BOOL bExistCrane=FALSE);
	LPCTSTR Msg_rcs_138();
	LPCTSTR Msg_rcs_142();
	LPCTSTR Msg_rcs_143();
	LPCTSTR Msg_rcs_144();
	LPCTSTR Msg_rcs_150();
	LPCTSTR Msg_rcs_152();
	LPCTSTR Msg_rcs_153();
	LPCTSTR Msg_rcs_154();
	LPCTSTR Msg_rcs_155();
	LPCTSTR Msg_rcs_156();
	LPCTSTR Msg_rcs_159();
	LPCTSTR Msg_rcs_160();
	LPCTSTR Msg_rcs_161();
	LPCTSTR Msg_rcs_162();
	LPCTSTR Msg_rcs_163();
	LPCTSTR Msg_rcs_163_1();
	LPCTSTR Msg_rcs_166();
	LPCTSTR Msg_rcs_168();
	LPCTSTR Msg_rcs_169();
	LPCTSTR Msg_rcs_170();
	LPCTSTR Msg_rcs_170_1();
	LPCTSTR Msg_rcs_173();
	LPCTSTR Msg_rcs_174();
	LPCTSTR Msg_rcs_174_1();
	LPCTSTR Msg_rcs_175();
	LPCTSTR Msg_rcs_175_1();
	LPCTSTR Msg_rcs_175_2();
	LPCTSTR Msg_rcs_176();
	LPCTSTR Msg_rcs_176_1();
	LPCTSTR Msg_rcs_179();
	LPCTSTR Msg_rcs_182();
	LPCTSTR Msg_rcs_182_1();
	LPCTSTR Msg_rcs_183_1();
	LPCTSTR Msg_rcs_183_2();
	LPCTSTR Msg_rcs_184();
	LPCTSTR Msg_rcs_185();
	LPCTSTR Msg_rcs_186();
	LPCTSTR Msg_rcs_187();
	LPCTSTR Msg_rcs_188();
	LPCTSTR Msg_rcs_188_1();
	LPCTSTR Msg_rcs_189_1();
	LPCTSTR Msg_rcs_189_2();
	LPCTSTR Msg_rcs_190();
	LPCTSTR Msg_rcs_191();
	LPCTSTR Msg_rcs_192();
	LPCTSTR Msg_rcs_193();
	LPCTSTR Msg_rcs_194();
	LPCTSTR Msg_rcs_195();
	LPCTSTR Msg_rcs_196();
	LPCTSTR Msg_rcs_197();
	LPCTSTR Msg_rcs_198();  // Add by GAY. MNET:3780. ('08.11.05). Output for Torsion Result Term.
	LPCTSTR Msg_rcs_199_1();  
	LPCTSTR Msg_rcs_199_2();
	LPCTSTR Msg_rcs_200_1();
	LPCTSTR Msg_rcs_200_2();
	LPCTSTR Msg_rcs_201();
	LPCTSTR Msg_rcs_202(); // Add by GAY. PMS:4076. ('10.02.24). Boundary Element Rebar.
	LPCTSTR Msg_rcs_202_Horz();
	LPCTSTR Msg_rcs_202_Vert();
	LPCTSTR Msg_rcs_203_WallStability(); // Add by GAY. PMS:4276. ('12.02.06). Stability Check Result. 
	LPCTSTR Msg_rcs_204();
	LPCTSTR Msg_rcs_205();
	LPCTSTR Msg_rcs_206();
	LPCTSTR Msg_rcs_207();
	LPCTSTR Msg_rcs_208();
	LPCTSTR Msg_rcs_209();
	LPCTSTR Msg_rcs_210();
	LPCTSTR Msg_rcs_211();
	LPCTSTR Msg_rcs_212();
	LPCTSTR Msg_rcs_213();
	LPCTSTR Msg_rcs_214();
	LPCTSTR Msg_rcs_214_0();
	LPCTSTR Msg_rcs_214_2();
	LPCTSTR Msg_rcs_215();
	LPCTSTR Msg_rcs_216();
	LPCTSTR Msg_rcs_217(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_218(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_219(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_220(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_221(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_222_1(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_222_2(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_222_3(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_223(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_224();
	LPCTSTR Msg_rcs_225(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_226(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_227(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_228(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_229(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_229_1();
	LPCTSTR Msg_rcs_230();
	LPCTSTR Msg_rcs_231();
	LPCTSTR Msg_rcs_232();
    LPCTSTR Msg_rcs_233();

	LPCTSTR Msg_rcs_Plate_001();
		
//--------Add by Hong,jiseon 2002.7.11 
//		_LS(IDS_RCS_TEXTOUT_SPAN)
	LPCTSTR Msg_rcs_Textout_Span();
//		_LS(IDS_RCS_TEXTOUT_STIRRUP)
	LPCTSTR Msg_rcs_Textout_Stirrup();
//		_LS(IDS_RCS_TEXTOUT_BEAM_DGN_TITLE)
	LPCTSTR Msg_rcs_Textout_Beam_Dgn_Title();
	//		_LS(IDS_RCS_TEXTOUT_BEAM_CHK_TITLE)
	LPCTSTR Msg_rcs_Textout_Beam_Chk_Title();
//		_LS(IDS_RCS_TEXTOUT_PSBEAM_DGN_TITLE)
	LPCTSTR Msg_rcs_Textout_PSBeam_Dgn_Title();
	//		_LS(IDS_RCS_TEXTOUT_PSBEAM_CHK_TITLE)
	LPCTSTR Msg_rcs_Textout_PSBeam_Chk_Title();
	LPCTSTR Msg_rcs_Textout_SRCBeam_Chk_Title();
//		_LS(IDS_RCS_TEXTOUT_COLM_DGN_TITLE)
	LPCTSTR Msg_rcs_Textout_Colm_Dgn_Title();
//		_LS(IDS_RCS_TEXTOUT_BRCE_DGN_TITLE)
	LPCTSTR Msg_rcs_Textout_Brce_Dgn_Title();
//		_LS(IDS_RCS_TEXTOUT_COLM_CHK_TITLE)
	LPCTSTR Msg_rcs_Textout_Colm_Chk_Title();
//		_LS(IDS_RCS_TEXTOUT_BRCE_CHK_TITLE)
	LPCTSTR Msg_rcs_Textout_Brce_Chk_Title();
//		_LS(IDS_RCS_TEXTOUT_WALL_DGN_TITLE)
	LPCTSTR Msg_rcs_Textout_Wall_Dgn_Title();
//		_LS(IDS_RCS_TEXTOUT_STORY)
	LPCTSTR Msg_rcs_Textout_Story();
//		_LS(IDS_RCS_TEXTOUT_WALL_MARK)
	LPCTSTR Msg_rcs_Textout_Wall_Mark();
//		_LS(IDS_RCS_TEXTOUT_V_REBAR)
	LPCTSTR Msg_rcs_Textout_V_Rebar();
//		_LS(IDS_RCS_TEXTOUT_H_REBAR)
	LPCTSTR Msg_rcs_Textout_H_Rebar();
//		_LS(IDS_RCS_TEXTOUT_END_REBAR)
	LPCTSTR Msg_rcs_Textout_End_Rebar();
//		_LS(IDS_RCS_TEXTOUT_BAR_LAYER)
	LPCTSTR Msg_rcs_Textout_Bar_Layer();
//		_LS(IDS_RCS_TEXTOUT_WALL_CHK_TITLE)
	LPCTSTR Msg_rcs_Textout_Wall_Chk_Title();
//	IDS_RCS_TEXTOUT_SHAPE
	LPCTSTR Msg_rcs_Textout_Shape();

//IDS_RCS_TEXTOUT_HEIGHT
	LPCTSTR Msg_rcs_Textout_Height();

	// Modify by GAY. PMS:4076. ('10.02.26). for Boundary Element rebar+length.
	LPCTSTR Msg_rcs_Textout_BE_Rebar();
	LPCTSTR Msg_rcs_Textout_BE_Leng();

/*
//		_LS(IDS_RCS_TEXTOUT_WALL_LAYER_TITLE)
	LPCTSTR Msg_rcs_Textout_Wall_Layer_Title();
//		_LS(IDS_RCS_TEXTOUT_SUB_TOT_WEIGHT)
	LPCTSTR Msg_rcs_Textout_Sub_Tot_Weight();
*/
	// Coded by Seungjun MNet:No.2393 ('20060830)
	// stress
	LPCTSTR Msg_rcs_Service_00();
	//
	LPCTSTR Msg_rcs_Service_00_EC();            // 6. Stress Check.

	LPCTSTR Msg_rcs_Service_00_1();
	LPCTSTR Msg_rcs_Service_00_2();
	LPCTSTR Msg_rcs_Service_00_3();
	LPCTSTR Msg_rcs_Service_00_3_1();
	LPCTSTR Msg_rcs_Service_00_3_2();
	LPCTSTR Msg_rcs_Service_00_4();
	LPCTSTR Msg_rcs_Service_00_5();
	LPCTSTR Msg_rcs_Service_00_5_1();
	LPCTSTR Msg_rcs_Service_00_6();
	// Crack checking
	LPCTSTR Msg_rcs_Service_01();               // 3. Crack Width.
	LPCTSTR Msg_rcs_Service_01_1();
	LPCTSTR Msg_rcs_Service_01_2();
	LPCTSTR Msg_rcs_Service_02(int iCrackType); // Negative - Dry(or Humidity, Corrosion, High Corrosion).
	LPCTSTR Msg_rcs_Service_03(int iCrackType); // Positive - Dry(or Humidity, Corrosion, High Corrosion).
	LPCTSTR Msg_rcs_Service_04();               // Allowable Crack Width(wa)
	LPCTSTR Msg_rcs_Service_05();               // Crack Width(w)
	LPCTSTR Msg_rcs_Service_06();               // Check Ratio(w/wa)
	LPCTSTR Msg_rcs_Service_07();               // Allowable Crack Width(sa)
	LPCTSTR Msg_rcs_Service_08();               // Crack Width(s)

	// Deflection checking
	LPCTSTR Msg_rcs_Service_11();               // 4. Deflection.
	LPCTSTR Msg_rcs_Service_11_1();
	LPCTSTR Msg_rcs_Service_12();               // Maximum Deflection Comparison.
	LPCTSTR Msg_rcs_Service_13();               // Loadcombination and position.
	LPCTSTR Msg_rcs_Service_14();

	// Fatigue checking
	LPCTSTR Msg_rcs_Service_15();

	LPCTSTR Msg_rcs_Fatigue_01();
	LPCTSTR Msg_rcs_Fatigue_02();
	LPCTSTR Msg_rcs_Fatigue_03();
	LPCTSTR Msg_rcs_Fatigue_04();

	LPCTSTR Msg_rcs_Mu();
	LPCTSTR Msg_rcs_Mr();

	// for russia plate
	LPCTSTR Msg_rcs_300();
	LPCTSTR Msg_rcs_301();
	LPCTSTR Msg_rcs_302();
	LPCTSTR Msg_rcs_303();
	LPCTSTR Msg_rcs_304();
	LPCTSTR Msg_rcs_305(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_306(BOOL bNextLine=TRUE);
	LPCTSTR Msg_rcs_307(BOOL bNextLine=TRUE);

	LPCTSTR Msg_rcs_Service_00_7();
	LPCTSTR Msg_rcs_Service_01_3();

	LPCTSTR Msg_rcs_Service_16();
	LPCTSTR Msg_rcs_Service_17();
	
	LPCTSTR Msg_DesignCode();

	LPCTSTR Msg_JOINT();
	LPCTSTR Msg_JOINT_RATIO();

	LPCTSTR Msg_END();
	LPCTSTR Msg_MIDDLE();
	LPCTSTR Msg_LINEAR_CREEP();
	LPCTSTR Msg_NONLINEAR_CREEP();
	LPCTSTR Msg_CRACKED_SECTION();
	LPCTSTR Msg_DESIGN_FOR_AXIAL_FLEXURE();
	LPCTSTR Msg_DESIGN_FOR_SHEAR();        

private:
    CString GetCharLineBreak(BOOL bNextLine);
};

#include "HeaderPost.h"



#endif // !defined(AFX_DGNMSGRCS_H__32DAA753_762C_470A_BFE2_A30BBD24CC42__INCLUDED_)
