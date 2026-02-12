#if !defined(AFX_CMMVHLITEMSOUTHAFRICALOADGRID_H__4FB51AF2_3F8C_4F6F_AB6A_B2B466EF1E06__INCLUDED_)
#define AFX_CMMVHLITEMSOUTHAFRICALOADGRID_H__4FB51AF2_3F8C_4F6F_AB6A_B2B466EF1E06__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlItemSouthAfricaLoadGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemSouthAfricaLoadGrid window

class CCMMvhlItemSouthAfricaLoadGrid : public CGXGridWnd
{
// Construction
public:
	CCMMvhlItemSouthAfricaLoadGrid();
	BOOL m_bInitDone;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlItemSouthAfricaLoadGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMMvhlItemSouthAfricaLoadGrid();
	void InitGrid();
	void SetDataSource(T_MVHL_D* pMvhl,int nIndex, int nFrom);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();
	void SetEnable(BOOL bEnable);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMMvhlItemSouthAfricaLoadGrid)
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
	int m_nIndex;
	int m_nFrom;
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

#endif // !defined(AFX_CMMVHLITEMSOUTHAFRICALOADGRID_H__4FB51AF2_3F8C_4F6F_AB6A_B2B466EF1E06__INCLUDED_)
