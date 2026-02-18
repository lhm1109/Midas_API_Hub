#if !defined(AFX_DGNGENLRDFMODDLG_H__147352C6_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENLRDFMODDLG_H__147352C6_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenLrdfModDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"

class CDBDoc;
/////////////////////////////////////////////////////////////////////////////
// CDgnGenLrdfModDlg dialog

class CDgnGenLrdfModDlg : public CMenuBarChildDlg
{
// Construction
public:
	CDgnGenLrdfModDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { OnDgnExecute(); }

// Dialog Data
	//{{AFX_DATA(CDgnGenLrdfModDlg)
	enum { IDD = IDD_DGN_GEN_LRDF_MOD_DLG };
	int		m_nOption;
	double	m_RFactor;
	BOOL	m_bReduAll;
	BOOL	m_bReduAxl;
	BOOL	m_bReduMom;
	BOOL	m_bReduShr;
	//}}AFX_DATA

public:
	void Initial_SelectItem();
	void Initial_Data();
	BOOL ErrorCheck();
	BOOL UpdateReduKindCtrl(BOOL bReduAll);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenLrdfModDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnGenLrdfModDlg)
	afx_msg void OnDgnClose();
	afx_msg void OnDgnExecute();
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnReductionAr();
	afx_msg void OnDgnReductionDel();
	afx_msg void OnDgnCtrlReduKind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENLRDFMODDLG_H__147352C6_D0B0_11D3_888F_0000C0F30D4D__INCLUDED_)
