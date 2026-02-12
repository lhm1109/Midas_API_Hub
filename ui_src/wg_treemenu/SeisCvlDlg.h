#if !defined(__SEISCVLDLG_H__)
#define __SEISCVLDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SeisCvlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\wg_base\MChildFormView.h"

class CSeisCvlDlg : public MChildFormView
{
protected:
	CSeisCvlDlg();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSeisCvlDlg)

// Form Data
public:
	//{{AFX_DATA(CSeisCvlDlg)
	enum { IDD = IDD_TM_SEIS_CVL };

	mit::frx::MComboBox	m_cmbMenu;
	mit::frx::MButton   m_btnTable;

	CPlaceHolder	m_wndPlaceHolder;	
	//}}AFX_DATA

// Attributes
public:
	CStringArray m_aMenyStr;
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_aChildDlg;
	CArray<int,int>                               m_aDlgID;
	CArray<UINT, UINT>                            m_aTableID;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;
	
// Operations
public:
	int  _GetHeightFromFont();

	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void AddChildDlgs();
	void DelChildDlgs();
	void InitContent();
	void InitComboMenu();
	void AdjustComboListBox();
	void LayoutChildDialog();

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeisCvlDlg)
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSeisCvlDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSeisCvlDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg void OnTmMenuCmb();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__SEISCVLDLG_H__)
