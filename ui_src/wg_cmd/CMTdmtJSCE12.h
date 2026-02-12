#if !defined(AFX_CMTdmtJSCE12_H__679D421D_F698_4D93_88EA_787D0753B1F0__INCLUDED_)
#define AFX_CMTdmtJSCE12_H__679D421D_F698_4D93_88EA_787D0753B1F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtJSCE12.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtJSCE12 dialog

class CCMTdmtJSCE12 : public CDlgChild
{
// Construction
public:
	CCMTdmtJSCE12(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtJSCE12)
	enum { IDD = IDD_ETC_HCRP_TDMT_JSCE12 };

	/*CSpinButtonCtrl	m_jsceRhSpin;*/
	CFormulaEditSpin	m_jsceRh;
	/*CSpinButtonCtrl	m_jsceAcSpin;*/
	CFormulaEditSpin	m_jsceAc;
	/*CSpinButtonCtrl	m_jsceAgeSolSpin;*/
	CFormulaEditSpin	m_jsceAgeSol;
	
	CEditUnit	m_jsceVs;
	CEditUnit	m_jsceCc;
	CEditUnit	m_jsceWc;
	
	CEditUnit	m_jsceComp;     
	CEditUnit	m_jsceImpact;     
	

	CTextUnit m_jsceVsUnit;
	CTextUnit m_jsceCcUnit;
	CTextUnit m_jsceWcUnit;
	CTextUnit m_jsceCompUnit;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtJSCE12)
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
	//{{AFX_MSG(CCMTdmtJSCE12)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdJsceRhSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdJsceAcSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdJsceAgeSolSpin(NMHDR* pNMHDR, LRESULT* pResult);
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTdmtJSCE12_H__679D421D_F698_4D93_88EA_787D0753B1F0__INCLUDED_)
