#if !defined(AFX_BNDR_IELC_DLG_H__)
#define AFX_BNDR_IELC_DLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrIelcDlg.h : header file
//
#include "..\wg_db\SelectLC.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrIelcDlg dialog

class CBndrIelcDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrIelcDlg(CWnd* pParent = NULL);   // standard constructor
//----------------------------------------------------------------
// Overridables
public:
	virtual void Execute();

//----------------------------------------------------------------
// Implementations
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlOp;

// Dialog Data
	//{{AFX_DATA(CBndrIelcDlg)
	enum { IDD = IDD_TM_BNDR_IELC };
	CSelectLC	m_LoadCase;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrIelcDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBndrIelcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmIelcBtnLc();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDR_IELC_DLG_H__)
