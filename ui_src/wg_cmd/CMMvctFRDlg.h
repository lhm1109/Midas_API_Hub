#if !defined(__CMMVCTFRDLG_H__)
#define __CMMVCTFRDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctFRDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctFRDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvctFRDlg : public CDialogMove
{
	// Construction
public:
	CCMMvctFRDlg(CWnd* pParent = NULL);   // standard constructor
	
//----------------------------------------------------------------------
// Interface
public:

	//----------------------------------------------------------------------
	// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();

protected:
	CDBDoc* m_pDoc;
	T_MVCTfr_D m_Data;
	CArray<UINT, UINT> m_aCtrlClass;
	CArray<UINT, UINT> m_aCtrlPosFrame;
	CArray<UINT, UINT> m_aCtrlPosPlate;
	CArray<UINT, UINT> m_aCtrlIGPMethod;

	// Dialog Data
		//{{AFX_DATA(CCMMvctDlg)
	enum { IDD = IDD_CMD_ML_MVCT_FR };
	CEditUnit m_edtIGPDist;
	CTextUnit m_untIGPDist;
	CFormulaEditSpin m_spinIGPN;
	mit::frx::MComboBox	m_cboForceMoment;
	mit::frx::MComboBox	m_cboDisplacement;
	mit::frx::MComboBox	m_cboReaction;
	mit::frx::MComboBox m_cboLink;
	mit::frx::MButton	m_chkForceMoment;
	mit::frx::MButton	m_chkDisplacement;
	mit::frx::MButton	m_chkReaction;
	mit::frx::MButton	m_chkLink;
	mit::frx::MButton m_chkStressCalc;
	mit::frx::MButton m_chkConcurrentCalc;
	mit::frx::MButton m_chkConcurrentLink;
	mit::frx::MButton m_chkStressCalcFrame;
	CFormulaEditSpin m_spinMSMV;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;
	int		m_nLoadPointSel;

	CFoldGroupBox_Cross	m_chkFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_arReaction;
	CArray<UINT, UINT> m_arDisplacement;
	CArray<UINT, UINT> m_arForceMoment;
	CArray<UINT, UINT> m_arLink;
	CArray<UINT, UINT> m_aAnalysis;

	void InitGroupData();
	void AlignControl();  // 임시로 Influence Line Option 관련 Control을 숨긴다.

	// Generated message map functions
	//{{AFX_MSG(CCMMvctDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvctFiltersReaction();
	afx_msg void OnCmdMvctFiltersDisplacement();
	afx_msg void OnCmdMvctFiltersForce();
	afx_msg void OnCmdMvctFiltersLink();
	afx_msg void OnCmdMvctOk();
	afx_msg void OnCmdMvctCancel();
	afx_msg void OnCmdMvctFiltersReactions();
	afx_msg void OnCmdMvctFiltersDisplacements();
	afx_msg void OnCmdMvctFiltersForceMoments();
	afx_msg void OnCmdMvctFiltersLinks();
	afx_msg void OnChangeIGPMethod();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVCTFRDLG_H__19488B30_1026_423F_B720_EBEE876FE673__INCLUDED_)
