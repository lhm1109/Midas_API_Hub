#if !defined(AFX_CMMVHLLISTTRDLG_H__63D2312E_0468_4149_954A_59D8A5C8C225__INCLUDED_)
#define AFX_CMMVHLLISTTRDLG_H__63D2312E_0468_4149_954A_59D8A5C8C225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListTrDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlListTrDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMMvhlListTrDlg(CWnd* pParent = NULL);   // standard constructor

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

	int  FindInsertionPos(T_MVHLtr_K Key, T_MVHLtr_D& Data);
	BOOL InsertItem(T_MVHLtr_K Key, T_MVHLtr_D &Data);
	BOOL DeleteItem(T_MVHLtr_K Key, T_MVHLtr_D &Data);
	BOOL ModifyItem(T_MVHLtr_K KeyOld, T_MVHLtr_K Key, T_MVHLtr_D &Data);
	CString DataToStr(int i, T_MVHLtr_K Key, T_MVHLtr_D &Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlListTrDlg)
	enum { IDD = IDD_CMD_ML_MVHL_LIST_TR };
	MButton	m_btnAdd2;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlListTrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlListTrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnAddUser();
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

#endif // !defined(AFX_CMMVHLLISTTRDLG_H__63D2312E_0468_4149_954A_59D8A5C8C225__INCLUDED_)
