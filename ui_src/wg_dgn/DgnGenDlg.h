#if !defined(AFX_DGNGENDLG_H__C26DFA01_D417_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNGENDLG_H__C26DFA01_D417_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnGenDlg.h : header file
//
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnGenDlg form view


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnGenDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CDgnGenDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnGenDlg)

// Form Data
public:
	//{{AFX_DATA(CDgnGenDlg)
	enum { IDD = IDD_DGN_GEN };
	mit::frx::MButton m_wndTableBtn;
	CPlaceHolder	m_wndPlaceHolder;
	mit::frx::MComboBox	m_ComboMenu;
	//}}AFX_DATA

// Attributes
public:
#if defined(_TOWER)
	CString m_ComboMenuStr[1];
// #elif defined(_CIVIL_JP)
// 	CString m_ComboMenuStr[1];
#elif defined(_CIVIL)
	#if defined(_CIVIL_JP)
		CString m_ComboMenuStr[5];
	#else
		CString m_ComboMenuStr[10];
	#endif
#elif defined(_MGEN_CH)
	CString m_ComboMenuStr[19];
#else
	CString m_ComboMenuStr[17];
#endif

	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*>	m_ChildDlgs;
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
	BOOL IsEnbleForSdgn(int nTableID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnGenDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual bool ShowApplyBtn();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDgnGenDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual afx_msg void OnTmClose();
	virtual afx_msg void OnTmExecute();

	// Generated message map functions
	//{{AFX_MSG(CDgnGenDlg)
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDgnTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNGENDLG_H__C26DFA01_D417_11D3_888F_0000C0F30D4D__INCLUDED_)
