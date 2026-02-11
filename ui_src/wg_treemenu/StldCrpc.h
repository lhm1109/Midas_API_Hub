#if !defined(AFX_STLDCRPC_H__3824CA30_4AB4_4160_840F_CCB1E0DA413B__INCLUDED_)
#define AFX_STLDCRPC_H__3824CA30_4AB4_4160_840F_CCB1E0DA413B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldCrpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldCrpc dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxLdgr.h"

class CStldCrpc : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CStldCrpc(CWnd* pParent = NULL);   // standard constructor
	void Execute() { OnTmExecute(); };

// Dialog Data
	//{{AFX_DATA(CStldCrpc)
	enum { IDD = IDD_TM_STLD_CRPC };
	CCobxLdgr	m_CobxLdgr;
	CString	m_strCreep;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldCrpc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldCrpc)
	afx_msg void OnTmDefineLdgrBtn();
	afx_msg void OnTmExecute();
	afx_msg void OnTmClose();
	afx_msg void OnDeltaposTmDaySpin(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnTmSdOptRadio();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STLDCRPC_H__3824CA30_4AB4_4160_840F_CCB1E0DA413B__INCLUDED_)
