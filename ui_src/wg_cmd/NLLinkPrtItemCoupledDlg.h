#if !defined(__NLLINKPRTITEMCOUPLEDDLG_H__)
#define      __NLLINKPRTITEMCOUPLEDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtItemCoupledDlg.h : header file

#include "../MIT_frx/MTabCtrl.h"

//
/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtItemCoupledDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CNLLinkPrtItemCoupledDlg : public CDialogMove
{
// Construction
public:
	CNLLinkPrtItemCoupledDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CNLLinkPrtItemCoupledDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_ITEM_COUPLED };
	
	mit::frx::MTabCtrl	m_Tab;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtItemCoupledDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
		
public:
	double m_dStif[21];
	double m_dDamp[21];

	int m_nType;    // 0:spring  1:Linear Dashpot  2:0+1
	int m_nAppType; // 0:Elem Type1  1: Force Type  2:Elem Type2
	void SetParamData(int nAppType, int  nType )
	{
		m_nAppType = nAppType;
		m_nType = nType;
	}


// Implementation
protected:
	CDBDoc* m_pDoc;

	CArray<UINT,UINT> m_arLinkType;
	CArray<UINT,UINT> m_arDLinkType;

	BOOL m_bInit;

	void ChangeLinkNumber(int nEditNum);
	void GetData(int nTabIndex);
	void SetData(int nTabIndex);

	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtItemCoupledDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCmd1Edit();
	afx_msg void OnChangeCmd2Edit();
	afx_msg void OnChangeCmd3Edit();
	afx_msg void OnChangeCmd4Edit();
	afx_msg void OnChangeCmd5Edit();
	afx_msg void OnChangeCmd6Edit();
	afx_msg void OnChangeCmd7Edit();
	afx_msg void OnChangeCmd8Edit();
	afx_msg void OnChangeCmd9Edit();
	afx_msg void OnChangeCmd10Edit();
	afx_msg void OnChangeCmd11Edit();
	afx_msg void OnChangeCmd12Edit();
	afx_msg void OnChangeCmd13Edit();
	afx_msg void OnChangeCmd14Edit();
	afx_msg void OnChangeCmd15Edit();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__NLLINKPRTITEMCOUPLEDDLG_H__)
