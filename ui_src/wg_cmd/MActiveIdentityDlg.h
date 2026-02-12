#if !defined(__MACTIVEIDENTITYDLG_H__)
#define      __MACTIVEIDENTITYDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MActiveIdentityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMActiveIdentityDlg dialog
#include "CMDlgBase.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_SelectCtrl.h"

#include "..\wg_common\wg_common_Query.h"
#include "..\wg_base\AutomationBase.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CMActiveIdentityDlg : public CCMDlgBase, public CAutomationBase //public CGlobalModeless , public CDBUpdateConnector
{
// Construction
public:
	CMActiveIdentityDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMActiveIdentityDlg)
	enum { IDD = IDD_CMD_ACTIVE_IDENTITY_DLG };
	CListBox	m_lbItemList;
	int		m_nCategory;
	BOOL	m_bIsActiveLayer;
	int		m_nStoryOpt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMActiveIdentityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
protected:
	CArray<UINT, UINT> arStoryOption;

	// Generated message map functions
	//{{AFX_MSG(CMActiveIdentityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdCurUcsplane();
	afx_msg void OnCancel();
	afx_msg void OnCmdApply();
	afx_msg void OnCmdActiveallButton();
	afx_msg void OnCmdActiveAccum();
	afx_msg void OnCmdInactiveAccum();
	afx_msg void OnCmdBngr();
	afx_msg void OnCmdLdgr();
	afx_msg void OnCmdMember();
	afx_msg void OnCmdLagr();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void MakeListNone();
	void MakeListGroup();
	void MakeListNpln();
	void MakeListStory();
	void MakeListBngr();
	void MakeListLdgr();
	void MakeListMember();
	void MakeListLagr();

	void ChangeCategory();
	int  GetCurCategory();

	void GetQueryCondition(CQueryCondition& QCond);
	void DoActiveOp();
	int  GetCategory();

	void ShowHideLoadingAreaPlane(BOOL bShow); //show/hide에 따라서 loading plane을 보여주거나 숨긴다.. 단 숨기면서 member radio 버튼의 위치를 움직인다.
	CRect m_rectLoadingPlane;
	CRect m_rectMember;

	void SetActiveObserver(int* pActiveCategory, CMapEx<UINT,UINT,UINT,UINT>& mKey, int* pOption);
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACTIVEIDENTITYDLG_H__DF4DE921_22E4_11D4_8899_0000C0A2329D__INCLUDED_)
