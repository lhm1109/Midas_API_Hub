#if !defined(AFX_STRUCTNODEDLG_H__70E9CCA5_3443_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_STRUCTNODEDLG_H__70E9CCA5_3443_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructNodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructNodeDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditNode.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CStructNodeDlg : public MChildFormView
{
protected:
	CStructNodeDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStructNodeDlg)

// Form Data
public:
	
	//{{AFX_DATA(CStructNodeDlg)
	enum { IDD = IDD_TM_NODE };
	mit::frx::MButton	m_wndTableBtn;
	CPlaceHolder	m_wndPlaceHolder;
	CEditNode	m_EditNode;
	mit::frx::MComboBox	m_ComboMenu;
	//}}AFX_DATA

// Attributes
public:
	CStringArray m_ComboMenuStr;
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;     // 콤보박스 옆에 있는 _T("...")버튼 클릭시에 Main에 날릴 CommandID, (아마도 Table을 보여주라는 명령)
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;
// Operations
public:
	int  _GetHeightFromFont();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	void InitContent();
	void InitComboMenu();
	void AdjustComboListBox();
	void SetHolderSize(int nHeight);
	void OnTmExecAndClose(BOOL bExecute);

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructNodeDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructNodeDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructNodeDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewStartNum();
	afx_msg void OnComboMenuSelect();
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTNODEDLG_H__70E9CCA5_3443_11D3_8899_0000C0A2329D__INCLUDED_)
