#if !defined(__DGNSEISANCHORCONCFAIL_H__)
#define __DGNSEISANCHORCONCFAIL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorConcFail.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorConcFail dialog

class CDgnSeisAnchorConcFailSub;
class CDgnSeisAnchorConcFail : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchorConcFail(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorConcFail)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_CONC_FAIL };

	BOOL m_bUser;
	CDlgTabCtrl	m_tabConcFail;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorConcFail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetData(T_ANEV_D& Data);
	void GetData(T_ANEV_D& Data);

	void Data2Dlg();
	BOOL Dlg2Data();

	void SetRstData();
	void MakeEmfFile(CString strPathForEmf, int nDir);
protected:
	T_ANEV_D m_Data;
	BOOL m_bNeedReCalc[2];

	CDgnSeisAnchorConcFailSub* m_pConcFailSubL;
	CDgnSeisAnchorConcFailSub* m_pConcFailSubT;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorConcFail)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisAnchorConcFailUesrChk();
	afx_msg void OnDgnSeisAnchorConcFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchorConcFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHORCONCFAIL_H__)
