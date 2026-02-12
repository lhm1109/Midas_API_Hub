#if !defined(__CMMVCTCHDLG_H__)
#define __CMMVCTCHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctCHDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHDlg dialog
#include "HeaderPre.h"

class CCMMvctCHUserPage;
class CCMMvctCHSbemPage;
class CCMMvctCHCbemPage;
class CCMMvctCHArchPage;
class CCMMvctCHCablPage;
class CCMMvctCHSuspPage;
class CCMMvctCHRCPage;
class CCMMvctCHStlPage;
class CCMMvctCHUrbanPage;
class CCMMvctCHTrainPage;
class CCMMvctCHRailPage;


class __MY_EXT_CLASS__ CCMMvctCHDlg : public CDialogMove
{
// Construction
public:
	CCMMvctCHDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Interface
public:

//----------------------------------------------------------------------
// Implementation
protected:
	void Data2Dlg();
	BOOL Dlg2Data();

	void CreatePage_ImpactFactor();
	void Data2Dlg_ImpactFactor();
	BOOL Dlg2Data_ImpactFactor();
	void DestroyPage_ImpactFactor();

protected:
	CDBDoc* m_pDoc;
	T_MVCTch_D m_Data;
	CArray<UINT, UINT> m_aCtrlPosFrame;
	CArray<UINT, UINT> m_aCtrlPosPlate;
	CArray<UINT, UINT> m_aCtrlIGPMethod;

	int m_nCodeType, m_nBrgType, m_nSpanType, m_nFreqType, m_nArchType;

// Dialog Data
	//{{AFX_DATA(CCMMvctCHDlg)
	enum { IDD = IDD_CMD_ML_MVCT_CHINA };
	CEditUnit m_edtIGPDist;
	CTextUnit m_untIGPDist;
	CEditUnit m_edtArchL;
	CTextUnit m_untArchL;
	mit::frx::MComboBox	m_wndArchTypeCmb;
	mit::frx::MComboBox	m_wndSpanTypeCmb;
	mit::frx::MComboBox	m_wndFreqTypeCmb;
	mit::frx::MComboBox	m_wndBrgTypeCmb;
	mit::frx::MComboBox	m_wndCodeTypeCmb;
	int     m_nBrgClass;
	BOOL    m_bBrgClassChk;
	CFormulaEditSpin m_spinIGPN;
	mit::frx::MComboBox	m_cboForceMoment;
	mit::frx::MComboBox	m_cboDisplacement;
	mit::frx::MComboBox	m_cboReaction;
	mit::frx::MComboBox m_cboLink;
	mit::frx::MComboBox	m_cboArchGrup;
	mit::frx::MButton	m_chkForceMoment;
	mit::frx::MButton	m_chkDisplacement;
	mit::frx::MButton	m_chkReaction;
	mit::frx::MButton	m_chkLink;
	mit::frx::MButton m_chkStressCalc;
	mit::frx::MButton m_chkStressCalcFrame;
	mit::frx::MButton m_chkComponent;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;
	int		m_nLoadPointSel;	
	BOOL  m_bImpactFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctCHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT, UINT> m_arReaction;
	CArray<UINT, UINT> m_arDisplacement;
	CArray<UINT, UINT> m_arForceMoment;
	CArray<UINT,UINT> m_arLink;
	
	CArray<UINT, UINT> m_aCtrlCodeType;
	CArray<UINT, UINT> m_aCtrlFreqType;
	CArray<UINT, UINT> m_aCtrlArchType;
	CArray<UINT, UINT> m_aCtrlSpanType;
	CArray<UINT, UINT> m_aCtrlBrgType;
	CArray<UINT, UINT> m_aCtrlArchGrp;

	CArray<UINT, UINT> m_aCtrlBrgClass;
	
	CCMMvctCHUserPage*  m_pUserPage;
	CCMMvctCHSbemPage*  m_pSbemPage;
	CCMMvctCHCbemPage*  m_pCbemPage;
	CCMMvctCHArchPage*  m_pArchPage;
	CCMMvctCHCablPage*  m_pCablPage;
	CCMMvctCHSuspPage*  m_pSuspPage;
	CCMMvctCHRCPage*    m_pRCPage;
	CCMMvctCHStlPage*   m_pStlPage;
	CCMMvctCHUrbanPage* m_pUrbanPage;
	CCMMvctCHTrainPage* m_pTrainPage;
	CCMMvctCHTrainPage* m_pRailwayPage;
	CCMMvctCHRailPage*  m_pRailBrgPage;
	CCMMvctCHRailPage*  m_pRailCulPage;
	
	void InitGroupData();
	void AlignControl();    
	void InitSetCmb4ImpactFactor();
	void CtrlManager();

	void ChangeBridgeTypeCombo();
	// Generated message map functions
	//{{AFX_MSG(CCMMvctCHDlg)
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
	afx_msg void OnCmdMvctBrgClassCheck();
	afx_msg void OnSelchangeCmdMvctCodeTypeMtdCmb();
	afx_msg void OnSelchangeCmdMvctBrgTypeCmb();
	afx_msg void OnSelchangeCmdMvctSpanCalcMtdCmb();
	afx_msg void OnSelchangeCmdMvctFreqMtdCmb();
	afx_msg void OnSelchangeCmdMvctArchTypeCmb();
	afx_msg void OnChangeIGPMethod();
	afx_msg void OnCmdMvctImpactFactorChk();
	afx_msg void OnCmdComponentChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTDLG_H__)
