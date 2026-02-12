#if !defined(__CMMVLDLISTCHDLG_H__)
#define __CMMVLDLISTCHDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldListCHDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvldListCHDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvldListCHDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMMvldListCHDlg(CWnd* pParent = NULL);   // standard constructor

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
	BOOL InsertItem(T_MVLDch_K Key, T_MVLDch_D &Data);
	BOOL DeleteItem(T_MVLDch_K Key, T_MVLDch_D &Data);
	BOOL ModifyItem(T_MVLDch_K KeyOld, T_MVLDch_K Key, T_MVLDch_D &Data);
	CString DataToStr(int i, T_MVLDch_K Key, T_MVLDch_D &Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldListCHDlg)
	enum { IDD = IDD_CMD_ML_MVLD_LIST_CHINA };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldListCHDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvldListCHDlg)
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

#endif // !defined(__CMMVLDLISTDLG_H__)
