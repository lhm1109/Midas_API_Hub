#include "StdAfx.h"
#include "wg_dgn.h"

#include "../wg_db/DBDoc.h"
#include "../wg_db/DBLib.h"
#include "../wg_db/UnitCtrl.h"

#include "DgnConBentUpInfoGrid.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnConBentUpInfoGrid
// 
CDgnConBentUpInfoGrid::CDgnConBentUpInfoGrid(CWnd* pParent) : CTBBrowserWnd()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pParent = pParent;

	m_bCanDeleteRowByPressingKey = TRUE;
}

CDgnConBentUpInfoGrid::~CDgnConBentUpInfoGrid()
{

}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CDgnConBentUpInfoGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnConBentUpInfoGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnConBentUpInfoGrid message handlers

void CDgnConBentUpInfoGrid::Initialize()
{
	CGXGridWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	// GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	GetParam()->EnableUndo(FALSE);                                // Undo / Redo 금지
	GetParam()->EnableMoveRows(FALSE);                            // Row 이동 금지
	GetParam()->EnableMoveCols(FALSE);                            // Col 이동 금지
	GetParam()->EnableSelection(GX_SELFULL);                       // Row Selection
	GetParam()->EnableTrackRowHeight(GX_TRACK_NOTHEADER);         // Row Size 변경금지
	//GetParam()->EnableTrackColWidth(GX_TRACK_ALL);
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	
	EnableInsertRowByPressingKey(TRUE);
	EnableAppendRow(TRUE);
	EnableDeleteRowByPressingKey(TRUE);

	SetDrawingTechnique(gxDrawUsingMemDC);                        // MemDC 사용

	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, (CGXStyle)pProp->sInvertNoBorder);

	// Grid Num
	SetColCount(D_COL_GRP_GAP);
	SetRowCount(2);

	ChangeStandardStyle(CGXStyle().SetFont(CGXFont_GC().SetSize(9).SetBold(FALSE)).SetVerticalAlignment(DT_BOTTOM).SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	
	auto L_SetColWidth = [this] (ROWCOL nFromCol, ROWCOL nToCol, int nWidth)
	{
		SetColWidth(nFromCol, nToCol, globalUtils.ScaleByDPI(nWidth));
	};

	// Grid Size
	//SetRowHeight(0, 2, 20);
	L_SetColWidth(0,							0,							18);
	L_SetColWidth(D_COL_NUM,			D_COL_NUM,			37);
	L_SetColWidth(D_COL_SIZE,			D_COL_SIZE,			45);
	L_SetColWidth(D_COL_DB,				D_COL_DB,				35);
	L_SetColWidth(D_COL_XI,				D_COL_XI,				40);
	L_SetColWidth(D_COL_ANGLE,		D_COL_ANGLE,		40);
	L_SetColWidth(D_COL_HI,				D_COL_HI,				39);
	L_SetColWidth(D_COL_GRP_APP,	D_COL_GRP_APP,	37);
	L_SetColWidth(D_COL_GRP_NUM,	D_COL_GRP_NUM,	38);
	L_SetColWidth(D_COL_GRP_GAP,	D_COL_GRP_GAP,	39);

	// Set Title
	SetHeaderTitle();

	// Set Style
	SetStyleRange(CGXRange().SetCols(D_COL_NUM),  CGXStyle().SetHorizontalAlignment(DT_RIGHT).SetValueType(GX_VT_NUMERIC));

	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE);	
	CString csChoiceList = _T("");
	int nSize = aRBName.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		csChoiceList += aRBName[i]+_T("\n");
	}
	SetStyleRange(CGXRange().SetCols(D_COL_SIZE), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T("")));
	
	SetStyleRange(CGXRange().SetCols(D_COL_DB, D_COL_HI), CGXStyle().SetHorizontalAlignment(DT_RIGHT).SetValueType(GX_VT_NUMERIC));
	
	SetStyleRange(CGXRange().SetCols(D_COL_GRP_APP), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("0")));

	SetStyleRange(CGXRange().SetCols(D_COL_GRP_NUM, D_COL_GRP_GAP), CGXStyle().SetHorizontalAlignment(DT_RIGHT).SetValueType(GX_VT_NUMERIC));

	SetStyleRange(CGXRange().SetRows(1), CGXStyle().SetIncludeDraw3dFrame(TRUE).SetHorizontalAlignment(DT_CENTER).SetValueType(GX_VT_STRING)
		.SetFormat(GX_FMT_FIXED).SetPlaces(0).SetEnabled(FALSE).SetDraw3dFrame(gxFrameRaised).SetIncludeInterior(TRUE).SetInterior(RGB(255, 0, 0)));

	SetFrozenRows(1, 1);

	SetCurrentCell(0, 0);	

	SetExpressionRowCol(1, 0, _T("1"));

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetScrollBarMode(SB_HORZ, gxnAutomatic | gxnEnhanced);
	
	LockUpdate(FALSE);
	Redraw();
}

BOOL CDgnConBentUpInfoGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CTBBrowserWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	if(nRow == 1)
	{
		style.SetInterior(CTBGrid::m_Color.RowColHeaderBg );
	}

	return TRUE;
}
 
void CDgnConBentUpInfoGrid::SetValue(ROWCOL nRow, ROWCOL nCol, T_BURB_SUB_D& BrubD)
{
	CString strTemp;
	switch ((int)nCol)
	{
	case D_COL_NUM:
		strTemp.Format(_T("%d"), BrubD.nNum);
		break;
	case D_COL_SIZE	:
		strTemp = BrubD.strDia;
		break;
	case D_COL_DB:	
		strTemp.Format(_T("%g"), BrubD.dDb);
		break;
	case D_COL_XI:	
		strTemp.Format(_T("%g"), BrubD.dXi);
		break;
	case D_COL_ANGLE:
		strTemp.Format(_T("%g"), BrubD.dAngle);
		break;
	case D_COL_HI:
		strTemp.Format(_T("%g"), BrubD.dhi);
		break;
	case D_COL_GRP_APP:
		strTemp.Format(_T("%s"), BrubD.bGroup == TRUE ? _T("1") : _T("0"));
		break;
	case D_COL_GRP_NUM:
		strTemp.Format(_T("%d"), BrubD.nGroupNum);
		break;
	case D_COL_GRP_GAP:
		strTemp.Format(_T("%g"), BrubD.dGroupGap);
		break;
	default:
		ASSERT(0);
		break;
	}
	
	SetExpressionRowCol(nRow, nCol, strTemp);

	SetEnableByMod(nRow, nCol);
}

void CDgnConBentUpInfoGrid::SetData(IN CArray<T_BURB_SUB_D, T_BURB_SUB_D&>& aData)
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	SetRowCount(aData.GetSize() + 1); // 제일 마지막 행은 비어있게 하려고 RowCont를 두번 세팅함 
	SetRowCount(aData.GetSize() + 2); 
	
	int nRow = GetRowCount();

	// Data찍어줌
	for(int i = 0; i < aData.GetSize() ; i++)
	{		
		SetValue(i + 2, aData[i]);
		SetEnableByMod(i + 2, D_COL_GRP_APP);
	}
	
	SetNoRowVal();
	
	SetCurrentCell(2, 1); // CurSel을 다른 Row로 이동시켰다 와야지 마지막 행에 값 입력 할 때 OnInsertEmptyRecord()가 불러지기 때문에 CurSel을 변경해줌..

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnConBentUpInfoGrid::SetNoRowVal()
{
	int nRowCnt = GetRowCount();
	// 0번 row에 Number찍어줌
	for(int nRow = 2; nRow <= nRowCnt; nRow++)
	{		
		CString strTemp;
		strTemp.Format(_T("%d"), nRow - 1);
		SetExpressionRowCol(nRow , 0, strTemp);
	}
}

