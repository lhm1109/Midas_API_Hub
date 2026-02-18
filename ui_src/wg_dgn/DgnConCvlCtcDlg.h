#if !defined(AFX_DGNCONCVLCTCDLG_H__0FCF365A_D4AD_458E_BEC1_6AB7321DE5A7__INCLUDED_)
#define AFX_DGNCONCVLCTCDLG_H__0FCF365A_D4AD_458E_BEC1_6AB7321DE5A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConCvlCtcDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnConCvlCtcDlg dialog

class CDgnConCvlCtcDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnConCvlCtcDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnConCvlCtcDlg)
	enum { IDD = IDD_DGN_CON_CVL_CTC_DLG };
	int		m_nOption;
	double	m_dCtcUpFactor;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConCvlCtcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConCvlCtcDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCtcAr();
	afx_msg void OnDgnCtcDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONCVLCTCDLG_H__0FCF365A_D4AD_458E_BEC1_6AB7321DE5A7__INCLUDED_)
