#if !defined(__DgnSeisAnchor2FailDlg_H__)
#define __DgnSeisAnchor2FailDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2FailDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2FailDlg dialog
#include "HeaderPre.h"

class CDgnSeisAnchor2ConcFail;
class CDgnSeisAnchor2Pryout;
class __MY_EXT_CLASS__ CDgnSeisAnchor2FailDlg : public CDialogMove
{
// Construction
public:
	CDgnSeisAnchor2FailDlg(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchor2FailDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR2_FAIL_DLG };

	CDlgTabCtrl	m_tabFail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchor2FailDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void MakeEmfAndClose(CString strPathForEmf, int nType, int nDir);
	void MakeEmfFile();
	void Data2Dlg();
	BOOL Dlg2Data(BOOL bOKBtn=FALSE, BOOL bRedraw = FALSE);
	void Calc_BearingAnchorForDlg(int nFailType);
	
	T_ANEV_D m_Data;
	BOOL m_bIsReport;
	int m_nRatTypeFail[4];

protected:
	CDgnSeisAnchor2ConcFail* m_pConcFail;
	CDgnSeisAnchor2Pryout* m_pPryOut;

	BOOL m_bSetEmf;
	int m_nTypeForEmf; // 0:ConcFail, 1:PryOut
	int m_nDirForEmf;  // 0:교축, 1:교축직각
	CString m_strPathForEmf;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchor2FailDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer( UINT_PTR nIDEvent );
	afx_msg void OnDgnSeisAnchorFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchorFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchorRedrawFailBtn();
	afx_msg void OnDgnSeisAnchorFailOKBtn();
	afx_msg void OnDgnSeisAnchorFailCancelBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2FailDlg_H__)
