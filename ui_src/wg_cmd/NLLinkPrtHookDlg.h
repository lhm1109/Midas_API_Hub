#if !defined(AFX_NLLINKPRTHOOKDLG_H__96D69B21_4718_4B29_8401_E0BC2FA8C87E__INCLUDED_)
#define AFX_NLLINKPRTHOOKDLG_H__96D69B21_4718_4B29_8401_E0BC2FA8C87E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtHookDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtHookDlg dialog

class CNLLinkPrtHookDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtHookDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtHookDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_HOOK };
	CTextUnit	m_wndOpenUnit;
	CTextUnit	m_wndStifUnit;
	CEditUnit	m_wndOpen;
	CEditUnit	m_wndStif;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtHookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_NLLP_D m_Data;
	int m_nDofType;  
protected:
	void ShowData(int nFlag);
	MillustViewer m_wndPicture;
	MillustViewer m_wndPicture2;

	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtHookDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLLINKPRTHOOKDLG_H__96D69B21_4718_4B29_8401_E0BC2FA8C87E__INCLUDED_)
