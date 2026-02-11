#if !defined(AFX_CMTHICKNESSPAGE_H__50132E61_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_CMTHICKNESSPAGE_H__50132E61_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThicknessPage.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_ChildDialog.h"   // for CChildDialog

/////////////////////////////////////////////////////////////////////////////
// CCMThicknessPage dialog
class CCMPropertiesDlg;

#define CCMThicknessPage_base CChildDialog
class CCMThicknessPage : public CCMThicknessPage_base
{
// Construction
public:
	CCMThicknessPage(CDBDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Attributes
public:
	CDBDoc* m_pDoc;
	CCMPropertiesDlg* m_pParent;

// Dialog Data
	//{{AFX_DATA(CCMThicknessPage)
	enum { IDD = IDD_CMD_THICKNESS_PAGE };
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
public:
	virtual void OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint);
	virtual void UpdateBuffer();
	virtual void UnitChanged();
	virtual void OnActivatePage();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThicknessPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit = FALSE);
	virtual void MakeItemEx(BOOL bUnitChanged = FALSE);

	BOOL InsertItem(T_THIK_K Key, T_THIK_D &Data);
	BOOL DeleteItem(T_THIK_K Key, T_THIK_D &Data);
	BOOL ModifyItem(T_THIK_K KeyOld, T_THIK_K Key, T_THIK_D &Data);
	CString DataToStr(int i, T_THIK_K Key, T_THIK_D &Data);

protected:

	// Generated message map functions
	//{{AFX_MSG(CCMThicknessPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnModify();
	afx_msg void OnDelete();
	afx_msg void OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	afx_msg void OnCmdMpBtnImport();
	afx_msg void OnCmdBtnRenum();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHICKNESSPAGE_H__50132E61_1E8E_11D3_92DE_0000C0B0E6B3__INCLUDED_)
