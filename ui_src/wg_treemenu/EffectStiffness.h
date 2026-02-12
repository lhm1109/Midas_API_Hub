#if !defined(__EFFECTSITFFNESS_H__)
#define __EFFECTSITFFNESS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructStldDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EffectStiffness form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_base\wg_base_PlaceHolder.h"

#include "..\wg_base\MChildFormView.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CEffectStiffness : public MChildFormView
{
protected:
	CEffectStiffness();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEffectStiffness)

	// Form Data
public:
	//{{AFX_DATA(EffectStiffness)
	enum { IDD = IDD_TM_STFF };
	CPlaceHolder	m_wndPlaceHolder;
	MComboBox	m_ComboMenu;
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
	void AddChildDlgs(int nMode);

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EffectStiffness)
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

	// Implementation
protected:
	virtual ~CEffectStiffness();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(EffectStiffness)
	virtual void OnTmExecute();
	virtual void OnTmClose();

	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EFFECTSITFFNESS_H__)
