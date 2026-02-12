#if !defined(AFX_CCFCLISTDLG_H__613CB5E1_3A24_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_CCFCLISTDLG_H__613CB5E1_3A24_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CcfcListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CCcfcListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CCcfcListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CCcfcListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCcfcListDlg)
	enum { IDD = IDD_ETC_CCFC_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCcfcListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_CCFC_K Key, T_CCFC_D &Data);
	BOOL DeleteItem(T_CCFC_K Key, T_CCFC_D &Data);
	BOOL ModifyItem(T_CCFC_K KeyOld, T_CCFC_K Key, T_CCFC_D &Data);
	CString DataToStr(int i, T_CCFC_K Key, T_CCFC_D &Data);

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CCcfcListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCcfcBtnAdd();
	afx_msg void OnCcfcBtnModify();
	afx_msg void OnCcfcBtnDelete();
	afx_msg void OnCcfcBtnClose();
	afx_msg void OnDblclkCcfcList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCFCLISTDLG_H__613CB5E1_3A24_11D5_880F_00010263A1F4__INCLUDED_)
