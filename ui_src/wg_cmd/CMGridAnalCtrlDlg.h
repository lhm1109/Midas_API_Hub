#if !defined(__CMGridAnalCtrlDlg_H__)
#define __CMGridAnalCtrlDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMGridAnalCtrlDlg.h : header file
//

#include "CMDlgBase.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditUnit.h"
#include "..\wg_db\TextUnit.h"
#include "..\wg_base\FoldGroupBox.h"

#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"
using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMGridAnalCtrlDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMGridAnalCtrlDlg : public CDialogMove
{
	// Construction
public:
	CCMGridAnalCtrlDlg(CWnd* pParent = NULL);   // standard constructor

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
	T_GACD_D m_Data;

	// Dialog Data
	//{{AFX_DATA(CCMGridAnalCtrlDlg)
	enum { IDD = IDD_CMD_GRID_ANAL_CTRL_DLG };

	int		m_nReaction;
	int		m_nDisplacement;
	int		m_nForceMoment;
	int		m_nLink;

	MComboBox	m_cboForceMoment;
	MComboBox	m_cboDisplacement;
	MComboBox	m_cboReaction;
	MComboBox	m_cboLink;
	
	MButton	m_chkForceMoment;
	MButton	m_chkDisplacement;
	MButton	m_chkReaction;
	MButton	m_chkLink;

	CEditUnit m_edtNum;
	CEditUnit m_edtDist;
	CTextUnit m_untDist;

	//CFoldGroupBox_Cross	m_chkFilter;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridAnalCtrlDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:
	CArray<UINT,UINT> m_arAnalCtrl;
	CArray<UINT,UINT> m_arReaction;
	CArray<UINT,UINT> m_arDisplacement;
	CArray<UINT,UINT> m_arForceMoment;
	CArray<UINT,UINT> m_arLink;

	void InitUnit();
	void InitGroupData();

	// Generated message map functions
	//{{AFX_MSG(CCMGridAnalCtrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdFiltersReaction();
	afx_msg void OnCmdFiltersDisplacement();
	afx_msg void OnCmdFiltersForce();
	afx_msg void OnCmdFiltersLink();

	afx_msg void OnCmdFiltersReactions();
	afx_msg void OnCmdFiltersDisplacements();
	afx_msg void OnCmdFiltersForceMoments();
	afx_msg void OnCmdFiltersLinks();

	afx_msg void OnCmdOk();
	afx_msg void OnCmdCancel();

	afx_msg LRESULT OnFoldButtonClicked(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMGridAnalCtrlDlg_H__)
