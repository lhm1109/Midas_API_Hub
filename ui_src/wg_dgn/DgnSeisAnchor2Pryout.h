#if !defined(__DgnSeisAnchor2Pryout_H__)
#define __DgnSeisAnchor2Pryout_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchor2Pryout.h : header file
//

#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog
#include "..\wg_base\wg_base_DlgTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2Pryout dialog

class CDgnSeisAnchor2PryoutSub;
class CDgnSeisAnchor2Pryout : public CChildDialog
{
// Construction
public:
	CDgnSeisAnchor2Pryout(CWnd* pParent = NULL, BOOL bPrint=FALSE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchor2Pryout)
	enum { IDD = IDD_DGN_SEIS_ANCHOR2_PRYOUT };

	BOOL m_bUser;
	CDlgTabCtrl	m_tabPryout;	
	BOOL m_bPrintCF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchor2Pryout)
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
	int m_nRatTypeFailPry[4];

	CDgnSeisAnchor2PryoutSub* m_pPryoutSubL;
	CDgnSeisAnchor2PryoutSub* m_pPryoutSubT;

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchor2Pryout)
	virtual BOOL OnInitDialog();	
	afx_msg void OnDgnSeisAnchor2PryoutUesrChk();
	afx_msg void OnDgnSeisAnchor2PryoutChangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDgnSeisAnchor2PryoutChangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DgnSeisAnchor2Pryout_H__)
