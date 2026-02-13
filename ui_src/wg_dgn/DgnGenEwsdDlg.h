#if !defined(AFX_DGNGENEWSDDLG_H__)
#define AFX_DGNGENEWSDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenEwsdDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenEwsdDlg dialog

class CDBDoc;

class CDgnGenEwsdDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnGenEwsdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnGenEwsdDlg)
	enum { IDD = IDD_DGN_GEN_EWSD_DLG };
	int		m_nOption;
	BOOL		m_bJCheck;
	double	m_dISbz;
	double	m_dIZTop;
	double	m_dIZBot;
	double	m_dJSbz;
	double	m_dJZTop;
	double	m_dJZBot;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenEwsdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenEwsdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnEwsdAr();
	afx_msg void OnDgnEwsdDel();
	afx_msg void OnDgnJClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENEWSDDLG_H__)
