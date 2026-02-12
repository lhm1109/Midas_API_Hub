#if !defined(AFX_NLLINKPRTFRICDLG_H__26221755_0F3A_41E1_B223_4BD6C58D5B40__INCLUDED_)
#define AFX_NLLINKPRTFRICDLG_H__26221755_0F3A_41E1_B223_4BD6C58D5B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtFricDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\MIT_frx\MillustViewer.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtFricDlg dialog

class CNLLinkPrtFricDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtFricDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtFricDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_FRIC };
	CTextUnit	m_wndStifnessUnit;
	CTextUnit	m_wndRadiSurfUnit;
	CTextUnit m_wndRateParaUnit;	 //1unit
	CEditUnit	m_wndHystAlph;
	CEditUnit	m_wndHystBeta;
	CEditUnit	m_wndRadiSurf;
	CEditUnit	m_wndRatePara;
	CEditUnit	m_wndFricFast;
	CEditUnit	m_wndFricSlow;
	CEditUnit	m_wndStifness;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtFricDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_NLLP_D m_Data;
	int m_nDofType;  
protected:
	void ShowData(int nFlag);
	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtFricDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
//	afx_msg void OnChangeCmdNllkBetaEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	MillustViewer m_wndPicture;
	MillustViewer m_wndPicture2;
	MillustViewer m_wndPicture3;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NLLINKPRTFRICDLG_H__26221755_0F3A_41E1_B223_4BD6C58D5B40__INCLUDED_)
