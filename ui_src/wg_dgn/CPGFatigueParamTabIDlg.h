#pragma once

// CPGFatigueParamTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabIDlg dialog

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

class __MY_EXT_CLASS__ CCPGFatigueParamTabIDlg : public CChildDialog
{
// Construction
public:
	CCPGFatigueParamTabIDlg(CWnd* pParent = NULL, int* pElemType=NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGFatigueParamTabIDlg)
	enum { IDD = IDD_DGN_CPG_FATIGUE_PARAM_TAB_I_DLG };
	
	mit::frx::MComboBox m_cmbCategory;
	CEditUnit	m_dADTT;
	CEditUnit	m_dNum;
	int       m_nWarpType;
	CEditUnit m_edtTopFlange;
	CTextUnit m_untTopFlange;
	CEditUnit m_edtBotFlange;
	CTextUnit m_untBotFlange;

	CFoldGroupBox_Cross	m_chkGroup;
	CArray <UINT, UINT> arWarpingCtrlID;

		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGFatigueParamTabIDlg)
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
	int* m_pElemType;

protected:
	CDBDoc *m_pDoc;
	void InitCombo();
	void InitUnit();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGFatigueParamTabIDlg)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
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

