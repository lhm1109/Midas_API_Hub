#if !defined(__CMMVCTDLG_H__)
#define __CMMVCTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvctDlg : public CDialogMove
{
// Construction
public:
	CCMMvctDlg(CWnd* pParent = NULL);   // standard constructor

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
	//{{AFX_DATA(CCMMvctDlg)
	enum { IDD = IDD_CMD_ML_MVCT };
	CFormulaEditSpin m_spinMaxVehicle;
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
	mit::frx::MButton m_chkConcurrentLink;
	mit::frx::MButton m_chkStressCalcFrame;
	mit::frx::MButton m_chkMaxVehicle;
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
	CArray<UINT,UINT> m_arReaction;
	CArray<UINT,UINT> m_arDisplacement;
	CArray<UINT,UINT> m_arForceMoment;
	CArray<UINT,UINT> m_arLink;
	CArray<UINT,UINT> m_aAnalysis;

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
	afx_msg void OnCmdMvctMaxVehicleChk();
	afx_msg void OnChangeIGPMethod();	

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTDLG_H__)
