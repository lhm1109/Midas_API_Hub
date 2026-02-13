#include "stdafx.h"

#include "BrdgSeisEvalKISTEC2015_Report.h"

#include "..\wg_base\Product.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_dgn\DgnProgressDlg.h"

#include "..\wg_db\BrdgSeisEvalResult.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_MathFunc.h"

#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\VehlDB.h"
#include "..\wg_base\PL_StrParser.h"
#include "..\wg_cmd\CMMvcdDlg.h"

#include "..\wg_main\wg_main.h" 
#include "..\wg_base\ServiceProvider.h" 
#include "..\wg_gps\GPS_ServiceDef.h" 

#include "..\wg_base\wg_base_ArrayUtil.h"

#include "DgnSeisAnchor2FailDlg.h"
#include "DgnSeisBearingPropItemDlg.h"

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include <fstream.h>	// for rptwofstream
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, int& iSheetIdx, CArray<UINT, UINT>& arEvgpBearing, CArray<UINT, UINT>& raBrev)
{
	CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if ( !pResult ) { ASSERT(0); }

	T_BSED_D BsedD;
	if ( !m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD) ) ASSERT(0);

	int iIdx = 1;
	INT_PTR nBearing = arEvgpBearing.GetSize();
	if ( nBearing != raBrev.GetSize() ) {
		ASSERT(0);
		return TRUE;
	}

	for ( INT_PTR i = 0; i < nBearing; ++i )
	{
		T_BREV_K BearingK = raBrev.GetAt(i);
		T_BRDGEVAL_BEARING ResD;
		if ( !pResult->ReadSeisEvalResultBearing(BearingK, 0, TRUE, ResD) ) continue;
		if ( !ResD.bCheck ) continue;

		T_BREV_D BrevD;
		if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BearingK, BrevD) ) continue;;

		// set sheet
		SetBearingEvaluationSheetPage(pXL, BrevD.strName, iSheetIdx, iIdx);

		// set head
		int stRow = 1;
		CString RptBlock;
		short Row_Ref = 0, Col_Ref = 0;
		switch ( ResD.ParamD.enBearing )
		{
		case BEARING_STEEL_POTo:
			RptBlock = _T("[7_0]Title_Bearing_Port"); // 포트받침
			break;
		case BEARING_STEEL_POTx:
			RptBlock = _T("[7_0]Title_Bearing_Port"); // 포트받침
			break;
		case BEARING_STEEL_HBsC:
			RptBlock = _T("[7_0]Title_Bearing_Blass");// 고력황동받침
			break;
			//case BEARING_ELAST:
			//    RptBlock = _T("[7_0]Title_Bearing_Elastic");// 탄성받침
			//    break;
		default:
			ASSERT(0);
			break;
		}
		int rowCnt = CopyRange(pXL, RptBlock, stRow);
		stRow += rowCnt;

		// 7.1 Bearing Body
		if ( !Print_BearingBody(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;
		// 7.2 앵커부 평가 검토
		//  1) 받침제원
		//  2) 강재파괴

		if ( !Print_BearingAnchorStl(pXL, pProDlg, nProCnt, nProTot, BearingK, ResD, stRow) ) return FALSE;

		//  3) 콘크리트 파괴
		if ( !Print_BearingAnchorConc(pXL, pProDlg, nProCnt, nProTot, BearingK, ResD, stRow) ) return FALSE;
		//  4) 콘크리트 프라이아웃
		if ( !Print_BearingAnchorPryout(pXL, pProDlg, nProCnt, nProTot, BearingK, ResD, stRow) ) return FALSE;

		// KISTEC2019 인장검토
		if ( BsedD.nDesignCode == KISTEC_2019 || BsedD.nDesignCode == KALIS_2023)
		{
			if ( ResD.AnchorD[0].bCheckN || ResD.AnchorD[1].bCheckN )
			{
				// 5) 인장을 받는 앵커의 강재파괴
				if ( !Print_BearingAnchorTenStl(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;

				// 6) 인장을 받는 앵커의 콘크리트 파괴
				if ( !Print_BearingAnchorTenConc(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;

				// 7) 인장을 받는 앵커의 뽑힘
				if ( !Print_BearingAnchorTenPull(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;

				// 8) 인장을 받는 앵커의 콘크리트측면 파열
				if ( !Print_BearingAnchorTenSide(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;

				// 9) 인장과 전단의 상관 작용
				if ( !Print_BearingAnchorTenRela(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;
			}
		}

		// 7.3 받침부 평가 요약
		if ( !Print_BearingAnchorSummary(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;

		pXL->SetHPageBreak(53, stRow, 28);
	}
	if ( iIdx > 1 ) iSheetIdx++;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingSuppLengTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot, int& iSheetIdx, CArray<UINT, UINT>& arEvgpBearing, CArray<UINT, UINT>& raBrev)
{
	CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if ( !pResult ) { ASSERT(0); }

	INT_PTR nBearing = arEvgpBearing.GetSize();
	if ( nBearing == 0 ) return TRUE;
	if ( nBearing != raBrev.GetSize() ) {
		ASSERT(0);
		return TRUE;
	}

	CArray<_DGN_SEISEVAL_SuppLengD, _DGN_SEISEVAL_SuppLengD&> arResD;
	arResD.RemoveAll();

	int iIdx = 0;
	int stRow = 1;
	CString RptBlock;
	short Row_Ref = 0, Col_Ref = 0;
	for ( INT_PTR i = 0; i < nBearing; ++i )
	{
		T_BREV_K BearingK = raBrev.GetAt(i);

		BOOL bChk = TRUE;
		T_BRDGEVAL_BEARING ResD;
		if ( !pResult->ReadSeisEvalResultBearing(BearingK, 0, TRUE, ResD) ) bChk = FALSE;
		if ( !ResD.bCheck )  bChk = FALSE;
		if ( !ResD.SuppLengD.bCheck )  bChk = FALSE;
		if ( !bChk )
		{
			if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }
			continue;
		}
		iIdx++;
		if ( iIdx == 1 )
		{
			// set sheet
			SetBearingLengthEvaluationSheetPage(pXL, iSheetIdx);
		}

		T_BREV_D BrevD;
		if ( !m_pDoc->m_pAttrCtrl2->GetBrev(BearingK, BrevD) ) continue;;

		// set head
		RptBlock = _T("[8_x]Title_Pier_SupLength");
		int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		CString strName;
		strName.Format(_T("8.%d %s 받침지지길이 평가"), iIdx, BrevD.strName);
		PrintCellData(pXL, _T("[8_x]Title_Pier_Name"), strName, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if ( !Print_BearingSuppLen(pXL, pProDlg, nProCnt, nProTot, ResD, stRow) ) return FALSE;

		_DGN_SEISEVAL_SuppLengD Res4TB(BrevD.strName, m_dFactorLengthMM*ResD.SuppLengD.dNC, m_dFactorLengthMM*ResD.SuppLengD.dND, ResD.SuppLengD.dRat);
		arResD.Add(Res4TB);
	}

	if ( iIdx > 0 )
	{
		if ( !Print_BearingSuppLenSummary(pXL, pProDlg, nProCnt, nProTot, arResD, iIdx, stRow) ) return FALSE;
		pXL->SetHPageBreak(53, stRow, 28);
	}
	else
	{
		if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }
	}

	return TRUE;
}

void CBrdgSeisEvalKISTEC2015_Report::SetBearingEvaluationSheetPage(IExcel* pExcel, const CString& strName, const int& iIdx, int& iCntIdx)
{
	m_strBaseSheet = _T("7.받침성능평가(Base)");

	CString RptBlock;

	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d-%d.%s 받침내진성능평가"), iIdx, iCntIdx, strName);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iCntIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::SetBearingLengthEvaluationSheetPage(IExcel* pExcel, int& iIdx)
{
	m_strBaseSheet = _T("8.받침지지길이평가(Base)");

	CString RptBlock;

	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.받침지지길이평가"), iIdx);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::IsPrintMark(const double& dVal1, const double& dVal2)
{
	return (fabs(dVal1) > cDGN_Zero && fabs(dVal2) > cDGN_Zero);
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingBody(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;
	//========================================================================================================================
	// 7.1 Bearing Body
	CString RptBlock = _T("[7_1]Title_Bearing_Body");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;
	// 7.1 1) 수평저항력 평가
	RptBlock = _T("[7_1_1]Bearing_Hor_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_1_1_1]Bearing_Hor_Resist");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	//////////////////////////////////////////////////////////////////////////
	CMapEx<CString, LPCTSTR, CArrayEx<int, int>, CArrayEx<int, int>&> mAnchorParamAll;   mAnchorParamAll.RemoveAll();
	CMapEx<CString, LPCTSTR, CArrayEx<int, int>, CArrayEx<int, int>&> mAnchorParamBL;    mAnchorParamBL.RemoveAll();
	CMapEx<CString, LPCTSTR, CArrayEx<int, int>, CArrayEx<int, int>&> mAnchorParamBT;    mAnchorParamBT.RemoveAll();
	mAnchorParamAll.InitHashTable(11);
	mAnchorParamBL.InitHashTable(11);
	mAnchorParamBT.InitHashTable(11);
	CArrayEx<CString, CString> aString; aString.RemoveAll();

	int enBearingTemp = 0;
	CArrayEx<int, int> aKey;
	CArrayEx<int, int> aBL;
	CArrayEx<int, int> aBT;
	int iAnchorParaSize = ResD.AnchorParamAll.GetSize();
	for ( int k = 0; k < iAnchorParaSize; k++ )
	{
		CString strName = ResD.AnchorParamAll[k].strName;

		if ( mAnchorParamAll.Lookup(strName, aKey) )
		{
			aKey.Add(k); mAnchorParamAll.SetAt(strName, aKey);
		}
		else
		{
			aKey.RemoveAll();
			aKey.Add(k); mAnchorParamAll.SetAt(strName, aKey);
			aString.Add(strName);
		}

		if ( ResD.AnchorParamAll[k].dnBL > 0 )
		{
			if ( mAnchorParamBL.Lookup(strName, aBL) )
			{
				aBL.Add(k); mAnchorParamBL.SetAt(strName, aBL);
			}
			else
			{
				aBL.RemoveAll();
				aBL.Add(k); mAnchorParamBL.SetAt(strName, aBL);
			}
		}

		if ( ResD.AnchorParamAll[k].dnBT > 0 )
		{
			if ( mAnchorParamBT.Lookup(strName, aBT) )
			{
				aBT.Add(k); mAnchorParamBT.SetAt(strName, aBT);
			}
			else
			{
				aBT.RemoveAll();
				aBT.Add(k); mAnchorParamBT.SetAt(strName, aBT);
			}
		}

		if ( ResD.AnchorParamAll[k].nBearing == BEARING_ELAST )
			enBearingTemp++;
	}
	int iAnchorParaSizeMap = aString.GetSize();
	//////////////////////////////////////////////////////////////////////////

	for ( int i = 0; i < iAnchorParaSizeMap/*ResD.AnchorParamAll.GetSize()*/; i++ )
	{
		int iIdxBP = i % 4;
		if ( iIdxBP == 0 )
		{
			if ( i > 3 ) stRow += 4;
			RptBlock = _T("[7_1_1_1]Bearing_Hor_Table");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		}

		//////////////////////////////////////////////////////////////////////////
		CString strName = aString[i];
		aKey.RemoveAll();
		if ( !mAnchorParamAll.Lookup(strName, aKey) ) { ASSERT(0); continue; }
		int ni = aKey[0];
		int nNum = aKey.GetSize();
		//////////////////////////////////////////////////////////////////////////

		RptBlock.Format(_T("S7_1_1_Name%d"), iIdxBP + 1);
		PrintCellData(pXL, RptBlock, ResD.AnchorParamAll[ni].strName, Row_Ref, Col_Ref, stRow);
		RptBlock.Format(_T("S7_1_1_Longi%d"), iIdxBP + 1);
		PrintCellData(pXL, RptBlock, ResD.AnchorParamAll[ni].dCapaL, Row_Ref, Col_Ref, stRow);
		RptBlock.Format(_T("S7_1_1_Trans%d"), iIdxBP + 1);
		PrintCellData(pXL, RptBlock, ResD.AnchorParamAll[ni].dCapaT, Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	RptBlock = _T("[7_1_1_2]Bearing_Hor_Numb");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	for ( int i = 0; i < iAnchorParaSizeMap/*ResD.AnchorParamAll.GetSize()*/; i++ )
	{
		int iIdxBP = i % 4;
		if ( iIdxBP == 0 )
		{
			if ( i > 3 ) stRow += 4;
			RptBlock = _T("[7_1_1_1]Bearing_Hor_Table");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		}

		//////////////////////////////////////////////////////////////////////////
		CString strName = aString[i];
		aKey.RemoveAll();
		if ( !mAnchorParamAll.Lookup(strName, aKey) ) { ASSERT(0); continue; }
		int ni = aKey[0];

		aBL.RemoveAll();
		aBT.RemoveAll();
		if ( !mAnchorParamBL.Lookup(strName, aBL) ) { aBL.RemoveAll(); }
		if ( !mAnchorParamBT.Lookup(strName, aBT) ) { aBT.RemoveAll(); }
		int nNumL = aBL.GetSize();
		int nNumT = aBT.GetSize();
		//////////////////////////////////////////////////////////////////////////

		RptBlock.Format(_T("S7_1_1_Name%d"), iIdxBP + 1);
		PrintCellData(pXL, RptBlock, ResD.AnchorParamAll[ni].strName, Row_Ref, Col_Ref, stRow);
		RptBlock.Format(_T("S7_1_1_Longi%d"), iIdxBP + 1);
		PrintCellData(pXL, RptBlock, /*ResD.AnchorParamAll[ni].dnBL **/ nNumL, Row_Ref, Col_Ref, stRow);
		RptBlock.Format(_T("S7_1_1_Trans%d"), iIdxBP + 1);
		PrintCellData(pXL, RptBlock, /*ResD.AnchorParamAll[ni].dnBT **/ nNumT, Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	RptBlock = _T("[7_1_1_3]Bearing_Hor_Total_Resist");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pXL, _T("S7_1_1_2_FBCL"), ResD.BodyD[0].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_2_FBCT"), ResD.BodyD[1].HorD.dFBC, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	RptBlock = _T("[7_1_1_4]Bearing_Hor_Total_Force");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pXL, _T("S7_1_1_3_2_Ang"), ResD.ParamD.dRelAngle, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_3_2_FnL"), ResD.BodyD[0].HorD.dFn, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_3_2_FnT"), ResD.BodyD[1].HorD.dFn, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_3_2_VEL"), ResD.BodyD[0].HorD.dVe, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_3_2_VET"), ResD.BodyD[1].HorD.dVe, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	if (ResD.BodyD[0].HorD.bRoundSect)
	{
		RptBlock = _T("[7_1_1_4]Bearing_Hor_Total_Force_1_R");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		BOOL bAbutment = FALSE;
		if (ResD.BodyD[0].HorD.dFn == 0.0 && ResD.BodyD[1].HorD.dFn == 0.0) bAbutment = TRUE;
		if (bAbutment)  PrintCellData(pXL, _T("S7_1_1_3_FnL_R"), _T("-"), Row_Ref, Col_Ref, stRow);
		else			PrintCellData(pXL, _T("S7_1_1_3_FnL_R"), ResD.BodyD[0].HorD.dFnAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_VEL_R"), ResD.BodyD[0].HorD.dVeAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_FBDL_R"), ResD.BodyD[0].HorD.dFBD, Row_Ref, Col_Ref, stRow);

		if (bAbutment)  PrintCellData(pXL, _T("S7_1_1_3_FnT_R"), _T("-"), Row_Ref, Col_Ref, stRow);
		else			PrintCellData(pXL, _T("S7_1_1_3_FnT_R"), ResD.BodyD[1].HorD.dFnAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_VET_R"), ResD.BodyD[1].HorD.dVeAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_FBDT_R"), ResD.BodyD[1].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	}
	else  // 기존
	{
		RptBlock = _T("[7_1_1_4]Bearing_Hor_Total_Force_1");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		BOOL bAbutment = FALSE;
		if (ResD.BodyD[0].HorD.dFn == 0.0 && ResD.BodyD[1].HorD.dFn == 0.0) bAbutment = TRUE;
		if (bAbutment)  PrintCellData(pXL, _T("S7_1_1_3_FnL"), _T("-"), Row_Ref, Col_Ref, stRow);
		else			PrintCellData(pXL, _T("S7_1_1_3_FnL"), ResD.BodyD[0].HorD.dFnAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_VEL"), ResD.BodyD[0].HorD.dVeAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_FBDL"), ResD.BodyD[0].HorD.dFBD, Row_Ref, Col_Ref, stRow);

		if (bAbutment)  PrintCellData(pXL, _T("S7_1_1_3_FnT"), _T("-"), Row_Ref, Col_Ref, stRow);
		else			PrintCellData(pXL, _T("S7_1_1_3_FnT"), ResD.BodyD[1].HorD.dFnAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_VET"), ResD.BodyD[1].HorD.dVeAng, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_1_2_FBDT"), ResD.BodyD[1].HorD.dFBD, Row_Ref, Col_Ref, stRow);

	}
	stRow += rowCnt;

	RptBlock = _T("[7_1_1_4]Bearing_Hor_Total_Force_2");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	//GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	RptBlock = _T("[7_1_1_5]Bearing_Hor_Result");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pXL, _T("S7_1_1_4_FBCL"), ResD.BodyD[0].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_4_FBDL"), ResD.BodyD[0].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.BodyD[0].HorD.dFBC, ResD.BodyD[0].HorD.dFBD))
	{
		PrintCellData(pXL, _T("S7_1_1_4_RatL"), ResD.BodyD[0].HorD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_1_1_4_ResultL"), Chk_OK_Sign(0, 1.0, ResD.BodyD[0].HorD.dRat), Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pXL, _T("S7_1_1_4_FBCT"), ResD.BodyD[1].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_1_1_4_FBDT"), ResD.BodyD[1].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.BodyD[1].HorD.dFBC, ResD.BodyD[1].HorD.dFBD))
	{
		PrintCellData(pXL, _T("S7_1_1_4_RatT"), ResD.BodyD[1].HorD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_1_1_4_ResultT"), Chk_OK_Sign(0, 1.0, ResD.BodyD[1].HorD.dRat), Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	if ( enBearingTemp > 0/*ResD.ParamD.enBearing == BEARING_ELAST*/ )
	{
		// 7.1 2) 전단변위 평가
		RptBlock = _T("[7_1_2]Bearing_Disp");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pXL, _T("S7_1_2_1_dBC_L"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBC, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_1_2_2_FBCL"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_2_3_FBDL"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBD, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_1_2_4_FBCL"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_2_4_FBDL"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.BodyD[0].DispD.dFBC, ResD.BodyD[0].DispD.dFBD))
		{
			PrintCellData(pXL, _T("S7_1_2_4_RatL"), ResD.BodyD[0].DispD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_1_2_4_ResultL"), Chk_OK_Sign(0, 1.0, ResD.BodyD[0].DispD.dRat), Row_Ref, Col_Ref, stRow);
		}



		PrintCellData(pXL, _T("S7_1_2_1_dBC_T"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBC, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_1_2_2_FBCT"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_2_3_FBDT"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBD, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_1_2_4_FBCT"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_1_2_4_FBDT"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.BodyD[1].DispD.dFBC, ResD.BodyD[1].DispD.dFBD))
		{
			PrintCellData(pXL, _T("S7_1_2_4_RatT"), ResD.BodyD[1].DispD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_1_2_4_ResultT"), Chk_OK_Sign(0, 1.0, ResD.BodyD[1].DispD.dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }
	//========================================================================================================================

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintBearingFig(IExcel* pExcel, CString& strMPhiFileName, const int& stRow)
{
	//
	int nH = 12;
	int nW = 12;
	//
	CString strPath = strMPhiFileName;

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow + 1, 9, nH, nW, strPath);
	}
	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintBearingProjFig(IExcel* pExcel, CString& strMPhiFileName, const int& stRow)
{
	//
	int nH = 18;
	int nW = 25;
	//
	CString strPath = strMPhiFileName;

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow, 3, nH, nW, strPath);
	}
	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorStl(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 7.2 앵커부 평가 검토
	CString RptBlock = _T("[7_2]Title_Anchor");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorParamAll.GetSize() > 0 )
	{
		if ( !Print_BearingAnchorStl_Multi(pXL, pProDlg, nProCnt, nProTot, BrevK, ResD, stRow) ) return FALSE;
	}
	else
	{
		if ( !Print_BearingAnchorStl_Single(pXL, pProDlg, nProCnt, nProTot, BrevK, ResD, stRow) ) return FALSE;
	}

	// 2) 강재파괴
	RptBlock = _T("[7_2_2]Titil_Steel_Fracture");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	CString strEQSign = _T("");
	CString strPlusSign = _T("+");

	RptBlock = _T("[7_2_2_1_1]FASC_Anchor_Multi_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	const int iAnchorParaSize = ResD.AnchorParamAll.GetSize();

	//////////////////////////////////////////////////////////////////////////
	CMapEx<CString, LPCTSTR, CArrayEx<int, int>, CArrayEx<int, int>&> mAnchorParamAll;   mAnchorParamAll.RemoveAll();
	CMapEx<CString, LPCTSTR, CArrayEx<int, int>, CArrayEx<int, int>&> mAnchorParamBL;    mAnchorParamBL.RemoveAll();
	CMapEx<CString, LPCTSTR, CArrayEx<int, int>, CArrayEx<int, int>&> mAnchorParamBT;    mAnchorParamBT.RemoveAll();
	mAnchorParamAll.InitHashTable(11);
	mAnchorParamBL.InitHashTable(11);
	mAnchorParamBT.InitHashTable(11);
	CArrayEx<CString, CString> aString; aString.RemoveAll();

	T_BSED_D BsedD;
	if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

	CArrayEx<int, int> aKey;
	CArrayEx<int, int> aBL;
	CArrayEx<int, int> aBT;
	for ( int k = 0; k < iAnchorParaSize; k++ )
	{
		CString strName = ResD.AnchorParamAll[k].strName;

		if ( mAnchorParamAll.Lookup(strName, aKey) )
		{
			aKey.Add(k);
			mAnchorParamAll.SetAt(strName, aKey);
		}
		else
		{
			aKey.RemoveAll();
			aKey.Add(k);
			mAnchorParamAll.SetAt(strName, aKey);
			aString.Add(strName);
		}

		if ( ResD.AnchorParamAll[k].dnBL > 0 )
		{
			if ( mAnchorParamBL.Lookup(strName, aBL) )
			{
				aBL.Add(k); mAnchorParamBL.SetAt(strName, aBL);
			}
			else
			{
				aBL.RemoveAll();
				aBL.Add(k); mAnchorParamBL.SetAt(strName, aBL);
			}
		}

		if ( ResD.AnchorParamAll[k].dnBT > 0 )
		{
			if ( mAnchorParamBT.Lookup(strName, aBT) )
			{
				aBT.Add(k); mAnchorParamBT.SetAt(strName, aBT);
			}
			else
			{
				aBT.RemoveAll();
				aBT.Add(k); mAnchorParamBT.SetAt(strName, aBT);
			}
		}
	}
	int iAnchorParaSizeMap = aString.GetSize();
	//////////////////////////////////////////////////////////////////////////

	if ( mAnchorParamBL.GetSize() > 0  /*ResD.AnchorParamAll[0].dnBL > 0.0 */ )
	{
		if(BsedD.nDesignCode == KALIS_2023)		RptBlock = _T("[7_2_2_1_1]FASC_Anchor_Multi_2023");
		else                             		RptBlock = _T("[7_2_2_1_1]FASC_Anchor_Multi");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		int nB = 0;
		for ( int k = 0; k < iAnchorParaSizeMap; k++ )
		{
			RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi_Start_Line");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

			if ( k == 0 )  strEQSign = _T("=");
			else           strEQSign = _T("");

			if ( k == iAnchorParaSizeMap - 1 ) strPlusSign = _T("");
			else                               strPlusSign = _T("+");

			//////////////////////////////////////////////////////////////////////////
			CString strName = aString[k];
			aKey.RemoveAll();
			aBL.RemoveAll();
			if ( !mAnchorParamAll.Lookup(strName, aKey) ) { ASSERT(0); continue; }
			if ( !mAnchorParamBL.Lookup(strName, aBL) ) { aBL.RemoveAll(); }
			int nK = aKey[0];
			int nNum = aBL.GetSize();	nB += nNum;
			double nnA = ResD.AnchorParamAll[nK].nnA;
			double dVsParam = ResD.AnchorParamAll[nK].dVsParam;
			double dAse = ResD.AnchorParamAll[nK].dAse;
			double dfua = ResD.AnchorParamAll[nK].dfua;
			//////////////////////////////////////////////////////////////////////////

			PrintCellData(pXL, _T("S7_2_2_1_1A_Plus_Sign"), strPlusSign, Row_Ref, Col_Ref, stRow);

			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_BP_Numb"), nNum, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_nA"), nnA      /*ResD.AnchorParamAll[k].nnA     */, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_BP_VsParam"), dVsParam /*ResD.AnchorParamAll[k].dVsParam*/, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_Ase"), m_dFactorAreamm2*dAse     /*ResD.AnchorParamAll[k].dAse    */, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_futa"), m_dFactorStress *dfua     /*ResD.AnchorParamAll[k].dfua    */, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_EQ_Sign"), strEQSign, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			if ( k == iAnchorParaSizeMap - 1 )
			{
				RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi_End_Line");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				if(BsedD.nDesignCode == KALIS_2023)
				{ 
					PrintCellData(pXL, _T("S7_2_2_1_1_Multi_FASC"), ResD.AnchorD[0].FASD.dVsa1, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("[7_2_2_1_1]FASC_Anchor_Multi_eq");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

					PrintCellData(pXL, _T("S7_2_2_1_1B_Vsa2_D"), ResD.AnchorD[0].FASD.dD, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_1B_Vsa2_B"), nB, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_1B_Vsa2_A"), ResD.GetTotalAnchorNum(), Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_1B_Vsa2"), ResD.AnchorD[0].FASD.dVsa2, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_1B_Vancc"), ResD.AnchorD[0].FASD.dFASC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					PrintCellData(pXL, _T("S7_2_2_1_1_Multi_FASC"), ResD.AnchorD[0].FASD.dFASC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}							
			}
		}
		stRow++;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
		stRow++;
	}

	RptBlock = _T("[7_2_2_1_2]FASC_Anchor_Multi_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;


	if ( mAnchorParamBT.GetSize() > 0/*ResD.AnchorParamAll[0].dnBT > 0.0 */ )
	{
		if(BsedD.nDesignCode == KALIS_2023)		RptBlock = _T("[7_2_2_1_2]FASC_Anchor_Multi_2023");
		else                             		RptBlock = _T("[7_2_2_1_2]FASC_Anchor_Multi");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		int nB = 0;
		for ( int k = 0; k < iAnchorParaSizeMap; k++ )
		{
			RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi_Start_Line");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

			if ( k == 0 )  strEQSign = _T("=");
			else           strEQSign = _T("");

			if ( k == iAnchorParaSizeMap - 1 ) strPlusSign = _T("");
			else                               strPlusSign = _T("+");

			//////////////////////////////////////////////////////////////////////////
			CString strName = aString[k];
			aKey.RemoveAll();
			aBT.RemoveAll();
			if ( !mAnchorParamAll.Lookup(strName, aKey) ) { ASSERT(0); continue; }
			if ( !mAnchorParamBT.Lookup(strName, aBT) ) { aBT.RemoveAll(); }
			int nK = aKey[0];
			int nNum = aBT.GetSize();	nB += nNum;
			double nnA = ResD.AnchorParamAll[nK].nnA;
			double dVsParam = ResD.AnchorParamAll[nK].dVsParam;
			double dAse = ResD.AnchorParamAll[nK].dAse;
			double dfua = ResD.AnchorParamAll[nK].dfua;
			//////////////////////////////////////////////////////////////////////////

			PrintCellData(pXL, _T("S7_2_2_1_1A_Plus_Sign"), strPlusSign, Row_Ref, Col_Ref, stRow);

			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_BP_Numb"), nNum, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_nA"), nnA		/*ResD.AnchorParamAll[k].nnA     */, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_BP_VsParam"), dVsParam/*ResD.AnchorParamAll[k].dVsParam*/, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_Ase"), m_dFactorAreamm2*   dAse	/*ResD.AnchorParamAll[k].dAse    */, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_Multi_futa"), m_dFactorStress *  dfua	/*ResD.AnchorParamAll[k].dfua    */, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_2_1_1A_EQ_Sign"), strEQSign, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			if ( k == iAnchorParaSizeMap - 1 )
			{
				RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi_End_Line");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				if(BsedD.nDesignCode == KALIS_2023)
				{
					PrintCellData(pXL, _T("S7_2_2_1_1_Multi_FASC"), ResD.AnchorD[1].FASD.dVsa1, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("[7_2_2_1_2]FASC_Anchor_Multi_eq");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

					PrintCellData(pXL, _T("S7_2_2_1_2B_Vsa2_D"), ResD.AnchorD[1].FASD.dD, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_2B_Vsa2_B"), nB, Row_Ref, Col_Ref, stRow); // 저항하는 받침만 고려
					PrintCellData(pXL, _T("S7_2_2_1_2B_Vsa2_A"), ResD.GetTotalAnchorNum(), Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_2B_Vsa2"), ResD.AnchorD[1].FASD.dVsa2, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_2_1_2B_Vancc"), ResD.AnchorD[1].FASD.dFASC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					PrintCellData(pXL, _T("S7_2_2_1_1_Multi_FASC"), ResD.AnchorD[1].FASD.dFASC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
			}
		}
		stRow++;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
		stRow++;
	}

	RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi_Desc");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;
	if(BsedD.nDesignCode == KALIS_2023)
	{
		RptBlock = _T("[7_2_2_1]FASC_Anchor_Multi_Desc_2023");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;

	RptBlock = _T("[7_2_2_2]FASD_M");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_2_2_1]FASD_M_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( mAnchorParamBT.GetSize() > 0/*ResD.AnchorParamAll[0].dnBL > 0.0*/ )
	{
		RptBlock = _T("[7_2_2_2_1]FASD_M_L");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_2_2_M_FASDL"), ResD.AnchorD[0].FASD.dFASD, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[7_2_2_2_2]FASD_M_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( mAnchorParamBT.GetSize() > 0/*ResD.AnchorParamAll[0].dnBT > 0.0*/ )
	{
		RptBlock = _T("[7_2_2_2_2]FASD_M_T");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_2_2_M_FASDT"), ResD.AnchorD[1].FASD.dFASD, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;

	RptBlock = _T("[7_2_2_3]FASC_FASD");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_2_3_1]FASC_FASD_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( mAnchorParamBT.GetSize() > 0/*ResD.AnchorParamAll[0].dnBL > 0.0*/ )
	{
		RptBlock = _T("[7_2_2_3_1]FASC_FASD_L");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_2_3_FASCL"), ResD.AnchorD[0].FASD.dFASC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_2_3_FASDL"), ResD.AnchorD[0].FASD.dFASD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.AnchorD[0].FASD.dFASC, ResD.AnchorD[0].FASD.dFASD))
		{
			PrintCellData(pXL, _T("S7_2_2_3_RatL"), ResD.AnchorD[0].FASD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_2_2_3_ResultL"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FASD.dRat), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}


	RptBlock = _T("[7_2_2_3_2]FASC_FASD_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( mAnchorParamBT.GetSize() > 0/*ResD.AnchorParamAll[0].dnBT > 0.0*/ )
	{
		RptBlock = _T("[7_2_2_3_2]FASC_FASD_T");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_2_3_FASCT"), ResD.AnchorD[1].FASD.dFASC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_2_3_FASDT"), ResD.AnchorD[1].FASD.dFASD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.AnchorD[1].FASD.dFASC, ResD.AnchorD[1].FASD.dFASD))
		{
			PrintCellData(pXL, _T("S7_2_2_3_RatT"), ResD.AnchorD[1].FASD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_2_2_3_ResultT"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FASD.dRat), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorStl_Single(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 1) 받침제원
	CString RptBlock = _T("[7_2_1]Bearing_Data");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);

	T_ANEV_D AnevD;
	T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
	T_BRPR_D BrprD;
	if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) { /*ASSERT(0); */ }

	CString strFileName;
	strFileName = AnevD.strName;//BrprD.strName;
	if ( !CFileCtrl::ConvertToValidFileName(strFileName) )
		return FALSE;

	CString strPicFileName;
	strPicFileName.Format(_T("%sBearing_%s.emf"), m_strPicPath, strFileName);

	CDgnSeisBearingPropItemDlg dlg;
	dlg.SetParamData(AnevD.BrprK, BrprD);
	dlg.MakeEmfAndClose(strPicFileName);
	dlg.DoModal();
	::Sleep(1000);

	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	PrintBearingFig(pXL, strPicFileName, stRow);

	PrintCellData(pXL, _T("S7_2_1_B"), m_dFactorLengthMM*ResD.AnchorParamAll[0].dB, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_C"), m_dFactorLengthMM*ResD.AnchorParamAll[0].dC, Row_Ref, Col_Ref, stRow);

	PrintCellData(pXL, _T("S7_2_1_hef"), m_dFactorLengthMM*ResD.AnchorParamAll[0].dhef, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_SL"), m_dFactorLengthMM*ResD.AnchorParamAll[0].dS1, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_ST"), m_dFactorLengthMM*ResD.AnchorParamAll[0].dS2, Row_Ref, Col_Ref, stRow);

	PrintCellData(pXL, _T("S7_2_1_da"), m_dFactorLengthMM*ResD.AnchorParamAll[0].da, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_M"), m_dFactorLengthMM*ResD.AnchorParamAll[0].ds, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_nL"), ResD.AnchorParamAll[0].nnL, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_nT"), ResD.AnchorParamAll[0].nnT, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_nA"), ResD.AnchorParamAll[0].nnA, Row_Ref, Col_Ref, stRow);

	CString strType1, strType2;
	Get_AnchorTypeName(ResD.AnchorParamAll[0].enAnchType, strType1, strType2);
	PrintCellData(pXL, _T("S7_2_1_AnchorType1"), strType1, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_AnchorType2"), strType2, Row_Ref, Col_Ref, stRow);

	PrintCellData(pXL, _T("S7_2_1_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_Ase"), m_dFactorAreamm2*ResD.AnchorParamAll[0].dAse, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_fya"), m_dFactorStress*ResD.AnchorParamAll[0].dfya, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_futa"), m_dFactorStress*ResD.AnchorParamAll[0].dfua, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_futa_lim"), m_dFactorStress* ResD.AnchorParamAll[0].dfuaLim, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_hefUser"), m_dFactorLengthMM*ResD.AnchorParamAll[0].dhef, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_1_hcop"), m_dFactorLengthMM*ResD.ParamD.dhcop, Row_Ref, Col_Ref, stRow);

	if ( ResD.AnchorParamAll[0].nStlBearing == BEARING_STEEL_POTo )
	{
		pXL->SetValue(stRow + 28, 13, _T("앵커의 유효단면적(소켓직경)"));
	}
	else
	{
		CString sCell = pXL->GetCell(stRow + 19, 0);
		pXL->DeletCell(sCell, sCell);
		stRow--;
	}

	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorStl_Multi(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 1) 받침제원
	CString RptBlock = _T("[7_2_1]Bearing_Multi_Data");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);

	T_ANEV_D AnevD;
	T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);
	T_BRPR_D BrprD;
	if ( !m_pDoc->m_pAttrCtrl2->GetBrpr(AnevD.BrprK, BrprD) ) { /*ASSERT(0); */ }

	const int iAnchorParaSize = ResD.AnchorParamAll.GetSize();

	int iTitleIndex = 0;
	CString strTitle = _T("");
	CString strDash = _T("-");
	CString strNameTemp = _T("-");

	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	int k = 0;

	for ( int j = 0; j < iAnchorParaSize; j++ )
	{
		if ( ResD.AnchorParamAll[j].strName.CompareNoCase(strNameTemp) == 0 )
		{
			continue;
		}
		else
		{
			k += 1;
		}

		strNameTemp = ResD.AnchorParamAll[j].strName;

		if ( k > ResD.AnchorParamAll[j].dBPNumb ) continue;

		RptBlock = _T("[7_2_1_1]Bearing_Multi_Body_Prop");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		iTitleIndex += 1;
		strTitle.Format(_T("'[%d]"), iTitleIndex);

		PrintCellData(pXL, _T("S7_2_1_1_Bearing_Numb"), strTitle, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_1_Bearing_Name"), ResD.AnchorParamAll[j].strName, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_2_1_M_B"), m_dFactorLengthMM*ResD.AnchorParamAll[j].dB, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_C"), m_dFactorLengthMM*ResD.AnchorParamAll[j].dC, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_2_1_M_SL"), m_dFactorLengthMM*ResD.AnchorParamAll[j].dS1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_ST"), m_dFactorLengthMM*ResD.AnchorParamAll[j].dS2, Row_Ref, Col_Ref, stRow);

		if ( ResD.AnchorParamAll[j].nStlBearing == BEARING_STEEL_POTo )
		{
			PrintCellData(pXL, _T("S7_2_1_M_M"), m_dFactorLengthMM*ResD.AnchorParamAll[j].ds, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_1_M_da"), strDash, Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pXL, _T("S7_2_1_M_M"), strDash, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_1_M_da"), m_dFactorLengthMM*ResD.AnchorParamAll[j].da, Row_Ref, Col_Ref, stRow);
		}

		PrintCellData(pXL, _T("S7_2_1_M_nL"), ResD.AnchorParamAll[j].nnL, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_nT"), ResD.AnchorParamAll[j].nnT, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_nA"), ResD.AnchorParamAll[j].nnA, Row_Ref, Col_Ref, stRow);

		CString strType1, strType2;
		Get_AnchorTypeName(ResD.AnchorParamAll[j].enAnchType, strType1, strType2);
		PrintCellData(pXL, _T("S7_2_1_M_AnchorType1"), strType1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_AnchorType2"), strType2, Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;

		RptBlock = _T("[7_2_1_2]Bearing_Multi_Else_Prop");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pXL, _T("S7_2_1_M_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_Ase"), m_dFactorAreamm2*ResD.AnchorParamAll[j].dAse, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_fya"), m_dFactorStress*ResD.AnchorParamAll[j].dfya, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_futa"), m_dFactorStress*ResD.AnchorParamAll[j].dfua, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_futa_lim"), m_dFactorStress* ResD.AnchorParamAll[j].dfuaLim, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_hefUser"), m_dFactorLengthMM*ResD.AnchorParamAll[j].dhef, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_1_M_hcop"), m_dFactorLengthMM*ResD.ParamD.dhcop, Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;

	}
	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorConc(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	T_BSED_D BsedD;
	if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

	// 3) 콘크리트 파괴
	CString RptBlock = BsedD.nDesignCode == KALIS_2023 ? _T("[7_2_3]Titil_Concrete_2023") : _T("[7_2_3]Titil_Concrete");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt + 1;

	for ( int j = 0; j < 2; ++j ) // Longitudinal, Transverse
	{
		RptBlock = j == 0 ? _T("[7_2_3_1_1]Titil_Concrete_L") : _T("[7_2_3_1_2]Titil_Concrete_T");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		RptBlock = _T("[7_2_3_1]Concrete_Pic_Data");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		if ( TRUE )//!ResD.ParamD.bUserConc)
		{
			T_UNIT_INDEX CurIndex, CngIndex;
			m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
			CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);

			T_ANEV_D AnevD;
			T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);

			CString strFileName;
			strFileName = AnevD.strName;
			if ( !CFileCtrl::ConvertToValidFileName(strFileName) )
				return FALSE;

			CString strPicFileName;
			strPicFileName.Format(_T("%s%s_AnchorConc_%d.emf"), m_strPicPath, strFileName, j + 1);

			CDgnSeisAnchor2FailDlg dlg;
			dlg.m_Data = AnevD;
			dlg.m_bIsReport = TRUE;
			dlg.m_nRatTypeFail[0] = ResD.AnchorD[0].FACD.nRatType;
			dlg.m_nRatTypeFail[1] = ResD.AnchorD[1].FACD.nRatType;
			dlg.m_nRatTypeFail[2] = ResD.AnchorD[0].FAPD.nRatType;
			dlg.m_nRatTypeFail[3] = ResD.AnchorD[1].FAPD.nRatType;
			dlg.MakeEmfAndClose(strPicFileName, 0, j);
			dlg.DoModal();
			::Sleep(1000);

			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

			PrintBearingProjFig(pXL, strPicFileName, stRow);
		}

		stRow += rowCnt;

		const int nAnchorParamLongi = ResD.AnchorParamConcLongi.GetSize();
		const int nAnchorParamTrans = ResD.AnchorParamConcTrans.GetSize();

		int nAnchorParam = 0;

		if ( j == 0 )
		{
			nAnchorParam = nAnchorParamLongi;
		}
		else
		{
			nAnchorParam = nAnchorParamTrans;
		}

		if ( nAnchorParam < 2 )
		{
			RptBlock = _T("[7_2_3_1_1]Concrete_S_Data");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

			if ( j == 0 )
			{
				PrintCellData(pXL, _T("S7_2_3_1_S1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.ds1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_S2"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.ds2, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pXL, _T("S7_2_3_1_S1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.ds2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_S2"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.ds1, Row_Ref, Col_Ref, stRow);
			}
			PrintCellData(pXL, _T("S7_2_3_1_Sax"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dsax, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_3_1_Say"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dsay, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}

		RptBlock = _T("[7_2_3_1_2]Concrete_Ca_Data");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pXL, _T("S7_2_3_1_ca1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1_org, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_1_ca1p"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1p, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_1_ca2"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_1_ca2p"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca2p, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_1_15ca1_ca1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1_org, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_1_15ca1"), m_dFactorLengthMM*1.5*ResD.AnchorD[j].FACD.dca1_org, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if ( nAnchorParam > 0 )
		{
			// ▶ 단일 앵커 및 앵커 그룹 판단
			RptBlock = _T("S7_2_3_1_2");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;

			if ( nAnchorParam < 2 )
			{
				if ( ResD.AnchorD[j].FACD.nAvcType == 0 )
				{
					RptBlock = _T("S7_2_3_1_2_Nontypical");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_3_1_2_Group");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_2_Group_Num"), ResD.AnchorD[j].FACD.nnBC_org, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else if ( ResD.AnchorD[j].FACD.n1AnchorType_org == RESIST_1ANCHOR_1 )
				{
					RptBlock = _T("S7_2_3_1_2_1anchor");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_3_1_2_Anchor");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_2_Anchor_Num"), ResD.AnchorD[j].FACD.nnBC_org, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					if ( ResD.AnchorD[j].FACD.ds1 > cDgn_Zero )
					{
						if ( ResD.AnchorD[j].FACD.ds1 < 2.0*1.5*ResD.AnchorD[j].FACD.dca1_org )
							RptBlock = j == 0 ? _T("S7_2_3_1_2_s1_LT") : _T("S7_2_3_1_2_s2_LT");
						else
							RptBlock = j == 0 ? _T("S7_2_3_1_2_s1_ME") : _T("S7_2_3_1_2_s2_ME");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						stRow += rowCnt;
					}

					if ( j == 0 )
					{
						if ( ResD.AnchorD[j].FACD.dsax > cDgn_Zero )
						{
							if ( ResD.AnchorD[j].FACD.dsax < 2.0*1.5*ResD.AnchorD[j].FACD.dca1_org )
								RptBlock = _T("S7_2_3_1_2_sax_LT");
							else
								RptBlock = _T("S7_2_3_1_2_sax_ME");

							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							stRow += rowCnt;
						}
					}
					else
					{
						if ( ResD.AnchorD[j].FACD.dsay > cDgn_Zero )
						{
							if ( ResD.AnchorD[j].FACD.dsay < 2.0*1.5*ResD.AnchorD[j].FACD.dca1_org )
								RptBlock = _T("S7_2_3_1_2_say_LT");
							else
								RptBlock = _T("S7_2_3_1_2_say_ME");

							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							stRow += rowCnt;
						}
					}

					if ( ResD.AnchorD[j].FACD.n1AnchorType_org == RESIST_1ANCHOR_ALL )
					{
						RptBlock = _T("S7_2_3_1_2_Anchor");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_2_Anchor_Num"), ResD.AnchorD[j].FACD.nnBC_org, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_3_1_2_Group");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_2_Group_Num"), ResD.AnchorD[j].FACD.nnBC_org, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
			}
			else
			{
				RptBlock = _T("S7_2_3_1_2_Nontypical");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				RptBlock = _T("S7_2_3_1_2_Group");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_2_Group_Num"), ResD.AnchorD[j].FACD.nnBC_org, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			stRow++;

			if ( j == TRANS )
			{
				// ▶ 콘크리트파괴 저항면적 상정시 3면 이상 가장자리의 영향 검토
				RptBlock = _T("S7_2_3_1_3");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				if ( ResD.AnchorD[j].FACD.dca2 < 1.5*ResD.AnchorD[j].FACD.dca1_org )
					RptBlock = _T("S7_2_3_1_3_ca2_LT");
				else
					RptBlock = _T("S7_2_3_1_3_ca2_ME");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				if ( ResD.AnchorD[j].FACD.bChk2face )
				{
					if ( ResD.AnchorD[j].FACD.dca2p < 1.5*ResD.AnchorD[j].FACD.dca1_org )
						RptBlock = _T("S7_2_3_1_3_ca2p_LT");
					else
						RptBlock = _T("S7_2_3_1_3_ca2p_ME");
				}
				else
				{
					RptBlock = _T("S7_2_3_1_3_ca2p_None");
				}
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				if ( ResD.AnchorD[j].FACD.dha < 1.5*ResD.AnchorD[j].FACD.dca1_org )
					RptBlock = _T("S7_2_3_1_3_hcop_LT");
				else
					RptBlock = _T("S7_2_3_1_3_hcop_ME");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				if ( ResD.AnchorD[j].FACD.bEdgeEff )
				{
					if ( ResD.ParamD.bUserConc )
					{
						RptBlock = _T("S7_2_3_1_3_NeedButNoRecal");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						stRow += rowCnt;
					}
					else
					{
						if ( j == 1 )
						{
							if ( ResD.AnchorParamConcLongi.GetSize() > 1 )
							{
								RptBlock = _T("S7_2_3_1_3_Recal_Title");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								stRow += rowCnt;

								RptBlock = _T("S7_2_3_1_3_Recal_M");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_3_1_3_M_ca1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;

							}
							else
							{
								RptBlock = _T("S7_2_3_1_3_Recal");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_3_1_3_ca2"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca2, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_3_1_3_ha"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dha, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_3_1_3_say"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dsay, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_3_1_3_ca1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;
							}
						}
						else
						{
							if ( ResD.AnchorParamConcTrans.GetSize() > 1 )
							{
								RptBlock = _T("S7_2_3_1_3_Recal_Title");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								stRow += rowCnt;

								RptBlock = _T("S7_2_3_1_3_Recal_M");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_3_1_3_M_ca1_L"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;
							}
							else
							{
								RptBlock = _T("S7_2_3_1_3_Recal_L");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_3_1_3_ca2_L"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca2, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_3_1_3_ha_L"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dha, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_3_1_3_sax"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dsax, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_3_1_3_ca1_L"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.dca1, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;
							}
						}

						if ( fabs(ResD.AnchorD[j].FACD.dca1 - ResD.AnchorD[j].FACD.dca1_org) > cDgn_Zero && ResD.AnchorD[j].FACD.nnBC > 0 )
						{
							// ▶ 단일 앵커 및 앵커 그룹 판단
							RptBlock = _T("S7_2_3_1_2_Re");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							stRow += rowCnt;

							if ( ResD.AnchorD[j].FACD.n1AnchorType == RESIST_1ANCHOR_1 )
							{
								RptBlock = _T("S7_2_3_1_2_1anchor");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								stRow += rowCnt;

								RptBlock = _T("S7_2_3_1_2_Anchor");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_3_1_2_Anchor_Num"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;
							}
							else
							{
								if ( ResD.AnchorD[j].FACD.ds1 > cDgn_Zero )
								{
									if ( ResD.AnchorD[j].FACD.ds1 < 2.0*1.5*ResD.AnchorD[j].FACD.dca1_org )
										RptBlock = j == 0 ? _T("S7_2_3_1_2_s1_LT") : _T("S7_2_3_1_2_s2_LT");
									else
										RptBlock = j == 0 ? _T("S7_2_3_1_2_s1_ME") : _T("S7_2_3_1_2_s2_ME");
									rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
									stRow += rowCnt;
								}

								if ( j == 0 )
								{
									if ( ResD.AnchorD[j].FACD.dsax > cDgn_Zero )
									{
										if ( ResD.AnchorD[j].FACD.dsax < 2.0*1.5*ResD.AnchorD[j].FACD.dca1_org )
											RptBlock = _T("S7_2_3_1_2_sax_LT");
										else
											RptBlock = _T("S7_2_3_1_2_sax_ME");

										rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
										stRow += rowCnt;
									}
								}
								else
								{
									if ( ResD.AnchorD[j].FACD.dsay > cDgn_Zero )
									{
										if ( ResD.AnchorD[j].FACD.dsay < 2.0*1.5*ResD.AnchorD[j].FACD.dca1_org )
											RptBlock = _T("S7_2_3_1_2_say_LT");
										else
											RptBlock = _T("S7_2_3_1_2_say_ME");

										rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
										stRow += rowCnt;
									}
								}


								if ( ResD.AnchorD[j].FACD.n1AnchorType == RESIST_1ANCHOR_ALL )
								{
									RptBlock = _T("S7_2_3_1_2_Anchor");
									rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
									GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
									PrintCellData(pXL, _T("S7_2_3_1_2_Anchor_Num"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
									stRow += rowCnt;
								}
								else
								{
									RptBlock = _T("S7_2_3_1_2_Group");
									rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
									GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
									PrintCellData(pXL, _T("S7_2_3_1_2_Group_Num"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
									stRow += rowCnt;
								}
							}
						}
					}
				}
				else
				{
					RptBlock = _T("S7_2_3_1_3_NoRecal");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;
				}
				stRow++;
			}

			// ▶ 콘크리트 저항깊이 ha 결정
			if(ResD.ParamD.bUserConc)
			{
				RptBlock = _T("S7_2_3_1_4_User");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_4_User_Ha"), m_dFactorLengthMM * ResD.AnchorD[j].FACD.dha, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				RptBlock = _T("S7_2_3_1_4");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_4_hcop"), m_dFactorLengthMM * ResD.ParamD.dhcop, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_4_15ca1"), m_dFactorLengthMM * ResD.AnchorD[j].FACD.d15ca1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_4_ha"), m_dFactorLengthMM * ResD.AnchorD[j].FACD.dha, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			stRow++;

			// ▶ 콘크리트파괴 투영면적 산정
			RptBlock = _T("S7_2_3_1_5");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
			if ( ResD.ParamD.bUserConc || ResD.AnchorD[j].FACD.nAvcType == 0 || nAnchorParam > 1 )
			{
				RptBlock = _T("S7_2_3_1_5_Type0");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_5_Type0_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				if ( ResD.AnchorD[j].FACD.nAvcType == 1 )
				{
					if ( j == 0 )
					{
						RptBlock = _T("S7_2_3_1_5_Type1_L");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_5_Type1_L_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
					}
					else
					{
						RptBlock = _T("S7_2_3_1_5_Type1_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_5_Type1_T_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
					}
				}
				else if ( ResD.AnchorD[j].FACD.nAvcType == 2 )
				{
					if ( j == 0 )
					{
						RptBlock = _T("S7_2_3_1_5_Type2");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_5_Type2_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
					}
					else
					{
						RptBlock = _T("S7_2_3_1_5_Type2_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_5_Type2_Avc_T"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
					}
				}
				else if ( ResD.AnchorD[j].FACD.nAvcType == 3 )
				{
					RptBlock = _T("S7_2_3_1_5_Type3");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_Type3_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
				}
				else if ( ResD.AnchorD[j].FACD.nAvcType == 4 )
				{
					if ( j == 0 )
					{
						RptBlock = _T("S7_2_3_1_5_Type4");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_5_Type4_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
					}
					else
					{
						RptBlock = _T("S7_2_3_1_5_Type4_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_5_Type4_Avc_T"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
					}
				}
				else if ( ResD.AnchorD[j].FACD.nAvcType == 5 )
				{
					RptBlock = _T("S7_2_3_1_5_Type5");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_Type5_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc_cal, Row_Ref, Col_Ref, stRow);
				}
				else ASSERT(0);
				stRow += rowCnt;
			}

			const int iAnchorParaSizeLongi = ResD.AnchorParamConcLongi.GetSize();
			const int iAnchorParaSizeTrans = ResD.AnchorParamConcTrans.GetSize();
			CString strEQSign = _T("");
			CString strPlusSign = _T("+");

			if ( ResD.ParamD.bUserConc )
			{
				RptBlock = _T("S7_2_3_1_5_Type0_Avco");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_5_Type0_Avco_Val"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				RptBlock = _T("S7_2_3_1_5_Avco_Total_Form");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				if ( j == 0 )
				{
					for ( int x = 0; x < iAnchorParaSizeLongi; x++ )
					{
						RptBlock = _T("S7_2_3_1_5_Avco_Total_Start");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

						if ( x == 0 )  strEQSign = _T("=");
						else           strEQSign = _T("");

						if ( x == iAnchorParaSizeLongi - 1 ) strPlusSign = _T("");
						else                                  strPlusSign = _T("+");

						PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total_Ca"), m_dFactorLengthMM*ResD.AnchorParamConcLongi[x].dCa1Re, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;

						if ( x == iAnchorParaSizeLongi - 1 )
						{
							RptBlock = _T("S7_2_3_1_5_Avco_Total_End");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvcoSum, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;

							RptBlock = _T("S7_2_3_1_5_Avco_Avg_Form");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Avg_Sum"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvcoSum, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Avg_Numb"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Avg"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;
						}
					}
				}
				else
				{
					for ( int z = 0; z < iAnchorParaSizeTrans; z++ )
					{
						RptBlock = _T("S7_2_3_1_5_Avco_Total_Start");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

						if ( z == 0 )  strEQSign = _T("=");
						else           strEQSign = _T("");

						if ( z == iAnchorParaSizeTrans - 1 ) strPlusSign = _T("");
						else                                  strPlusSign = _T("+");

						PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total_Ca"), m_dFactorLengthMM*ResD.AnchorParamConcTrans[z].dCa1Re, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;

						if ( z == iAnchorParaSizeTrans - 1 )
						{
							RptBlock = _T("S7_2_3_1_5_Avco_Total_End");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Total"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvcoSum, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;

							RptBlock = _T("S7_2_3_1_5_Avco_Avg_Form");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Avg_Sum"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvcoSum, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Avg_Numb"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_5_Avco_Avg"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;
						}
					}
				}

				if ( ResD.AnchorD[j].FACD.dAvc_cal < ResD.AnchorD[j].FACD.dAvcLim )
				{
					RptBlock = _T("S7_2_3_1_5_Avc_LT");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc_LT_n"), ResD.AnchorD[j].FACD.nn, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc_LT_Avco"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc_LT_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvcLim, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_3_1_5_M_n");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_M_nn"), ResD.AnchorD[j].FACD.nn, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_3_1_5_Avc1");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc1_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					RptBlock = _T("S7_2_3_1_5_Avc_ME");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc_ME_n"), ResD.AnchorD[j].FACD.nn, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc_ME_Avco"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc_ME_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvcLim, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_3_1_5_M_n");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_M_nn"), ResD.AnchorD[j].FACD.nn, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_3_1_5_Avc2");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_5_Avc2_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FACD.dAvc, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
			}
			stRow++;

			// ▶ 전단력을 받는 앵커의 콘크리트 파괴강도 Vcbg
			if(BsedD.nDesignCode == KALIS_2023)
			{
				RptBlock = _T("S7_2_3_1_6_2023");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_Avc"), m_dFactorAreamm2* ResD.AnchorD[j].FACD.dAvc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_Avco"), m_dFactorAreamm2* ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_phiedv"), ResD.AnchorD[j].FACD.dpsi_edv, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_phicv"), ResD.AnchorD[j].FACD.dpsi_cv, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb"), ResD.AnchorD[j].FACD.dVb, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vcbg"), ResD.AnchorD[j].FACD.dVcbg, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				RptBlock = _T("S7_2_3_1_6");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_6_Avc"), m_dFactorAreamm2 * ResD.AnchorD[j].FACD.dAvc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_Avco"), m_dFactorAreamm2 * ResD.AnchorD[j].FACD.dAvco, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_phiedv"), ResD.AnchorD[j].FACD.dpsi_edv, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_Vb"), ResD.AnchorD[j].FACD.dVb, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_Vcbg"), ResD.AnchorD[j].FACD.dVcbg, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}

			if ( ResD.AnchorD[j].FACD.dca2 < ResD.AnchorD[j].FACD.d15ca1/*1.5*ResD.AnchorD[j].FACD.dca1*/ )
			{
				RptBlock = _T("S7_2_3_1_6_phi1");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_6_phi1_ca2"), m_dFactorLengthMM*min(ResD.AnchorD[j].FACD.dca2, ResD.AnchorD[j].FACD.dca2p), Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_phi1_15ca1"), m_dFactorLengthMM*ResD.AnchorD[j].FACD.d15ca1/*1.5*ResD.AnchorD[j].FACD.dca1*/, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_3_1_6_phi1_phiedv"), ResD.AnchorD[j].FACD.dpsi_edv, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				RptBlock = _T("S7_2_3_1_6_phi2");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;
			}

			if(BsedD.nDesignCode == KALIS_2023)
			{
				RptBlock = _T("S7_2_3_1_6_2023_phicv_eq");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_6_2023_phicv_val"), ResD.AnchorD[j].FACD.dpsi_cv, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			//TODO 계산서 적용 필요 

			RptBlock = _T("S7_2_3_1_6_Cal_Vb_Total_Equation");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;

			if ( j == 0 )
			{
				for ( int x = 0; x < iAnchorParaSizeLongi; x++ )
				{
					RptBlock = _T("S7_2_3_1_6_Cal_Vb_Total_Start");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

					if ( x == 0 )  strEQSign = _T("=");
					else           strEQSign = _T("");

					if ( x == iAnchorParaSizeLongi - 1 ) strPlusSign = _T("");
					else                                  strPlusSign = _T("+");

					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_val2"), ResD.AnchorParamConcLongi[x].dVbParam, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_le"), m_dFactorLengthMM * ResD.AnchorParamConcLongi[x].dle, Row_Ref, Col_Ref, stRow);

					if ( ResD.AnchorParamConcLongi[x].nStlBearing == BEARING_STEEL_POTo )
					{
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da"), m_dFactorLengthMM*ResD.AnchorParamConcLongi[x].ds, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da2"), m_dFactorLengthMM*ResD.AnchorParamConcLongi[x].ds, Row_Ref, Col_Ref, stRow);
					}
					else
					{
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da"), m_dFactorLengthMM*ResD.AnchorParamConcLongi[x].da, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da2"), m_dFactorLengthMM*ResD.AnchorParamConcLongi[x].da, Row_Ref, Col_Ref, stRow);
					}

					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_ca1"), m_dFactorLengthMM * ResD.AnchorParamConcLongi[x].dCa1Re, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					if ( x == iAnchorParaSizeLongi - 1 )
					{
						RptBlock = _T("S7_2_3_1_6_Cal_Vb_Total_End");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Total"), ResD.AnchorD[j].FACD.dVb_Total, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;

						RptBlock = _T("S7_2_3_1_6_Vb_Param_Desc");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						stRow += rowCnt;

						RptBlock = _T("S7_2_3_1_6_Cal_Vb_Avg_Form");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Avg_Sum"), ResD.AnchorD[j].FACD.dVb_Total, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Avg_Numb"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Vb"), ResD.AnchorD[j].FACD.dVbAvg, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;

						if(BsedD.nDesignCode == KALIS_2023)
						{
							RptBlock = _T("S7_2_3_1_6_2023_Vb_eq");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb_ca1"), m_dFactorLengthMM*ResD.AnchorParamConcLongi[x].dCa1Re, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vbmax"),  ResD.AnchorD[j].FACD.dVbMax, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb1"),    ResD.AnchorD[j].FACD.dVbAvg, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb2"),    ResD.AnchorD[j].FACD.dVbMax, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb3"),    ResD.AnchorD[j].FACD.dVb, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;
							stRow++;

							const double dD = 0.88 * ResD.AnchorD[j].FACD.dPuD;
							const double dDBC = dD * ResD.AnchorD[j].FACD.nnBC / ResD.AnchorD[j].FACD.nTotalBearing;
							const double dVbrkec1 = ResD.AnchorD[j].FACD.dVcbg + 0.6 * dDBC;
							const double dVbrkec2 = 1.6 * ResD.AnchorD[j].FACD.dVcbg;
							RptBlock = _T("S7_2_3_1_7_2023");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_Vbrkec1"), dVbrkec1, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_Vbrkec2"), dVbrkec2, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_Vbrkec"), ResD.AnchorD[j].FACD.dVbrkec, Row_Ref, Col_Ref, stRow);

							PrintCellData(pXL, _T("S7_2_3_1_7_2023_D"), dD, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_nBC"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_nB"), ResD.AnchorD[j].FACD.nTotalBearing, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_DBC"), dDBC, Row_Ref, Col_Ref, stRow);

							stRow += rowCnt;
						}
					}
				}
			}
			else
			{
				for ( int x = 0; x < iAnchorParaSizeTrans; x++ )
				{
					RptBlock = _T("S7_2_3_1_6_Cal_Vb_Total_Start");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

					if ( x == 0 )  strEQSign = _T("=");
					else           strEQSign = _T("");

					if ( x == iAnchorParaSizeTrans - 1 ) strPlusSign = _T("");
					else                                  strPlusSign = _T("+");

					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_val2"), ResD.AnchorParamConcTrans[x].dVbParam, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_le"), m_dFactorLengthMM * ResD.AnchorParamConcTrans[x].dle, Row_Ref, Col_Ref, stRow);

					if ( ResD.AnchorParamConcTrans[x].nStlBearing == BEARING_STEEL_POTo )
					{
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da"), m_dFactorLengthMM*ResD.AnchorParamConcTrans[x].ds, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da2"), m_dFactorLengthMM*ResD.AnchorParamConcTrans[x].ds, Row_Ref, Col_Ref, stRow);
					}
					else
					{
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da"), m_dFactorLengthMM*ResD.AnchorParamConcTrans[x].da, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_da2"), m_dFactorLengthMM*ResD.AnchorParamConcTrans[x].da, Row_Ref, Col_Ref, stRow);
					}

					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_ca1"), m_dFactorLengthMM * ResD.AnchorParamConcTrans[x].dCa1Re, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					if ( x == iAnchorParaSizeTrans - 1 )
					{
						RptBlock = _T("S7_2_3_1_6_Cal_Vb_Total_End");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Total"), ResD.AnchorD[j].FACD.dVb_Total, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;

						RptBlock = _T("S7_2_3_1_6_Vb_Param_Desc");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						stRow += rowCnt;

						RptBlock = _T("S7_2_3_1_6_Cal_Vb_Avg_Form");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Avg_Sum"), ResD.AnchorD[j].FACD.dVb_Total, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Avg_Numb"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_6_Cal_Vb_Vb"), ResD.AnchorD[j].FACD.dVbAvg, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
						
						if(BsedD.nDesignCode == KALIS_2023)
						{
							RptBlock = _T("S7_2_3_1_6_2023_Vb_eq");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb_fck"), m_dFactorStress * ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb_ca1"), m_dFactorLengthMM * ResD.AnchorParamConcTrans[x].dCa1Re, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vbmax"), ResD.AnchorD[j].FACD.dVbMax, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb1"), ResD.AnchorD[j].FACD.dVbAvg, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb2"), ResD.AnchorD[j].FACD.dVbMax, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_6_2023_Vb3"), ResD.AnchorD[j].FACD.dVb, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;
							stRow++;

							const double dD = 0.88*ResD.AnchorD[j].FACD.dPuD;
							const double dDBC = dD * ResD.AnchorD[j].FACD.nnBC / ResD.AnchorD[j].FACD.nTotalBearing;
							const double dVbrkec1 = ResD.AnchorD[j].FACD.dVcbg + 0.6*dDBC;
							const double dVbrkec2 = 1.6 * ResD.AnchorD[j].FACD.dVcbg;
							RptBlock = _T("S7_2_3_1_7_2023");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_Vbrkec1"), dVbrkec1, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_Vbrkec2"), dVbrkec2, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_Vbrkec") , ResD.AnchorD[j].FACD.dVbrkec, Row_Ref, Col_Ref, stRow);

							PrintCellData(pXL, _T("S7_2_3_1_7_2023_D"), dD, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_nBC"), ResD.AnchorD[j].FACD.nnBC, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_nB"), ResD.AnchorD[j].FACD.nTotalBearing, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_3_1_7_2023_DBC"), dDBC, Row_Ref, Col_Ref, stRow);

							stRow += rowCnt;
						}
					}
				}
			}
			stRow++;

			if ( ResD.AnchorD[j].FACD.bAnchorRebar )
			{
				if(BsedD.nDesignCode == KALIS_2023)
				{
					RptBlock = _T("S7_2_3_1_8_2023");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_8_2023_nas"), ResD.AnchorD[j].FACD.nnas, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_8_2023_Asas"), m_dFactorAreamm2* ResD.AnchorD[j].FACD.dAsas, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_8_2023_fya"), m_dFactorStress* ResD.AnchorD[j].FACD.dfya, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_8_2023_Vbrks"), ResD.AnchorD[j].FACD.dVsav, Row_Ref, Col_Ref, stRow);

					stRow += rowCnt;
					stRow++;
				}
				else
				{
					RptBlock = _T("S7_2_3_1_7");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_7_nas"), ResD.AnchorD[j].FACD.nnas, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_7_Asas"), m_dFactorAreamm2 * ResD.AnchorD[j].FACD.dAsas, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_7_fya"), m_dFactorStress * ResD.AnchorD[j].FACD.dfya, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_7_Vsav"), ResD.AnchorD[j].FACD.dVsav, Row_Ref, Col_Ref, stRow);

					PrintCellData(pXL, _T("S7_2_3_1_7_Vcbg_Vcbg"), ResD.AnchorD[j].FACD.dVcbg, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_7_Vcbg_Vsav"), ResD.AnchorD[j].FACD.dVsav, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_3_1_7_Vcbg"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);

					stRow += rowCnt;
					stRow++;
				}
			}

			if(BsedD.nDesignCode == KALIS_2023)
			{
				RptBlock = _T("S7_2_3_1_9_2023");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;
				if(ResD.AnchorD[j].FACD.bAnchorRebar)
				{
					if(j == 0)
					{
						RptBlock = _T("S7_2_3_1_9_2023_Vbrkc2_L");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrkec_L"), ResD.AnchorD[j].FACD.dVbrkec, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrks_L"), ResD.AnchorD[j].FACD.dVsav, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrkc2_val_L"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_3_1_9_2023_Vbrkc2_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrkec_T"), ResD.AnchorD[j].FACD.dVbrkec, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrks_T"), ResD.AnchorD[j].FACD.dVsav, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrkc2_val_T"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
				else
				{
					if(j == 0)
					{
						RptBlock = _T("S7_2_3_1_9_2023_Vbrkc1_L");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrkc1_val_L"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_3_1_9_2023_Vbrkc1_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_3_1_9_2023_Vbrkc1_val_T"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
			}
			else
			{
				RptBlock = _T("S7_2_3_1_8");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;
				if(j == 0)
				{
					RptBlock = _T("S7_2_3_1_8_L");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_8_L_FACC"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					RptBlock = _T("S7_2_3_1_8_T");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_3_1_8_T_FACC"), ResD.AnchorD[j].FACD.dFACC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
			}
			stRow++;
		}
		else
		{
			RptBlock = _T("[7_2]SkipCheck");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}
	}

	RptBlock = _T("[7_2_3_2]FACD_M");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_3_2_1]FACD_M_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[0].FACD.nTotalBearingNumb > 0 )
	{
		RptBlock = _T("[7_2_3_2_1]FACD_M_Longi");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_3_2_FASD_L"), ResD.AnchorD[0].FACD.dFASD, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_2_nBC_L"), ResD.AnchorD[0].FACD.nnBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_2_nB_L"), ResD.AnchorD[0].FACD.nTotalBearingNumb, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_2_FACD_M_L"), ResD.AnchorD[0].FACD.dFACD, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[7_2_3_2_2]FACD_M_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[1].FACD.nTotalBearingNumb > 0 )
	{
		RptBlock = _T("[7_2_3_2_2]FACD_M_Trans");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_3_2_FASD_T"), ResD.AnchorD[1].FACD.dFASD, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_2_nBC_T"), ResD.AnchorD[1].FACD.nnBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_2_nB_T"), ResD.AnchorD[1].FACD.nTotalBearingNumb, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_2_FACD_M_T"), ResD.AnchorD[1].FACD.dFACD, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;

	RptBlock = _T("[7_2_3_3]FAC_Eval");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_3_3_1]FAC_Eval_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[0].FACD.nTotalBearingNumb > 0 )
	{
		RptBlock = _T("[7_2_3_3_1]FAC_Eval_Longi");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_3_3_FACC_L"), ResD.AnchorD[0].FACD.dFACC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_3_FACD_L"), ResD.AnchorD[0].FACD.dFACD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.AnchorD[0].FACD.dFACC, ResD.AnchorD[0].FACD.dFACD))
		{
			PrintCellData(pXL, _T("S7_2_3_3_FACR_L"), ResD.AnchorD[0].FACD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_2_3_3_FACR_ResultL"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FACD.dRat), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[7_2_3_3_2]FAC_Eval_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[1].FACD.nTotalBearingNumb > 0 )
	{
		RptBlock = _T("[7_2_3_3_2]FAC_Eval_Trans");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_3_3_FACC_T"), ResD.AnchorD[1].FACD.dFACC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_3_3_FACD_T"), ResD.AnchorD[1].FACD.dFACD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.AnchorD[1].FACD.dFACC, ResD.AnchorD[1].FACD.dFACD))
		{
			PrintCellData(pXL, _T("S7_2_3_3_FACR_T"), ResD.AnchorD[1].FACD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_2_3_3_FACR_ResultT"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FACD.dRat), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorPryout(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BREV_K& BrevK, const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	T_BSED_D BsedD;
	if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

	// 4) 콘크리트 프라이아웃
	CString RptBlock = BsedD.nDesignCode == KALIS_2023 ? _T("[7_2_4]Titile_Concrete_Pryout_2023") : _T("[7_2_4]Titile_Concrete_Pryout");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;
	stRow++;

	for ( int j = 0; j < 2; ++j ) // Longitudinal, Transverse
	{
		RptBlock = j == 0 ? _T("[7_2_4_1]Titile_Pryout_L") : _T("[7_2_4_1]Titile_Pryout_T");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		RptBlock = _T("[7_2_4_1]Pryout_Pic_Data");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		if ( TRUE )//!ResD.ParamD.bUserPryOut)
		{
			T_UNIT_INDEX CurIndex, CngIndex;
			m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
			CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);

			T_ANEV_D AnevD;
			T_ANEV_K AnevK = GetAnevByBrev(BrevK, AnevD);

			CString strFileName;
			strFileName = AnevD.strName;
			if ( !CFileCtrl::ConvertToValidFileName(strFileName) )
				return FALSE;

			CString strPicFileName;
			strPicFileName.Format(_T("%s%s_AnchorPryout_%d.emf"), m_strPicPath, strFileName, j + 1);

			CDgnSeisAnchor2FailDlg dlg;
			dlg.m_Data = AnevD;
			dlg.m_bIsReport = TRUE;
			dlg.m_nRatTypeFail[0] = ResD.AnchorD[0].FACD.nRatType;
			dlg.m_nRatTypeFail[1] = ResD.AnchorD[1].FACD.nRatType;
			dlg.m_nRatTypeFail[2] = ResD.AnchorD[0].FAPD.nRatType;
			dlg.m_nRatTypeFail[3] = ResD.AnchorD[1].FAPD.nRatType;
			dlg.MakeEmfAndClose(strPicFileName, 1, j);
			dlg.DoModal();
			::Sleep(1000);

			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex);

			PrintBearingProjFig(pXL, strPicFileName, stRow);
		}

		stRow += rowCnt;

		const int nAnchorParamLongi = ResD.AnchorParamPryLongi.GetSize();
		const int nAnchorParamTrans = ResD.AnchorParamPryTrans.GetSize();

		int nAnchorParam = 0;

		T_BRDGEVAL_ANCHOR_PARAM AnchorParam;

		if ( j == 0 )
		{
			nAnchorParam = nAnchorParamLongi;
			if ( nAnchorParam > 0 ) AnchorParam = ResD.AnchorParamPryLongi[0];
		}
		else
		{
			nAnchorParam = nAnchorParamTrans;
			if ( nAnchorParam > 0 ) AnchorParam = ResD.AnchorParamPryTrans[0];

		}

		if ( nAnchorParam < 2 )
		{
			RptBlock = _T("[7_2_4_1_1]Pryout_S_Data");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

			if ( j == 0 )
			{
				PrintCellData(pXL, _T("S7_2_4_1_S1"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.ds1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_S2"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.ds2, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pXL, _T("S7_2_4_1_S1"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.ds2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_S2"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.ds1, Row_Ref, Col_Ref, stRow);
			}
			PrintCellData(pXL, _T("S7_2_4_1_Sax"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dsax, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_1_Say"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dsay, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}

		RptBlock = _T("[7_2_4_1_2]Pryout_Ca_Data");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pXL, _T("S7_2_4_1_ca1"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dca1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_4_1_ca1p"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dca1p, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_4_1_ca2"), m_dFactorLengthMM *ResD.AnchorD[j].FAPD.dca2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_4_1_ca2p"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dca2p, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		int nBCL = static_cast<int>(ResD.ParamD.nnBCL);
		int nBCT = static_cast<int>(ResD.ParamD.nnBCT);
		int nB = static_cast<int>(j == 0 ? AnchorParam.nnT : AnchorParam.nnL);
		if ( nB == 1 )
		{
			if ( j == 0 )
			{
				if ( nBCT > 1 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
				{
					nBCT -= 1;
				}
			}
			else
			{
				if ( nBCL > 1 ) // 가력방향 반대편 앵커열의 앵커가 1개인 경우 제외
				{
					nBCL -= 1.;
				}
			}
		}

		if ( ResD.AnchorD[j].FAPD.nnBC_org > 0 )  // ResD.AnchorD[j].FAPD.nnBC_org > 1
		{
			// ▶ 단일 앵커 및 앵커 그룹 판단
			RptBlock = _T("S7_2_4_1_2");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;

			if ( nAnchorParam < 2 )
			{
				if ( ResD.AnchorD[j].FAPD.nANcType == 0 )
				{
					RptBlock = _T("S7_2_4_1_2_Nontypical");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;

					if ( ResD.AnchorD[j].FAPD.n1AnchorType_org == RESIS_MULTIANCHOR )
					{
						RptBlock = _T("S7_2_4_1_2_Group");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_2_Group_Num"), ResD.AnchorD[j].FAPD.nnBC_org, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_4_1_2_Anchor");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_2_Anchor_Num"), ResD.AnchorD[j].FAPD.nnBC_org, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
				else if ( ResD.AnchorD[j].FAPD.n1AnchorType_org == RESIST_1ANCHOR_1 )
				{
					RptBlock = _T("S7_2_4_1_2_1anchor");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_4_1_2_Anchor");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_2_Anchor_Num"), ResD.AnchorD[j].FAPD.nnBC_org, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					if ( ResD.AnchorD[j].FAPD.ds1 > cDGN_Zero )
					{
						if ( ResD.AnchorD[j].FAPD.ds1 < 2.0*1.5*AnchorParam.dhefRe )
						{
							RptBlock = j == 0 ? _T("S7_2_4_1_2_s1_LT") : _T("S7_2_4_1_2_s2_LT");
						}
						else
						{
							RptBlock = j == 0 ? _T("S7_2_4_1_2_s1_ME") : _T("S7_2_4_1_2_s2_ME");
						}
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						stRow += rowCnt;
					}

					if ( j == 0 )
					{
						if ( ResD.AnchorD[j].FAPD.dsax > cDGN_Zero )
						{
							if ( ResD.AnchorD[j].FAPD.dsax < 2.0*1.5*AnchorParam.dhefRe )
							{
								RptBlock = _T("S7_2_4_1_2_sax_LT");
							}
							else
							{
								RptBlock = _T("S7_2_4_1_2_sax_ME");
							}
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							stRow += rowCnt;
						}

						if ( nBCT > 1 )
						{
							double ds2say = fabs(ResD.AnchorD[j].FAPD.ds2*(ResD.AnchorD[j].FAPD.nnB - 1.0) + ResD.AnchorD[j].FAPD.dsay);
							if ( ds2say > cDGN_Zero )
							{
								if ( ds2say < 2.0*1.5*AnchorParam.dhefRe )
								{
									RptBlock = _T("S7_2_4_1_2_s1say_LT");
								}
								else
								{
									RptBlock = _T("S7_2_4_1_2_s1say_ME");
								}
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								stRow += rowCnt;
							}
						}
					}
					else
					{
						if ( ResD.AnchorD[j].FAPD.dsay > cDGN_Zero )
						{
							if ( ResD.AnchorD[j].FAPD.dsay < 2.0*1.5* AnchorParam.dhefRe )
							{
								RptBlock = _T("S7_2_4_1_2_say_LT");
							}
							else
							{
								RptBlock = _T("S7_2_4_1_2_say_ME");
							}
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							stRow += rowCnt;
						}

						if ( nBCL > 1 )
						{
							double ds2sax = fabs(ResD.AnchorD[j].FAPD.ds2*(ResD.AnchorD[j].FAPD.nnB - 1.0) + ResD.AnchorD[j].FAPD.dsax);
							if ( ds2sax > cDGN_Zero )
							{
								if ( ds2sax < 2.0*1.5*AnchorParam.dhefRe )
								{
									RptBlock = _T("S7_2_4_1_2_s1sax_LT");
								}
								else
								{
									RptBlock = _T("S7_2_4_1_2_s1sax_ME");
								}
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								stRow += rowCnt;
							}
						}
					}

					if ( ResD.AnchorD[j].FAPD.n1AnchorType_org == RESIST_1ANCHOR_ALL )
					{
						RptBlock = _T("S7_2_4_1_2_Anchor");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_2_Anchor_Num"), ResD.AnchorD[j].FAPD.nnBC_org, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_4_1_2_Group");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_2_Group_Num"), ResD.AnchorD[j].FAPD.nnBC_org, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
			}
			else
			{
				RptBlock = _T("S7_2_4_1_2_Group");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_2_Group_Num"), ResD.AnchorD[j].FAPD.nnBC_org, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}

			stRow++;

			double dmaxs = 0.0;
			if ( ResD.AnchorD[j].FAPD.bEdgeEff )
			{
				if ( ResD.ParamD.bUserPryOut )
				{
					RptBlock = _T("S7_2_4_1_3_NeedButNoRecal");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;
				}
			}

			// ▶ 콘크리트파괴 투영면적 산정
			RptBlock = _T("S7_2_4_1_4");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
			if ( ResD.ParamD.bUserPryOut || ResD.AnchorD[j].FAPD.nANcType == 0 || nAnchorParam > 1 )
			{
				RptBlock = _T("S7_2_4_1_4_Type0");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_4_Type0_Avc"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANc_Cal, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				CString strCell;
				switch ( ResD.AnchorD[j].FAPD.nANcType )
				{
				case 1:
					RptBlock = j == 0 ? _T("S7_2_4_1_4_Type1_L") : _T("S7_2_4_1_4_Type1_T");
					strCell = j == 0 ? _T("S7_2_4_1_4_Type1_L_Avc") : _T("S7_2_4_1_4_Type1_T_Avc");
					break;
				case 2:
					RptBlock = j == 0 ? _T("S7_2_4_1_4_Type2_L") : _T("S7_2_4_1_4_Type2_T");
					strCell = j == 0 ? _T("S7_2_4_1_4_Type2_L_Avc") : _T("S7_2_4_1_4_Type2_T_Avc");
					break;
				case 3:
					RptBlock = j == 0 ? _T("S7_2_4_1_4_Type3_L") : _T("S7_2_4_1_4_Type3_T");
					strCell = j == 0 ? _T("S7_2_4_1_4_Type3_L_Avc") : _T("S7_2_4_1_4_Type3_T_Avc");
					break;
				case 4:
					RptBlock = j == 0 ? _T("S7_2_4_1_4_Type4_L") : _T("S7_2_4_1_4_Type4_T");
					strCell = j == 0 ? _T("S7_2_4_1_4_Type4_L_Avc") : _T("S7_2_4_1_4_Type4_T_Avc");
					break;
				case 5:
					RptBlock = j == 0 ? _T("S7_2_4_1_4_Type5_L") : _T("S7_2_4_1_4_Type5_T");
					strCell = j == 0 ? _T("S7_2_4_1_4_Type5_L_Avc") : _T("S7_2_4_1_4_Type5_T_Avc");
					break;
				case 6:
					RptBlock = j == 0 ? _T("S7_2_4_1_4_Type6_L") : _T("S7_2_4_1_4_Type6_T");
					strCell = j == 0 ? _T("S7_2_4_1_4_Type6_L_Avc") : _T("S7_2_4_1_4_Type6_T_Avc");
					break;
				default:
					ASSERT(0);
					break;
				}
				if ( !strCell.IsEmpty() )
				{
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, strCell, m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANc_Cal, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
			}

			const int iAnchorParaSizeLongi = ResD.AnchorParamPryLongi.GetSize();
			const int iAnchorParaSizeTrans = ResD.AnchorParamPryTrans.GetSize();
			CString strEQSign = _T("");
			CString strPlusSign = _T("+");

			if ( ResD.ParamD.bUserPryOut )
			{
				RptBlock = _T("S7_2_4_1_4_Type0_Anco");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_3_1_4_Type0_Avco_Val"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				if ( nAnchorParam > 1 )
				{
					RptBlock = _T("S7_2_4_1_4_Anco_Total_Equation");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;

					if ( j == LONGI )
					{
						for ( int k = 0; k < iAnchorParaSizeLongi; k++ )
						{
							RptBlock = _T("S7_2_4_1_4_Anco_Total_Start");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

							if ( k == 0 )  strEQSign = _T("=");
							else           strEQSign = _T("");

							if ( k == iAnchorParaSizeLongi - 1 ) strPlusSign = _T("");
							else                                  strPlusSign = _T("+");

							PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_4_1_4_ANco_hef_Each"), ResD.AnchorParamPryLongi[k].dhefRe, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_4_1_4_Anco_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;

							if ( k == iAnchorParaSizeLongi - 1 )
							{
								RptBlock = _T("S7_2_4_1_4_Anco_Total_End");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Total"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANcoSum, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;

								RptBlock = _T("S7_2_4_1_4_Anco_Avg_Form");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Sum"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANcoSum, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Numb"), ResD.AnchorD[j].FAPD.nnBC, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Avg"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;
							}
						}
						stRow++;
					}
					else
					{
						for ( int k = 0; k < iAnchorParaSizeTrans; k++ )
						{
							RptBlock = _T("S7_2_4_1_4_Anco_Total_Start");
							rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
							GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

							if ( k == 0 )  strEQSign = _T("=");
							else           strEQSign = _T("");

							if ( k == iAnchorParaSizeTrans - 1 ) strPlusSign = _T("");
							else                                  strPlusSign = _T("+");

							PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_4_1_4_ANco_hef_Each"), ResD.AnchorParamPryTrans[k].dhefRe, Row_Ref, Col_Ref, stRow);
							PrintCellData(pXL, _T("S7_2_4_1_4_Anco_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
							stRow += rowCnt;

							if ( k == iAnchorParaSizeTrans - 1 )
							{
								RptBlock = _T("S7_2_4_1_4_Anco_Total_End");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Total"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANcoSum, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;

								RptBlock = _T("S7_2_4_1_4_Anco_Avg_Form");
								rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
								GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Sum"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANcoSum, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Numb"), ResD.AnchorD[j].FAPD.nnBC, Row_Ref, Col_Ref, stRow);
								PrintCellData(pXL, _T("S7_2_4_1_4_Anco_Avg"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
								stRow += rowCnt;
							}
						}
						stRow++;
					}
				}
				else
				{
					RptBlock = _T("S7_2_4_1_4_ANco");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANco_hef"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dhef, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANco_ANco"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}

				if ( ResD.AnchorD[j].FAPD.dANc_Cal < ResD.AnchorD[j].FAPD.dANcLim )
				{
					RptBlock = _T("S7_2_4_1_4_ANc_LT");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc_LT_n"), ResD.AnchorD[j].FAPD.nn, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc_LT_ANco"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc_LT_ANc"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANcLim, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_4_1_4_n");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_nA"), ResD.AnchorD[j].FAPD.nnA, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_nBC"), ResD.AnchorD[j].FAPD.nnBC, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_nn"), ResD.AnchorD[j].FAPD.nn, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_4_1_4_ANc1");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc1_ANc"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANc, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					RptBlock = _T("S7_2_4_1_4_ANc_ME");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc_ME_n"), ResD.AnchorD[j].FAPD.nn, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc_ME_ANco"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc_ME_ANc"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANcLim, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_4_1_4_n");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_nA"), ResD.AnchorD[j].FAPD.nnA, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_nBC"), ResD.AnchorD[j].FAPD.nnBC, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_4_nn"), ResD.AnchorD[j].FAPD.nn, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;

					RptBlock = _T("S7_2_4_1_4_ANc2");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_4_ANc2_ANc"), m_dFactorAreamm2*ResD.AnchorD[j].FAPD.dANc, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
			}
			stRow++;

			// ▶ 전단력을 받는 앵커의 콘크리트 프라이아웃 Vcpg
			if ( ResD.AnchorD[j].FAPD.n1AnchorType == RESIST_1ANCHOR_ALL && ResD.AnchorD[j].FAPD.n1AnchorAllResist > 1 )
			{
				RptBlock = _T("S7_2_4_1_5_1Anchor");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_1Anchor_n"), ResD.AnchorD[j].FAPD.n1AnchorAllResist, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_1Anchor_kcp"), ResD.AnchorD[j].FAPD.dkcp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_1Anchor_Ncbg"), ResD.AnchorD[j].FAPD.dNcbg, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_1Anchor_Vcpg"), ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				RptBlock = _T("S7_2_4_1_5");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_kcp"), ResD.AnchorD[j].FAPD.dkcp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Ncbg"), ResD.AnchorD[j].FAPD.dNcbg, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Vcpg"), ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
			}
			stRow += rowCnt;

			if ( m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dhef < 65.0 )
			{
				RptBlock = _T("S7_2_4_1_5_kcp_LT65");
			}
			else
			{
				RptBlock = _T("S7_2_4_1_5_kcp_ME65");
			}
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;

			if(BsedD.nDesignCode == KALIS_2023)
			{
				RptBlock = _T("S7_2_4_1_5_Cal_Ncbg_2023");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_ANc_2023"), m_dFactorAreamm2* ResD.AnchorD[j].FAPD.dANc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_ANco_2023"), m_dFactorAreamm2* ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_psi_edN_2023"), ResD.AnchorD[j].FAPD.dpsi_edN, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_psi_cN_2023"), ResD.AnchorD[j].FAPD.dpsi_cN, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_Nb_2023"), ResD.AnchorD[j].FAPD.dNb, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_Ncbg_2023"), ResD.AnchorD[j].FAPD.dNcbg, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				RptBlock = _T("S7_2_4_1_5_Cal_Ncbg");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_ANc"), m_dFactorAreamm2 * ResD.AnchorD[j].FAPD.dANc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_ANco"), m_dFactorAreamm2 * ResD.AnchorD[j].FAPD.dANco, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_psi"), ResD.AnchorD[j].FAPD.dpsi_edN, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_Nb"), ResD.AnchorD[j].FAPD.dNb, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_Ncbg"), ResD.AnchorD[j].FAPD.dNcbg, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}

			double dcamin = min(min(ResD.AnchorD[j].FAPD.dca1, ResD.AnchorD[j].FAPD.dca1p),
				min(ResD.AnchorD[j].FAPD.dca2, ResD.AnchorD[j].FAPD.dca2p));
			if ( dcamin < 1.5*ResD.AnchorD[j].FAPD.dhef )
			{
				RptBlock = _T("S7_2_4_1_5_Cal_Ncbg_psi_LT");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_psi_LT_camin"), m_dFactorLengthMM*dcamin, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_psi_LT_15hef"), m_dFactorLengthMM*1.5*ResD.AnchorD[j].FAPD.dhef, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_Cal_Ncbg_psi_LT_psi"), ResD.AnchorD[j].FAPD.dpsi_edN, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			else
			{
				RptBlock = _T("S7_2_4_1_5_Cal_Ncbg_psi_ME");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;
			}

			if(BsedD.nDesignCode == KALIS_2023)
			{
				switch (ResD.AnchorParamAll[0].enAnchType)
				{
					case ANCH_CAST_HEADED_STUD: // Cast-In-Place Anchor Bolt  "Headed Stud"
					case ANCH_CAST_HEADED_BOLT: // Cast-In-Place Anchor Bolt  "Headed Bolt"
					case ANCH_CAST_HOOKEDBOLTL: // Cast-In-Place Anchor Bolt  "Hooked Bolt-L"
					case ANCH_CAST_HOOKEDBOLTJ: // Cast-In-Place Anchor Bolt  "Hooked Bolt-J"
						RptBlock = _T("S7_2_4_1_5_Cal_psi_cN1_2023"); // 선설치
						break;
					case ANCH_POST_EXPANSIONTC: // Post-Installed Anchor Bolt "Torque-controlled anchors"
					case ANCH_POST_UNDERCUTANC: // Post-Installed Anchor Bolt "Undercut anchors"
					case ANCH_POST_EXPANSIONDC: // Post-Installed Anchor Bolt "Displacement-controlled anchors"
					case ANCH_POST_ADHESIVEANC: // Post-Installed Anchor Bolt "Adhesive anchors"
						RptBlock = _T("S7_2_4_1_5_Cal_psi_cN2_2023"); // 후설치
						break;
					default:
						ASSERT(0);
						RptBlock = _T("S7_2_4_1_5_Cal_psi_cN1_2023");
						break;

				}
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;
			}

			if ( (j == LONGI && iAnchorParaSizeLongi < 2) || (j == TRANS && iAnchorParaSizeTrans < 2) )
			{
				if ( ResD.AnchorD[j].FAPD.nNbType == 1 )
				{
					RptBlock = _T("S7_2_4_1_5_Nb1");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_hef"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dhef, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_Nb"), ResD.AnchorD[j].FAPD.dNb, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else if ( ResD.AnchorD[j].FAPD.nNbType == 2 )
				{
					RptBlock = _T("S7_2_4_1_5_Nb2");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_hef"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dhef, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_Nb"), ResD.AnchorD[j].FAPD.dNb, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else if ( ResD.AnchorD[j].FAPD.nNbType == 3 )
				{
					RptBlock = _T("S7_2_4_1_5_Nb3");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_hef"), m_dFactorLengthMM*ResD.AnchorD[j].FAPD.dhef, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_Nb"), ResD.AnchorD[j].FAPD.dNb, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else ASSERT(0);
			}
			else
			{
				int iAnchorParaSizeCal = 0;
				if ( j == LONGI ) iAnchorParaSizeCal = iAnchorParaSizeLongi;
				else           iAnchorParaSizeCal = iAnchorParaSizeTrans;

				int nNbTypeTotal = 0;
				if ( iAnchorParaSizeCal > 0 )
				{
					nNbTypeTotal = (j == LONGI) ? ResD.AnchorParamPryLongi[0].nNbType : ResD.AnchorParamPryTrans[0].nNbType;
				}

				RptBlock = _T("S7_2_4_1_5_Nb1_Total_Equation");
				if ( nNbTypeTotal == 1 ) RptBlock = _T("S7_2_4_1_5_Nb1_Total_Equation");
				else if ( nNbTypeTotal == 2 ) RptBlock = _T("S7_2_4_1_5_Nb2_Total_Equation");
				else if ( nNbTypeTotal == 3 ) RptBlock = _T("S7_2_4_1_5_Nb3_Total_Equation");

				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				stRow += rowCnt;

				for ( int k = 0; k < iAnchorParaSizeCal; k++ )
				{
					int nNbType = 1;
					double dhefRe = 0.0;
					if ( j == LONGI )
					{
						nNbType = ResD.AnchorParamPryLongi[k].nNbType;
						dhefRe = ResD.AnchorParamPryLongi[k].dhefRe;
					}
					else
					{
						nNbType = ResD.AnchorParamPryTrans[k].nNbType;
						dhefRe = ResD.AnchorParamPryTrans[k].dhefRe;
					}

					if ( nNbType == 1 ) RptBlock = _T("S7_2_4_1_5_Nb1_Total_Start");
					else if ( nNbType == 2 ) RptBlock = _T("S7_2_4_1_5_Nb2_Total_Start");
					else if ( nNbType == 3 ) RptBlock = _T("S7_2_4_1_5_Nb3_Total_Start");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

					if ( k == 0 )  strEQSign = _T("=");
					else         strEQSign = _T("");

					if ( k == iAnchorParaSizeCal - 1 ) strPlusSign = _T("");
					else                             strPlusSign = _T("+");

					if ( nNbType == 1 )
					{
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_Total_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_Total_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_Total_hef"), dhefRe, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb1_Tota_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
					}
					else if ( nNbType == 2 )
					{
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_Total_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_Total_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_Total_hef"), dhefRe, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb2_Tota_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
					}
					else if ( nNbType == 3 )
					{
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_Total_EQ"), strEQSign, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_Total_fck"), m_dFactorStress*ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_Total_hef"), dhefRe, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb3_Tota_Plus"), strPlusSign, Row_Ref, Col_Ref, stRow);
					}
					else ASSERT(0);

					stRow += rowCnt;

					if ( k == iAnchorParaSizeCal - 1 )
					{
						RptBlock = _T("S7_2_4_1_5_Nb_Total_Last");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb_Total_Sum"), ResD.AnchorD[j].FAPD.dNbSum, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;

						RptBlock = _T("S7_2_4_1_5_Nb_Total_End");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb_Sum"), ResD.AnchorD[j].FAPD.dNbSum, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb_Num"), ResD.AnchorD[j].FAPD.nnBC, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_5_Nb_Avg"), ResD.AnchorD[j].FAPD.dNb, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
			}
			if(BsedD.nDesignCode == KALIS_2023)
			{
				RptBlock = _T("S7_2_4_1_5_Vpryec");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_Vpryec_Val"), ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;
			}
			stRow++;

			if(BsedD.nDesignCode == KALIS_2023)
			{
				RptBlock = _T("S7_2_4_1_5_1_Vpry_EffD");
				rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
				GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pXL, _T("S7_2_4_1_5_1_D"), ResD.AnchorD[j].FAPD.dD, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_1_BC"), ResD.AnchorD[j].FAPD.nnBC, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_1_B"), ResD.AnchorD[j].FAPD.nTotalBearing, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_2_4_1_5_1_Dbc"), ResD.AnchorD[j].FAPD.dDbc, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt;

				if(ResD.AnchorD[j].FAPD.enIgnore == BEARING_PRYOUT_CONSIDER)
				{
					RptBlock = _T("S7_2_4_1_5_1_Consider");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_5_1_Consider_Dbc"), ResD.AnchorD[j].FAPD.dDbc, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_1_Consider_Vpryec14"), 0.25 * ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
				}
				else
				{
					RptBlock = _T("S7_2_4_1_5_1_Ignore");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_5_1_Ignore_Dbc"), ResD.AnchorD[j].FAPD.dDbc, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_5_1_Ignore_Vpryec14"), 0.25 * ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
				}
				stRow += rowCnt;

				if(ResD.AnchorD[j].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER)
				{
					RptBlock = _T("[7_2]Pryout_Ignore_Opt");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					stRow += rowCnt;
				}

				stRow++;

				if(ResD.AnchorD[j].FAPD.enIgnore == BEARING_PRYOUT_IGNORE) continue;
			}

			// ▶ 인장에 저항하는 앵커철근 고려
			if ( ResD.AnchorD[j].FAPD.bAnchorRebar )
			{
				if(BsedD.nDesignCode == KALIS_2023)
				{
					RptBlock = _T("S7_2_4_1_6_2023");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_6_Vprys_Asat"), m_dFactorAreamm2* ResD.AnchorD[j].FAPD.dAsat, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_6_Vprys_fya"), m_dFactorStress* ResD.AnchorD[j].FAPD.dfya, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_6_Vprys"), ResD.AnchorD[j].FAPD.dVsat, Row_Ref, Col_Ref, stRow);
				}
				else
				{
					RptBlock = _T("S7_2_4_1_6");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_6_Asat"), m_dFactorAreamm2 * ResD.AnchorD[j].FAPD.dAsat, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_6_fya"), m_dFactorStress * ResD.AnchorD[j].FAPD.dfya, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_6_Vsat"), ResD.AnchorD[j].FAPD.dVsat, Row_Ref, Col_Ref, stRow);

					PrintCellData(pXL, _T("S7_2_4_1_6_Vcpg_org"), ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_6_kcpVsat"), ResD.AnchorD[j].FAPD.dkcp * ResD.AnchorD[j].FAPD.dVsat, Row_Ref, Col_Ref, stRow);
					PrintCellData(pXL, _T("S7_2_4_1_6_Vcpg"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
				}
				stRow += rowCnt;
				stRow++;
			}
			
			// ▶ 전단력을 받는 앵커의 콘크리트 프라이아웃 공급역량
			RptBlock = _T("S7_2_4_1_7");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
			if(BsedD.nDesignCode == KALIS_2023)
			{
				if ( ResD.AnchorD[j].FAPD.bAnchorRebar )
				{
					if(j == 0)
					{
						RptBlock = _T("S7_2_4_1_7_Vpryc2_L");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc2_Vcpg_L"), ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc2_Vprys_L"), ResD.AnchorD[j].FAPD.dVsat, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc2_Vpryc_L"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_4_1_7_Vpryc2_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc2_Vcpg_T"), ResD.AnchorD[j].FAPD.dVcpg, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc2_Vprys_T"), ResD.AnchorD[j].FAPD.dVsat, Row_Ref, Col_Ref, stRow);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc2_Vpryc_T"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
				else
				{
					if(j == 0)
					{
						RptBlock = _T("S7_2_4_1_7_Vpryc1_L");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc1_Vcpg_L"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
					else
					{
						RptBlock = _T("S7_2_4_1_7_Vpryc1_T");
						rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
						GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
						PrintCellData(pXL, _T("S7_2_4_1_7_Vpryc1_Vcpg_T"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
						stRow += rowCnt;
					}
				}
				stRow++;
			}
			else
			{
				if(j == 0)
				{
					RptBlock = _T("S7_2_4_1_7_FAPCL");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_7_VcpgL"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				else
				{
					RptBlock = _T("S7_2_4_1_7_FAPCT");
					rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
					GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
					PrintCellData(pXL, _T("S7_2_4_1_7_VcpgT"), ResD.AnchorD[j].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
					stRow += rowCnt;
				}
				stRow++;
			}
		}
		else
		{
			RptBlock = _T("[7_2]SkipCheck");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}
	}

	RptBlock = _T("[7_2_4_2]FAPD_M");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_4_2_1]FAPD_M_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[0].FAPD.nTotalAnchorNumb )
	{
		if(ResD.AnchorD[0].FAPD.enIgnore == BEARING_PRYOUT_IGNORE)
		{
			RptBlock = _T("[7_2]Pryout_Ignore");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[7_2_4_2_1]FAPD_M_Longi");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_4_2_L_FASD"), ResD.AnchorD[0].FAPD.dFASD, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_2_L_nBC"), ResD.AnchorD[0].FAPD.nn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_2_L_nB"), ResD.AnchorD[0].FAPD.nTotalAnchorNumb, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_2_L_FAPD_M"), ResD.AnchorD[0].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[7_2_4_2_2]FAPD_M_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[1].FAPD.nTotalAnchorNumb )
	{
		if(ResD.AnchorD[1].FAPD.enIgnore == BEARING_PRYOUT_IGNORE)
		{
			RptBlock = _T("[7_2]Pryout_Ignore");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[7_2_4_2_2]FAPD_M_Trans");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_4_2_T_FASD"), ResD.AnchorD[1].FAPD.dFASD, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_2_T_nBC"), ResD.AnchorD[1].FAPD.nn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_2_T_nB"), ResD.AnchorD[1].FAPD.nTotalAnchorNumb, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_2_T_FAPD_M"), ResD.AnchorD[1].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;
	//////////////////////////////////////////////////////////////////////////

	// 다) 콘크리트파괴 성능평가
	RptBlock = _T("[7_2_4_3]FAPC");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_4_3_1]FAPC_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[0].FAPD.nTotalAnchorNumb > 0.0 )
	{
		if(ResD.AnchorD[0].FAPD.enIgnore == BEARING_PRYOUT_IGNORE)
		{
			RptBlock = _T("[7_2]Pryout_Ignore");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[7_2_4_3_1]FAPC_Longi");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_4_3_L_FAPC"), ResD.AnchorD[0].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_3_L_FAPD"), ResD.AnchorD[0].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[0].FAPD.dFAPC, ResD.AnchorD[0].FAPD.dFAPD))
			{
				PrintCellData(pXL, _T("S7_2_4_3_L_FAPR"), ResD.AnchorD[0].FAPD.dRat, Row_Ref, Col_Ref, stRow);
				CString strSign = Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FAPD.dRat);
				if(ResD.ParamD.iEvalCode == KALIS_2023)
				{
					if(ResD.AnchorD[0].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER) strSign += _T("*");
				}
				PrintCellDataMark(pXL, _T("S7_2_4_3_L_FAPD_CHK"), strSign, Row_Ref, Col_Ref, stRow);
			}
			stRow += rowCnt;
		}
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[7_2_4_3_2]FAPC_Title");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[1].FAPD.nTotalAnchorNumb > 0.0 )
	{
		if(ResD.AnchorD[1].FAPD.enIgnore == BEARING_PRYOUT_IGNORE)
		{
			RptBlock = _T("[7_2]Pryout_Ignore");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[7_2_4_3_2]FAPC_Trans");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_4_3_T_FAPC"), ResD.AnchorD[1].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_4_3_T_FAPD"), ResD.AnchorD[1].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[1].FAPD.dFAPC, ResD.AnchorD[1].FAPD.dFAPD))
			{
				PrintCellData(pXL, _T("S7_2_4_3_T_FAPR"), ResD.AnchorD[1].FAPD.dRat, Row_Ref, Col_Ref, stRow);
				CString strSign = Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FAPD.dRat);
				if(ResD.ParamD.iEvalCode == KALIS_2023)
				{
					if(ResD.AnchorD[1].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER) strSign += _T("*");
				}
				PrintCellDataMark(pXL, _T("S7_2_4_3_T_FAPD_CHK"), strSign, Row_Ref, Col_Ref, stRow);
			}
			stRow += rowCnt;
		}
	}
	else
	{
		RptBlock = _T("[7_2]SkipCheck");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	stRow++;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }
	//========================================================================================================================

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorSummary(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	T_BSED_D BsedD;
	if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
	
	CString strDash = _T("-");

	// 7.3 받침부 평가 요약
	CString RptBlock = _T("[7_3]FA_Summary");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_3_FBCL"), ResD.BodyD[0].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_3_FBDL"), ResD.BodyD[0].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.BodyD[0].HorD.dFBC, ResD.BodyD[0].HorD.dFBD))
	{
		PrintCellData(pXL, _T("S7_3_FBRL"), ResD.BodyD[0].HorD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_3_FBRL_CHK"), Chk_OK_Sign(0, 1.0, ResD.BodyD[0].HorD.dRat), Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pXL, _T("S7_3_FBCT"), ResD.BodyD[1].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_3_FBDT"), ResD.BodyD[1].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.BodyD[1].HorD.dFBC, ResD.BodyD[1].HorD.dFBD))
	{
		PrintCellData(pXL, _T("S7_3_FBRT"), ResD.BodyD[1].HorD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_3_FBRT_CHK"), Chk_OK_Sign(0, 1.0, ResD.BodyD[1].HorD.dRat), Row_Ref, Col_Ref, stRow);
	}

	if ( ResD.BodyD[0].DispD.bCheck )
	{
		PrintCellData(pXL, _T("S7_3_FBCL_SD"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FBDL_SD"), m_dFactorLengthMM*ResD.BodyD[0].DispD.dFBD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.BodyD[0].DispD.dFBC, ResD.BodyD[0].DispD.dFBD))
		{
			PrintCellData(pXL, _T("S7_3_FBRL_SD"), ResD.BodyD[0].DispD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_3_FBRL_SD_CHK"), Chk_OK_Sign(0, 1.0, ResD.BodyD[0].DispD.dRat), Row_Ref, Col_Ref, stRow);
		}
	}
	if ( ResD.BodyD[1].DispD.bCheck )
	{
		PrintCellData(pXL, _T("S7_3_FBCT_SD"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FBDT_SD"), m_dFactorLengthMM*ResD.BodyD[1].DispD.dFBD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.BodyD[1].DispD.dFBC, ResD.BodyD[1].DispD.dFBD))
		{
			PrintCellData(pXL, _T("S7_3_FBRT_SD"), ResD.BodyD[1].DispD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_3_FBRT_SD_CHK"), Chk_OK_Sign(0, 1.0, ResD.BodyD[1].DispD.dRat), Row_Ref, Col_Ref, stRow);
		}
	}

	PrintCellData(pXL, _T("S7_3_FASCL"), ResD.AnchorD[0].FASD.dFASC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_3_FASDL"), ResD.AnchorD[0].FASD.dFASD, Row_Ref, Col_Ref, stRow);

	if(BsedD.nDesignCode == KALIS_2023)
	{
		PrintCellData(pXL, _T("S7_3_FACC_NAME"), _T("콘크리트\n브레이크아웃"), Row_Ref, Col_Ref, stRow);
	}
	if ( ResD.AnchorD[0].FACD.nTotalBearingNumb > 0.0 )
	{
		PrintCellData(pXL, _T("S7_3_FACCL"), ResD.AnchorD[0].FACD.dFACC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FACDL"), ResD.AnchorD[0].FACD.dFACD, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pXL, _T("S7_3_FACCL"), strDash, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FACDL"), strDash, Row_Ref, Col_Ref, stRow);
	}

	if ( ResD.AnchorD[0].FAPD.nTotalAnchorNumb > 0.0 )
	{
		PrintCellData(pXL, _T("S7_3_FAPCL"), ResD.AnchorD[0].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FAPDL"), ResD.AnchorD[0].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pXL, _T("S7_3_FAPCL"), strDash, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FAPDL"), strDash, Row_Ref, Col_Ref, stRow);
	}

	if(IsPrintMark(ResD.AnchorD[0].FASD.dFASC, ResD.AnchorD[0].FASD.dFASD))
	{
		PrintCellData(pXL, _T("S7_3_FASRL"), ResD.AnchorD[0].FASD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_3_FASRL_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FASD.dRat), Row_Ref, Col_Ref, stRow);
	}

	if(IsPrintMark(ResD.AnchorD[0].FACD.dFACC, ResD.AnchorD[0].FACD.dFACD))
	{
		PrintCellData(pXL, _T("S7_3_FACRL"), ResD.AnchorD[0].FACD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_3_FACRL_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FACD.dRat), Row_Ref, Col_Ref, stRow);
	}

	bool bAddPryoutCmt = false;
	if(IsPrintMark(ResD.AnchorD[0].FAPD.dFAPC, ResD.AnchorD[0].FAPD.dFAPD))
	{
		PrintCellData(pXL, _T("S7_3_FAPRL"), ResD.AnchorD[0].FAPD.dRat, Row_Ref, Col_Ref, stRow);
		CString strSign = Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FAPD.dRat);
		if(ResD.ParamD.iEvalCode == KALIS_2023)
		{
			if(ResD.AnchorD[0].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER) strSign += _T("*");
			bAddPryoutCmt = true;
		}
		PrintCellDataMark(pXL, _T("S7_3_FAPRL_CHK"), strSign, Row_Ref, Col_Ref, stRow);
	}


	PrintCellData(pXL, _T("S7_3_FASCT"), ResD.AnchorD[1].FASD.dFASC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_3_FASDT"), ResD.AnchorD[1].FASD.dFASD, Row_Ref, Col_Ref, stRow);

	if ( ResD.AnchorD[1].FACD.nTotalBearingNumb > 0.0 )
	{
		PrintCellData(pXL, _T("S7_3_FACCT"), ResD.AnchorD[1].FACD.dFACC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FACDT"), ResD.AnchorD[1].FACD.dFACD, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pXL, _T("S7_3_FACCT"), strDash, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FACDT"), strDash, Row_Ref, Col_Ref, stRow);
	}

	if ( ResD.AnchorD[1].FAPD.nTotalAnchorNumb > 0.0 )
	{
		PrintCellData(pXL, _T("S7_3_FAPCT"), ResD.AnchorD[1].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FAPDT"), ResD.AnchorD[1].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pXL, _T("S7_3_FAPCT"), strDash, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_3_FAPDT"), strDash, Row_Ref, Col_Ref, stRow);
	}

	if(IsPrintMark(ResD.AnchorD[1].FASD.dFASC, ResD.AnchorD[1].FASD.dFASD))
	{
		PrintCellData(pXL, _T("S7_3_FASRT"), ResD.AnchorD[1].FASD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_3_FASRT_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FASD.dRat), Row_Ref, Col_Ref, stRow);
	}

	if(IsPrintMark(ResD.AnchorD[1].FACD.dFACC, ResD.AnchorD[1].FACD.dFACD))
	{
		PrintCellData(pXL, _T("S7_3_FACRT"), ResD.AnchorD[1].FACD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_3_FACRT_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FACD.dRat), Row_Ref, Col_Ref, stRow);
	}

	if(IsPrintMark(ResD.AnchorD[1].FAPD.dFAPC, ResD.AnchorD[1].FAPD.dFAPD))
	{
		PrintCellData(pXL, _T("S7_3_FAPRT"), ResD.AnchorD[1].FAPD.dRat, Row_Ref, Col_Ref, stRow);
		CString strSign = Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FAPD.dRat);
		if(ResD.ParamD.iEvalCode == KALIS_2023)
		{
			if(ResD.AnchorD[1].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER) strSign += _T("*");
			bAddPryoutCmt = true;
		}
		PrintCellDataMark(pXL, _T("S7_3_FAPRT_CHK"), strSign, Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	// KISTEC2019 인장검토
	if ( (BsedD.nDesignCode == KISTEC_2019 || BsedD.nDesignCode == KALIS_2023) && 
		(ResD.AnchorD[0].bCheckN || ResD.AnchorD[1].bCheckN) )
	{
		CString RptBlock = _T("[7_3]FA_Summary_Tension");
		int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		 
		if ( ResD.AnchorD[0].bCheckN )
		{
			PrintCellData(pXL, _T("S7_3_FNSC"), ResD.AnchorD[0].FNSD.dFNSC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNSD"), ResD.AnchorD[0].FNSD.dFNSD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[0].FNSD.dFNSC, ResD.AnchorD[0].FNSD.dFNSD))
			{
				PrintCellData(pXL, _T("S7_3_FNSR"), ResD.AnchorD[0].FNSD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pXL, _T("S7_3_FNSR_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNSD.dRat), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pXL, _T("S7_3_FNCCL"), ResD.AnchorD[0].FNCD.dFNCC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNCDL"), ResD.AnchorD[0].FNCD.dFNCD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[0].FNCD.dFNCC, ResD.AnchorD[0].FNCD.dFNCD))
			{
				PrintCellData(pXL, _T("S7_3_FNCRL"), ResD.AnchorD[0].FNCD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pXL, _T("S7_3_FNCRL_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNCD.dRat), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pXL, _T("S7_3_FNPC"), ResD.AnchorD[0].FNPD.dFNPC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNPD"), ResD.AnchorD[0].FNPD.dFNPD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[0].FNPD.dFNPC, ResD.AnchorD[0].FNPD.dFNPD))
			{
				PrintCellData(pXL, _T("S7_3_FNPR"), ResD.AnchorD[0].FNPD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pXL, _T("S7_3_FNPR_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNPD.dRat), Row_Ref, Col_Ref, stRow);
			}

			if ( ResD.AnchorD[0].FNRD.bCheck )
			{
				PrintCellData(pXL, _T("S7_3_FNRCL"), ResD.AnchorD[0].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_3_FNRDL"), ResD.AnchorD[0].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
				if(IsPrintMark(ResD.AnchorD[0].FNRD.dFNRC, ResD.AnchorD[0].FNRD.dFNRD))
				{
					PrintCellData(pXL, _T("S7_3_FNRRL"), ResD.AnchorD[0].FNRD.dRat, Row_Ref, Col_Ref, stRow);
					PrintCellDataMark(pXL, _T("S7_3_FNRRL_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNRD.dRat), Row_Ref, Col_Ref, stRow);
				}
			}
			double dRat = ResD.AnchorD[0].FNVD.dRat / ResD.AnchorD[0].FNVD.dRatLim;
			PrintCellData(pXL, _T("S7_3_FNCL"), ResD.AnchorD[0].FNVD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNDL"), ResD.AnchorD[0].FNVD.dRatLim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNRL"), dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_3_FNRL_CHK"), Chk_OK_Sign(0, 1.0, dRat), Row_Ref, Col_Ref, stRow);
		}
		if ( ResD.AnchorD[1].bCheckN )
		{
			if ( ResD.AnchorD[1].FNRD.bCheck )
			{
				PrintCellData(pXL, _T("S7_3_FNRCT"), ResD.AnchorD[1].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
				PrintCellData(pXL, _T("S7_3_FNRDT"), ResD.AnchorD[1].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
				if(IsPrintMark(ResD.AnchorD[1].FNRD.dFNRC, ResD.AnchorD[1].FNRD.dFNRD))
				{
					PrintCellData(pXL, _T("S7_3_FNRRT"), ResD.AnchorD[1].FNRD.dRat, Row_Ref, Col_Ref, stRow);
					PrintCellDataMark(pXL, _T("S7_3_FNRRT_CHK"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNRD.dRat), Row_Ref, Col_Ref, stRow);
				}
			}
			double dRat = ResD.AnchorD[1].FNVD.dRat / ResD.AnchorD[1].FNVD.dRatLim;
			PrintCellData(pXL, _T("S7_3_FNCT"), ResD.AnchorD[1].FNVD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNDT"), ResD.AnchorD[1].FNVD.dRatLim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_3_FNRT"), dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_3_FNRT_CHK"), Chk_OK_Sign(0, 1.0, dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	if(bAddPryoutCmt)
	{
		RptBlock = _T("S7_PRYOUT_CHK_COMMENT");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;
	}

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingSuppLen(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 1) 공급역량 NC : 교각의 받침지지길이
	CString RptBlock = _T("[8_x]Pier_SupLength_D1");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S8_NC"), m_dFactorLengthMM*ResD.SuppLengD.dNC, Row_Ref, Col_Ref, stRow);
	// 2) 소요역량 ND : 교각의 응답변위
	PrintCellData(pXL, _T("S8_Disp"), m_dFactorLengthMM*ResD.SuppLengD.dDe, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_Ptop"), ResD.SuppLengD.dPtop, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_Me"), ResD.SuppLengD.dMe, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_Rs"), ResD.SuppLengD.dRs, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	if ( ResD.SuppLengD.dRs > 1.0 )
	{
		RptBlock = _T("[8_x]Pier_SupLength_D2");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S8_EQ_Ptop"), ResD.SuppLengD.dPtop, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_EQ_Disp1"), ResD.SuppLengD.dDe, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_EQ_Me"), ResD.SuppLengD.dMe, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_EQ_Disp2"), m_dFactorLengthMM*ResD.SuppLengD.dDe, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_EQ_DispPD"), m_dFactorLengthMM*ResD.SuppLengD.dDpd, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S8_LamDR"), ResD.SuppLengD.dLd_DR, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_DispPD"), m_dFactorLengthMM*ResD.SuppLengD.dDpd, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_DispDem"), m_dFactorLengthMM*ResD.SuppLengD.dDdem, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}

	// 나)  소요역량 NDS8_Summary_Table_Body_Rat
	RptBlock = _T("[8_x]Pier_SupLength_D3");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S8_RDisp"), m_dFactorLengthMM*ResD.SuppLengD.dDdem, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_Nmin"), m_dFactorLengthMM*ResD.SuppLengD.Nmin, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_ND"), m_dFactorLengthMM*ResD.SuppLengD.dND, Row_Ref, Col_Ref, stRow);

	PrintCellData(pXL, _T("S8_Nmin_Calc"), m_dFactorLengthMM*ResD.SuppLengD.Nmin, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_L"), ResD.SuppLengD.dL, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_H"), ResD.SuppLengD.dH, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_Theta"), ResD.SuppLengD.dTheta, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	//3) 받침지지길이 성능평가
	RptBlock = _T("[8_x]Pier_SupLength_D4");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S8_3_NC"), m_dFactorLengthMM*ResD.SuppLengD.dNC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S8_3_ND"), m_dFactorLengthMM*ResD.SuppLengD.dND, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.SuppLengD.dNC, ResD.SuppLengD.dND))
	{
		PrintCellData(pXL, _T("S8_3_Rat"), ResD.SuppLengD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S8_3_Result"), Chk_OK_Sign(0, 1.0, ResD.SuppLengD.dRat), Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	if ( pProDlg->Update_ProgressDlg(nProCnt, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingSuppLenSummary(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const CArray<_DGN_SEISEVAL_SuppLengD, _DGN_SEISEVAL_SuppLengD&>& arResD, int& iIdx, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 8.x 받침지지길이 평가 요약
	CString RptBlock = _T("[8_x]Title_Summary_SupLength");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	CString strName;
	strName.Format(_T("8.%d 받침지지길이 평가 요약"), ++iIdx);
	PrintCellData(pXL, _T("[8_x]Title_Summary_Name"), strName, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	INT_PTR nRes = arResD.GetSize() - 1;
	RptBlock = _T("[8_x]Summary_Table_Head");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;
	for ( INT_PTR i = 0; i < nRes; ++i )
	{
		RptBlock = _T("[8_x]Summary_Table_Body");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S8_Summary_Table_Body_Name"), arResD[i].strName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_Summary_Table_Body_NC"), arResD[i].dNC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_Summary_Table_Body_ND"), arResD[i].dND, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(arResD[i].dNC, arResD[i].dND))
		{
			PrintCellData(pXL, _T("S8_Summary_Table_Body_Rat"), arResD[i].dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S8_Summary_Table_Body_Result"), Chk_OK_Sign(0, 1.0, arResD[i].dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	RptBlock = _T("[8_x]Summary_Table_End");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	if ( nRes >= 0 )
	{
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S8_Summary_Table_End_Name"), arResD[nRes].strName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_Summary_Table_End_NC"), arResD[nRes].dNC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S8_Summary_Table_End_ND"), arResD[nRes].dND, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(arResD[nRes].dNC, arResD[nRes].dND))
		{
			PrintCellData(pXL, _T("S8_Summary_Table_End_Rat"), arResD[nRes].dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S8_Summary_Table_End_Result"), Chk_OK_Sign(0, 1.0, arResD[nRes].dRat), Row_Ref, Col_Ref, stRow);
		}
	}
	stRow += rowCnt;
	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

void CBrdgSeisEvalKISTEC2015_Report::Get_AnchorTypeName(const EN_ANCH_TYPE& enType, CString& strType1, CString& strType2)
{
	strType1.Empty();
	strType2.Empty();
	switch ( enType )
	{
	case ANCH_CAST_HEADED_STUD:
		strType1 = _T("선설치");   strType2 = _T("헤드스터드 앵커");
		break;
	case ANCH_CAST_HEADED_BOLT:
		strType1 = _T("선설치");   strType2 = _T("헤드볼트 앵커");
		break;
	case ANCH_CAST_HOOKEDBOLTL:
		strType1 = _T("선설치");   strType2 = _T("90도 갈고리볼트 앵커");
		break;
	case ANCH_CAST_HOOKEDBOLTJ:
		strType1 = _T("선설치");   strType2 = _T("180도 갈고리볼트 앵커");
		break;
	case ANCH_POST_EXPANSIONTC:
		strType1 = _T("후설치");   strType2 = _T("비틀림제어 확장앵커");
		break;
	case ANCH_POST_UNDERCUTANC:
		strType1 = _T("후설치");   strType2 = _T("언더컷 앵커");
		break;
	case ANCH_POST_EXPANSIONDC:
		strType1 = _T("후설치");   strType2 = _T("변위제어 확장앵커");
		break;
	case ANCH_POST_ADHESIVEANC:
		strType1 = _T("후설치");   strType2 = _T("부탁식 앵커");
		break;
	default:
		ASSERT(0);
		break;
	}
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorTenStl(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 5) 인장을 받는 앵커의 강재파괴
	CString RptBlock = _T("[7_2_5]Concrete_Tension_Steel");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);

	if ( ResD.AnchorD[0].bCheckN )
	{
		if(ResD.AnchorTens.nLinK > 0)
		{
			PrintCellData(pXL, _T("S7_2_5_1_Link"), ResD.AnchorTens.nLinK, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_5_1_NodeI"), ResD.AnchorTens.nNodeI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_5_1_NodeJ"), ResD.AnchorTens.nNodeJ, Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pXL, _T("S7_2_5_1_Link"), _T("지점"), Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pXL, _T("S7_2_5_1_BP"), ResD.AnchorTens.strName, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_2_5_1_nA"), ResD.AnchorD[0].FNSD.nn, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_5_1_Ase"), m_dFactorAreamm2*ResD.AnchorD[0].FNSD.dAse, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_5_1_futa"), m_dFactorStress *ResD.AnchorD[0].FNSD.dfuta, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_5_1_FNSC"), ResD.AnchorD[0].FNSD.dFNSC, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_2_5_2_FNSD"), ResD.AnchorD[0].FNSD.dFNSD, Row_Ref, Col_Ref, stRow);

		PrintCellData(pXL, _T("S7_2_5_3_FNSC"), ResD.AnchorD[0].FNSD.dFNSC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_5_3_FNSD"), ResD.AnchorD[0].FNSD.dFNSD, Row_Ref, Col_Ref, stRow);
		if(IsPrintMark(ResD.AnchorD[0].FNSD.dFNSC, ResD.AnchorD[0].FNSD.dFNSD))
		{
			PrintCellData(pXL, _T("S7_2_5_3_Rat"), ResD.AnchorD[0].FNSD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pXL, _T("S7_2_5_3_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNSD.dRat), Row_Ref, Col_Ref, stRow);
		}
	}

	stRow += rowCnt;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorTenConc(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	T_BSED_D BsedD;
	if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

	// 5) 인장을 받는 앵커의 강재파괴
	CString RptBlock = _T("[7_2_6]Titile_Tension_Concrete");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	for ( int i = 0; i < 1/*2*/; ++i )
	{
		RptBlock = _T("[7_2_6_1]Titile_TConc_Param");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_6_1_ca1"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.dca1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_6_1_ca1p"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.dca1p, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_6_1_ca2"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.dca2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_6_1_ca2p"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.dca2p, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_6_1_s1"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.ds1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_6_1_s2"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.ds2, Row_Ref, Col_Ref, stRow);

		stRow += rowCnt + 1;

		RptBlock = _T("[7_2_6_1]Titile_TConc_Project");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		CString strCell;
		switch ( ResD.AnchorD[i].FNCD.nProjectType )
		{
		case 1:
			RptBlock = i == 0 ? _T("S7_2_6_1_Prj_Type1") : _T("S7_2_6_1_Prj_Type1_T");
			strCell = i == 0 ? _T("S7_2_6_1_Prj_Type1_Anc") : _T("S7_2_6_1_Prj_Type1_Anc_T");
			break;
		case 2:
			RptBlock = i == 0 ? _T("S7_2_6_1_Prj_Type2") : _T("S7_2_6_1_Prj_Type2_T");
			strCell = i == 0 ? _T("S7_2_6_1_Prj_Type2_Anc") : _T("S7_2_6_1_Prj_Type2_Anc_T");
			break;
		case 3:
			RptBlock = i == 0 ? _T("S7_2_6_1_Prj_Type3") : _T("S7_2_6_1_Prj_Type3_T");
			strCell = i == 0 ? _T("S7_2_6_1_Prj_Type3_Anc") : _T("S7_2_6_1_Prj_Type3_Anc_T");
			break;
		case 4:
			RptBlock = _T("S7_2_6_1_Prj_Type4");
			strCell = _T("S7_2_6_1_Prj_Type4_Anc");
			break;
		default:
			ASSERT(0);
			break;
		}
		if ( !RptBlock.IsEmpty() )
		{
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, strCell, m_dFactorAreamm2*ResD.AnchorD[i].FNCD.dANcorg, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}

		RptBlock = _T("S7_2_6_1_ANco");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_6_1_ANco_hef"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.dhef, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_6_1_ANco_ANco"), m_dFactorAreamm2 *ResD.AnchorD[i].FNCD.dANco, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		double dAncLim = ResD.AnchorD[i].FNCD.nn*ResD.AnchorD[i].FNCD.dANco;
		if ( ResD.AnchorD[i].FNCD.dANcorg < dAncLim )
		{
			RptBlock = _T("S7_2_6_1_ANc_LT");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_ANc_LT_n"), ResD.AnchorD[i].FNCD.nn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_ANc_LT_ANco"), m_dFactorAreamm2 *ResD.AnchorD[i].FNCD.dANco, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_ANc_LT_ANc"), m_dFactorAreamm2 *dAncLim, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("S7_2_6_1_n");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_nn"), ResD.AnchorD[i].FNCD.nn, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("S7_2_6_1_ANc1");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_ANc1_ANc"), m_dFactorAreamm2 *ResD.AnchorD[i].FNCD.dANc, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("S7_2_6_1_ANc_ME");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_ANc_ME_n"), ResD.AnchorD[i].FNCD.nn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_ANc_ME_ANco"), m_dFactorAreamm2 *ResD.AnchorD[i].FNCD.dANco, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_ANc_ME_ANc"), m_dFactorAreamm2 *dAncLim, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("S7_2_6_1_n");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_nn"), ResD.AnchorD[i].FNCD.nn, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;

			RptBlock = _T("S7_2_6_1_ANc2");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_ANc2_ANc"), m_dFactorAreamm2 *ResD.AnchorD[i].FNCD.dANc, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
		stRow++;

		if(BsedD.nDesignCode == KALIS_2023)
		{
			RptBlock = _T("[7_2_6_1]Titile_TConc_Vcbg_2023");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Anc_2023"), m_dFactorAreamm2 * ResD.AnchorD[i].FNCD.dANc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Anco_2023"), m_dFactorAreamm2 * ResD.AnchorD[i].FNCD.dANco, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_psi_edN_2023"), ResD.AnchorD[i].FNCD.dpsi_edN, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_psi_cN_2023"), ResD.AnchorD[i].FNCD.dpsi_cN, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Nb_2023"), ResD.AnchorD[i].FNCD.dNb, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Vcbg_2023"), ResD.AnchorD[i].FNCD.dNcbg, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("[7_2_6_1]Titile_TConc_Vcbg");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Anc"), m_dFactorAreamm2 * ResD.AnchorD[i].FNCD.dANc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Anco"), m_dFactorAreamm2 * ResD.AnchorD[i].FNCD.dANco, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_psi_edN"), ResD.AnchorD[i].FNCD.dpsi_edN, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Nb"), ResD.AnchorD[i].FNCD.dNb, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_Vcbg"), ResD.AnchorD[i].FNCD.dNcbg, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}

		if ( ResD.AnchorD[i].FNCD.dpsi_edN < 1.0 )
		{
			double dca1min = min(ResD.AnchorD[i].FNCD.dca1, ResD.AnchorD[i].FNCD.dca1p);
			double dca2min = min(ResD.AnchorD[i].FNCD.dca2, ResD.AnchorD[i].FNCD.dca2p);
			double dcamin = min(dca1min, dca2min);
			RptBlock = _T("S7_2_6_1_Vcbg_psiLE");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_psiLE_camin"), m_dFactorLengthMM*dcamin, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_psiLE_15hef"), m_dFactorLengthMM*ResD.AnchorD[i].FNCD.dhef*1.5, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_6_1_Vcbg_psiLE_psi"), ResD.AnchorD[i].FNCD.dpsi_edN, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("S7_2_6_1_Vcbg_psiMT");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}

		if(BsedD.nDesignCode == KALIS_2023)
		{
			switch(ResD.AnchorTens.enAnchType)
			{
				case ANCH_CAST_HEADED_STUD: // Cast-In-Place Anchor Bolt  "Headed Stud"
				case ANCH_CAST_HEADED_BOLT: // Cast-In-Place Anchor Bolt  "Headed Bolt"
				case ANCH_CAST_HOOKEDBOLTL: // Cast-In-Place Anchor Bolt  "Hooked Bolt-L"
				case ANCH_CAST_HOOKEDBOLTJ: // Cast-In-Place Anchor Bolt  "Hooked Bolt-J"
				RptBlock = _T("S7_2_4_1_5_Cal_psi_cN1_2023");	// 선설치
				break;
				case ANCH_POST_EXPANSIONTC: // Post-Installed Anchor Bolt "Torque-controlled anchors"
				case ANCH_POST_UNDERCUTANC: // Post-Installed Anchor Bolt "Undercut anchors"
				case ANCH_POST_EXPANSIONDC: // Post-Installed Anchor Bolt "Displacement-controlled anchors"
				case ANCH_POST_ADHESIVEANC: // Post-Installed Anchor Bolt "Adhesive anchors"
				RptBlock = _T("S7_2_4_1_5_Cal_psi_cN2_2023");	// 후설치
				break;
				default:
				ASSERT(0);
				RptBlock = _T("S7_2_4_1_5_Cal_psi_cN1_2023");
				break;

			}
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}

		RptBlock = _T("S7_2_6_1_Vcbg_NbCal");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_6_1_Vcbg_NbCal_Nb"), ResD.AnchorD[i].FNCD.dNb, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		RptBlock = _T("S7_2_6_1_Vcbg_NbDesc");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);

		stRow += rowCnt + 1;
	}

	RptBlock = _T("[7_2_6_2]FNCC");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_6_2_L_FNCC"), ResD.AnchorD[0].FNCD.dFNCC, Row_Ref, Col_Ref, stRow);
	//PrintCellData(pXL, _T("S7_2_6_2_T_FNCC"), ResD.AnchorD[1].FNCD.dFNCC, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt + 1;

	RptBlock = _T("[7_2_6_3]FNCD");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_6_3_L_FNCD"), ResD.AnchorD[0].FNCD.dFNCDCal, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_6_3_L_NN"), ResD.AnchorD[0].FNCD.nn, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_6_3_L_NA"), ResD.AnchorD[0].FNCD.nnA, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_6_3_L_FNCD_CAL"), ResD.AnchorD[0].FNCD.dFNCD, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt + 1;

	RptBlock = _T("[7_2_6_4]FNCR");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_6_4_L_FNCC"), ResD.AnchorD[0].FNCD.dFNCC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_6_4_L_FNCD"), ResD.AnchorD[0].FNCD.dFNCD, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.AnchorD[0].FNCD.dFNCC, ResD.AnchorD[0].FNCD.dFNCD))
	{
		PrintCellData(pXL, _T("S7_2_6_4_L_FNCR"), ResD.AnchorD[0].FNCD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_6_4_L_FNCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNCD.dRat), Row_Ref, Col_Ref, stRow);
	}
	stRow += rowCnt + 1;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorTenPull(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 5) 인장을 받는 앵커의 강재파괴
	CString RptBlock = _T("[7_2_7]Titile_Anchor_Pull");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_7_1]Anchor_Pull_Str");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorTens.enAnchType == ANCH_POST_EXPANSIONTC ||
		ResD.AnchorTens.enAnchType == ANCH_POST_UNDERCUTANC ||
		ResD.AnchorTens.enAnchType == ANCH_POST_EXPANSIONDC ||
		ResD.AnchorTens.enAnchType == ANCH_POST_ADHESIVEANC )
	{
		RptBlock = _T("S7_2_7_1_Np1");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_7_1_Np1_Np"), ResD.AnchorD[0].FNPD.dNp, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	if ( ResD.AnchorTens.enAnchType == ANCH_CAST_HEADED_STUD ||
		ResD.AnchorTens.enAnchType == ANCH_CAST_HEADED_BOLT )
	{
		RptBlock = _T("S7_2_7_1_Np2");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_7_1_Np2_Abrg"), ResD.AnchorTens.dAbrg, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_7_1_Np2_fck"), m_dFactorStress *ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_7_1_Np2_Np"), ResD.AnchorD[0].FNPD.dNp, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("S7_2_7_1_Np3");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_7_1_Np3_fck"), m_dFactorStress  *ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_7_1_Np3_eh"), m_dFactorLengthMM*ResD.AnchorTens.deh, Row_Ref, Col_Ref, stRow);
		if ( ResD.AnchorTens.nStlBearing == BEARING_STEEL_POTo )
		{
			PrintCellData(pXL, _T("S7_2_7_1_Np3_da"), m_dFactorLengthMM*ResD.AnchorTens.ds, Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pXL, _T("S7_2_7_1_Np3_da"), m_dFactorLengthMM*ResD.AnchorTens.da, Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pXL, _T("S7_2_7_1_Np3_Np"), ResD.AnchorD[0].FNPD.dNp, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if ( ResD.AnchorTens.nStlBearing == BEARING_STEEL_POTo )
		{
			RptBlock = _T("S7_2_7_1_Np3_Socket");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_7_1_Np3_Socket_ds"), m_dFactorLengthMM*ResD.AnchorTens.ds, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
	}


	RptBlock = _T("S7_2_7_1_Npn");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_7_1_Npn_Np"), ResD.AnchorD[0].FNPD.dNp, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_7_1_Npn_Npn"), ResD.AnchorD[0].FNPD.dNpn, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;
	stRow++;

	RptBlock = _T("[7_2_7_2]FNPC");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_7_2_nA"), ResD.AnchorD[0].FNPD.nn, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_7_2_Npn"), ResD.AnchorD[0].FNPD.dNpn, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_7_2_FNPC"), ResD.AnchorD[0].FNPD.dFNPC, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt + 1;

	RptBlock = _T("[7_2_7_3]FNPD");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_7_3_FNPD"), ResD.AnchorD[0].FNPD.dFNPD, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt + 1;

	RptBlock = _T("[7_2_7_4]FNPR");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_7_4_FNPC"), ResD.AnchorD[0].FNPD.dFNPC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_7_4_FNPD"), ResD.AnchorD[0].FNPD.dFNPD, Row_Ref, Col_Ref, stRow);
	if(IsPrintMark(ResD.AnchorD[0].FNPD.dFNPC, ResD.AnchorD[0].FNPD.dFNPD))
	{
		PrintCellData(pXL, _T("S7_2_7_4_FNPR"), ResD.AnchorD[0].FNPD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_7_4_FNPR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNPD.dRat), Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt + 1;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorTenSide(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 8) 인장을 받는 앵커의 콘크리트측면 파열
	CString RptBlock = _T("[7_2_8]Titile_Tension_Concrete_Side");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	// 후설치 앵커이므로 검토할 필요 없음.
	if ( ResD.AnchorD[0].FNRD.nType == 1 )
	{
		RptBlock = _T("S7_2_8_Type1");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }
		return TRUE;
	}

	RptBlock = _T("S7_2_8_1_str");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	BOOL bChk[2] = { FALSE, FALSE };
	for ( int i = 0; i < 2; ++i )
	{
		RptBlock = i == 0 ? _T("S7_2_8_1_L") : _T("S7_2_8_1_T");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		if ( ResD.AnchorD[i].FNRD.nType == 2 )
		{
			RptBlock = _T("S7_2_8_Type2");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			stRow += rowCnt + 1;
			continue;
		}
		bChk[i] = TRUE;

		RptBlock = _T("S7_2_8_Type3");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		stRow += rowCnt;

		RptBlock = _T("S7_2_8_1_str_Cal_");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_8_1_str_ca1"), m_dFactorLengthMM*ResD.AnchorD[0].FNRD.dca1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_8_1_str_Abrg"), ResD.AnchorTens.dAbrg, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_8_1_str_fck"), m_dFactorStress  *ResD.ParamD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pXL, _T("S7_2_8_1_str_Nsb"), ResD.AnchorD[0].FNRD.dNsb, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if ( ResD.AnchorD[i].FNRD.nNsbgType == 1 )
		{
			RptBlock = _T("S7_2_8_1_LT");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_8_1_LT_s1"), m_dFactorLengthMM*ResD.AnchorD[i].FNRD.ds1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_8_1_LT_ca1"), m_dFactorLengthMM*ResD.AnchorD[i].FNRD.dca1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_8_1_LT_Nsb"), ResD.AnchorD[i].FNRD.dNsb, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_8_1_LT_Nsbg"), ResD.AnchorD[i].FNRD.dNsbg, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
		else
		{
			RptBlock = _T("S7_2_8_1_ME");
			rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
			GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
			PrintCellData(pXL, _T("S7_2_8_1_ME_Nsbg"), ResD.AnchorD[i].FNRD.dNsbg, Row_Ref, Col_Ref, stRow);
			stRow += rowCnt;
		}
	}

	if ( bChk[0] || bChk[1] )
	{
		RptBlock = _T("[7_2_8_2]FNRC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		if ( bChk[0] ) PrintCellData(pXL, _T("S7_2_8_2_L_FNRC"), ResD.AnchorD[0].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
		if ( bChk[1] ) PrintCellData(pXL, _T("S7_2_8_2_T_FNRC"), ResD.AnchorD[1].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt + 1;

		RptBlock = _T("[7_2_8_3]FNRD");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		if ( bChk[0] ) PrintCellData(pXL, _T("S7_2_8_3_L_FNRD"), ResD.AnchorD[0].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
		if ( bChk[1] ) PrintCellData(pXL, _T("S7_2_8_3_T_FNRD"), ResD.AnchorD[1].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt + 1;

		RptBlock = _T("[7_2_8_4]FNRR");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		if ( bChk[0] )
		{
			PrintCellData(pXL, _T("S7_2_8_4_L_FNRC"), ResD.AnchorD[0].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_8_4_L_FNRD"), ResD.AnchorD[0].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[0].FNRD.dFNRC, ResD.AnchorD[0].FNRD.dFNRD))
			{
				PrintCellData(pXL, _T("S7_2_8_4_L_FNRR"), ResD.AnchorD[0].FNRD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pXL, _T("S7_2_8_4_L_FNRR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNRD.dRat), Row_Ref, Col_Ref, stRow);
			}
		}
		if ( bChk[1] )
		{
			PrintCellData(pXL, _T("S7_2_8_4_T_FNRC"), ResD.AnchorD[1].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pXL, _T("S7_2_8_4_T_FNRD"), ResD.AnchorD[1].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
			if(IsPrintMark(ResD.AnchorD[1].FNRD.dFNRC, ResD.AnchorD[1].FNRD.dFNRD))
			{
				PrintCellData(pXL, _T("S7_2_8_4_T_FNRR"), ResD.AnchorD[1].FNRD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pXL, _T("S7_2_8_4_T_FNRR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNRD.dRat), Row_Ref, Col_Ref, stRow);
			}
		}
		stRow += rowCnt;
		stRow++;
	}

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BearingAnchorTenRela(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	const T_BRDGEVAL_BEARING& ResD, int& stRow)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 9) 인장과 전단의 상관 작용
	CString RptBlock = _T("[7_2_9]Titile_Anchor_TSR");
	int rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[7_2_9_1]Titile_Anchor_TSR_L");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_9_1_FARL"), ResD.AnchorD[0].FNVD.dRatS, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_9_1_FNRL"), ResD.AnchorD[0].FNVD.dRatN, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[0].FNVD.nType == 1 )
	{
		RptBlock = _T("S7_2_9_1_FNR_TC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_9_1_FNR_TCR"), ResD.AnchorD[0].FNVD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_9_1_FNR_TCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNVD.dRat), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else if ( ResD.AnchorD[0].FNVD.nType == 2 )
	{
		RptBlock = _T("S7_2_9_1_FNR_SC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_9_1_FNR_SCR"), ResD.AnchorD[0].FNVD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_9_1_FNR_SCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNVD.dRat), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else if ( ResD.AnchorD[0].FNVD.nType == 3 )
	{
		RptBlock = _T("S7_2_9_1_FNR_TSC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_9_1_FNR_TSCR"), ResD.AnchorD[0].FNVD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_9_1_FNR_TSCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNVD.dRat), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else ASSERT(0);

	stRow++;

	RptBlock = _T("[7_2_9_2]Titile_Anchor_TSR_T");
	rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
	GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pXL, _T("S7_2_9_2_FART"), ResD.AnchorD[1].FNVD.dRatS, Row_Ref, Col_Ref, stRow);
	PrintCellData(pXL, _T("S7_2_9_2_FNRT"), ResD.AnchorD[1].FNVD.dRatN, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	if ( ResD.AnchorD[1].FNVD.nType == 1 )
	{
		RptBlock = _T("S7_2_9_2_FNR_TC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_9_2_FNR_TCR"), ResD.AnchorD[1].FNVD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_9_2_FNR_TCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNVD.dRat), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else if ( ResD.AnchorD[1].FNVD.nType == 2 )
	{
		RptBlock = _T("S7_2_9_2_FNR_SC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_9_2_FNR_SCR"), ResD.AnchorD[1].FNVD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_9_2_FNR_SCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNVD.dRat), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else if ( ResD.AnchorD[1].FNVD.nType == 3 )
	{
		RptBlock = _T("S7_2_9_2_FNR_TSC");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("S7_2_9_2_FNR_TSCR"), ResD.AnchorD[1].FNVD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pXL, _T("S7_2_9_2_FNR_TSCR_Result"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNVD.dRat), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else ASSERT(0);

	stRow++;

	if ( pProDlg->Update_ProgressDlg(nProCnt++, nProTot) ) { pXL->Quit(); delete pXL; return FALSE; }

	return TRUE;
}