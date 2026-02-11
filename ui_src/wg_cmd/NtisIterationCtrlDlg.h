#if !defined(AFX_NTISITERATIONCTRLDLG_H__0DFAC801_CC31_4249_BEC4_D19254E58821__INCLUDED_)
#define AFX_NTISITERATIONCTRLDLG_H__0DFAC801_CC31_4249_BEC4_D19254E58821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NtisIterationCtrlDlg.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CNtisIterationCtrlDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CNtisIterationCtrlDlg : public CDialogMove
{
// Construction
public:
	CNtisIterationCtrlDlg(T_NTIS_BOUNDARY *pData, BOOL bUseBoundaryNL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNtisIterationCtrlDlg)
	enum { IDD = IDD_ETC_NTIS_ITER_DLG };

	BOOL      m_bConvergenceFail;
	
	CFormulaEditSpin m_edtMaxIteration;
	BOOL      m_bDispNorm;
	BOOL      m_bForcNorm;
	BOOL      m_bEnrgNorm;
	CEditUnit m_edtDispNorm;
	CEditUnit m_edtForcNorm;
	CEditUnit m_edtEnrgNorm;
	
	mit::frx::MComboBox m_cbxStiffScheme;
	CEditUnit m_edtNumofIterStiff;
	CEditUnit m_edtMaxBisectLevel;
	mit::frx::MButton m_chkSmartBisection;
	CEditUnit m_edtDivThreshold;

	mit::frx::MButton  m_chkEnableLineSearch;
	mit::frx::MComboBox m_cmbLineSearchOption;
	CEditUnit m_edtLineSearchNum;
	CEditUnit m_edtMaxLineSearch;
	CEditUnit m_edtLineSearchTol;

	int m_nRungeKutta;
	CEditUnit m_edtBoundaryNLTol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNtisIterationCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_NTIS_BOUNDARY *m_pData;
	BOOL m_bUseBoundaryNL;

// Implementation
protected:
	void Data2Dlg();
	void Dlg2Data();
	void SetLineSearchOptCmb();
	void EnableLineSearch(bool bEnable);
	// Generated message map functions
	//{{AFX_MSG(CNtisIterationCtrlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdClose();
	afx_msg void OnChkCriteria();
	afx_msg void OnCmdNonlinearChk1();
	afx_msg void OnCmdNonlinearChk2();
	afx_msg void OnCmdNonlinearChk3();
	afx_msg void OnUseLineSearch();
	afx_msg void SetStiffUpdateSchemeCbx();
	afx_msg void OnCmdChangeSchemeCbx();
	afx_msg void OnCmdChangeLineSearchOptCbx();
// 	afx_msg void OnMaxIterSpin(NMHDR* pNMHDR, LRESULT* pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#include "HeaderPost.h"
#endif // !defined(AFX_NTISITERATIONCTRLDLG_H__0DFAC801_CC31_4249_BEC4_D19254E58821__INCLUDED_)
