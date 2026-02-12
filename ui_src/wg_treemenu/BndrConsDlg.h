#if !defined(AFX_BNDRCONSDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDRCONSDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrConsDlg.h : header file
//

#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\mit_frx\MillustViewer.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CBndrConsDlg dialog

class CBndrConsDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrConsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	virtual void PostNcDestroy() {	m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy();	}
	void    SetModifyData(T_CONS_D data);

public:
	virtual BOOL ExternalInit(UINT key);
	virtual BOOL ExternalInit_by_SelectLabel(UINT key);

// Dialog Data
	//{{AFX_DATA(CBndrConsDlg)
	enum { IDD = IDD_TM_BNDR_CONS };
	CCobxBngr	m_wndGroupCombo;
	int		m_nOption;
	//}}AFX_DATA
	BOOL	m_FlagAll[2];
	BOOL  m_Flag[7];
	mit::frx::MillustViewer m_wndPicture;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrConsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*   m_pDoc;
	void EnableValueEdit(BOOL bEnable);
	void SetControl_7thDOF();
	void MoveControls(int dDy);
	void ResizeControls(int dDy);

	// Generated message map functions
	//{{AFX_MSG(CBndrConsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnToggleFlag();
	afx_msg void OnTmDefineGroupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDRCONSDLG_H__C4F5F3A2_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
