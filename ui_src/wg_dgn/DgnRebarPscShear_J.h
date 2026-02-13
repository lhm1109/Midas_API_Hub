#if !defined(__DGNREBARPSCSHEAR_J_H__)
#define __DGNREBARPSCSHEAR_J_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRebarPscShear_J.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\ChildDialog.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarPscShear_J dialog

class CDgnRebarPscShear_J : public CChildDialog
{
// Construction
public:
	CDgnRebarPscShear_J(CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_SBAR* pData, BOOL bSymmetry);
	BOOL GetCurData(T_RPSC_SBAR* pData);

// Dialog Data
	//{{AFX_DATA(CDgnRebarPscShear_J)
	enum { IDD = IDD_DGN_REBAR_PSC_SHEAR_J };
	CEditUnit	m_editTensRf;
	CTextUnit	m_unitStrrAw;
	CEditUnit	m_editStrrAw;
	CTextUnit	m_unitStrrPch;
	CEditUnit	m_editStrrPch;
	MButton	m_chkStirrup;
	CEditUnit	m_editLbarAlt;
	CTextUnit	m_unitLbarAlt;
	CEditUnit	m_editLbarAwt;
	CTextUnit	m_unitLbarAwt;
	CEditUnit	m_editLbarPch;
	CTextUnit	m_unitLbarPch;
	CEditUnit	m_editSbarAng;
	CTextUnit	m_unitSbarAng;
	CEditUnit	m_editSbarAw;
	CTextUnit	m_unitSbarAw;
	CEditUnit	m_editSbarPch;
	CTextUnit	m_unitSbarPch;
	CEditUnit	m_editTensAng;
	CTextUnit	m_unitTensAng;
	CEditUnit	m_editTenAp;
	CTextUnit	m_unitTenAp;
	CEditUnit	m_editTensPch;
	CTextUnit	m_unitTensPch;
	CEditUnit	m_editTensPe;
	CTextUnit	m_unitTensPe;
	MButton	m_chkLbar;
	MButton	m_chkSbar;
	MButton	m_chkTens;
	MButton m_chkAcp;
	CEditUnit m_editCoverThk;
	CTextUnit m_unitCoverThk;
	MButton m_chkIncFC;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnRebarPscShear_J)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	void Dlg2Data(T_RPSC_SBAR* pData);

protected:
	BOOL m_bSymmetry;
	T_RPSC_SBAR m_Data;
	BOOL m_bShowStirrup;
	CArray<UINT, UINT> m_aCtrlStirrup;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDgnRebarPscShear_J)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnLbarCheck();
	afx_msg void OnDgnSbarCheck();
	afx_msg void OnDgnTensCheck();
	afx_msg void OnDgnStrrCheck();
	afx_msg void OnDgnAcpCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNREBARPSCSHEAR_J_H__)
