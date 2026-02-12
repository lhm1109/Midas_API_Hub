#if !defined(AFX_CMSTAGEDEFINEDLG_H__B0ED55D7_9EF9_401D_B715_16D6FB9C4EF5__INCLUDED_)
#define AFX_CMSTAGEDEFINEDLG_H__B0ED55D7_9EF9_401D_B715_16D6FB9C4EF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageDefineDlg.h : header file
//
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_base\SpinBtnExCtrl.h"

using namespace mit::frx;

#include "CMDlgBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageDefineDlg dialog
/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CCMStageDefineDlg : public CInternationalDlg // public CCMDlgBase
{
// Construction
public:
	
	CArray<double , double> m_UserDefSteps;
	CCMStageDefineDlg(CWnd* pParent = NULL);   // standard constructor
	BOOL GenBysuffixButton();
	BOOL GetStagData(T_STAG_D& rData);
	BOOL GetNumOfDiv(CEdit* pEdit, int &nDiv);
	void StepOptionMan();
	void SaveOptionMan();
	void SetListHeader();
	void SaveData()         ;
	void DeleteCurSelItems();
	void SaveAdditionalStep();
// Dialog Data
	//{{AFX_DATA(CCMStageDefineDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_DLG };
	CListCtrl	m_wndDaysList;
	CFormulaEditSpin	m_wndTStep;
	BOOL	m_bTStep;
	//CString	m_strDuration;
	CFormulaEditSpin	m_wndNumberOfTime;
	CString	m_strStageName;
	CString	m_strSuffix;
	int		m_nStepOption;
	BOOL	m_bSaveStage;
	BOOL	m_bSaveStep;
	CString	m_strDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageDefineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageDefineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdOk();
	afx_msg void OnCmdCancel();
	afx_msg void OnCmdApply();
	afx_msg LRESULT OnNcHitTest(CPoint point);  




	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDeltaposTstepSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStepOption();
	afx_msg void OnCmdStageCheck();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdDelButton();
	afx_msg void OnCmdGenStepButton();
	afx_msg void OnCmdAstepClearButton();
	afx_msg void OnCmdAstepModButton();
	afx_msg void OnDeltaposCmdDurSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
#include "HeaderPost.h"      /////////
/////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEDEFINEDLG_H__B0ED55D7_9EF9_401D_B715_16D6FB9C4EF5__INCLUDED_)
