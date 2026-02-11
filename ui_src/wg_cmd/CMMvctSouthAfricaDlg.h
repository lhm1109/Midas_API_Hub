#if !defined(__CMMVCTSOUTHAFRICADLG_H__)
#define __CMMVCTSOUTHAFRICADLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvctSouthAfricaDlg.h : header file
//

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvctSouthAfricaDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvctSouthAfricaDlg : public CDialogMove
{
// Construction
public:
	CCMMvctSouthAfricaDlg(CWnd* pParent = NULL);   // standard constructor

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
	CArray<UINT, UINT> m_aCtrlPosFrame;
	CArray<UINT, UINT> m_aCtrlPosPlate;
	CArray<UINT, UINT> m_aCtrlIGPMethod;

// Dialog Data
	//{{AFX_DATA(CCMMvctSouthAfricaDlg)
	enum { IDD = IDD_CMD_ML_MVCT_SOUTH_AFRICA };
	//int m_nBrgType;
	CEditUnit m_edtIGPDist;
	CTextUnit m_untIGPDist;
	CEdit m_editMaxVehicle;  
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
	mit::frx::MButton m_chkStressCalcFrame;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctSouthAfricaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<UINT,UINT> m_arReaction;
	CArray<UINT,UINT> m_arDisplacement;
	CArray<UINT,UINT> m_arForceMoment;
	CArray<UINT,UINT> m_arLink;

	void InitGroupData();
	//void AlignControl();  // 임시로 Influence Line Option 관련 Control을 숨긴다.

	// Generated message map functions
	//{{AFX_MSG(CCMMvctSouthAfricaDlg)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnCmdMvctMethod();
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVCTSOUTHAFRICADLG_H__)
