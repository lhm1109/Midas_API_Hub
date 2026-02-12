#if !defined(AFX_THISNOTICEDLG_H__F92430D1_1DBC_4A84_B514_EA13CBEA90EF__INCLUDED_)
#define AFX_THISNOTICEDLG_H__F92430D1_1DBC_4A84_B514_EA13CBEA90EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisNoticeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CThisNoticeDlg dialog

class CThisNoticeDlg : public CDialog
{
// Construction
public:
	CThisNoticeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThisNoticeDlg)
	enum { IDD = IDD_ETC_THIS_SQLD_NOTC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThisNoticeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThisNoticeDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THISNOTICEDLG_H__F92430D1_1DBC_4A84_B514_EA13CBEA90EF__INCLUDED_)
