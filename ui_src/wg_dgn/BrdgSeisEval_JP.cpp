#include "stdafx.h"
#include "BrdgSeisEval_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "..\wg_dgn\DgnProgressDlg.h"

#include "SeismicDesignSimd_H14.h"
#include "SeismicDesignSimd_H24.h"
#include "SeismicDesignSimd_H29.h"
#include "SeismicDesignSird_H14.h"
#include "SeismicDesignSird_H24.h"
#include "SeismicDesignSisr_H14.h"
#include "SeismicDesignSisr_H24.h"
#include "SeismicDesignSisr_H29.h"
#include "SeismicDesignSidp_H14.h"
#include "SeismicDesignSibd_H14.h"
#include "SeismicDesignSibd_H29.h"
#include "SeismicDesignSiet.h"
#include "SeismicDesignSumm.h"


#pragma region // CBrdgSeisEval_JP
CBrdgSeisEval_JP::CBrdgSeisEval_JP()
{
	m_pSeismicDesignSimd = NULL;//최대응답변위 조사
	m_pSeismicDesignSird = NULL;//잔류변위 조사
	m_pSeismicDesignSibd = NULL;//휨 조사
	m_pSeismicDesignSidp = NULL;//변위 소성율 조사
	m_pSeismicDesignSisr = NULL;//전단 조사
	m_pSeismicDesignSiet = NULL;//그 외 조사
	m_pSeismicDesignSumm = NULL;//요약결과

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return;

	T_SIDC_D dSidc;
	dSidc.Initialize();
	pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc);

	switch (dSidc.nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		m_pSeismicDesignSimd = new CSeismicDesignSimd_H14();
		m_pSeismicDesignSird = new CSeismicDesignSird_H14();
		m_pSeismicDesignSibd = new CSeismicDesignSibd_H14();
		m_pSeismicDesignSidp = new CSeismicDesignSidp_H14();
		m_pSeismicDesignSisr = new CSeismicDesignSisr_H14();
		m_pSeismicDesignSiet = new CSeismicDesignSiet();
		m_pSeismicDesignSumm = new CSeismicDesignSumm();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		m_pSeismicDesignSimd = new CSeismicDesignSimd_H24();
		m_pSeismicDesignSird = new CSeismicDesignSird_H24();
		m_pSeismicDesignSibd = new CSeismicDesignSibd_H14();
		m_pSeismicDesignSidp = new CSeismicDesignSidp_H14();
		m_pSeismicDesignSisr = new CSeismicDesignSisr_H24();
		m_pSeismicDesignSiet = new CSeismicDesignSiet();
		m_pSeismicDesignSumm = new CSeismicDesignSumm();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
	case BrdgSE_CodeJP::JROAD5_R07:
		m_pSeismicDesignSimd = new CSeismicDesignSimd_H29();
		m_pSeismicDesignSird = new CSeismicDesignSird_H24();
		m_pSeismicDesignSibd = new CSeismicDesignSibd_H29();
		m_pSeismicDesignSidp = NULL;
		m_pSeismicDesignSisr = new CSeismicDesignSisr_H29();
		m_pSeismicDesignSiet = new CSeismicDesignSiet();
		m_pSeismicDesignSumm = new CSeismicDesignSumm();
		if (pDoc->m_pAttrCtrl2->GetQSifaH29()->GetCount() < 1) {
			T_SIFA_H29_K key = 1;
			T_SIFA_H29_D tSifaH29; tSifaH29.Initialize();
			BOOL bRes = pDoc->m_pDataCtrl->AddSifaH29(key, tSifaH29);
			ASSERT(bRes);
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

CBrdgSeisEval_JP::~CBrdgSeisEval_JP()
{
	if (m_pSeismicDesignSimd != NULL) {	delete m_pSeismicDesignSimd; m_pSeismicDesignSimd = NULL; }
	if (m_pSeismicDesignSird != NULL) { delete m_pSeismicDesignSird; m_pSeismicDesignSird = NULL; }
	if (m_pSeismicDesignSibd != NULL) { delete m_pSeismicDesignSibd; m_pSeismicDesignSibd = NULL; }
	if (m_pSeismicDesignSidp != NULL) { delete m_pSeismicDesignSidp; m_pSeismicDesignSidp = NULL; }
	if (m_pSeismicDesignSisr != NULL) { delete m_pSeismicDesignSisr; m_pSeismicDesignSisr = NULL; }
	if (m_pSeismicDesignSiet != NULL) { delete m_pSeismicDesignSiet; m_pSeismicDesignSiet = NULL; }
	if (m_pSeismicDesignSumm != NULL) { delete m_pSeismicDesignSumm; m_pSeismicDesignSumm = NULL; }
}

BOOL CBrdgSeisEval_JP::DoSeisEval(CDgnProgressDlg* pProgress)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return FALSE;
	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (pResult == NULL) return FALSE;

	try
	{
		AddProgressStep(pProgress);
		if (m_pSeismicDesignSimd != NULL) m_pSeismicDesignSimd->Calculate(pProgress);
 		if (m_pSeismicDesignSird != NULL) m_pSeismicDesignSird->Calculate(pProgress);
 		if (m_pSeismicDesignSibd != NULL) m_pSeismicDesignSibd->Calculate(pProgress);
 		if (m_pSeismicDesignSidp != NULL) m_pSeismicDesignSidp->Calculate(pProgress);
		if (m_pSeismicDesignSisr != NULL) m_pSeismicDesignSisr->Calculate(pProgress);
		if (m_pSeismicDesignSiet != NULL) m_pSeismicDesignSiet->Calculate(pProgress);
		if (m_pSeismicDesignSumm != NULL) m_pSeismicDesignSumm->Calculate(pProgress);
	}
	catch (BOOL bStop)
	{
		ASSERT(bStop == FALSE);
		pResult->DeleteAllResult();
		return FALSE;
	}

	return TRUE;
}

void CBrdgSeisEval_JP::AddProgressStep(CDgnProgressDlg* pProgress)
{
	if (m_pSeismicDesignSimd != NULL) m_pSeismicDesignSimd->AddProgressStep(pProgress);
 	if (m_pSeismicDesignSird != NULL) m_pSeismicDesignSird->AddProgressStep(pProgress);
 	if (m_pSeismicDesignSibd != NULL) m_pSeismicDesignSibd->AddProgressStep(pProgress);
 	if (m_pSeismicDesignSidp != NULL) m_pSeismicDesignSidp->AddProgressStep(pProgress);
	if (m_pSeismicDesignSisr != NULL) m_pSeismicDesignSisr->AddProgressStep(pProgress);
	if (m_pSeismicDesignSiet != NULL) m_pSeismicDesignSiet->AddProgressStep(pProgress);
	if (m_pSeismicDesignSumm != NULL) m_pSeismicDesignSumm->AddProgressStep(pProgress);
}

void CBrdgSeisEval_JP::DoMemo()
{
	BOOL bStop = FALSE;
	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_BRDG_SEIS_EVAL_JP);

	int nCount = 10;
	for (int i = 0; i < nCount; ++i)
	{
		bStop = ProDlg.Update_ProgressDlg(i + 1, nCount);
		Sleep(500);
	}
	ProDlg.Delete_ProgressDlg();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	pResult->SetResultFlag(TRUE);
}
#pragma endregion

#pragma region // CUnitConversion
CUnitConversion::CUnitConversion(const int& nForc, const int& nLeng) : CCurUnitSaver(TRUE)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) return;
	// Code 단위계 변환		
	T_UNIT_INDEX CodeUnit;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CodeUnit);
	CodeUnit.nBase_Force = nForc;
	CodeUnit.nBase_Length = nLeng;
	pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
}

CUnitConversion::~CUnitConversion()
{
}
#pragma endregion