#if !defined(AFX_STRUCTMASSDLG_H__D2C7A802_B45B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
#define AFX_STRUCTMASSDLG_H__D2C7A802_B45B_11D3_92DE_0000C0B0E6B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructMassDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructMassDlg dialog
#include "..\wg_base\MChildFormView.h"
#include "..\Mit_frx\MComboBox.h"
#include "..\Mit_frx\MButton.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

class CStructMassDlg : public MChildFormView
{
protected:
	CStructMassDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStructMassDlg)

// Form Data
public:
	//{{AFX_DATA(CStructMassDlg)
	enum { IDD = IDD_TM_MASS };
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
	void CreateChildDlgs();
	void InitContent();
	void InitComboMenu();
	void AdjustComboListBox();
	void LayoutChildDialog();

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);
	virtual bool ShowApplyBtn();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructMassDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructMassDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void OnTmExecute();
	virtual void OnTmClose();
	// Generated message map functions
	//{{AFX_MSG(CStructMassDlg)
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTMASSDLG_H__D2C7A802_B45B_11D3_92DE_0000C0B0E6B3__INCLUDED_)
