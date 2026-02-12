#if !defined(AFX_STLDTIMELOADSFORCS_H__3824CA30_4AB4_4160_840F_CCB1E0DA413B__INCLUDED_)
#define AFX_STLDTIMELOADSFORCS_H__3824CA30_4AB4_4160_840F_CCB1E0DA413B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StldTimeLoadsForCS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStldTimeLoadsForCS dialog
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_CobxLdgr.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

class CStldTimeLoadsForCS : public CMenuBarChildDlg , public CDBUpdateConnector
{
// Construction
public:
	CStldTimeLoadsForCS(CWnd* pParent = NULL);   // standard constructor
	void Execute() { OnTmExecute(); };

// Dialog Data
	//{{AFX_DATA(CStldTimeLoadsForCS)
	enum { IDD = IDD_TM_STLD_TLCS };
	CCobxLdgr	m_CobxLdgr;
	CFormulaEditSpin	m_wndDay;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStldTimeLoadsForCS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStldTimeLoadsForCS)
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

#endif // !defined(AFX_STLDTIMELOADSFORCS_H__3824CA30_4AB4_4160_840F_CCB1E0DA413B__INCLUDED_)
