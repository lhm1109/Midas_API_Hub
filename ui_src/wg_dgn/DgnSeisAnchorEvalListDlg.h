#if !defined(__DGNSEISANCHOREVALLISTDLG_H__)
#define __DGNSEISANCHOREVALLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisAnchorEvalListDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorEvalListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisAnchorEvalListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisAnchorEvalListDlg(CWnd* pParent = NULL);   // standard constructor

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
	void MakeItemEx();

	int  FindInsertionPos(int nID);	
	BOOL InsertItem(T_ANEV_K Key, T_ANEV_D &Data);
	BOOL DeleteItem(T_ANEV_K Key, T_ANEV_D &Data);
	BOOL ModifyItem(T_ANEV_K KeyOld, T_ANEV_K Key, T_ANEV_D &Data);
	CString DataToStr(int i, T_ANEV_K Key, T_ANEV_D &Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisAnchorEvalListDlg)
	enum { IDD = IDD_DGN_SEIS_ANCHOR_EVAL_LIST_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisAnchorEvalListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisAnchorEvalListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSeisAnchorEvalAddBtn();
	afx_msg void OnSeisAnchorEvalModBtn();
	afx_msg void OnSeisAnchorEvalDelBtn();
	afx_msg void OnSeisAnchorEvalCopyBtn();
	afx_msg void OnSeisAnchorEvalCloseBtn();
	afx_msg void OnSeisAnchorEvalDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISANCHOREVALLISTDLG_H__)
