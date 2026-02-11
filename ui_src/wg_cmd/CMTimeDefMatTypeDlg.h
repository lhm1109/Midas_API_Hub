#if !defined(AFX_CMTIMEDEFMATTYPEDLG_H__)
#define      AFX_CMTIMEDEFMATTYPEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMTimeDefMatTypeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMTimeDefMatTypeDlg dialog
#include "..\wg_db\wg_db_DBDoc.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMTimeDefMatTypeDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMTimeDefMatTypeDlg(CWnd* pParent = NULL);   // standard constructor
	void SetListHeader();
// Dialog Data
	//{{AFX_DATA(CCMTimeDefMatTypeDlg)
	enum { IDD = IDD_TIME_DEP_MAT_TYPE };
	CListCtrl	m_wndList;
	//}}AFX_DATA

	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();
	void UpdateTdmtList();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMTimeDefMatTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc *m_pDoc;

	CString GetCopyName(CString strOrigin);
	// Generated message map functions
	//{{AFX_MSG(CCMTimeDefMatTypeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdClose();
	afx_msg void OnCmdAdd();
	afx_msg void OnCmdModify();
	afx_msg void OnCmdDelete();
	afx_msg void OnDblclkCmdTimeDepMatList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTIMEDEFMATTYPEDLG_H__52F6A038_B8DB_4792_BFF6_45C937C88B71__INCLUDED_)
