#if !defined(__BARTREEMENU2_H__)
#define      __BARTREEMENU2_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BarTreeMenu2.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "MySEC3DTabWnd.h"

#include "..\wg_base\wg_base_TreeMenuBarBase.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CBarTreeMenu2 window
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class CTreeMenuCtrl;
class CTreeTableCtrl;
class CTreeGroupDlg;
class CTreeWorkCtrl;
class CTreeGroupCtrl;
class CTreeReportCtrl;
class CTreeJUDCtrl;
class CTreeSeisCtrl;

class __MY_EXT_CLASS__ CBarTreeMenu2 : public CControlBarBase
{
	DECLARE_DYNAMIC(CBarTreeMenu2)
	// Construction
public:
	CBarTreeMenu2();

// Attributes
public:
// Operations
public:
	virtual void AddGroupTab();
	virtual void RemoveGroupTab();
	virtual void AddWorkTab();
	virtual void RemoveWorkTab();
	virtual void AddReportTab();
	virtual void RemoveReportTab();
	virtual void AddJUDTab();
	virtual void RemoveJUDTab();
	virtual void AddSeisTab();
	virtual void RemoveSeisTab();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarTreeMenu2)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ShowDlgWithInit(int Mode, UINT key);

	int  GetCurMode();
	void DestroyTabWnd(mit::frx::MBaseTabWnd* pTabWnd); //by LBI
	virtual ~CBarTreeMenu2();

	virtual void OnPressCloseButton ();

	CTreeMenuCtrl* GetTreeMenuCtrl() { return m_pTreeMenuCtrl; }

	CWnd* GetCurWnd();

// Generated message map functions
protected:
	MySEC3DTabWnd       m_wndTab          ;
	
	CTreeMenuCtrl *     m_pTreeMenuCtrl;
	CTreeTableCtrl*     m_pTreeTableCtrl;
	CTreeWorkCtrl*      m_pTreeWorkCtrl;
	CTreeGroupCtrl*     m_pTreeGroupCtrl;
	CTreeReportCtrl*    m_pTreeReportCtrl;
	CTreeJUDCtrl*       m_pTreeJUDCtrl;  
	CTreeSeisCtrl*      m_pTreeSeisCtrl;

	CWnd*               m_pCurWnd;
	
	int                 m_nCurMode;
	int                 m_nPrevMode;  // 이전 모드
	
	// 현재 설정될 Mode의 임시저장소의 용도로만 사용된다.
	int                 m_ThisMode;
protected:
	void CreateDefaultTreeMenu();
	
	void _ChildLayout();
	CWnd* _GetCurWnd();
	void _SetCurWnd(CWnd* pCurWnd);
	

	//{{AFX_MSG(CBarTreeMenu2)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
protected:
	COLORREF m_TreeCtrlBkColor;
public:
	virtual void SetBkGroundColor(COLORREF BkColor);
	
	virtual BOOL OnShowControlBarMenu (CPoint point);
};


#include "HeaderPost.h"
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__BARTREEMENU2_H__)
