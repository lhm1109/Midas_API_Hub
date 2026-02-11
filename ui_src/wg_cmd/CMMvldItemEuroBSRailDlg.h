#if !defined(__CCMMVLDITEMEUROBSRAILDLG_H__)
#define __CCMMVLDITEMEUROBSRAILDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldItemEuroBSRailDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvldItemEuroBSRailDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvldItemEuroBSRailDlg : public CDialogMove
{
	// Construction
public:
	CCMMvldItemEuroBSRailDlg(CWnd* pParent = NULL);   // standard constructor
	~CCMMvldItemEuroBSRailDlg();

	// Dialog Data
	//{{AFX_DATA(CCMSectionManagerStiffenerSubDlg)
	enum { IDD = IDD_CMD_ML_MVLD_ITEM_EURO_BS_RAIL };

	CEditUnit m_edtPsi1Fact1;
	CEditUnit m_edtPsi1Fact2;
	CEditUnit m_edtPsi1Fact3;
	BOOL      m_bPsi1Fact;
	CEditUnit m_edtMultiFact1;
	CEditUnit m_edtMultiFact2;
	CEditUnit m_edtMultiFact3;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSectionManagerStiffenerSubDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


	// Implementation
public:
	void Data2Dlg();
	BOOL Dlg2Data();

	double m_dPsi1Fact1;
	double m_dPsi1Fact2;
	double m_dPsi1Fact3;
	double m_dMultiFact1;
	double m_dMultiFact2;
	double m_dMultiFact3;


	// Generated message map functions
	//{{AFX_MSG(CCMSectionManagerStiffenerSubDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnCmdIgnorePsi1FactorChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CCMMVLDITEMEUROBSRAILDLG_H__)
