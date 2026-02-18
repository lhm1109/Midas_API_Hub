#if !defined(AFX_DgnStlStreJPDlg_H__)
#define AFX_DgnStlStreJPDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlStreJPDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlStreJPDlg dialog

class CDgnStlStreJPDlg : public CMenuBarChildDlg
{
	// Construction
public:
	CDgnStlStreJPDlg(CWnd* pParent = NULL);   // standard constructor
	//virtual void Execute() {}

	// Dialog Data
		//{{AFX_DATA(CDgnStlStreJPDlg)
	enum { IDD = IDD_DGN_STL_JP_STRE_DLG};

	int	m_nOption;

	int	m_nGussetPlane;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlStreJPDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void EnableDisableControls();

	// Generated message map functions
	//{{AFX_MSG(CDgnStlStreJPDlg)
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

#endif // !defined(AFX_DgnStlStreJPDlg_H__)
