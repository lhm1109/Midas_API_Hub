#if !defined(__CMMVLDLISTIDDLG_H__)
#define __CMMVLDLISTIDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CMMvldListIDDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldListIDDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMMvldListIDDlg(CWnd* pParent = NULL);   // standard constructor

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
	virtual void MakeItemEx(BOOL bUnitChanged=FALSE);

	int  FindInsertionPos(int nID);
	BOOL InsertItem(T_MVLDid_K Key, T_MVLDid_D &Data);
	BOOL DeleteItem(T_MVLDid_K Key, T_MVLDid_D &Data);
	BOOL ModifyItem(T_MVLDid_K KeyOld, T_MVLDid_K Key, T_MVLDid_D &Data);
	CString DataToStr(int i, T_MVLDid_K Key, T_MVLDid_D &Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldListIDDlg)
	enum { IDD = IDD_CMD_ML_MVLD_LIST_INDIA };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldListIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldListIDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMMVLDLISTIDDLG_H__)
