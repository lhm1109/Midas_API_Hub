#if !defined(AFX_NLLINKPRTLEADDLG_H__52D4EDF7_FE51_4615_9C08_AD24F1CE25E0__INCLUDED_)
#define AFX_NLLINKPRTLEADDLG_H__52D4EDF7_FE51_4615_9C08_AD24F1CE25E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtLeadDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtLeadDlg dialog

class CNLLinkPrtLeadDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtLeadDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtLeadDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_LEAD };
	CTextUnit	m_wndStifnessUnit;
	CTextUnit	m_wndYildStrnUnit;
	CEditUnit	m_wndStifness;
	CEditUnit	m_wndYildStrn;
	CEditUnit	m_wndPostYild;
	CEditUnit	m_wndHystBeta;
	CEditUnit	m_wndHystAlph;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtLeadDlg)
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
	MillustViewer m_wndPicture3;

	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtLeadDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
//	afx_msg void OnChangeCmdNllkBetaEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLLINKPRTLEADDLG_H__52D4EDF7_FE51_4615_9C08_AD24F1CE25E0__INCLUDED_)
