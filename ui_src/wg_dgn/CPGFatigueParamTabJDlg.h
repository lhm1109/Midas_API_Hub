#if !defined(AFX_CPGFatigueParamTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
#define AFX_CPGFatigueParamTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPGFatigueParamTabJDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabJDlg dialog

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

class __MY_EXT_CLASS__ CCPGFatigueParamTabJDlg : public CChildDialog
{
// Construction
public:
	CCPGFatigueParamTabJDlg(CWnd* pParent = NULL, int* pElemType = NULL);   // standard constructor

	
// Dialog Data
	//{{AFX_DATA(CCPGFatigueParamTabJDlg)
	enum { IDD = IDD_DGN_CPG_FATIGUE_PARAM_TAB_J_DLG };
	
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
	//{{AFX_VIRTUAL(CCPGFatigueParamTabJDlg)
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
	//{{AFX_MSG(CCPGFatigueParamTabJDlg)
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

#endif // !defined(AFX_CPGFatigueParamTabJDlg_H__823E992D_C5CC_4048_B8B3_21B0B19F8F64__INCLUDED_)
