#if !defined(AFX_NLLINKPRTGAPDLG_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
#define AFX_NLLINKPRTGAPDLG_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtGapDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtGapDlg dialog

class CNLLinkPrtGapDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtGapDlg(CWnd* pParent = NULL);   // standard constructor
	~CNLLinkPrtGapDlg();

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtGapDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_GAP };
	CBCGPStatic	m_wndOpenTitle;
	CTextUnit	m_wndOpenUnit;
	CTextUnit	m_wndStifUnit;
	CEditUnit	m_wndOpen;
	CEditUnit	m_wndStif;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtGapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_NLLP_D m_Data;
	int m_nDofType;  
	int m_nCodeType;
protected:
	void ShowData(int nFlag);
	MillustViewer m_wndPicture;
	MillustViewer m_wndPicture2;

	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtGapDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLLINKPRTGAPDLG_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
