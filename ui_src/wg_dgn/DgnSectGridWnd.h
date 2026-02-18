// DgnSectGridWnd.h: interface for the CDgnSectGridWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNSECTGRIDWND_H__1A1A9FFB_9953_4078_8B04_D73CC7674A2B__INCLUDED_)
#define AFX_DGNSECTGRIDWND_H__1A1A9FFB_9953_4078_8B04_D73CC7674A2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const COLORREF cDGN_SelColor1   = RGB(166,202,240);
const COLORREF cDGN_SelColor2   = RGB(136,172,210);
const COLORREF cDGN_UnSelColor1 = RGB(255,255,255);
const COLORREF cDGN_UnSelColor2 = RGB(225,225,225);

class CDgnSectGridWnd : public CGXGridWnd  
{
// Construction
public:
	CDgnSectGridWnd();

// Attributes
public:

// Operations
public:
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSectGridWnd)
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual ~CDgnSectGridWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDgnSectGridWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern unsigned int WM_DGN_SECT_GRID_CELL_CHEK;
extern unsigned int WM_DGN_SECT_GRID_CELL_CLIK;
#endif // !defined(AFX_DGNSECTGRIDWND_H__1A1A9FFB_9953_4078_8B04_D73CC7674A2B__INCLUDED_)
