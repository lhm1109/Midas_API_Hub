#if !defined(__CMSECTITEMGRID4COMPOSITE_H__)
#define __CMSECTITEMGRID4COMPOSITE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemGrid4Composite.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

class CCMSectItemTap;
class CCMSecViewWnd;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Composite window

class CCMSectItemGrid4Composite : public CGXGridWnd
{
// Construction
public:
	CCMSectItemGrid4Composite();
	virtual ~CCMSectItemGrid4Composite();
	BOOL m_bInit;

//----------------------------------------------------------------------
// Attributes
public:

//----------------------------------------------------------------------
// Operations
public:
	void InitGrid(CCMSecViewWnd* pPreview);
	void SetDataSource(T_SECT_D* pSectD, CCMSectItemTap* pParent);  
	void ChangeShape(int nNewShapeIndex, BOOL bSlab);
	void ShowData();

	void ShowDataStlGirder( int n, int nRow, CStringArray& aSizeList, int nPlace, CString csUnit, double dSlabWidth, double * pSize );

	BOOL SaveData();
	BOOL SaveFlag(BOOL bExcludeJoint=FALSE);
	BOOL SaveSize();
	void SaveSizeStlGirder(int nSizeRow, int nNum, int n, double dNum, double* pSize );
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
	void SetColumnStatusGetStatus(CArray<BOOL, BOOL>& raHideJoint, CArray<BOOL, BOOL>& raHideSizeI, CArray<BOOL, BOOL>& raHideSizeJ);
	void SetColumnStatusShowHide(ROWCOL nRow, BOOL bHide);
	BOOL IsEditing();
	void CancelEditing();
	CGXControl* GetEditingControl();

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL GetIntValue(CString& csNum, int& nNum);
	BOOL ConvertFormulaToValue(CString& value);
	void CompStlGSectSyAutoCalc(double* pSize, BOOL bJ);
protected:
	CCMSecViewWnd* m_pPreview;
	CCMSectItemTap* m_pParent;
	T_SECT_D* m_pSectD;
	T_SECT_SECTION_D m_SectBefore;
	int m_nShapeIndex; // 0:B, 1:I, 2:CI, 3:CT, 6:Tub, 7:Stl Girder Box, 8:Stl Girder I, 9:Stl Girder Tub
										 // 100:1cell, 101:2cell, 102:3cell, 108:4cell, 109:multi-cell, 103:mid, 104:half, 105:tee, 106:plat, 107:I, 110:value, 111:CMPWEB
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
	int m_nNumGirder;   // new
	int m_nNumSlab;     // new
	int m_nNumStiffener;// new
	int m_nNumStiffener2;// new	- v831 에 들어간 Stiffener

	int m_nSymmetryRow; // used
	int m_nCellTypeRow;
	int m_nCellShapeRow;
	int m_nJointRow;    // used
	int m_nSizeIRow;    // used
	int m_nSizeJRow;    // used
	int m_nStiffIRow;
	int m_nStiffJRow;
	int m_nHunchRow;
	int m_nCMPWebRowI;
	int m_nCMPWebRowJ;
	int m_nGirderRow;     // new
	int m_nSlabRow;       // new
	int m_nStiffenerRow;  // new
	int m_nStiffenerRow_I; // new	- v831 에 들어간 Stiffener
	int m_nStiffenerRow_J; // new	- v831 에 들어간 Stiffener
	int m_nStiffenerRow2;  // new	- v831 에 들어간 Stiffener
	int m_nSymmetryStlGSectI;  // Stl Girder 에서 사용
	int m_nSymmetryStlGSectJ;  // Stl Girder 에서 사용

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

	BOOL m_bSymmetry;
	BOOL m_bHunch;
	BOOL m_bValidateFail;
	BOOL m_bOnStartEditing;
	BOOL m_bUseShearCheck;
	BOOL m_bUseAutoShearCheck;
	BOOL m_bUseAutoTorsionCheck;
	BOOL m_bSymmetryStlGSectI;  // Stl Girder 에서 사용
	BOOL m_bSymmetryStlGSectJ;  // Stl Girder 에서 사용

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemGrid4Composite)
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
	CString CCMSectItemGrid4Composite::GetCListCellType(int nKindCellType);
	CString CCMSectItemGrid4Composite::GetCListCellShape(int nKindCellShape);
	int GetGirderList(CStringArray& raGirderList, CStringArray& raGirderListU, CArray<double, double>& raGirderListV);
	int GetSlabList(CStringArray& raSlabList, CStringArray& raSlabListU, CArray<double, double>& raSlabListV);
	int GetStiffenerList(CStringArray& raStiffenerList, CStringArray& raStiffenerListU, CArray<double, double>& raStiffenerListV);
	int GetJointList(CStringArray& raJointList);
	int GetSizeList(CStringArray& raSizeList);
	int GetUnitList(CStringArray& raUnitList);
	
	int  GetStiffList(int n, CArray<double, double> &raStiff);
	void SetStiffList(int n, CArray<double, double> &raStiff);

	void GetSectBefore(T_SECT_SECTION_D& SectBefore);


	

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemGrid4Composite)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSECTITEMGRID4COMPOSITE_H__)

