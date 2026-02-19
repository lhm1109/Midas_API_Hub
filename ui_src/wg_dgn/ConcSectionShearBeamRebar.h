#if !defined(__CONCSECTIONSHEARBEAMREBAR_H__)
#define __CONCSECTIONSHEARBEAMREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConcSectionShearBeamRebar.h : header file
//

#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\ChildDialog.h"
#include "DgnStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CConcSectionShearBeamRebar dialog

class I_DlgViewPanel;
class CConcSectionBeamRebarViewDlg;
class CConcSectionCreateShearBeamRebar;
class CConcSectionShearBeamRebar : public CChildDialog
{
// Construction
public:
	CConcSectionShearBeamRebar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConcSectionShearBeamRebar)
	enum { IDD = IDD_DGN_CON_SHEAR_BEAM_REBAR_TAB };
	
	CDlgTabCtrl	m_ctrlTab;
	BOOL        m_bSameIMJ;
	//}}AFX_DATA
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionShearBeamRebar)
public:
	virtual BOOL DestroyWindow();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void Data2Dlg();
	BOOL Dlg2Data();
	int  GetCurSel();

	T_RBGC_SHBR m_DataI;
	T_RBGC_SHBR m_DataM;
	T_RBGC_SHBR m_DataJ;

protected:
	CConcSectionBeamRebarViewDlg* m_pParent;

	CConcSectionCreateShearBeamRebar* m_pShearRebarI;
	CConcSectionCreateShearBeamRebar* m_pShearRebarM;
	CConcSectionCreateShearBeamRebar* m_pShearRebarJ;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionShearBeamRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeingCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONSHEARBEAMREBAR_H__)
