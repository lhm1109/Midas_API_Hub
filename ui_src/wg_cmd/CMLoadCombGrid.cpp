// CMLoadCombGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "CMLoadCombMainGrid.h"
#include "CMLoadCombDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombGrid

CCMLoadCombGrid::CCMLoadCombGrid()
{
	m_pParentDlg = 0;
	m_pDoc = 0;
	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_nLCtype = D_LCOM_CB_GENERAL;
	m_csPostfix = _T("CB");

	m_nFixedCol = 0;
	m_nStldCount = 0;
	m_nSplcCount = 0;
	m_nThisCount = 0;
	m_nMvldCount = 0;
	m_nSmlcCount = 0;
	m_nSgldCount = 0;
	m_nEspcCount = 0;
	m_nGilcCount = 0;
	m_nCranCount = 0;
	m_nNspcCount = 0;
	m_nNespCount = 0;
	m_nLcomCount = 0;

	// MNET:1209, KJH20050204
#if defined(_CIVIL)
	CString csErrMsg(_T(""));
	m_bElastStag = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLCBConcrete,
																														 _LSX(JTG D60-04), csErrMsg);
#else
	m_bElastStag = FALSE;
#endif
	// End KJH20050204
}

CCMLoadCombGrid::~CCMLoadCombGrid()
{
}

//--------------------------------------------------------------------------
// Interface
void CCMLoadCombGrid::Initialize(CDBDoc* pDoc)
{
	m_pDoc = pDoc;
	CTBBrowserWnd::Initialize();
	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(TRUE);   // Enable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key
	MakeItemEx();
}

void CCMLoadCombGrid::SetLcomType(int nLcomType)
{
	m_nLcomType = nLcomType;
	switch(m_nLcomType)
	{
	case D_LCOMTYPE_GENERAL : 
		m_nLCtype = D_LCOM_CB_GENERAL;  m_csPostfix = _T("CB"); break;
	case D_LCOMTYPE_STEEL   : 
		m_nLCtype = D_LCOM_CB_STEEL;    m_csPostfix = _T("CBS"); break;
	case D_LCOMTYPE_CONCRETE: 
		m_nLCtype = D_LCOM_CB_CONCRETE; m_csPostfix = _T("CBC"); break;
	case D_LCOMTYPE_FDN     : 
		m_nLCtype = D_LCOM_CB_FOOTING;  m_csPostfix = _T("CBF"); break;
	case D_LCOMTYPE_SRC     : 
		m_nLCtype = D_LCOM_CB_SRC;      m_csPostfix = _T("CBR"); break;
	case D_LCOMTYPE_ALUMINUM: 
		m_nLCtype = D_LCOM_CB_ALUMINUM; m_csPostfix = _T("CBA"); break;
	case D_LCOMTYPE_STLCOMP: 
		m_nLCtype = D_LCOM_CB_STLCOMP; m_csPostfix = _T("CBSC"); break;
	case D_LCOMTYPE_CFSTEEL   : 
		m_nLCtype = D_LCOM_CB_CFSTEEL;  m_csPostfix = _T("CBCF"); break;
	case D_LCOMTYPE_SEISMIC: 
		m_nLCtype = D_LCOM_CB_SEISMIC;  m_csPostfix = _T("CBSM"); break;
	case D_LCOMTYPE_LINEAR_EVAL: 
		m_nLCtype = D_LCOM_CB_LINEAR_EVAL;  m_csPostfix = _T("CBLN"); break;
	default: ASSERT(0);       
		m_nLCtype = D_LCOM_CB_GENERAL;  m_csPostfix = _T("CB"); break;
	}
}

