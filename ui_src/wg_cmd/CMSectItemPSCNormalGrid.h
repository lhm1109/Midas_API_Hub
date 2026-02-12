#if !defined(AFX_CMSECTITEMPSCNORMALGRID_H__09F12CC2_935B_45E6_8038_5BC91966E8AB__INCLUDED_)
#define AFX_CMSECTITEMPSCNORMALGRID_H__09F12CC2_935B_45E6_8038_5BC91966E8AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCNormalGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormalGrid window

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_base\CMBaseGrid.h"
class CCMSectItemPSCNormalGrid : public CCMBaseGrid
{
// Construction
public:
	CCMSectItemPSCNormalGrid();
	BOOL m_bInitDone;

// Attributes
public:

// Operations
public:

	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCNormalGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSectItemPSCNormalGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSCNormalGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetDownValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetUpValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL InvalidDoubleNumber(int nErrCode);
	BOOL ValidateError(LPCTSTR lpszError);
	void SetStyleOnSizeCell();

	BOOL VerifyPasteRange(CGXRange& from,CGXRange& to);
	int  GetSelectedRowsCount(int top,int bottom);
	int  GetSelectedColsCount(int left,int right);

	void ChangeByType();
	void ChangeBySect();
	void ChangeByCell();
	void ChangeByJoint();
	int GetSellType();
	int GetHalfType();
	int IsHalfType();

protected:
	T_SECT_D* m_pSect;

	CWnd* m_pParent;
	int m_n1OR2;
	BOOL m_bOnStartEditing;
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual BOOL CopyRange(const CGXRangeList& selList);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMPSCNORMALGRID_H__09F12CC2_935B_45E6_8038_5BC91966E8AB__INCLUDED_)

