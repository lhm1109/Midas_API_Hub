#if !defined(AFX_CMSTAGECONSDLG_H__9D818D21_1D81_4390_9453_07A2EFF5A37A__INCLUDED_)
#define AFX_CMSTAGECONSDLG_H__9D818D21_1D81_4390_9453_07A2EFF5A37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageConsDlg.h : header file
//
//#include "CMDlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// CCMStageConsDlg dialog
//#include "BtnST.h"

class CCMStageConsDlg : public CChildDialogMove ,  public CDBUpdateConnector
{
// Construction

	CArray<double, double> m_DurationList;
public:
	void CtrlMan();
	void UpdateStageList();
	CCMStageConsDlg(CWnd* pParent = NULL);   // standard constructor

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
// Dialog Data
	//{{AFX_DATA(CCMStageConsDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_CONS_DLG };
	CListCtrl	m_wndStageList;
	//}}AFX_DATA
private:
	/*
	CButtonST m_wndDown;
	CButtonST m_wndUp;
	CButtonST m_wndBottommost;
	CButtonST m_wndTopMost;
	*/
	void LayoutCtrl(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageConsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageConsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdDelete1();
	afx_msg void OnCmdGenerate();
	afx_msg void OnCmdModify1();
	afx_msg void OnCmdAddNext();
	afx_msg void OnCmdAddPrev();
	afx_msg void OnDblclkStageList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdShow();
	afx_msg void OnCmdAddAnalysis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGECONSDLG_H__9D818D21_1D81_4390_9453_07A2EFF5A37A__INCLUDED_)
