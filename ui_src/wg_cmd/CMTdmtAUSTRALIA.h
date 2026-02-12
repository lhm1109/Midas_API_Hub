#if !defined(AFX_CMTdmtAUSTRALIA_H__INCLUDED_)
#define AFX_CMTdmtAUSTRALIA_H__INCLUDED_

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
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO dialog

class CCMTdmtAUSTRALIA: public CDlgChild
{
	// Construction
public:
	CCMTdmtAUSTRALIA(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CCMTdmtAASHTO) 
	enum { IDD = IDD_ETC_HCRP_TDMT_AUSTRALIA };  
	CEditUnit	m_edtAScomp;
	CTextUnit	m_AScompunit;
	int		    m_nExposureEnv;
	CEditUnit	m_edtASthick;
	CTextUnit	m_ASthickunit;
	int		    m_nEpsDry;
	CEditUnit	m_edtEpsDry;	
	CTextUnit     	m_ASageunit;
	CFormulaEditSpin	      m_ASage;
	//CEditUnit	m_edtStressFactor;
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
	void SetEnable();
	void SetFrameName(const CString& strFrameName);
	void SetData2DlgByCodeChange(int nCode);
	T_TDMT_D* m_pData;
	
	// Implementation
protected:
	void SetUnitType();	
	
	// Generated message map functions
	//{{AFX_MSG(CCMTdmtAASHTO)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdHcrpAsAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDryRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CMTdmtAUSTRALIA_H__INCLUDED_)
