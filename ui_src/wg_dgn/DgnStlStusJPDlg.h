#if !defined(AFX_DgnStlStusJPDlg_H__)
#define AFX_DgnStlStusJPDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlStusJPDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStusJPDlg dialog

class CDgnStlStusJPDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CDgnStlStusJPDlg(CWnd* pParent = NULL);   // standard constructor
	//virtual void Execute() {}

	// Dialog Data
		//{{AFX_DATA(CDgnStlStusJPDlg)
	enum { IDD = IDD_DGN_STL_JP_STUS_DLG};

	int	m_nOption;

	BOOL m_bUpperLimitLB;
	BOOL m_bUpperLimitBC;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlStusJPDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void EnableDisableControls();

	// Generated message map functions
	//{{AFX_MSG(CDgnStlStusJPDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
// 	afx_msg void OnDgnExecute();
// 	afx_msg void OnDgnClose();
	afx_msg void OnDgnArDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DgnStlStusJPDlg_H__)
