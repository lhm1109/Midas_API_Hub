// DgnMomentCurvatureDesign.cpp: implementation of the CDgnMomentCurvatureDesign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnMomentCurvatureDesign.h"

#include <io.h> // for Message
#include "..\wg_base\MSOffice.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\MathFunc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\QSort.h"

#include "DgnSectionDrawWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMomentCurvatureDesign::CDgnMomentCurvatureDesign()
{
	m_arCalcNumPoint.RemoveAll();
	m_arCalcAngle.RemoveAll();
	m_arMPhiD.RemoveAll();
	m_arResult.RemoveAll();
	m_pDoc = CDBDoc::GetDocPoint();  
	m_nCode_Length = D_UNITSYS_LENGTH_INDEX_MM;
	m_nCode_Force  = D_UNITSYS_FORCE_INDEX_N;
}

CDgnMomentCurvatureDesign::~CDgnMomentCurvatureDesign()
{
	m_arCalcNumPoint.RemoveAll();
	m_arCalcAngle.RemoveAll();
	m_arMPhiD.RemoveAll();
	m_arResult.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////

BOOL CDgnMomentCurvatureDesign::CalcMomentCurvature(CArray<T_MPCC_K, T_MPCC_K>& KeyList)
{
	int i, nSize, nChk;	
	nSize = KeyList.GetSize();
	if(nSize == 0) return FALSE;

	T_UNIT_INDEX CodeUnit, CurrUnit;	
	// Code 단위계 변환 //////////////////////////////////////////////////////	
	CCurUnitSaver UnitSaver(TRUE);
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	CodeUnit = CurrUnit;		
	CodeUnit.nBase_Force  = m_nCode_Force;
	CodeUnit.nBase_Length = m_nCode_Length;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	//////////////////////////////////////////////////////////////////////////

	m_pDoc->m_pMatlDB->Create_RebarData(m_nCode_Length, m_nCode_Force, TRUE);
	
	BOOL bIsOK = TRUE;;
	T_MPCC_D MpccD;
	double dAxisAngle;
	int    nNumPoint;
	DGN_CALC_MPHI_SECT   MPhiD;
	DGN_CALC_MPHI_RESULT MPhiResD;
	for(i=0 ; i<nSize ; i++)
	{
		m_arCalcNumPoint.RemoveKey(KeyList[i]);
		m_arCalcAngle.RemoveKey(KeyList[i]);
		m_arMPhiD.RemoveKey(KeyList[i]);
		m_arResult.RemoveKey(KeyList[i]);

		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(KeyList[i], MpccD)) { bIsOK = FALSE;  continue; }
		nChk = CalcMomentCurvature(MpccD, MPhiD, dAxisAngle, nNumPoint, MPhiResD);
		if(nChk != 0) { bIsOK = FALSE;  continue; }
		
		m_arCalcNumPoint.SetAt(KeyList[i], nNumPoint);
		m_arCalcAngle.SetAt(KeyList[i], dAxisAngle);
		m_arMPhiD.SetAt(KeyList[i], MPhiD);
		m_arResult.SetAt(KeyList[i], MPhiResD);
	}
	
	// 단위계 복구 ///////////////////////////////////////////////////////////	
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	//////////////////////////////////////////////////////////////////////////	

	return bIsOK;
}

int CDgnMomentCurvatureDesign::CalcMomentCurvature(T_MPCC_D& MpccD, DGN_CALC_MPHI_RESULT& ResultD, int iIdx)
{
	T_MPCC_D MpccD_Code = MpccD;
	
	T_UNIT_INDEX CodeUnit, CurrUnit;	
	// Code 단위계 변환 //////////////////////////////////////////////////////	
	CCurUnitSaver UnitSaver(TRUE);
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	CodeUnit = CurrUnit;		
	CodeUnit.nBase_Force  = m_nCode_Force;
	CodeUnit.nBase_Length = m_nCode_Length;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	//////////////////////////////////////////////////////////////////////////
	
	m_pDoc->m_pMatlDB->Create_RebarData(m_nCode_Length, m_nCode_Force, TRUE);
	// Change Unit (CurrUnit -> CodeUnit).
	m_pDoc->m_pUnitCtrl->ConvertUnitMpccPrevious(MpccD_Code);

	DGN_CALC_MPHI_SECT MPhiD;
	double dAxisAngle;
	int    nNumPoint;
	int nChk = CalcMomentCurvature(MpccD_Code, MPhiD, dAxisAngle, nNumPoint, ResultD);	

	if(iIdx>=0)
	{
		m_arCalcNumPoint.SetAt(iIdx, nNumPoint);
		m_arCalcAngle.SetAt(iIdx, dAxisAngle);
		m_arMPhiD.SetAt(iIdx, MPhiD);
		m_arResult.SetAt(iIdx, ResultD);
	}
	// 단위계 복구 ///////////////////////////////////////////////////////////	
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	//////////////////////////////////////////////////////////////////////////	
	
	ConvValueTgtUnit2CurUnit(ResultD);

	return nChk;
}

