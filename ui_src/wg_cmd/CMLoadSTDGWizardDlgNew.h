#if !defined(__CMLOADSTDGWIZARDDLGNEW_H__)
#define __CMLOADSTDGWIZARDDLGNEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMLoadSTDGWizardDlgNew.h : header file
//

#include "..\MIT_frx\MComboBox.h"

#include "..\wg_db\wg_db_DBDoc.h"

struct T_ADDL_D
{
	T_STLD_K StldKey;
	double   dSotryIncr;
	double   dStartDay;
	double   dDayIncr;

	void Initialize()
	{
		StldKey    = 0;
		dSotryIncr = 0.;
		dStartDay  = 0.;
		dDayIncr   = 0.;
	}
};

struct T_ATGN_D
{
	BOOL     bCheck;
	T_STLD_K StldKey;
	double   dStoryIncr;
	double   dStageDurt;
	double   dMemberAge;
	CArray<T_ADDL_D, T_ADDL_D&>arAddDeadLoad;

	void Initialize()
	{
		bCheck = TRUE;
		StldKey    = 0;
		dStoryIncr = 0.;
		dStageDurt = 0.;
		dMemberAge = 0.;
		arAddDeadLoad.RemoveAll();
	} 
};

class CCMStageSpreadGrid;
class CCMLoadESEQMainGrid;
class CCMLoadESEQSubGrid;
class CCMStageStdgSubGrid;

/////////////////////////////////////////////////////////////////////////////
// CCMLoadSTDGWizardDlgNew dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMLoadSTDGWizardDlgNew : public CDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CCMLoadSTDGWizardDlgNew(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMLoadSTDGWizardDlgNew();

	void InitAutoGenerationData();

// Dialog Data
	//{{AFX_DATA(CCMLoadSTDGWizardDlgNew)
	enum { IDD = IDD_CMD_CONS_STAGE_WIZARD_NEW };
	mit::frx::MComboBox	m_cobxLoadCaseMain;
	//}}AFX_DATA
	mit::frx::MComboBox m_cobxLoadCase[3];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMLoadSTDGWizardDlgNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

protected:
	CDBDoc* m_pDoc;
	T_ESQW_D m_Data;
	T_ATGN_D m_ATGNData;
	T_ESQW_ADD_LOAD m_SuperData[3];
	
	CCMStageSpreadGrid*   m_pSpreadGrid;
	CCMLoadESEQMainGrid*  m_pNormalGrid;
	CCMLoadESEQSubGrid*   m_pNormalSubGrid;
	CCMStageStdgSubGrid*  m_pSuperGrid[3];

	BOOL m_bUpdateCheck;
	BOOL m_bSpreadFormat;
	CArray<UINT, UINT> m_aCtrlNormal;
	CArray<UINT, UINT> m_aCtrlSpread;

protected:
	void AlignControl();
	void UpdateLoadcaseCombo(CComboBox* pCombo, BOOL bIncludeNone=TRUE);
	void ChangeFormat(BOOL bSpreadFormat);
	void InitLoadData();
	void Data2Dlg();
	BOOL Dlg2Data();
	int  IsEmptyData(T_ESQW_D &data);
	BOOL IsProperData(T_ESEQ_SEL &data);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMLoadSTDGWizardDlgNew)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCmdLoadcaseMoreBtn();
	afx_msg void OnCmdFormatBtn();
	afx_msg void OnSelchangeCmdEsqwLoadcase1Cbo();
	afx_msg void OnSelchangeCmdEsqwLoadcase2Cbo();
	afx_msg void OnSelchangeCmdEsqwLoadcase3Cbo();
	afx_msg void OnCmdEseqRemoveBtn();
	afx_msg void OnCmdEseqGenBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMLOADSTDGWIZARDDLGNEW_H__)
