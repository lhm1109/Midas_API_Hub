#include "stdafx.h"
#include "RptRatingSTL_NR2006.h"

#include "Dgn_RatingRptManager.h"
#include "Rating_NR2006_STL_XLOut.h"

#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace dgn::lib;

CRptRatingSTL_NR2006::CRptRatingSTL_NR2006(int nDgnCode)
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

	m_pDoc->SetCivilCodeDgn(nDgnCode/*BD_21_01_PSC_RATING*/);
	m_pDataCtrl = new CCRCDataCtrl();

	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if(m_pForcCtrl==NULL) { ASSERT(0); return; }
}

CRptRatingSTL_NR2006::~CRptRatingSTL_NR2006(void)
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptRatingSTL_NR2006::Execute_StlRatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	if(m_bStopExecute) return FALSE;
	Progress(3, 0);
	int nProgressPercent = 0;

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_SRAR_K, T_SRAR_K> aPrintElemKey;
	m_pDoc->m_pAttrCtrl2->GetSrarKeyList(aPrintElemKey);

	CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetAlcsSTLKeyList(arAlcs);
	int nAlcs = arAlcs.GetSize();
	if(nAlcs==0) return FALSE;

	int nPrintElemSize = aPrintElemKey.GetSize();
	if(nPrintElemSize==0 || nAlcs==0)
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);

		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);

		return FALSE;
	}

	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;

	BOOL bSuccess = TRUE;
	CAnalysisResult* pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();

	try
	{
		// Parameter
		T_RPSS_D RpssD;
		if(!m_pDoc->m_pAttrCtrl2->GetRpss(RpssD)) RpssD.Initialize();

		CArray<_T_NR2006_STL_XLOUT, _T_NR2006_STL_XLOUT&> arXlout;
		arXlout.RemoveAll();
		for(int i=0; i<nPrintElemSize; ++i)
		{
			T_ELEM_K  ElemK = aPrintElemKey[i];

			// 출력 옵션 셋팅
			// 1. 출력 여부 결정
			T_SRAS_D SrasD; SrasD.Initialize();  // 설계 위치
			if(!m_pDoc->m_pAttrCtrl2->GetSras(ElemK, SrasD)) continue;
			T_SRAR_D SrarD; SrarD.Initialize();  // 출력 위치
			if(!m_pDoc->m_pAttrCtrl2->GetSrar(ElemK, SrarD)) continue;

			_T_NR2006_STL_XLOUT xlOutD;
			xlOutD.Initialize();
			xlOutD.ElemK = ElemK;
			xlOutD.bIJ[0] = ((SrasD.nPos == 0 || SrasD.nPos == 2) && (SrarD.iPrintChk == 0 || SrarD.iPrintChk == 2));
			xlOutD.bIJ[1] = ((SrasD.nPos == 1 || SrasD.nPos == 2) && (SrarD.iPrintChk == 1 || SrarD.iPrintChk == 2));

			for(int j=0; j<nAlcs; ++j)
			{
				T_ALCS_K AlcsK = arAlcs[j];
				T_ALCS_D AlcsD;
				if(!m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD)) continue; // Rating case
				BOOL bULS = (RpssD.bRptUltLim  && AlcsD.nLimitState == 0); // ULS

				xlOutD.mBend.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
				xlOutD.mShear.SetAt(AlcsK, (RpssD.bUlsShear && bULS));
				xlOutD.mStiff.SetAt(AlcsK, (RpssD.bUlsIntBearStiff && bULS));
			}
			arXlout.Add(xlOutD);

			//////////////////////////////////////////////////////////////////////////
			if(m_bStopExecute)	return FALSE;
			nProgressPercent += nProgressIncrementPercent;
			Progress(0, nProgressPercent);
			Progress(2, nProgressPercent);
		}

		// 계산서 출력
		bSuccess = arXlout.GetSize() < 1 ? FALSE : TRUE;
		if(bSuccess)
		{
			// 계산서 출력	 	
			CString strDataPath = m_pDoc->GetPathName();
			CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

			CRating_NR2006_STL_XLOut* pmyReport = nullptr;
			if(iDgnCode == STL_NR_GN_CIV_025_06)
			{
				pmyReport = new CRating_NR2006_STL_XLOut();
			}
			else
			{
				throw;
			}

			pmyReport->SetDataCtrlPointer(m_pDataCtrl);
			pmyReport->PrintReport(strBasePath, strDataPath, strPath, iPrintOpt, arXlout);   // strPath : 경로+파일이름

			if(pmyReport)
				delete pmyReport;
		}
	}
	catch(...)
	{
		bSuccess = FALSE;
	}

	return bSuccess;
}