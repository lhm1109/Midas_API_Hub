#if !defined(AFX_CMMATERIALPAGE_H__898B8922_1CC3_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMMATERIALPAGE_H__898B8922_1CC3_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMaterialPage.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog

/////////////////////////////////////////////////////////////////////////////
// CCMMaterialPage dialog
class CCMPropertiesDlg;

#define CCMMaterialPage_base CChildDialog
// class CCMMaterialPage : public CXPropertyPage
class CCMMaterialPage : public CCMMaterialPage_base
{
// Construction
public:
	CCMMaterialPage(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CDBDoc* m_pDoc;
	CCMPropertiesDlg* m_pParent;

// Dialog Data
	//{{AFX_DATA(CCMMaterialPage)
	enum { IDD = IDD_CMD_MATERIAL_PAGE };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void OnActivatePage();

	//virtual void OnActivatePage(int nItemOld, int nItemNew);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMaterialPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetHeaderTitle(BOOL bInit = FALSE);
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);

	BOOL InsertItem(T_MATL_K Key, T_MATL_D &Data);
	BOOL DeleteItem(T_MATL_K Key, T_MATL_D &Data);
	BOOL ModifyItem(T_MATL_K KeyOld, T_MATL_K Key, T_MATL_D &Data);
	CString DataToStr(int i, T_MATL_K Key, T_MATL_D &Data);

	// Generated message map functions
	//{{AFX_MSG(CCMMaterialPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	afx_msg void OnCmdMpBtnImport();
	afx_msg void OnCmdMpBtnRenum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMATERIALPAGE_H__898B8922_1CC3_11D3_92DE_0000C0B0E6B3__INCLUDED_)
