#if !defined(AFX_CMSECTITEMGRID_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_)
#define AFX_CMSECTITEMGRID_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSectItemGrid.h : header file
//

#include "..\wg_db\wg_db_DBLib.h"

class CCMSecViewWnd;
/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid window

class CCMSectItemGrid : public CGXGridWnd
{
// Construction
public:
	CCMSectItemGrid();
	virtual ~CCMSectItemGrid();
	BOOL m_bInitDone;
	static UINT WM_GRID_DATA_CHANGED;

//----------------------------------------------------------------------
// Attributes
public:

//----------------------------------------------------------------------
// Operations
public:
	void InitGrid(CCMSecViewWnd* pPreview);
	void SetDataSource(T_SECT_SECTBASE_D* pSectBase, T_SECP_D *pPSCDesign, int nSectType, int *pCellNum);
	// 20080226 mylee - Composite General 형태로 초기화
	void SetDataSource2(T_SECT_STIFFNESS* pStiff, double* pPeriOut, double* pPeriIn, double* pYBar, double* pZBar, 
		T_SECT_SECTBASE_D* pSectBase, int nSectType, int *pCellNum);
	void ChangeSectType(int nSectType); // 20080104 mylee
	void ChangeShape(int nNewShapeIndex);
	void SetWarpingEffect(BOOL bConsiderWarpingEffect);
	void SetDefineValue(int nSectType);
	//void SetPSCDesign(BOOL bPSCDesign);
	void ShowData();
	BOOL SaveData();
	BOOL SaveSize();
	BOOL SaveStiffness();
	BOOL GetPeriAndStiff();
	void SetTapered(BOOL tapered, int iOrj=0);//set tapered
	BOOL IsTapered();//check whether section is tapered or not


//----------------------------------------------------------------------
// Implementation
protected:
	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	void OnSizeChange();
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);

	BOOL GetDoubleValue(CString& csNum, double& dNum);
	BOOL IsValidDoubleNumber(CString value, int &nErrCode);
	BOOL ConvertFormulaToValue(CString& value);
	BOOL IsNumeric(CString str, int& ecode);
	BOOL InvalidDoubleNumber(int nErrCode);
	BOOL ValidateError(LPCTSTR lpszError);
	void SetStyleOnSizeCell();

protected:
	CCMSecViewWnd* m_pPreview;
	T_SECT_SECTBASE_D* m_pSectBase;
	T_SECP_D *m_pPSCDesign;
	BOOL m_bConsiderWarpingEffect;
	//BOOL m_bPSCDesign;
	int m_nSectType;
	int m_nShapeIndex;
	int *m_pCellNumber;
	int m_iOrj;
	BOOL m_tapered;
	// Composite General 타입을 위해 추가한 다섯 개 변수. 
	// Compo_Gen 일 경우는 aGeneral Part 의 값을 참조하고
	// 이외의 타입을 경우는 결국 m_pSectBase 안의 값을 참조한다. 
	T_SECT_STIFFNESS* m_pStiff;
	double* m_pPeriOut;
	double* m_pPeriIn;
	double* m_pYBar;
	double* m_pZBar;

	int m_STIFF_MAX_NUM;
	int m_TOTAL_ROW_NUM;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectItemGrid)
	//}}AFX_VIRTUAL
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	virtual BOOL OnValidateCell(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnActivateGrid(BOOL bActivate);
	virtual void OnModifyCell(ROWCOL nRow, ROWCOL nCol);
	virtual void OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnEndEditing(ROWCOL nRow, ROWCOL nCol);
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);
// Implementation
public:
	T_SECT_K m_SectKOrg;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCMSectItemGrid)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSECTITEMGRID_H__07084701_849A_11D4_92DF_00010263A1CC__INCLUDED_)
