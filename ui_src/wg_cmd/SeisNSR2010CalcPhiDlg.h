#if !defined(AFX_SEISNSR2010CALCPHIDLG_H_)
#define AFX_SEISNSR2010CALCPHIDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisNSR2010CalcPhiDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisNSR2010CalcPhiDlg dialog

class CSeisNSR2010CalcPhiDlg : public CDialogMove
{
	// Construction
public:
	CSeisNSR2010CalcPhiDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CSeisNSR2010CalcPhiDlg)
	enum { IDD = IDD_ETC_SEIS_CALCPHI_NSR2010 };
	CEditUnit	m_editA_X;
	CEditUnit	m_editA_Y;
	CEditUnit	m_editP_X;
	CEditUnit	m_editP_Y;
	CEditUnit	m_editR_X;
	CEditUnit	m_editR_Y;
	CEditUnit	m_editResult_X;
	CEditUnit	m_editResult_Y;

	//}}AFX_DATA
public:
	void GetResult(double& dPhi_X, double& dPhi_Y) { dPhi_X = m_dResultX; dPhi_Y = m_dResultY; }
protected:
	BOOL CalcPhi();

	double m_dResultX;
	double m_dResultY;	

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisNSR2010CalcPhiDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSeisNSR2010CalcPhiDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCalcPhiBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEISNSR2010CALCPHIDLG_H_)
