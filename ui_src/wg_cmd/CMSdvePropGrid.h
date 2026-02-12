#if !defined(AFX_CMSdvePropGrid_H__20121120__INCLUDED_)
#define AFX_CMSdvePropGrid_H__20121120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSdvePropGrid.h : header file
//

// CGXGridWnd Drived class
class CCMSdvePropGrid : public CGXGridWnd
{
	// Construction and Destruction
public:
	CCMSdvePropGrid() { }

	// Attributes
public:

	// Operations
public:
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL CanPaste() { return FALSE; }

	// Implementations
	BOOL TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);

	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSdvePropGrid_H__20121120__INCLUDED_)
