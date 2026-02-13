#if !defined(__DgnSeisAnchor2ConcFail_H__)
#define __DgnSeisAnchor2ConcFail_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2ConcFail.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ConcFail dialog

class CDgnSeisAnchor2ConcFailSub;
class CDgnSeisAnchor2ConcFail : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchor2ConcFail(CWnd* pParent = NULL, BOOL bPrint=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchor2ConcFail)
	enum { IDD = IDD_DGN_SEIS_ANCHOR2_CONC_FAIL };

	BOOL m_bUser;
	CDlgTabCtrl	m_tabConcFail;	
	BOOL m_bPrintCF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchor2ConcFail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetData(T_ANEV_D& Data, int nRatType[4]);
	void GetData(T_ANEV_D& Data);

	void Data2Dlg();
	BOOL Dlg2Data(BOOL bOKBtn=FALSE);

	void SetRstData();
	void MakeEmfFile(CString strPathForEmf, int nDir);

	void SetViewData();
protected:
	T_ANEV_D m_Data;
	BOOL m_bNeedReCalc[2];
	int m_nRatTypeFailConc[4];

	CDgnSeisAnchor2ConcFailSub* m_pConcFailSubL;
	CDgnSeisAnchor2ConcFailSub* m_pConcFailSubT;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchor2ConcFail)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnSeisAnchor2ConcFailUesrChk();
	afx_msg void OnDgnSeisAnchor2ConcFailChangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchor2ConcFailChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2ConcFail_H__)
