#if !defined(AFX_CMDCSS2GREACT_H__24D1B647_5A59_461D_BD59_264D35BA10D0__INCLUDED_)
#define AFX_CMDCSS2GREACT_H__24D1B647_5A59_461D_BD59_264D35BA10D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMDcsS2GReact.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMDcsS2GReact dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMDcsS2GReact : public CDialogMove
{
// Construction
public:
	CCMDcsS2GReact(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMDcsS2GReact)
	enum { IDD = IDD_DCS_SDS2GEN_REACT };
	CListBox	m_lstSelStory;
	CListBox	m_lstStory;
	CEdit	m_edtFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMDcsS2GReact)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	void InitCtrlData();
	void InitStorData();

	// Generated message map functions
	//{{AFX_MSG(CCMDcsS2GReact)
	afx_msg void OnCmdG2sreactStorySelbtn();
	afx_msg void OnCmdG2sreactStoryRemovebtn();
	afx_msg void OnDblclkCmdG2sreactStoryList();
	afx_msg void OnDblclkCmdG2sreactSelstoryList();
	afx_msg void OnCmdG2sreactFileBtn();
	afx_msg void OnCmdG2sreactCancel();
	afx_msg void OnCmdG2sreactOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdG2sreactStorySelectall();
	afx_msg void OnCmdG2sreactStoryRemoveall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMDCSS2GREACT_H__24D1B647_5A59_461D_BD59_264D35BA10D0__INCLUDED_)
