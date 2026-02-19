#pragma once

// CPGFatigueParamTabIDlg_AASHTO19.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabIDlg_AASHTO19 dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"


/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGFatigueParamDlg;
class CDBDoc;

class __MY_EXT_CLASS__ CCPGFatigueParamTabIDlg_AASHTO19 : public CChildDialog
{
// Construction
public:
	CCPGFatigueParamTabIDlg_AASHTO19(CWnd* pParent = NULL, int nIJ=0);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGFatigueParamTabIDlg_AASHTO19)
	enum { IDD = IDD_DGN_CPG_FATIGUE_PARAM_TAB_I_AASHTO19_DLG };
	
	CEditUnit m_dNL;
	CEditUnit	m_dADTTpr;
	CEditUnit	m_dADTTsl;
	CEditUnit	m_dNum;

	BOOL      m_bTopF_Top;
	BOOL      m_bTopF_Bot;
	BOOL      m_bBotF_Top;
	BOOL      m_bBotF_Bot;

	mit::frx::MComboBox m_cmbTopF_Top;
	mit::frx::MComboBox m_cmbTopF_Bot;
	mit::frx::MComboBox m_cmbBotF_Top;
	mit::frx::MComboBox m_cmbBotF_Bot;

	int       m_nWarpType;
	CEditUnit m_edtTopFlange;
	CTextUnit m_untTopFlange;

	CEditUnit m_edtTopFlangeBot;
	CTextUnit m_untTopFlangeBot;

	CEditUnit m_edtBotFlangeTop;
	CTextUnit m_untBotFlangeTop;

	CEditUnit m_edtBotFlange;
	CTextUnit m_untBotFlange;

	CFoldGroupBox_Cross	m_chkGroup;
	CArray <UINT, UINT> m_arWarpingCtrlID;

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGFatigueParamTabIDlg_AASHTO19)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCpfpData();
	void SetCpfpData();
	void ControlsEnableDisable();
	void  ResizeWindow();
	CRect GetResizeHeight();

	CCPGFatigueParamDlg* m_pParent;
	T_CPFP_TYPE m_Data;

protected:
	int m_nIJ;
	CDBDoc *m_pDoc;
	void InitCombo();
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGFatigueParamTabIDlg_AASHTO19)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();

	afx_msg void OnTopF_Top_Chk();
	afx_msg void OnTopF_Bot_Chk();
	afx_msg void OnBotF_Top_Chk();
	afx_msg void OnBotF_Bot_Chk();

	afx_msg void OnWarpingStressTypeRdo();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

