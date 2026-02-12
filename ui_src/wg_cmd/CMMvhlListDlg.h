#if !defined(__CMMVHLLISTDLG_H__)
#define __CMMVHLLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvhlListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMMvhlListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMMvhlListDlg(CWnd* pParent = NULL);   // standard constructor

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

	int  FindInsertionPos(T_MVHL_K Key, T_MVHL_D& Data);
	BOOL InsertItem(T_MVHL_K Key, T_MVHL_D &Data);
	BOOL DeleteItem(T_MVHL_K Key, T_MVHL_D &Data);
	BOOL ModifyItem(T_MVHL_K KeyOld, T_MVHL_K Key, T_MVHL_D &Data);
	CString DataToStr(int i, T_MVHL_K Key, T_MVHL_D &Data);

	void ModifyVehicle(T_MVHL_D& Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvhlListDlg)
	enum { IDD = IDD_CMD_ML_MVHL_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvhlListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMMvhlListDlg)
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

#endif // !defined(__CMMVHLLISTDLG_H__)
