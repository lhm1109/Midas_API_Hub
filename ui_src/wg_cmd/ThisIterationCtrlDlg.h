#if !defined(AFX_THISITERATIONCTRLDLG_H__0DFAC801_CC31_4249_BEC4_D19254E58821__INCLUDED_)
#define AFX_THISITERATIONCTRLDLG_H__0DFAC801_CC31_4249_BEC4_D19254E58821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisIterationCtrlDlg.h : header file
//

#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CThisIterationCtrlDlg dialog
struct T_THIS_D;
class CThisIterationCtrlDlg : public CDialogMove
{
// Construction
public:
	CThisIterationCtrlDlg(T_THIS_D *pData, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThisIterationCtrlDlg)
	enum { IDD = IDD_ETC_THIS_ITER_DLG };
	BOOL      m_bConvergenceFail;
	CEditUnit m_edtMinStepSize;
	CFormulaEditSpin m_edtMaxIteration;
	BOOL      m_bDispNorm;
	BOOL      m_bForcNorm;
	BOOL      m_bEnrgNorm;
	CEditUnit m_edtDispNorm;
	CEditUnit m_edtForcNorm;
	CEditUnit m_edtEnrgNorm;
	CEditUnit m_edtMaxStepSize;
	int       m_nRungeKutta;
	CEditUnit m_edtRungeKutaTolerance;
	BOOL      m_bUseLineSearch;
	CEditUnit m_edtLineSearchNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThisIterationCtrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	T_THIS_D *m_pData;

	CArray<UINT, UINT> m_aLineSearch;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThisIterationCtrlDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCmdNonlinearChk1();
	afx_msg void OnCmdNonlinearChk2();
	afx_msg void OnCmdNonlinearChk3();
	afx_msg void OnUseLineSearch();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THISITERATIONCTRLDLG_H__0DFAC801_CC31_4249_BEC4_D19254E58821__INCLUDED_)
