// DgnSectGridWnd.cpp: implementation of the CDgnSectGridWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSectGridWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
unsigned int WM_DGN_SECT_GRID_CELL_CLIK = 
 RegisterWindowMessage(_T("WM_DGN_SECT_GRID_CELL_CLIK"));
unsigned int WM_DGN_SECT_GRID_CELL_CHEK = 
 RegisterWindowMessage(_T("WM_DGN_SECT_GRID_CELL_CHEK"));
CDgnSectGridWnd::CDgnSectGridWnd()
{

}

CDgnSectGridWnd::~CDgnSectGridWnd()
{

}

BEGIN_MESSAGE_MAP(CDgnSectGridWnd, CGXGridWnd)
	//{{AFX_MSG_MAP(CDgnSectGridWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnSectGridWnd::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	GetParam()->SetLockReadOnly(FALSE);
	if(nRow!=0)
	{
		CWnd* pParent = this->GetParent();
		ASSERT(pParent);
		
	  if(nCol==1)
	  {
			int check = _ttoi(GetValueRowCol(nRow,1));
		  if(check!=1)
		  {
			  SetValueRange(CGXRange(nRow,1),_T("1"),gxOverride,0,GX_UPDATENOW);
				SetStyleRange(CGXRange(nRow,0,nRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_SelColor1)));
		  }
		  else
		  {
			  SetValueRange(CGXRange(nRow,1),_T("0"),gxOverride,0,GX_UPDATENOW);
				SetStyleRange(CGXRange(nRow,0,nRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_UnSelColor1)));
		  }
			pParent->SendMessage(WM_DGN_SECT_GRID_CELL_CHEK,0L,0L);
		}

		for(int iRow=1; iRow<= GetRowCount(); iRow++)
		{
			int check = _ttoi(GetValueRowCol(iRow,1));
			if(check!=0)
				SetStyleRange(CGXRange(iRow,0,iRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_SelColor1)));
		  else
				SetStyleRange(CGXRange(iRow,0,iRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_UnSelColor1)));
		}

		SetStyleRange(CGXRange(nRow,0,nRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_SelColor2)));

		pParent->SendMessage(WM_DGN_SECT_GRID_CELL_CLIK,0L,0L);
	}
			
	GetParam()->SetLockReadOnly(TRUE);

	return TRUE;
}

BOOL CDgnSectGridWnd::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{

	ROWCOL nRow=1, nCol=1;

	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	VERIFY(GetCurrentCell(nRow, nCol));
	if (nMessage == WM_KEYDOWN || nMessage == VK_SPACE)
	{
		// 셀이 ACTIVE인 상태에서 좌우 화살표 키가 눌러지면 셀 이동을 하지 않는다.
		if (nChar == VK_LEFT || nChar == VK_RIGHT)
		{
			CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
			return TRUE;
		}

		ROWCOL iNextRow = 0;
		if(nChar == VK_UP || nChar == VK_DOWN)
		{
			//Previous Row
		  int check = _ttoi(GetValueRowCol(nRow,1));
			GetParam()->SetLockReadOnly(FALSE);
		  if(check!=0)
				SetStyleRange(CGXRange(nRow,0,nRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_SelColor1)));
		  else
				SetStyleRange(CGXRange(nRow,0,nRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_UnSelColor1)));

			CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
			//Moved Row
			GetCurrentCell(iNextRow, nCol);
			if(iNextRow < 1)              iNextRow = 1;
			if(iNextRow > GetRowCount())  iNextRow = GetRowCount();

			SetCurrentCell(iNextRow, nCol);
			SetStyleRange(CGXRange(iNextRow,0,iNextRow,GetColCount()),CGXStyle().SetInterior(CGXBrush().SetColor(cDGN_SelColor2)));
			GetParam()->SetLockReadOnly(TRUE);
			CWnd* pParent = this->GetParent();
			ASSERT(pParent);
			pParent->SendMessage(WM_DGN_SECT_GRID_CELL_CLIK,0L,0L);
		}
		else if(nChar == VK_SPACE)
		{
			OnLButtonClickedRowCol(nRow, 1, 0, NULL);
		}
	}

	//CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	return TRUE;
}
