#if !defined(AFX_CMSECTITEMPSCNCELLDOWNGRID_H__D022AB02_90AE_4900_96BD_77AE48174EAF__INCLUDED_)
#define AFX_CMSECTITEMPSCNCELLDOWNGRID_H__D022AB02_90AE_4900_96BD_77AE48174EAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemPSCnCellDownGrid.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellDownGrid window

class CCMSectItemPSCnCellDownGrid : public CCMBaseGrid
{
// Construction
public:
	CCMSectItemPSCnCellDownGrid();
	BOOL m_bInitDone;
	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemPSCnCellDownGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCMSectItemPSCnCellDownGrid();
	void InitGrid();
	void SetDataSource(T_SECT_D* pSect,CWnd* pParent);
	void ShowData();
	BOOL SaveData();
	void OnSizeChange();
	void SetLeftRight(BOOL nLeftRight){m_nLeftRight=nLeftRight;}

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemPSCnCellDownGrid)
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
	void ChangeStatusLeft(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole);
	void ChangeStatusRight(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole);
	void ChangeStatus();
	void StatusReset();
	BOOL VerifyPasteRange(CGXRange& from,CGXRange& to);
	int  GetSelectedRowsCount(int top,int bottom);
	int  GetSelectedColsCount(int left,int right);
protected:
	T_SECT_D* m_pSect;
	T_SECT_D_PSC_NCEL2 m_pnCell2;
	CWnd* m_pParent;
	int m_nLeftRight;
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

#endif // !defined(AFX_CMSECTITEMPSCNCELLDOWNGRID_H__D022AB02_90AE_4900_96BD_77AE48174EAF__INCLUDED_)
