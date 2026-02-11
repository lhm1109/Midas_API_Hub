#if !defined(AFX_STRUCTBNDRDLG_H__C4F5F3A1_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STRUCTBNDRDLG_H__C4F5F3A1_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructBndrDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CStructBndrDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"

class CStructBndrDlg : public MChildFormView
{
protected:
	CStructBndrDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStructBndrDlg)

// Form Data
public:
	//{{AFX_DATA(CStructBndrDlg)
	enum { IDD = IDD_TM_BNDR };
	mit::frx::MButton	m_wndTableBtn;
	mit::frx::MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
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
	void InitContent();
	void InitComboMenu();
	void AdjustComboListBox();
	void AddChildDlgs(int nMode);
	void SetHolderSize(int nHeight);

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructBndrDlg)
	public:
	virtual bool ShowApplyBtn();
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructBndrDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructBndrDlg)
	virtual afx_msg void OnTmClose();
	virtual afx_msg void OnTmExecute();

	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTBNDRDLG_H__C4F5F3A1_B2C9_11D3_92DE_0000C0B0E6B3__INCLUDED_)
