#if !defined(AFX_DGNGENSLENDLG_H__F6014782_E201_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENSLENDLG_H__F6014782_E201_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenSlenDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenSlenDlg dialog

class CDgnGenSlenDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenSlenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenSlenDlg)
	enum { IDD = IDD_DGN_GEN_SLEN_DLG };
	double	m_CLMT;
	double	m_TLMT;
	BOOL	m_NotCheck;
	int		m_nOption;
	//}}AFX_DATA

public:
	void Initial_Data();
	void Initial_SelectItem();

	BOOL ErrorCheck();
	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();
	void Update_DataByCode();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenSlenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenSlenDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnSlenderAr();
	afx_msg void OnDgnSlenderDel();
	afx_msg void OnDgnSlenderNocheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENSLENDLG_H__F6014782_E201_11D3_888F_0000C0F30D4D__INCLUDED_)
