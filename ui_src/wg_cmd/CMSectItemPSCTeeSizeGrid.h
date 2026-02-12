#if !defined(AFX_CMSECTITEMPSCTEESIZEGRID_H__661BE271_0BD3_4E29_8479_F73BF4E0F8C3__INCLUDED_)
#define AFX_CMSECTITEMPSCTEESIZEGRID_H__661BE271_0BD3_4E29_8479_F73BF4E0F8C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCTeeSizeGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCTeeSizeGrid window

class CCMSectItemPSCTeeSizeGrid : public CCMBaseGrid
{
// Construction
public:
	CCMSectItemPSCTeeSizeGrid();
	BOOL m_bInitDone;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCTeeSizeGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSectItemPSCTeeSizeGrid();
	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();
	void DecideEnable();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSCTeeSizeGrid)
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

#endif // !defined(AFX_CMSECTITEMPSCTEESIZEGRID_H__661BE271_0BD3_4E29_8479_F73BF4E0F8C3__INCLUDED_)


