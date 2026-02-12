#if !defined(AFX_SEISUBCCLACDLG_H__87054310_2D5E_478E_ABD5_625442E42DD9__INCLUDED_)
#define AFX_SEISUBCCLACDLG_H__87054310_2D5E_478E_ABD5_625442E42DD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisUBCClacDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;

/////////////////////////////////////////////////////////////////////////////
// CSeisUBCClacDlg dialog

class CSeisUBCClacDlg : public CDialogMove
{
// Construction
public:
	CSeisUBCClacDlg(CWnd* pParent = NULL);   // standard constructor
	
	BOOL CalcPeriod(double dH, int nMethod, double& dPeriod);
	double m_hnX;
	double m_hnY;
	double m_dPeriod_X;
	double m_dPeriod_Y;

// Dialog Data
	//{{AFX_DATA(CSeisUBCClacDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_UBC };
	CEditUnit	m_wndHy;
	CEditUnit	m_wndHx;
	int		m_xrdo;
	int		m_yrdo;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisUBCClacDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*   m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CSeisUBCClacDlg)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChangeX();
	afx_msg void OnSelChangeY();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISUBCCLACDLG_H__87054310_2D5E_478E_ABD5_625442E42DD9__INCLUDED_)
