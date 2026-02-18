#if !defined(AFX_DGNSTLUCFDLG_H__E2B7DC41_E2B3_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSTLUCFDLG_H__E2B7DC41_E2B3_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlUcfDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlUcfDlg dialog

class CDgnStlUcfDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnStlUcfDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnStlUcfDlg)
	enum { IDD = IDD_DGN_STL_UCF_DLG };
	int		m_nOption;
	double	m_Ucf;
	//}}AFX_DATA

public:


public:
	void Initial_Data();
	void Initial_SelectItem();

	BOOL ErrorCheck();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlUcfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlUcfDlg)
	afx_msg void OnDgnCbAr();
	afx_msg void OnDgnCbDel();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLUCFDLG_H__E2B7DC41_E2B3_11D3_888F_0000C0F30D4D__INCLUDED_)