int CDgnMomentCurvatureDesign::CalcMomentCurvature(T_MPCC_D& MpccD, DGN_CALC_MPHI_SECT& MPhiD, double& dAxisAngle, int& nNumPoint, DGN_CALC_MPHI_RESULT& ResultD)
{	
	T_ECUC_D EcucD;
	if(!m_pDoc->m_pAttrCtrl2->GetEcuc(EcucD)) EcucD.Initialize();	
	if(!m_DllManager.ConvertMpcc_ToEngine(MpccD, EcucD, m_nCode_Length, m_nCode_Force, MPhiD, dAxisAngle, nNumPoint)) return DGN_PMCV_ERR_MPHI_DATA;
	int nChk = m_DllManager.GCalc_MPhiCurve(dAxisAngle, nNumPoint, MPhiD, ResultD);
	return nChk;	
}

BOOL CDgnMomentCurvatureDesign::Get_MPhiResult(T_MPCC_K Key, DGN_CALC_MPHI_RESULT& ResultD)
{
	ResultD.Initialize();
	T_MPCC_D MpccD;
	T_ECUC_D EcucD;
	int    nNumPoint_Org, nNumPoint_Cal;
	double dAxisAngle_Org, dAxisAngle_Cal;
	DGN_CALC_MPHI_SECT MPhiD_Org, MPhiD_Cal;
	
	T_UNIT_INDEX CodeUnit, CurrUnit;	
	// Code 단위계 변환 //////////////////////////////////////////////////////	
	CCurUnitSaver UnitSaver(TRUE);
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	CodeUnit = CurrUnit;		
	CodeUnit.nBase_Force  = m_nCode_Force;
	CodeUnit.nBase_Length = m_nCode_Length;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	//////////////////////////////////////////////////////////////////////////
	if(!m_pDoc->m_pAttrCtrl2->GetMpcc(Key, MpccD)) return FALSE;
	if(!m_pDoc->m_pAttrCtrl2->GetEcuc(EcucD)) EcucD.Initialize();	

	m_pDoc->m_pMatlDB->Create_RebarData(m_nCode_Length, m_nCode_Force, TRUE);

	if(!m_DllManager.ConvertMpcc_ToEngine(MpccD, EcucD, m_nCode_Length, m_nCode_Force, MPhiD_Org, dAxisAngle_Org, nNumPoint_Org)) return FALSE;	
	// 단위계 복구 ///////////////////////////////////////////////////////////	
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	//////////////////////////////////////////////////////////////////////////	
	if(!m_arCalcNumPoint.Lookup(Key, nNumPoint_Cal)) return FALSE;
	if(!m_arCalcAngle.Lookup(Key, dAxisAngle_Cal)) return FALSE;
	if(!m_arMPhiD.Lookup(Key, MPhiD_Cal)) return FALSE;
	if(!m_arResult.Lookup(Key, ResultD)) return FALSE;

	if(nNumPoint_Org  != nNumPoint_Cal ) return FALSE;
	if(dAxisAngle_Org != dAxisAngle_Cal) return FALSE;
	if(!MPhiD_Org.IsEqual(MPhiD_Cal)) return FALSE;

	ConvValueTgtUnit2CurUnit(ResultD);

	return TRUE;
}

BOOL CDgnMomentCurvatureDesign::Get_EvgpMPhiResult(const int& iIdx, const T_MPCC_D& MpccD, DGN_CALC_MPHI_RESULT& ResultD)
{
	ResultD.Initialize();
	T_ECUC_D EcucD;
	int    nNumPoint_Org, nNumPoint_Cal;
	double dAxisAngle_Org, dAxisAngle_Cal;
	DGN_CALC_MPHI_SECT MPhiD_Org, MPhiD_Cal;

	T_UNIT_INDEX CodeUnit, CurrUnit;	
	// Code 단위계 변환 //////////////////////////////////////////////////////	
	CCurUnitSaver UnitSaver(TRUE);
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	CodeUnit = CurrUnit;		
	CodeUnit.nBase_Force  = m_nCode_Force;
	CodeUnit.nBase_Length = m_nCode_Length;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	//////////////////////////////////////////////////////////////////////////
	if(!m_pDoc->m_pAttrCtrl2->GetEcuc(EcucD)) EcucD.Initialize();	

	m_pDoc->m_pMatlDB->Create_RebarData(m_nCode_Length, m_nCode_Force, TRUE);

	if(!m_DllManager.ConvertMpcc_ToEngine(MpccD, EcucD, m_nCode_Length, m_nCode_Force, MPhiD_Org, dAxisAngle_Org, nNumPoint_Org)) return FALSE;	
	// 단위계 복구 ///////////////////////////////////////////////////////////	
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	//////////////////////////////////////////////////////////////////////////	
	if(!m_arCalcNumPoint.Lookup(iIdx, nNumPoint_Cal)) return FALSE;
	if(!m_arCalcAngle.Lookup(iIdx, dAxisAngle_Cal)) return FALSE;
	if(!m_arMPhiD.Lookup(iIdx, MPhiD_Cal)) return FALSE;
	if(!m_arResult.Lookup(iIdx, ResultD)) return FALSE;

	if(nNumPoint_Org  != nNumPoint_Cal ) return FALSE;
	if(dAxisAngle_Org != dAxisAngle_Cal) return FALSE;
	if(!MPhiD_Org.IsEqual(MPhiD_Cal)) return FALSE;

	ConvValueTgtUnit2CurUnit(ResultD);

	return TRUE;
}

int  CDgnMomentCurvatureDesign::Get_MPhiResultKeyList(CArray<T_MPCC_K, T_MPCC_K>& KeyList)
{
	KeyList.RemoveAll();

	DGN_CALC_MPHI_RESULT ResultD;
	T_MPCC_K Key;
	POSITION Pos = m_arResult.GetStartPosition();	
	while(Pos)
	{
		m_arResult.GetNextAssoc(Pos,Key,ResultD);
		if(Get_MPhiResult(Key, ResultD) == TRUE) KeyList.Add(Key);
	}

	// Sorting
	int nSize = KeyList.GetSize();
	if(nSize >= 2)
	{		
		int* pKeyList = new int[nSize];
		for(int i=0 ; i<nSize ; i++) pKeyList[i] = KeyList[i];
		CQSort::QSortInt(pKeyList, nSize);
		for(int i=0 ; i<nSize ; i++) KeyList[i] = pKeyList[i];
		delete[] pKeyList;
	}

	return nSize;
}

BOOL CDgnMomentCurvatureDesign::Print_MPhiResult(CArray<T_MPCC_K, T_MPCC_K>& KeyList)
{
	int i, nSize;
	T_MPCC_D MpccD;
	DGN_CALC_MPHI_SECT   MPhiD;
	DGN_CALC_MPHI_RESULT ResultD;

	nSize = KeyList.GetSize();
	for(i=nSize-1 ; i>=0 ; i--)
	{
		if(!Get_MPhiResult(KeyList[i], ResultD)) KeyList.RemoveAt(i);
	}

	nSize = KeyList.GetSize();
	if(nSize == 0)
	{
		AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_NOTPRINT_REPORT));
		return FALSE;	
	}
	
    bool bRTF = CDBLib::IsWordRTF();
	CString strExt    = CMSOffice::GetExtensionName( bRTF ? EXT_WORDRTF : EXT_WORD);
	CString strDotExt = _T(".") + strExt;
	CString strFileFormat = _T("");
	strFileFormat.Format(_T("Word File(*.%s)|*.%s|All File(*.*)|*.*||"), strExt, strExt);

	CString csFileName = CDBLib::MakeFileNameWithNewExt(strDotExt);
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;

	CFileDialog fd(FALSE, strDotExt, szFileName,
								 OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
								 strFileFormat, NULL);
	if (fd.DoModal() == IDOK)
	{
		DGN_REPORT_OPTION PrintOption;
        PrintOption.Initialize();

		CArray<DGN_REPORT_MPHI_RESULT, DGN_REPORT_MPHI_RESULT&> arReportD;
		CArray<CString, CString&> arFigFileName;

		int nOutType;
		BOOL bRunMS, bRowMark, bColumnMark;
		CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);

		CString strFileName = fd.GetPathName();

		for(i=0 ; i<100 ; i++)
		{
			if(access(CW2A(strFileName), 0) == 0)	remove(CW2A(strFileName));
			else break;

			if(access(CW2A(strFileName), 0) == 0)	
			{
				CFileCtrl fCtrl(strFileName);
				strFileName.Format(_T("%s_%d.%s"), fCtrl.GetFilePathAndNameWithoutExtension(), i+1, fCtrl.GetFileExtension());
			}
			else break;
		}
		if(access(CW2A(strFileName), 0) == 0)
		{
			AfxMessageBox(_LS(IDS_DGN_MPHI_CURVE_NOTCREATE_REPORT));
			return FALSE;
		}		

		int HSize = 6000;
		int BSize = 8000;
		TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
		CRect canvas(0, 0, BSize, HSize);

		
		CFileCtrl fCtrl(strFileName);
		CString strFileNameWithoutEx = fCtrl.GetFilePathAndNameWithoutExtension();
		CString strSectFigFileName, strChartFigFileName;

		T_UNIT_INDEX CodeUnit, CurrUnit, PrintUnit;			
		// Code 단위계 변환 //////////////////////////////////////////////////////	
		CCurUnitSaver UnitSaver(TRUE);
		m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
		CodeUnit = PrintUnit = CurrUnit;		
		CodeUnit.nBase_Force  = m_nCode_Force;
		CodeUnit.nBase_Length = m_nCode_Length;
		PrintUnit.nBase_Force  = CurrUnit.nBase_Force ;//D_UNITSYS_FORCE_INDEX_KN;
		PrintUnit.nBase_Length = CurrUnit.nBase_Length;//D_UNITSYS_LENGTH_INDEX_M;
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(PrintUnit);
		//////////////////////////////////////////////////////////////////////////

		nSize = KeyList.GetSize();		
		DGN_REPORT_MPHI_RESULT ReportD;
		for(i=0 ; i<nSize ; i++)
		{
			// 삽도용 정보는 PrintUnit단위로 받아 옴
			if(!m_pDoc->m_pAttrCtrl2->GetMpcc(KeyList[i], MpccD)) continue;
			if(!Get_MPhiResult(KeyList[i], ResultD)) continue;
			strSectFigFileName.Format(_T("%s_%s_Sect.emf"), strFileNameWithoutEx, MpccD.strName);
			strChartFigFileName.Format(_T("%s_%s_Chart.emf"), strFileNameWithoutEx, MpccD.strName);
			if(!Make_SectionFig(strSectFigFileName, canvas, &MpccD)) continue;
			if(!Make_MPhiCurveFig(strChartFigFileName, canvas, &ResultD, 0)) continue;
			arFigFileName.Add(strSectFigFileName);
			arFigFileName.Add(strChartFigFileName);

			// DGN_REPORT_MPHI_RESULT용 정보는 Code 단위로계 받아옴
			if(!m_arMPhiD.Lookup(KeyList[i], MPhiD)) continue;
			if(!m_arResult.Lookup(KeyList[i], ResultD)) continue;
			if(!m_DllManager.ConvertResult_ToReportDLL(MPhiD, ResultD, strSectFigFileName, strChartFigFileName, ReportD)) continue;
			arReportD.Add(ReportD);
		}

		// 단위계 복구 ///////////////////////////////////////////////////////////	
		m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
		//////////////////////////////////////////////////////////////////////////	

		PrintOption.strPath   = strFileName;
		PrintOption.iPrintOpt = 0;
		PrintOption.nRecType  = nOutType;
		PrintOption.bIsView   = TRUE;
