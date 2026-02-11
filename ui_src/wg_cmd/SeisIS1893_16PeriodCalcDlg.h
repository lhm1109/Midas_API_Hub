#if !defined(AFX_SEISIS1893PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)
#define AFX_SEISIS1893PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisIS02PeriodCalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisIS1893PeriodCalcDlg dialog

enum
{
	D_P100_PERIOD_METHOD1,  // 1. T = 0.085H^(3/4)
	D_P100_PERIOD_METHOD2,  // 2. T = 0.075H^(3/4)
	D_P100_PERIOD_METHOD3,  // 3. T = 0.050H^(3/4)
	D_P100_PERIOD_METHOD4,  // 4. T = 0.075H^(3/4) /sqrt(Ac)
	D_P100_PERIOD_METHOD5   // 5. T = 1.0n
};

class CSeisIS1893PeriodCalcDlg : public CDialogMove
{
// Construction
public:
	CSeisIS1893PeriodCalcDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double dH,double dAc,double dD,int nMethod,double& dPeriod);
	double m_dPeriod_X;
	double m_dPeriod_Y;

// Dialog Data
	//{{AFX_DATA(CSeisIS1893PeriodCalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_IS1893_2016 };
	CEditUnit	m_wndYHedit;
	CEditUnit	m_wndXHedit;
	CEditUnit	m_wndYDedit;
	CEditUnit	m_wndXDedit;
	CEditUnit	m_wndYAcedit;
	CEditUnit	m_wndXAcedit;
	int		m_nXMethod;
	int		m_nYMethod;
	double	m_dXAcedit;
	double	m_dYAcedit;
	double	m_dXDedit;
	double	m_dYDedit;
	double	m_dXHedit;
	double	m_dYHedit;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisIS1893PeriodCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisIS1893PeriodCalcDlg)
	virtual void OnOK();
	afx_msg void OnRdoXmethod();
	afx_msg void OnRdoYmethod();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


	BOOL m_bNTCFlag;
	BOOL m_bP100Flag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISIS02PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)
