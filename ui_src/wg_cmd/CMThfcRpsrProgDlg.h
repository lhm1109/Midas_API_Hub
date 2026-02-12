#if !defined(AFX_CMTHFCRPSRPROGDLG_H__1CD0C12C_90C5_4FD9_BBDD_3714C8F2EC0F__INCLUDED_)
#define AFX_CMTHFCRPSRPROGDLG_H__1CD0C12C_90C5_4FD9_BBDD_3714C8F2EC0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThfcRpsrProgDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMThfcRpsrProgDlg dialog

class CCMThfcRpsrProgDlg : public CDialog
{
// Construction
public:
	CCMThfcRpsrProgDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMThfcRpsrProgDlg)
	enum { IDD = IDD_ETC_THFC_RPSR_PROG };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThfcRpsrProgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMThfcRpsrProgDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHFCRPSRPROGDLG_H__1CD0C12C_90C5_4FD9_BBDD_3714C8F2EC0F__INCLUDED_)