void CCMLoadCombGrid::ChangeLcomType(int nLcomType)
{
	SetLcomType(nLcomType);
	if (!IsDisplayMode()) return;

	ROWCOL nCol;
	GetColumnNoFor(3, nCol);

	CString TypeList = CCMLoadCombMainGrid::GetLcomTypeList(m_nLcomType);

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(TypeList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	GetColumnNoFor(2, nCol);
	CString csChoiceList = CCMLoadCombMainGrid::GetActiveChoiceList(m_nLcomType);
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	// MNET:1209, KJH20050204
	BOOL bElastHide = !m_bElastStag || m_nLcomType!=D_LCOMTYPE_CONCRETE;
	SetStyleRange(CGXRange().SetCols(5), CGXStyle().SetEnabled(!bElastHide));
	HideCols(5, 5, bElastHide);
	// End KJH20050204
}

void CCMLoadCombGrid::MakeItemEx()
{
	CWaitCursor wait;

	BOOL bOldLock = LockUpdate(TRUE);

	CArray<T_LCOM_K, T_LCOM_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_nLcomType, aKeyList);
	int nRecCount = aKeyList.GetSize();
	SetRowCount(0);
	SetRecordCount(nRecCount);
	
	T_LCOM_D lcom;
	for (int i = 0; i < nRecCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, aKeyList[i], lcom);
		SetValue((ROWCOL)(i+1), aKeyList[i], lcom);
	}

	SetEnableDisableLcomCol();

	SetRightValidCell(1, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMLoadCombGrid::OnStageChanged()
{
	if (!IsDisplayMode()) return;
	BOOL bOldLock = LockUpdate(TRUE);

	// 포커스 가진 윈도우 핸들 저장
	HWND hwnd = ::GetFocus();

	// 수정중이면 일단 취소한다.
	if (m_nEditMode != noMode)
	{
		// Active 상태이면 deactive시켜야 CancelEdit내에서 RemoveRow할 때 
		// Validation Check하지 않는다.
		ROWCOL nRow, nCol;
		GetCurrentCell(nRow, nCol);
		CGXControl* pControl = GetControl(nRow, nCol);
		if (pControl->IsActive()) pControl->SetActive(FALSE);
		CancelEdit();  
	}

	SetColInfo();
	MakeItemEx();

	// 이전 포커스 가진 윈도우 복구
	if (::IsWindow(hwnd)) ::SetFocus(hwnd);
	LockUpdate(bOldLock); 
	if (!bOldLock) Redraw();
}

void CCMLoadCombGrid::UpdateBuffer()
{
	if (!IsDisplayMode()) return;
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bOldLock = LockUpdate(TRUE);

	T_LCOM_K Key, KeyBak;
	T_LCOM_D Data, DataBak;

	BOOL bDoMakeItemEx = FALSE;
	// 하중 조건에 변경이 있는지 먼저 조사해서 처리하고 나서 
	// 하중 조합에 대해서 처리한다.
	UpdateLoadCase(bDoMakeItemEx);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		if (m_nLcomType == D_LCOMTYPE_GENERAL)
		{
			if (nCmd != UR_LCOM_ADD && nCmd != UR_LCOM_DEL &&
					nCmd != UR_LCOM_MFD && nCmd != UR_LCOM_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_STEEL)
		{
			if (nCmd != UR_LCST_ADD && nCmd != UR_LCST_DEL &&
					nCmd != UR_LCST_MFD && nCmd != UR_LCST_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (nCmd != UR_LCCO_ADD && nCmd != UR_LCCO_DEL &&
					nCmd != UR_LCCO_MFD && nCmd != UR_LCCO_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_SRC)
		{
			if (nCmd != UR_LCSC_ADD && nCmd != UR_LCSC_DEL &&
					nCmd != UR_LCSC_MFD && nCmd != UR_LCSC_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_CFSTEEL)
		{
			if (nCmd != UR_LCCF_ADD && nCmd != UR_LCCF_DEL &&
					nCmd != UR_LCCF_MFD && nCmd != UR_LCCF_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_FDN)
		{
			if (nCmd != UR_LCFT_ADD && nCmd != UR_LCFT_DEL &&
					nCmd != UR_LCFT_MFD && nCmd != UR_LCFT_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_ALUMINUM)
		{
			if (nCmd != UR_LCAL_ADD && nCmd != UR_LCAL_DEL &&
					nCmd != UR_LCAL_MFD && nCmd != UR_LCAL_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_STLCOMP)
		{
			if (nCmd != UR_LCSCM_ADD && nCmd != UR_LCSCM_DEL &&
				  nCmd != UR_LCSCM_MFD && nCmd != UR_LCSCM_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_CFSTEEL)
		{
			if (nCmd != UR_LCCF_ADD && nCmd != UR_LCCF_DEL &&
					nCmd != UR_LCCF_MFD && nCmd != UR_LCCF_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_SEISMIC)
		{
			if (nCmd != UR_LCSM_ADD && nCmd != UR_LCSM_DEL &&
				nCmd != UR_LCSM_MFD && nCmd != UR_LCSM_MFS) continue;
		}
		else if (m_nLcomType == D_LCOMTYPE_LINEAR_EVAL)
		{
			if (nCmd != UR_LCLN_ADD && nCmd != UR_LCLN_DEL &&
				nCmd != UR_LCLN_MFD && nCmd != UR_LCLN_MFS) continue;
		}

		switch(nCmd)
		{
		case(UR_LCOM_ADD):
		case(UR_LCST_ADD):
		case(UR_LCCO_ADD):
		case(UR_LCSC_ADD):
		case(UR_LCFT_ADD):
		case(UR_LCAL_ADD):
		case(UR_LCSCM_ADD):
		case(UR_LCCF_ADD):
		case(UR_LCSM_ADD):
		case(UR_LCLN_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				InsertRecordByKey(Key, Data);
				break;
		case(UR_LCOM_DEL):
		case(UR_LCST_DEL):
		case(UR_LCCO_DEL):
		case(UR_LCSC_DEL):
		case(UR_LCFT_DEL):
		case(UR_LCAL_DEL):
		case(UR_LCSCM_DEL):				
		case(UR_LCCF_DEL):
		case(UR_LCSM_DEL):
		case(UR_LCLN_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				DeleteRecordByKey(Key, Data);
				break;
		case(UR_LCOM_MFD):
		case(UR_LCST_MFD):
		case(UR_LCCO_MFD):
		case(UR_LCSC_MFD):
		case(UR_LCFT_MFD):
		case(UR_LCAL_MFD):
		case(UR_LCSCM_MFD):
		case(UR_LCCF_MFD):
		case(UR_LCSM_MFD):
		case(UR_LCLN_MFD):
				// 여기서는 저장만 하고 UR_LCOM_MFS에서 처리한다.
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_LCOM_MFS):
		case(UR_LCST_MFS):
		case(UR_LCCO_MFS):
		case(UR_LCSC_MFS):
		case(UR_LCFT_MFS):
		case(UR_LCAL_MFS):
		case(UR_LCSCM_MFS):
		case(UR_LCCF_MFS):
		case(UR_LCLN_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetLcom(m_nLcomType, nKey, Key, Data);
				DeleteRecordByKey(Key, Data);
				InsertRecordByKey(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				bDoMakeItemEx = TRUE;
				break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_nEditMode = CTBBrowserWnd::noMode;
		if (GetRowCount() < ncRow) ncRow = GetRowCount();
		if (GetColCount() < ncCol) ncCol = GetColCount();
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		m_nEditMode = nOldMode;
	}

	if(bDoMakeItemEx)
	{
		MakeItemEx();
	} 

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

//--------------------------------------------------------------------------
// Implementation
void CCMLoadCombGrid::SetColInfo()
{
	CString item;
	int nCount;

	m_aColInfo.RemoveAll();
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__No), 0, 1)); 
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__Name), 0, 2)); 
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__Active), 0, 3)); 
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__Type), 0, 4)); 
	m_aColInfo.Add(SLcomColItem(_LSX(E), 0, 5)); // MNET:1209, KJH20050204  // PMS:XXXX-JWKWON-20091014 : 문자열 번역 안하는 걸로
	m_nFixedCol = 5;

	//D_LCOM_STATIC		1 : 정적 하중 조건
	T_STLD_K Key;
	T_STLD_D Data;
	CArray<T_STLD_K, T_STLD_K> aStldKey;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKey);
	nCount = aStldKey.GetSize();
	m_nStldCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		Key = aStldKey[i];
		m_pDoc->m_pAttrCtrl->GetStld(Key, Data);
		if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외
		item.Format(_T("%s(ST)"), Data.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_STATIC, Key));
		m_nStldCount++;
	}
	//m_nStldCount = nCount;

	//D_LCOM_SPECTRUM		2 : 응답 스펙트럼 하중 조건
	T_SPLC_K SplcKey;
	T_SPLC_D SplcData;
	CArray<T_SPLC_K, T_SPLC_K> aSplcKey;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcKey);
	nCount = aSplcKey.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		SplcKey = aSplcKey[i];
		m_pDoc->m_pAttrCtrl->GetSplc(SplcKey, SplcData);
		item.Format(_T("%s(RS)"), SplcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_SPECTRUM, SplcKey));
	}
	m_nSplcCount = nCount;

	//D_LCOM_SPECTRUM_ND	: ND용 응답 스펙트럼 하중 조건
	T_NSPC_K NspcKey;
	T_NSPC_D NspcData;
	CArray<T_NSPC_K, T_NSPC_K> aNspcKey;
	m_pDoc->m_pAttrCtrl->GetNspcKeyList(aNspcKey);
	nCount = aNspcKey.GetSize();
	m_nNspcCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		NspcKey = aNspcKey[i];
		m_pDoc->m_pAttrCtrl->GetSplc(NspcKey, NspcData);
		if(!NspcData.bNonDissipative) continue;
		item.Format(_T("%s(NRS)"), NspcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_SPECTRUM_ND, NspcKey));
		m_nNspcCount++;
	}

	//D_LCOM_HISTORY		3 : 시간 이력 하중 조건
	T_THIS_K ThisKey;
	T_THIS_D ThisData;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
	nCount = aThisKey.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		ThisKey = aThisKey[i];
		m_pDoc->m_pAttrCtrl->GetThis(ThisKey, ThisData);
		item.Format(_T("%s(TH)"), ThisData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_HISTORY, ThisKey));
	}
	m_nThisCount = nCount;
	
	//D_LCOM_MOVING		  4 : 이동 하중 조건
	// 코드에 따른 방식으로 수정 
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			T_MVLD_K MvldKey;
			T_MVLD_D MvldData;
			CArray<T_MVLD_K, T_MVLD_K> aMvldKey;
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(aMvldKey);
			nCount = aMvldKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldKey = aMvldKey[i];
				m_pDoc->m_pAttrCtrl->GetMvld(MvldKey, MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldKey));
			}
			m_nMvldCount = nCount;
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			T_MVLDch_K MvldchKey;
			T_MVLDch_D MvldchData;
			CArray<T_MVLDch_K, T_MVLDch_K> aMvldchKey;
			m_pDoc->m_pAttrCtrl->GetMvldchKeyList(aMvldchKey);
			nCount = aMvldchKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldchKey = aMvldchKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldch(MvldchKey, MvldchData);
				item.Format(_T("%s(MV)"), MvldchData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldchKey));
			}
			m_nMvldCount = nCount;
		}
		break;
	case D_MOVE_CODE_JAPAN:
		{
			T_MVLDjp_K MvldjpKey;
			T_MVLDjp_D MvldjpData;
			CArray<T_MVLDjp_K, T_MVLDjp_K> aMvldjpKey;
			m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(aMvldjpKey);
			nCount = aMvldjpKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldjpKey = aMvldjpKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldjp(MvldjpKey, MvldjpData);
				item.Format(_T("%s(MV)"), MvldjpData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldjpKey));
			}
			m_nMvldCount = nCount;   
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			T_MVLDid_K MvldidKey;
			T_MVLDid_D MvldidData;
			CArray<T_MVLDid_K, T_MVLDid_K> aMvldidKey;
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(aMvldidKey);
			nCount = aMvldidKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldidKey = aMvldidKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldid(MvldidKey, MvldidData);
				item.Format(_T("%s(MV)"), MvldidData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldidKey));
			}
			m_nMvldCount = nCount;
		}
		break;
	case D_MOVE_CODE_BS:
		{
			T_MVLDbs_K MvldbsKey;
			T_MVLDbs_D MvldbsData;
			CArray<T_MVLDbs_K, T_MVLDbs_K> aMvldbsKey;
			m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(aMvldbsKey);
			nCount = aMvldbsKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldbsKey = aMvldbsKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldbs(MvldbsKey, MvldbsData);
				item.Format(_T("%s(MV)"), MvldbsData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldbsKey));
			}
			m_nMvldCount = nCount;
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			T_MVLDfr_K MvldfrKey;
			T_MVLDfr_D MvldfrData;
			CArray<T_MVLDfr_K, T_MVLDfr_K> aMvldfrKey;
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(aMvldfrKey);
			nCount = aMvldfrKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldfrKey = aMvldfrKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldfr(MvldfrKey, MvldfrData);
				item.Format(_T("%s(MV)"), MvldfrData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldfrKey));
			}
			m_nMvldCount = nCount;
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			T_MVLDtr_K MvldKey;
			T_MVLDtr_D MvldData;
			CArray<T_MVLDtr_K, T_MVLDtr_K> aMvldKey;
			m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(aMvldKey);
			nCount = aMvldKey.GetSize();
			for (int i = 0; i < nCount; i++)
			{
				MvldKey = aMvldKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldtr(MvldKey, MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldKey));
			}
			m_nMvldCount = nCount;
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	/*
	int nMovingType = CProduct::GetMovingType();
	if (nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_US)
	{
		if(nMovingType == D_PRODUCT_MOVING_US && CProduct::IsIndia())
		{
			T_MVLDid_K MvldidKey;
			T_MVLDid_D MvldidData;
			CArray<T_MVLDid_K, T_MVLDid_K> aMvldidKey;
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(aMvldidKey);
			nCount = aMvldidKey.GetSize();
			for (i = 0; i < nCount; i++)
			{
				MvldidKey = aMvldidKey[i];
				m_pDoc->m_pAttrCtrl->GetMvldid(MvldidKey, MvldidData);
				item.Format(_T("%s(MV)"), MvldidData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldidKey));
			}
			m_nMvldCount = nCount;
		}
		else
		{
			T_MVLD_K MvldKey;
			T_MVLD_D MvldData;
			CArray<T_MVLD_K, T_MVLD_K> aMvldKey;
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(aMvldKey);
			nCount = aMvldKey.GetSize();
			for (i = 0; i < nCount; i++)
			{
				MvldKey = aMvldKey[i];
				m_pDoc->m_pAttrCtrl->GetMvld(MvldKey, MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldKey));
			}
			m_nMvldCount = nCount;
		}
	}
	else if (nMovingType == D_PRODUCT_MOVING_JP)
	{
		T_MVLDjp_K MvldjpKey;
		T_MVLDjp_D MvldjpData;
		CArray<T_MVLDjp_K, T_MVLDjp_K> aMvldjpKey;
		m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(aMvldjpKey);
		nCount = aMvldjpKey.GetSize();
		for (i = 0; i < nCount; i++)
		{
			MvldjpKey = aMvldjpKey[i];
			m_pDoc->m_pAttrCtrl->GetMvldjp(MvldjpKey, MvldjpData);
			item.Format(_T("%s(MV)"), MvldjpData.LoadCaseName);
			m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldjpKey));
		}
		m_nMvldCount = nCount;
	}
	else if (nMovingType == D_PRODUCT_MOVING_CH)
	{
		T_MVLDch_K MvldchKey;
		T_MVLDch_D MvldchData;
		CArray<T_MVLDch_K, T_MVLDch_K> aMvldchKey;
		m_pDoc->m_pAttrCtrl->GetMvldchKeyList(aMvldchKey);
		nCount = aMvldchKey.GetSize();
		for (i = 0; i < nCount; i++)
		{
			MvldchKey = aMvldchKey[i];
			m_pDoc->m_pAttrCtrl->GetMvldch(MvldchKey, MvldchData);
			item.Format(_T("%s(MV)"), MvldchData.LoadCaseName);
			m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldchKey));
		}
		m_nMvldCount = nCount;
	}
	else
	{
		ASSERT(0);
	}
	*/

	//D_LCOM_SETTLEMENT	5 : 지점침하 하중 조건
	T_SMLC_K SmlcKey;
	T_SMLC_D SmlcData;
	CArray<T_SMLC_K, T_SMLC_K> aSmlcKey;
	m_pDoc->m_pAttrCtrl->GetSmlcKeyList(aSmlcKey);
	nCount = aSmlcKey.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		SmlcKey = aSmlcKey[i];
		m_pDoc->m_pAttrCtrl->GetSmlc(SmlcKey, SmlcData);
		item.Format(_T("%s(SM)"), SmlcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_SETTLEMENT, SmlcKey));
	}
	m_nSmlcCount = nCount;

	//D_LCOM_STAGE 13 : 시공해석 하중 조건
	T_SGLD_K SgldKey;
	T_SGLD_D SgldData;
	CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
	nCount = aSgldKey.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		SgldKey = aSgldKey[i];
		m_pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData);
		item.Format(_T("%s(CS)"), SgldData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_STAGE, SgldKey));
	}
	m_nSgldCount = nCount;

	//D_LCOM_ECCENSPEC  : 우발편심 응답 스펙트럼 하중 조건
	T_ESPC_K EspcKey;
	T_ESPC_D EspcData;
	CArray<T_ESPC_K, T_ESPC_K> aEspcKey;
	m_pDoc->m_pAttrCtrl->GetEspcKeyList(aEspcKey);
	nCount = aEspcKey.GetSize();
	m_nEspcCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		EspcKey = aEspcKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetSplc(EspcKey, SplcData)) {ASSERT(0); continue;}
		if(!SplcData.bConsiderEccen) continue;
		
		m_pDoc->m_pAttrCtrl->GetEspc(EspcKey, EspcData);
		item.Format(_T("%s(ES)"), EspcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_ECCENSPEC, EspcKey));
		m_nEspcCount++;
	}

	//D_LCOM_ECCENSPEC_ND  : ND용 우발편심 응답 스펙트럼 하중 조건
	m_pDoc->m_pAttrCtrl->GetNspcKeyList(aNspcKey);
	nCount = aNspcKey.GetSize();
	m_nNespCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		NspcKey = aNspcKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetSplc(NspcKey, NspcData)) {ASSERT(0); continue;}
		if(!NspcData.bConsiderEccen) continue;
		if(!NspcData.bNonDissipative) continue;

		m_pDoc->m_pAttrCtrl->GetNspc(NspcKey, NspcData);
		item.Format(_T("%s(NES)"), NspcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_ECCENSPEC_ND, NspcKey));
		m_nNespCount++;
	}

	//D_LCOM_GRIDMODEL  : 격자모델 하중 조건
	T_GILC_K GilcKey;
	T_GILC_D GilcData;
	CArray<T_GILC_K, T_GILC_K> aGilcKey;
	m_pDoc->m_pAttrCtrl->GetGilcKeyList(aGilcKey);
	nCount = aGilcKey.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		GilcKey = aGilcKey[i];
		m_pDoc->m_pAttrCtrl->GetGilc(GilcKey, GilcData);
		item.Format(_T("%s(GI)"), GilcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_GRIDMODEL, GilcKey));
	}
	m_nGilcCount = nCount;
	
	//D_LCOM_CRANE 19 : Crane 하중
	BOOL bExistCrane = (m_pDoc->m_pAttrCtrl->GetCountCran()>0);
	if(bExistCrane)
	{
		item.Format(_T("%s%s"), _LS(IDS_CMD_CRANE), _T("(CN)"));
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_CRANE, 1));
		m_nCranCount = 1;
		if(m_pDoc->m_pAttrCtrl->IsExistCranBraking())
		{
			item.Format(_T("%s%s"), _LS(IDS_CMD_CRANE_BRK), _T("(CN)"));
			m_aColInfo.Add(SLcomColItem(item, D_LCOM_CRANE, 2));
			m_nCranCount = 2;
		}
	}

	//D_LCOM_CB_??? : 하중조합
	CArray<T_LCOM_K, T_LCOM_K> aLcomKey;
	T_LCOM_K LcomKey;
	T_LCOM_D LcomData;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(m_nLcomType, aLcomKey);
	nCount = aLcomKey.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		LcomKey = aLcomKey[i];
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LcomKey, LcomData);
		item.Format(_T("%s(%s)"), LcomData.LoadCombName, m_csPostfix);
		m_aColInfo.Add(SLcomColItem(item, m_nLCtype, LcomKey));
	}
	m_nLcomCount = nCount;


	// Description column
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__Description), 0, 6));

	int nColNo = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);

	// Set Title Header
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));
	//SetStyleRange(CGXRange().SetCols(2), CGXStyle()
	//    .SetControl(GX_IDS_CTRL_CHECKBOX3D)
	//    .SetHorizontalAlignment(DT_CENTER));
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));
	CString csChoiceList = CCMLoadCombMainGrid::GetActiveChoiceList(m_nLcomType);
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T("")));

	CString TypeList = CCMLoadCombMainGrid::GetLcomTypeList(m_nLcomType);

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(TypeList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T("")));
	SetStyleRange(CGXRange().SetCols(6, nColNo-1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	// MNET1209, KJH20050204, JTG D60-04 only
	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("0")));
	SetStyleRange(CGXRange().SetCols(nColNo), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));
	SetFrozenCols(1, 1);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetColWidthByDPI(1, 1, 80);  // no
	SetColWidthByDPI(2, 2, 88);  // name
	SetColWidthByDPI(3, 3, 85);  // active
	SetColWidthByDPI(4, 4, 95);  // type
	SetColWidthByDPI(5, 5, 25);  // elastic stage => MNET:1209, KJH20050204
	SetColWidthByDPI(nColNo, nColNo, 230); // description
	if (nColNo > m_nFixedCol+1) ResizeColWidthsToFit(CGXRange().SetCols(m_nFixedCol+1, nColNo-1));

	// MNET:1209, KJH20050204
	BOOL bElastHide = !m_bElastStag || m_nLcomType!=D_LCOMTYPE_CONCRETE;
	SetStyleRange(CGXRange().SetCols(5), CGXStyle().SetEnabled(!bElastHide));
	HideCols(5, 5, bElastHide);
	// End KJH20050204

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMLoadCombGrid::SetValue(ROWCOL nRow, T_LCOM_K key, T_LCOM_D &lcom)
{
	CStringArray aValue;
	ConvDataToStrArray(lcom, aValue);
	int nSize = aValue.GetSize();
	for (int i = 0; i < nSize; i++)
		//SetValueRange(CGXRange(nRow, i+1), aValue[i]);
		SetExpressionRowCol(nRow, i+1, aValue[i]);

	// MNET:1209, KJH20050204
	if(m_bElastStag && m_nLcomType==D_LCOMTYPE_CONCRETE)
	{
		BOOL bEnable = aValue[2].Find(_LS(IDS_CMD_LCOM_SERVICE))>=0;
		SetStyleRange(CGXRange(nRow, 5, nRow, 5), CGXStyle().SetEnabled(bEnable));
	}
	// END KJH20050204

	// Key 설정
	SetStyleRange(CGXRange(nRow, 0), 
								CGXStyle().SetUserAttribute(ID_USER_ATTR_KEY, (long)key));
}

