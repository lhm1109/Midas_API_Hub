#if !defined(AFX_BNDREWSFDLG_H__66BE7322_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDREWSFDLG_H__66BE7322_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrEwsfDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\mit_frx\MillustViewer.h"
/////////////////////////////////////////////////////////////////////////////
// CBndrEwsfDlg dialog

class CBndrEwsfDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrEwsfDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CBndrEwsfDlg)
	enum { IDD = IDD_TM_BNDR_EWSF };
	int		m_nOption;
	CCobxBngr	m_wndGroupCombo;
	CEditUnit m_wndScaleFactor;
	CEditUnit m_ZTopEdt;
	CEditUnit m_ZBotEdt;
	CEditUnit m_wndScaleFactor_j;
	CEditUnit m_ZTopEdt_j;
	CEditUnit m_ZBotEdt_j;
	BOOL      m_bJ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrEwsfDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT, UINT> m_aCtrlScaleFactor_i, m_aCtrlScaleFactor_j;
	mit::frx::MillustViewer m_wndPicture;
	// Generated message map functions
	//{{AFX_MSG(CBndrEwsfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnChangeOption();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnCheckJ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDREWSFDLG_H__66BE7322_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
