#if !defined(AFX_CMSTAGEITEMTENDONDLG_H__30FA3E5B_9B5B_4CBC_9F79_ED96FCAD2087__INCLUDED_)
#define AFX_CMSTAGEITEMTENDONDLG_H__30FA3E5B_9B5B_4CBC_9F79_ED96FCAD2087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMStageItemTendonDlg.h : header file
//
#include "CMStageItemBase.h"
/////////////////////////////////////////////////////////////////////////////
// CCMStageItemTendonDlg dialog
struct T_STAG_D;
class CCMStageItemTendonDlg : public CCMStageItemBase
{
// Construction
	T_STAG_D * m_pCurStageD;
public:
	CCMStageItemTendonDlg(CWnd* pParent = NULL);   // standard constructor

	void SetCurStageData(T_STAG_D* pStageD);
	void UpdateGroupList();
	void SetListCtrlHeader();
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void _AddFunc(int nWhat); // (0) Act Add (1) Deact Add
	void UpdateGroupListByCurState();
	void ResetParam();
// Dialog Data
	//{{AFX_DATA(CCMStageItemTendonDlg)
	enum { IDD = IDD_CMD_DEF_STAGE_ITEM_TENDON };
	CListCtrl	m_DeactiveGroupList;
	CListCtrl	m_ActiveGroupList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMStageItemTendonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMStageItemTendonDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMSTAGEITEMTENDONDLG_H__30FA3E5B_9B5B_4CBC_9F79_ED96FCAD2087__INCLUDED_)
