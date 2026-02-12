#if !defined(AFX_CMTHRDLOADINCREMENT_H__CC931933_F5BF_4980_B98D_10415BF8D748__INCLUDED_)
#define AFX_CMTHRDLOADINCREMENT_H__CC931933_F5BF_4980_B98D_10415BF8D748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThrdLoadIncrement.h : header file
//
#include "..\wg_db\wg_db_EditUnit.h"

#include "CMChildBarBase.h"

#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;
/////////////////////////////////////////////////////////////////////////////
// CCMThrdLoadIncrement dialog

class CCMThrdLoadIncrement : public CCMChildBarBase
{
// Construction
public:
	CCMThrdLoadIncrement(CWnd* pParent = NULL);   // standard constructor
	virtual void	Execute(){ OnCmdOk(); }
	virtual BOOL ExternalInit(UINT key);

	BOOL      m_bModify;

// Dialog Data
	//{{AFX_DATA(CCMThrdLoadIncrement)
	enum { IDD = IDD_DBAR_THRD_LOAD_INC };
	MComboBox	m_cmbLC;
	CEditUnit	m_edtScaleFactor;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThrdLoadIncrement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void MakeLCCombo();
	void Data2Dlg(T_THRD_D& rData);
	void Dlg2Data(T_THRD_D& rData);

	CString strOldName;

	// Generated message map functions
	//{{AFX_MSG(CCMThrdLoadIncrement)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOk();
	afx_msg void OnCmdCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHRDLOADINCREMENT_H__CC931933_F5BF_4980_B98D_10415BF8D748__INCLUDED_)
