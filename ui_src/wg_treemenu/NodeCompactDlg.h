#if !defined(__NODECOMPACTDLG_H__)
#define      __NODECOMPACTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeCompactDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNodeCompactDlg dialog

class CNodeCompactDlg : public CMenuBarChildDlg
{
// Construction
public:
	CNodeCompactDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	CArray<UINT,UINT> m_ByNumCtrl;

// Dialog Data
	//{{AFX_DATA(CNodeCompactDlg)
	enum { IDD = IDD_TM_NODE_COMPACT };
	int		m_nRange;
	UINT	m_nStartNum;
	UINT	m_nEndNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeCompactDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeCompactDlg)
	afx_msg void OnTmSelectAll();
	afx_msg void OnTmSelectRange();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODECOMPACTDLG_H__5D6FF605_9923_11D3_8899_0000C0A2329D__INCLUDED_)
