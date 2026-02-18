// RptPlateGirder_IRC22_15.h: interface for the CRptPlateGirder_IRC22_15 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptPlateGirder_IRC22_15_H__)
#define AFX_RptPlateGirder_IRC22_15_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\XLOut.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

#include "CRCDataCtrl.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptPlateGirder_IRC22_15 : public CXLOut
{
public:
	CRptPlateGirder_IRC22_15();
	virtual ~CRptPlateGirder_IRC22_15();

	void SetDetailSheetPage(CMSExcel* pXL, CString strNameBefore, CString strNameCurrent);  
	void PrintReport(CString strPath, int iPrintOpt, BOOL bSaveImage);       // EC4 출력

	double m_dH, m_dtw, m_dB1, m_dBf1, m_dtf1, m_dB2, m_dBf2, m_dBf3, m_dtf2, m_dFillet;	// Section.
	double m_dHw;
	double m_dr1, m_dr2; // Fillet
	double m_dArea, m_dAsy, m_dAsz, m_dRxx, m_dRyy, m_dRzz;
	double m_dSyy, m_dSzz, m_dZyy, m_dZzz , m_dCwp;
	double m_dYbar, m_dZbar, m_dQyb, m_dQzb;
	double m_dCym, m_dCyp;
	double m_dCzm, m_dCzp;
	double m_dTanA, m_dRoy, m_dRoz, m_dRop, m_dRyz;
	double m_dy1, m_dy2, m_dy3, m_dy4;  
	double m_dz1, m_dz2, m_dz3, m_dz4;
	double m_dy0, m_dz0; // 전단중심.
	int m_nSectType;
	double m_dFxx, m_dFyy, m_dFzz, m_dMx, m_dMy, m_dMz;						// Force.
	double m_dKy, m_dKz, m_dLy, m_dLz, m_dLu;											// Length.
	double m_dLKy, m_dLKz;
	double m_dCLMT, m_dTLMT, m_dCHy, m_dCHz;
	double m_dCb, m_dCmy, m_dCmz;
	double m_dRatBy,m_dRatBz;
	double m_dRatSy,m_dRatSz;
	//  
	double m_dArea_eff, m_dRxx_eff, m_dRyy_eff, m_dRzz_eff;
	double m_dy1_eff, m_dy2_eff, m_dy3_eff, m_dy4_eff;
	double m_dz1_eff, m_dz2_eff, m_dz3_eff, m_dz4_eff;
	double m_dRoy_eff, m_dRoz_eff;
	double m_dAr,m_dFyk,m_dBeff;
	// Force & Stress
	double m_dfa, m_dfsy, m_dfsz, m_dfbcy, m_dfbty, m_dfbcz, m_dfbtz;
	double m_dFa, m_dFsy, m_dFsz, m_dFBCy, m_dFBTy, m_dFBCz, m_dFBTz;
	double m_dAYbyIShortTerm,m_dAYbyILongTerm;
	double m_dmodularratiopermt,m_dmodularratiotransient;
	double m_dLentot;
protected:
	void SetUnitConvertFactor();
	BOOL Get_RptElemKey(ArrElemPairKey& aRptElemKey);

	// Print Title
	void PrintTitle_Sub_Title_Shear_ResistanceChk(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintTitle_Sub_Title_Shear_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintTitle_Sub_Title_Shear_LeftWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintTitle_Sub_Title_Shear_Right_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	void PrintTitle_Sub_Title_Stress_Limitation_Chk(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	// Design Input Data
	void PrintDesignInfo(CMSExcel* pXL, int& iStRow, int iElemNo, int nPosi);
	void PrintDesignInfo_Mod(CMSExcel* pXL, int& iStRow, int iElemNo, int nPosi,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect, T_PGBR_IRC_BASE& PgbrIRC);
	void PrintDesignInfo_Material(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_Section(CMSExcel* pXL, int& iStRow, CString strBaseSheet,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);

	void PrintDesignInfo_SectionSize(CMSExcel* pXL, int& iStRow, CString strBaseSheet,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);

	void PrintDesignInfo_SectionImage_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionImage_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionImage_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	void PrintDesignInfo_SectionSize_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);
	void PrintDesignInfo_SectionSize_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);
	void PrintDesignInfo_SectionSize_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);
	void PrintDesignInfo_SectionStiffness(CMSExcel* pXL, int& iStRow, CString strBaseSheet,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);

	//Positive Flexure
	virtual void SetValue_I_P_01(CMSExcel* pExcel, CString& RptBlock, short &Row_Ref, short &Col_Ref, int& stRow,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect);
	void SetValue_I_P_Sect    (CMSExcel* pExcel, CString& RptBlock, CSG_MEMB_RES_AASHTO_LRFD_D& RptData , int& stRow);
	void SetValue_I_P_02_Title(CMSExcel* pExcel, CString& RptBlock , int& stRow);
	void SetValue_I_P_02_01   (CMSExcel* pExcel, CString& RptBlock, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, int& stRow);
	void SetValue_I_P_02_01_01(CMSExcel* pExcel, CString& RptBlock, CSG_MEMB_RES_AASHTO_LRFD_D& RptData, int& stRow);
 // virtual void SetValue_I_P_02_02   (CMSExcel* pExcel, CString& RptBlock, CSG_MEMB_RES_AASHTO_LRFD_STR_SECT_PROP_D& RptData , int& stRow);
	//Report Data
	void SetRptData(ElemPairK ElemK, int nPosi,CSG_MEMB_POSD& InData, CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect, T_PGBR_BASE& PgbrBase);
	void SetRptData_SectStiffness(int nPosi,  DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData);

	void ConvertCGSGlobalData(T_CPGD_D &CpgdD, S_CSGD_D &rData);

	//Other Reqd Functuions
	int  CopyRange(CMSExcel* pExcel, CString& RptBlock, int stRow);
	// Bending Check Result Print
	void PrintBendingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int nPosiNega, T_PGBR_BASE& PgbrBaseGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	void PrintBendingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int nPosiNega, T_PGBR_BASE& PgbrBaseGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail,CSG_MEMB_RES_AASHTO_LRFD_D& RptData , T_SECT_STIFFNESS& SpscSect,T_PGLS_D& PglsD, T_PGBR_IRC_BASE& PgbrIRCBase);
	void PrintStressInformation(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& StressInfoD);
	void PrintStressInformationDB(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_STRESS_INFO_I& StrInfoD);
	

	void PrintCellData(CMSExcel* pExcel, CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow);
	void PrintCellData(CMSExcel* pExcel, CString strCellname, double    dData, short &rRow_Ref, short &rCol_Ref, int& stRow);
	void PrintCellData(CMSExcel* pExcel, CString strBaseBook, CString strSheetName, CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow);
	void PrintCellData(CMSExcel* pExcel, CString strBaseBook, CString strSheetName, CString strCellname, double    dData, short &rRow_Ref, short &rCol_Ref, int& stRow);
	BOOL CalcCellID(int stRow, short Row, short Col, short Row_Ref, short Col_Ref, short &rRow_Calc, short &rCol_Calc);
	void DeleteCellData(CMSExcel* pExcel, CString strBaseBook, CString strSheetName, CString strCellname, int  fwdDelRows, short &rRow_Ref, short &rCol_Ref, int& stRow);

	// Vertical Shear Check Result Print_DB
	void PrintShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_ShearDgnForce(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase);
	void PrintShearResistance_PlasticRestance(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_Vpl_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase);
	void PrintShearResistance_Vpl_Rd_ChkRatio(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase);
	void PrintShearResistance_Vbw_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase);
	void PrintShearResistance_Vbw_Rd_Calc_Sum(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase);
	void PrintShearResistance_Vbw_Rd_Calc_Part(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_Vbw_Rd_Param& Vbw_Rd_Param);
	void PrintShearResistance_ShearBucklingCoeff(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_SBC_type1_lessthan_1(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam);
	void PrintShearResistance_SBC_type1_morethan_1(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam);
	void PrintShearResistance_SBC_type1_calc_param(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam);
	void PrintShearResistance_SBC_type2(CMSExcel* pXL,  const CString& strBaseSheet, int& iStRow, const T_PRVS_Vbw_Rd_Param& rVbwRdParam);
	void PrintShearResistance_Vbf_Rd_Calc(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail, const T_PRVS_Vbw_Rd_Param& rVbwRdParam);
	void PrintShearResistance_Vbf_Rd_Calc_Sum(CMSExcel* pXL, const CString& strBaseSheet, int& iStRow, T_PRVS_BASE& PrvsBase);
	CString GetClassString(int iClass);
	// Vertical Shear Check Result Print_GenSect
	void PrintShearResistance_WebShearBucklingCoeff_GenSect(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_FlangeShearBucklingCoeff_GenSect(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_ContributionWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_SubPanel_Lamda_w(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_Vbw_Rd_Sub_Panel& VbwSubPanel);
	void PrintShearResistance_ContributionFlange(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_Interation_M_V(CMSExcel* pXL, int& iStRow, CString strBaseSheet, int iClassSect, double deta1, double dEta1_lim, double deta3, double dRatio);
	void PrintShearResistance_Mf_Rd(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);

	// Lateral Torsional Buckling Check Result Print
	void PrintLateralTorsionalBucklingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail);
	void PrintLateralTorsionalBucklingResistance_Neg(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail);
	void PrintLateralBending_SteelOnly(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLTB_BASE& PltbGd, T_PLTB_BASE& PltbBase, T_PLTB_DETAIL& PltbDetail);

	// Transverse Force Check Result Print
	void PrintTransverseForceResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail,  T_PGBR_IRC_BASE& PgbrIRCBasePRTF);
	
	// Longitudinal Shear Check Result Print
	void PrintLongitudinalShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail,  T_PGBR_IRC_BASE& PgbrIRCGd);
 
	 // Fatigue Check Result Print 
	void PrintFatigueResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail,  T_PGBR_IRC_BASE& PgbrIRCGd);
	
	// Stress Limit Check Result Print  
	void PrintStressLimit(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail,  T_PGBR_IRC_BASE& PgbrIRCGd,BOOL bCheckService[4]);

	// Longitudinal Shear Serviceability Check Result Print  
	void PrintLongitudinalShear_SLS(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail);

	void Print_CheckElemPosition(CMSExcel* pExcel, int& iStRow, CString strBaseSheet, int iElemNo, int nPosi);
	void PrintStressInfo4Class(CMSExcel* pXL, int& iStRow, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aPartLine, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aDgnLinePartD);
	void PrintClassificationofSection(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS);
	void PrintClassificationofSectionDetail(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS,double dTopClassRat,double dBotClassRat,double dWebClassRat);


	BOOL Get_DesignInfo_Section(ElemPairK ElemK, BOOL bPosiI);
	CString Get_String4PartShape(int nPartType);
	CString Get_String4ShearLamdaw(BOOL bWeb, int iLamda_w_Type);
	CString Get_String4ShearSubPanelLamdaw(BOOL bWeb, int iLamda_w_Type);
	CString Get_String4ShearXwType(BOOL bWeb, int iEndSupportType, int iLamda_w_scope);

	CString GetLoadCombinationName(int OrgL);
	CString GetStressPosition(int iPosition);

