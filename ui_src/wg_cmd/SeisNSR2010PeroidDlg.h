#if !defined(AFX_SEISNSR2010PERIODDLG_H_)
#define AFX_SEISNSR2010PERIODDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisIBC2012PeriodDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisNSR2010PeriodDlg dialog

class CSeisNSR2010PeriodDlg : public CDialogMove
{
	// Construction
public:
	CSeisNSR2010PeriodDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double dHn, int nN, double dCw, double& dPeriod, int nMethod);
	double m_dPeriodX;
	double m_dPeriodY;	
	double m_dHnX;
	double m_dHnY;
	int    m_nNX;
	int    m_nNY;
	double m_dCwX;
	double m_dCwY;	

	// Dialog Data
	//{{AFX_DATA(CSeisNSR2010PeriodDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_NSR2010 };
	CEditUnit	m_wndCwY;
	CEditUnit	m_wndCwX;
	CEditUnit	m_wndHnY;
	CEditUnit	m_wndHnX;
	CEditUnit m_wndNX;
	CEditUnit m_wndNY;
	int		m_nMethodX;
	int		m_nMethodY;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisNSR2010PeriodDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	void CtrlManager();

	// Generated message map functions
	//{{AFX_MSG(CSeisNSR2010PeriodDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRdoXmethodChange();
	afx_msg void OnRdoYmethodChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISNSR2010PERIODDLG_H_)
