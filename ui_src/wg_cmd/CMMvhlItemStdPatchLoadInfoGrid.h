// CMMvhlItemStdPatchLoadInfoGrid.h: interface for the CCMMvhlItemStdPatchLoadInfoGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMMvhlItemStdPatchLoadInfoGrid_H__INCLUDED_)
#define AFX_CMMvhlItemStdPatchLoadInfoGrid_H__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemStdPatchLoadInfoGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdPatchLoadInfoGrid window

class CCMMvhlItemStdPatchLoadInfoGrid : public CGXGridWnd
{
// Construction
public:
	CCMMvhlItemStdPatchLoadInfoGrid();
	BOOL m_bInitDone;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemStdPatchLoadInfoGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMMvhlItemStdPatchLoadInfoGrid();
	void InitGrid(int nGridWidth);
	void SetDataSource(T_MVHL_D* pMvhl,CWnd* pParent, int nSize);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMMvhlItemStdPatchLoadInfoGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL SetDownValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetUpValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL InvalidDoubleNumber(int nErrCode);
	BOOL ValidateError(LPCTSTR lpszError);
	//void SetStyleOnSizeCell();
	void SetStyleOnSizeCell();

protected:
	
	T_MVHL_D* m_pMvhl;
	CWnd* m_pParent;
	int m_nSize, m_nTypeSize;
	int m_nGridWidth;
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMvhlItemStdPatchLoadInfoGrid_H__INCLUDED_)

