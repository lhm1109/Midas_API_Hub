// VerifyDgnResultFile.h: interface for the CVerifyDgnResultFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERIFYDGNRESULTFILE_H__)
#define AFX_VERIFYDGNRESULTFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\BnFile.h"

#include "CRCDataCtrl.h"

class CResultDataFilePosition;
class CPostCtrl; 


class CVerifyDgnResultFile  
{
public:
	CVerifyDgnResultFile();
	virtual ~CVerifyDgnResultFile();

public:
	void Set_DataCtrlPoint(CCRCDataCtrl* pDataCtrl);
	BOOL CheckAndGetFilePosition(int nDesignResultId, BOOL bDisplayHistory, CResultDataFilePosition* pRdfp);
	BOOL ClearFilePosition(int nDesignResultId, CResultDataFilePosition* pRdfp);

protected:
	BOOL CheckDesignForceResultData(CFile* pFile);
	BOOL CheckDesignLCaseForceResultData(CFile* pFile);
	BOOL CheckDesignPartLoadCaseForceResultData(CFile* pFile);

	LONGLONG Seek(CFile* pFile, LONGLONG lOff, UINT nFrom);
	void SetGlobalVariable(BOOL bDisplayHistory, CResultDataFilePosition* pRdfp);
	void SaveHistoryNF(CString String);
	void DisplayErrorMessage(LPCTSTR lpszErrorMsg, int nErrorCode);
	BOOL CheckBnFileHead(CFile* pFile);

protected:  
	T_SQPG_D m_SqpgD;
	BOOL RB_SQR0_FORCE();
	BOOL RB_Design_Elem();
	BOOL RB_Force_Lcom();  
	BOOL RB_Force_Lcom_Part1();  
	BOOL RB_Force_Lcom_Part2();  
	BOOL RB_Force_Lcom_ExTp();  
	BOOL RB_Force_Stage();
	BOOL RB_Shrs_Stage();
	BOOL RB_Force_Tnpr();
	BOOL RB_Force_Tnsc();
	BOOL RB_Stress_Add();
	BOOL RB_Force_LTerm();
	BOOL RB_Force_STerm();
	BOOL RB_Force_JTG_Tndn();
	BOOL RB_Stress_Lcom();
	BOOL RB_Stress_Lcom_Part1();
	BOOL RB_Stress_Lcom_Part2();
    BOOL RB_Stress_LTerm_Part2();
    BOOL RB_Stress_STerm_Part2();
	BOOL RB_Stress_Lcom_Part_ScfpCr();
	BOOL RB_Stress_Lcom_Part_ScfpSh();
	BOOL RB_Stress_Lcom_Part_ScfpTe();
	BOOL RB_Stress_Lcom_Part_ScfpRe();
	BOOL RB_Force_Lcom_ScfpSum();
	BOOL RB_Stress_Tnpr();
	BOOL RB_Stress_Tnsc();
	BOOL RB_Stress_LTerm();
	BOOL RB_Stress_STerm();
	BOOL RB_Stress_TnprPart2();
	BOOL RB_Stress_TnscPart2();
	BOOL RB_Stss_Lcom();
	BOOL RB_Force_DL_TB05();
	BOOL RB_Force_LL_TB05();
	BOOL RB_Stress_DL_TB05();
	BOOL RB_Stress_LL_TB05();
	BOOL RB_Force_CSTP_TB05();
	BOOL RB_Stress_CSTP_TB05();
	BOOL RB_Force_EQE2_JTG();
	BOOL RB_ForceWeight_JTG();

	//////////////////////////////////////////////////////////////////////////
	// Expansion Force & Stress Position : 2 Positions -> 5 Positions
	//////////////////////////////////////////////////////////////////////////
	BOOL RB_Force_Lcom_EX();  
	BOOL RB_Force_Lcom_Part1_EX();  
	BOOL RB_Force_Lcom_Part2_EX();  
	BOOL RB_Force_Lcom_ExTp_EX();  
	BOOL RB_Force_Stage_EX();
	BOOL RB_Shrs_Stage_EX();
	BOOL RB_Force_Tnpr_EX();
	BOOL RB_Force_Tnsc_EX();
	BOOL RB_Stress_Add_EX();
	BOOL RB_Force_LTerm_EX();
	BOOL RB_Force_JTG_Tndn_EX();
	BOOL RB_Stress_Lcom_EX();
	BOOL RB_Stress_Lcom_Part1_EX();
	BOOL RB_Stress_Lcom_Part2_EX();
	BOOL RB_Stress_Lcom_Part_ScfpCr_EX();
	BOOL RB_Stress_Lcom_Part_ScfpSh_EX();
	BOOL RB_Stress_Lcom_Part_ScfpTe_EX();
	BOOL RB_Stress_Lcom_Part_ScfpRe_EX();
	BOOL RB_Force_Lcom_ScfpSum_EX();
	BOOL RB_Stress_Tnpr_EX();
	BOOL RB_Stress_Tnsc_EX();
	BOOL RB_Stress_LTerm_EX();
	BOOL RB_Stress_TnprPart2_EX();
	BOOL RB_Stress_TnscPart2_EX();
	BOOL RB_Stss_Lcom_EX();
	BOOL RB_Force_DL_TB05_EX();
	BOOL RB_Force_LL_TB05_EX();
	BOOL RB_Stress_DL_TB05_EX();
	BOOL RB_Stress_LL_TB05_EX();
	BOOL RB_Force_CSTP_TB05_EX();
	BOOL RB_Stress_CSTP_TB05_EX();
	BOOL RB_Force_EQE2_JTG_EX();
	BOOL RB_ForceWeight_JTG_EX();

