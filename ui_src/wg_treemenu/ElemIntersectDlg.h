#if !defined(AFX_ELEMINTERSECTDLG_H__FD5F8EA3_FE3B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_ELEMINTERSECTDLG_H__FD5F8EA3_FE3B_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElemIntersectDlg.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CElemIntersectDlg dialog

class CElemIntersectDlg : public CMenuBarChildDlg
{
// Construction
public:
	CElemIntersectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CElemIntersectDlg)
	enum { IDD = IDD_TM_ELEM_INTERSECT };
	CTextUnit	m_wndTolUnit;
	CEditUnit	m_wndTolEdit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElemIntersectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CElemIntersectDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMINTERSECTDLG_H__FD5F8EA3_FE3B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
