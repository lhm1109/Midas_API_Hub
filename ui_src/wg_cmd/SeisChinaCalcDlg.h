#if !defined(AFX_SEISCHINACALCDLG_H__C934706D_CB73_405F_B643_66DA8E9CF35A__INCLUDED_)
#define AFX_SEISCHINACALCDLG_H__C934706D_CB73_405F_B643_66DA8E9CF35A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisChinaCalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisChinaCalcDlg dialog

class CSeisChinaCalcDlg : public CDialogMove
{
// Construction
public:
	CSeisChinaCalcDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double dH, double dB, int nN, int nMethod, double& dPeriod);
	double m_dPeriod_X;
	double m_dPeriod_Y;
	int		 m_nUnitChange;	
	int		m_nNX;
	int		m_nNY;
	double	m_dBX;
	double	m_dBY;
	double	m_dHX;
	double	m_dHY;	
// Dialog Data
	//{{AFX_DATA(CSeisChinaCalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_CHINA };
	CEditUnit	m_wndNY;
	CEditUnit	m_wndNX;
	CEditUnit	m_wndBY;
	CEditUnit	m_wndBX;
	CEditUnit	m_wndHX;
	CEditUnit	m_wndHY;
	int		m_nXMethod;
	int		m_nYMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisChinaCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisChinaCalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRdoXmethod();
	afx_msg void OnRdoYmethod();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISCHINACALCDLG_H__C934706D_CB73_405F_B643_66DA8E9CF35A__INCLUDED_)
