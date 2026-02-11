#if !defined(AFX_IETHLISTDLG_H__9558BE37_2001_4B05_9349_F08664074540__INCLUDED_)
#define AFX_IETHLISTDLG_H__9558BE37_2001_4B05_9349_F08664074540__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IethListDlg.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CIethListDlg dialog
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CIethListDlg : public CDialogMove, public CDBUpdateConnector
{
// Construction
public:
	CIethListDlg(CWnd* pParent = NULL);   // standard constructor

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
	virtual void MakeItemEx();

	BOOL InsertItem(T_IETH_K Key, T_IETH_D &Data);
	BOOL DeleteItem(T_IETH_K Key, T_IETH_D &Data);
	BOOL ModifyItem(T_IETH_K KeyOld, T_IETH_K Key, T_IETH_D &Data);
	CString DataToStr(int i, T_IETH_K Key, T_IETH_D &Data);

protected:
	CDBDoc* m_pDoc;

// Dialog Data
	//{{AFX_DATA(CIethListDlg)
	enum { IDD = IDD_ETC_IETH_LIST };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIethListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIethListDlg)
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

#endif // !defined(AFX_IETHLISTDLG_H__9558BE37_2001_4B05_9349_F08664074540__INCLUDED_)
