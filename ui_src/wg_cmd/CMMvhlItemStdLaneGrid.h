// CMMvhlItemStdLaneGrid.h: interface for the CCMMvhlItemStdLaneGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMMVHLITEMSTDLANEGRID_H__42DACDCE_94AF_48AC_93EE_3A416BDD2115__INCLUDED_)
#define AFX_CMMVHLITEMSTDLANEGRID_H__42DACDCE_94AF_48AC_93EE_3A416BDD2115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdLaneGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdLaneGrid window

class CCMMvhlItemStdLaneGrid : public CGXGridWnd
{
// Construction
public:
	CCMMvhlItemStdLaneGrid();
	BOOL m_bInitDone;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdLaneGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMMvhlItemStdLaneGrid();
	void InitGrid();
	void SetDataSource(T_MVHL_D* pMvhl,CWnd* pParent, BOOL bStd);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMMvhlItemStdLaneGrid)
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
	int  GetSelectedRowsCount(int top,int bottom);
	int  GetSelectedColsCount(int left,int right);
	BOOL VerifyPasteRange(CGXRange& from,CGXRange& to);
protected:
	T_MVHL_D* m_pMvhl;
	CWnd* m_pParent;
	BOOL m_bStd;
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

#endif // !defined(AFX_CMMVHLITEMSTDLANEGRID_H__42DACDCE_94AF_48AC_93EE_3A416BDD2115__INCLUDED_)
