#if !defined(AFX_CMSPECMODEGRID_H__70C3E3A7_FDBA_4176_90A9_1590DCA76AAA__INCLUDED_)
#define AFX_CMSPECMODEGRID_H__70C3E3A7_FDBA_4176_90A9_1590DCA76AAA__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSpecModeGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
/////////////////////////////////////////////////////////////////////////////
// CCMWindPulsationModeGrid window
class CCMWindPulsationDlg;
class CCMWindPulsationModeGrid : public CGXGridWnd
{
// Construction
public:
	CCMWindPulsationModeGrid();
	BOOL m_bInitDone;
	BOOL m_bRedraw;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMWindPulsationModeGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMWindPulsationModeGrid();
	void InitGrid(CCMWindPulsationDlg* pParent, int nDirection);
	void SetDataSource(CArray<bool, bool>* pData, CArray<double, double>& arModePercent, BOOL bSelect);
	void ShowData();
	BOOL SaveData();
	void SetSelect(BOOL bSelect) {	m_bSelect = bSelect; }
	// Generated message map functions
protected:
	//{{AFX_MSG(CCMWindPulsationModeGrid)
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
	int m_nDirection;
	CCMWindPulsationDlg* m_pParent;
	CArray<bool, bool>* m_pData;
	CArray<double, double> m_arModePercent;

	BOOL m_bSelect;
	int m_nModeNum;
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