public:
	BOOL Get_CheckResultAndPrintElemPositionType(ElemPairK ElemK);
	
public:
	BOOL m_bStopExecute;
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};

protected:
	double m_dArea_g, m_dIyy_g, m_dIzz_g, m_dYbar_g, m_dZbar_g;
	double  m_dIyy, m_dIzz;
	double m_dArea_cr, m_dIyy_cr, m_dIzz_cr, m_dYbar_cr, m_dZbar_cr;
	double m_dIw,m_dIt,dMpl_Rd;

	double m_dSize[10];
	double m_dBc, m_dtc, m_dHh;
	 

	double m_dfy, m_dfy1, m_dEs , m_dFu;
	double m_dfck, m_dEcm;
	double m_dfsk, m_dErs;  
	double m_dMomentRatio;
	int m_iBendingChapterIx;
	int m_iShearChapterIx;
	int m_iLTBChapterIx;
	int m_iTransverseForceChapterIx;
	int m_iLongitudinalChapterShearIx;
	int m_iFatigueChapterIx;
	int m_iStressLimitChapterIx;
	int m_iLongShearSLSChapterIx;

	CString m_strShapeType,m_strMatNameSteel;
	int m_iShapeType;
protected:
	CDBDoc* m_pDoc;

	CCRCDataCtrl* m_pDataCtrl;

	T_CPGD_D m_CpgdD;
	ArrElemPairKey m_arDetlElem;  // 상세계산서 출력 부재 List.
	
	CString m_FontType;
	CArray<CString, CString> m_arSheetName;  // 현재 Book이 가지고 있는 Sheet의 이름들.

	CSG_MEMB_POSD_KSCE_LSD *m_pMembInD;
	//CSG_MEMB_RES_AASHTO_LRFD_D *m_pMembRes;
	CSG_MEMB_RES_AASHTO_LRFD_D *m_pMembRes;

	double m_dZero;

	//Init Data
	int m_nChkType; // 1:Strength, 2:Serviceability, 3:Constructibility, 4:Fatigue, 5:Shear connectors, 6:Stiffeners

	BOOL   m_bCurved;
	BOOL   m_bSupprotNode;
	BOOL   m_bAppTapered;
	BOOL   m_b1p3RhMy;
	BOOL   m_bPBTFA4SH;
	BOOL   m_bDistortion;
	BOOL   m_bIsRatingDgn;

	double m_dRadius;
	double m_dN;
	double m_dCosTap; // Tapered경우 I,J단의 경사에 대한 Cos값 

	// section data - slab deck
	//double m_dBs; 부모클래스 사용 
	//double m_dts; 부모클래스 사용 
	//double m_dth; 부모클래스 사용 // concrete deck 지압판(?) + top flange thickness 포함.
	double m_dcrt; // concrete deck top reinforcement.
	double m_dcrb; // concrete deck bottom reinforcement.

	// material data - slab deck
	double m_dfc;
	//double m_dEc; 부모클래스 사용 
	double m_dArt;
	double m_dArb;
	double m_dFyr;
	double m_dRebarDia_Top, m_dRebarSpace_Top;
	double m_dRebarDia_Bot,m_dRebarSpace_Bot;
	double m_dBeffFactor;
	// section data - steel girder
	double m_dbfc;
	double m_dbfc_betweenWeb;
	double m_dbfc_hf;
	double m_dtfc;
	double m_dbft;
	double m_dbft_betweenWeb;
	double m_dbft_hf;
	double m_dtft;
	double m_dD;    // web 순길이
	double m_dAoh;
	double m_dAoh_nc;
	double m_dAft;
	double m_dAfb;
	//double m_dtw;

	// material data - steel girder
	double m_dFyc;
	double m_dFyt;
	double m_dFyw;
	//double m_dEs; 부모 클래스 사용

	CString m_strFyc;
	CString m_strFyt;
	CString m_strFyw;
	double  m_dOrgFyc;
	double  m_dOrgFyt;
	double  m_dOrgFyw;

	// shear connector
	int    m_iType; // 0:stud shear, 1:channel
	int    m_nCategoryType_SC; // shear connector Category Tyep
	double m_dH_SC;
	double m_dd_SC;
	double m_dW_SC;
	double m_dp_use; // 설치 간격
	double m_dst_use; // C to C
	int    m_nn; // 횡단면으로 배치된 전단연결재의 개수
	double m_dFyu;
	double m_dFrc;

	// Longitudinal Stiffeners
	BOOL   m_bExist_ls; //??//
	BOOL   m_bExist_ls_flg; // 압축 플랜지의 보강재 유무 
	int    m_nType_ls; // 1:1면, 2:2면
	int    m_nLStfnNumCFlg; // 압축플랜지 보강재 수
	double m_dbl_ls;
	double m_dts_ls;
	double m_dd0_ls;
	double m_dd1_ls;
	double m_dI_ls;
	double m_dI_FromCenter_ls;//
	double m_dA_ls;
	double m_dy_ls;
	double m_dFys_ls;
	double m_dw;
	double m_dIs;
	double m_dIs_web;
	int    m_nCurveType;  // 0 : convex, 1: concave
	BOOL   m_bIJDirct;   // i->j : true , j-> i : false


	// Transverse Stiffeners
	BOOL   m_bExist_ts; //??//
	BOOL   m_bExist_longStiff_Web;
	int    m_nType_ts; // 1:1면, 2:2면
	double m_dbt_ts;
	double m_dtp_ts;
	double m_dd0_ts;
	double m_dFys_ts;

	// Bearing Stiffeners
	double m_dFys_bs;


	// Deck Overhang Loads
	BOOL   m_bUseDohl;
	double m_dDistForce;
	double m_dConcForce;
	double m_dEccenLoad;

	// section modulus
	double m_dI;
	double m_dI_3n, m_dI_r;
	double m_dI_n, m_dI_r3;

	double m_dStop; // Girder 
	double m_dStop_3n, m_dStop_r3;// long
	double m_dStop_n,  m_dStop_r; // short

	double m_dSbot; // Girder
	double m_dSbot_3n, m_dSbot_r3;// long
	double m_dSbot_n,  m_dSbot_r; // short

	double m_dScom_z; // compression section modulus for total section about y-axis (for lateral bending check)

	double m_dyc;
	double m_dyt;
	double m_dy_n_slab;   // 탄성 중립축에서부터 압축을 받는 slab까지의 거리

	// Forces
	double m_dMd1; // Steel
	double m_dMd2; // Long-term
	double m_dMd3; // Short-term
	double m_dMuy; // Strength 휨 부재력
	double m_dVu_gr;
	double m_dVu_lt;
	double m_dVu_st;
	double m_dVu_sm;
	double m_dMuz;
	double m_dF_l;
	double m_dP_l;
	double m_dAlpha;
	double m_dM_Lat_R;  
	double m_dM_Lat_cf; // Curved.. 집중하중
	double m_dM_Lat_df; // Curved.. 분포하중
	double m_dM_Lat;    // Curved..
	double m_dVu;
	double m_dVuInclined; // Box tub일때만 사용
	double m_dT;
	double m_dT1; // DC1
	double m_dT2; // DC2+DW
	double m_dT3; // LL+IL 
	//
	double m_dMy_Ulm_max; // Unbraced Length Moment


	// Length
	double m_dLb;
	BOOL m_bUseUnbraceMoment;

	// Fatigue
	double m_dADTT_SL;
	double m_dADTT_SL75;
	int    m_nDetailCat;
	double m_dn_Fati;

	// 동바리 여부
	BOOL  m_bShored;
	// Cantilever
	BOOL  m_bCantilever;
	// Box Option
	BOOL  m_bMultiBox;
	// Theta ( Box, Tub)
	double m_dTheta;

	BOOL m_bCompactPosiServ; // 사용한계검토시  compact section 여뷰


	// 중간값
	BOOL m_bPNAinWeb;
	int  m_nCase_PNA;
	double m_dY_Bar_PNA;
	BOOL m_bClassfication;
	double m_dA0;

	// Serviceability Concrete Effective Check
	double m_dfr;
	double m_dfdeck;

	//double m_dMp; // Plastic Moment
