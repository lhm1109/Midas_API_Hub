#if !defined(AFX_DGNCONWALLMARKDLG_H__D5222C01_3DD2_11D4_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONWALLMARKDLG_H__D5222C01_3DD2_11D4_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnConWallMarkDlg.h : header file
//
#include "..\wg_base\wg_base_MenuBarChildDlg.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_baseMouseEdit2.h"

/////////////////////////////////////////////////////////////////////////////
// CDgnConWallMarkDlg dialog

class CDBDoc;
class CDgnConWallMarkDlg : public CMenuBarChildDlg, public CDBUpdateConnector
{
// Construction
public:
	CDgnConWallMarkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual void Execute() { }

// Dialog Data
	//{{AFX_DATA(CDgnConWallMarkDlg)
	enum { IDD = IDD_DGN_CON_WMARK_DLG };
	CListCtrl	m_MarkList;
	CMouseEdit	m_IdList;
	CString	m_strMarkName;
	//}}AFX_DATA

public:
	CString m_strIDList;
	CArray<int,int> m_nIDList;

public:
	void Initial_Data();
	void Initial_MouseEdit();
	void Initial_ListCtrlBox();
	void Initial_ExistListData();
	void Select_ListCtrlItem(int nItem);
	void Write_ListCtrlBox(int Index,CString strName,CString strIDs);

	BOOL ErrorCheck();

	LRESULT OnMViewLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEnter(WPARAM wParam, LPARAM lParam);
	LRESULT OnNodeListEscape(WPARAM wParam, LPARAM lParam);

	virtual void OnUpdate(CView* pView,LPARAM lHint,CObject* pHint);
	virtual void UpdateBuffer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConWallMarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDBDoc* m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CDgnConWallMarkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDgnWallmarkAdd();
	afx_msg void OnDgnWallmarkDelete();
	afx_msg void OnDgnWallmarkModify();
	afx_msg void OnDgnClose();
	afx_msg void OnItemchangedDgnWallmarkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONWALLMARKDLG_H__D5222C01_3DD2_11D4_888F_0000C0F30D4D__INCLUDED_)
