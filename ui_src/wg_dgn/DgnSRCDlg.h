#if !defined(AFX_DGNSRCDLG_H__5D7AB3A2_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNSRCDLG_H__5D7AB3A2_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnSrcDlg.h : header file
//
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnSrcDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnSrcDlg : public MChildFormView
{
protected:
	CDgnSrcDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnSrcDlg)

// Form Data
public:
	//{{AFX_DATA(CDgnSrcDlg)
	enum { IDD = IDD_DGN_SRC };
	mit::frx::MButton	m_wndTableBtn;
	mit::frx::MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

// Attributes
public:
	#if defined(_CIVIL)
		CString m_ComboMenuStr[1];
	#else
		CString m_ComboMenuStr[2];
	#endif

	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*>	m_ChildDlgs;
	CArray<int,int>																m_DlgIDs;
	CArray<UINT,UINT>															m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

// Operations
public:
	int  GetHeightFromFont();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	void Update_ComboMenu();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnSrcDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDgnSrcDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual afx_msg void OnTmClose();
	virtual afx_msg void OnTmExecute();

	// Generated message map functions
	//{{AFX_MSG(CDgnSrcDlg)
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDgnTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSRCDLG_H__5D7AB3A2_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