CString CCMLoadCombGrid::GetKeyString(ROWCOL nRow)
{
	CString csKey;
	CGXStyle style;
	GetStyleRowCol(nRow, 0, style);
	style.GetUserAttribute(ID_USER_ATTR_KEY, csKey);
	return csKey;
}

void CCMLoadCombGrid::SetHeaderTitle()
{
	int nColCount = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);
	SetColCount(nColCount);
	
	for (int i = 0; i < nColCount; i++)
	{
		SetStyleRange(CGXRange(0, (ROWCOL)(i+1)), CGXStyle()
			.SetValue(m_aColInfo[i].csName));
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMLoadCombGrid::ConvDataToStrArray(T_LCOM_D &lcom, CStringArray &aValue)
{
	int nSize = m_aColInfo.GetSize();
	aValue.SetSize(nSize);
	aValue[0].Format(_T("%d"), lcom.LoadCombId);
	aValue[1] = lcom.LoadCombName;
	aValue[2] = CCMLoadCombMainGrid::GetActiveName(m_nLcomType, lcom.nActive);
	aValue[3] = CCMLoadCombMainGrid::GetCombTypeStr(lcom.LoadCombType);
	aValue[4] = lcom.bElasticStage ? _T("1") : _T("0"); // MNET:1209, KJH20050204
	aValue[nSize-1] = lcom.Description;

	int nCount = m_pDoc->m_pAttrCtrl->GetCountLcomCase(m_nLcomType, lcom);

	CMap<T_SGLD_K, T_SGLD_K, int, int> mapErect;
	m_pDoc->m_pAttrCtrl->GetErectionSelected(mapErect);

	for (int i = 0; i < nCount; i++)
	{
#if defined(_CIVIL)
		if (lcom.aCombination[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = lcom.aCombination[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_CVL && SgldKey < D_SGLD_SUM_CVL)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}
#else
		if (lcom.aCombination[i].AnalType == D_LCOM_STAGE)
		{
			unsigned int SgldKey = lcom.aCombination[i].LoadCaseKey;
			// Erection Load 및 CS로 정의된 것만 보여주기(Tendon 제외).
			if (SgldKey >= D_SGLD_ER1_GEN && SgldKey < D_SGLD_SUM_GEN)
			{
				int nTemp = 0;
				if (!mapErect.Lookup(SgldKey, nTemp)) continue;
			}
		}
#endif
		SetLcomCaseValue(aValue, lcom.aCombination[i]);
	}
}

void CCMLoadCombGrid::SetLcomCaseValue(CStringArray &aValue, T_LCOM_BASE &base)
{
	int nSize = m_aColInfo.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		if (m_aColInfo[i].nLCtype == base.AnalType && m_aColInfo[i].nKey == base.LoadCaseKey) break;
	}
	if (i == nSize)
	{
		//ASSERT(0);  // not found -> ERROR
		return;
	}
	aValue[i].Format(_T("%.18f"), base.Factor);
}

void CCMLoadCombGrid::UpdateLoadCase(BOOL& bDoMakeItemEx)
{
	// Current Cell 보전을 위해 저장
	ROWCOL ncRow,ncCol;
	if (!GetCurrentCell(ncRow, ncCol))
	{ ncRow = ncCol = 1; }

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STLD_K stldKey, stldKeyBak;
	T_STLD_D stldData, stldDataBak;
	T_SPLC_K splcKey, splcKeyBak;
	T_SPLC_D splcData, splcDataBak;
	T_THIS_K thisKey, thisKeyBak;
	T_THIS_D thisData, thisDataBak;
	T_MVLD_K mvldKey, mvldKeyBak;
	T_MVLD_D mvldData, mvldDataBak;
	T_MVLDjp_K mvldjpKey, mvldjpKeyBak;
	T_MVLDjp_D mvldjpData, mvldjpDataBak;
	T_MVLDch_K mvldchKey, mvldchKeyBak;
	T_MVLDch_D mvldchData, mvldchDataBak;
	T_MVLDid_K mvldidKey, mvldidKeyBak;
	T_MVLDid_D mvldidData, mvldidDataBak;
	T_MVLDbs_K mvldbsKey, mvldbsKeyBak;
	T_MVLDbs_D mvldbsData, mvldbsDataBak;
	T_SMLC_K smlcKey, smlcKeyBak;
	T_SMLC_D smlcData, smlcDataBak;
	T_SGLD_K sgldKey;
	T_SGLD_D sgldData;
	T_ESPC_K espcKey, espcKeyBak;
	T_ESPC_D espcData, espcDataBak;
	T_NSPC_K nspcKey, nspcKeyBak;
	T_NSPC_D nspcData, nspcDataBak;
	T_GILC_K gilcKey, gilcKeyBak;
	T_GILC_D gilcData, gilcDataBak;
	T_LCOM_K lcomKey, lcomKeyBak;
	T_LCOM_D lcomData, lcomDataBak;
	
	//int nLcomType = D_LCOMTYPE_GENERAL;

	BOOL bSgldChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		int nLCtype = 0;
		
		switch(nCmd)
		{
		case(UR_STLD_ADD):
			{
				pViewBuff->GetStld(nKey, stldKey, stldData);
				InsertColumn(stldData.LoadCaseName, D_LCOM_STATIC, stldKey, stldData.LoadCaseId);
			}
			break;
		case(UR_STLD_DEL):
			{
				pViewBuff->GetStld(nKey, stldKey, stldData);
				DeleteColumn(D_LCOM_STATIC, stldKey);
			}
			break;
		case(UR_STLD_MFD):
			{
				pViewBuff->GetStld(nKey, stldKeyBak, stldDataBak);
			}
			break;
		case(UR_STLD_MFS):
			{
				pViewBuff->GetStld(nKey, stldKey, stldData);
				DeleteColumn(D_LCOM_STATIC, stldKey);
				InsertColumn(stldDataBak.LoadCaseName, D_LCOM_STATIC, stldKeyBak, stldDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;

		case(UR_SPLC_ADD):
			{
				pViewBuff->GetSplc(nKey, splcKey, splcData);
				nspcKey = espcKey = splcKey; nspcData = espcData = splcData;
				InsertColumn(splcData.LoadCaseName, D_LCOM_SPECTRUM, splcKey, splcData.LoadCaseId);
				InsertColumn(splcData.LoadCaseName, D_LCOM_SPECTRUM_ND, nspcKey, nspcData.LoadCaseId);
				InsertColumn(splcData.LoadCaseName, D_LCOM_ECCENSPEC, espcKey, espcData.LoadCaseId);
				InsertColumn(splcData.LoadCaseName, D_LCOM_ECCENSPEC_ND, nspcKey, nspcData.LoadCaseId);
			}
			break;
		case(UR_SPLC_DEL):
			{
				pViewBuff->GetSplc(nKey, splcKey, splcData);
				nspcKey = espcKey = splcKey;
				DeleteColumn(D_LCOM_SPECTRUM, splcKey);
				DeleteColumn(D_LCOM_SPECTRUM_ND, nspcKey);
				DeleteColumn(D_LCOM_ECCENSPEC, espcKey);
				DeleteColumn(D_LCOM_ECCENSPEC_ND, nspcKey);
			}
			break;
		case(UR_SPLC_MFD):
			{
				pViewBuff->GetSplc(nKey, splcKeyBak, splcDataBak);
				nspcKeyBak = espcKeyBak = splcKeyBak; nspcDataBak = espcDataBak = splcDataBak;
			}
			break;
		case(UR_SPLC_MFS):
			{
				pViewBuff->GetSplc(nKey, splcKey, splcData);
				nspcKey = espcKey = splcKey; espcData = nspcData = splcData;
				DeleteColumn(D_LCOM_SPECTRUM, splcKey);
				DeleteColumn(D_LCOM_SPECTRUM_ND, nspcKey);
				DeleteColumn(D_LCOM_ECCENSPEC, espcKey);
				DeleteColumn(D_LCOM_ECCENSPEC_ND, nspcKey);
				InsertColumn(splcDataBak.LoadCaseName, D_LCOM_SPECTRUM, splcKeyBak, splcDataBak.LoadCaseId);
				InsertColumn(nspcDataBak.LoadCaseName, D_LCOM_SPECTRUM_ND, nspcKeyBak, nspcDataBak.LoadCaseId);
				InsertColumn(espcDataBak.LoadCaseName, D_LCOM_ECCENSPEC, espcKeyBak, espcDataBak.LoadCaseId);
				InsertColumn(nspcDataBak.LoadCaseName, D_LCOM_ECCENSPEC_ND, nspcKeyBak, nspcDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;

		case(UR_THIS_ADD):
			{
				pViewBuff->GetThis(nKey, thisKey, thisData);
				InsertColumn(thisData.LoadCaseName, D_LCOM_HISTORY, thisKey, thisData.LoadCaseId);
			}
			break;
		case(UR_THIS_DEL):
			{
				pViewBuff->GetThis(nKey, thisKey, thisData);
				DeleteColumn(D_LCOM_HISTORY, thisKey);
			}
			break;
		case(UR_THIS_MFD):
			{
				pViewBuff->GetThis(nKey, thisKeyBak, thisDataBak);
			}
			break;
		case(UR_THIS_MFS):
			{
				pViewBuff->GetThis(nKey, thisKey, thisData);
				DeleteColumn(D_LCOM_HISTORY, thisKey);
				InsertColumn(thisDataBak.LoadCaseName, D_LCOM_HISTORY, thisKeyBak, thisDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;

		case(UR_MVLD_ADD):
			{
				pViewBuff->GetMvld(nKey, mvldKey, mvldData);
				InsertColumn(mvldData.LoadCaseName, D_LCOM_MOVING, mvldKey, mvldData.LoadCaseId);
			}
			break;
		case(UR_MVLD_DEL):
			{
				pViewBuff->GetMvld(nKey, mvldKey, mvldData);
				DeleteColumn(D_LCOM_MOVING, mvldKey);
			}
			break;
		case(UR_MVLD_MFD):
			{
				pViewBuff->GetMvld(nKey, mvldKeyBak, mvldDataBak);
			}
			break;
		case(UR_MVLD_MFS):
			{
				pViewBuff->GetMvld(nKey, mvldKey, mvldData);
				DeleteColumn(D_LCOM_MOVING, mvldKey);
				InsertColumn(mvldDataBak.LoadCaseName, D_LCOM_MOVING, mvldKeyBak, mvldDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;
		// japan version
		case(UR_MVLDjp_ADD):
			{
				pViewBuff->GetMvldjp(nKey, mvldjpKey, mvldjpData);
				InsertColumn(mvldjpData.LoadCaseName, D_LCOM_MOVING, mvldjpKey, mvldjpData.LoadCaseId);
			}
			break;
		case(UR_MVLDjp_DEL):
			{
				pViewBuff->GetMvldjp(nKey, mvldjpKey, mvldjpData);
				DeleteColumn(D_LCOM_MOVING, mvldjpKey);
			}
			break;
		case(UR_MVLDjp_MFD):
			{
				pViewBuff->GetMvldjp(nKey, mvldjpKeyBak, mvldjpDataBak);
			}
			break;
		case(UR_MVLDjp_MFS):
			{
				pViewBuff->GetMvldjp(nKey, mvldjpKey, mvldjpData);
				DeleteColumn(D_LCOM_MOVING, mvldjpKey);
				InsertColumn(mvldjpDataBak.LoadCaseName, D_LCOM_MOVING, mvldjpKeyBak, mvldjpDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;
		// Chinese Version
		case(UR_MVLDch_ADD):
			{
				pViewBuff->GetMvldch(nKey, mvldchKey, mvldchData);
				InsertColumn(mvldchData.LoadCaseName, D_LCOM_MOVING, mvldchKey, mvldchData.LoadCaseId);
			}
			break;
		case(UR_MVLDch_DEL):
			{
				pViewBuff->GetMvldch(nKey, mvldchKey, mvldchData);
				DeleteColumn(D_LCOM_MOVING, mvldchKey);
			}
			break;
		case(UR_MVLDch_MFD):
			{
				pViewBuff->GetMvldch(nKey, mvldchKeyBak, mvldchDataBak);
			}
			break;
		case(UR_MVLDch_MFS):
			{
				pViewBuff->GetMvldch(nKey, mvldchKey, mvldchData);
				DeleteColumn(D_LCOM_MOVING, mvldchKey);
				InsertColumn(mvldchDataBak.LoadCaseName, D_LCOM_MOVING, mvldchKeyBak, mvldchDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;
		case(UR_MVLDid_ADD):
			{
				pViewBuff->GetMvldid(nKey, mvldidKey, mvldidData);
				InsertColumn(mvldidData.LoadCaseName, D_LCOM_MOVING, mvldidKey, mvldidData.LoadCaseId);
			}
			break;
		case(UR_MVLDid_DEL):
			{
				pViewBuff->GetMvldid(nKey, mvldidKey, mvldidData);
				DeleteColumn(D_LCOM_MOVING, mvldidKey);
			}
			break;
		case(UR_MVLDid_MFD):
			{
				pViewBuff->GetMvldid(nKey, mvldidKeyBak, mvldidDataBak);
			}
			break;
		case(UR_MVLDid_MFS):
			{
				pViewBuff->GetMvldid(nKey, mvldidKey, mvldidData);
				DeleteColumn(D_LCOM_MOVING, mvldidKey);
				InsertColumn(mvldidDataBak.LoadCaseName, D_LCOM_MOVING, mvldidKeyBak, mvldidDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;
		case(UR_MVLDbs_ADD):
			{
				pViewBuff->GetMvldbs(nKey, mvldbsKey, mvldbsData);
				InsertColumn(mvldbsData.LoadCaseName, D_LCOM_MOVING, mvldbsKey, mvldbsData.LoadCaseId);
			}
			break;
		case(UR_MVLDbs_DEL):
			{
				pViewBuff->GetMvldbs(nKey, mvldbsKey, mvldbsData);
				DeleteColumn(D_LCOM_MOVING, mvldbsKey);
			}
			break;
		case(UR_MVLDbs_MFD):
			{
				pViewBuff->GetMvldbs(nKey, mvldbsKeyBak, mvldbsDataBak);
			}
			break;
		case(UR_MVLDbs_MFS):
			{
				pViewBuff->GetMvldbs(nKey, mvldbsKey, mvldbsData);
				DeleteColumn(D_LCOM_MOVING, mvldbsKey);
				InsertColumn(mvldbsDataBak.LoadCaseName, D_LCOM_MOVING, mvldbsKeyBak, mvldbsDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;      
		case(UR_SMLC_ADD):
			{
				pViewBuff->GetSmlc(nKey, smlcKey, smlcData);
				InsertColumn(smlcData.LoadCaseName, D_LCOM_SETTLEMENT, smlcKey, smlcData.LoadCaseId);
			}
			break;
		case(UR_SMLC_DEL):
			{
				pViewBuff->GetSmlc(nKey, smlcKey, smlcData);
				DeleteColumn(D_LCOM_SETTLEMENT, smlcKey);
			}
			break;
		case(UR_SMLC_MFD):
			{
				pViewBuff->GetSmlc(nKey, smlcKeyBak, smlcDataBak);
			}
			break;
		case(UR_SMLC_MFS):
			{
				pViewBuff->GetSmlc(nKey, smlcKey, smlcData);
				DeleteColumn(D_LCOM_SETTLEMENT, smlcKey);
				InsertColumn(smlcDataBak.LoadCaseName, D_LCOM_SETTLEMENT, smlcKeyBak, smlcDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;
		case(UR_STAG_ADD):
		case(UR_STAG_DEL):
			bSgldChanged = TRUE;
			bDoMakeItemEx = TRUE;
			// Modify될 떄 한번만 DoMakeItemEX를 부를려고 넣은 코드 인데.. STAG는 모양이 달라서 이렇게 넣음
			break;

		case(UR_GILC_ADD):
			{
				pViewBuff->GetGilc(nKey, gilcKey, gilcData);
				InsertColumn(gilcData.LoadCaseName, D_LCOM_GRIDMODEL, gilcKey, gilcData.LoadCaseId);
			}
			break;
		case(UR_GILC_DEL):
			{
				pViewBuff->GetGilc(nKey, gilcKey, gilcData);
				DeleteColumn(D_LCOM_GRIDMODEL, gilcKey);
			}
			break;
		case(UR_GILC_MFD):
			{
				pViewBuff->GetGilc(nKey, gilcKeyBak, gilcDataBak);
			}
			break;
		case(UR_GILC_MFS):
			{
				pViewBuff->GetGilc(nKey, gilcKey, gilcData);
				DeleteColumn(D_LCOM_GRIDMODEL, gilcKey);
				InsertColumn(gilcDataBak.LoadCaseName, D_LCOM_GRIDMODEL, gilcKeyBak, gilcDataBak.LoadCaseId);
				bDoMakeItemEx = TRUE;
			}
			break;

		case(UR_CRAN_ADD):
		case(UR_CRAN_DEL):
		case(UR_CRAN_MFS):
			if(m_pDoc->m_pAttrCtrl->GetCountCran()>0)
			{
				InsertColumn(CString(_LS(IDS_CMD_CRANE)), D_LCOM_CRANE, 1, 1);
				if(m_pDoc->m_pAttrCtrl->IsExistCranBraking())
					InsertColumn(CString(_LS(IDS_CMD_CRANE_BRK)), D_LCOM_CRANE, 2, 2);
				else
					DeleteColumn(D_LCOM_CRANE, 2);
			}
			else
			{
				DeleteColumn(D_LCOM_CRANE, 1);
				DeleteColumn(D_LCOM_CRANE, 2);
			}
			bDoMakeItemEx = TRUE; // Modify될 떄 한번만 DoMakeItemEX를 부를려고 넣은 코드 인데.. CRAN은 UR_CRAN_MFD가 불리는지 확신할 수 없어서 이렇게 넣음
			break;
		case(UR_CRAN_MFD):
			break;

		case(UR_LCOM_ADD):
			if (nCmd == UR_LCOM_ADD) nLCtype = D_LCOM_CB_GENERAL;
		case(UR_LCST_ADD):
			if (nCmd == UR_LCST_ADD) nLCtype = D_LCOM_CB_STEEL;
		case(UR_LCCO_ADD):
			if (nCmd == UR_LCCO_ADD) nLCtype = D_LCOM_CB_CONCRETE;
		case(UR_LCFT_ADD):
			if (nCmd == UR_LCFT_ADD) nLCtype = D_LCOM_CB_FOOTING;
		case(UR_LCSC_ADD):
			if (nCmd == UR_LCSC_ADD) nLCtype = D_LCOM_CB_SRC;
		case(UR_LCAL_ADD):
			if (nCmd == UR_LCAL_ADD) nLCtype = D_LCOM_CB_ALUMINUM;
		case(UR_LCSCM_ADD):
			if (nCmd == UR_LCSCM_ADD) nLCtype = D_LCOM_CB_STLCOMP;
		case(UR_LCCF_ADD):
			if (nCmd == UR_LCCF_ADD) nLCtype = D_LCOM_CB_CFSTEEL;
		case(UR_LCSM_ADD):
			if (nCmd == UR_LCSM_ADD) nLCtype = D_LCOM_CB_SEISMIC;
		case(UR_LCLN_ADD):
			if (nCmd == UR_LCLN_ADD) nLCtype = D_LCOM_CB_LINEAR_EVAL;

			// 요 코드는 헷갈리기 쉬우니 주의..; break없이 ADD들은 여기까지 옴
			if (nLCtype == m_nLCtype)
			{
				pViewBuff->GetLcom(m_nLcomType, nKey, lcomKey, lcomData);
				InsertColumn(lcomData.LoadCombName, m_nLCtype, lcomKey, lcomData.LoadCombId);
			}
			break;
		case(UR_LCOM_DEL):
			if (nCmd == UR_LCOM_DEL) nLCtype = D_LCOM_CB_GENERAL;
		case(UR_LCST_DEL):
			if (nCmd == UR_LCST_DEL) nLCtype = D_LCOM_CB_STEEL;
		case(UR_LCCO_DEL):
			if (nCmd == UR_LCCO_DEL) nLCtype = D_LCOM_CB_CONCRETE;
		case(UR_LCFT_DEL):
			if (nCmd == UR_LCFT_DEL) nLCtype = D_LCOM_CB_FOOTING;
		case(UR_LCSC_DEL):
			if (nCmd == UR_LCSC_DEL) nLCtype = D_LCOM_CB_SRC;
		case(UR_LCAL_DEL):
			if (nCmd == UR_LCAL_DEL) nLCtype = D_LCOM_CB_ALUMINUM;
		case(UR_LCSCM_DEL):
			if (nCmd == UR_LCSCM_DEL) nLCtype = D_LCOM_CB_STLCOMP;
		case(UR_LCCF_DEL):
			if (nCmd == UR_LCCF_DEL) nLCtype = D_LCOM_CB_CFSTEEL;
		case(UR_LCSM_DEL):
			if (nCmd == UR_LCSM_DEL) nLCtype = D_LCOM_CB_SEISMIC;
		case(UR_LCLN_DEL):
			if (nCmd == UR_LCLN_DEL) nLCtype = D_LCOM_CB_LINEAR_EVAL;
			if (nLCtype == m_nLCtype)
			{
				pViewBuff->GetLcom(m_nLcomType, nKey, lcomKey, lcomData);
				DeleteColumn(m_nLCtype, lcomKey);
			}
			break;
		case(UR_LCOM_MFD):
			if (nCmd == UR_LCOM_MFD) nLCtype = D_LCOM_CB_GENERAL;
		case(UR_LCST_MFD):
			if (nCmd == UR_LCST_MFD) nLCtype = D_LCOM_CB_STEEL;
		case(UR_LCCO_MFD):
			if (nCmd == UR_LCCO_MFD) nLCtype = D_LCOM_CB_CONCRETE;
		case(UR_LCFT_MFD):
			if (nCmd == UR_LCFT_MFD) nLCtype = D_LCOM_CB_FOOTING;
		case(UR_LCSC_MFD):
			if (nCmd == UR_LCSC_MFD) nLCtype = D_LCOM_CB_SRC;
		case(UR_LCAL_MFD):
			if (nCmd == UR_LCAL_MFD) nLCtype = D_LCOM_CB_ALUMINUM;
		case(UR_LCSCM_MFD):
			if (nCmd == UR_LCSCM_MFD) nLCtype = D_LCOM_CB_STLCOMP;
		case(UR_LCCF_MFD):
			if (nCmd == UR_LCCF_MFD) nLCtype = D_LCOM_CB_CFSTEEL;
		case(UR_LCSM_MFD):
			if (nCmd == UR_LCSM_MFD) nLCtype = D_LCOM_CB_SEISMIC;
		case(UR_LCLN_MFD):
			if (nCmd == UR_LCLN_MFD) nLCtype = D_LCOM_CB_LINEAR_EVAL;
			if (nLCtype == m_nLCtype)
			{
				pViewBuff->GetLcom(m_nLcomType, nKey, lcomKeyBak, lcomDataBak);
				bDoMakeItemEx = TRUE;
			}
			break;
		case(UR_LCOM_MFS):
			if (nCmd == UR_LCOM_MFS) nLCtype = D_LCOM_CB_GENERAL;
		case(UR_LCST_MFS):
			if (nCmd == UR_LCST_MFS) nLCtype = D_LCOM_CB_STEEL;
		case(UR_LCCO_MFS):
			if (nCmd == UR_LCCO_MFS) nLCtype = D_LCOM_CB_CONCRETE;
		case(UR_LCFT_MFS):
			if (nCmd == UR_LCFT_MFS) nLCtype = D_LCOM_CB_FOOTING;
		case(UR_LCSC_MFS):
			if (nCmd == UR_LCSC_MFS) nLCtype = D_LCOM_CB_SRC;
		case(UR_LCAL_MFS):
			if (nCmd == UR_LCAL_MFS) nLCtype = D_LCOM_CB_ALUMINUM;
		case(UR_LCSCM_MFS):
			if (nCmd == UR_LCSCM_MFS) nLCtype = D_LCOM_CB_STLCOMP;
		case(UR_LCCF_MFS):
			if (nCmd == UR_LCCF_MFS) nLCtype = D_LCOM_CB_CFSTEEL;
		case(UR_LCSM_MFS):
			if (nCmd == UR_LCSM_MFS) nLCtype = D_LCOM_CB_SEISMIC;
		case(UR_LCLN_MFS):
			if (nCmd == UR_LCLN_MFS) nLCtype = D_LCOM_CB_LINEAR_EVAL;
			if (nLCtype == m_nLCtype)
			{
				pViewBuff->GetLcom(m_nLcomType, nKey, lcomKey, lcomData);
				DeleteColumn(m_nLCtype, lcomKey);
				InsertColumn(lcomDataBak.LoadCombName, m_nLCtype, lcomKeyBak, lcomDataBak.LoadCombId);
			}
			break;
		}
	}
	if (bSgldChanged)
	{
		int nSgldNum = m_pDoc->m_pAttrCtrl->GetCountSgld();
		if (m_nSgldCount == 0 && nSgldNum > 0)
		{
			for (int i = 0; i < nSgldNum; i++)
			{
				sgldKey = i+1;
				m_pDoc->m_pAttrCtrl->GetSgld(sgldKey, sgldData);
				InsertColumn(sgldData.LoadCaseName, D_LCOM_STAGE, sgldKey, sgldData.LoadCaseId);
			}
		}
		else if (m_nSgldCount > 0 && nSgldNum == 0)
		{
			for (int i = 0; i < nSgldNum; i++)
			{
				sgldKey = i+1;
				DeleteColumn(D_LCOM_STAGE, sgldKey);
			}
		}
	}

	// Current Cell이 없으면 복구한다.
	ROWCOL ntRow, ntCol;
	if (!GetCurrentCell(ntRow, ntCol)) 
	{
		if (GetColCount() < ncCol) ncCol = GetColCount();

		BOOL bSet = SetLeftValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetDownValidCell(ncRow, ncCol);
		ASSERT(bSet);
	}
}

BOOL CCMLoadCombGrid::InsertRecordByKey(T_LCOM_K Key, T_LCOM_D &Data)
{
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL i = 0;
	for (i = 0; i < nRowCount; i++)
		if (_tstol(GetValueRowCol(i+1, 1)) > Data.LoadCombId) break;

	InsertRecord(i+1);
	SetValue(i+1, Key, Data);

	return TRUE;
}

BOOL CCMLoadCombGrid::DeleteRecordByKey(T_LCOM_K Key, T_LCOM_D &Data)
{
	ROWCOL ncRow, ncCol;
	BOOL bCurCell = GetCurrentCell(ncRow, ncCol);
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	for (ROWCOL i = 0; i < nRowCount; i++)
	{
		if (_tstol(GetValueRowCol(i+1, 1)) == Data.LoadCombId)
		{
			if (bCurCell && i+1 == ncRow && m_nEditMode == edit) CancelEdit();
			DeleteRecord(i+1); 
			break;
		}
	}
	return TRUE;
}

BOOL CCMLoadCombGrid::InsertColumn(CString &csName, int nLCtype, T_LCOM_K nKey, int nLCid)
{
	int nStart;
	int nCount;
	int nPos;
	int i = 0;
	if (nLCtype == D_LCOM_STATIC) 
	{
		nStart = m_nFixedCol;
		nCount = m_nStldCount;
		T_STLD_D Data;
		m_pDoc->m_pAttrCtrl->GetStld(nKey, Data);
		if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) return TRUE;
		for (i = 0; i < nCount; i++)
		{
			m_pDoc->m_pAttrCtrl->GetStld(m_aColInfo[nStart+i].nKey, Data);
			if (Data.LoadCaseId > nLCid) break;
		}
		csName += _T("(ST)");
		nPos = nStart+i;
		m_nStldCount++;
	}
	else if (nLCtype == D_LCOM_SPECTRUM) 
	{
		nStart = m_nFixedCol+m_nStldCount;
		nCount = m_nSplcCount;
		T_SPLC_D Data;
		for (i = 0; i < nCount; i++)
		{
			m_pDoc->m_pAttrCtrl->GetSplc(m_aColInfo[nStart+i].nKey, Data);
			if (Data.LoadCaseId > nLCid) break;
		}
		csName += _T("(RS)");
		nPos = nStart+i;
		m_nSplcCount++;
	}
	else if (nLCtype == D_LCOM_SPECTRUM_ND) 
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount;
		nCount = m_nNspcCount;
		T_NSPC_D Data;
		m_pDoc->m_pAttrCtrl->GetNspc(m_aColInfo[nStart].nKey, Data);
		csName += _T("(NRS)");
		nPos = nStart;
		m_nNspcCount++;
	}
	else if (nLCtype == D_LCOM_HISTORY)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount;
		nCount = m_nThisCount;
		T_THIS_D Data;
		for (i = 0; i < nCount; i++)
		{
			m_pDoc->m_pAttrCtrl->GetThis(m_aColInfo[nStart+i].nKey, Data);
			if (Data.LoadCaseId > nLCid) break;
		}
		csName += _T("(TH)");
		nPos = nStart+i;
		m_nThisCount++;
	}
	else if (nLCtype == D_LCOM_MOVING)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount;
		nCount = m_nMvldCount;
		// 코드에 따른 방식으로 수정 
		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
		{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			{
				T_MVLD_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvld(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		case D_MOVE_CODE_CHINA:
			{
				T_MVLDch_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldch(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		case D_MOVE_CODE_JAPAN:
			{
				T_MVLDjp_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldjp(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		case D_MOVE_CODE_INDIA:
			{
				T_MVLDid_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldid(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		case D_MOVE_CODE_BS:
			{
				T_MVLDbs_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldbs(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		case D_MOVE_CODE_FRANCE:
			{
				T_MVLDfr_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldfr(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		case D_MOVE_CODE_TRANS:
			{
				T_MVLDtr_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldtr(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			break;
		default:
			ASSERT(0);
			break;
		}
		/*
		int nMovingType = CProduct::GetMovingType();
		if (nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_US)
		{
			if(nMovingType == D_PRODUCT_MOVING_US && CProduct::IsIndia())
			{
				T_MVLDid_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvldid(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
			else
			{
				T_MVLD_D Data;
				for (i = 0; i < nCount; i++)
				{
					m_pDoc->m_pAttrCtrl->GetMvld(m_aColInfo[nStart+i].nKey, Data);
					if (Data.LoadCaseId > nLCid) break;
				}
			}
		}
		else if (nMovingType == D_PRODUCT_MOVING_JP)
		{
			T_MVLDjp_D Data;
			for (i = 0; i < nCount; i++)
			{
				m_pDoc->m_pAttrCtrl->GetMvldjp(m_aColInfo[nStart+i].nKey, Data);
				if (Data.LoadCaseId > nLCid) break;
			}
		}
		else if (nMovingType == D_PRODUCT_MOVING_CH)
		{
			T_MVLDch_D Data;
			for (i = 0; i < nCount; i++)
			{
				m_pDoc->m_pAttrCtrl->GetMvldch(m_aColInfo[nStart+i].nKey, Data);
				if (Data.LoadCaseId > nLCid) break;
			}
		}
		else
		{
			ASSERT(0);
		}
		*/
		csName += _T("(MV)");
		nPos = nStart+i;
		m_nMvldCount++;
	}
	else if (nLCtype == D_LCOM_SETTLEMENT)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount;
		nCount = m_nSmlcCount;
		T_SMLC_D Data;
		for (i = 0; i < nCount; i++)
		{
			m_pDoc->m_pAttrCtrl->GetSmlc(m_aColInfo[nStart+i].nKey, Data);
			if (Data.LoadCaseId > nLCid) break;
		}
		csName += _T("(SM)");
		nPos = nStart+i;
		m_nSmlcCount++;
	}
	else if (nLCtype == D_LCOM_STAGE)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount+m_nSmlcCount;
		nCount = m_nSgldCount;
		T_SGLD_D Data;
		m_pDoc->m_pAttrCtrl->GetSgld(m_aColInfo[nStart].nKey, Data);
		csName += _T("(CS)");
		nPos = nStart;
		m_nSgldCount++;
	}
	else if (nLCtype == D_LCOM_ECCENSPEC)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount+m_nSmlcCount+m_nSgldCount;
		nCount = m_nEspcCount;
		T_ESPC_D Data;
		m_pDoc->m_pAttrCtrl->GetEspc(m_aColInfo[nStart].nKey, Data);
		csName += _T("(ES)");
		nPos = nStart;
		m_nEspcCount++;
	}
	else if (nLCtype == D_LCOM_ECCENSPEC_ND)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount+m_nSmlcCount+m_nSgldCount+m_nEspcCount;
		nCount = m_nNespCount;
		T_NSPC_D Data;
		m_pDoc->m_pAttrCtrl->GetNspc(m_aColInfo[nStart].nKey, Data);
		csName += _T("(NES)");
		nPos = nStart;
		m_nNespCount++;
	}
	else if (nLCtype == D_LCOM_GRIDMODEL)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount+m_nSmlcCount+m_nSgldCount+m_nEspcCount+m_nNespCount;
		nCount = m_nGilcCount;
		T_GILC_D Data;
		m_pDoc->m_pAttrCtrl->GetGilc(m_aColInfo[nStart].nKey, Data);
		csName += _T("(GI)");
		nPos = nStart;
		m_nGilcCount++;
	}
	else if (nLCtype == D_LCOM_CRANE)
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount+m_nSmlcCount+m_nSgldCount+m_nEspcCount+m_nNespCount+m_nGilcCount;
		nCount = m_nCranCount;
		csName += _T("(CN)");
		nPos = (nLCid==1) ? nStart : nStart+1;
		m_nCranCount++;
	}
	else if (nLCtype == m_nLCtype) 
	{
		nStart = m_nFixedCol+m_nStldCount+m_nSplcCount+m_nNspcCount+m_nThisCount+m_nMvldCount+m_nSmlcCount+m_nSgldCount+m_nEspcCount+m_nNespCount+m_nGilcCount+m_nCranCount;
		nCount = m_nLcomCount;
		T_LCOM_D Data;
		for (i = 0; i < nCount; i++)
		{
			m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, m_aColInfo[nStart+i].nKey, Data);
			if (Data.LoadCombId > nLCid) break;
		}
		csName += _T("(") + m_csPostfix + _T(")");
		nPos = nStart+i;
		m_nLcomCount++;
	}

	m_aColInfo.InsertAt(nPos, SLcomColItem(csName, nLCtype, nKey));
	BOOL bOldLock = LockUpdate(TRUE);
	InsertCols(nPos+1, 1);
	SetStyleRange(CGXRange(0, (ROWCOL)(nPos+1)), CGXStyle().SetValue(csName));
	SetStyleRange(CGXRange().SetCols(nPos+1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	ResizeColWidthsToFit(CGXRange().SetCols(nPos+1, nPos+1));
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;
}

BOOL CCMLoadCombGrid::DeleteColumn(int nLCtype, T_LCOM_K nKey)
{
	int nCount = m_aColInfo.GetSize();
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (m_aColInfo[i].nLCtype == nLCtype && m_aColInfo[i].nKey == nKey) break;

	if (i == nCount) return FALSE;
	m_aColInfo.RemoveAt(i);
	RemoveCols(i+1, i+1);

	if (nLCtype == D_LCOM_STATIC) m_nStldCount--;
	else if (nLCtype == D_LCOM_SPECTRUM) m_nSplcCount--;
	else if (nLCtype == D_LCOM_SPECTRUM_ND) m_nNspcCount--;
	else if (nLCtype == D_LCOM_HISTORY) m_nThisCount--;
	else if (nLCtype == D_LCOM_MOVING) m_nMvldCount--;
	else if (nLCtype == D_LCOM_SETTLEMENT) m_nSmlcCount--;
	else if (nLCtype == D_LCOM_STAGE) m_nSgldCount--;
	else if (nLCtype == D_LCOM_ECCENSPEC) m_nEspcCount--;
	else if (nLCtype == D_LCOM_ECCENSPEC_ND) m_nNespCount--;
	else if (nLCtype == D_LCOM_GRIDMODEL) m_nGilcCount--;
	else if (nLCtype == D_LCOM_CRANE) m_nCranCount--;
	else if (nLCtype == m_nLCtype) m_nLcomCount--;
	else ASSERT(0);
	
	return TRUE;
}

// 인자로 넘어온 배열에 있는 모든 row를 Select 상태로 변경한다.
// 기존의 Select된 row는 없어진다.
void CCMLoadCombGrid::SelectGridRow(long* aRow, long nSize)
{
	// Row 배열을 오름차순으로 sorting한다.
	qsort((void*)aRow, nSize, sizeof(long), CCompFunc::longAsc);

	CGXRangeList* pSelList = GetParam()->GetRangeList();
	pSelList->DeleteAll();  // 기존 선택을 해제한다.

	for (long i = 0; i < nSize; )
	{
		CGXRange* pRange = new CGXRange();
		// 연속된 Row를 찾아서 1개의 Range로 만든다.
		long j = 0;
		for (j = i+1; j < nSize; j++)
		{
			if (aRow[j] != aRow[j-1] + 1) break;
		}
		pRange->SetRows(aRow[i], aRow[j-1]);
		pSelList->AddTail(pRange);
		i = j;
	}
}

BOOL CCMLoadCombGrid::GetValue(ROWCOL nRow, T_LCOM_D &lcom)
{
	lcom.Initialize();

	ROWCOL nColCount = GetColCount();
	CStringArray aValue;
	for (int i = 0; i < nColCount; i++)
		aValue.Add(GetValueRowCol(nRow, i+1));

	return ConvStrArrayToData(aValue, lcom);
}


//--------------------------------------------------------------------------
// Overriadable
// Return은 항상 SetFieldValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMLoadCombGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	int nColID;
	VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	// MNET:1209, KJH20050204
	if(nCol==3 && m_bElastStag && m_nLcomType==D_LCOMTYPE_CONCRETE)
	{
		BOOL bEnable = value.Find(_LS(IDS_CMD_LCOM_SERVICE))>=0;
		SetStyleRange(CGXRange(nRow, 5, nRow, 5), CGXStyle().SetEnabled(bEnable));
	}
	// END KJH20050204

	if (!ValidateField(value, nColID)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
}

// Return은 항상 SetRecordValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMLoadCombGrid::OnValidateRecord(ROWCOL nRow)
{
	// CStringArray 레코드를 만들어 validation을 검사한다.
	CRowColArray aCols;
	CStringArray value;
	int nColCount = GetColCount();
	aCols.SetSize(nColCount);
	value.SetSize(nColCount);
	for (int i = 0; i < nColCount; i++)
	{
		VERIFY(GetColumnNoFor(i, aCols[i]));
		value[i] = GetValueRowCol(nRow, i+1);
	}
	if (!ValidateRecord(value, aCols)) return SetRecordValidation(FALSE);
	return SetRecordValidation(TRUE);
}

BOOL CCMLoadCombGrid::ValidateField(CString& value, int nColID)
{
	ROWCOL nColCount = GetColCount();
	if (nColID > 4 && nColID < nColCount-1)
	{
		int nErrCode;
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}    
	}

	return TRUE;
}

BOOL CCMLoadCombGrid::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
	int nPos = aCols[2]-1;
	CString csName = aValue[nPos];
	nPos = aCols[3]-1;
	CString csType = aValue[nPos];
	csName.TrimLeft(); csName.TrimRight();
	csType.TrimLeft(); csType.TrimRight();

	if (csName == _T(""))
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Name_has_not_been_input_));
		return SetRecordValidation(FALSE);
	}
	if (csType == _T(""))
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Type_has_not_been_input_));
		return SetRecordValidation(FALSE);
	}

	ROWCOL nColCount = GetColCount();
	for (int i = 5; i < nColCount-1; i++) // load case factor field
	{
		nPos = aCols[i]-1;
		if (!ValidateField(aValue[nPos], i)) return FALSE;
	}
	return TRUE;
}

BOOL CCMLoadCombGrid::OnAddNewRecord(ROWCOL nRow)
{
	T_LCOM_D lcom;
	if (!GetValue(nRow, lcom)) return FALSE;

	BOOL bOldLock = LockUpdate(TRUE);
	if (!m_pDoc->m_pDataCtrl->AddLcom(m_nLcomType, lcom))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}
	CancelEdit();
	SetEnableDisableLcomCol();  // Add의 경우 여기서 한 번 더 호출해야 한다.
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	GetBrowseParam()->m_nRecordCount -= 1; 
	return SetRecordValidation(TRUE);
}

BOOL CCMLoadCombGrid::OnModifyRecord(ROWCOL nRow)
{
	T_LCOM_D lcom;

	CString csOldName = GetValueRowColBeforeEdit(nRow, 2);
	if (!GetValue(nRow, lcom)) return FALSE;

	// 수정한다. 실패하면 에러 리턴
	BOOL bOldLock = LockUpdate(TRUE);
	if (!m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, csOldName, lcom))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return SetRecordValidation(TRUE);
}

BOOL CCMLoadCombGrid::OnDeleteRecord(ROWCOL nRow)
{
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	CString csName;
	csName = GetValueRowCol(nRow, 2);
	if (m_nEditMode == addnew) return CancelEdit(); // addnew 상태이면 그냥 취소
	if (!m_pDoc->m_pDataCtrl->DelLcom(m_nLcomType, csName)) return FALSE;

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);
	return TRUE;
}

BOOL CCMLoadCombGrid::OnDeleteRecords(CRowColArray &awRows)
{
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bNewRecordIncluded = FALSE;
	if (m_nEditMode == addnew)
	{
		// addnew 상태에서 현재 행이 포함되어 있으면 뺀다.
		for (int i = 0; i < awRows.GetSize(); i++)
		{
			if (GetBrowseParam()->m_nCurrentRow == awRows.GetAt(i))
			{ awRows.RemoveAt(i); bNewRecordIncluded = TRUE; break; }
		}
	}
	
	CArray<CString, CString&> aName;
	int nSize = awRows.GetSize();
	if (nSize == 0) return FALSE;
	aName.SetSize(nSize);
	for (int i = 0; i < nSize; i++)
		aName[i] = GetValueRowCol(awRows[i], 2);
	if (!m_pDoc->m_pDataCtrl->DelLcom(m_nLcomType, aName)) return FALSE;
	if (bNewRecordIncluded) VERIFY(CancelEdit());

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);

	return TRUE;
}

//--------------------------------------------------------------------------
// Grid's Standard Overridable Functions
// Record선택시 title row와 append row는 제거한다.
void CCMLoadCombGrid::OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey)
{
	long nSize, nTmpSize;
	if (changedRect)
	{
		if (!bIsDraggin)
		{
			CRowColArray awRows;
			//if ((nSize = GetSelectedRows(awRows, TRUE, FALSE)) == 0)
			nSize = GetSelectedRows(awRows, TRUE, FALSE);
			// 전체 테이블 선택시 헤더와 append row는 제거한다.
			for (long i = nSize-1; i >= 0; i--)
				if (awRows[i] == GetRowCount() || awRows[i] == 0)
					awRows.RemoveAt(i);
			nTmpSize = awRows.GetSize();
			if (nSize != nTmpSize)
			{
				SelectGridRow((long*)awRows.GetData(), nTmpSize);
				if (nTmpSize + 2 != nSize)  // append row가 선택된 경우는 다시 그려준다.
				{
					BOOL bOldLock = LockUpdate(FALSE);
					if (!bOldLock) Redraw();
					LockUpdate(bOldLock);
				}
			}
		}
	}
}

// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CCMLoadCombGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	// 수정 모드이면 에러 리턴
	if (IsEdit()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_Move_row_while_editing_));
		return FALSE;
	}

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount()+1) nDestRow--;  

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow-nFirstRow+1) return FALSE;


	int nStart, nInc;
	nStart = (int)nDestRow;
	if (nDestRow < nFirstRow)  // 선택 영역 앞으로 이동
	{
		nInc = 1;
	}
	else  // 선택 영역 뒤로 이동
	{
		nStart--;
		nInc = 0;
	}

	CArray<CString, CString&> aLoadCombName;
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	CString LoadCombName;
	T_LCOM_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		LoadCombName = GetValueRowColBeforeEdit(i, 2);
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LoadCombName, data);
		data.LoadCombId = nStart;
		nStart += nInc;

		aLoadCombName.Add(data.LoadCombName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, aLoadCombName, aData);
	return FALSE;
}

// 항상 FALSE를 리턴해야 한다.
// 왜냐 하면 Grid가 Paste 작업을 해서는 안 되기 때문
BOOL CCMLoadCombGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴

	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CGXGridWnd x;
	int nPasteMode;
	int nPastedCols;

	// 임시 GridWnd에 Paste 작업을 한다.
	// 먼저 Default Value Array를 만든다.
	int nColCount = GetColCount();
	CString* aDefVal = new CString[nColCount-1];  // key는 뺀다.
	for (int ix = 0; ix < nColCount-1; ix++)
		aDefVal[ix] = GetDefValue(ix);
	/*
	aDefVal[0] = _T("");  // name
	aDefVal[1] = CCMLoadCombMainGrid::GetLcomActiveDef(m_nLcomType);
	//aDefVal[2] = _LS(IDS_WG_CMD__ADDD__Linear);  // type
	aDefVal[2] = D_LCOM_CT_ADD;
	for (int ix = 3; ix < nColCount-1; ix++) aDefVal[ix] = _T("");
	*/

	if (!PasteFromClipboard(range, aDefVal, 0, x, nPasteMode, nPastedCols))
	{
		delete []aDefVal;
		x.DestroyWindow();
		DisplayWarningText();
		return FALSE;
	}
	delete []aDefVal;

	CStringArray value;
	value.SetSize(nColCount);
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	CRowColArray aCols;
	aCols.SetSize(nColCount);
	for (int i = 0; i < nColCount; i++)
		VERIFY(GetColumnNoFor(i, aCols[i]));

	// DB 수정이 되지 않는 경우이므로 Copy되는 Field Validation만 수행한다.
	if (nPasteMode == TB_PASTE_MODE_ONMODIFY)
	{
		int nColID;
		for (int j = range.left; j < range.left+nPastedCols; j++)
		{
			VERIFY(GetColumnIDFor(j, nColID));
			value[j-1] = x.GetValueRowCol(1, j);
			if (!ValidateField(value[j-1], nColID))
			{
				x.DestroyWindow();
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	else // DB에 추가 또는 수정이 일어나는 경우이므로 Record Validation을 한다.
	{
		int nxRowCount = x.GetRowCount();
		for (int i = 1; i <= nxRowCount; i++)
		{
			// Table에서 레코드 값을 가져온다.
			int nxColCount = x.GetColCount();
			for (int j = 1; j <= nxColCount; j++)
				value[j-1] = x.GetValueRowCol(i, j);
			// String Array에 대해 Validation 검사. (필드, 레코드 전체)
			if (!MakeDataArray(value, aCols, aData))
			{
				x.DestroyWindow();
				if (m_sWarningText.IsEmpty())
					SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_data__Operation_has_been_));
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	
	// 모드에 따라 Append 또는 Modify를 한다.
	if (nPasteMode == TB_PASTE_MODE_APPEND) // DB에 추가된다.
	{
		m_pDoc->m_pDataCtrl->AddLcom(m_nLcomType, aData);
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY)  // DB 수정 없다.
	{
		// 직접 그리드를 수정한다.
		CArray<int, int> aColID;
		aColID.SetSize(nPastedCols);
		for (int j = 0; j < nPastedCols; j++)
			VERIFY(GetColumnIDFor(range.left+j, aColID[j]));
		qsort((void*)aColID.GetData(), aColID.GetSize(), sizeof(UINT), CCompFunc::intAsc);
		ROWCOL nColNo;
		for (int j = 0; j < nPastedCols; j++)
		{
			VERIFY(GetColumnNoFor(aColID[j], nColNo));
			SetExpressionRowCol(range.top, nColNo, value[nColNo-1]);
			OnModifyCell(range.top, nColNo);
		}
	}   
	else if (nPasteMode == TB_PASTE_MODE_MODIFY) // DB가 수정된다.
	{
		// 기존 key를 가져온다.
		CArray<CString, CString&> aOldKey;
		int nxRowCount = x.GetRowCount();
		aOldKey.SetSize(nxRowCount);
		for (int i = 0; i < nxRowCount; i++)
			aOldKey[i] = GetValueRowColBeforeEdit(range.top+i, aCols[1]);
		m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, aOldKey, aData);
	}

	x.DestroyWindow();
	return FALSE;   // 실제 Paste는 하지 않는다.
}

BOOL CCMLoadCombGrid::PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
																				 CGXGridWnd &x, int &nPasteMode, int &nPastedCols)
{
	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	// Paste된 범위를 구한다.
	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();
	

	// 규칙
	// 범위가 다르면 경고 메세지를 보내고 리턴
	// Paste된 내용이 레코드의 모든 필드를 만족하지 못하면 Default 값으로 채운다.
	// column 구분이 없기 때문에 칼럼 by 칼럼으로 Paste한다.
	// 즉 column의 위치가 변경되었으면 변경된 대로 Paste 한다.
	// 고의적으로 Hide된 칼럼은 Default 값으로 채운다.

	// 용어
	// 원본 범위 : Copy해서 Clipboard로 들어간 row, col 갯수
	// 대상 범위 : User가 마우스로 선택해 놓은 범위

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol;
	int nColCount = GetColCount();
	int nPasteColCount = r.GetWidth();
	for( int k=1; k<r.GetWidth(); k++ )
	{
		if (range.left+k < nColCount && (IsHiddenByIntend(range.left+k) || IsColHidden(range.left+k)))
		{
			x.InsertCols(k+1, 1);
			aHideCol.Add(range.left+k);
			//int nID;
			//GetColumnIDFor(range.left+k, nID);
			//for (int m = r.top; m <= r.bottom; m++)
			//  x.SetExpressionRowCol(m, k+1, aDefVal[nID-1]);
			r.right++;
			nPasteColCount++;
		}
	}
	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	int nLeftInsertWidth, nRightInsertWidth;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Append Row를 넘어서면 수정과 추가가 중복되므로 에러
		if (nRow != GetRowCount() && (nRow + r.GetHeight()) > GetRowCount())
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}

		// 편집 모드이고 원본 범위가 2줄 이상이면 에러
		if (IsEdit() && r.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// 현재 셀이 Append Row에 있으면 모두 추가(DB에 추가된다.)
		if (nRow == GetRowCount())
			nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정상태를 유지(DB를 수정하지 않는다.)
		else if (IsEdit()) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 모두 수정(DB에서 수정이 된다.)
		else nPasteMode = TB_PASTE_MODE_MODIFY;
	}
	/*
	// 대상 범위가 원본 범위와 다른 경우
	else if (range.GetWidth() != r.GetWidth() || range.GetHeight() != r.GetHeight())
	{
		// 원본이 1줄이고 대상 범위와 칸수가 같으면 FILL 모드가 된다.
		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND; 
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else 
		{
			nPasteMode = TB_PASTE_MODE_MODIFY;
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			if (range.GetHeight() > 1)
			{
				x.SetRowCount(range.GetHeight());
				CString value;
				for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
				{
					value = x.GetValueRowCol(1, i+1);
					for (int j = 1; j < range.GetHeight(); j++)
						SetExpressionRowCol(j+1, i+1, value);
				}
			}
		}
	}
	*/
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// 편집 모드에서 편집 레코드가 대상 범위에 포함되어 있고 2줄 이상이면 에러 표시
		if (IsEdit() && nRow >= range.top && nRow <= range.bottom && range.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}
		
		// Append Row가 범위에 포함된 경우 다른 Row도 범위에 포함되어 있으면 에러
		if (range.bottom == GetRowCount() && range.GetHeight() > 1)
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_TB_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND; 
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else nPasteMode = TB_PASTE_MODE_MODIFY;

		// Fill Mode인지 검사한다.
		if (bFillMode)
		{
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			x.SetRowCount(range.GetHeight());
			CString value;
			for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
			{
				value = x.GetValueRowCol(1, i+1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j+1, i+1, value);
			}
			r.bottom = x.GetRowCount();
		}
	}

	// 레코드 또는 레코드 셋을 만든다.
	// 왼쪽과 오른쪽에 모자라는 칼럼수를 삽입한다.
	nPastedCols = r.GetWidth();
	nLeftInsertWidth = range.left-1;
	if (nLeftInsertWidth > 0) x.InsertCols(1, nLeftInsertWidth);
	nRightInsertWidth = GetColCount()-x.GetColCount();
	if (nRightInsertWidth > 0) x.InsertCols(x.GetColCount()+1, nRightInsertWidth);
	// 모자라는 값을 카피한다. (Append는 Default값, Modify는 기존 값)
	if (nPasteMode == TB_PASTE_MODE_APPEND)
	{
		int nID;
		CString value;
		// 속도 향상을 위해 column별로 복사
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 Default값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j]-1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID-1]);
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY || nPasteMode == TB_PASTE_MODE_MODIFY)
	{
		CString value;
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 기존의 값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
	}

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		SetWarningText(sErrMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CCMLoadCombGrid::MakeDataArray(CStringArray& value, CRowColArray& awCols, 
																		CArray<T_LCOM_D, T_LCOM_D&>& aData)
{
	T_LCOM_D data;
	data.Initialize();
	if (!ValidateAndMakeRecord(value, awCols, data)) return FALSE;
	aData.Add(data);
	return TRUE;
}

BOOL CCMLoadCombGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_LCOM_D &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	ConvStrArrayToData(value, data);
	return TRUE;
}

BOOL CCMLoadCombGrid::ConvStrArrayToData(CStringArray &aValue, T_LCOM_D& data)
{
	int nColCount = GetColCount();
	data.LoadCombId = _tstol(aValue[0]);
	data.LoadCombName = aValue[1];
	data.nActive = CCMLoadCombMainGrid::GetActive(m_nLcomType, aValue[2]);
	data.LoadCombType = CCMLoadCombMainGrid::GetCombType(aValue[3]);
	data.bElasticStage = aValue[4]==_T("1");
	data.Description = aValue[nColCount-1];

	int nCase = 0;
	double dFactor;
	int nColID;
	CString str;
	for (nColID = 5; nColID < nColCount-1; nColID++)
	{
		str = aValue[nColID];
		str.TrimLeft(); str.TrimRight();
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		dFactor = _tstof(str);
		if (str != _T("") && dFactor != 0.0)
		{
			if (nCase >= D_LCOM_NUMCOMB) 
			{
				CString csMsg;
				csMsg.Format(_LS(IDS_WG_CMD__ADDD__Error___Number_of_load_case_excee_MSG2), D_LCOM_NUMCOMB);
				SetWarningText(csMsg);
				DisplayWarningText();
				return FALSE;
			}
			T_LCOM_BASE LcomBase; LcomBase.Initialize();
			LcomBase.AnalType = m_aColInfo[nColID].nLCtype;
			LcomBase.LoadCaseKey = m_aColInfo[nColID].nKey;
			LcomBase.Factor = dFactor;
			data.aCombination.Add(LcomBase);
			nCase++;
		}
	}
	return TRUE;
}

// CIVIL에서 LC 칼럼의 Enable/Disable을 조정한다.
void CCMLoadCombGrid::SetEnableDisableLcomCol()
{
	int nLcomType = m_nLcomType;
	int nLcomNum = m_pDoc->m_pAttrCtrl->GetCountLcom(nLcomType);
	ROWCOL nRow, nCol;

	int nColNum = m_aColInfo.GetSize();
	int n = 0;
	for (n = 0; n < nColNum; n++)
		if (m_aColInfo[n].nLCtype == m_nLCtype) break;
	if (n == nColNum) nCol = 0;
	else nCol = n+1;

	nRow = 1;
	for (int i = 0; i < nLcomNum; i++)
	{
		int j = 0;
		for (j = 0; j < i; j++) // Enable
		{
			SetStyleRange(CGXRange(nRow, nCol+j), CGXStyle()
					.SetEnabled(TRUE));
		}
		for (; j < nLcomNum; j++) // Disable
		{
			SetStyleRange(CGXRange(nRow, nCol+j), CGXStyle()
					.SetEnabled(FALSE)
					.SetValue(_T("")));
		}
		nRow++;
	}
}


BEGIN_MESSAGE_MAP(CCMLoadCombGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CCMLoadCombGrid)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombGrid message handlers

// 마우스 클릭시 번호 컬럼이 헤더 컬럼처럼 동작하게 한다.
void CCMLoadCombGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	ROWCOL nhRow, nhCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		if (IsFrozenCol(nhCol)) // key column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}

	CTBBrowserWnd::OnLButtonDown(nFlags, point);
}

