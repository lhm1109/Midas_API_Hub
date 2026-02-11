#if !defined(AFX_CMELSPLSMATLDLG_H__5516C981_7C3D_4CC1_B45D_ECCAB8B3D955__INCLUDED_)
#define AFX_CMELSPLSMATLDLG_H__5516C981_7C3D_4CC1_B45D_ECCAB8B3D955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMElsPlsMatlDlg.h : header file
//
#include "..\wg_db\wg_db_DBDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCMElsPlsMatlDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCMElsPlsMatlDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCMElsPlsMatlDlg)
	enum { IDD = IDD_CMD_PROP_ELS_PLS };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMElsPlsMatlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_EPMT_K Key, T_EPMT_D &Data);
	int  FindInsertionPos(T_EPMT_K Key);
	BOOL DeleteItem(T_EPMT_K Key, T_EPMT_D &Data);
	BOOL ModifyItem(T_EPMT_K KeyOld, T_EPMT_K Key, T_EPMT_D &Data);
	CString DataToStr(int i, T_EPMT_K Key, T_EPMT_D &Data);

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CCMElsPlsMatlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnElsplsAdd();
	afx_msg void OnElsplsMod();
	afx_msg void OnElsplsDel();
	afx_msg void OnElsplsClose();
	afx_msg void OnDblclkElsplsList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMELSPLSMATLDLG_H__5516C981_7C3D_4CC1_B45D_ECCAB8B3D955__INCLUDED_)
