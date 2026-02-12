#if !defined(AFX_CMHYDSTAGECONSDLG_H__D9C69DF6_054C_4BB4_A5C0_48F09FD1E22C__INCLUDED_)
#define AFX_CMHYDSTAGECONSDLG_H__D9C69DF6_054C_4BB4_A5C0_48F09FD1E22C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydStageConsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMHydStageConsDlg dialog

class CCMHydStageConsDlg : public CChildDialogMove , public CDBUpdateConnector
{
// Construction
public:
	CCMHydStageConsDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	void MakeListHeader();
	void UpdateHydStageList();
// Dialog Data
	//{{AFX_DATA(CCMHydStageConsDlg)
	enum { IDD = IDD_CMD_DEF_HYD_STAGE_CONS_DLG };
	CListCtrl	m_StageList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydStageConsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydStageConsDlg)
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdAddNext();
	afx_msg void OnCmdAddPrev();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdDelete1();
	afx_msg void OnCmdModify1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkStageList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDSTAGECONSDLG_H__D9C69DF6_054C_4BB4_A5C0_48F09FD1E22C__INCLUDED_)
