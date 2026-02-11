#if !defined(AFX_CMSPECMODEGRID_H__70C3E3A7_FDBA_4176_90A9_1590DCA76AAA__INCLUDED_)
#define AFX_CMSPECMODEGRID_H__70C3E3A7_FDBA_4176_90A9_1590DCA76AAA__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecModeGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSpecModeGrid window

class CCMSpecModeGrid : public CGXGridWnd
{
// Construction
public:
	CCMSpecModeGrid();
	BOOL m_bInitDone;
	BOOL m_bRedraw;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSpecModeGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSpecModeGrid();
	void InitGrid();
	void SetDataSource(T_SPLC_D* pData);
	void ShowData();
	BOOL SaveData();
	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSpecModeGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetDownValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetUpValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void SetStyleOnSizeCell();
	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL InvalidDoubleNumber(int nErrCode);
	BOOL ValidateError(LPCTSTR lpszError);
	int  GetSelectedRowsCount(int top,int bottom);
	int  GetSelectedColsCount(int left,int right);
	BOOL VerifyPasteRange(CGXRange& from,CGXRange& to);
	 
protected:
	T_SPLC_D* m_pData;
	int m_nModeNum;
	CWnd* m_pParent;
	BOOL m_bOnStartEditing;
	int m_nInsertNum;
	int m_nIOrTee;
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CopyRange(const CGXRangeList& selList);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSPECMODEGRID_H__70C3E3A7_FDBA_4176_90A9_1590DCA76AAA__INCLUDED_)