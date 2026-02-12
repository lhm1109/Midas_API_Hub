#if !defined(AFX_SEISNBC95CALCDLG_H__3C341BEA_E4E2_4C6D_9741_345457252D1E__INCLUDED_)
#define AFX_SEISNBC95CALCDLG_H__3C341BEA_E4E2_4C6D_9741_345457252D1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisNBC95CalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisNBC95CalcDlg dialog

class CSeisNBC95CalcDlg : public CDialogMove
{
// Construction
public:
	CSeisNBC95CalcDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double dH, double dB, int nN, int nMethod, double& dPeriod);
	double m_dPeriod_X;
	double m_dPeriod_Y;
	
// Dialog Data
	//{{AFX_DATA(CSeisNBC95CalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_NBC95 };
	CEditUnit	m_wndBY;
	CEditUnit	m_wndBX;
	CEditUnit	m_wndHY;
	CEditUnit	m_wndHX;
	CEditUnit	m_wndNY;
	CEditUnit	m_wndNX;
	int		m_nXMethod;
	int		m_nYMethod;
	int		m_nNX;
	int		m_nNY;
	double	m_dHX;
	double	m_dHY;
	double	m_dBX;
	double	m_dBY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisNBC95CalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisNBC95CalcDlg)
	virtual void OnOK();
	afx_msg void OnRdoXmethod();
	afx_msg void OnRdoYmethod();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISNBC95CALCDLG_H__3C341BEA_E4E2_4C6D_9741_345457252D1E__INCLUDED_)
