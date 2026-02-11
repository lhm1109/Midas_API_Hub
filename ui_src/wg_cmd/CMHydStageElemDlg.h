#if !defined(AFX_CMHYDSTAGEELEMDLG_H__D5A7D52D_118C_4777_AF4A_E05ED28EB150__INCLUDED_)
#define AFX_CMHYDSTAGEELEMDLG_H__D5A7D52D_118C_4777_AF4A_E05ED28EB150__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMHydStageElemDlg.h : header file
//
#include "CMHydStageItemBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMHydStageElemDlg dialog
struct T_HSTG_D;
class CCMHydStageElemDlg : public CCMHydStageItemBase
{
// Construction
public:
	CCMHydStageElemDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListCtrlHeader();

	void SetCurStageData(T_HSTG_D* pStageD);
	void UpdateGroupListByCurState();
// Dialog Data
	//{{AFX_DATA(CCMHydStageElemDlg)
	enum { IDD = IDD_CMD_DEF_HYD_STAGE_ITEM_ELEM };
	CListBox	m_wndGroupList;
	CListCtrl	m_ActiveGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMHydStageElemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMHydStageElemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDefgroupButton();
	afx_msg void OnActAddButton();
	afx_msg void OnActDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMHYDSTAGEELEMDLG_H__D5A7D52D_118C_4777_AF4A_E05ED28EB150__INCLUDED_)
