// Add by sshan. MNET:2607.('20061220)

#if !defined(AFX_RCCRACKWIDTH_H__)
#define AFX_RCCRACKWIDTH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCCrackWidth.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CRCCrackWidth dialog

class CRCCrackWidth : public CMenuBarChildDlg
{
// Construction
public:
	CRCCrackWidth(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CRCCrackWidth)
	enum { IDD = IDD_DGN_RC_CRACK_WIDTH };
	int		m_nOption;
	double	m_dC1;
	double	m_dC2;
	double	m_dC3;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();

	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCCrackWidth)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void SetControl();
	CString GetRCCHCodeName();
	// Generated message map functions
	//{{AFX_MSG(CRCCrackWidth)
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

#endif // !defined(AFX_RCCRACKWIDTH_H__)
