#if !defined(__CMMvctJPNewDlg_H__)
#define __CMMvctJPNewDlg_H__

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

using namespace frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPNewDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvctJPNewDlg : public CDialogMove
{
// Construction
public:
	CCMMvctJPNewDlg(CWnd* pParent = NULL);   // standard constructor

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

// Dialog Data
	//{{AFX_DATA(CCMMvctJPNewDlg)
	enum { IDD = IDD_CMD_ML_MVCT_JAPAN_NEW };

	CFormulaEditSpin m_spinGPN;
	CEditUnit m_editIGPN;
	MComboBox	m_cboForceMoment;
	MComboBox	m_cboDisplacement;
	MComboBox	m_cboReaction;
	MComboBox m_cboLink;
	MButton	m_chkForceMoment;
	MButton	m_chkDisplacement;
	MButton	m_chkReaction;
	MButton	m_chkLink;
	MButton m_chkConcurrentLink;
	MButton m_chkStressCalcFrame;
	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;
		
	CFoldGroupBox_Cross	m_chkFilter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvctJPNewDlg)
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
	void AlignControl();  // 임시로 Influence Line Option 관련 Control을 숨긴다.

	// Generated message map functions
	//{{AFX_MSG(CCMMvctJPNewDlg)
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

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMvctJPNewDlg_H__)
