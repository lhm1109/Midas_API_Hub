#if !defined(AFX_CMSTAGEITEMBNDRDLG_H__C86C7802_F072_424A_8D92_CF422C66738B__INCLUDED_)
#define AFX_CMSTAGEITEMBNDRDLG_H__C86C7802_F072_424A_8D92_CF422C66738B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageItemBndrDlg.h : header file
//
#include "CMStageItemBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageItemBndrDlg dialog
struct T_STAG_D;
class CCMStageItemBndrDlg : public CCMStageItemBase 
{
// Construction
	T_STAG_D * m_pCurStageD;
public:
	CCMStageItemBndrDlg(CWnd* pParent = NULL);   // standard constructor
	
	void SetCurStageData(T_STAG_D* pStageD);
	void UpdateGroupList();
	void SetListCtrlHeader();
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void _AddFunc(int nWhat); // (0) Act Add (1) Deact Add
	void UpdateGroupListByCurState();  // Active List Deactive List 설정후에 호출할것...
	void ResetParam();
// Dialog Data
	//{{AFX_DATA(CCMStageItemBndrDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_ITEM_BNDR };
	CListBox	m_wndGroupList;
	CListCtrl	m_DeactiveGroupList;
	CListCtrl	m_ActiveGroupList;
	int		m_nPosOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageItemBndrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageItemBndrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDefineGroupButton();
	afx_msg void OnActAddButton();
	afx_msg void OnActDeleteButton();
	afx_msg void OnActModifyButton();
	afx_msg void OnDeactAdd();
	afx_msg void OnDeactDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEITEMBNDRDLG_H__C86C7802_F072_424A_8D92_CF422C66738B__INCLUDED_)
