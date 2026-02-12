#if !defined(__CMREBARCOMPOSITESTIFFDLG_H__)
#define __CMREBARCOMPOSITESTIFFDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageCompSectStiffDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeStiffDlg Grid
// CGXGridWnd Drived class

class CCMRebarCompositeStiffGrid : public CGXGridWnd
{
// Construction and Destruction
public:
	CCMRebarCompositeStiffGrid() { }

// Attributes
public:

// Operations
public:
	virtual BOOL ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt=1, UINT flags=0);
	//virtual BOOL CanPaste() { return FALSE; } // 2002. 12. 03 Copy 되도록 수정 
	virtual BOOL CanCut() { return FALSE; }  
	virtual BOOL OnPasteFromClipboard(const CGXRange &range);

	virtual BOOL Copy( );


// Implementations
	BOOL TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);
	BOOL SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl);

	BOOL SetLeftValidCell(ROWCOL nRow, ROWCOL nCol);
	BOOL SetRightValidCell(ROWCOL nRow, ROWCOL nCol);
	CString GetEditingValue(ROWCOL nRow, ROWCOL nCol);
};

/////////////////////////////////////////////////////////////////////////////
//
// CCMRebarCompositeStiffDlg dialog

class CCMRebarCompositeStiffDlg : public CDialogMove
{
// Construction
public:
	CCMRebarCompositeStiffDlg(CDBDoc* pDoc, CArray<T_SECT_SECTBASE_D, T_SECT_SECTBASE_D&>* pData,
														CStringArray* pTitle, BOOL bReadOnly=FALSE, CWnd* pParent=NULL);
// Attributes
public:

// Dialog Data
	//{{AFX_DATA(CCMRebarCompositeStiffDlg)
	enum { IDD = IDD_CMD_SP_STIFF_DLG_COMPO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarCompositeStiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CCMRebarCompositeStiffGrid m_wndGrid; 

	CDBDoc* m_pDoc;
	CArray<T_SECT_SECTBASE_D, T_SECT_SECTBASE_D&>* m_pData;
	CStringArray* m_pTitle;
	
	int m_nColSize;
	BOOL m_bReadOnly;

// Implementation
protected:
	void SetTitleInGrid();
	void SetUnitInGrid();
	void SetStiffnessData();
	void SetStiffnessData(ROWCOL nCol, T_SECT_SECTBASE_D* pStiffness);
	void SetDialogSize();
	void SetGridReadOnly(ROWCOL nFromCol, ROWCOL nToCol);
public:

	// Generated message map functions
	//{{AFX_MSG(CCMRebarCompositeStiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSpSdBtnOk();
	afx_msg void OnCmdSpSdBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARCOMPOSITESTIFFDLG_H__)
