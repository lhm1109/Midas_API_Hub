#if !defined(AFX_BNDRRIGDDLG_H__66BE7324_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRRIGDDLG_H__66BE7324_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrRigdDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\mit_frx\MButton.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrRigdDlg dialog

class CBndrRigdDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrRigdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
	virtual BOOL ExternalInit(UINT key);
	virtual BOOL ExternalInit_by_SelectLabel(UINT key);
// Dialog Data
	//{{AFX_DATA(CBndrRigdDlg)
	enum { IDD = IDD_TM_BNDR_RIGD };
	CTextUnit	m_wndDistUnit;
	mit::frx::MEdit	m_wndDistEdit;
	mit::frx::MButton	m_wndCopyLoad;
	mit::frx::MButton m_wndReleaseSlaveNode;
	CMouseEdit	m_wndMasterNode;
	int		m_nOption;
	CString	m_csMasterNode;
	CCobxBngr	m_wndGroupCombo;
	//}}AFX_DATA
	BOOL    m_bDOF[6];
	mit::frx::MillustViewer m_wndPicture;

	CArray<UINT, UINT> m_aCtrlOption;
	CArray<UINT, UINT> m_aCtrlDisable;
	CArray<UINT, UINT> m_aCtrlCopyLoad;
	CArray<UINT, UINT> m_aCtrlAxis;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrRigdDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void ShowHideStartLinkNumCtrl();

	// Generated message map functions
	//{{AFX_MSG(CBndrRigdDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmReleaseSlaveNode();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnBtnRigidBody();
	afx_msg void OnBtnPlaneXY();
	afx_msg void OnBtnPlaneXZ();
	afx_msg void OnBtnPlaneYZ();
	afx_msg void OnChangeOption();
	afx_msg void OnTmCopyLoad();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnViewStartLinkNum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRRIGDDLG_H__66BE7324_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
