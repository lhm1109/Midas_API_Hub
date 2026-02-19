#if !defined(AFX_DGNSTLCBDLG_H__147352C2_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSTLCBDLG_H__147352C2_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlCbDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnStlCbDlg dialog

class CDgnStlCbDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnStlCbDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnStlCbDlg)
	enum { IDD = IDD_DGN_STL_CB_DLG };
	int		m_nOption;
	double	m_Cb;
	BOOL	m_bAutoCb;
	//}}AFX_DATA

public:


public:
	void Initial_SelectItem();
	void Initial_Data();
	// Add by ZINU.('01.1.3).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlCbDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnStlCbDlg)
	virtual void Execute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnCbAr();
	afx_msg void OnDgnCbDel();
	afx_msg void OnDgnCbcalProgram();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLCBDLG_H__147352C2_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