//  double m_dMy; // Yield Moment
	double m_dMyt; // Yield Moment of tension flange
	double m_dMyc; // Yield Moment of compression flange

	double m_dVn;
	double m_dVcr; // shear-buckling resistance.

	double m_dRh;

	// 응력 계산
	double m_dfbuc; // compression flange
	double m_dfbut; // tension falange
	double m_dft_g;
	double m_dfb_g;
	double m_dft_st;
	double m_dfb_st;
	double m_dft_lt;
	double m_dfb_lt;
	double m_dftop;
	double m_dfbot;

	double m_dMyMax_Pos;
	double m_dMyMax_Nes;
	double m_dftop_range;
	double m_dfbot_range;
	double m_df_top_com_st;
	double m_df_top_ten_st;
	double m_df_bot_com_st;
	double m_df_bot_ten_st;

	//double m_dfl;   // stress by Muz

	double m_dLength4SC;
	int    m_nSFCalc;
	int    m_nWarpType;
	double m_dTopFlange;
	double m_dBotFlange;
	int    m_nLongStiNum_Left;
	int    m_nLongStiNum_Right;
	BOOL m_bTrstProvided;
	double m_dTrstPitch;
	int m_nRowNo;
};

#include "HeaderPost.h"

#endif // !defined(AFX_RptPlateGirder_IRC22_15_H__)
