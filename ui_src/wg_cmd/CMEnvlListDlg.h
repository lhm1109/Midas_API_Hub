#if !defined(__CMENVLLISTDLG_H__)
#define __CMENVLLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMEnvlListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMEnvlListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMEnvlListDlg : public CDialogMove, CDBUpdateConnector
{
// Construction
public:
	CCMEnvlListDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	virtual void MakeItemEx();

	int  FindInsertionPos(int nID);
	BOOL InsertItem(T_ENVL_K Key, T_ENVL_D &Data);
	BOOL DeleteItem(T_ENVL_K Key);
	BOOL ModifyItem(T_ENVL_K KeyOld, T_ENVL_K Key, T_ENVL_D &Data);

	void ShowEnvelope(int nSel);

protected:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CCMEnvlListDlg)
	enum { IDD = IDD_CMD_ENVL_LIST };
	CListCtrl	m_List;
	CListBox	m_List1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMEnvlListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMEnvlListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdClose();
	afx_msg void OnSelchangeCmdList();
	afx_msg void OnDblclkCmdList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMENVLLISTDLG_H__)
