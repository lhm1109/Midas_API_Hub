// RptPlateGirder_EC4.h: interface for the CRptPlateGirder_EC4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptPlateGirder_EC4_H__)
#define AFX_RptPlateGirder_EC4_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\XLOut.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

#include "Dgn_DllManager_Base.h"

#include "CRCDataCtrl.h"

#include "HeaderPre.h"

typedef void (GetStrDgnLang)(CStringW, CStringW&);
typedef BOOL (SetStrLanguage)(CStringW);

class __MY_EXT_CLASS__ CRptPlateGirder_EC4 : public CXLOut, CDgn_DllManager_Base
{
public:
	CRptPlateGirder_EC4();
	virtual ~CRptPlateGirder_EC4();

	void SetDetailSheetPage(CMSExcel* pXL, CString strNameBefore, CString strNameCurrent);  
	void PrintReport(CString strPath, int iPrintOpt, BOOL bSaveImage, int nLang); // EC4 출력, PMS.5847 EC excel report for Czech, nLanguage 0:Eng, 1:Cz

public:
	BOOL Get_RptElemKey(ArrElemPairKey& aRptElemKey, ArrElemPairKey& aRptGenElemKey);

protected:
	GetStrDgnLang*  m_pGetStrDgnLang;
    SetStrLanguage* m_pSetStrLanguage;
    CString GetDgnStrValue(CString strResID);
    void    SetMultiLang(CMSExcel* pExcel, const int& stRow, const int& stCol, CString strVal, BOOL bSubSupType=FALSE);

protected:
	void SetUnitConvertFactor();

	// Print Title
	void PrintTitle_Sub_Title_Shear_ResistanceChk(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintTitle_Sub_Title_Shear_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintTitle_Sub_Title_Shear_LeftWeb(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintTitle_Sub_Title_Shear_Right_Web(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	void PrintTitle_Sub_Title_Stress_Limitation_Chk(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	// Design Input Data
	void PrintDesignInfo(CMSExcel* pXL, int& iStRow, int iElemNo, int nPosi);
	void PrintDesignInfo_Material(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_Section(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	void PrintDesignInfo_SectionSize(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionImage_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionImage_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionImage_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionSize_Compo_I(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionSize_Compo_Box(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionSize_Compo_Tub(CMSExcel* pXL, int& iStRow, CString strBaseSheet);
	void PrintDesignInfo_SectionStiffness(CMSExcel* pXL, int& iStRow, CString strBaseSheet);

	// Bending Check Result Print
	void PrintBendingResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int nPosiNega, T_PGBR_BASE& PgbrBaseGd, T_PGBR_BASE& PgbrBase, T_PGBR_DETAIL& PgbrDetail);
	void PrintStressInformation(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_GLINE_DGN_D& GLineDgn, T_DLINE_CALC_D& StressInfoD);
	void PrintStressInformationDB(CMSExcel* pXL, int& iStRow, CString strBaseSheet, T_STRESS_INFO_I& StrInfoD);
	
	// Vertical Shear Check Result Print_DB
	void PrintShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
	void PrintShearResistance_ShearDgnForce(CMSExcel* pXL, int& iStRow, T_PRVS_BASE& PrvsGd, T_PRVS_BASE& PrvsBase, T_PRVS_DETAIL& PrvsDetail);
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

	// Transverse Force Check Result Print
	void PrintTransverseForceResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PRTF_BASE& PrtfBase, T_PRTF_DETAIL& PrtfDetail);
	
	// Longitudinal Shear Check Result Print
	void PrintLongitudinalShearResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGLS_BASE& PglsGd, T_PGLS_BASE& PglsBase, T_PGLS_DETAIL& PglsDetail);
 
	 // Fatigue Check Result Print 
	void PrintFatigueResistance(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGRF_BASE& PgrfBase, T_PGRF_DETAIL& PgrfDetail);
	
	// Stress Limit Check Result Print  
	void PrintStressLimit(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PGBS_BASE& PgbsBase, T_PGBS_DETAIL& PgbsDetail);

	// Longitudinal Shear Serviceability Check Result Print  
	void PrintLongitudinalShear_SLS(CMSExcel* pXL, int& iStRow, int iChapIndex, int iMaxMin, T_PLSS_BASE& PlssGd, T_PLSS_BASE& PlssBase, T_PLSS_DETAIL& PlssDetail);

	void Print_CheckElemPosition(CMSExcel* pExcel, int& iStRow, CString strBaseSheet, int iElemNo, int nPosi);
	void PrintStressInfo4Class(CMSExcel* pXL, int& iStRow, CArray<T_GLINE_PART_DGN,T_GLINE_PART_DGN&>& aPartLine, CArray<T_DLINE_PART_D,T_DLINE_PART_D&>& aDgnLinePartD);
	void PrintClassificationofSection(CMSExcel* pXL, int& iStRow, int iTopCS, int iWebCS, int iBotCS, int iSectCS);

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
	double m_dArea, m_dIyy, m_dIzz, m_dYbar, m_dZbar;
	double m_dArea_cr, m_dIyy_cr, m_dIzz_cr, m_dYbar_cr, m_dZbar_cr;

	double m_dSize[10];
	double m_dBc, m_dtc, m_dHh;
	int m_nSectType;  

	double m_dfy, m_dfy1, m_dEs;
	double m_dfck, m_dEcm;
	double m_dfsk, m_dErs;  

	int m_iBendingChapterIx;
	int m_iShearChapterIx;
	int m_iLTBChapterIx;
	int m_iTransverseForceChapterIx;
	int m_iLongitudinalChapterShearIx;
	int m_iFatigueChapterIx;
	int m_iStressLimitChapterIx;
	int m_iLongShearSLSChapterIx;

protected:
	CDBDoc* m_pDoc;

	CCRCDataCtrl* m_pDataCtrl;

	T_CPGD_D m_CpgdD;
	ArrElemPairKey m_arDetlElem;  // 상세계산서 출력 부재 List.
	
	CString m_FontType;
	CArray<CString, CString> m_arSheetName;  // 현재 Book이 가지고 있는 Sheet의 이름들.

	double m_dZero;

};

#include "HeaderPost.h"

#endif // !defined(AFX_RptPlateGirder_EC4_H__)
