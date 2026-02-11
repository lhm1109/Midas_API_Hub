#if !defined(__CMIepiItemDlg_H__)
#define __CMIepiItemDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMIepiItemDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "CMChildBarBase.h"
#include "..\wg_db\SelectLC.h"


/////////////////////////////////////////////////////////////////////////////
// CCMIepiItemDlg dialog

class CCMIepiItemDlg  : public CCMChildBarBase
{
public:
	CCMIepiItemDlg();           // protected constructor used by dynamic creation
	// DECLARE_DYNCREATE(CCMIepiItemDlg)

	// Dialog Data
	//{{AFX_DATA(CCMIepiItemDlg)
	enum { IDD = IDD_CMD_IEPI };
	CSelectLC	m_LoadCase;
	BOOL m_bImportSTLoad;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMIepiItemDlg)	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMIepiItemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void Execute();
	afx_msg BOOL DoImportSTLoad();
	afx_msg void OnSelectModeRdo();
	afx_msg void OnIelcDlgBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int m_nSelectMode; // D_SELECT_MODE
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlOp;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMIepiItemDlg_H__)
