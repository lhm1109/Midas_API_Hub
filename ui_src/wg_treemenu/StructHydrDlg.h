#if !defined(AFX_STRUCTHYDRDLG_H__F6FE2A41_17B1_11D5_880F_00010263A1F4__INCLUDED_)
#define AFX_STRUCTHYDRDLG_H__F6FE2A41_17B1_11D5_880F_00010263A1F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructHydrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructHydrDlg dialog
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\mit_frx\MButton.h"
#include "..\mit_frx\MComboBox.h"

class CStructHydrDlg : public MChildFormView
{
// Construction
public:
	CStructHydrDlg();   // standard constructor
	DECLARE_DYNCREATE(CStructHydrDlg)

// Dialog Data
	//{{AFX_DATA(CStructHydrDlg)
	enum { IDD = IDD_TM_HYDR };
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

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructHydrDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructHydrDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructHydrDlg)
	afx_msg void  OnComboMenuSelect();
	afx_msg int   OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void  OnTmTableBtn();

	virtual void OnTmExecute();
	virtual void OnTmClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTHYDRDLG_H__F6FE2A41_17B1_11D5_880F_00010263A1F4__INCLUDED_)
