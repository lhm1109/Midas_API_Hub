#if !defined(AFX_HYDRTEMPERDLG_H__F6FE2A42_17B1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HYDRTEMPERDLG_H__F6FE2A42_17B1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HydrTemperDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"

/////////////////////////////////////////////////////////////////////////////
// CHydrTemperDlg dialog

class CHydrTemperDlg : public CMenuBarChildDlg
{
// Construction
public:
	CHydrTemperDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CHydrTemperDlg)
	enum { IDD = IDD_TM_HYDR_TEMPER };
	CTextUnit	m_temperunit;
	CEditUnit	m_temper;
	CCobxBngr	m_wndGroupCombo;
	int		m_option;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHydrTemperDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_group;

	// Generated message map functions
	//{{AFX_MSG(CHydrTemperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmChangeOption();
	afx_msg void OnTmDefineGroupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYDRTEMPERDLG_H__F6FE2A42_17B1_11D5_880F_00010263A1F4__INCLUDED_)
