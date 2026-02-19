#if !defined(__CONCSECTIONTORSIONBEAMREBAR_H__)
#define __CONCSECTIONTORSIONBEAMREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionTorsionBeamRebar.h : header file
//

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\ChildDialog.h"
#include "DgnStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CConcSectionTorsionBeamRebar dialog

class I_DlgViewPanel;
class CConcSectionBeamRebarViewDlg;
class CConcSectionCreateTorsionBeamRebar;
class CConcSectionTorsionBeamRebar : public CChildDialog
{
// Construction
public:
	CConcSectionTorsionBeamRebar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConcSectionTorsionBeamRebar)
	enum { IDD = IDD_DGN_CON_TORSION_BEAM_REBAR_TAB };
	
	CDlgTabCtrl	m_ctrlTab;
	BOOL        m_bSameIMJ;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionTorsionBeamRebar)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Data2Dlg();
	BOOL Dlg2Data();
	int  GetCurSel();

	T_RBGC_TORBR m_DataI;
	T_RBGC_TORBR m_DataM;
	T_RBGC_TORBR m_DataJ;

protected:
	CConcSectionBeamRebarViewDlg* m_pParent;

	CConcSectionCreateTorsionBeamRebar* m_pTorsionRebarI;
	CConcSectionCreateTorsionBeamRebar* m_pTorsionRebarM;
	CConcSectionCreateTorsionBeamRebar* m_pTorsionRebarJ;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionTorsionBeamRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeingCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONTORSIONBEAMREBAR_H__)
