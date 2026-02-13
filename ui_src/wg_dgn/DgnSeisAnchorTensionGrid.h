#if !defined(__DgnSeisAnchorTensionGrid_H__)
#define __DgnSeisAnchorTensionGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorTensionGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorTensionGrid window

class CDgnSeisAnchorTensionGrid : public CGXGridWnd
{

	// Construction
public:
	CDgnSeisAnchorTensionGrid();
	virtual ~CDgnSeisAnchorTensionGrid();

	// Operations
public:
	void Initialize();
 	void SetData2Grid(T_ANEV_D& Data);
 	BOOL GetGrid2Data(T_ANEV_D& Data);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL CanPaste() { return FALSE; } 복사 허용
	virtual BOOL CanCut() { return FALSE; }
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);


protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);

	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	CDBDoc* m_pDoc;
	// Generated message map functions

	//{{AFX_MSG(CDgnSeisAnchorTensionGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchorTensionGrid_H__)