void CDgnConBentUpInfoGrid::SetValue(ROWCOL nRow, T_BURB_SUB_D& tData)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nColCnt = GetColCount();
	for(int col = 1 ; col <= nColCnt ; col++)
	{
		SetValue(nRow, col, tData);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnConBentUpInfoGrid::GetData(OUT CArray<T_BURB_SUB_D, T_BURB_SUB_D&>& aData)
{
	int nRowCnt = GetRowCount();

	aData.SetSize(nRowCnt - 2);

	BOOL bOldLock = LockUpdate(TRUE);

	for(int i = 0 ; i < aData.GetSize() ; i++)
	{
		GetValue(i + 2, aData[i]);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnConBentUpInfoGrid::SetHeaderTitle()
{
	// Column Title
	SetCoveredCellsRowCol(0, 0, 1, 0);

	SetCoveredCellsRowCol(0, D_COL_NUM, 1, D_COL_NUM);
	SetStyleRange(CGXRange(0, D_COL_NUM 			), CGXStyle().SetValue(_LS(IDS_DGN_NUM_DOT)));

	SetCoveredCellsRowCol(0, D_COL_SIZE, 1, D_COL_SIZE);
	SetStyleRange(CGXRange(0, D_COL_SIZE			), CGXStyle().SetValue(_LS(IDS_DGN_CON_BEAM_Size)));

	SetCoveredCellsRowCol(0, D_COL_DB, 1, D_COL_DB);
	SetStyleRange(CGXRange(0, D_COL_DB			), CGXStyle().SetValue(_LS(IDS_DGN_CON_BEAM_Db)));

	SetCoveredCellsRowCol(0, D_COL_XI, 1, D_COL_XI);
	SetStyleRange(CGXRange(0, D_COL_XI		), CGXStyle().SetValue(_LS(IDS_DGN_XI)));

	SetCoveredCellsRowCol(0, D_COL_ANGLE, 1, D_COL_ANGLE);
	SetStyleRange(CGXRange(0, D_COL_ANGLE	), CGXStyle().SetValue(_LS(IDS_DGN_ANGLE)));

	SetCoveredCellsRowCol(0, D_COL_HI, 1, D_COL_HI);
	SetStyleRange(CGXRange(0, D_COL_HI			), CGXStyle().SetValue(_LS(IDS_DGN_HI)));
	
	SetCoveredCellsRowCol(0, D_COL_GRP_APP, 0, D_COL_GRP_GAP);
	SetStyleRange(CGXRange(0, D_COL_GRP_APP		), CGXStyle().SetValue(_LS(IDS_DGN_PSC_RPTJTG_Tendon_Group)));

	SetStyleRange(CGXRange(1, D_COL_GRP_APP		), CGXStyle().SetValue(_LS(IDS_DGN_APP_DOT)));
	SetStyleRange(CGXRange(1, D_COL_GRP_NUM		), CGXStyle().SetValue(_LS(IDS_DGN_NUM_DOT)));
	SetStyleRange(CGXRange(1, D_COL_GRP_GAP		), CGXStyle().SetValue(_LS(IDS_DGN_GAP)));
}

void CDgnConBentUpInfoGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBBrowserWnd::OnModifyCell(nRow, nCol);

	SetEnableByMod(nRow, nCol);

	BOOL bOldLock = LockUpdate(TRUE);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnConBentUpInfoGrid::SetEnableByMod(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nRowTemp, nColTemp;
	BOOL bCanGetCurCell;
	if(GetCurrentCell(nRowTemp, nColTemp))
	{
		bCanGetCurCell = TRUE;
	}
	else
	{
		bCanGetCurCell = FALSE;
	}

	if(nCol == D_COL_GRP_APP)
	{
		CString strApp;
		
		strApp = GetValueRowCol(nRow, D_COL_GRP_APP);
	
		SetStyleRange(CGXRange(nRow, D_COL_GRP_NUM), CGXStyle().SetEnabled(strApp == _T("1")));
		SetStyleRange(CGXRange(nRow, D_COL_GRP_GAP), CGXStyle().SetEnabled(strApp == _T("1")));
	}
}

void CDgnConBentUpInfoGrid::GetValue(ROWCOL nRow, T_BURB_SUB_D& tData)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nColCnt = GetColCount();
	for(int col = 1 ; col <= nColCnt ; col++)
	{
		GetValue(nRow, col, tData);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnConBentUpInfoGrid::GetValue(ROWCOL nRow, ROWCOL nCol, T_BURB_SUB_D& tData)
{
	if(nCol == 0)
	{
		return;
	}

	ROWCOL nColBackup, nRowBackup;
	CString strNow;
	if(GetCurrentCell(nRowBackup, nColBackup))
	{
		strNow = GetValueRowCol(nRow, nCol);
	}
	else
	{
		strNow = GetExpressionRowCol(nRow, nCol);
	}
	
	switch (nCol)
	{
	case D_COL_NUM:
		tData.nNum	= _ttoi(strNow);
		break;
	case D_COL_SIZE	:		
		tData.strDia = strNow;
		break;
	case D_COL_DB:
		tData.dDb = _tstof(strNow);
		break;
	case D_COL_XI:
		tData.dXi = _tstof(strNow);
		break;
	case D_COL_ANGLE:
		tData.dAngle = _tstof(strNow);
		break;
	case D_COL_HI	:
		tData.dhi = _tstof(strNow);
		break;
	case D_COL_GRP_APP	:
		tData.bGroup = strNow == _T("1") ? TRUE : FALSE;
		break;
	case D_COL_GRP_NUM	:
		tData.nGroupNum = _ttoi(strNow);
		break;		
	case D_COL_GRP_GAP  :
		tData.dGroupGap = _tstof(strNow);
		break;		
	default:
		ASSERT(0);
		break;
	}
}


BOOL CDgnConBentUpInfoGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString szValue = GetValueRowCol(nRow, nCol);
	CString szMsg;
	//szValue = szValue.Trim();
	//	int nErrCode;

	switch ((int)nCol)
	{
	case D_COL_NUM:		
		{
			int nVal = _ttoi(szValue);
			if(nVal < 1)
			{
				szMsg.Format(_LS(IDS_DGN_STR_IS_INVALID), _LS(IDS_DGN_NUM_DOT));
				SetWarningText(szMsg);
				return FALSE;
			}
		}
		break;
	case D_COL_SIZE:
		{
			CStringArray aRBName;
			CDBLib::GetRebarNameListByCode(aRBName, TRUE);				
			BOOL bExist = FALSE;
			int nSize = aRBName.GetSize();
			for (int i = 0; i < nSize; i++)
			{
				if(aRBName[i] == szValue)
				{
					bExist = TRUE;
					break;
				}
			}
			if(bExist == FALSE)
			{
				szMsg.Format(_LS(IDS_DGN_STR_IS_INVALID), _LS(IDS_DGN_CON_BEAM_Size));
				SetWarningText(szMsg);
				return FALSE;
			}
		}
		break;
	case D_COL_DB:
	case D_COL_XI:	
		// Db      > IDS_DGN_CON_BEAM_Db
		// Xi      > IDS_DGN_XI
		break;
	case D_COL_ANGLE:
		{
			double dAngle = _tstof(szValue);
			if(dAngle <= 0 || 360 <= dAngle)
			{
				szMsg.Format(_LS(IDS_DGN_STR_IS_INVALID), _LS(IDS_DGN_ANGLE));
				SetWarningText(szMsg);
				return FALSE;
			}
		}
		break;
	case D_COL_HI:
		//hi	> IDS_DGN_HI
		break;
	case D_COL_GRP_APP:
		if(szValue != _T("1") && szValue != _T("0"))
		{
			szMsg.Format(_LS(IDS_DGN_STR_IS_INVALID), _LS(IDS_DGN_APP_DOT));
			SetWarningText(szMsg);
			return FALSE;
		}
		break;
	case D_COL_GRP_NUM:
		{
			CString szApp = GetExpressionRowCol(nRow, D_COL_GRP_APP);
			if(szApp == _T("1"))
			{
				int nVal = _ttoi(szValue);
				if(nVal < 1)
				{
					szMsg.Format(_LS(IDS_DGN_STR_IS_INVALID), _LS(IDS_DGN_NUM_DOT));
					SetWarningText(szMsg);
					return FALSE;
				}
			}			
		}
		break;
	case D_COL_GRP_GAP:
		{
			CString szApp = GetExpressionRowCol(nRow, D_COL_GRP_APP);
			if(szApp == _T("1"))
			{
				double dVal = _tstof(szValue);
				if(dVal < 0.0)
				{
					szMsg.Format(_LS(IDS_DGN_STR_IS_INVALID), _LS(IDS_DGN_GAP));
					SetWarningText(szMsg);
					return FALSE;
				}
			}
		}
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CDgnConBentUpInfoGrid::OnPasteFromClipboard(const CGXRange &range)
{
	OpenClipboard(); // open clipboard
	HGLOBAL handle = GetClipboardData(CF_UNICODETEXT);
	LPTSTR psz = (LPTSTR)GlobalLock(handle);
	DWORD size = (DWORD)GlobalSize(handle);
	GlobalUnlock(handle);
	CloseClipboard(); // close the clipboard
	
	TCHAR* aClipboardStr = new TCHAR[size / 2];	
    int i = 0;
	for(i = 0; i < size; i+=2)  // 2의 배수 Idx에 의미있는 Char가 있고 1의 배수 Idx에는 '\0'이 있어서 이런식으로 String변환을 해줌
	{
		if(psz[i] == '\0')
		{
			aClipboardStr[i / 2] = psz[i];
			break;
		}
		ASSERT(psz[i + 1] == '\0'); // 여기 ASSERT 걸리면  1의 배수 Idx에는 '\0'이 있다는 규칙이 잘못된 경우.. 여기 ASSERT걸리면 이 반복문을 다시 만들어야함..
		aClipboardStr[i / 2] = psz[i];		
	}
	ASSERT(i < size);  // 여기 ASSERT 걸리면 aClipboardStr의 끝이 없는 경우 임.. 메모리가 뻑날것 임..

	CString  sub, szData = aClipboardStr;
	LPTSTR   buf = szData.GetBuffer(1);
	int      pos = 0, endpos, len = szData.GetLength();
	TCHAR    endTCHAR;
	int		 col = range.left;
	int  	 row = range.top;
	CGXStyle style;

	BOOL bOldLock = LockUpdate(TRUE);
	
	while(pos < len)
	{
		if(buf[pos] == _T('\t') || buf[pos] == _T('\r') || buf[pos] == _T('\n'))
		{
			endTCHAR = buf[pos];
			endpos = pos;
		}		
		else // find the end of the item then copy the item to the cell
		{
			endpos = pos + 1;
			while(endpos < len)
			{
				endTCHAR = buf[endpos];
				if(endTCHAR == _T('\n') || endTCHAR == _T('\r') || endTCHAR == _T('\t')) break;
				endpos++;
			}

			if(col <= (int)GetColCount() && row <= (int)GetRowCount() - 1)
			{
				GetStyleRowCol(row, col, style);				
				//if(! style.GetIncludeEnabled())
				//{
					sub = szData.Mid(pos, endpos - pos);

					CString strValBackup;
					strValBackup = GetValueRowCol(row, col);
					SetExpressionRowCol(row, col, sub);
					if(!OnValidateField(row, col))
					{
						SetExpressionRowCol(row, col, strValBackup);
					}
					SetEnableByMod(row, col);
				//}
			}
		}

		if(endTCHAR == _T('\t')) col++;
		if(endTCHAR == _T('\r') || endTCHAR == _T('\n'))
		{
			col = range.left;
			row++;
			if(buf[endpos] == _T('\r') && buf[endpos + 1] == _T('\n')) endpos++;
		}
		pos = endpos + 1;
	}
	szData.ReleaseBuffer();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	delete []aClipboardStr;

	return TRUE;	
}

BOOL CDgnConBentUpInfoGrid::OnLButtonHitRowCol(ROWCOL nHitRow, ROWCOL nHitCol, ROWCOL nDragRow, ROWCOL nDragCol, CPoint point, UINT flags, WORD nHitState)
{
	if(!CTBBrowserWnd::OnLButtonHitRowCol(nHitRow, nHitCol, nDragRow, nDragCol, point, flags, nHitState))
		return FALSE;
	
	SetEnableByMod(nHitRow, nHitCol);

	return TRUE;
}

BOOL CDgnConBentUpInfoGrid::OnInsertEmptyRecord(ROWCOL nRow)
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
	//	if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckBox))) bActive = TRUE;
	//	else if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckListComboBox))) bActive = TRUE;

	BOOL bOldLock = LockUpdate(TRUE);
	
	CString szTemp;
	szTemp.Format(_T("%d"), GetRowCount() - 2);
	SetExpressionRowCol(nRow, 0, szTemp);
	
	szTemp.Format(_T("%d"), GetRowCount() - 1);
	SetExpressionRowCol(nRow + 1, 0, szTemp);

	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		nCol = i + 1;
		value = GetDefValue(nCol);		
		if (!value.IsEmpty() && (nCol != ncCol || !bActive) )
		{			
			SetExpressionRowCol(nRow, nCol, value);
			SetEnableByMod(nRow, nCol);
		}
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;
}

CString CDgnConBentUpInfoGrid::GetDefValue(int nIndex)
{
	T_BURB_SUB_D tData;

	//Convert Out
	T_BURB_D tBrubForUnitOut;
	tBrubForUnitOut.aBurbSub.SetSize(1);
	tBrubForUnitOut.aBurbSub[0] = tData;
	m_pDoc->m_pUnitCtrl->ConvertUnitBurbOut(tBrubForUnitOut);
	tData = tBrubForUnitOut.aBurbSub[0];

	CString strTemp;
	switch (nIndex)
	{
	case D_COL_NUM:
		{			
			strTemp.Format(_T("%d"), tData.nNum);
		}
		break;
	case D_COL_SIZE	:
		{
			strTemp = tData.strDia;
		}
		break;
	case D_COL_DB:	
		{
			strTemp.Format(_T("%g"), tData.dDb);
		}
		break;
	case D_COL_XI	:	
		{
			strTemp.Format(_T("%g"), tData.dXi);
		}
		break;
	case D_COL_ANGLE:
		{
			strTemp.Format(_T("%g"), tData.dAngle);
		}
		break;
	case D_COL_HI:
		{
			strTemp.Format(_T("%g"), tData.dhi);
		}
		break;
	case D_COL_GRP_APP:
		{
			strTemp = tData.bGroup == TRUE ? _T("1") : _T("0");
		}
		break;
	case D_COL_GRP_NUM:
		{
			strTemp.Format(_T("%d"), tData.nGroupNum);
		}
		break;
	case D_COL_GRP_GAP:
		{
			strTemp.Format(_T("%g"), tData.dGroupGap);
		}
		break;		
	default:
		ASSERT(0);
		break;
	}
		
	return strTemp;
}

void CDgnConBentUpInfoGrid::GetCurSelRows(CArray<ROWCOL, ROWCOL>& aSelRows)
{
	CRowColArray arRow;
	GetSelectedRows(arRow, TRUE, FALSE);

	int nRow = arRow.GetSize();
	aSelRows.SetSize(nRow);

	for(int i = 0; i < nRow; i++)
	{
		aSelRows[i] = arRow[i] - 1;
	}
}

void CDgnConBentUpInfoGrid::UpdateUnit()
{
	BOOL bOldLock = LockUpdate(TRUE);

	T_BURB_D BurbD;
	BurbD.Initialize();	
	GetData(BurbD.aBurbSub);
	m_pDoc->m_pUnitCtrl->ConvertUnitBurbPrevious(BurbD);
	SetData(BurbD.aBurbSub);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CDgnConBentUpInfoGrid::OnDeleteRecord( ROWCOL nRow )
{
	CTBBrowserWnd::OnDeleteRecord(nRow);
	SetNoRowVal();
	return TRUE;
}

BOOL CDgnConBentUpInfoGrid::OnDeleteRecords( CRowColArray &awRows )
{
	CTBBrowserWnd::OnDeleteRecords(awRows);
	SetNoRowVal();
	return TRUE;
}
