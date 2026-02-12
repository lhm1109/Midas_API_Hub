// BndrStoryGroupGrid.cpp : implementation file
//

#include "stdafx.h"
#include "BndrStoryGroupGrid.h"


#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_StagCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrStoryGroupGrid

CBndrStoryGroupGrid::CBndrStoryGroupGrid()
{
	m_pDoc = 0;
}

CBndrStoryGroupGrid::~CBndrStoryGroupGrid()
{
}


BEGIN_MESSAGE_MAP(CBndrStoryGroupGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CBndrStoryGroupGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBndrStoryGroupGrid message handlers

void CBndrStoryGroupGrid::Initialize()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	GetParam()->EnableUndo(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);
		
	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	GetParam()->EnableTrackRowHeight(FALSE);  
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);


	LockUpdate(TRUE);

	SetColCount(2);
	SetRowCount(0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(FALSE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING)
			.SetFormat(GX_FMT_TEXT));
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetEnabled(FALSE));
	SetValueRange(CGXRange(0, 1), _LS(IDS_TM_Story));
	SetValueRange(CGXRange(0, 2), _LS(IDS_TM_Group));
	//SetRowHeight(0, 0, globalUtils.ScaleByDPI(36));
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	HideCols(0, 0, TRUE); // Number Column 숨김

	SetColWidth(1, 1, globalUtils.ScaleByDPI(78));
	SetColWidth(2, 2, globalUtils.ScaleByDPI(100));

	ShowAllData();

	LockUpdate(FALSE);
}

void CBndrStoryGroupGrid::ShowAllData()
{
	BOOL bOldLock = LockUpdate(TRUE);

	SetBngrColumn(2);
	int nStor = m_pDoc->m_pAttrCtrl->GetCountStor();
	
	int nRowCount = GetRowCount();
	if (nRowCount > 0) RemoveRows(1, nRowCount);
	SetRowCount(nStor);
	
	BOOL bStageMode;
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0) bStageMode = TRUE;
	else bStageMode = FALSE;

	CArray<T_STOR_K, T_STOR_K> aStorKey;
	T_STOR_D Stor;
	T_STDG_D Stdg;
	T_BNGR_D Bngr;

	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKey);
	for (int i = 0; i < nStor; i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(aStorKey[i], Stor);
		if (!m_pDoc->m_pAttrCtrl->GetStdgFromBaseDB(aStorKey[i], Stdg))
			Bngr.GroupName = _LS(IDS_TM_BNGR_DEFAULT);
		else 
		{
			m_pDoc->m_pAttrCtrl->GetBngr(Stdg.GroupKey, Bngr);
			if (bStageMode && !m_pDoc->m_pStagCtrl->IsCurActiveBngrKey(Stdg.GroupKey))
				SetStyleRange(CGXRange(1+i, 2), CGXStyle()
					.SetEnabled(FALSE).SetControl(GX_IDS_CTRL_STATIC));
		}

		SetValueRange(CGXRange(1+i, 1), Stor.StoryName);
		SetValueRange(CGXRange(1+i, 2), Bngr.GroupName);
	}
	if (nStor > 0) SetCurrentCell(1, 2);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CBndrStoryGroupGrid::SetBngrColumn(ROWCOL nCol)
{
	CString szChoiceList, tmp;
	CArray<CString, CString&> aBngrNameList;
	m_pDoc->m_pAttrCtrl->GetBngrList(aBngrNameList);

	BOOL bStageMode;
	if (m_pDoc->m_pStagCtrl->GetCurStag() != 0) bStageMode = TRUE;
	else bStageMode = FALSE;

	szChoiceList.Format(_T("%s\n"), _LS(IDS_TM_BNGR_DEFAULT));
	int nSize = aBngrNameList.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (bStageMode) 
		{
			T_BNGR_K BngrKey = m_pDoc->m_pAttrCtrl->GetBngrKey(aBngrNameList[i]);
			if (!m_pDoc->m_pStagCtrl->IsCurActiveBngrKey(BngrKey)) continue;
		}
		tmp.Format(_T("%s\n"), aBngrNameList[i]);
		szChoiceList += tmp;
	}

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncCol == nCol) CGXGridWnd::SetCurrentCell(ncRow, ncCol);
}

