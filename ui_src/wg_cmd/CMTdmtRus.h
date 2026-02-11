#if !defined(AFX_CMTDMTRUS_H__INCLUDED_)
#define AFX_CMTDMTRUS_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmtRUS.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_CobxTdmf.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCMTdmtRUS dialog

class CCMTdmtRUS : public CDlgChild
{
// Construction
public:
	CCMTdmtRUS(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMTdmtRUS)
	enum { IDD = IDD_ETC_HCRP_TDMT_RUS };

	CEditUnit m_edtConcB;
	CTextUnit m_untConcB;
	//CTextUnit m_untRelaHumi;
	CEditUnit m_edtM;
	CTextUnit m_untM;
	//CTextUnit m_untAge;
	int       m_nCuringMeth;
	int       m_nCementType;
	BOOL      m_bCreep;
	int       m_nConcType;
	CEditUnit m_edtWater;
	CTextUnit m_untWater;
	CEditUnit m_edtMaxSize;
	CTextUnit m_untMaxSize;
	CEditUnit m_edtAir;
	CTextUnit m_untAir;
	CEditUnit m_edtPz;

	/*CSpinButtonCtrl	m_spnRelaHumi;*/
	CFormulaEditSpin m_edtRelaHumi;
	/*CSpinButtonCtrl	m_spnAge;*/
	CFormulaEditSpin m_edtAge;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmtRUS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	void SetData2Dlg();
	void SetDlg2Data();
	T_TDMT_D* m_pData;

// Implementation
protected:
	void InitUnit();

	// Generated message map functions
	//{{AFX_MSG(CCMTdmtRUS)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdHcrpRUSRelaSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdHcrpRUSAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTDMTRUS_H__INCLUDED_)
