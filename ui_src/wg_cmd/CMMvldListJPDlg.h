#if !defined(__CMMVLDLISTJPDLG_H__)
#define __CMMVLDLISTJPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMvldListJPDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCMMvldListJPDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CCMMvldListJPDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMMvldListJPDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

//----------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------
protected:
	void SetHeaderTitle();
	virtual void MakeItemEx(BOOL bUnitChanged=FALSE);

	int  FindInsertionPos(int nID);
	BOOL InsertItem(T_MVLDjp_K Key, T_MVLDjp_D &Data);
	BOOL DeleteItem(T_MVLDjp_K Key, T_MVLDjp_D &Data);
	BOOL ModifyItem(T_MVLDjp_K KeyOld, T_MVLDjp_K Key, T_MVLDjp_D &Data);
	CString DataToStr(int i, T_MVLDjp_K Key, T_MVLDjp_D &Data);
	CString GetstrLiveLoadType(int nLiveLoadType);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CCMMvldListJPDlg)
	enum { IDD = IDD_CMD_ML_MVLD_LIST_JAPAN };
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMvldListJPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCMMvldListJPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMVLDLISTJPDLG_H__86563A7A_48D6_4FAC_AD4C_D045A7E867B2__INCLUDED_)
