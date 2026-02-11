#if !defined(__CMMVCTRUSDLG_H__)
#define __CMMVCTRUSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctRusDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctRusDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvctRusDlg : public CDialogMove
{
// Construction
public:
	CCMMvctRusDlg(CWnd* pParent = NULL);   // standard constructor

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
	T_MVCT_D m_Data;
	CArray<UINT, UINT> m_aCtrlMethod;
	CArray<UINT, UINT> m_aCtrlPosFrame;
	CArray<UINT, UINT> m_aCtrlPosPlate;
	CArray<UINT, UINT> m_aCtrlIGPMethod;

// Dialog Data
	//{{AFX_DATA(CCMMvctRusDlg)
	enum { IDD = IDD_CMD_ML_MVCT_RUS };
	CEditUnit m_edtIGPDist;
	CTextUnit m_untIGPDist;
	CFormulaEditSpin m_spinGPN;
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
	mit::frx::MButton m_chkStressCalcFrame;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;
	int		m_nLoadPointSel;
	mit::frx::MComboBox m_cmbMatlType;
	mit::frx::MComboBox m_cmbBridgeType;
	CEditUnit m_edtDynaFact;
	CEditUnit m_editMaxVehicle;
	CFormulaEditSpin m_spinMaxVehicle;
	CEditUnit m_edtMaxSpacing;
	CTextUnit m_untMaxSpacing;
	CFormulaEditSpin m_spinIncrementVehicle;
	mit::frx::MComboBox m_cmbMatlType_AK;
	mit::frx::MComboBox m_cmbBridgeType_AK;
	CEditUnit m_edtDynaFact_AK;
	CEditUnit m_edtMinFactorS2;
	
	CFoldGroupBox_Cross	m_chkFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctRusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT,UINT> m_arReaction;
	CArray<UINT,UINT> m_arDisplacement;
	CArray<UINT,UINT> m_arForceMoment;
	CArray<UINT,UINT> m_arLink;
	CArray<UINT,UINT> m_aAnalysis;
	CArray<UINT,UINT> m_arRussia;
	CArray<UINT,UINT> m_arRussia2_TramcarControl;
	CArray<UINT,UINT> m_arLineOption;

	void InitGroupData();
	void InitComboBox();
	void AlignControl();  // 임시로 Influence Line Option 관련 Control을 숨긴다.
	void ResetDynamicFactorData(BOOL bMatl);
	void ResetAKDynamicFactorData(BOOL bMatl);

	// Generated message map functions
	//{{AFX_MSG(CCMMvctRusDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdMvctFiltersReaction();
	afx_msg void OnCmdMvctFiltersDisplacement();
	afx_msg void OnCmdMvctFiltersForce();
	afx_msg void OnCmdMvctFiltersLink();
	//afx_msg void OnDeltaposCmdMvctOptionSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdMvctOk();
	afx_msg void OnCmdMvctCancel();
	afx_msg void OnCmdMvctFiltersReactions();
	afx_msg void OnCmdMvctFiltersDisplacements();
	afx_msg void OnCmdMvctFiltersForceMoments();
	afx_msg void OnCmdMvctFiltersLinks();
	afx_msg void OnChangeIGPMethod();
	afx_msg void OnCmbMvctMatlTypeCmb();
	afx_msg void OnCmbMvctBridgeTypeCmb();
	afx_msg void OnCmbMvctAKMatlTypeCmb();
	afx_msg void OnCmbMvctAKBridgeTypeCmb();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTRUSDLG_H__)
