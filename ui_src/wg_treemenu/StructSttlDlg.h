#if !defined(AFX_STRUCTSTTLDLG_H__93312C01_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
#define AFX_STRUCTSTTLDLG_H__93312C01_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructSttlDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructSttlDlg dialog
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "..\wg_base\MChildFormView.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;
class CStructSttlDlg : public MChildFormView
{
// Construction
public:
	CStructSttlDlg();   // standard constructor
	DECLARE_DYNCREATE(CStructSttlDlg)

// Dialog Data
	//{{AFX_DATA(CStructSttlDlg)
	enum { IDD = IDD_TM_STTL };
	MButton	m_wndTableBtn;
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

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructSttlDlg)
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual bool ShowApplyBtn() { return false; }
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructSttlDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructSttlDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTSTTLDLG_H__93312C01_6EBB_11D4_9AAF_0000C0B9C58C__INCLUDED_)