BOOL CCMLoadCombGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	// 여기서는 Default 값을 채워 넣는 역할을 하면 된다.
	// 새 레코드가 입력되는 순간에 번호 자동 입력(DB에서 가져온다.)
	BOOL bActive;
	ROWCOL nCol, ncRow, ncCol;
	CString value;

	// 현재 셀이 Active되어 있으면 값을 채워 넣지 않는다.
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl && pControl->IsActive()) bActive = TRUE;
	else bActive = FALSE;
	if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckBox))) bActive = TRUE;
	else if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckListComboBox))) bActive = TRUE;

	int nColCount = m_nFixedCol;
	for (int i = 0; i < nColCount; i++)
	{
		VERIFY(GetColumnNoFor(i+1, nCol));
		value = GetDefValue(i);
		if (!value.IsEmpty() && (nCol != ncCol || !bActive)) 
			SetExpressionRowCol(nRow, nCol, value);
	}

	return TRUE;
}

CString CCMLoadCombGrid::GetDefValue(int ix)
{
	if (ix == 0) return _T("");
	if (ix == 1) return CCMLoadCombMainGrid::GetLcomActiveDef(m_nLcomType);
	if (ix == 2) return D_LCOM_CT_ADD;
	if (ix == 4) return _T("0");
	return _T("");
	int nColCount = GetColCount();
	CString* aDefVal = new CString[nColCount-1];  // key는 뺀다.
	aDefVal[0] = _T("");  // name
	aDefVal[1] = CCMLoadCombMainGrid::GetLcomActiveDef(m_nLcomType);
	//aDefVal[2] = _LS(IDS_WG_CMD__ADDD__Linear);  // type
	aDefVal[2] = D_LCOM_CT_ADD;
	for (int i = 3; i < nColCount-1; i++) aDefVal[ix] = _T("");
}

