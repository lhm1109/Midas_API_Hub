#if !defined(AFX_CMHYDSTAGEBNDRDLG_H__10B5BB47_BF0F_477A_97A5_EEFDD3B2B30A__INCLUDED_)
#define AFX_CMHYDSTAGEBNDRDLG_H__10B5BB47_BF0F_477A_97A5_EEFDD3B2B30A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydStageBndrDlg.h : header file
//

#include "CMHydStageItemBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMHydStageBndrDlg dialog

class CCMHydStageBndrDlg : public CCMHydStageItemBase
{
// Construction
public:
	CCMHydStageBndrDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListCtrlHeader();
	void SetCurStageData(T_HSTG_D *pStageD);
	void UpdateGroupListByCurState();  // Active List Deactive List 설정후에 호출할것...
	void _AddFunc(int nWhat); // (0) Act Add (1) Deact Add
// Dialog Data
	//{{AFX_DATA(CCMHydStageBndrDlg)
	enum { IDD = IDD_CMD_DEF_HYD_STAGE_ITEM_BNDR };
	CListBox	m_wndGroupList;
	CListCtrl	m_DeactiveGroupList;
	CListCtrl	m_ActiveGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydStageBndrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydStageBndrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDefgroupButton();
	afx_msg void OnActAddButton();
	afx_msg void OnDeactAdd();
	afx_msg void OnActDeleteButton();
	afx_msg void OnDeactDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDSTAGEBNDRDLG_H__10B5BB47_BF0F_477A_97A5_EEFDD3B2B30A__INCLUDED_)
