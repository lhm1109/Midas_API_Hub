#if !defined(AFX_CMSTAGEITEMELEMDLG_H__67E7B945_595D_4915_9551_3BD640436545__INCLUDED_)
#define AFX_CMSTAGEITEMELEMDLG_H__67E7B945_595D_4915_9551_3BD640436545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageItemElemDlg.h : header file
//
#include "CMStageItemBase.h"
#include "../MIT_frx/MEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageItemElemDlg dialog
struct T_STAG_D;
class CCMStageItemElemDlg : public CCMStageItemBase 
{
protected:
	T_STAG_D * m_pCurStageD;
	double     m_Age       ;
	double     m_Redist    ;
// Construction
public:
	void SetCurStageData(T_STAG_D* pStageD);
	void UpdateGroupList();
	CCMStageItemElemDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListCtrlHeader();
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateGroupListByCurState();
	void _AddFunc(int nWhat); // (0) Act Add (1) Deact Add
	void ResetParam();
// Dialog Data
	//{{AFX_DATA(CCMStageItemElemDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_ITEM_ELEM };
	mit::frx::MEdit	m_wndRedistEdit;
	mit::frx::MEdit	m_wndAgeEdit;
	CListBox	m_wndGroupList;
	CListCtrl	m_DeactiveGroupList;
	CListCtrl	m_ActiveGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageItemElemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageItemElemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdDefgroupButton();
	afx_msg void OnDeactDeleteButton();
	afx_msg void OnDeactAdd();
	afx_msg void OnActAddButton();
	afx_msg void OnActDeleteButton();
	afx_msg void OnActModifyButton();
	afx_msg void OnDeltaposCmdAgeSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeactModifyButton();
	afx_msg void OnDeltaposCmdRedistSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEITEMELEMDLG_H__67E7B945_595D_4915_9551_3BD640436545__INCLUDED_)
