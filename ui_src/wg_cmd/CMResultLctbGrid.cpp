// CMResultLctbGrid.cpp : implementation file
//
// 2004. 02. 13   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMResultLctbGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMLoadCombMainGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

union D_LCTB_LCOM
{
	unsigned int keymap;
	struct
	{
		unsigned int stld : 16;   // 0-65535
		unsigned int type : 16;   // 0-65535
	}key;
};

/////////////////////////////////////////////////////////////////////////////
// CCMResultLctbGrid

CCMResultLctbGrid::CCMResultLctbGrid()
{
	m_pDoc = 0;

	m_nFixedCol = 0;
	m_nStldCount = 0;
	m_nSplcCount = 0;
	m_nThisCount = 0;
	m_nMvldCount = 0;
	m_nSmlcCount = 0;
	m_nSgldCount = 0;
	m_nEspcCount = 0;
	m_nNspcCount = 0;
	m_nNespCount = 0;
}

CCMResultLctbGrid::~CCMResultLctbGrid()
{
}

//--------------------------------------------------------------------------
// Interface
void CCMResultLctbGrid::Initialize(CDBDoc* pDoc)
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

void CCMResultLctbGrid::MakeItemEx()
{
	SetModeDisplayOnly(FALSE);

	CWaitCursor wait;

	BOOL bOldLock = LockUpdate(TRUE);

	CArray<T_LCTB_K, T_LCTB_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetLctbKeyList(aKeyList);
	int nRecCount = aKeyList.GetSize();
	SetRowCount(0);
	SetRecordCount(nRecCount);
	
	T_LCTB_D data;
	for (int i = 0; i < nRecCount; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLctb(aKeyList[i], data);
		SetValue((ROWCOL)(i+1), aKeyList[i], data);
	}

	SetRightValidCell(1, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetModeDisplayOnly(TRUE);
}

void CCMResultLctbGrid::UpdateBuffer()
{
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

	BOOL bChagned = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		// 현재는 삭제 기능만 가능 
		switch(nCmd)
		{
		case (UR_LCTB_ADD):
		case (UR_LCTB_MFS):
		case (UR_LCTB_MFD):
		case (UR_LCTB_DEL):
			bChagned = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if (bChagned) MakeItemEx();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

//--------------------------------------------------------------------------
// Implementation
void CCMResultLctbGrid::SetColInfo()
{
	// 먼저 컬럼의 최대공약수를 구해야 한다. 

	CString item;
	int nCount;

	m_aColInfo.RemoveAll();
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__No), 0, 1)); 
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__Name), 0, 2)); 
	m_nFixedCol = 2;

	// Make LCOM key map that LCTB have.
	T_LCTB_K LctbK;
	T_LCTB_D LctbD;
	D_LCTB_LCOM LcomK;
	unsigned int nLcomCount;
	CMap<unsigned int, unsigned int, unsigned int, unsigned int> m_aLcomMapper;
	m_aLcomMapper.InitHashTable(31);
	m_aLcomMapper.RemoveAll();

	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartLctb();
	while (pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextLctb(pos, LctbK, LctbD);
		for (int i=0; i<LctbD.aLctbBase.GetSize(); i++)
		{
			LcomK.key.stld = LctbD.aLctbBase[i].LoadCaseKey;
			LcomK.key.type = LctbD.aLctbBase[i].iAnalType;
			if (m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) 
				m_aLcomMapper.SetAt(LcomK.keymap, ++nLcomCount);
			else 
			{
				nLcomCount = 1;
				m_aLcomMapper.SetAt(LcomK.keymap, nLcomCount);
			}
		}
	}

	//D_LCOM_STATIC		1 : 정적 하중 조건
	T_STLD_K Key;
	T_STLD_D Data;
	CArray<T_STLD_K, T_STLD_K> aStldKey;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldKey);
	LcomK.key.type = D_LCOM_STATIC;
	nCount = aStldKey.GetSize();
	m_nStldCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		Key = aStldKey[i];
		LcomK.key.stld = Key;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetStld(Key, Data);
		if (Data.LoadCaseType == _T("CS") || Data.LoadCaseType == _T("TCS")) continue;  // 시공타입은 제외
		item.Format(_T("%s(ST)"), Data.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_STATIC, Key));
		m_nStldCount++;
	}

	//D_LCOM_SPECTRUM		2 : 응답 스펙트럼 하중 조건
	T_SPLC_K SplcKey;
	T_SPLC_D SplcData;
	CArray<T_SPLC_K, T_SPLC_K> aSplcKey;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcKey);
	LcomK.key.type = D_LCOM_SPECTRUM;
	nCount = aSplcKey.GetSize();
	m_nSplcCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		SplcKey = aSplcKey[i];
		LcomK.key.stld = SplcKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetSplc(SplcKey, SplcData);
		item.Format(_T("%s(RS)"), SplcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_SPECTRUM, SplcKey));
		m_nSplcCount++;
	}

	//D_LCOM_SPECTRUM_ND  : ND 응답 스펙트럼 하중 조건
	T_NSPC_K NspcKey;
	T_NSPC_D NspcData;
	CArray<T_NSPC_K, T_NSPC_K> aNspcKey;
	LcomK.key.type = D_LCOM_SPECTRUM_ND;
	m_pDoc->m_pAttrCtrl->GetNspcKeyList(aNspcKey);
	nCount = aNspcKey.GetSize();
	m_nNspcCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		NspcKey = aNspcKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetSplc(NspcKey, SplcData)) {ASSERT(0); continue;}
		if(!SplcData.bNonDissipative) continue;

		LcomK.key.stld = NspcKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetNspc(NspcKey, NspcData);
		item.Format(_T("%s(NRS)"), NspcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_SPECTRUM_ND, NspcKey));
		m_nNspcCount++;
	}

	//D_LCOM_HISTORY		3 : 시간 이력 하중 조건
	T_THIS_K ThisKey;
	T_THIS_D ThisData;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	m_pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey, FALSE);
	LcomK.key.type = D_LCOM_HISTORY;
	nCount = aThisKey.GetSize();
	m_nThisCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		ThisKey = aThisKey[i];
		LcomK.key.stld = ThisKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetThis(ThisKey, ThisData);
		item.Format(_T("%s(TH)"), ThisData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_HISTORY, ThisKey));
		m_nThisCount++;
	}
	
	//D_LCOM_MOVING		  4 : 이동 하중 조건
	// 코드에 따른 방식으로 수정 
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	LcomK.key.type = D_LCOM_MOVING;
	m_nMvldCount = 0;

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
				LcomK.key.stld = MvldKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvld(MvldKey, MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldKey));
				m_nMvldCount++;
			}
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
				LcomK.key.stld = MvldchKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvldch(MvldchKey, MvldchData);
				item.Format(_T("%s(MV)"), MvldchData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldchKey));
				m_nMvldCount++;
			}
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
				LcomK.key.stld = MvldjpKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvldjp(MvldjpKey, MvldjpData);
				item.Format(_T("%s(MV)"), MvldjpData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldjpKey));
				m_nMvldCount++;
			}
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
				LcomK.key.stld = MvldidKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvldid(MvldidKey, MvldidData);
				item.Format(_T("%s(MV)"), MvldidData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldidKey));
				m_nMvldCount++;
			}
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
				LcomK.key.stld = MvldbsKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvldbs(MvldbsKey, MvldbsData);
				item.Format(_T("%s(MV)"), MvldbsData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldbsKey));
				m_nMvldCount++;
			}
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
				LcomK.key.stld = MvldfrKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvldfr(MvldfrKey, MvldfrData);
				item.Format(_T("%s(MV)"), MvldfrData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldfrKey));
				m_nMvldCount++;
			}
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
				LcomK.key.stld = MvldKey;
				if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
				m_pDoc->m_pAttrCtrl->GetMvldtr(MvldKey, MvldData);
				item.Format(_T("%s(MV)"), MvldData.LoadCaseName);
				m_aColInfo.Add(SLcomColItem(item, D_LCOM_MOVING, MvldKey));
				m_nMvldCount++;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	//D_LCOM_SETTLEMENT	5 : 지점침하 하중 조건
	T_SMLC_K SmlcKey;
	T_SMLC_D SmlcData;
	CArray<T_SMLC_K, T_SMLC_K> aSmlcKey;
	m_pDoc->m_pAttrCtrl->GetSmlcKeyList(aSmlcKey);
	LcomK.key.type = D_LCOM_SETTLEMENT;
	nCount = aSmlcKey.GetSize();
	m_nSmlcCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		SmlcKey = aSmlcKey[i];
		LcomK.key.stld = SmlcKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetSmlc(SmlcKey, SmlcData);
		item.Format(_T("%s(SM)"), SmlcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_SETTLEMENT, SmlcKey));
		m_nSmlcCount++;
	}

	//D_LCOM_STAGE 13: 시공해석 하중 조건
	T_SGLD_K SgldKey;
	T_SGLD_D SgldData;
	CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
	m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
	LcomK.key.type = D_LCOM_STAGE;
	nCount = aSgldKey.GetSize();
	m_nSgldCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		SgldKey = aSgldKey[i];

		LcomK.key.stld = SgldKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData);
		item.Format(_T("%s(CS)"), SgldData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_STAGE, SgldKey));
		m_nSgldCount++;
	}

	//D_LCOM_ECCENSPEC  : 우발편심 응답 스펙트럼 하중 조건
	T_ESPC_K EspcKey;
	T_ESPC_D EspcData;
	CArray<T_ESPC_K, T_ESPC_K> aEspcKey;
	LcomK.key.type = D_LCOM_ECCENSPEC;
	m_pDoc->m_pAttrCtrl->GetEspcKeyList(aEspcKey);
	nCount = aEspcKey.GetSize();
	m_nEspcCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		EspcKey = aEspcKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetSplc(EspcKey, SplcData)) {ASSERT(0); continue;}
		if(!SplcData.bConsiderEccen) continue;

		LcomK.key.stld = EspcKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetEspc(EspcKey, EspcData);
		item.Format(_T("%s(ES)"), EspcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_ECCENSPEC, EspcKey));
		m_nEspcCount++;
	}

	//D_LCOM_ECCENSPEC_ND  : ND 우발편심 응답 스펙트럼 하중 조건
	LcomK.key.type = D_LCOM_ECCENSPEC_ND;
	m_pDoc->m_pAttrCtrl->GetNspcKeyList(aNspcKey);
	nCount = aNspcKey.GetSize();
	m_nNespCount = 0;
	for (int i = 0; i < nCount; i++)
	{
		NspcKey = aNspcKey[i];
		if(!m_pDoc->m_pAttrCtrl->GetSplc(NspcKey, SplcData)) {ASSERT(0); continue;}
		if(!SplcData.bConsiderEccen) continue;
		if(!SplcData.bNonDissipative) continue;

		LcomK.key.stld = NspcKey;
		if (!m_aLcomMapper.Lookup(LcomK.keymap, nLcomCount)) continue;
		m_pDoc->m_pAttrCtrl->GetNspc(NspcKey, NspcData);
		item.Format(_T("%s(NES)"), NspcData.LoadCaseName);
		m_aColInfo.Add(SLcomColItem(item, D_LCOM_ECCENSPEC_ND, NspcKey));
		m_nNespCount++;
	}

	// Description column
	m_aColInfo.Add(SLcomColItem(_LS(IDS_WG_CMD__ADDD__Description), 0, 3));

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
			);
			//.SetEnabled(FALSE));
	HideCols(1, 1, TRUE);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING)
			//.SetEnabled(FALSE)
			);

	SetStyleRange(CGXRange().SetCols(3, nColNo-1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4)
			//.SetEnabled(FALSE)
			);

	SetStyleRange(CGXRange().SetCols(nColNo), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING)
			//.SetEnabled(FALSE)
			);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetColWidthByDPI(1, 1,  0);  // no
	SetColWidthByDPI(2, 2, 58);  // name
	SetColWidthByDPI(nColNo, nColNo, 200); // description
	if (nColNo > 3) ResizeColWidthsToFit(CGXRange().SetCols(3, nColNo-1));

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMResultLctbGrid::SetValue(ROWCOL nRow, T_LCTB_K key, T_LCTB_D &data)
{
	CStringArray aValue;
	ConvDataToStrArray(data, aValue);
	int nSize = aValue.GetSize();
	for (int i = 0; i < nSize; i++)
		SetExpressionRowCol(nRow, i+1, aValue[i]);

	// Key 설정
	SetStyleRange(CGXRange(nRow, 0), 
								CGXStyle().SetUserAttribute(ID_USER_ATTR_KEY, (long)key));
}

