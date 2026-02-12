#if !defined(AFX_SEISIBC2012PERIODDLG_H__93EE7C7D_2745_42A2_9E93_4B0368D2D0D3__INCLUDED_)
#define AFX_SEISIBC2012PERIODDLG_H__93EE7C7D_2745_42A2_9E93_4B0368D2D0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisIBC2012PeriodDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisIBC2012PeriodDlg dialog

class CSeisIBC2012PeriodDlg : public CDialogMove
{
// Construction
public:
	CSeisIBC2012PeriodDlg(CWnd* pParent = NULL, int nCode = 0);   // standard constructor
	BOOL CalcPeriod(double dHn, int nN, double dCw, double& dPeriod, int nMethod);
	double m_dPeriodX;
	double m_dPeriodY;	
	double m_dHnX;
	double m_dHnY;
	int    m_nNX;
	int    m_nNY;
	double m_dCwX;
	double m_dCwY;	

	void SetCode(int nCode);
	
// Dialog Data
	//{{AFX_DATA(CSeisIBC2012PeriodDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_IBC2012 };
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
	//{{AFX_VIRTUAL(CSeisIBC2012PeriodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nCode;
	double m_dLengFactCode;
	
	void CtrlManager();
	void SetControlByCode(int nCode);

	// Generated message map functions
	//{{AFX_MSG(CSeisIBC2012PeriodDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRdoXmethodChange();
	afx_msg void OnRdoYmethodChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISIBC2012PERIODDLG_H__93EE7C7D_2745_42A2_9E93_4B0368D2D0D3__INCLUDED_)
