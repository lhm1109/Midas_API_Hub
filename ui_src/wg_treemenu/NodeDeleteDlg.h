#if !defined(__NODEDELETEDLG_H__)
#define      __NODEDELETEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeDeleteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNodeDeleteDlg dialog
class  _NodeDelete
{
public:
	BOOL m_bFreeNode;
	CArray<UINT, UINT> m_KeyList;
};


class CNodeDeleteDlg : public CMenuBarChildDlg
{
// Construction
public:
	CNodeDeleteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();
	BOOL GetParameter(_NodeDelete& ND);
	BOOL DelNode(_NodeDelete& ND);
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);

	CArray<UINT,UINT> m_PickingCtrl;

// Dialog Data
	//{{AFX_DATA(CNodeDeleteDlg)
	enum { IDD = IDD_TM_NODE_DEL };
	CMouseEdit	m_wndNodeNumber;
	int		m_nOption;
	BOOL	m_bFreeNode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeDeleteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNodeDeleteDlg)
	afx_msg void OnTmPickRadio();
	afx_msg void OnTmSelRadio();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEDELETEDLG_H__E3A50C02_6049_11D3_8899_0000C0A2329D__INCLUDED_)
