#if !defined(AFX_CMSTAGEUDEFSTEPDLG_H__83F537B4_9C31_449F_8491_AF2590484FF2__INCLUDED_)
#define AFX_CMSTAGEUDEFSTEPDLG_H__83F537B4_9C31_449F_8491_AF2590484FF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageUDefStepDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMStageUDefStepDlg dialog

class CCMStageUDefStepDlg : public CChildDialogMove
{
// Construction
public:
	CArray<double, double> * m_pSteps;
	CCMStageUDefStepDlg(CWnd* pParent = NULL);   // standard constructor

	void SetSteps(CArray<double, double> * pSteps);
	void SetListHeader();
	void InsertStepData(int nStep);
	void UpdateCurStepDatas();
	void DeleteCurSelItems();
	void SaveData();
// Dialog Data
	//{{AFX_DATA(CCMStageUDefStepDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_UDSTEP_DLG };
	CListCtrl	m_wndStepList;
	CString	m_strDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageUDefStepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageUDefStepDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAddButton();
	afx_msg void OnCmdClearButton();
	afx_msg void OnCmdDelButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEUDEFSTEPDLG_H__83F537B4_9C31_449F_8491_AF2590484FF2__INCLUDED_)
