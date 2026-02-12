#if !defined(AFX_ANTIVIBRATIONSUPPORTGBITEM_ESB_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
#define AFX_ANTIVIBRATIONSUPPORTGBITEM_ESB_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

class CAntivibrationSupportGBItem_ESB : public CChildDialog
{
// Construction
public:
	CAntivibrationSupportGBItem_ESB(CWnd* pParent = NULL);   // standard constructor
	~CAntivibrationSupportGBItem_ESB();

// Dialog Data
	//{{AFX_DATA(CAntivibrationSupportGBItem_ESB)
	enum {IDD = IDD_ETC_ANTISUPPORT_GB_ESB};
	CEditUnit	m_wndEffiDEdit;
	CEditUnit	m_wndEffiTrEdit;
	CEditUnit	m_wndMechKvcEdit_ESB;
	CEditUnit	m_wndMechKvtEdit_ESB;
	CEditUnit	m_wndMechK1Edit_ESB;
	CEditUnit	m_wndTk;
	CEditUnit	m_wndTq;
	MComboBox	m_wndMechUComb_ESB;

	CTextUnit	m_wndEffiDUnit;
	CTextUnit	m_wndEffiTrUnit;
	CTextUnit	m_wndMechKvcUnit_ESB;
	CTextUnit	m_wndMechKvtUnit_ESB;
	CTextUnit	m_wndMechK1Unit_ESB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntivibrationSupportGBItem_ESB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_ASGB_D m_Data;

	void SetData2Dlg(T_ASGB_D& data);
	BOOL SetDlg2Data(T_ASGB_D& data);
	void EnableDisableItemCtrls(int nInputMethod);
protected:
	// Generated message map functions
	//{{AFX_MSG(CAntivibrationSupportGBItem_ESB)
	afx_msg void OnSelChangeMechUCmb();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InitMechUCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIVIBRATIONSUPPORTGBITEM_ESB_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
