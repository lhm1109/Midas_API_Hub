#if !defined(AFX_ETC_GDS_NSR2010CALCPHIDLG_H_)
#define AFX_ETC_GDS_NSR2010CALCPHIDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ETC_GDS_NSR2010CalcPhiDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NSR2010CalcPhiDlg dialog

class CETC_GDS_NSR2010CalcPhiDlg : public CDialogMove
{
	// Construction
public:
	CETC_GDS_NSR2010CalcPhiDlg(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CETC_GDS_NSR2010CalcPhiDlg)
	enum { IDD = IDD_ETC_GDS_CALCPHI_NSR2010 };
	CEditUnit	m_editA_X;
	CEditUnit	m_editP_X;
	CEditUnit	m_editR_X;
	CEditUnit	m_editResult_X;

	//}}AFX_DATA
public:
	void GetResult(double& dPhi_X) { dPhi_X = m_dResultX;}
protected:
	BOOL CalcPhi();

	double m_dResultX;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETC_GDS_NSR2010CalcPhiDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CETC_GDS_NSR2010CalcPhiDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCalcPhiBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETC_GDS_NSR2010CALCPHIDLG_H_)
