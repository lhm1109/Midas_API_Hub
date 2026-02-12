#if !defined(AFX_CMTDMTJAPAN_H__679D421D_F698_4D93_88EA_787D0753B1F0__INCLUDED_)
#define AFX_CMTDMTJAPAN_H__679D421D_F698_4D93_88EA_787D0753B1F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtJapan.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtJapan dialog

class CCMTdmtJapan : public CDlgChild
{
// Construction
public:
	CCMTdmtJapan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtJapan)
	enum { IDD = IDD_ETC_HCRP_TDMT_JAPAN };
	/*CSpinButtonCtrl	m_jpanrelaspin;*/
	CFormulaEditSpin	m_jpanrela;
	CTextUnit	m_jpannotaunit;
	CEditUnit	m_jpannota;
	CTextUnit	m_jpancompunit;
	CEditUnit	m_jpancomp;
	CTextUnit	m_jpanageunit;
	CEditUnit	m_jpanLambda;
	/*CSpinButtonCtrl	m_jpanagespin;*/
	CFormulaEditSpin	m_jpanage;
	int		m_jpancemen;
	int		m_jpanhumid;
	int		m_nCalcMethod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtJapan)
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
	void EnableJapanRelaCtrl(BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtJapan)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdHcrpJapanRelRd1();
	afx_msg void OnCmdHcrpJapanRelRd2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTJAPAN_H__679D421D_F698_4D93_88EA_787D0753B1F0__INCLUDED_)
