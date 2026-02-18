#pragma once

// CPGFatigueParamTabIDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabIDlg_CSA dialog

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

class __MY_EXT_CLASS__ CCPGFatigueParamTabIDlg_CSA : public CChildDialog
{
	// Construction
public:
	CCPGFatigueParamTabIDlg_CSA(CWnd* pParent = NULL);   // standard constructor


	// Dialog Data
	//{{AFX_DATA(CCPGFatigueParamTabIDlg_CSA)
	enum { IDD = IDD_DGN_CPG_FATIGUE_PARAM_TAB_I_CSA_DLG };

	mit::frx::MComboBox m_cmbCategory;
	mit::frx::MComboBox m_cmbGirderType;
	mit::frx::MComboBox m_cmbDailyTruck;

	CEditUnit	m_edtWeightTruck;
	CTextUnit m_untWeightTruck;
	double    m_dDesignLife;

	BOOL      m_bAutoChk;
	double   	m_dNdValue;
	double   	m_dPValue;
	CFoldGroupBox_Cross	m_chkGirderGroup;
	CFoldGroupBox_Cross	m_chkDailyGroup;
	CArray <UINT, UINT> arWarpingCtrlID;

	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPGFatigueParamTabIDlg_CSA)
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
	CDBDoc *m_pDoc;
	void InitCombo();
	void InitUnit();
	void InitChk();
	void Data2Dlg();
	BOOL Dlg2Data();

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCPGFatigueParamTabIDlg_CSA)
	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
	afx_msg void OnWarpingStressTypeRdo();
	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDgnFatigueAutoChk();
};

/////////////////////////////////
#include "HeaderPost.h"   //////////
/////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

