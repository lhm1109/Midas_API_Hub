#if !defined(AFX_CMSECTITEMPSCIJOINTGRID_H__48B290FF_72B2_41E2_8BF7_E993C3A4832A__INCLUDED_)
#define AFX_CMSECTITEMPSCIJOINTGRID_H__48B290FF_72B2_41E2_8BF7_E993C3A4832A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCIJointGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCIJointGrid window

class CCMSectItemPSCIJointGrid : public CCMBaseGrid
{
// Construction
public:
	CCMSectItemPSCIJointGrid();
	BOOL m_bInitDone;
	BOOL m_bRedraw;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCIJointGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSectItemPSCIJointGrid();
	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent,int nIOrTee=0);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();
	void DecideEnable();
	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSCIJointGrid)
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
	void ShowLeftJoint();
	void ShowRightJoint();
	
protected:
	T_SECT_D* m_pSect;
	
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

#endif // !defined(AFX_CMSECTITEMPSCIJOINTGRID_H__48B290FF_72B2_41E2_8BF7_E993C3A4832A__INCLUDED_)