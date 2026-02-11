#if !defined(AFX_HSFCLISTDLG_H__4B6310A2_241E_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_HSFCLISTDLG_H__4B6310A2_241E_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HsfcListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CHsfcListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CHsfcListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CHsfcListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHsfcListDlg)
	enum { IDD = IDD_ETC_HSFC_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHsfcListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_HSFC_K Key, T_HSFC_D &Data);
	BOOL DeleteItem(T_HSFC_K Key, T_HSFC_D &Data);
	BOOL ModifyItem(T_HSFC_K KeyOld, T_HSFC_K Key, T_HSFC_D &Data);
	CString DataToStr(int i, T_HSFC_K Key, T_HSFC_D &Data);

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CHsfcListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHsfcBtnAdd();
	afx_msg void OnHsfcBtnModify();
	afx_msg void OnHsfcBtnDelete();
	afx_msg void OnHsfcBtnClose();
	afx_msg void OnDblclkHsfcList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HSFCLISTDLG_H__4B6310A2_241E_11D5_880F_00010263A1F4__INCLUDED_)
