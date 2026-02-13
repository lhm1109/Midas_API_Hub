// RptSteelGirder_AASHTO12.h: interface for the RptSteelGirder_AASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptSteelGirder_AASHTO12_H__)
#define AFX_RptSteelGirder_AASHTO12_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder.h"
#include "..\wg_base\Thread.h"
//#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
 
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

#include "CRCDataCtrl.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CRptSteelGirder_AASHTO12 : public CThread
{
public:
	CRptSteelGirder_AASHTO12();
    CRptSteelGirder_AASHTO12(int nDgnCode);
	virtual ~CRptSteelGirder_AASHTO12();
	
	
protected:
	CDBDoc* m_pDoc;
	T_CPGD_D m_CpgdD;
	BOOL m_bStopExecute;
	CCRCDataCtrl* m_pDataCtrl;
	
public: 
	BOOL Execute_CSGReport_AASHTO12(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};

    void SetRptData(int ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_MEMB_RES_AASHTO_LRFD_D& RptData);

protected:	
	void SetRptData_SectStiffness(int nPosi,  DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData);
	BOOL ShouldCheckPosiIJ(int nFlag, BOOL& bCheckCalcI, BOOL& bCheckCalcJ);
	void ConvertCsgCalcUlmForce(T_CSGD_ULMF &For, CSG_ULMF_D &rData);
	void ConvertCsgCalcChkForce(T_CSGD_COMF &For, CSG_FORCE_COMP &rData);
	void ConvertCsgCalcForce( T_CSGD_FORCE &For, CSG_FORCE_D &rData);
	CString GetLoadCombType(int iMaxType);

	CString Get_StageName(T_STAG_K StagK);
	int GetMaxRatioCase(T_CSFC_BASE &CsfcB);
	void SetBstfPrintData(int ElemK, int nPosi, UINT nOrgLcomK, CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D& BstfResD);

	CString GetThicknessString(int nDgnCode, double dt);
	CString GetThicknessKDS(double dt);
};

#include "HeaderPost.h"

#endif // !defined(AFX_RptSteelGirder_AASHTO12_H__)
