#if !defined(AFX_CMTHMAINDLG_H__012D781A_9933_412B_80EE_0BA7E684536F__INCLUDED_)
#define AFX_CMTHMAINDLG_H__012D781A_9933_412B_80EE_0BA7E684536F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMThMainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMThMainDlg dialog
#include "CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\MIT_frx\MButton.h"
#include "..\MIT_frx\MComboBox.h"

using namespace mit::frx;

class CMenuBarChildDlg;
class CCMThMainDlg : public CCMDlgBarBase, public CDBUpdateConnector
{
// Construction
protected:
	static TCHAR * m_ComboMenuStr[];
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

public:
	CCMThMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMThMainDlg();
	void CreateChildDlgs();
	void DeleteChildDlgs();
	void InitChildDlgByID(int nID, UINT key);
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void SetThrdMain(int nFuncType, int nComboIndex);
	void LayoutChildDialog();
	int  _GetHeightFromFont();
	void InitComboMenu();
	void InitContent();

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int nID);

	DECLARE_DYNCREATE(CCMThMainDlg)
// Dialog Data
	//{{AFX_DATA(CCMThMainDlg)
	enum { IDD = IDD_DBAR_TIMEHISTORY_MAIN };
	MButton	m_wndTableButton;
	MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMThMainDlg)
	public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual bool ShowApplyBtn();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMThMainDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	afx_msg void OnCmdTableButton();
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMTHMAINDLG_H__012D781A_9933_412B_80EE_0BA7E684536F__INCLUDED_)
