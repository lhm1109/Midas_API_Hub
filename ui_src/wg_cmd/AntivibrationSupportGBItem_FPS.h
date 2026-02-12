#if !defined(AFX_ANTIVIBRATIONSUPPORTGBITEM_FPS_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
#define AFX_ANTIVIBRATIONSUPPORTGBITEM_FPS_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtGapDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\mit_frx\MComboBox.h"
using namespace frx;
/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_FPS dialog

class CAntivibrationSupportGBItem_FPS : public CChildDialog
{
// Construction
public:
	CAntivibrationSupportGBItem_FPS(CWnd* pParent = NULL);   // standard constructor
	~CAntivibrationSupportGBItem_FPS();

// Dialog Data
	//{{AFX_DATA(CAntivibrationSupportGBItem_FPS)
	enum {IDD = IDD_ETC_ANTISUPPORT_GB_FPS};
	MComboBox	m_wndEffiRComb; 
	CEditUnit	m_wndEffiREdit;
	CEditUnit	m_wndMechKvcEdit_FPS;
	CEditUnit	m_wndMechKvtEdit_FPS;
	CEditUnit	m_wndMechKhEdit_FPS;
	CEditUnit	m_wndMechrEdit_FPS;
	MComboBox	m_wndMechUs_FPS;
	MComboBox	m_wndMechUf_FPS;
	CEditUnit	m_wndMechUsEdit_FPS;
	CEditUnit	m_wndMechUfEdit_FPS;
	CEditUnit	m_wndTk;
	CEditUnit	m_wndTq;
	CEditUnit	m_wndDgnHDistEdit_FPS;
	CEditUnit	m_wndDgnVBearEdit_FPS;

	CTextUnit	m_wndEffiRUnit;
	CTextUnit	m_wndMechKvcUnit_FPS;
	CTextUnit	m_wndMechKvtUnit_FPS;
	CTextUnit	m_wndMechDyUnit_FPS;
	CTextUnit	m_wndMechrUnit_FPS;
	CTextUnit	m_wndDgnHDistUnit_FPS;
	CTextUnit	m_wndDgnVBearUnit_FPS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntivibrationSupportGBItem_FPS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	T_ASGB_D m_Data;
	int m_nDofType;  
	int m_nCodeType;
	void SetData2Dlg(T_ASGB_D& data);
	BOOL SetDlg2Data(T_ASGB_D& data);
	void EnableDisableItemCtrls(int nInputMethod);
protected:
	// Generated message map functions
	//{{AFX_MSG(CAntivibrationSupportGBItem_FPS)
	afx_msg void OnSelChangeBasicEffiRCmb();
	afx_msg void OnSelChangeMechUsCmb();
	afx_msg void OnSelChangeMechUfCmb();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	void Data2Dlg();
	BOOL Dlg2Data();
	void InitBasicEffiRCmb();
	void InitMechUfCmb();
	void InitMechUsCmb();

	CDBDoc* m_pDoc;
	double ConvertUnitROut(double dR);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIVIBRATIONSUPPORTGBITEM_FPS_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
