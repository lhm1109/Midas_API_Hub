// CMSdvePropGrid.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdvePropGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL CCMSdvePropGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			if (TabKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
			if (RightKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		case VK_LEFT: // 이전 셀로 이동
			if (LeftKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		switch(nChar)
		{
			// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (SpaceKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}


BOOL CCMSdvePropGrid::TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	GetParent()->GetDlgItem(IDC_CMD_SP_SD_BTN_OK)->SetFocus();
	return TRUE;
}

BOOL CCMSdvePropGrid::LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetLeftValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMSdvePropGrid::RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetRightValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMSdvePropGrid::SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (pControl && !pControl->IsActive() 
		&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
	{
		if (!pControl->IsInit()) pControl->Init(nRow, nCol);
		CString value = GetExpressionRowCol(nRow, nCol);
		pControl->SetActive(TRUE);
		pControl->Refresh(); // 반드시 호출

		if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면
			((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
		return TRUE;
	}
	return FALSE;
}

BOOL CCMSdvePropGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (j < 1) { i--; j = GetColCount(); }  

	while (i >= 1)
	{
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사

			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				SetCurrentCell(i, j);
				return TRUE;
			}
			j--;  // 이전 열을 조사


		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}

BOOL CCMSdvePropGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol+1; // 현재 행 다음 열
	if (j > GetColCount()) { i++; j = 1; }

	while (i <= GetRowCount())
	{
		while (j <= GetColCount())
		{
			// 열이 선택될 수 있는지 조사


			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				SetCurrentCell(i, j);
				return TRUE;
			}
			j++;  // 다음 열을 조사


		}
		i++;  // 다음 줄로 이동
		j = 1;
	}

	return FALSE;
}