#if(_DEBUG)
		PrintOption.bIsView   = FALSE; // Debug시에는 DgnEngine에서 Open상태로 종료되므로
#endif		
		PrintOption.nForceUnit_Data   = CodeUnit.nBase_Force;
		PrintOption.nLengthUnit_Data  = CodeUnit.nBase_Length;
		PrintOption.nForceUnit_Print  = PrintUnit.nBase_Force;
		PrintOption.nLengthUnit_Print = PrintUnit.nBase_Length;
        PrintOption.bRTF              = bRTF;

		BOOL bChk = m_DllManager.GPrint_MPhiCurveReport(PrintOption, arReportD);
		nSize = arFigFileName.GetSize();
		for(i=0 ; i<nSize ; i++)
		{
			if(access(CW2A(arFigFileName[i]), 0) == 0) remove(CW2A(arFigFileName[i]));
		}

		return bChk;
	}
	
	return FALSE;
}

// For Civil Seismic Evaluation
BOOL CDgnMomentCurvatureDesign::Print_EvgpMPhiResult(CString strFileName, CArray<T_MPCC_D, T_MPCC_D&>& arMpccD)
{
	// 주의사항
	// 1. T_MPCC_D의 bEvgp가 TRUE인 경우
	//    dAxisAngle 값과 상관없이 0도, 90도 일 때의 두 가지 경우를 출력.
	// 2. T_MPCC_D의 bEvgp가 FALSE인 경우
	//    dAxisAngle, +90도 일 때의 두 가지 경우를 출력.
	int nOutType;
	BOOL bRunMS, bRowMark, bColumnMark;
	CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);

	int HSize = 6000;
	int BSize = 8000;
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CRect canvas(0, 0, BSize, HSize);

	CFileCtrl fCtrl(strFileName);
	CString strFileNameWithoutEx = fCtrl.GetFilePathAndNameWithoutExtension();
	CString strSectFigFileName, strChartFigFileName;

	double dFor = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_FORCE, 1.0);

	T_UNIT_INDEX CodeUnit, CurrUnit, PrintUnit;			
	// Code 단위계 변환 //////////////////////////////////////////////////////	
	//CCurUnitSaver UnitSaver(TRUE);
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	CodeUnit = PrintUnit = CurrUnit;		
	CodeUnit.nBase_Force  = m_nCode_Force;
	CodeUnit.nBase_Length = m_nCode_Length;
	PrintUnit.nBase_Force  = CurrUnit.nBase_Force ;//D_UNITSYS_FORCE_INDEX_KN;
	PrintUnit.nBase_Length = CurrUnit.nBase_Length;//D_UNITSYS_LENGTH_INDEX_M;
	//m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(PrintUnit);
	//////////////////////////////////////////////////////////////////////////

	CArray<DGN_REPORT_MPHI_RESULT, DGN_REPORT_MPHI_RESULT&> arReportD;
	CArray<CString, CString&> arFigFileName;
	arReportD.RemoveAll();
	arFigFileName.RemoveAll();

	int nSize = arMpccD.GetSize();
	for(int i=0 ; i<nSize ; i++)
	{
		T_MPCC_D MpccD = arMpccD[i]; 
		MpccD.dAxialForce *= dFor; // code unit

		// 삽도용 정보는 PrintUnit단위로 받아 옴
		DGN_CALC_MPHI_RESULT ResultD;
		if(!Get_EvgpMPhiResult(i, MpccD, ResultD)) continue;
		strSectFigFileName.Format(_T("%s_%s_Sect%d.emf"), strFileNameWithoutEx, MpccD.strName,i);
		strChartFigFileName.Format(_T("%s_%s_Chart%d.emf"), strFileNameWithoutEx, MpccD.strName,i);
		if(!Make_SectionFig(strSectFigFileName, canvas, &MpccD)) continue;
		if(!Make_MPhiCurveFig(strChartFigFileName, canvas, &ResultD, 0)) continue;
		arFigFileName.Add(strSectFigFileName);
		arFigFileName.Add(strChartFigFileName);

		// DGN_REPORT_MPHI_RESULT용 정보는 Code 단위로계 받아옴
		DGN_CALC_MPHI_SECT   MPhiD;      
		if(!m_arMPhiD.Lookup(i, MPhiD)) continue;
		if(!m_arResult.Lookup(i, ResultD)) continue;
		DGN_REPORT_MPHI_RESULT ReportD;
		if(!m_DllManager.ConvertResult_ToReportDLL(MPhiD, ResultD, strSectFigFileName, strChartFigFileName, ReportD)) continue;
		arReportD.Add(ReportD);
	}

	// 단위계 복구 ///////////////////////////////////////////////////////////	
	//m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	//////////////////////////////////////////////////////////////////////////	
    
    DGN_REPORT_OPTION PrintOption;
    PrintOption.Initialize();
	PrintOption.strPath   = strFileName;
	PrintOption.iPrintOpt = 0;
	PrintOption.nRecType  = nOutType;
	PrintOption.bIsView   = TRUE;
