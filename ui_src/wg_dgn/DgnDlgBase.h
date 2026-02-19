#if !defined(AFX_DGNDLGBASE_H__1D975981_0195_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNDLGBASE_H__1D975981_0195_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SWDlgBase.h : header file
//

#include "..\wg_base\wg_base_ChildDialogMove.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnDlgBase window

class CDgnDlgBase : public CChildDialogMove
{
// Construction
public:
	CDgnDlgBase(UINT nIDTemplate, CWnd* pParentWnd = NULL);

// Attributes
public:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CDgnDlgBase)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Operations
public:

// Overrides
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnDlgBase)
	public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDgnDlgBase)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateDlg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWDLGBASE_H__B9431BE3_5938_11D3_A014_0080AD78AA2C__INCLUDED_)
