#if !defined(AFX_DGNALUDLG_H__BE8B36E2_D0BC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_DGNALUDLG_H__BE8B36E2_D0BC_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnStlDlg.h : header file
//
#include "..\wg_base\wg_base_FormViewInBar.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnStlDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnAluDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CDgnAluDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnAluDlg)

// Form Data
public:
	//{{AFX_DATA(CDgnStlDlg)
	enum { IDD = IDD_DGN_STL };
	mit::frx::MButton m_wndTableBtn;
	CPlaceHolder	m_wndPlaceHolder;
	mit::frx::MComboBox	m_ComboMenu;
	//}}AFX_DATA

// Attributes
public:
	CString m_ComboMenuStr[4];//add by maxiao 2015-10-12
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT,UINT>                             m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

// Operations
public:
	int  GetHeightFromFont();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	// Add by ZINU.('01.1.2).
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode(BOOL bUpdate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnStlDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDgnAluDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual afx_msg void OnTmClose();
	virtual afx_msg void OnTmExecute();
	// Generated message map functions
	//{{AFX_MSG(CDgnStlDlg)
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDgnTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNSTLDLG_H__BE8B36E2_D0BC_11D3_92DE_0000C0B0E6B3__INCLUDED_)
