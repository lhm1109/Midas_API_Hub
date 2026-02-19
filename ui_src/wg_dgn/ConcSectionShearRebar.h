#if !defined(__CONCSECTIONSHEARREBAR_H__)
#define __CONCSECTIONSHEARREBAR_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscMoment.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"
#include "DgnStruct.h"
#include "DgnTabCtrl.h"

class CConcSectionCreateShearRebar_I;
class CConcSectionCreateShearRebar_J;

/////////////////////////////////////////////////////////////////////////////
// CConcSectionShearRebar dialog

class I_DlgViewPanel;
class ConcSectionRebarViewDlg;
using namespace mit::frx;
class CConcSectionShearRebar : public CChildDialog
{
// Construction
public:
	CConcSectionShearRebar(CWnd* pParent = NULL);   // standard constructor

// interface
public:
	
	T_REBT_SHBR m_DataI;
	T_REBT_SHBR m_DataJ;
	BOOL bSameIJ;

	void LoadDlgData(BOOL bSameij, T_REBT_SHBR iData, T_REBT_SHBR jData);
	void SaveDlgData();

	int GetCurSel();

	CString m_strDgnCode;

// Dialog Data
	//{{AFX_DATA(CConcSectionShearRebar)
	enum { IDD = IDD_DGN_CON_SHEAR_REBAR_TAB };
	//CDGNTabCtrl	m_ctrlTab;
	CDlgTabCtrl	m_ctrlTab;
	MButton	m_chkSameIJ;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConcSectionShearRebar)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	
	ConcSectionRebarViewDlg* m_pParent;

	CConcSectionCreateShearRebar_I* m_pShearRebarI;
	CConcSectionCreateShearRebar_J* m_pShearRebarJ;

protected:
	void LayoutChildDlg();

	void Data2Dlg();
	BOOL Dlg2Data();

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConcSectionShearRebar)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdSameCheck();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CONCSECTIONSHEARREBAR_H__)