BOOL CCMLoadCombGrid::IsDisplayMode()
{
	if (m_pParentDlg == 0) { ASSERT(0); return FALSE; }
	return m_pParentDlg->IsSpreadSheetStyle();
}

// HideColsByIntend로 인해 숨겨진 칼럼은 복사, Paste에서 빼야 한다.
BOOL CCMLoadCombGrid::Copy()
{
	// Gen Trial은 Copy 기능 막는다.
#if defined(_MGEN) && defined(_TRIAL)
	AfxMessageBox(_LS(IDS_TB_CANT_COPY_IN_TRIAL));
	return FALSE;
#endif

	// 원래 Range를 저장하고 의도적으로 숨겨진 Column은 뺀다.
	CGXRangeList OrgRange;
	CGXRangeList* pRangeList = GetParam()->GetRangeList();
	OrgRange = *pRangeList;
	int nColCnt = GetColCount();
	SelectRange(CGXRange().SetCols(1), FALSE, FALSE);
	for( int i=2; i<=nColCnt; i++ )
	{
		if( IsHiddenByIntend(i) || IsColHidden(i) )
			SelectRange(CGXRange().SetCols(i), FALSE, FALSE);
	}
	// 복사하고
	BOOL bResult = CTBBrowserWnd::Copy();
	// 원래 RangeList를 복구한다.
	*pRangeList = OrgRange;
	return bResult;
}
