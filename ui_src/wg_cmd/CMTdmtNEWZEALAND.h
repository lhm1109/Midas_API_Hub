#if !defined(AFX_CMTdmtNEWZEALAND_H__INCLUDED_)
#define AFX_CMTdmtNEWZEALAND_H__INCLUDED_

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
#include "..\MIT_frx\McomboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO dialog

class CCMTdmtNEWZEALAND: public CDlgChild
{
	// Construction
public:
	CCMTdmtNEWZEALAND(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CCMTdmtAASHTO) 
	enum { IDD = IDD_ETC_HCRP_TDMT_NEWZEALAND };  
	CEditUnit	m_edtAScomp;
	CTextUnit	m_AScompunit;
	int		    m_nExposureEnv;
	CEditUnit	m_edtASthick;
	CEditUnit	m_edtRFactorShirink;  
	CEditUnit	m_edtAggFactor;
	CTextUnit	m_ASthickunit;  
	mit::frx::MComboBox m_cbxDrying;
	CEditUnit	m_edtDrying;  
	CTextUnit     	m_ASageunit;
	/*CSpinButtonCtrl	m_ASagespin;  */
	CFormulaEditSpin m_ASage;
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
	void SetData2DlgByCodeChange(int nCode);
	T_TDMT_D* m_pData;
	int m_nCode;
	int m_nModify; // (0) Add (1) Modify 
	// Implementation
protected:
	void SetUnitType();
	void SetCtrl(int nCode);
	void SetEnable();
	void SetComboWidth(CComboBox& ComboBox);

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtAASHTO)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdHcrpAsAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeDryingCbx();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	

};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CMTdmtNEWZEALAND_H__INCLUDED_)
