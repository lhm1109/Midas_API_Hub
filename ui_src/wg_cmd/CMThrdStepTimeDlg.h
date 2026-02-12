#if !defined(AFX_CMTHRDSTEPTIMEDLG_H__D06E141D_BE1D_4A69_99E1_55FC9A8071E3__INCLUDED_)
#define AFX_CMTHRDSTEPTIMEDLG_H__D06E141D_BE1D_4A69_99E1_55FC9A8071E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdStepTimeDlg.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepTimeDlg dialog

class CCMThrdStepTimeDlg : public CCMChildBarBase
{
// Construction
public:
	CCMThrdStepTimeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMThrdStepTimeDlg)
	enum { IDD = IDD_DBAR_THRD_STEP_TIME };
	MComboBox	m_cmbTimeStep;
	CSelectLC	m_cmbLoadCase;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdStepTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_THRD_D		m_pDlgData;
	BOOL        m_bModify;
	CString strOldName;

	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();

	// Generated message map functions
	//{{AFX_MSG(CCMThrdStepTimeDlg)
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmdBarThloadcase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDSTEPTIMEDLG_H__D06E141D_BE1D_4A69_99E1_55FC9A8071E3__INCLUDED_)
