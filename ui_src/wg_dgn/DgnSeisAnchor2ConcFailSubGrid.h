#if !defined(__DgnSeisAnchor2ConcFailSubGrid_H__)
#define __DgnSeisAnchor2ConcFailSubGrid_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2ConcFailSubGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ConcFailSubGrid window

class CDgnSeisAnchor2ConcFailSub;

class CDgnSeisAnchor2ConcFailSubGrid : public CGXGridWnd
{

	// Construction
public:
	CDgnSeisAnchor2ConcFailSubGrid(CWnd* pParent = NULL);
	virtual ~CDgnSeisAnchor2ConcFailSubGrid();

	// Operations
public:
	void Initialize();
 	void SetData2Grid(T_ANEV_CONC_FAIL_D* pConcFail, T_ANEV_D& AnevD);
 	BOOL GetGrid2Data(T_ANEV_CONC_FAIL_D* pConcFail);
	void EnableDisableGrid(BOOL Enable);

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
	virtual BOOL OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt);
	//virtual BOOL CanPaste() { return FALSE; } // 복사 허용
	virtual BOOL CanCut() { return FALSE; }
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);


protected:
	void SetDataToRow(ROWCOL nRow, CStringArray& raValue, T_ANEV_FAIL_SEL_LAYER_D& LayerOrg);
	BOOL GetRowToData(ROWCOL nRow, CStringArray& raValue);

	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void SetViewData();

	CDgnSeisAnchor2ConcFailSub* m_pParent;

	CDBDoc* m_pDoc;
	T_ANEV_CONC_FAIL_D m_ConcFail;
	T_ANEV_D m_AnevD;
	// Generated message map functions

	//{{AFX_MSG(CDgnSeisAnchor2ConcFailSubGrid)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2ConcFailSubGrid_H__)
