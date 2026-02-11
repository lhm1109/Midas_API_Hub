#if !defined(AFX_StructColmCapDlg_H__70E9CCA6_3443_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_StructColmCapDlg_H__70E9CCA6_3443_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructColmCapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructColmCapDlg form view
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditElem.h"
#include "..\wg_db\wg_db_EditNode.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\FoldGroupBox.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_base\MChildFormView.h"

using namespace mit::frx;

class CStructColmCapDlg : public MChildFormView
{
protected:
	CStructColmCapDlg();           // protected constructor used by dynamic creation
 	DECLARE_DYNCREATE(CStructColmCapDlg)

// Form Data
public:
	
	//{{AFX_DATA(CStructColmCapDlg)
	enum { IDD = IDD_TM_COLM_CAP };
	MButton	m_wndTableBtn;
	CEditNode	m_EditNode;
	CEditElem	m_EditElem;
	CPlaceHolder	m_wndPlaceHolder;
	MComboBox	m_ComboMenu;
	CFoldGroupBox_Cross	m_gbStartNum;
	//}}AFX_DATA

// Attributes
public:
	CStringArray m_ComboMenuStr;
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
	CRect m_RectPlaceHolder;
	int m_nCurChildDlg;

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
	void SetHolderSize(int nHeight);
	//void ShowExecuteButton(UINT nShowHide);
	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);
	void OnTmExecAndClose(BOOL bExecute);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructColmCapDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructColmCapDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructColmCapDlg)
	afx_msg void OnTmClose();
	afx_msg void OnComboMenuSelect();
	afx_msg void OnTmExecute();
	afx_msg void OnViewStartNodeNum();
	afx_msg void OnViewStartElemNum();
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_StructColmCapDlg_H__70E9CCA6_3443_11D3_8899_0000C0A2329D__INCLUDED_)
