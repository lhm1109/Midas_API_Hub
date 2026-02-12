#if !defined(AFX_CMTDMTINDIA2011_H__)
#define AFX_CMTDMTINDIA2011_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtIndia.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtIndia2011 dialog

class CCMTdmtIndia2011 : public CDlgChild
{
// Construction
public:
	CCMTdmtIndia2011(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtIndia2011)
	enum { IDD = IDD_ETC_HCRP_TDMT_INDIA2011 };
	CEditUnit m_INDIAComp;
	CEditUnit m_INDIANota;
	CTextUnit m_INDIACompUnit;
	CTextUnit m_INDIANotaUnit;
	/*CSpinButtonCtrl m_INDIARelaSpin;*/
	CFormulaEditSpin m_INDIARela;
	/*CSpinButtonCtrl m_INDIAAgeSpin;*/
	CFormulaEditSpin m_INDIAAge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtIndia2011)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void SetDlg2Data();
	T_TDMT_D* m_pData;

	int m_nCementType;
	CArray<UINT, UINT> m_aCementRadioCtrl;
// Implementation
protected:
	void SetUnitType();

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtIndia2011)
	virtual BOOL OnInitDialog();
	virtual void OnCmdHcrpIndia2011CementType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTINDIA2011_H__AFX_CMTDMTINDIA_H__)
