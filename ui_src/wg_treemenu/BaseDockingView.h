#if !defined(__BASE_DOCKING_VIEW_H__)
#define __BASE_DOCKING_VIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MBaseDockingView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MBaseDockingView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "..\wg_base\MChildFormView.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class MBaseDockingView : public MChildFormView
{
// Attributes
protected:
	MComboBox	m_cmbMenu;
	MButton		m_btnTable;

	int												m_nCurChildDlg;
	CRect											m_RectPlaceHolder;
	CPlaceHolder									m_wndPlaceHolder;
	CStringArray									m_aMenyStr;
	CArray<int, int>								m_aDlgID;
	CArray<UINT, UINT>								m_aTableID;
	CArray<CMenuBarChildDlg*, CMenuBarChildDlg*>	m_aChildDlg;

protected:
	MBaseDockingView();
	DECLARE_DYNCREATE(MBaseDockingView)

	MBaseDockingView(UINT nIDTemplate);
	virtual ~MBaseDockingView();
// Operations
	virtual void DoDataExchange(CDataExchange* pDX);

private:
	void DelChildDlgs();

	void InitComboMenu();
	int  GetHeightFromFont();
	void AdjustComboListBox();
public:
	BOOL ShowChildDlgByID(int nID);
protected:
	void ShowCurChildDlg(int nDlgIndex);

protected:
	// Generated message map functions
	//{{AFX_MSG(MBaseDockingView)
	virtual void OnInitialUpdate();
	virtual void OnTmClose();
	virtual void OnTmExecute();
	afx_msg void OnSelChangeMenuCmb();
	afx_msg void OnTmTableBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__BASE_DOCKING_DLG_H__)