	//++
	T_SQLC_D m_SqlcD;
	int m_iChkElem;
	BOOL RB_SQLC_Lcase_Force();
	BOOL RB_Design_Lcase_Elem();
	BOOL RB_Force_Lcase_Stld();
	BOOL RB_Force_Lcase_Splc();
	BOOL RB_Force_Lcase_Espc();
	BOOL RB_Force_Lcase_This();
	BOOL RB_Force_Lcase_Sgld();
	BOOL RB_Force_Lcase_Step();
	BOOL RB_Force_Lcase_Mvld();
	BOOL RB_Force_Lcase_Smlc();
	BOOL RB_Force_Lcase_GilcStrb();
	BOOL RB_Force_Lcase_GilcSbcf();
	BOOL RB_Force_Lcase_LcomStrb();
	BOOL RB_Force_Lcase_LcomSbcf();

	BOOL RB_LoadCase_Stld();
	BOOL RB_LoadCase_Splc();
	BOOL RB_LoadCase_Espc();
	BOOL RB_LoadCase_This();
	BOOL RB_LoadCase_Sgld();
	BOOL RB_LoadCase_Step();
	BOOL RB_LoadCase_Mvld();
    BOOL RB_LoadCase_MvldBS(); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL RB_LoadCase_Smlc();
	BOOL RB_LoadCase_GilcStrb();
	BOOL RB_LoadCase_GilcSbcf();
	BOOL RB_LoadCase_LcomStrb();
	BOOL RB_LoadCase_LcomStrbMin();
	BOOL RB_LoadCase_LcomSbcf();
	
	BOOL RB_PartLoadCase_Stld();
	BOOL RB_PartLoadCase_Splc();
	BOOL RB_PartLoadCase_Espc();
	BOOL RB_PartLoadCase_This();
	BOOL RB_PartLoadCase_Sgld();
	BOOL RB_PartLoadCase_Step();
	BOOL RB_PartLoadCase_Mvld();
    BOOL RB_PartLoadCase_MvldBS(); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL RB_PartLoadCase_Smlc();
	BOOL RB_PartLoadCase_GilcStrb();
	BOOL RB_PartLoadCase_GilcSbcf();
	BOOL RB_PartLoadCase_LcomStrb();
	BOOL RB_PartLoadCase_LcomStrbMin();
	BOOL RB_PartLoadCase_LcomSbcf();
	BOOL RB_PartLoadCase_StldScfp();
	BOOL RB_PartLoadCase_SgldScfp();

	// 5 Position, Force Expansion - 2 Position -> 5 Position
	BOOL RB_LoadCase_Stld_EX();
	BOOL RB_LoadCase_Splc_EX();
	BOOL RB_LoadCase_Espc_EX();
	BOOL RB_LoadCase_This_EX();
	BOOL RB_LoadCase_Sgld_EX();
	BOOL RB_LoadCase_Step_EX();
	BOOL RB_LoadCase_Mvld_EX();
    BOOL RB_LoadCase_MvldBS_EX();// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL RB_LoadCase_Smlc_EX();
	BOOL RB_LoadCase_GilcStrb_EX();
	BOOL RB_LoadCase_GilcSbcf_EX();
	BOOL RB_LoadCase_LcomStrb_EX();
	BOOL RB_LoadCase_LcomStrbMin_EX();
	BOOL RB_LoadCase_LcomSbcf_EX();

	// 5 Position, Part Force Expansion - 2 Position -> 5 Position
	BOOL RB_PartLoadCase_Stld_EX();
	BOOL RB_PartLoadCase_Splc_EX();
	BOOL RB_PartLoadCase_Espc_EX();
	BOOL RB_PartLoadCase_This_EX();
	BOOL RB_PartLoadCase_Sgld_EX();
	BOOL RB_PartLoadCase_Step_EX();
	BOOL RB_PartLoadCase_Mvld_EX();
    BOOL RB_PartLoadCase_MvldBS_EX();// Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
    BOOL RB_PartLoadCase_Smlc_EX();
	BOOL RB_PartLoadCase_GilcStrb_EX();
	BOOL RB_PartLoadCase_GilcSbcf_EX();
	BOOL RB_PartLoadCase_LcomStrb_EX();
	BOOL RB_PartLoadCase_LcomStrbMin_EX();
	BOOL RB_PartLoadCase_LcomSbcf_EX();
	BOOL RB_PartLoadCase_StldScfp_EX();
	BOOL RB_PartLoadCase_SgldScfp_EX();

protected:
	BOOL m_bDisplayMessage;  
	CResultDataFilePosition* m_pRdfp;
	
	CFile* m_pFile;
	CBnFileResult* m_pBnFile;
	char ID_M[4];
	LONGLONG ID_S,IST_R,LNG_R,NUM_R;
	float m_RetFileVersion;

	CPostCtrl *m_pPostCtrl;
	CCRCDataCtrl* m_pDataCtrl;

	int m_iDgnCode;
	int m_iDgnCodeType;
};

#endif // !defined(AFX_VERIFYDGNRESULTFILE_H__)
