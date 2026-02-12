#if !defined(AFX_CMSECTITEMPSCMIDSIZEGRID_H__46D930B0_C481_4DE9_BC94_EC4E2889105B__INCLUDED_)
#define AFX_CMSECTITEMPSCMIDSIZEGRID_H__46D930B0_C481_4DE9_BC94_EC4E2889105B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCMidSizeGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCMidSizeGrid window

class CCMSectItemPSCMidSizeGrid : public CCMBaseGrid
{
// Construction
public:
	CCMSectItemPSCMidSizeGrid();
	BOOL m_bInitDone;
	BOOL m_bStarted;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCMidSizeGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSectItemPSCMidSizeGrid();
	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();
	void DecideEnable();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSCMidSizeGrid)
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

#endif // !defined(AFX_CMSECTITEMPSCMIDSIZEGRID_H__46D930B0_C481_4DE9_BC94_EC4E2889105B__INCLUDED_)