#if(_DEBUG)
	PrintOption.bIsView   = FALSE; // Debug시에는 DgnEngine에서 Open상태로 종료되므로
#endif		
	PrintOption.nForceUnit_Data   = CodeUnit.nBase_Force;
	PrintOption.nLengthUnit_Data  = CodeUnit.nBase_Length;
	PrintOption.nForceUnit_Print  = PrintUnit.nBase_Force;
	PrintOption.nLengthUnit_Print = PrintUnit.nBase_Length;
    PrintOption.bRTF              = CDBLib::IsWordRTF();

	BOOL bChk = m_DllManager.GPrint_MPhiCurveReport(PrintOption, arReportD);
	nSize = arFigFileName.GetSize();
	for(int i=0 ; i<nSize ; i++)
	{
		if(access(CW2A(arFigFileName[i]), 0) == 0) remove(CW2A(arFigFileName[i]));
	}

	return bChk;
}

BOOL CDgnMomentCurvatureDesign::Draw_Section(CDC* pDC, CRect canvas, T_MPCC_D* pMpccD, BOOL bEMF)
{
	// Check!!! Mander모델을 위해서 추가작업 필요
	int  nTextSize = 9;
	int  nType = 1; //Column
	BOOL bDimShape = FALSE;
	BOOL bDimRebar = FALSE;
	BOOL bFillShape = TRUE;
	BOOL bFillRebar = TRUE;
	int  nIMJ = 0;
	
	BOOL bShape = FALSE;
	BOOL bRebar = FALSE;
	BOOL bGen   = FALSE;

	T_SECT_D SectD;  SectD.Initialize();
	T_RCHK_D RchkD;  RchkD.Initialize();
	T_REBT_D RebtD;  RebtD.Initialize();	
	if(m_pDoc->m_pAttrCtrl->GetSectDesign(pMpccD->SectK, SectD))
	{
		if(SectD.nStype == D_SECT_TYPE_REGULAR || SectD.nStype == D_SECT_TYPE_TAPERED)
		{
			if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_B   || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_P    || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SR   || 
				 SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SB  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_OCT  || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_SOCT ||
				 SectD.SectBefore.Shape == D_SECT_SHAPE_REG_TRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_STRK || SectD.SectBefore.Shape == D_SECT_SHAPE_REG_HTRK)
			{ bShape = TRUE;  bGen = FALSE; }
			else if(SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
			{ bShape = TRUE;  bGen = TRUE; }
		}
		else if(SectD.nStype == D_SECT_TYPE_USER && SectD.SectBefore.Shape == D_SECT_SHAPE_REG_GEN)
		{ bShape = TRUE;  bGen = TRUE; }
	}

	if(bShape)
	{
		if(!bGen)
		{ bRebar = m_pDoc->m_pAttrCtrl->GetRchk(pMpccD->SectK, RchkD); }
		else
		{ bRebar = m_pDoc->m_pAttrCtrl->GetRebt(pMpccD->SectK, RebtD); }
	}



	DGN_GSEC_POLYGON_LIST aCoreOuterPolygon;  aCoreOuterPolygon.RemoveAll();
	DGN_GSEC_POLYGON_LIST aCoreInnerPolygon;  aCoreInnerPolygon.RemoveAll();

	
	if(bRebar)
	{	
		T_FIMP_D FimpConcD1;  FimpConcD1.Initialize();
		T_FIMP_D FimpConcD2;  FimpConcD2.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetFimp(pMpccD->MatlConcK1, FimpConcD1)) return FALSE;
		if(FimpConcD1.nMaterialType == 0 && FimpConcD1.nHysModelType == 6)
		{ 
			if(m_pDoc->m_pAttrCtrl->GetFimp(pMpccD->MatlConcK2, FimpConcD2))
			{		
				if(FimpConcD2.nMaterialType == 0 && FimpConcD2.nHysModelType == 6)
				{
					if(FimpConcD2.PROP.CON_MANDER.nConcType == 1)
					{
						DGN_GSEC_SHAPE_D rSection;
						DGN_GSEC_REBAR_COLUMN rRebar;

						T_UNIT_INDEX CurrUnit;	
						m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
						m_pDoc->m_pMatlDB->Create_RebarData(CurrUnit.nBase_Length, CurrUnit.nBase_Force, TRUE);

						// 단면 정보
						m_DrawSection.ConvertToDLL_SectData(SectD, nIMJ, rSection);	
						// 철근 정보
						if(bGen) m_DllManager.ConvertToDLL_RebarData(SectD, nIMJ, RebtD, rRebar);
						else     m_DllManager.ConvertToDLL_RebarData(SectD, nIMJ, RchkD.COLM, rRebar);

						if(!m_DllManager.GCalc_ConfinedConcretePolygonData_Column(rSection, rRebar, aCoreOuterPolygon, aCoreInnerPolygon, -rSection.dYBar, -rSection.dZBar))
						{ aCoreOuterPolygon.RemoveAll();  aCoreInnerPolygon.RemoveAll(); }
					}
				}
			}
		}
	}

	_DGN_DRAW_SECT_OPTION optionD;  optionD.Initialize();
	optionD.CanvasType = !bEMF ? 0 : 1;
	optionD.Canvas = canvas;
	optionD.bReDraw = TRUE;
	optionD.bRebar  = bRebar;
	optionD.DimSize = !bEMF ? 10 : max(10, min(canvas.Height(), canvas.Width())/30);
	optionD.RebarSize = !bEMF ? 4 : max(4, min(canvas.Height(), canvas.Width())/75);

	optionD.AxisType = 2;
	// MQC 8776 JHYUN 20141217
	//optionD.RotAngle = CMathFunc.m_trrad * pMpccD->dAxisAngle;
	
	m_DrawSection.Draw_Section_RcColumn_For_SectK(pDC, pMpccD->SectK, pMpccD->nPosition, optionD, TRUE, aCoreOuterPolygon, aCoreInnerPolygon);

	return TRUE;
}

