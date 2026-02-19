#if !defined(__DGNXLPROGRESSDLG_H__)
#define __DGNXLPROGRESSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnProgressDlg.h : header file
//

#include "..\wg_base\wg_base_ModelessMoveDlg.h"
#include "wg_dgnRes2.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnXLProgressDlg dialog

#include "HeaderPre.h"

class CCRCExcelOutput;

class __MY_EXT_CLASS__ CDgnXLProgressDlg : public CModelessMoveDlg
{
// Construction
public:
	CDgnXLProgressDlg(CWnd* pParent = NULL);   // standard constructor
	~CDgnXLProgressDlg();

// Dialog Data
	//{{AFX_DATA(CDgnXLProgressDlg)
	enum { IDD = IDD_DGN_PROGRESS_XL_DLG };
	CProgressCtrl	m_ProgressBar;
	CString	m_strName;
	CString	m_strPercent;
	//}}AFX_DATA

public:
	BOOL m_bStopExecute;
	CCRCExcelOutput* m_pExcelOut;
 
	void Execute();
	LRESULT OnMessage(WPARAM wp, LPARAM lp);  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnXLProgressDlg)
	public:
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnXLProgressDlg)
	afx_msg void OnDestroy();
	afx_msg void OnDgnStopBtn();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNXLPROGRESSDLG_H__)
