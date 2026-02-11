#if !defined(__CMSECTITEMGRID4CI_H__)
#define __CMSECTITEMGRID4CI_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemGrid4CI.h : header file
//
#include "..\wg_db\wg_db_DBLib.h"

class CCMSecViewWnd;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4CI window

class CCMSectItemGrid4CI : public CGXGridWnd
{
// Construction
public:
	CCMSectItemGrid4CI();
	virtual ~CCMSectItemGrid4CI();
	BOOL m_bInit;

//----------------------------------------------------------------------
// Attributes
public:

//----------------------------------------------------------------------
// Operations
public:
	void InitGrid(CCMSecViewWnd* pPreview);
	void SetDataSource(T_SECT_D* pSectD);
	void ChangeShape(int nNewShapeIndex);
	void ShowData();
	BOOL SaveData();
	BOOL SaveFlag(BOOL bExcludeJoint=FALSE);
	BOOL SaveSize();

protected:
	void SetViewData();

//----------------------------------------------------------------------
// Implementation
protected:
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
	void SetColumnStatus();
	void SetColumnStatusSetHideFlag(CArray<BOOL, BOOL>& raHideFlag, int nCount, CArray<int, int>& raType, CArray<int, int>& raBegin, CArray<int, int>& raEnd);
	void SetColumnStatusGetStatus(CArray<BOOL, BOOL>& raHideJoint, CArray<BOOL, BOOL>& raHideSize);
	void SetColumnStatusShowHide(ROWCOL nRow, BOOL bHide);
	BOOL IsEditing();
	void CancelEditing();
	CGXControl* GetEditingControl();

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL ConvertFormulaToValue(CString& value);

protected:
	CCMSecViewWnd* m_pPreview;
	T_SECT_D* m_pSectD;
	int m_nShapeIndex;

	int m_nNumSymmetry;
	int m_nNumCellType;
	CString m_csNameCellType; CString m_csValueCellType;
	int m_nNumCellShape; 
	CString m_csNameCellShape; CString m_csValueCellShape;
	int m_nNumJoint;
	int m_nNumSize;
	int m_nJointType; // In/Out, Left/Right
	int m_nJointNum0; // Single(J1)
	int m_nJointNum1; // Out or Left
	int m_nJointNum2; // In or Right
	int m_nJoint;
	int m_nSizeNum0;
	int m_nSizeNum1;
	int m_nSizeNum2;

	int m_nSymmetryRow;
	int m_nCellTypeRow;
	int m_nCellShapeRow;
	int m_nJointRow;
	int m_nSizeIRow;

	BOOL m_bSymmetry;
	BOOL m_bValidateFail;
	BOOL m_bOnStartEditing;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemGrid4CI)
	//}}AFX_VIRTUAL
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnStartEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
	virtual void OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol);
// Implementation
public:
	int GetJointList(CStringArray& raJointList);
	int GetSizeList(CStringArray& raSizeList);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemGrid4CI)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMGRID4CI_H__)
