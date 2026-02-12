#if !defined(AFX_SEISJISCALCDLG_H__79D95401_24EE_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_SEISJISCALCDLG_H__79D95401_24EE_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisJISCalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSeisJISCalcDlg dialog

class CSeisJISCalcDlg : public CDialogMove
{
// Construction
public:
	CSeisJISCalcDlg(CWnd* pParent = NULL);   // standard constructor
	void SetPeriodValue(double hx, double hy, double ax, double ay);
	void GetPeriodValue(double& hx, double& hy, double& ax, double& ay);

// Dialog Data
	//{{AFX_DATA(CSeisJISCalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_JIS };
	CEditUnit	m_wndAy;
	CEditUnit	m_wndAx;
	CEditUnit	m_wndHy;
	CEditUnit	m_wndHx;
	double	m_ax;
	double	m_ay;
	double	m_hx;
	double	m_hy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisJISCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisJISCalcDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISJISCALCDLG_H__79D95401_24EE_11D5_880F_00010263A1F4__INCLUDED_)