BOOL CDgnMomentCurvatureDesign::Draw_MPhiCurve(CDC* pDC, CRect canvas, DGN_CALC_MPHI_RESULT* pResultD, int nType, BOOL bEMF)
{
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);

	CDgn_DrawManager DrawManager;
	DGNDRAW_MPHI_MULTI_CHART_DATA   MPhiChart;     MPhiChart.Initialize();
	DGNDRAW_MPHI_MULTI_CHART_OPTION ChartOption;   ChartOption.Initialize();			
	ChartOption.CanvasType = !bEMF ? 0 : 1;
	ChartOption.Canvas = canvas;
	ChartOption.nFontSize = !bEMF ? 12 : max(12, min(canvas.Height(), canvas.Width())/30);
	ChartOption.nForceUnit_Data  = CurIndex.nBase_Force;
	ChartOption.nLengthUnit_Data = CurIndex.nBase_Length;
	ChartOption.nForceUnit_Draw  = CurIndex.nBase_Force;
	ChartOption.nLengthUnit_Draw = CurIndex.nBase_Length;
	ChartOption.nCurDecimal      = -1;
	ChartOption.nMomentDecimal   = 0;
	ChartOption.nLengthDecimal   = -1;
	ChartOption.nStrainDecimal   = 4;	
	CStringW strWCurvature(_LS(IDS_DGN_MPHI_CURVE_CURVATURE) + _T(" x10^-3$"));
	ChartOption.strCurvature     = strWCurvature;
	ChartOption.dTopMargin       = 0.05;
	ChartOption.dBottomMargin    = 0.20;
	ChartOption.dLeftMargin      = !bEMF ? 0.30 : 0.25;
	ChartOption.dRightMargin     = 0.05;
	
	MPhiChart.bDuplex = !(pResultD->bSymmetry);
	MPhiChart.bDisplayIdeal = pResultD->bIsIdeal;
	MPhiChart.nCurveType    = nType;

	if(pResultD->bIsIdeal)  MPhiChart.bUseultimateIdeal = TRUE;
	
	MPhiChart.dMom_P[0] = pResultD->MPhi_c[0].dMoment;   MPhiChart.dPhi_P[0] = pResultD->MPhi_c[0].dCurvature *1000.0;
	MPhiChart.dMom_P[1] = pResultD->MPhi_y0[0].dMoment;  MPhiChart.dPhi_P[1] = pResultD->MPhi_y0[0].dCurvature*1000.0;
	MPhiChart.dMom_P[2] = pResultD->MPhi_y[0].dMoment;   MPhiChart.dPhi_P[2] = pResultD->MPhi_y[0].dCurvature *1000.0;
	MPhiChart.dMom_P[3] = pResultD->MPhi_u[0].dMoment;   MPhiChart.dPhi_P[3] = pResultD->MPhi_u[0].dCurvature *1000.0;
	MPhiChart.dMom_P[4] = pResultD->MPhi_yI[0].dMoment;  MPhiChart.dPhi_P[4] = pResultD->MPhi_yI[0].dCurvature*1000.0;
	MPhiChart.dMom_P[5] = pResultD->MPhi_uI[0].dMoment;  MPhiChart.dPhi_P[5] = pResultD->MPhi_uI[0].dCurvature*1000.0;
	MPhiChart.dPhi_yI_Max_P = pResultD->dMPhi_yI_CurvatureMax[0]*1000.0;
		
	MPhiChart.dMom_N[0] = pResultD->MPhi_c[1].dMoment;   MPhiChart.dPhi_N[0] = pResultD->MPhi_c[1].dCurvature *1000.0;
	MPhiChart.dMom_N[1] = pResultD->MPhi_y0[1].dMoment;  MPhiChart.dPhi_N[1] = pResultD->MPhi_y0[1].dCurvature*1000.0;
	MPhiChart.dMom_N[2] = pResultD->MPhi_y[1].dMoment;   MPhiChart.dPhi_N[2] = pResultD->MPhi_y[1].dCurvature *1000.0;
	MPhiChart.dMom_N[3] = pResultD->MPhi_u[1].dMoment;   MPhiChart.dPhi_N[3] = pResultD->MPhi_u[1].dCurvature *1000.0;
	MPhiChart.dMom_N[4] = pResultD->MPhi_yI[1].dMoment;  MPhiChart.dPhi_N[4] = pResultD->MPhi_yI[1].dCurvature*1000.0;
	MPhiChart.dMom_N[5] = pResultD->MPhi_uI[1].dMoment;  MPhiChart.dPhi_N[5] = pResultD->MPhi_uI[1].dCurvature*1000.0;
	MPhiChart.dPhi_yI_Max_N = pResultD->dMPhi_yI_CurvatureMax[1]*1000.0;
	

	int i, nSize;
	nSize = pResultD->arCurvePos.GetSize();
	MPhiChart.arMultiPoint_P.SetSize(nSize);
	for(i=0 ; i<nSize ; i++)
	{
		MPhiChart.arMultiPoint_P[i].dCurvature   = pResultD->arCurvePos[i].dCurvature*1000.0;
		MPhiChart.arMultiPoint_P[i].dMoment      = pResultD->arCurvePos[i].dMoment;
		MPhiChart.arMultiPoint_P[i].dNeutralAxis = pResultD->arCurvePos[i].dNeutralAxis;
		MPhiChart.arMultiPoint_P[i].dConcStrain  = pResultD->arCurvePos[i].dConcStrain[0];
		MPhiChart.arMultiPoint_P[i].dSteelStrain = (-1.0)*pResultD->arCurvePos[i].dRebarStrain[1];
		MPhiChart.arMultiPoint_P[i].dPn_ConcComp    = pResultD->arCurvePos[i].dPn_ConcComp;
		MPhiChart.arMultiPoint_P[i].dPn_SteelComp   = pResultD->arCurvePos[i].dPn_RebarComp;
		MPhiChart.arMultiPoint_P[i].dPn_SteelTen    = (-1.0)*pResultD->arCurvePos[i].dPn_RebarTen;
	}
	if(MPhiChart.bDuplex)
	{
		nSize = pResultD->arCurveNeg.GetSize();
		MPhiChart.arMultiPoint_N.SetSize(nSize);
		for(i=0 ; i<nSize ; i++)
		{
			MPhiChart.arMultiPoint_N[i].dCurvature   = pResultD->arCurveNeg[i].dCurvature*1000.0;
			MPhiChart.arMultiPoint_N[i].dMoment      = pResultD->arCurveNeg[i].dMoment;
			MPhiChart.arMultiPoint_N[i].dNeutralAxis = pResultD->arCurveNeg[i].dNeutralAxis;
			MPhiChart.arMultiPoint_N[i].dConcStrain  = pResultD->arCurveNeg[i].dConcStrain[0];
			MPhiChart.arMultiPoint_N[i].dSteelStrain = (-1.0)*pResultD->arCurveNeg[i].dRebarStrain[1];
			MPhiChart.arMultiPoint_N[i].dPn_ConcComp    = pResultD->arCurveNeg[i].dPn_ConcComp;
			MPhiChart.arMultiPoint_N[i].dPn_SteelComp   = pResultD->arCurveNeg[i].dPn_RebarComp;
			MPhiChart.arMultiPoint_N[i].dPn_SteelTen    = (-1.0)*pResultD->arCurveNeg[i].dPn_RebarTen;
		}
	}

	
	return DrawManager.Draw_Chart_MPhi_Multi(pDC, MPhiChart, ChartOption);
}


