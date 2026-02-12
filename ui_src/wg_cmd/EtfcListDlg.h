#if !defined(AFX_ETFCLISTDLG_H__BAC17D61_2356_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_ETFCLISTDLG_H__BAC17D61_2356_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EtfcListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_EditUnit.h"

/////////////////////////////////////////////////////////////////////////////
// CEtfcListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CEtfcListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CEtfcListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEtfcListDlg)
	enum { IDD = IDD_ETC_ETFC_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtfcListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();

protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx();

	BOOL InsertItem(T_ETFC_K Key, T_ETFC_D &Data);
	BOOL DeleteItem(T_ETFC_K Key, T_ETFC_D &Data);
	BOOL ModifyItem(T_ETFC_K KeyOld, T_ETFC_K Key, T_ETFC_D &Data);
	CString DataToStr(int i, T_ETFC_K Key, T_ETFC_D &Data);

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CEtfcListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEtfcBtnAdd();
	afx_msg void OnEtfcBtnModify();
	afx_msg void OnEtfcBtnDelete();
	afx_msg void OnEtfcBtnClose();
	afx_msg void OnDblclkEtfcList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETFCLISTDLG_H__BAC17D61_2356_11D5_880F_00010263A1F4__INCLUDED_)
