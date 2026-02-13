#if !defined(AFX_DGNPSCCRACKWIDTHDLG_H__)
#define AFX_DGNPSCCRACKWIDTHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnPscCrackWidthDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnPscCrackWidthDlg dialog

class CDgnPscCrackWidthDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnPscCrackWidthDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnPscCrackWidthDlg)
	enum { IDD = IDD_DGN_PSC_CRACK_WIDTH_DLG };
	int		m_nOption;
	double	m_dC1;
	double	m_dC2;
	double	m_dC3;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnPscCrackWidthDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnPscCrackWidthDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCrackAr();
	afx_msg void OnDgnCrackDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNPSCCRACKWIDTHDLG_H__)
