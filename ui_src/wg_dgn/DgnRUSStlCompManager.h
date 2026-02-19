// Dgn_DrawManager.h: interface for the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DgnRUSStlCompManager_H__)
#define __DgnRUSStlCompManager_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"
#include "DgnEngineProgressDlg.h"
#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_STL_OD_Struct.h"


typedef BOOL (RUSStlCompGirder_DetailReport)(void*, void*,  void*,  size_t, BOOL &, BOOL &);

class CDgnRUSStCompManager : public CDgn_DllManager_Base  
{
public:
	CDgnRUSStCompManager();
	virtual ~CDgnRUSStCompManager();

	BOOL InitialData(int nDgnType, int nDgnCode);

	// Table Default Value Setting
	BOOL Get_StlCompTableValue(STL_SNiP_TB_D &rData);
	// Before Composite Data Ctrl - Design
	BOOL Make_StlCompBeforeBaseData(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_BEFORE_BASE_SNiP &BeforeMembBaseD);
	BOOL Check_StlCompBeforeStrength( STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_BEFORE_BASE_SNiP &BeforeMembBaseD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &BeforeStrResD);
	BOOL Check_StlCompBeforeStability(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_BEFORE_BASE_SNiP &BeforeMembBaseD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &BeforeStaResD);
	// After Composite Data Ctrl - Design
	BOOL Make_StlCompAfterBaseData(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD);
	BOOL Make_StlCompShearConnBaseData(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP &AfterMembBaseD);
	BOOL Check_StlCompKappaCheck(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD, STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD);
	BOOL Check_StlCompAfterStrength(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD, STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD,
																																				 STL_COMP_MEMB_RESD_SNiP_AFTER_STR &AfterStrResD);
	BOOL Check_StlCompAfterStability(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD, STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD,
																																					STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &AfterStaResD);
	BOOL Check_StlCompShearConnector(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP &ShearConnBaseD, STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN &ShearConnResD);
	BOOL Check_StlCompFatigue(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD, STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD,
																																					STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &AfterFatiResD);
	BOOL Check_StlCompServiseability( STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD, STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD,
																																						STL_COMP_MEMB_RESD_SNiP_AFTER_SERV &AfterServResD);
	// Excel Report
	BOOL Print_StlCompDetailReport_RUS(int nSelUnit, int nDgnCode, CString strProgramDir, CString strBasefileDir, CString strSaveFileName, STL_COMP_RPT_RES_TOTAL &DummyOutput,
																																									BOOL& bVersionOK, BOOL& bRegression);

protected:
	// Table Default Value Setting
	PDGNENGINE_SETD  *m_pGet_StlCompTableValue;

	// Before Composite Data Ctrl - Design
	PDGNENGINE_FUNC  *m_pMake_StlCompBeforeBaseData;
	PDGNENGINE_FUNC3  *m_pCheck_StlCompBeforeStrength;
	PDGNENGINE_FUNC3  *m_pCheck_StlCompBeforeStability;

	// After Composite Data Ctrl - Design
	PDGNENGINE_FUNC  *m_pMake_StlCompAfterBaseData;
	PDGNENGINE_FUNC  *m_pMake_StlCompShearConnBaseData;
	PDGNENGINE_FUNC3  *m_pCheck_StlCompKappaCheck;
	PDGNENGINE_FUNC6  *m_pCheck_StlCompAfterStrength;
	PDGNENGINE_FUNC6  *m_pCheck_StlCompAfterStability;
	PDGNENGINE_FUNC3  *m_pCheck_StlCompShearConnector;
	PDGNENGINE_FUNC6  *m_pCheck_StlCompFatigue;
	PDGNENGINE_FUNC6  *m_pCheck_StlCompServiseability;

	// Excel Report
	RUSStlCompGirder_DetailReport  *m_pPrint_StlCompDetailReport_RUS;

};

#endif // !defined(__DgnRUSStlCompManager_H__)
