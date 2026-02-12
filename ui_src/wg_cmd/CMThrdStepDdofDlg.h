#if !defined(AFX_CMTHRDSTEPDDOFDLG_H__9903B672_F7DE_4C4B_B383_D33710B0A8AA__INCLUDED_)
#define AFX_CMTHRDSTEPDDOFDLG_H__9903B672_F7DE_4C4B_B383_D33710B0A8AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdStepDdofDlg.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepDdofDlg dialog

class CCMThrdStepDdofDlg : public CCMChildBarBase
{
// Construction
public:
	CCMThrdStepDdofDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMThrdStepDdofDlg)
	enum { IDD = IDD_DBAR_THRD_STEP_DDOF };
	CSelectLC	m_cmbLoadCase;
	MComboBox	m_cmbValue;
	MComboBox	m_cmbComponents;
	MComboBox	m_cmbPoint;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdStepDdofDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_THRD_D		m_pDlgData;
	BOOL        m_bModify;
	CString strOldName;

	void MakeCombo();
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();

	// Generated message map functions
	//{{AFX_MSG(CCMThrdStepDdofDlg)
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDSTEPDDOFDLG_H__9903B672_F7DE_4C4B_B383_D33710B0A8AA__INCLUDED_)
