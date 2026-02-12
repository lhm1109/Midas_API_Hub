#if !defined(AFX_CMSECTITEMGRID4PSCNEW_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_)
#define AFX_CMSECTITEMGRID4PSCNEW_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemGrid4PscNew.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

class CCMSectItemTap;
class CCMSecViewWnd;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4PscNew window

class CCMSectItemGrid4PscNew : public CGXGridWnd
{
// Construction
public:
	CCMSectItemGrid4PscNew();
	virtual ~CCMSectItemGrid4PscNew();
	BOOL m_bInit;

//----------------------------------------------------------------------
// Attributes
public:

//----------------------------------------------------------------------
// Operations
public:
	void InitGrid(CCMSecViewWnd* pPreview);
	void SetDataSource(T_SECT_D* pSectD, CCMSectItemTap* pParent);
	void ChangeShape(int nNewShapeIndex);
	void SetWarpingEffect(BOOL bConsiderWarpingEffect);
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

	void OnCalculateButton_I();
	void OnCalculateButton_J();

protected:
	BOOL m_bConsiderWarpingEffect;
	CCMSecViewWnd* m_pPreview;
	CCMSectItemTap* m_pParent;
	T_SECT_D* m_pSectD;
	int m_nShapeIndex;

	int m_nNumSymmetry;
	int m_nNumCellType; int m_nKindCellType; 
	CString m_csNameCellType; CString m_csValueCellType;
	int m_nNumCellShape; int m_nKindCellShape; 
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
	int m_nNumSitff;
	int m_nNumHunch;
	int m_nNumCMPWEB;
	int m_nNumSitff_Warping;

	int m_nSymmetryRow;
	int m_nCellTypeRow;
	int m_nCellShapeRow;
	int m_nJointRow;
	int m_nSizeIRow;
	int m_nSizeJRow;
	int m_nStiffIRow;
	int m_nStiffJRow;
	int m_nHunchRow;
	int m_nCMPWebRowI;
	int m_nCMPWebRowJ;
	int m_nStiffWarpingIRow;
	int m_nStiffWarpingJRow;

	int m_nNumSideHole;
	int m_nSideHoleRow;
	
	int m_nNumShearCheck;
	int m_nShearOptionRow;
	int m_nAutoShearCheckRow[2];
	int m_nShearCheckRow[2];
	int m_nNumMinWebThik;
	int m_nAutoMinWebThikRow[2];
	int m_nMinWebThikRow[2];
	int m_nNumShearQy;
	int m_nAutoShearQyRow[2];
	int m_nShearQyRow[2];

	int m_nRowCal_I;
	int m_nRowCal_J;

	BOOL m_bSymmetry;
	BOOL m_bHunch;
	BOOL m_bValidateFail;
	BOOL m_bOnStartEditing;
	BOOL m_bUseShearCheck;
	BOOL m_bUseAutoShearCheck;
	BOOL m_bUseAutoTorsionCheck;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemGrid4PscNew)
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
	void SetCobxColumn(CGXRange& rRange, LPCTSTR lpszChoiceList);
	CString CCMSectItemGrid4PscNew::GetCListCellType(int nKindCellType);
	CString CCMSectItemGrid4PscNew::GetCListCellShape(int nKindCellShape);
	int GetJointList(CStringArray& raJointList);
	int GetSizeList(CStringArray& raSizeList);
	int GetUnitList(CStringArray& raUnitList);
	
	int  GetStiffList(int n, CArray<double, double> &raStiff);
	void SetStiffList(int n, CArray<double, double> &raStiff);

	int  GetStiffWarpingList(int n, CArray<double, double> &raStiff);
	void SetStiffWarpingList(int n, CArray<double, double> &raStiff);

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemGrid4PscNew)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMGRID4PSCNEW_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_)

