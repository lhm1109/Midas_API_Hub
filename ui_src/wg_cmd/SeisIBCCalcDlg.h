#if !defined(AFX_SEISIBCCALCDLG_H__04C9E1E1_3D38_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_SEISIBCCALCDLG_H__04C9E1E1_3D38_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisIBCCalcDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSeisIBCCalcDlg dialog

class CSeisIBCCalcDlg : public CDialogMove
{
// Construction
public:
	CSeisIBCCalcDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL CalcPeriod(double dH, int nN, int nMethod, double& dPeriod);
	double m_hnX;
	double m_hnY;
	double m_dPeriod_X;
	double m_dPeriod_Y;
	double m_nNX;
	double m_nNY;
// Dialog Data
	//{{AFX_DATA(CSeisIBCCalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_IBC };
	CEditUnit	m_wndNy;
	CEditUnit	m_wndNx;
	CEditUnit	m_wndHy;
	CEditUnit	m_wndHx;
	int		m_xrdo;
	int		m_yrdo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisIBCCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*   m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CSeisIBCCalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeX();
	afx_msg void OnSelChangeY();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISIBCCALCDLG_H__04C9E1E1_3D38_11D5_880F_00010263A1F4__INCLUDED_)
