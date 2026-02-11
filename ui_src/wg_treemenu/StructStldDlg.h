#if !defined(AFX_STRUCTSTLDDLG_H__66BE7325_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STRUCTSTLDDLG_H__66BE7325_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructStldDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructStldDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_base\wg_base_PlaceHolder.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\MChildFormView.h"


class CStructStldDlg : public MChildFormView
{
protected:
	CStructStldDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStructStldDlg)

// Form Data
public:
	//{{AFX_DATA(CStructStldDlg)
	enum { IDD = IDD_TM_STLD };
	mit::frx::MButton	m_wndTableBtn;
	CPlaceHolder	m_wndPlaceHolder;
	mit::frx::MComboBox	m_ComboMenu;
	//}}AFX_DATA

// Attributes
public:
	CStringArray m_ComboMenuStr;
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
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
	void LayoutChildDialog();
	void AddChildDlgs(int nMode);

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

	virtual bool ShowApplyBtn();
	virtual void OnTmExecute();
	virtual void OnTmClose();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructStldDlg)
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructStldDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructStldDlg)
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTSTLDDLG_H__66BE7325_B396_11D3_92DE_0000C0B0E6B3__INCLUDED_)
