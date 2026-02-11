#if !defined(AFX_CMGRIDMODELANALDATADLG_H__FFA7486A_2E80_4684_BFE4_B8333DF1A594__INCLUDED_)
#define AFX_CMGRIDMODELANALDATADLG_H__FFA7486A_2E80_4684_BFE4_B8333DF1A594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

/////////////////////////////////////////////////////////////////////////////
// CCMGridModelAnalDataDlg dialog
class CMenuBarChildDlg;
class CCMGridModelAnalDataDlg : public MChildFormView, public CDBUpdateConnector
{

protected:
	static TCHAR * m_ComboMenuStr[];
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;
	CArray<const TCHAR*, const TCHAR*> ComboStrs;

public:
	CCMGridModelAnalDataDlg(CWnd* pParent=NULL);
	virtual ~CCMGridModelAnalDataDlg();
	void CreateChildDlgs();
	void DeleteChildDlgs();
	BOOL ShowChildDlgByID(int nID);
	void ShowCurChildDlg(int nDlgIndex);
	void LayoutChildDialog();
	int  _GetHeightFromFont();
	void InitComboMenu();
	void InitContent();

	virtual void OnTabSelect(WPARAM wParam,LPARAM lParam);
	virtual void OnTabUnSelect(WPARAM wParam,LPARAM lParam);
	virtual CWnd* GetDlgByID(int nID);

	DECLARE_DYNCREATE(CCMGridModelAnalDataDlg)
// Dialog Data
	//{{AFX_DATA(CCMGridModelAnalDataDlg)
	enum { IDD = IDD_CMD_GRID_MODEL_TM_DLG };
	CButton	m_wndTableButton;
	CComboBox	m_ComboMenu;  
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMGridModelAnalDataDlg)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMGridModelAnalDataDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCmdTableButton();
	afx_msg void OnComboMenuSelect();
	virtual void OnTmClose();
	virtual void OnTmExecute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMGRIDMODELANALDATADLG_H__FFA7486A_2E80_4684_BFE4_B8333DF1A594__INCLUDED_)
