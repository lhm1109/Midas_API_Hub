#if !defined(__THISLISTDLG_H__)
#define __THISLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThisListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "CMDlgBase.h"

/////////////////////////////////////////////////////////////////////////////
// CThisListDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CThisListDlg : public CCMDlgBase, public CDBUpdateConnector
{
// Construction
public:
	CThisListDlg(CWnd* pParent = NULL);   // standard constructor
	enum { IDD = IDD_ETC_THIS_LIST };

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

	BOOL InsertItem(T_THIS_K Key, T_THIS_D &Data);
	BOOL DeleteItem(T_THIS_K Key, T_THIS_D &Data);
	BOOL ModifyItem(T_THIS_K KeyOld, T_THIS_K Key, T_THIS_D &Data);
	CString DataToStr(int i, T_THIS_K Key, T_THIS_D &Data);

protected:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CThisListDlg)
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThisListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThisListDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdEigvCtrl();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString GetNextListName(const CString& listName);
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__THISLISTDLG_H__)
