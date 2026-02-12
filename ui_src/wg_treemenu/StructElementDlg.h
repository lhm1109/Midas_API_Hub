#if !defined(AFX_STRUCTELEMENTDLG_H__70E9CCA6_3443_11D3_8899_0000C0A2329D__INCLUDED_)
#define AFX_STRUCTELEMENTDLG_H__70E9CCA6_3443_11D3_8899_0000C0A2329D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StructElementDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStructElementDlg form view
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
#include "..\MIT_frx\MTreeChildImpl.h"
#include "..\wg_base\MChildFormView.h"

class CStructElementDlg : public MChildFormView
{
protected:
	DECLARE_DYNCREATE(CStructElementDlg)

// Form Data
public:
	CStructElementDlg();           // protected constructor used by dynamic creation

	//{{AFX_DATA(CStructElementDlg)
	enum { IDD = IDD_TM_ELEM };
	mit::frx::MButton m_wndTableBtn;
	CEditNode m_EditNode;
	CEditElem m_EditElem;
	CPlaceHolder m_wndPlaceHolder;
	mit::frx::MComboBox	m_ComboMenu;
	///CFoldGroupBox_Cross	m_gbStartNum;
	CBCGPStatic m_stNodeNum;
	CBCGPStatic m_stElemNum;
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
	void ShowExecuteButton(UINT nShowHide);
	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int Mode);
	void OnTmExecAndClose(BOOL bExecute);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStructElementDlg)
	public:
	virtual void OnInitialUpdate();
	virtual bool ShowApplyBtn();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStructElementDlg();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStructElementDlg)

	virtual afx_msg void OnTmClose();
	virtual afx_msg void OnTmExecute();

	afx_msg void OnComboMenuSelect();
	afx_msg void OnViewStartNodeNum();
	afx_msg void OnViewStartElemNum();
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRUCTELEMENTDLG_H__70E9CCA6_3443_11D3_8899_0000C0A2329D__INCLUDED_)
