#if !defined(AFX_CPGShearConnTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGShearConnTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGShearConnTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGShearConnTabIDlg dialog

#include "..\wg_base\ChildDialog.h"

#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////
#include "HeaderPre.h"   //////////
/////////////////////////////////

class CCPGShearConnDlg;

class __MY_EXT_CLASS__ CCPGShearConnTabIDlg : public CChildDialog
{
// Construction
public:
	CCPGShearConnTabIDlg(CWnd* pParent = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGShearConnTabIDlg)
	enum { IDD = IDD_DGN_CPG_SHEAR_CONN_TAB_I_DLG };
	
	mit::frx::MComboBox m_cmbCategory;
	CEditUnit m_edtPitch;
	CTextUnit m_untPitch;
	CEditUnit m_edtHeight;
	CTextUnit m_untHeight;
	CEditUnit m_edtDia;
	CTextUnit m_untDia;	
	CEditUnit m_edtFu;
	CTextUnit m_untFu;	
	CEditUnit m_edtSpace;
	CTextUnit m_untSpace;
	CEditUnit m_edtNum;
	CEditUnit m_edtLength;
	CTextUnit m_untLength;
	CEditUnit m_edtFrcForce;
	CTextUnit m_untFrcForce;
	int       m_nSFCalc;

	CFoldGroupBox_Cross	m_chkGroup;
	CFoldGroupBox_Cross	m_chkFrc;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGShearConnTabIDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void GetCpscData();
	void SetCpscData();
	void ControlsEnableDisable();
	void ResizeDialog();
	void  ResizeWindow();
	CRect GetResizeHeight();

	CCPGShearConnDlg* m_pParent;
	T_CPSC_TYPE m_Data;

protected:
	void InitCombo();
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCPGShearConnTabIDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShearConnectDiaBtn();
	afx_msg void OnShearConnectFuBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPGShearConnTabIDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
