#if !defined(__CMIepiMainDlg_H__)
#define __CMIepiMainDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InelasticHingeMainDlg.h : header file
//
#include "CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\wg_base\MChildFormView.h"

#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMIepiMainDlg dialog
class CMenuBarChildDlg;
class CCMIepiMainDlg : public MChildFormView//, public CDBUpdateConnector
{
// Construction
protected:
	//static TCHAR * m_ComboMenuStr[];
	CArray<TCHAR*, TCHAR*> m_ComboMenuStr;
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

public:
	CCMIepiMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMIepiMainDlg();
	void CreateChildDlgs();
	void DeleteChildDlgs();
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void LayoutChildDialog();
	int  _GetHeightFromFont();
	void InitComboMenu();
	void AdjustComboListBox();
	void InitContent();

	virtual CWnd* GetDlgByID(int nID);

	DECLARE_DYNCREATE(CCMIepiMainDlg)
// Dialog Data
	//{{AFX_DATA(CCMIepiMainDlg)
	enum { IDD = IDD_DBAR_MOVE_MAIN };
	MButton	m_wndTableButton;
	MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMIepiMainDlg)
 	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OnTmClose();
	virtual void OnTmExecute();
	// Generated message map functions
	//{{AFX_MSG(CCMIepiMainDlg)
	afx_msg void OnCmdTableButton();
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__CMIepiMainDlg_H__)
