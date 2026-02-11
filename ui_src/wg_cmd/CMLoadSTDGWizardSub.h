#if !defined(AFX_CMLOADSTDGWIZARDSUB_H__0EAD4ED7_E1B0_45F0_95FA_BA1F381BA646__INCLUDED_)
#define AFX_CMLOADSTDGWIZARDSUB_H__0EAD4ED7_E1B0_45F0_95FA_BA1F381BA646__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadSTDGWizardSub.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardSub dialog

#include "..\MIT_frx\MComboBox.h"

#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

struct T_ATGN_D;
struct T_ADDL_D;
class CCMLoadSTDGWizardDlgNew;

class CCMLoadSTDGWizardSub : public CDialogMove
{
// Construction
public:
	CCMLoadSTDGWizardSub(CCMLoadSTDGWizardDlgNew* pDlg, T_ATGN_D* pATGNData, CWnd* pParent = NULL);   // standard constructor

	void SetATGNData(T_ATGN_D* pData);

// Dialog Data
	//{{AFX_DATA(CCMLoadSTDGWizardSub)
	enum { IDD = IDD_CMD_CONS_STAGE_WIZARD_SUB };
	mit::frx::MComboBox	m_StageCB;
	mit::frx::MComboBox	m_Load2CB;
	mit::frx::MComboBox	m_Load3CB;
	mit::frx::MComboBox	m_Load1CB;
	BOOL	m_nLoad1;
	BOOL	m_nLoad2;
	BOOL	m_nLoad3;
	BOOL	m_nStage;
	CFormulaEditSpin	m_dStoryIncr1;
	CFormulaEditSpin	m_dStoryIncr2;
	CFormulaEditSpin	m_dStoryIncr3;
	CFormulaEditSpin	m_dDayIncr1;
	CFormulaEditSpin	m_dDayIncr2;
	CFormulaEditSpin	m_dDayIncr3;
	CFormulaEditSpin	m_dStartDay1;
	CFormulaEditSpin	m_dStartDay2;
	CFormulaEditSpin	m_dStartDay3;
	CFormulaEditSpin	m_dMembAge;
	CFormulaEditSpin	m_dStageDur;
	CFormulaEditSpin	m_dStoryIncr;
	//}}AFX_DATA
 


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadSTDGWizardSub)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc*     m_pDoc;
	T_ATGN_D*   m_pATGNData;
	CCMLoadSTDGWizardDlgNew* m_pParentDlg;

	void SetData2Dlg();
	BOOL SetDlg2Data();
	void InitLoadCaseCombo(); 
	BOOL CheckValidation(T_ADDL_D &data);

	// Generated message map functions
	//{{AFX_MSG(CCMLoadSTDGWizardSub)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAdditionalDeadloadChk1();
	afx_msg void OnAdditionalDeadloadChk2();
	afx_msg void OnAdditionalDeadloadChk3();
	afx_msg void OnConstructionStageChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMLOADSTDGWIZARDSUB_H__0EAD4ED7_E1B0_45F0_95FA_BA1F381BA646__INCLUDED_)
