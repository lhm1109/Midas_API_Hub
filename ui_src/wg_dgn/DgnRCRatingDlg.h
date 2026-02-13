// DgnSteelRatingDlg.h: interface for the CDgnSteelRatingDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DgnRCRatingDlg_H__)
#define AFX_DgnRCRatingDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\MChildFormView.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnRCRatingDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CDgnRCRatingDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnRCRatingDlg)

	// Form Data
public:
	//{{AFX_DATA(CDgnConDlg)
	enum { IDD = IDD_DGN_RATING };
	mit::frx::MButton	m_wndTableBtn;
	mit::frx::MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

	// Attributes
public:
	// Change by Jaeoh. ('05.07.20)
	CStringArray m_ComboMenuStr;

	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>   m_DlgIDs;
	CArray<UINT,UINT> m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

	// Operations
public:
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	void Update_ComboMenu();

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode(BOOL bUpdate);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConDlg)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual ~CDgnRCRatingDlg();

	void SetTableBtnEnable();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDgnConDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDgnTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DgnRCRatingDlg_H__)
