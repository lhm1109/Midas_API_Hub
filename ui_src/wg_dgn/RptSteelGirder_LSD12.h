// RptSteelGirder_LSD12.h: interface for the RptSteelGirder_LSD12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RptSteelGirder_LSD12_H__)
#define AFX_RptSteelGirder_LSD12_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RptSteelGirder.h"
#include "..\wg_base\Thread.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\DB_ST_DN_RESULT_PSC.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
 
#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_CSG_XLStruct.h"

// #include "CRCDataCtrl.h"

#include "HeaderPre.h"

//class CDBDoc;


class __MY_EXT_CLASS__ CRptSteelGirder_LSD12 : public CThread
{
public:
	CRptSteelGirder_LSD12();
	virtual ~CRptSteelGirder_LSD12();
	
	
protected:
	CDBDoc* m_pDoc;
	T_CPGD_D m_CpgdD;
	BOOL m_bStopExecute;
	CCRCDataCtrl* m_pDataCtrl;

	
public: 
	BOOL Execute_CSGReport_LSD12(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage);
	void Start(CString sFileName, int iPrintOpt, BOOL bSaveImage) {};
	void Start() {};

	void SetRptData(int ElemK, int nPosi, CSG_MEMB_POSD& InData, CSG_DRES_PRT& RptData);

protected:
	void SetPrintPosition(ElemPairK ElemK, BOOL& bPrintI, BOOL& bPrintJ);
	void SetSpanRptData(CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>& aSpanInData,   CArray<CSG_DRES_SPAN_PRT,  CSG_DRES_SPAN_PRT&>& aSpanRptData);
	void GetSpanRptDataBase(const EN_BEAM_DGN_TYPE& enBeamType, CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>& aSpanInData, CArray<CSG_DRES_SPAN_PRT, CSG_DRES_SPAN_PRT&>& aSpanRptData);
	void SetElemRptData(ElemPairK ElemK, int nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_DRES_PRT&  RptData);
	void SetRptData_SectStiffness(int nPosi,  DGN_SECT_STIFFNESS & InData, CSG_SectionProperty& RptData);
	BOOL ShouldCheckPosiIJ(int nFlag, BOOL& bCheckCalcI, BOOL& bCheckCalcJ);

	void ConvertCsgCalcUlmForce(T_CSGD_ULMF &For, CSG_ULMF_D &rData);
	void ConvertCsgCalcChkForce(T_CSGD_COMF &For, CSG_FORCE_COMP &rData);
	void ConvertCsgCalcChkForce4Stage(int nPos, _DGN_FORC_CRC &For, CSG_FORCE_COMP &rData);
	void ConvertCsgCalcForce( T_CSGD_FORCE &For, CSG_FORCE_D &rData);

	BOOL CheckSpanData(T_CSPR_D &CsprD);
	void GetUnBracedLenthData(ElemPairK ElemK, CSG_MEMB_ULEN &Ulen);	
	//void GetCsgUnbracedLengthData4CS(T_ELEM_K ElemK, int nPosiNeg, UINT StagK, UINT StepK,  CSG_MEMB_POSD_KSCE_LSD& MembLcomD);


	void SetFatigPrtData( T_CSFC_BASE &CsfcB, CSG_FATI_RES_KSCE_LSD12 &FatiResD, CSG_FATI_RES_KSCE_LSD12_PRT &FatiD);
	void SetShearConnectPrtData(  T_CSCR_BASE &CscrB,  CSG_CSCR_RES_KSCE_LSD12 &CscrResD);

	CStringW Get_StageName(T_STAG_K StagK);
	CStringW Get_LcomName(T_LCOM_K LcomK);
	CStringW Get_LcomType(int nMax);


};

#include "HeaderPost.h"

#endif // !defined(AFX_RptSteelGirder_LSD12_H__)
