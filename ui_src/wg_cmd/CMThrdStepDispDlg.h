#if !defined(AFX_CMTHRDSTEPDISPDLG_H__D9169F05_7FDB_44A2_862D_D9DC7391A618__INCLUDED_)
#define AFX_CMTHRDSTEPDISPDLG_H__D9169F05_7FDB_44A2_862D_D9DC7391A618__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdStepDispDlg.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepDispDlg dialog

class CCMThrdStepDispDlg : public CCMChildBarBase
{
// Construction
public:
	CCMThrdStepDispDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMThrdStepDispDlg)
	enum { IDD = IDD_DBAR_THRD_STEP_DISP };
	CSelectLC	m_cmbLoadCase;
	MComboBox	m_cmbValue;
	MComboBox	m_cmbComponents;
	CMouseEdit	m_NodeNum;
	CString	m_strName;
	int		m_nResultType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdStepDispDlg)
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
	//{{AFX_MSG(CCMThrdStepDispDlg)
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDSTEPDISPDLG_H__D9169F05_7FDB_44A2_862D_D9DC7391A618__INCLUDED_)
