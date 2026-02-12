#if !defined(AFX_CMMVHLITEMSTDKRLSD15LOADGRID_H__4FB51AF2_3F8C_4F6F_AB6A_B2B466EF1E06__INCLUDED_)
#define AFX_CMMVHLITEMSTDKRLSD15LOADGRID_H__4FB51AF2_3F8C_4F6F_AB6A_B2B466EF1E06__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdKRLSD15LoadGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdKRLSD15LoadGrid window

class CCMMvhlItemStdKRLSD15LoadGrid : public CGXGridWnd
{
// Construction
public:
	CCMMvhlItemStdKRLSD15LoadGrid();
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdKRLSD15LoadGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMMvhlItemStdKRLSD15LoadGrid();
	void InitGrid();
	void SetDataSource(T_MVHL_D* pMvhl);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMMvhlItemStdKRLSD15LoadGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL SetDownValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetUpValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL InvalidDoubleNumber(int nErrCode);
	BOOL ValidateError(LPCTSTR lpszError);
	void SetStyleOnSizeCell();
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	int  GetLoadSize();
	int  GetSelectedRowsCount(int top,int bottom);
	int  GetSelectedColsCount(int left,int right);
	BOOL VerifyPasteRange(CGXRange& from,CGXRange& to);
protected:
	T_MVHL_D* m_pMvhl;
	BOOL m_bEnable;
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL CopyRange(const CGXRangeList& selList);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVHLITEMSTDKRLSD15LOADGRID_H__4FB51AF2_3F8C_4F6F_AB6A_B2B466EF1E06__INCLUDED_)
