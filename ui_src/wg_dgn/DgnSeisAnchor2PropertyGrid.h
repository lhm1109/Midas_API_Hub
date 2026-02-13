#if !defined(__DgnSeisAnchor2PropertyGrid_H__)
#define __DgnSeisAnchor2PropertyGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2PropertyGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2PropertyGrid window

class CDgnSeisAnchorEvalItem2NewSubPropertyDlg;

class CDgnSeisAnchor2PropertyGrid : public CGXGridWnd
{

	// Construction
public:
	CDgnSeisAnchor2PropertyGrid(CWnd* pParent = NULL);
	virtual ~CDgnSeisAnchor2PropertyGrid();

	// Operations
public:
	void Initialize();
 	void SetData2Grid(T_ANEV_BRPR_D& Data);
 	BOOL GetGrid2Data(T_ANEV_BRPR_D& Data);

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags);
	BOOL SetRightValidCell (ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell  (ROWCOL nRow, ROWCOL nCol);

	//////////////////////////////////////////////////////////////////////////
	// OnPasteFromClipboard 관련
	BOOL VerifyPasteRange(CGXRange& from, CGXRange& to);
	int  GetSelectedRowsCount(int top, int bottom);
	int  GetSelectedColsCount(int left, int right);
	void ShowData();

public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIehpYieldPropGrid)
	//}}AFX_VIRTUAL

protected:
	virtual BOOL GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt = gxCopy, int nType = 0);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	//virtual BOOL CanPaste() { return FALSE; } // 복사 허용
	virtual BOOL CanCut() { return FALSE; }
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);


protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);

	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void SetViewData();

	CDgnSeisAnchorEvalItem2NewSubPropertyDlg* m_pParent;

	CDBDoc* m_pDoc;
	// Generated message map functions

	//{{AFX_MSG(CDgnSeisAnchor2PropertyGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2PropertyGrid_H__)
