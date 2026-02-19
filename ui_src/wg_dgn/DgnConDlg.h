#if !defined(AFX_DGNCONDLG_H__5D7AB3A1_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
#define AFX_DGNCONDLG_H__5D7AB3A1_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DgnRCDlg.h : header file
//
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\wg_base_NotifyCtrl.h"
#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

class CMenuBarChildDlg;

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg Menu Item Structure

struct DgnConMenuItem
{
	CMenuBarChildDlg* pChildDlg;
	int nDlgID;
	UINT nTableID;
	CString strMenuText;

	DgnConMenuItem()
		: pChildDlg(NULL)
		, nDlgID(0)
		, nTableID(0)
		, strMenuText(_T(""))
	{
	}

	DgnConMenuItem(CMenuBarChildDlg* pDlg, int nID, UINT nTblID, const CString& strText)
		: pChildDlg(pDlg)
		, nDlgID(nID)
		, nTableID(nTblID)
		, strMenuText(strText)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////
// CDgnConDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDgnConDlg : public MChildFormView, public CDBUpdateConnector
{
protected:
	CDgnConDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDgnConDlg)

// Form Data
public:
	//{{AFX_DATA(CDgnConDlg)
	enum { IDD = IDD_DGN_CON };
	mit::frx::MButton	m_wndTableBtn;
	mit::frx::MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA

// Attributes
public:
	/**
	#if defined(_CIVIL)
		#if defined(_ORG)
		CString m_ComboMenuStr[5];
		#elif defined(_CH)
		CString m_ComboMenuStr[5];
		#elif defined(_JP)
		CString m_ComboMenuStr[4];
		#else
		CString m_ComboMenuStr[4];
		#endif
	#else
		#if defined(_ORG) && defined(_MGEN)
		CString m_ComboMenuStr[8];
		#else
		CString m_ComboMenuStr[6];
		#endif
	#endif
	**/
	CArray<DgnConMenuItem, DgnConMenuItem&> m_MenuItems;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

// Operations
public:
	int  GetHeightFromFont();

	void AddMenuItem(CMenuBarChildDlg* pDlg, int nDlgID, UINT nTableID, const CString& strMenuText);
	void ClearMenuItems();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex, int nSubIndex);
	void DeleteChildDlgs();
	void CreateChildDlgs();
    void CreateChildDlgs_CH(CString strConCode, bool bCodeCH);
    void CreateChildDlgs_Else(CString strConCode, bool bCodeCH);

	void Update_ComboMenu();
    void Update_ComboMenu_ORG(CString strConCode, CArray<CString, CString&>& m_ComboMenuStr, bool bCodeCH);
    void Update_ComboMenu_CH(CString strConCode, CArray<CString, CString&>& m_ComboMenuStr, bool bCodeCH);
    void Update_ComboMenu_JP(CString strConCode, CArray<CString, CString&>& m_ComboMenuStr, bool bCodeCH);
    void Update_ComboMenu_Else(CString strConCode, CArray<CString, CString&>& m_ComboMenuStr, bool bCodeCH);

	BOOL IsEnbleForBemw(int nTableID);
	
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void Update_InitDataByCode(BOOL bUpdate);
	void LayoutChildDialog();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgnConDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual bool ShowApplyBtn();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDgnConDlg();
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGNCONDLG_H__5D7AB3A1_D4E8_11D3_888F_0000C0F30D4D__INCLUDED_)
