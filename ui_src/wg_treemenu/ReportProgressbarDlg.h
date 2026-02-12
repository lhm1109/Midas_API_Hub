#if !defined(__REPORTPROGRESSBARDLG_H__)
#define __REPORTPROGRESSBARDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportProgressbarDlg.h : header file
//

#include "..\wg_base\wg_base_ModelessMoveDlg.h"
//#include "wg_dgnRes2.h"

/////////////////////////////////////////////////////////////////////////////
// CReportProgressbarDlg dialog

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CReportProgressbarDlg : public CModelessMoveDlg
{
// Construction
public:
	CReportProgressbarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportProgressbarDlg)
	enum { IDD = IDD_TM_DYGEN_PROGRESS_DLG };
	CProgressCtrl	m_ProgressBar;
	//}}AFX_DATA
	int m_nIdx;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportProgressbarDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReportProgressbarDlg)
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__REPORTPROGRESSBARDLG_H__)

