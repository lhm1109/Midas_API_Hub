#if !defined(__SEISLISTDLG_H__)
#define __SEISLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSeisListDlg dialog
#include "HeaderPre.h"
class __MY_EXT_CLASS__ CSeisListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CSeisListDlg(CWnd* pParent = NULL);   // standard constructor

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
	virtual void MakeItemEx(BOOL bUnitChanged=FALSE);

	int  FindInsertionPos(T_SEIS_K Key);
	BOOL InsertItem(T_SEIS_K Key, T_SEIS_D &Data);
	BOOL DeleteItem(T_SEIS_K Key, T_SEIS_D &Data);
	BOOL ModifyItem(T_SEIS_K KeyOld, T_SEIS_K Key, T_SEIS_D &Data);
	CString DataToStr(int i, T_SEIS_K Key, T_SEIS_D &Data);

protected:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CSeisListDlg)
	enum { IDD = IDD_ETC_SEIS_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSeisListDlg)
	afx_msg void OnCmdBtnAdd();
	afx_msg void OnCmdBtnModify();
	afx_msg void OnCmdBtnDelete();
	afx_msg void OnCmdBtnClose();
	afx_msg void OnDblclkCmdList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#include "HeaderPost.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISLISTDLG_H__)
