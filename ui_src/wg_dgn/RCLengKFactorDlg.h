#if !defined(AFX_DGNRCLENGKFACTORDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNRCLENGKFACTORDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RCLengKFactorDlg.h : header file
//

#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"

#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\wg_baseMouseEdit.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CRCLengKFactorDlg dialog

class CRCLengKFactorDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CRCLengKFactorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CRCLengKFactorDlg)
	enum { IDD = IDD_DGN_RC_CH_LENG_K_DLG };
	CMouseEdit	m_BLzCtrl;
	CMouseEdit	m_BLyCtrl;
	CTextUnit	m_BLzUnit;
	CTextUnit	m_BLyUnit;
	int		m_nOption;
	double	m_BLy;
	double	m_BLz;
	double	m_BKy;
	double	m_BKz;
	
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode();

public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRCLengKFactorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CRCLengKFactorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnExecute();
	afx_msg void OnDgnClose();
	afx_msg void OnDgnLengthAr();
	afx_msg void OnDgnLengthDel();
	afx_msg void OnDgnKfactorKyinput();
	afx_msg void OnDgnKfactorKzinput();
	afx_msg void OnDgnKfactorAr();
	afx_msg void OnDgnKfactorDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNRCLENGKFACTORDLG_H__147352C9_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
