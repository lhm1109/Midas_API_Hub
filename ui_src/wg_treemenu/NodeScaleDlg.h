#if !defined(__NODESCALEDLG_H__)
#define      __NODESCALEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeScaleDlg.h : header file
//
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CNodeScaleDlg dialog

class CNodeScaleDlg : public CMenuBarChildDlg
{
// Construction
public:
	CNodeScaleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute();

// Dialog Data
	//{{AFX_DATA(CNodeScaleDlg)
	enum { IDD = IDD_TM_NODE_SCALE };
	CTextUnit	m_ScaleUserUnit;
	CMouseEdit m_ScaleUserEdit;
	CFormulaEditSpin	m_ScaleZEdit;
	CFormulaEditSpin	m_ScaleYEdit;
	CFormulaEditSpin	m_ScaleXEdit;
	int		m_ScaleAboutCbo;
	int		m_SelectTypeCbo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeScaleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;
	CArray<UINT,UINT> m_arCtrlUser;
	// Generated message map functions
	//{{AFX_MSG(CNodeScaleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnScaleAbout();
	afx_msg void OnDeltaposTmNodeScalexSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTmNodeScaleySpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposTmNodeScalezSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODESCALEDLG_H__B96E0501_5737_11D4_9AAF_0000C0B9C58C__INCLUDED_)