BOOL CBndrStoryGroupGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bCurrentRow = (nRow == ncRow);
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(CTBGrid::m_Color.NormalModeBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
	else if (bCurrentRow) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentRowBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentRowFg);
	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

BOOL CBndrStoryGroupGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CGXGridWnd::OnEndEditing(nRow, nCol);
	if (nCol != 2) return TRUE;

	CString csGroupNew;

	CGXControl *pControl;
	pControl = GetControl(nRow, nCol);
	if (!pControl->GetModify()) return TRUE;
	if (pControl->IsActive()) pControl->GetValue(csGroupNew);
	else csGroupNew = GetValueRowCol(nRow, nCol);

	T_STDG_K Key;
	T_STDG_D Data;
	T_STDG_D DataOld;

	CString csStoryName = GetValueRowCol(nRow, 1);
	Key = m_pDoc->m_pAttrCtrl->GetStorKey(csStoryName);;
	Data.GroupKey = m_pDoc->m_pAttrCtrl->GetBngrKey(csGroupNew);
	if (Data.GroupKey == 0 && csGroupNew != _LS(IDS_TM_BNGR_DEFAULT))
		return TRUE;
	if (m_pDoc->m_pAttrCtrl->GetStdg(Key, DataOld))
	{
		if (DataOld.GroupKey == Data.GroupKey) return TRUE;
	}
	else
	{
		if (Data.GroupKey == 0) return TRUE;
	}

	if (!m_pDoc->m_pDataCtrl->AddStdg(Key, Data))
		return FALSE;
	return TRUE;
}

BOOL CBndrStoryGroupGrid::OnPasteFromClipboard(const CGXRange &range)
{
	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CString csErrMsg;
	CArray<T_STDG_K, T_STDG_K> aKey;
	CArray<T_STDG_D, T_STDG_D&> aData;
	CRowColArray awRows;
	int nTotalRow;
	T_STDG_K Key;
	T_STDG_D Data;
	CString csStoryName;
	CString csGroup;

	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	CGXGridWnd x;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), this, 0);
	x.Initialize();
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();
	int nPastedRows = r.bottom - r.top + 1;
	int nPastedCols = r.right - r.left + 1;
	if (nPastedCols > 1) 
	{
		csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG1);
		goto PASTE_ERROR;
	}

	nTotalRow = GetRowCount();

	GetSelectedRows(awRows);
	if (awRows.GetSize() > 1) // Select된 것이 2줄 이상
	{
		if (nPastedRows != 1)
		{
			if (awRows.GetSize() != nPastedRows)
			{
				csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG2);
				goto PASTE_ERROR;
			}
		}
		csGroup = x.GetValueRowCol(1, 1);
		aKey.SetSize(awRows.GetSize());
		aData.SetSize(awRows.GetSize());
		for (int i = 0; i < awRows.GetSize(); i++)
		{
			csStoryName = GetValueRowCol(awRows.GetAt(i), 1);
			if (nPastedRows > 1) csGroup = x.GetValueRowCol(1+i, 1);
			Key = m_pDoc->m_pAttrCtrl->GetStorKey(csStoryName);
			Data.Initialize();
			Data.GroupKey = m_pDoc->m_pAttrCtrl->GetBngrKey(csGroup);
			if (Data.GroupKey == 0 && csGroup.CompareNoCase(_LS(IDS_TM_BNGR_DEFAULT)) != 0)
			{
				csErrMsg.Format(_LS(IDS_TM_STDG_GRID_ERR_MSG3), csGroup);
				goto PASTE_ERROR;
			}
			aKey[i] = Key;
			aData[i] = Data;
		}
	}
	else  // Select된 것이 1줄
	{
		int nTgtRow;
		if (awRows.GetSize() == 0)
		{
			ROWCOL ncRow, ncCol;
			if (!GetCurrentCell(ncRow, ncCol))
			{
				csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG4);
				goto PASTE_ERROR;
			}
			nTgtRow = ncRow;
		}
		else nTgtRow = awRows[0];
		if (nTgtRow + nPastedRows - 1 > nTotalRow)
		{
			csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}

		aKey.SetSize(nPastedRows);
		aData.SetSize(nPastedRows);
		for (int i = 0; i < nPastedRows; i++)
		{
			csStoryName = GetValueRowCol(nTgtRow+i, 1);
			csGroup = x.GetValueRowCol(1+i, 1);
			Key = m_pDoc->m_pAttrCtrl->GetStorKey(csStoryName);
			Data.Initialize();
			Data.GroupKey = m_pDoc->m_pAttrCtrl->GetBngrKey(csGroup);
			if (Data.GroupKey == 0 && csGroup.CompareNoCase(_LS(IDS_TM_BNGR_DEFAULT)) != 0)
			{
				csErrMsg.Format(_LS(IDS_TM_STDG_GRID_ERR_MSG3), csGroup);
				goto PASTE_ERROR;
			}
			aKey[i] = Key;
			aData[i] = Data;
		}
	}
	
	x.DestroyWindow();
	m_pDoc->m_pDataCtrl->AddStdg(aKey, aData);
	return FALSE;   // 실제 Paste는 하지 않는다.

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}

