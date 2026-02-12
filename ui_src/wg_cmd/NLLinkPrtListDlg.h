#if !defined(__NLLINKPRTLISTDLG_H__)
#define __NLLINKPRTLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NLLinkPrtListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CNLLinkPrtListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CNLLinkPrtListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CNLLinkPrtListDlg(CWnd* pParent = NULL);   // standard constructor

//----------------------------------------------------------------------
// Overrides
//----------------------------------------------------------------------
public:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	void InitSelect(CString& str);
	
//----------------------------------------------------------------------
// Implementation
protected:
	void SetHeaderTitle();
	virtual void MakeItemEx(BOOL bUnitChanged=FALSE);

	int  FindInsertionPos(T_NLLP_K Key);
	BOOL InsertItem(T_NLLP_K Key, T_NLLP_D &Data);
	BOOL DeleteItem(T_NLLP_K Key, T_NLLP_D &Data);
	BOOL ModifyItem(T_NLLP_K KeyOld, T_NLLP_K Key, T_NLLP_D &Data);
	CString DataToStr(int i, T_NLLP_K Key, T_NLLP_D &Data);
	void ListSelect();

protected:
	CDBDoc* m_pDoc;
	BOOL bInitSelect;
	CString strInitName;

// Dialog Data
	//{{AFX_DATA(CNLLinkPrtListDlg)
	enum { IDD = IDD_ETC_NLLINK_PRT_LIST };
 	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNLLinkPrtListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNLLinkPrtListDlg)
 	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnCopy();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__NLLINKPRTLISTDLG_H__)
