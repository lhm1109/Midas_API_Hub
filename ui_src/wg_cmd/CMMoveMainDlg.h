#if !defined(AFX_CMMOVEMAINDLG_H__D631296C_5109_4E60_9FFA_E4F1AF0B8EDE__INCLUDED_)
#define AFX_CMMOVEMAINDLG_H__D631296C_5109_4E60_9FFA_E4F1AF0B8EDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMMoveMainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCMMoveMainDlg dialog
#include "CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"
#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

class CMenuBarChildDlg;
class CCMMoveMainDlg : public CCMDlgBarBase, public CDBUpdateConnector
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
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void UpdateBuffer();

public:
	CCMMoveMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMMoveMainDlg();
	void CreateChildDlgs();
	void DeleteChildDlgs();
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void LayoutChildDialog();
	int  _GetHeightFromFont();
	void InitComboMenu();
	void AdjustComboListBox();
	void InitContent();

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int nID);

	DECLARE_DYNCREATE(CCMMoveMainDlg)
// Dialog Data
	//{{AFX_DATA(CCMMoveMainDlg)
	virtual void OnTmExecute();
	virtual void OnTmClose();
	enum { IDD = IDD_DBAR_MOVE_MAIN };
	MButton	m_wndTableButton;
	MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMMoveMainDlg)
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
	//{{AFX_MSG(CCMMoveMainDlg)
	afx_msg void OnCmdTableButton();
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMMOVEMAINDLG_H__D631296C_5109_4E60_9FFA_E4F1AF0B8EDE__INCLUDED_)
