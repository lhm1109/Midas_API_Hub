#if !defined(AFX_CMTHRDSTEPWALLDLG_H__64DFBEF7_F150_43C2_972B_18A45239CB0A__INCLUDED_)
#define AFX_CMTHRDSTEPWALLDLG_H__64DFBEF7_F150_43C2_972B_18A45239CB0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdStepBeamDlg.h : header file
//

#include "..\wg_base\wg_base_MouseEdit.h"
#include "..\wg_db\wg_db_SelectLC.h"
#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepWallDlg dialog

class CCMThrdStepWallDlg : public CCMChildBarBase
{
// Construction
public:
	CCMThrdStepWallDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void	Execute(){ OnCmdApply(); }
	virtual BOOL ExternalInit(UINT key);

// Dialog Data
	//{{AFX_DATA(CCMThrdStepWallDlg)
	enum { IDD = IDD_DBAR_THRD_STEP_WALL };
	CSelectLC	m_cmbLoadCase;
	MComboBox	m_cmbValue;
	MComboBox	m_cmbComponents;
	MComboBox	m_cmbPoint;
	CMouseEdit	m_ctrlElemNo;
	CString	m_strName;
	int		m_nResultType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdStepWallDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString strOldName;
	T_THRD_D		m_pDlgData;
	BOOL        m_bModify;

	void MakeCombo();
	void MakeComponentCombo(int nType);
	void DataInit();
	void Data2Dlg();
	BOOL Dlg2Data();
	BOOL OnExecute();

	// Generated message map functions
	//{{AFX_MSG(CCMThrdStepWallDlg)
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdOK();
	afx_msg void OnCmdApply();
	afx_msg void OnChangeType();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDSTEPBEAMDLG_H__64DFBEF7_F150_43C2_972B_18A45239CB0A__INCLUDED_)
