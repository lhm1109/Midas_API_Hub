#if !defined(AFX_SEISIS02PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)
#define AFX_SEISIS02PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisIS02PeriodCalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisIS02PeriodCalcDlg dialog

class CSeisIS02PeriodCalcDlg : public CDialogMove
{
// Construction
public:
	CSeisIS02PeriodCalcDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double dH, double dB, int nMethod, double& dPeriod);
	double m_dPeriod_X;
	double m_dPeriod_Y;

// Dialog Data
	//{{AFX_DATA(CSeisIS02PeriodCalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_IS02 };
	CEditUnit	m_wndBY;
	CEditUnit	m_wndBX;
	CEditUnit	m_wndHY;
	CEditUnit	m_wndHX;
	int		m_nXMethod;
	int		m_nYMethod;
	double	m_dHX;
	double	m_dHY;
	double	m_dBX;
	double	m_dBY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisIS02PeriodCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisIS02PeriodCalcDlg)
	virtual void OnOK();
	afx_msg void OnRdoXmethod();
	afx_msg void OnRdoYmethod();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISIS02PERIODCALCDLG_H__6843F3BE_2DEE_4F35_BEB7_99D2AD4A5016__INCLUDED_)
