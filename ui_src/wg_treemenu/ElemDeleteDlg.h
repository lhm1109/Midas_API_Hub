#if !defined(__ELEMDELETEDLG_H__)
#define      __ELEMDELETEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemDeleteDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElemDeleteDlg dialog
class  _ElemDelete
{
public:
	BOOL m_bFreeNode;
	CArray<UINT, UINT> m_KeyList;
};


class CElemDeleteDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemDeleteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

	BOOL GetParameter(_ElemDelete& ED);
	BOOL DelElem(_ElemDelete& ED);
	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);

	CArray<UINT,UINT> m_PickingCtrl;

// Dialog Data
	//{{AFX_DATA(CElemDeleteDlg)
	enum { IDD = IDD_TM_ELEM_DEL };
	CMouseEdit	m_wndElemNumber;
	int		m_nOption;
	BOOL	m_bFreeNode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemDeleteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemDeleteDlg)
	afx_msg void OnTmPickRadio();
	afx_msg void OnTmSelRadio();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMDELETEDLG_H__E3A50C04_6049_11D3_8899_0000C0A2329D__INCLUDED_)
