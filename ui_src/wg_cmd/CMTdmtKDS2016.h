#if !defined(AFX_CMTDMTKDS2016_H__INCLUDED_)
#define AFX_CMTDMTKDS2016_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtKDS2016.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO dialog

class CCMTdmtKDS2016: public CDlgChild, public CDBUpdateConnector
{
	// Construction
public:
	CCMTdmtKDS2016(CWnd* pParent = NULL);   // standard constructor
	
	// Dialog Data
	//{{AFX_DATA(CCMTdmtAASHTO) 
	enum { IDD = IDD_ETC_HCRP_TDMT_KDS2016 };  

	CEditUnit m_edtfck;
	CTextUnit m_untfck;
	/*CSpinButtonCtrl	m_spnRH;*/
	CFormulaEditSpin m_edtRH;
	CEditUnit m_edtSize;
	CTextUnit m_untSize;
	int       m_nCementType;
	CEditUnit m_edtDensity;
	CTextUnit m_untAge;
	/*CSpinButtonCtrl	m_spnAge;*/
	CFormulaEditSpin m_edtAge;
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtKDS2016)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	void InitUnit();
	
	
public:
	void SetData2Dlg();
	void SetDlg2Data();

	T_TDMT_D* m_pData;	
	int m_nModify; // (0) Add (1) Modify 
	
	// Generated message map functions
	//{{AFX_MSG(CCMTdmtAASHTO)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdTdmtKSD2016RHSpin(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnCmdTdmtKSD2016WDBtn();
	afx_msg void OnCmdTdmtKSD2016AgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CMTDMTKDS2016_H__INCLUDED_)
