#if !defined(AFX_CMSTAGEDEFMAINDLG_H__51ED3308_550F_4D6D_B185_76005E632B7D__INCLUDED_)
#define AFX_CMSTAGEDEFMAINDLG_H__51ED3308_550F_4D6D_B185_76005E632B7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageDefMainDlg.h : header file
//
//#include "CMDlgBase.h"
//#include "CMStageGrid.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\MIT_frx\MComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageDefineDlg dialog
/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CCMStageDefMainDlg dialog
//struct T_STAG_D;
//class CDBDoc;
class  __MY_EXT_CLASS__ CCMStageDefMainDlg : public CChildDialogMove  , public CDBUpdateConnector
{
public:
	int    m_nAddOption; // (0) Add (1) Insert Next  (2) Insert Prev  (3) Modify
	double m_PrevDay;

	T_STAG_D * m_pStageData;
	T_STAG_K   m_CurStageK;
	CCMStageDefMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMStageDefMainDlg();
	
	void UpdateCurStageData();
	BOOL SetChildDlgs();                     //Additional Step 자동 생성시 
	void SetCurrentStage(T_STAG_K CurStageKey,double PrevDay,int nOption = 0); // Default Add(0)
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	
	BOOL GetNumOfDiv(CEdit* pEdit, int &nDiv);
	void StepOptionMan();
	void SaveResultMan();
	void SaveAdditionalStep();
	void CtrlMan();
// Dialog Data
	//{{AFX_DATA(CCMStageDefMainDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_MAIN_DLG };

	CListCtrl	m_wndDayList;
	CFormulaEditSpin	m_wndTStep;
	//CSpinButtonCtrl	m_wndTStepSpin;
	CDlgTabCtrl	m_wndTabCtrl;
	CString	m_strStageName;
	CString	m_strDuration;
	CString	m_strTStep;
	BOOL	m_bSaveStage;
	BOOL	m_bSaveStep;
	int		m_nStepOption;
	CString	m_strDays;
	mit::frx::MComboBox m_cmbStage;
	BOOL  m_bIncreStep;
	CEditUnit m_edtIncreStep;
	//}}AFX_DATA

public:
	//void OnChangeCurrentStag(T_STAG_K CurStagKey);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageDefMainDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	void ResetStagCombo();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageDefMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdApplyButton();
	afx_msg void OnCmdCancelButton();
	afx_msg void OnCmdOkButton();
	afx_msg void OnDeltaposCmdTimeStepSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdDurationSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStepOption();
	afx_msg void OnCmdStageCheck();
	afx_msg void OnCmdUserdefButton();
	afx_msg void OnStageInfoButton();
	afx_msg void OnCmdGenStepButton();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDelButton();
	afx_msg void OnCmdAstepClearButton();
	afx_msg void OnCmdAstepModButton();
	afx_msg void OnSelchangeCmdStageCmb();
	afx_msg void OnDeltaposCmdStageSpn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIncreStepChk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//CCMStageGrid *m_wndGrid;
};

/////////////////////////////////////
#include "HeaderPost.h"      /////////
/////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEDEFMAINDLG_H__51ED3308_550F_4D6D_B185_76005E632B7D__INCLUDED_)
