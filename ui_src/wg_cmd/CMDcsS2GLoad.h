#if !defined(AFX_CMDCSS2GLOAD_H__4BF115E6_1EE6_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CMDCSS2GLOAD_H__4BF115E6_1EE6_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDcsS2GLoad.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDcsS2GLoad dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDcsS2GLoad : public CDialogMove
{
// Construction
public:
	CCMDcsS2GLoad(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDcsS2GLoad)
	enum { IDD = IDD_DCS_SDS2GEN_LOAD };
	CListBox	m_lstSelStory;
	CListBox	m_lstStory;
	CEdit			m_edtFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDcsS2GLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void InitCtrlData();
	void InitStorData();

	// Generated message map functions
	//{{AFX_MSG(CCMDcsS2GLoad)
	afx_msg void OnCmdG2sloadStorySelbtn();
	afx_msg void OnCmdG2sloadStoryRemovebtn();
	afx_msg void OnDblclkCmdG2sloadStoryList();
	afx_msg void OnDblclkCmdG2sloadSelstoryList();
	afx_msg void OnCmdG2sloadFileBtn();
	afx_msg void OnCmdG2sloadCancel();
	afx_msg void OnCmdG2sloadOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdG2sloadStorySelectall();
	afx_msg void OnCmdG2sloadStoryRemoveall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDCSS2GLOAD_H__4BF115E6_1EE6_11D5_880F_00010263A1F4__INCLUDED_)
