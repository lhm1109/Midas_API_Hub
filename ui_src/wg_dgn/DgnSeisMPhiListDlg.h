#if !defined(__DGNSEISMPHILISTDLG_H__)
#define __DGNSEISMPHILISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSeisMPhiListDlg.h : header file
//

#include "..\wg_base\wg_base_DialogMove.h"
#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CDgnSeisMPhiListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CDgnSeisMPhiListDlg(CWnd* pParent = NULL);   // standard constructor

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
	BOOL InsertItem(T_MPCC_K Key, T_MPCC_D &Data);
	BOOL DeleteItem(T_MPCC_K Key, T_MPCC_D &Data);
	BOOL ModifyItem(T_MPCC_K KeyOld, T_MPCC_K Key, T_MPCC_D &Data);
	CString DataToStr(int i, T_MPCC_K Key, T_MPCC_D &Data);

protected:
	CDBDoc* m_pDoc;

public:
// Dialog Data
	//{{AFX_DATA(CDgnSeisMPhiListDlg)
	enum { IDD = IDD_DGN_SEIS_MPHI_LIST_DLG };
	CListCtrl	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSeisMPhiListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDgnSeisMPhiListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnAddBtn();
	afx_msg void OnDgnModBtn();
	afx_msg void OnDgnDelBtn();
	afx_msg void OnDgnCloseBtn();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DGNSEISMPHILISTDLG_H__)
