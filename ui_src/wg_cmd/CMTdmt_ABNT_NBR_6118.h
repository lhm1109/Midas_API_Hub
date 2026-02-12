#if !defined(AFX_CMTDMT_ABNT_NBR_6118_H__INCLUDED_)
#define AFX_CMTDMT_ABNT_NBR_6118_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTdmt_ABNT_NBR_6118.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"

#include "..\wg_base\wg_base_DlgChild.h"
#include "..\wg_base\SpinBtnExCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CCMTdmtAASHTO dialog

class CCMTdmt_ABNT_NBR_6118 : public CDlgChild, public CDBUpdateConnector
{
	// Construction
public:
	CCMTdmt_ABNT_NBR_6118(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CCMTdmtAASHTO) 
	enum { IDD = IDD_ETC_HCRP_TDMT_ABNT_NBR_6118	};

	CEditUnit m_edtfck;
	CTextUnit m_untfck;

	CFormulaEditSpin m_edtRH;
	CEditUnit m_edtThk;
	CTextUnit m_untThk;
	int       m_nRH;
	int       m_nCementPerformance;
	int       m_nDeflectionRange;
	CEditUnit m_edtDensity;
	CTextUnit m_untAge;

	mit::frx::MComboBox m_cbmCtype;
	mit::frx::MComboBox m_cbmAtype;

	CFormulaEditSpin m_edtAge;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTdmt_ABNT_NBR_6118)
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
	afx_msg void OnCmdTdmtNBRRHSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdTdmtNBRAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickRHRdo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CMTDMT_ABNT_NBR_6118_H__INCLUDED_)
