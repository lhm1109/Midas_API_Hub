#if !defined(AFX_CMTDMTINDIA_H__)
#define AFX_CMTDMTINDIA_H__

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
// CCMTdmtIndia dialog

class CCMTdmtIndia : public CDlgChild
{
// Construction
public:
	CCMTdmtIndia(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtIndia)
	enum { IDD = IDD_ETC_HCRP_TDMT_INDIA };
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
	//{{AFX_VIRTUAL(CCMTdmtIndia)
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
	//{{AFX_MSG(CCMTdmtIndia)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTINDIA_H__)
