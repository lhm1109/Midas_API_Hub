#if !defined(AFX_BNDROFFSDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_BNDROFFSDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BndrOffsDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxBngr.h"
#include "..\MIT_frx\MillustViewer.h"

/////////////////////////////////////////////////////////////////////////////
// CBndrOffsDlg dialog

class CBndrOffsDlg : public CMenuBarChildDlg
{
// Construction
public:
	CBndrOffsDlg(CWnd* pParent = NULL);   // standard constructor
	~CBndrOffsDlg();
	virtual void Execute();
	virtual void PostNcDestroy() { m_wndPicture.CloseDialog(); CMenuBarChildDlg::PostNcDestroy(); }
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CBndrOffsDlg)
	enum { IDD = IDD_TM_BNDR_OFFS };
	mit::frx::MComboBox	m_Type;
	int		m_nOption;
	CCobxBngr	m_wndGroupCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBndrOffsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	CBCGPStatic   m_Text[6];
	CEditUnit m_Value[6];
	CTextUnit m_Unit[6];

	mit::frx::MillustViewer m_wndPicture;

// Implementation
protected:
	CDBDoc* m_pDoc;
	CBitmap*  m_pBitmap;
	void EnableValueEdit(BOOL bEnable);
	void ChangeType(int nType);
	void ChangeBitMap();

	// Generated message map functions
	//{{AFX_MSG(CBndrOffsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnChangeOption();
	afx_msg void OnChangeType();
	afx_msg void OnTmDefineGroupButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BNDROFFSDLG_H__66BE7323_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