BOOL CDgnMomentCurvatureDesign::Make_SectionFig(CString strFileName, CRect canvas, T_MPCC_D* pMpccD)
{
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CMetaFileDC metaDC;


	metaDC.CreateEnhanced(NULL, strFileName , canvas, szDesc);
	metaDC.SetMapMode(MM_HIMETRIC);	

	BOOL bChk = Draw_Section(&metaDC, canvas, pMpccD, TRUE);
	
	HENHMETAFILE hMetaF = metaDC.CloseEnhanced();
	::DeleteEnhMetaFile(hMetaF);		
	
	return bChk;
}
BOOL CDgnMomentCurvatureDesign::Make_MPhiCurveFig(CString strFileName, CRect canvas, DGN_CALC_MPHI_RESULT* pResultD, int nType)
{
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CMetaFileDC metaDC;


	metaDC.CreateEnhanced(NULL, strFileName , canvas, szDesc);	
	metaDC.SetMapMode(MM_HIMETRIC);	

	BOOL bChk = Draw_MPhiCurve(&metaDC, canvas, pResultD, nType, TRUE);
	
	HENHMETAFILE hMetaF = metaDC.CloseEnhanced();
	::DeleteEnhMetaFile(hMetaF);		
	
	return bChk;
}

//////////////////////////////////////////////////////////////////////////
void CDgnMomentCurvatureDesign::ConvValueTgtUnit2CurUnit(DGN_CALC_MPHI_RESULT& ResultD)
{
	double dFactor_Force  = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_FORCE,  1.0);
	double dFactor_Area   = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_AREA, 1.0);
	double dFactor_Length = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_LENGTH, 1.0);
	ResultD.dArea_Rebar *= dFactor_Area;
	
	int i, nSize;
	for(i=0 ; i<2 ; i++)
	{
		ConvValueTgtUnit2CurUnit(ResultD.MPhi_c[i]);
		ConvValueTgtUnit2CurUnit(ResultD.MPhi_y0[i]);
		ConvValueTgtUnit2CurUnit(ResultD.MPhi_y[i]);
		ConvValueTgtUnit2CurUnit(ResultD.MPhi_yI[i]);
		ConvValueTgtUnit2CurUnit(ResultD.MPhi_u[i]);
		ConvValueTgtUnit2CurUnit(ResultD.MPhi_uI[i]);
		ResultD.dMPhi_yI_CurvatureMax[i] *= (1.0/dFactor_Length);
	}

	nSize = ResultD.arCurvePos.GetSize();
	for(i=0 ; i<nSize ; i++)
		ConvValueTgtUnit2CurUnit(ResultD.arCurvePos[i]);
	
	nSize = ResultD.arCurveNeg.GetSize();
	for(i=0 ; i<nSize ; i++)
		ConvValueTgtUnit2CurUnit(ResultD.arCurveNeg[i]);

	ResultD.dLimitAxialForce *= dFactor_Force;	
}
void CDgnMomentCurvatureDesign::ConvValueTgtUnit2CurUnit(DGN_CALC_MPHI_UNIT_RESULT& ResultUnitD)
{
	double dFactor_Length = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_LENGTH, 1.0);
	double dFactor_Stress = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_STRESS, 1.0);
	double dFactor_Force  = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_FORCE,  1.0);
	double dFactor_Moment = CUnitCtrl::ConvValueTgtUnit2CurUnit(m_nCode_Force, m_nCode_Length, D_UNITSYS_BASE_MOMENT, 1.0);

	ResultUnitD.dCurvature *= (1.0/dFactor_Length);
	ResultUnitD.dAxisForce *= dFactor_Force;	
	ResultUnitD.dMoment    *= dFactor_Moment;	
	ResultUnitD.dConcComp  *= dFactor_Length;
	ResultUnitD.dSteelComp *= dFactor_Length;
	ResultUnitD.dSteelTen  *= dFactor_Length;
	ResultUnitD.dRebarComp *= dFactor_Length;
	ResultUnitD.dRebarTen  *= dFactor_Length;
}

