// DgnPscDlg.h: interface for the CDgnPscDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNPSCDLG_H__5912F22D_5D7B_4323_9A99_AFEB32566892__INCLUDED_)
#define AFX_DGNPSCDLG_H__5912F22D_5D7B_4323_9A99_AFEB32566892__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnPscDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CDgnPscDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnPscDlg)

// Form Data
public:
	//{{AFX_DATA(CDgnConDlg)
	enum { IDD = IDD_DGN_PSC };
	mit::frx::MButton	m_wndTableBtn;
	mit::frx::MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

// Attributes
public:
	// Change by Jaeoh. ('05.07.20)
	CStringArray m_ComboMenuStr;
 /*
	#if defined(_CIVIL)
		// Change by ZINU.('04.12.28). 임시수정 by 신미영DR, 개발항목 1103.   		
		#if defined(_ORG)
	    CString m_ComboMenuStr[3];
		#elif defined(_JP)
	    CString m_ComboMenuStr[1];
		#elif defined(_CH)
	    CString m_ComboMenuStr[3];
		#else
	    CString m_ComboMenuStr[2];
		#endif    
	#else
		CString m_ComboMenuStr[1];
	#endif
	*/
	
	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>   m_DlgIDs;
	CArray<UINT,UINT> m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

// Operations
public:
	int  GetHeightFromFont();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void DeleteChildDlgs();

    void CreateChildDlgs();
    void CreateChildDlgsKSCE();
    void CreateChildDlgsJSCE();
    void CreateChildDlgsAASHTO();
    void CreateChildDlgsCSA();
    void CreateChildDlgsEURO();
	void CreateChildDlgsBRA();
    void CreateChildDlgsIRC();
    void CreateChildDlgsIRS();
    void CreateChildDlgsAS();
    void CreateChildDlgsKSCELSD15();
    void CreateChildDlgsBS();
    void CreateChildDlgsRUS();
    void CreateChildDlgsElse();

    void Update_ComboMenu();
    void Update_ComboMenuKSCE();
    void Update_ComboMenuJSCE();
    void Update_ComboMenuAASHTO(BOOL bPscComposite);
    void Update_ComboMenuEURO();
    void Update_ComboMenuIND();
    void Update_ComboMenuAS();
    void Update_ComboMenuKSCELSD15(BOOL bPscComposite);
    void Update_ComboMenuBS();
    void Update_ComboMenuRUS();
    void Update_ComboMenuElse();

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
	virtual ~CDgnPscDlg();
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

#endif // !defined(AFX_DGNPSCDLG_H__5912F22D_5D7B_4323_9A99_AFEB32566892__INCLUDED_)