void CBndrStoryGroupGrid::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (m_pDoc == 0) return;
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		ShowAllData();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_STAGE_CHANGED:
		ShowAllData();
		break;
	case D_UPDATE_POST_STAGE_CHANGED:
		ShowAllData();
		break;
	case D_UPDATE_STAGE_DBALL:
		ShowAllData();
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CBndrStoryGroupGrid::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bBngrChanged = FALSE;
	BOOL bStorChanged = FALSE;

	// Group의 변화를 먼저 조사
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		
		switch(nCmd)
		{
		case(UR_BNGR_ADD): 
		case(UR_BNGR_DEL): 
		case(UR_BNGR_MFS): 
		case(UR_BNGR_MFD):
				bBngrChanged = TRUE;
				break;
		case(UR_STOR_ADD):
		case(UR_STOR_DEL): 
		case(UR_STOR_MFS): 
		case(UR_STOR_MFD):
				bStorChanged = TRUE;
				break;
		}
	}
	if (bBngrChanged || bStorChanged) 
	{
		ShowAllData();
		return;
	}

	BOOL bOldLock = LockUpdate(TRUE);
	pos = pViewBuff->GetStartBuffer();

	T_STDG_K key; 
	T_STDG_D data;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STDG_ADD):
				pViewBuff->GetStdg(nKey, key, data);
				AddStdg(key, data);
				break;
		case(UR_STDG_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetStdg(nKey, key, data);
				DelStdg(key);
				break;
		default:
			break;
		}
	} // end of while

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

int CBndrStoryGroupGrid::FindPos(T_STDG_K key)
{
	CArray<T_STOR_K, T_STOR_K> aStorKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKey);

	int nCount = aStorKey.GetSize();
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (aStorKey[i] == key) break;
	if (i == nCount) return -1;
	return i;
}

void CBndrStoryGroupGrid::AddStdg(T_STDG_K key, T_STDG_D& rData)
{
	int nPos = FindPos(key);
	if (nPos < 0) return;

	T_BNGR_D Bngr;
	if (rData.GroupKey == 0)
		Bngr.GroupName = _LS(IDS_TM_BNGR_DEFAULT);
	else 
		m_pDoc->m_pAttrCtrl->GetBngr(rData.GroupKey, Bngr);

	SetValueRange(CGXRange(1+nPos, 2), Bngr.GroupName);
}

void CBndrStoryGroupGrid::DelStdg(T_STDG_K key)
{
	int nPos = FindPos(key);
	if (nPos < 0) return;

	SetValueRange(CGXRange(1+nPos, 2), _LS(IDS_TM_BNGR_DEFAULT));
}

