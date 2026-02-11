#if !defined(AFX_CMSectItemPSC_WarpingCheckGrid__INCLUDED_)
#define AFX_CMSectItemPSC_WarpingCheckGrid__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSC_WarpingCheckGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC_WarpingCheckGrid window

class CCMSectItemPSC_WarpingCheckGrid : public CGXGridWnd
{
	// Construction
public:
	CCMSectItemPSC_WarpingCheckGrid();
	BOOL m_bInitDone;

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC_WarpingCheckGrid)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CCMSectItemPSC_WarpingCheckGrid();
	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent, BOOL bTapered=FALSE);
	void ShowData();
	BOOL SaveData();
	void DecideEnable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSC_WarpingCheckGrid)
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
	T_SECT_D* m_pSect;
	CWnd* m_pParent;
	BOOL m_bOnStartEditing;
	BOOL m_bTapered;

	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL CopyRange(const CGXRangeList& selList);

};

// ********************************************************
// grid-J
// ********************************************************
class CCMSectItemPSC_WarpingCheckTapJGrid : public CGXGridWnd
{
	// Construction
public:
	CCMSectItemPSC_WarpingCheckTapJGrid();
	BOOL m_bInitDone;

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSC_WarpingCheckGrid)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CCMSectItemPSC_WarpingCheckTapJGrid();
	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent, BOOL bTap=FALSE);
	void ShowData();
	BOOL SaveData();
	void DecideEnable();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSC_WarpingCheckGrid)
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
	T_SECT_D* m_pSect;
	CWnd* m_pParent;
	BOOL m_bOnStartEditing;
 
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

#endif // !defined(AFX_CMSectItemPSC_WarpingCheckGrid__INCLUDED_)


