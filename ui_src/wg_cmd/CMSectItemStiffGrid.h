#if !defined(__CMSECTITEMSTIFFGRID_H__)
#define __CMSECTITEMSTIFFGRID_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemStiffGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\CMBaseGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStiffGrid window
class CCMSectItemPSCValue;
class CCMSectItemStiffGrid : public CCMBaseGrid
{
// Construction
public:
	CCMSectItemStiffGrid();
	virtual ~CCMSectItemStiffGrid();
	BOOL m_bInitDone;

//----------------------------------------------------------------------
// Attributes
public:

//----------------------------------------------------------------------
// Operations
public:
	void InitGrid();
	void SetDataSource(CWnd* pParent,T_SECT_SECTBASE_D* pSectBase);
	void SetWarpingEffect(BOOL bConsiderWarpingEffect);
	void ShowData();
	BOOL SaveData();

	void OnSizeChange();

//----------------------------------------------------------------------
// Implementation
protected:
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL InvalidDoubleNumber(int nErrCode);
	BOOL ValidateError(LPCTSTR lpszError);
	void SetStyleOnSizeCell();
	void OnCalculateButton();
	void OnDisplayCentroldButton();

protected:
	T_SECT_SECTBASE_D* m_pSectBase;
	CCMSectItemPSCValue* m_pParent;
	BOOL m_bConsiderWarpingEffect;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemStiffGrid)
	//}}AFX_VIRTUAL
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual void OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol);
// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemStiffGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMSTIFFGRID_H__)
