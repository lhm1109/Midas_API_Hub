#if !defined(AFX_HMTPLISTDLG_H__771F3F41_228C_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HMTPLISTDLG_H__771F3F41_228C_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HmtpListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CHmtpListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CHmtpListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CHmtpListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHmtpListDlg)
	enum { IDD = IDD_ETC_HMTP_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHmtpListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_HMTP_K Key, T_HMTP_D &Data);
	BOOL DeleteItem(T_HMTP_K Key, T_HMTP_D &Data);
	BOOL ModifyItem(T_HMTP_K KeyOld, T_HMTP_K Key, T_HMTP_D &Data);
	CString DataToStr(int i, T_HMTP_K Key, T_HMTP_D &Data);

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CHmtpListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHmtpBtnAdd();
	afx_msg void OnHmtpBtnModify();
	afx_msg void OnHmtpBtnDelete();
	afx_msg void OnHmtpBtnClose();
	afx_msg void OnDblclkHmtpList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HMTPLISTDLG_H__771F3F41_228C_11D5_880F_00010263A1F4__INCLUDED_)
