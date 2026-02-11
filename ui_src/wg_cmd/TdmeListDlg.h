#if !defined(AFX_TDMELISTDLG_H__771F3F41_228C_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_TDMELISTDLG_H__771F3F41_228C_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TdmeListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CTdmeListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CTdmeListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CTdmeListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTdmeListDlg)
	enum { IDD = IDD_ETC_TDME_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTdmeListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_TDME_K Key, T_TDME_D &Data);
	BOOL DeleteItem(T_TDME_K Key, T_TDME_D &Data);
	BOOL ModifyItem(T_TDME_K KeyOld, T_TDME_K Key, T_TDME_D &Data);
	CString DataToStr(int i, T_TDME_K Key, T_TDME_D &Data);
	CString GetCopyName(CString strOrigin);

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CTdmeListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTdmeBtnAdd();
	afx_msg void OnTdmeBtnModify();
	afx_msg void OnTdmeBtnDelete();
	afx_msg void OnTdmeBtnClose();
	afx_msg void OnDblclkTdmeList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCmdCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDMELISTDLG_H__771F3F41_228C_11D5_880F_00010263A1F4__INCLUDED_)
