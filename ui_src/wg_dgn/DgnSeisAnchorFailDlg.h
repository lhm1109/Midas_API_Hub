#if !defined(__DGNSEISANCHORFAILDLG_H__)
#define __DGNSEISANCHORFAILDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorFailDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorFailDlg dialog
#include "HeaderPre.h"

class CDgnSeisAnchorConcFail;
class CDgnSeisAnchorPryout;
class __MY_EXT_CLASS__ CDgnSeisAnchorFailDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchorFailDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorFailDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_FAIL_DLG };

	CDlgTabCtrl	m_tabFail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorFailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void MakeEmfAndClose(CString strPathForEmf, int nType, int nDir);
	void MakeEmfFile();
	void Data2Dlg();
	BOOL Dlg2Data();
	
	T_ANEV_D m_Data;
	BOOL m_bIsReport;

protected:
	CDgnSeisAnchorConcFail* m_pConcFail;
	CDgnSeisAnchorPryout* m_pPryOut;

	BOOL m_bSetEmf;
	int m_nTypeForEmf; // 0:ConcFail, 1:PryOut
	int m_nDirForEmf;  // 0:교축, 1:교축직각
	CString m_strPathForEmf;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorFailDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnDgnSeisAnchorFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchorFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchorFailOKBtn();
	afx_msg void OnDgnSeisAnchorFailCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHORFAILDLG_H__)
