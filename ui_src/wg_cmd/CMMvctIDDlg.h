#if !defined(__CMMVCTIDDLG_H__)
#define __CMMVCTIDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctIDDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctIDDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvctIDDlg : public CDialogMove
{
// Construction
public:
	CCMMvctIDDlg(CWnd* pParent = NULL);   // standard constructor

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
	T_MVCTid_D m_Data;

	CArray<UINT, UINT> m_aCtrlPosFrame;
	CArray<UINT, UINT> m_aCtrlPosPlate;
	CArray<UINT, UINT> m_aCtrlBrgType;
	CArray<UINT, UINT> m_aCtrlIGPMethod;

// Dialog Data
	//{{AFX_DATA(CCMMvctIDDlg)
	enum { IDD = IDD_CMD_ML_MVCT_INDIA };
	//int m_nBrgType;
	CEditUnit m_edtIGPDist;
	CTextUnit m_untIGPDist;
	CFormulaEditSpin m_spinMaxVehicle;
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
	mit::frx::MButton m_chkStressCalcFrame;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;
	mit::frx::MComboBox m_cmbTracks;
	mit::frx::MComboBox m_cmbWidth;
	CEditUnit m_edtWidth;
	CTextUnit m_untWidth;
	CEditUnit m_edtDepth;
	CTextUnit m_untDepth;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT,UINT> m_arReaction;
	CArray<UINT,UINT> m_arDisplacement;
	CArray<UINT,UINT> m_arForceMoment;
	CArray<UINT,UINT> m_arLink;

	void InitCombo();

	// Generated message map functions
	//{{AFX_MSG(CCMMvctIDDlg)
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
	afx_msg void OnCmdMvctBridgeTypeRdo();
	afx_msg void OnCmdMvctSleeperWidthCmb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTIDDLG_H__)
