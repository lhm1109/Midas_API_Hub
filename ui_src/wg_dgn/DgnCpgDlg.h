// DgnCpgDlg.h: interface for the CDgnCpgDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNCPGDLG_H__5912F22D_5D7B_4323_9A99_AFEB32566892__INCLUDED_)
#define AFX_DGNCPGDLG_H__5912F22D_5D7B_4323_9A99_AFEB32566892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnCpgDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CDgnCpgDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnCpgDlg)

	// Form Data
public:
	//{{AFX_DATA(CDgnConDlg)
	enum { IDD = IDD_DGN_CPG };
	MButton	m_wndTableBtn;
	MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

// Attributes
public:
	// Change by Jaeoh. ('05.07.20)
	CStringArray m_ComboMenuStr;

	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int, int>   m_DlgIDs;
	CArray<UINT, UINT> m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

	// Operations
public:
	int  GetHeightFromFont();

	BOOL ShowChildDlgByID(int nID, int nCPG = 0); // nCPG : 0 : CPG Design,  1 : CPG Rating Design
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
	void Update_ComboMenu();
	void GetChildDlgCurl(int nDgnCode, int nID, int &nCurl);
	void GetChildDlgSelectCurl(int nDgnCode, int nCurl, int &nChildIDIndex);

	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode(BOOL bUpdate);

protected:
	int ConvertDlgID_Rating2CPG(int nID);
	void EnalbeDisableCtrl4SubDlg(int nID, int nCurDgnCode);

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
	virtual ~CDgnCpgDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual afx_msg void OnTmClose();
	virtual afx_msg void OnTmExecute();

	// Generated message map functions
	//{{AFX_MSG(CDgnConDlg)
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDgnTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DGNCPGDLG_H__5912F22D_5D7B_4323_9A99_AFEB32566892__INCLUDED_)
