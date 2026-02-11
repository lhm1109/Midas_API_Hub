#if !defined(AFX_CMMVLDITEMTRGRID_H__3D706477_3712_42CB_B087_7FBE72A7CCE5__INCLUDED_)
#define AFX_CMMVLDITEMTRGRID_H__3D706477_3712_42CB_B087_7FBE72A7CCE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemTrGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemTrGrid window

class CCMMvldItemTrGrid : public CGXGridWnd
{
// Construction
public:
	CCMMvldItemTrGrid();
	BOOL m_bInitDone;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldItemTrGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMMvldItemTrGrid();
	void InitGrid();
	void SetDataSource(T_MVLDtr_D* pMvldTr,CWnd* pParent);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMMvldItemTrGrid)
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
protected:
	T_MVLDtr_D* m_pMvldTr;
	CWnd* m_pParent;
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDITEMTRGRID_H__3D706477_3712_42CB_B087_7FBE72A7CCE5__INCLUDED_)
