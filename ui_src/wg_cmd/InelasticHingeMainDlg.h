#if !defined(__INELASTICHINGEMAINDLG_H__)
#define __INELASTICHINGEMAINDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InelasticHingeMainDlg.h : header file
//
#include "CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CInelasticHingeMainDlg dialog
class CMenuBarChildDlg;
class CInelasticHingeMainDlg : public CCMDlgBarBase//, public CDBUpdateConnector
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
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//void UpdateBuffer();

public:
	CInelasticHingeMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInelasticHingeMainDlg();
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

	virtual void OnTmClose();
	virtual void OnTmExecute();

	DECLARE_DYNCREATE(CInelasticHingeMainDlg)
// Dialog Data
	//{{AFX_DATA(CInelasticHingeMainDlg)
	enum { IDD = IDD_DBAR_MOVE_MAIN };
	MButton	m_wndTableButton;
	MComboBox	m_ComboMenu;
	CPlaceHolder	m_wndPlaceHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInelasticHingeMainDlg)
 	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInelasticHingeMainDlg)
	afx_msg void OnCmdTableButton();
	afx_msg void OnComboMenuSelect();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__INELASTICHINGEMAINDLG_H__)
