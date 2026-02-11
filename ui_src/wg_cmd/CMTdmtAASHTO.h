#if !defined(AFX_CMTDMTAASHTO_H__4C6CEEE5_43FD_4AA5_A15F_3F53E4BF4CFD__INCLUDED_)
#define AFX_CMTDMTAASHTO_H__4C6CEEE5_43FD_4AA5_A15F_3F53E4BF4CFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtAASHTO.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MButton.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO dialog

class CCMTdmtAASHTO : public CDlgChild
{
// Construction
public:
	CCMTdmtAASHTO(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtAASHTO)
	enum { IDD = IDD_ETC_HCRP_TDMT_AASHTO };
	mit::frx::MButton	m_chkExpose;
	/*CSpinButtonCtrl	m_AASHTOrelaspin;*/
	CFormulaEditSpin	m_AASHTOrela;
	CTextUnit	m_AASHTOcompunit;
	CEditUnit	m_AASHTOcomp;
	CTextUnit	m_AASHTOageunit;
	/*CSpinButtonCtrl	m_AASHTOagespin;*/
	CFormulaEditSpin	m_AASHTOage;
	CEditUnit m_edtVolumeSurfaceRatio;
	CTextUnit m_untVolumeSurfaceRatio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtAASHTO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void SetDlg2Data();
	T_TDMT_D* m_pData;

// Implementation
protected:
	void SetUnitType();

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtAASHTO)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdHcrpJapanRelaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdHcrpJapanAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTAASHTO_H__4C6CEEE5_43FD_4AA5_A15F_3F53E4BF4CFD__INCLUDED_)