CString CCMResultLctbGrid::GetKeyString(ROWCOL nRow)
{
	CString csKey;
	CGXStyle style;
	GetStyleRowCol(nRow, 0, style);
	style.GetUserAttribute(ID_USER_ATTR_KEY, csKey);
	return csKey;
}

void CCMResultLctbGrid::SetHeaderTitle()
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

void CCMResultLctbGrid::ConvDataToStrArray(T_LCTB_D &data, CStringArray &aValue)
{
	int nSize = m_aColInfo.GetSize();
	aValue.SetSize(nSize);
	aValue[0].Format(_T("%d"), data.LoadContributionId);
	aValue[1] = data.strLctbName;
	aValue[nSize-1] = data.Description;

	for (int i = 0; i < data.aLctbBase.GetSize(); i++)
	{
		SetLcomCaseValue(aValue, data.aLctbBase[i]);
	}
}

void CCMResultLctbGrid::SetLcomCaseValue(CStringArray &aValue, T_LCTB_BASE &base)
{
	int nSize = m_aColInfo.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++)
	{
		if (m_aColInfo[i].nLCtype == base.iAnalType && m_aColInfo[i].nKey == base.LoadCaseKey) break;
	}
	if (i == nSize)
	{
		//ASSERT(0);  // not found -> ERROR
		return;
	}
	aValue[i].Format(_T("%.18f"), base.dFactor);
}

BOOL CCMResultLctbGrid::InsertRecordByKey(T_LCTB_K Key, T_LCTB_D &Data)
{
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL i = 0;
	for (i = 0; i < nRowCount; i++)
		if (_tstol(GetValueRowCol(i+1, 1)) > Data.LoadContributionId) break;

	InsertRecord(i+1);
	SetValue(i+1, Key, Data);

	return TRUE;
}

BOOL CCMResultLctbGrid::DeleteRecordByKey(T_LCTB_K Key, T_LCTB_D &Data)
{
	ROWCOL ncRow, ncCol;
	BOOL bCurCell = GetCurrentCell(ncRow, ncCol);
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	for (ROWCOL i = 0; i < nRowCount; i++)
	{
		if (_tstol(GetValueRowCol(i+1, 1)) == Data.LoadContributionId)
		{
			if (bCurCell && i+1 == ncRow && m_nEditMode == edit) CancelEdit();
			DeleteRecord(i+1); 
			break;
		}
	}
	return TRUE;
}

//--------------------------------------------------------------------------
// Overriadable
// Return은 항상 SetFieldValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMResultLctbGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	return TRUE;
	/*
	CString value;
	int nColID;
	VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nColID)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
	*/
}

// Return은 항상 SetRecordValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMResultLctbGrid::OnValidateRecord(ROWCOL nRow)
{
	return TRUE;
	/*
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
	*/
}

BOOL CCMResultLctbGrid::ValidateField(CString& value, int nColID)
{
	return TRUE;
	/*
	ROWCOL nColCount = GetColCount();
	if (nColID > 3 && nColID < nColCount-1)
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
	*/
}

BOOL CCMResultLctbGrid::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
	return TRUE;
	/*
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
	for (int i = 4; i < nColCount-1; i++) // load case factor field
	{
		nPos = aCols[i]-1;
		if (!ValidateField(aValue[nPos], i)) return FALSE;
	}
	return TRUE;
	*/
}

BOOL CCMResultLctbGrid::OnAddNewRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
	/*
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
	*/
}

BOOL CCMResultLctbGrid::OnModifyRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
	/*
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
	*/
}

BOOL CCMResultLctbGrid::OnDeleteRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
	/*
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	CString csName;
	csName = GetValueRowCol(nRow, 2);
	if (m_nEditMode == addnew) return CancelEdit(); // addnew 상태이면 그냥 취소
	if (!m_pDoc->m_pDataCtrl->DelLctb(csName)) return FALSE;

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);

	return TRUE;
	*/
}

int CCMResultLctbGrid::GetSelectedNameList(CArray<CString, CString&> &raName)
{
	raName.RemoveAll();
	
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	int nSize = awRows.GetSize();

	if (nSize > 0)
	{
		// Append Row는 뺀다.
		if (IsAppendRow(awRows.GetAt(nSize-1))) { nSize--; awRows.RemoveAt(nSize); }
		// Title Row도 뺀다.
		if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
		if (nSize == 0) return TRUE;

		raName.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
			raName[i] = GetValueRowCol(awRows[i], 2);
	}

	return raName.GetSize();
}


BOOL CCMResultLctbGrid::OnDeleteRecords(CRowColArray &awRows)
{
	ASSERT(0);
	return TRUE;

	/*
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
	if (!m_pDoc->m_pDataCtrl->DelLctb(aName)) return FALSE;
	if (bNewRecordIncluded) VERIFY(CancelEdit());

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);

	return TRUE;
	*/
}

//--------------------------------------------------------------------------
// Grid's Standard Overridable Functions
// Record선택시 title row와 append row는 제거한다.
/*
void CCMResultLctbGrid::OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey)
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
BOOL CCMResultLctbGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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
BOOL CCMResultLctbGrid::OnPasteFromClipboard(const CGXRange &range)
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
		for (i = 1; i <= nxRowCount; i++)
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
		for (j = 0; j < nPastedCols; j++)
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

BOOL CCMResultLctbGrid::PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
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
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left+k < nColCount && IsHiddenByIntend(range.left+k))
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
		for (j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j]-1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID-1]);
		}
		// 오른쪽 칼럼 카피
		for (j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
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
		for (j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
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
*/


BEGIN_MESSAGE_MAP(CCMResultLctbGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CCMResultLctbGrid)
	//ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMResultLctbGrid message handlers

// 마우스 클릭시 번호 컬럼이 헤더 컬럼처럼 동작하게 한다.
/*
void CCMResultLctbGrid::OnLButtonDown(UINT nFlags, CPoint point)
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
*/

BOOL CCMResultLctbGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
	/*
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
	*/
}

