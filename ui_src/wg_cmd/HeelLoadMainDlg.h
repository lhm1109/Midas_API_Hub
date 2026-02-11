#if !defined(AFX_HeelLoadMainDlg_H__E39085CF_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
#define AFX_HeelLoadMainDlg_H__E39085CF_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HeelLoadMainDlg.h : header file
//
#include "HeelLoadChildDialog.h"
/////////////////////////////////////////////////////////////////////////////
// CHeelLoadMainDlg dialog

#define IABSE         1
#define AIJRC_WALK    2
#define AIJRC_RUN     4
#define ALLEN_RAINER  8
#define IABSE1STEP    16
#define GB_T51288_FREE 21
#define GB_T51288_RHYT 22
#define JGJ_T441_WALK  23
#define JGJ_T441_RHYT  24
#define JGJ_T441_BRIDGE  25

class CHeelLoadMainDlg : public CDialogMove
{
// Construction
public:
	CString m_strFuncName;
	CHeelLoadMainDlg(CWnd* pParent = NULL);   // standard constructor
	~CHeelLoadMainDlg();   // standard destructor
	CMap <int,int,CHeelLoadChildDialog*,CHeelLoadChildDialog*> m_mapChildDlg;
	CArray <double,double>* m_parPeriod,*m_parAccel;
	CString m_sDescript;
	int m_nCurrentDlg;
	void UpdateCurDlg();
// Dialog Data
	//{{AFX_DATA(CHeelLoadMainDlg)
	enum { IDD = IDD_HEELDROPLOAD_MAIN };
	CComboBox	m_cmbDesignCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeelLoadMainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHeelLoadMainDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeEtcGdsSpectrumCmb();
	afx_msg void OnCloseupEtcGdsSpectrumCmb();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HeelLoadMainDlg_H__E39085CF_B9F2_11D3_AA8A_0080AD78AAC8__INCLUDED_)
