#if !defined(AFX_ETC_KSPERIODCALC_H__BAF5CBA1_2CF4_11D4_AA8B_0080AD78AAC8__INCLUDED_)
#define AFX_ETC_KSPERIODCALC_H__BAF5CBA1_2CF4_11D4_AA8B_0080AD78AAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_KSPeriodCalc.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

class CDBDoc;
class CAttrCtrl;
/////////////////////////////////////////////////////////////////////////////
// CETC_KSPeriodCalc dialog

class CETC_KSPeriodCalc : public CDialogMove
{
// Construction
public:
	BOOL CalcPeriod(double dB,double dHn,double dAc,double& dPeriod,int nMethod);
	CETC_KSPeriodCalc(CWnd* pParent = NULL);   // standard constructor
	double	m_dPeriod_x;
	double	m_dPeriod_y;
	int			m_nCodeType;
// Dialog Data
	//{{AFX_DATA(CETC_KSPeriodCalc)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD };
	CEditUnit	m_edtHn_y;
	CEditUnit	m_edtHn_x;
	CEditUnit	m_edtXAc;
	CEditUnit	m_edtYAc;
	CEditUnit	m_edtBy;
	CEditUnit	m_edtBx;
	double	m_dAc_X;
	double	m_dAc_Y;
	double	m_dHn_x;
	double	m_dBx;
	double	m_dBy;
	double	m_dHn_y;
	int		m_nXMethod;
	int		m_nYMethod;
	int		m_nXMethod2000;
	int		m_nYMethod2000;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_KSPeriodCalc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CETC_KSPeriodCalc)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRdoXmethodChange();
	afx_msg void OnRdoYmethodChange();
	afx_msg void OnCmdXmethod1Ks2000();
	afx_msg void OnCmdYmethod1Ks2000();
	afx_msg void OnCmdXmethodKs2000();
	afx_msg void OnCmdYmethodKs2000();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void AlignControl();
		
	CDBDoc* m_pDoc;
	CAttrCtrl* m_pAttrCtrl;
	CString m_strUnitLength;

	CArray<UINT,UINT> m_arOldType;
	CArray<UINT,UINT> m_arNewType;

	CArray<UINT,UINT> m_arAC;  // for ks2000
	CArray<UINT,UINT> m_arB;   // for others  
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_KSPERIODCALC_H__BAF5CBA1_2CF4_11D4_AA8B_0080AD78AAC8__INCLUDED_)
