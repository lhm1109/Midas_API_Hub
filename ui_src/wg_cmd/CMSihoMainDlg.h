#if !defined(__CMSihoMainDlg_H__)
#define __CMSihoMainDlg_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMSihoMainDlg.h : header file
//
#include "CMDlgBarBase.h"
#include "..\wg_base\wg_base_PlaceHolder.h"

#include "..\MIT_frx\MComboBox.h"
#include "..\MIT_frx\MButton.h"

#include "..\wg_db\wg_db_DBDoc.h"

using namespace mit::frx;

/////////////////////////////////////////////////////////////////////////////
// CCMSihoMainDlg dialog
class CMenuBarChildDlg;
class CCMSihoMainDlg : public CCMDlgBarBase//, public CDBUpdateConnector
{
// Construction
protected:
	//static TCHAR * m_ComboMenuStr[];
	CArray<const TCHAR*, const TCHAR*> m_ComboMenuStr;
	CArray<CMenuBarChildDlg* , CMenuBarChildDlg*> m_ChildDlgs;
	CArray<int,int>                               m_DlgIDs;
	CArray<UINT, UINT>                            m_TableIDs;
	int m_nCurChildDlg;
	CRect m_RectPlaceHolder;

	CDBDoc* m_pDoc;

public:
	//virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//void UpdateBuffer();

public:
	CCMSihoMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCMSihoMainDlg();
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

	DECLARE_DYNCREATE(CCMSihoMainDlg)
// Dialog Data
	//{{AFX_DATA(CCMSihoMainDlg)
	enum { IDD = IDD_CMD_HINGE_OUTPUT_CONTAINER };
	MButton	m_wndTableButton;
	MComboBox	m_ComboMenu;
	CBCGPStatic	m_wndPlaceHolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMSihoMainDlg)
 	public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual bool ShowApplyBtn() { return false; }
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCMSihoMainDlg)
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

#endif // !defined(__CMSihoMainDlg_H__)
