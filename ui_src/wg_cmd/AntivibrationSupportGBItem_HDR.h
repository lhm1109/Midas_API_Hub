#if !defined(AFX_ANTIVIBRATIONSUPPORTGBITEM_HDR_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
#define AFX_ANTIVIBRATIONSUPPORTGBITEM_HDR_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtGapDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CAntivibrationSupportGBItem_HDR dialog

class CAntivibrationSupportGBItem_HDR : public CChildDialog
{
// Construction
public:
	CAntivibrationSupportGBItem_HDR(CWnd* pParent = NULL);   // standard constructor
	~CAntivibrationSupportGBItem_HDR();

// Dialog Data
	//{{AFX_DATA(CAntivibrationSupportGBItem_HDR)
	enum {IDD = IDD_ETC_ANTISUPPORT_GB_HDR};
	CEditUnit	m_wndEffiDEdit;
	CEditUnit	m_wndEffiTrEdit;
	CEditUnit	m_wndMechKvcEdit;
	CEditUnit	m_wndMechKvtEdit;
	CEditUnit	m_wndMechK1Edit;
	CEditUnit	m_wndMechKdEdit;
	CEditUnit	m_wndMechOdEdit;
	CEditUnit	m_wndTk;
	CEditUnit	m_wndTq;
	CEditUnit	m_wndDgnHEquiStiffEdit_HDR;
	CEditUnit	m_wndEquiRto_HDR;
	CEditUnit	m_wndS2;
	CEditUnit	m_wndS1;

	CTextUnit	m_wndEffiDUnit;
	CTextUnit	m_wndEffiTrUnit;
	CTextUnit	m_wndMechKvcUnit;
	CTextUnit	m_wndMechKvtUnit;
	CTextUnit	m_wndMechK1Unit;
	CTextUnit	m_wndMechKdUnit;
	CTextUnit	m_wndMechQdUnit;
	CTextUnit	m_wndDgnHEquiStiffUnit_HDR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAntivibrationSupportGBItem_HDR)
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
	//{{AFX_MSG(CAntivibrationSupportGBItem_HDR)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	void Data2Dlg();
	BOOL Dlg2Data();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANTIVIBRATIONSUPPORTGBITEM_HDR_H__E93289AF_D809_4145_94DE_FA909D7220BC__INCLUDED_)
