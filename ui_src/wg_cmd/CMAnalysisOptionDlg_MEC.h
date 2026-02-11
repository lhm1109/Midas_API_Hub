#if !defined(__CMANALYSISOPTIONDLG_MEC_H__)
#define			 __CMANALYSISOPTIONDLG_MEC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMAnalysisOptionDlg.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MButton.h"
#include "../wg_db/DB_ST_DT_PREF.h"

/////////////////////////////////////////////////////////////////////////////
// CCMAnalysisOptionDlg_MEC dialog

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMAnalysisOptionDlg_MEC : public CDialogMove
{
// Construction
public:
	CCMAnalysisOptionDlg_MEC(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMAnalysisOptionDlg_MEC() {};
// Dialog Data
	//{{AFX_DATA(CCMAnalysisOptionDlg_MEC)
	enum { IDD = IDD_ETC_ANALYSIS_OPTION_MEC };

	mit::frx::MComboBox m_cmbEquationSolver;
	mit::frx::MButton	m_ChkMultiProcessor;
	CFormulaEditSpin m_edtspinMultiProcess;
	mit::frx::MButton	m_chkGPUAcceleration;
	CEditUnit m_edtStiffnessDiagonal;
	CEditUnit m_edtSpringConstant;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMAnalysisOptionDlg_MEC)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL OnInitDlgAnalOptionMEC();
	BOOL OnInitDlgAnop();

	BOOL Dlg2Data();
	BOOL Dlg2DataAnalOption();
	BOOL Dlg2DataAnop();

	BOOL CheckAnalOption();

	void InitializeEquationSolverCmb();
	void EnableNumProcessor(BOOL bEnable);

	T_ANALOPTION_MEC m_AnalOptionMEC;
	T_ANOP_D m_AnopD;

	// Generated message map functions
	//{{AFX_MSG(CCMAnalysisOptionDlg_MEC)
	afx_msg void OnClickMultiProcessorChk();
	afx_msg void OnCmdAnalyoptOk();
	afx_msg void OnCmdAnalyoptCancel();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMANALYSISOPTIONDLG_H__DE9A3B81_5F27_11D4_9AAF_0000C0B9C58C__INCLUDED_)
