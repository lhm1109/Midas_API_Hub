#if !defined(AFX_SEISKDS4117PERIODDLG_H__93EE7C7D_2745_42A2_9E93_4B0368D2D0D3__INCLUDED_)
#define AFX_SEISKDS4117PERIODDLG_H__93EE7C7D_2745_42A2_9E93_4B0368D2D0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisKDS4117PeriodDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisKDS4117PeriodDlg dialog

class CSeisKDS4117PeriodDlg : public CDialogMove
{
// Construction
public:
	CSeisKDS4117PeriodDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double hn, int N, double& dPeriod, int nMethod);
	double m_dPeriodX;
	double m_dPeriodY;	
	double m_dHnX;
	double m_dHnY;
	int    m_nNX;
	int    m_nNY;
	void SetTitleText(CString str) { m_strTitle = str; }

// Dialog Data
	//{{AFX_DATA(CSeisKDS4117PeriodDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_KDS4117 };
	CEditUnit	m_wndAcY;
	CEditUnit	m_wndAcX;
	CEditUnit	m_wndHnY;
	CEditUnit	m_wndHnX;
	CEditUnit m_wndNX;
	CEditUnit m_wndNY;
	int		m_nMethodX;
	int		m_nMethodY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisKDS4117PeriodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void CtrlManager();

	// Generated message map functions
	//{{AFX_MSG(CSeisKDS4117PeriodDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRdoXmethodChange();
	afx_msg void OnRdoYmethodChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CString m_strTitle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISKDS4117PERIODDLG_H__93EE7C7D_2745_42A2_9E93_4B0368D2D0D3__INCLUDED_)
