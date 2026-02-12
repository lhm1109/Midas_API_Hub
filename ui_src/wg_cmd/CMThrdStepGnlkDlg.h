#if !defined(AFX_CMTHRDSTEPGNLKDLG_H__F6C08D10_B809_4FD9_87C8_D0DBD3E00B8C__INCLUDED_)
#define AFX_CMTHRDSTEPGNLKDLG_H__F6C08D10_B809_4FD9_87C8_D0DBD3E00B8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdStepGnlkDlg.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepGnlkDlg dialog

class CCMThrdStepGnlkDlg : public CCMChildBarBase
{
// Construction
public:
	CCMThrdStepGnlkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMThrdStepGnlkDlg)
	enum { IDD = IDD_DBAR_THRD_STEP_GNLK };
	CSelectLC	m_cmbLoadCase;
	MComboBox	m_cbxNllk;
	MComboBox	m_cmbValue;
	MComboBox	m_cmbComponents;
	MComboBox	m_cmbPoint;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdStepGnlkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	T_THRD_D		m_pDlgData;
	BOOL        m_bModify;
	CString strOldName;

	void MakeCombo();
	void MakeNllkKeyList();
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();

	// Generated message map functions
	//{{AFX_MSG(CCMThrdStepGnlkDlg)
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDSTEPGNLKDLG_H__F6C08D10_B809_4FD9_87C8_D0DBD3E00B8C__INCLUDED_)
