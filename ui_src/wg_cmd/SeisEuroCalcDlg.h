#if !defined(AFX_SEISEUROCALCDLG_H__3337EEF0_1C69_4653_9817_A66390884A43__INCLUDED_)
#define AFX_SEISEUROCALCDLG_H__3337EEF0_1C69_4653_9817_A66390884A43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisEuroCalcDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisEuroCalcDlg dialog

enum
{
	D_P100_PERIOD_METHOD1,  // 1. T = 0.085H^(3/4)
	D_P100_PERIOD_METHOD2,  // 2. T = 0.075H^(3/4)
	D_P100_PERIOD_METHOD3,  // 3. T = 0.050H^(3/4)
	D_P100_PERIOD_METHOD4,  // 4. T = 0.075H^(3/4) /sqrt(Ac)
	D_P100_PERIOD_METHOD5   // 5. T = 1.0n
};

class CSeisEuroCalcDlg : public CDialogMove
{
// Construction
public:
	BOOL CalcPeriod(double dH,double dAc,double dD,int nMethod,double& dPeriod);
	CSeisEuroCalcDlg(CWnd* pParent = NULL);   // standard constructor
	double	m_dPeriod_x;
	double	m_dPeriod_y;
// Dialog Data
	//{{AFX_DATA(CSeisEuroCalcDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPERIOD_EURO };
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

	inline void SetNTCFlag()  { m_bNTCFlag =TRUE; }
	inline void SetP100Flag() { m_bP100Flag=TRUE; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisEuroCalcDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisEuroCalcDlg)
	afx_msg void OnRdoXmethodChange();
	afx_msg void OnRdoYmethodChange();
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void CustomizeNTC();
	void CustomizeP100();

	BOOL m_bNTCFlag;
	BOOL m_bP100Flag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISEUROCALCDLG_H__3337EEF0_1C69_4653_9817_A66390884A43__INCLUDED_)
