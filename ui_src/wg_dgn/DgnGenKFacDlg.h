#if !defined(AFX_DGNGENKFACDLG_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENKFACDLG_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenKFacDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnGenKFacDlg dialog

class CDBDoc;

class CDgnGenKFacDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnGenKFacDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnGenKFacDlg)
	enum { IDD = IDD_DGN_GEN_K_DLG };
	int		m_nOption;
	double	m_BKy;
	double	m_BKz;
	double m_BKt;

	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	// Add by ZINU.('02.3.21).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenKFacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	// Generated message map functions
	//{{AFX_MSG(CDgnGenKFacDlg)
	virtual BOOL OnInitDialog();
	virtual void Execute();
	afx_msg void OnDgnKfactorKyinput();
	afx_msg void OnDgnKfactorKzinput();
	afx_msg void OnDgnKfactorAr();
	afx_msg void OnDgnKfactorDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENKFACDLG_H__147352C7_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
