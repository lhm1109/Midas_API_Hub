#if !defined(AFX_BNDRPRLSDLG_H__7B79DFC1_36CC_11D4_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRPRLSDLG_H__7B79DFC1_36CC_11D4_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrPrlsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBndrPrlsDlg dialog
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MillustViewer.h"

class CBndrPrlsDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrPrlsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnTmExecute(); }
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
	virtual BOOL ExternalInit(UINT key);

protected:
	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlDisable;

	mit::frx::MillustViewer m_wndPicture;

public:

// Dialog Data
	//{{AFX_DATA(CBndrPrlsDlg)
	enum { IDD = IDD_TM_BNDR_PRLS };
	CCobxBngr	m_wndGroupCombo;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrPrlsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
		void CtrlSetCheck(CWnd* pParent, CArray<UINT,UINT> &aCtrlID, int nCheck);

	// Generated message map functions
	//{{AFX_MSG(CBndrPrlsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmClose();
	afx_msg void OnTmExecute();
	afx_msg void OnTmAllpin();
	afx_msg void OnTmAllfix();
	afx_msg void OnTmOptDelete();
	afx_msg void OnTmOptAdd();
	afx_msg void OnTmDefineGroupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRPRLSDLG_H__7B79DFC1_36CC_11D4_92DE_0000C0B0E6B3__INCLUDED_)
