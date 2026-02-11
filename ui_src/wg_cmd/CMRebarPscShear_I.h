#if !defined(__CMREBARPSCSHEAR_I_H__)
#define __CMREBARPSCSHEAR_I_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMRebarPscShear_I.h : header file
//
//#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMRebarPscShear_I dialog

class CCMRebarPscShear_I : public CChildDialog
{
// Construction
public:
	CCMRebarPscShear_I(CWnd* pParent = NULL);   // standard constructor

// interface
public:
	void SetCurData(T_RPSC_SBAR* pData);
	BOOL GetCurData(T_RPSC_SBAR* pData);

// Dialog Data
	//{{AFX_DATA(CCMRebarPscShear_I)
	enum { IDD = IDD_CMD_REBAR_PSC_SHEAR_I };
	CEditUnit	m_editTensRf;
	CTextUnit	m_unitStrrAw;
	CEditUnit	m_editStrrAw;
	CTextUnit	m_unitStrrPch;
	CEditUnit	m_editStrrPch;
	CButton	m_chkStirrup;
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
	CButton	m_chkLbar;
	CButton	m_chkSbar;
	CButton	m_chkTens;
	CButton m_chkAcp;
	CEditUnit m_editCoverThk;
	CTextUnit m_unitCoverThk;
	CButton m_chkIncFC;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMRebarPscShear_I)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void Data2Dlg();
	void Dlg2Data(T_RPSC_SBAR* pData);

protected:
	T_RPSC_SBAR m_Data;
	BOOL m_bShowStirrup;
	CArray<UINT, UINT> m_aCtrlStirrup;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMRebarPscShear_I)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdLbarCheck();
	afx_msg void OnCmdSbarCheck();
	afx_msg void OnCmdTensCheck();
	afx_msg void OnCmdStrrCheck();
	afx_msg void OnCmdAcpCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMREBARPSCSHEAR_I_H__)
