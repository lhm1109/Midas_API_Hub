#if !defined(AFX_BNDRFRLSDLG_H__66BE7322_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRFRLSDLG_H__66BE7322_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrFrlsDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MillustViewer.h"
#include "..\MIT_frx\MButton.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrFrlsDlg dialog

class CBndrFrlsDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrFrlsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
	virtual BOOL ExternalInit(UINT key);
// Dialog Data
	//{{AFX_DATA(CBndrFrlsDlg)
	enum { IDD = IDD_TM_BNDR_FRLS };
	CTextUnit	m_textMxyzUnit;
	CTextUnit	m_textFxyzUnit;
	int		m_nOption;
	CCobxBngr	m_wndGroupCombo;
	//}}AFX_DATA
	CEditUnit m_Value[2][FRLS_SIZE];  // i, j
	mit::frx::MButton   m_Flag[2][FRLS_SIZE];

	mit::frx::MillustViewer m_wndPicture;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrFrlsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	void EnableValueEdit(BOOL bEnable);
	void EnableFlag(BOOL bEnableI, BOOL bEnableJ);
	void EnableValue(BOOL bEnableI, BOOL bEnableJ);
	void SetControl_7thDOF();
	void MoveControls(int dDy);
	void ResizeControls(int dDy);
	CArray<UINT, UINT> m_aTypeCtrl;

	// Generated message map functions
	//{{AFX_MSG(CBndrFrlsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnToggleFlag();
	afx_msg void OnClickTypicalBtn();
	afx_msg void OnTmDefineGroupButton();
	afx_msg void OnTmChangeType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRFRLSDLG_H__66BE7322_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
