#if !defined(AFX_CMMVCTTRDLG_H__69BC9357_DB0C_4DCF_8525_46894AC6E0CA__INCLUDED_)
#define AFX_CMMVCTTRDLG_H__69BC9357_DB0C_4DCF_8525_46894AC6E0CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctTrDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMMvctTrDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvctTrDlg : public CDialogMove
{
// Construction
public:
	CCMMvctTrDlg(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CCMMvctTrDlg)
	enum { IDD = IDD_CMD_ML_MVCT_TR };
	mit::frx::MButton	m_chkDisp;
	mit::frx::MButton	m_chkForce;
	mit::frx::MButton	m_chkReact;
	mit::frx::MButton	m_chkCombinedStress;

	CFormulaEditSpin	m_spinIGM1;
	CEditUnit	m_edtIGM2;
	CTextUnit m_untIGM2;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctTrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvctTrDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChangeIGM();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	T_MVCTtr_D m_Data;
	CDBDoc* m_pDoc;

	CArray<UINT, UINT> m_nLoadPointSelection;
	CArray<UINT, UINT> m_nAnalysisResult;
	CArray<UINT, UINT> m_aCtrlIGM;
public:
	void Data2Dlg();
	BOOL Dlg2Data();
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVCTTRDLG_H__69BC9357_DB0C_4DCF_8525_46894AC6E0CA__INCLUDED_)
