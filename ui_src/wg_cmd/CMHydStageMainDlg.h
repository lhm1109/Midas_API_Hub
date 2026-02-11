#if !defined(AFX_CMHYDSTAGEMAINDLG_H__35354CEE_0D8D_467F_8912_74FCB3BB8C69__INCLUDED_)
#define AFX_CMHYDSTAGEMAINDLG_H__35354CEE_0D8D_467F_8912_74FCB3BB8C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydStageMainDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_DlgTabCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"

#include "..\MIT_frx\MButton.h"
#include "..\wg_db\FormulaEditUnitSpin.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageMainDlg dialog

class CCMHydStageMainDlg : public CChildDialogMove  , public CDBUpdateConnector
{
// Construction
public:
	int m_nAddOption; // (0) Add (1) Insert Next  (2) Insert Prev
	T_HSTG_D * m_pStageData;
	T_HSTG_K   m_CurStageK ;
	

	CCMHydStageMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMHydStageMainDlg();


	void UpdateCurStageData();
	BOOL SetChildDlgs();
	void SetCurrentStage(T_HSTG_K CurStageKey,int nOption = 0); // Default Add(0)
	void SaveAdditionalStep();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

	void EnableDisableCtrl();

// Dialog Data
	//{{AFX_DATA(CCMHydStageMainDlg)
	enum { IDD = IDD_CMD_DEF_HYD_STAGE_MAIN_DLG };
	CListCtrl	m_wndTimeList;
	CDlgTabCtrl	m_wndTab;
	CEditUnit m_wndInitTempEdit;
	CFormulaEditUnitSpin	m_edtDuration;
	CFormulaEditUnitSpin	m_edtTStep;
	CString	m_strTimes;
	CString	m_strStageName;
	MButton m_bInitTemperature;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydStageMainDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydStageMainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposCmdTimeStepSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposCmdDurationSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdOkButton();
	afx_msg void OnCmdCancelButton();
	afx_msg void OnCmdApplyButton();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDelButton();
	afx_msg void OnCmdGenStepButton();
	afx_msg void OnCmdClearButton();
	afx_msg void OnCmdModifyButton();
	afx_msg void OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStageInfoButton();
	afx_msg void OnCmdInitTemperature();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


class IDBDoc
{
public:
	static void m_pAttrCtrl_GetHstgKeyList(CArray<T_HSTG_K,T_HSTG_K>&rKeyList){};
	static BOOL m_pAttrCtrl_DelHstg(CArray<T_HSTG_K,T_HSTG_K>&rKeyList){ return TRUE;};
	static BOOL m_pAttrCtrl_GetHstg       (T_HSTG_K HstgK,T_HSTG_D& HstgD){return TRUE;};
	static BOOL m_pDataCtrl_AddHstg       (T_HSTG_D & HstgD){return TRUE;};
	static BOOL m_pDataCtrl_ModifyHstg    (T_HSTG_K HstgK,T_HSTG_D & HstgD){return TRUE;};
	static BOOL m_pDataCtrl_InsertHstg    (T_HSTG_K HstgK,BOOL bIsNext,T_HSTG_D & HstgD){return TRUE;};// Next
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDSTAGEMAINDLG_H__35354CEE_0D8D_467F_8912_74FCB3BB8C69__INCLUDED_)