void CDgnMomentCurvatureDesign::Set_DefaultData(T_MPCC_K Key, DGN_CALC_MPHI_RESULT& ResultD)
{
	ResultD.Initialize();
	ResultD.MPhi_c[0].dCurvature  = 0.00001;  ResultD.MPhi_c[0].dMoment  = 200.0*((double)Key);
	ResultD.MPhi_y0[0].dCurvature = 0.00004;  ResultD.MPhi_y0[0].dMoment = 400.0*((double)Key);
	ResultD.MPhi_y[0].dCurvature  = 0.00006;  ResultD.MPhi_y[0].dMoment  = 450.0*((double)Key);
	ResultD.MPhi_yI[0].dCurvature = 0.00003;  ResultD.MPhi_yI[0].dMoment = 425.0*((double)Key);
	ResultD.MPhi_u[0].dCurvature  = 0.00015;  ResultD.MPhi_u[0].dMoment  = 500.0*((double)Key);
	ResultD.MPhi_uI[0].dCurvature = 0.00015;  ResultD.MPhi_uI[0].dMoment = 425.0*((double)Key);

	ResultD.MPhi_c[1].dCurvature  = -0.00001;  ResultD.MPhi_c[1].dMoment  = -100.0*((double)Key);
	ResultD.MPhi_y0[1].dCurvature = -0.00004;  ResultD.MPhi_y0[1].dMoment = -300.0*((double)Key);
	ResultD.MPhi_y[1].dCurvature  = -0.00006;  ResultD.MPhi_y[1].dMoment  = -350.0*((double)Key);
	ResultD.MPhi_yI[1].dCurvature = -0.00003;  ResultD.MPhi_yI[1].dMoment = -325.0*((double)Key);
	ResultD.MPhi_u[1].dCurvature  = -0.00015;  ResultD.MPhi_u[1].dMoment  = -400.0*((double)Key);
	ResultD.MPhi_uI[1].dCurvature = -0.00015;  ResultD.MPhi_uI[1].dMoment = -325.0*((double)Key);
}
