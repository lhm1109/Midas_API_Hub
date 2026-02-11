#if !defined(__ELEMCOMPACTDLG_H__)
#define      __ELEMCOMPACTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemCompactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemCompactDlg dialog

class CElemCompactDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemCompactDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	CArray<UINT,UINT> m_ByNumCtrl;

// Dialog Data
	//{{AFX_DATA(CElemCompactDlg)
	enum { IDD = IDD_TM_ELEM_COMPACT };
	int		m_nRange;
	UINT	m_nStartNum;
	UINT	m_nEndNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemCompactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemCompactDlg)
	afx_msg void OnTmSelectAll();
	afx_msg void OnTmSelectRange();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMCOMPACTDLG_H__5D6FF604_9923_11D3_8899_0000C0A2329D__INCLUDED_)
