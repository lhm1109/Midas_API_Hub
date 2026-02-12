#if !defined(__CMSectItemGrid4Stlg_H__)
#define __CMSectItemGrid4Stlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemGrid4Stlg.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

class CCMSectItemTap;
class CCMSecViewWnd;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Stlg window

class CCMSectItemGrid4Stlg : public CGXGridWnd
{
// Construction
public:
	CCMSectItemGrid4Stlg();
	virtual ~CCMSectItemGrid4Stlg();
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
	void ShowData();

	void ShowDataStlGirderBoxI( int n, int nRow, CStringArray& aSizeList, int nPlace, CString csUnit, double * pSize );
	void ShowDataStlGirderMCell( int n, int nRow, CStringArray& aSizeList, int nPlace, CString csUnit, double * pSize );

	BOOL SaveData();
	void SetSizeAuto();
	BOOL SaveFlag();
	BOOL SaveSize();
	void SaveSizeStlGirderBoxI(int nSizeRow, int n, double* pSize );
	void SaveSizeStlGirderMCell(int nSizeRow, int n, double* pSize );
	
protected:

	void SetColumnStatus();
	void SetViewData();

//----------------------------------------------------------------------
// Implementation
protected:
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);  
	void SetColumnStatusSetHideFlag(CArray<BOOL, BOOL>& raHideFlag, int nCount, CArray<int, int>& raType, CArray<int, int>& raBegin, CArray<int, int>& raEnd);  
	void SetColumnStatusGetStatus(CArray<BOOL, BOOL>& raHideSizeI, CArray<BOOL, BOOL>& raHideSizeJ);
	void SetColumnStatusShowHide(ROWCOL nRow, BOOL bHide);
	BOOL IsEditing();
	void CancelEditing();
	CGXControl* GetEditingControl();

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL GetIntValue(CString& csNum, int& nNum);
	BOOL ConvertFormulaToValue(CString& value);
	void StlGSectSyAutoCalc(double* pSize, BOOL bJ);
protected:
	CCMSecViewWnd* m_pPreview;
	CCMSectItemTap* m_pParent;
	T_SECT_D* m_pSectD;
	int m_nShapeIndex; // 0:B, 1:I
		
	int m_nNumSize;
	int m_nJoint;
	
	int m_nNumStiffener2;// new	- v831 에 들어간 Stiffener
	int m_nNumRivet;

	int m_nSizeIRow;    // used
	int m_nSizeJRow;    // used

	int m_nStiffenerRow2;  // new	- v831 에 들어간 Stiffener
	int m_nRivetRow;
	int m_nSymmetryStlGSectI;  // Stl Girder 에서 사용
	int m_nSymmetryStlGSectJ;  // Stl Girder 에서 사용
		
	BOOL m_bValidateFail;
	BOOL m_bOnStartEditing;
	BOOL m_bSymmetryStlGSectI;  // Stl Girder 에서 사용
	BOOL m_bSymmetryStlGSectJ;  // Stl Girder 에서 사용

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemGrid4Stlg)
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
	int GetSizeList(CStringArray& raSizeList);
	int GetUnitList(CStringArray& raUnitList);
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemGrid4Stlg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMSectItemGrid4Stlg_H__)

