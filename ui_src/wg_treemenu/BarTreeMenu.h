#if !defined(__BARTREEMENU_H__)
#define      __BARTREEMENU_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BarTreeMenu.h : header file
//

#include "..\wg_db\wg_db_DBDoc.h"
#include "MySEC3DTabWnd.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#include "HeaderPre.h"

/////////////////////////////////////////////////////////////////////////////
// CBarTreeMenu window
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/


class CTreeMenuCtrl;
class CTreeTableCtrl;
class CTreeGroupDlg;
class CTreeWorkCtrl;
class CTreeGroupCtrl;
class CTreeReportCtrl;
class CTreeJUDCtrl;
class CTreeSeisCtrl;

class __MY_EXT_CLASS__ CBarTreeMenu : public CTreeMenuBarBase
{
	DECLARE_DYNAMIC(CBarTreeMenu)
	// Construction
public:
	CBarTreeMenu();

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
	//{{AFX_VIRTUAL(CBarTreeMenu)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL ShowDlgWithInit(int Mode, UINT key);
	BOOL ShowDlgWithInit_by_SelectLabel(int Mode, UINT Key);

	virtual int GetThisMode();
	virtual void SetMode(int Mode);
	virtual int  GetMode();
	virtual void SetModeOnly(int Mode);
	int  GetCurMode();
	//!@#$void DestroyTabWnd(SEC3DTabWnd* pTabWnd); // by jbseon
	void DestroyTabWnd(mit::frx::MBaseTabWnd* pTabWnd); //by LBI
	virtual ~CBarTreeMenu();

	virtual void OnPressCloseButton ();

	CTreeMenuCtrl* GetTreeMenuCtrl() { return m_pTreeMenuCtrl; }
	virtual CWnd* GetCurChild() { return m_pCurWnd; }
	virtual CWnd* GetCurTreeWnd();

// Generated message map functions
protected:
	MySEC3DTabWnd       m_wndTab             ;
	MySEC3DTabWnd       m_wndDgnTab          ;    // Tab Widnow member for Design
	MySEC3DTabWnd       m_wndGPSTab          ;		// Tab Window member for GPS
	MySEC3DTabWnd       m_wndCmdTab          ;
	MySEC3DTabWnd       m_wndNodeElemTab     ;
	MySEC3DTabWnd       m_wndSettlementTab   ;  // Tab Window Member for Settlement
	MySEC3DTabWnd       m_wndHydrationTab    ;
	MySEC3DTabWnd       m_wndTimeDepTab      ;
	MySEC3DTabWnd       m_wndMeshTab         ;
	MySEC3DTabWnd       m_wndJUDTab          ;
	MySEC3DTabWnd       m_wndWindPTab        ;  //WindPressureTab...
	MySEC3DTabWnd       m_wndEBeamTab        ;
	MySEC3DTabWnd       m_wndStripTab        ;
	MySEC3DTabWnd       m_wndDropPanelTab    ;
	MySEC3DTabWnd       m_wndColmCapTab      ;
	MySEC3DTabWnd       m_wndColmLocTab      ;
	MySEC3DTabWnd       m_wndInfillTab       ;
	MySEC3DTabWnd       m_wndSeisTab         ;
	MySEC3DTabWnd       m_wndSeisCommonTab   ;
	MySEC3DTabWnd       m_wndSeisMFactTab	   ;
	MySEC3DTabWnd       m_wndSeisXYFactTab	;
	MySEC3DTabWnd       m_wndHighTemperatureTab      ;
	//  영근이가 헤딩을 했습니다.
	MySEC3DTabWnd       m_wndSvTab;
	MySEC3DTabWnd       m_wndSeisCvlTab      ;
	MySEC3DTabWnd       m_wndSeisCvlMiscTab  ;
	// seismic investigation Civil JP
	MySEC3DTabWnd       m_wndSeisSetTargetTab;
	MySEC3DTabWnd       m_wndSeisSetInvestTab;
	//MySEC3DTabWnd       m_wndCvlSeisResult3dTab;
	
	MySEC3DTabWnd		m_wndPostTensionTab;

	//  요기까정...
	
	CTreeMenuCtrl *     m_pTreeMenuCtrl;
	CTreeTableCtrl*     m_pTreeTableCtrl;
	//CTreeGroupDlg*      m_pTreeGroupDlg;
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
	//void CreateDefaultTreeMenu(LPCREATESTRUCT lpCreateStruct);
	void CreateDefaultTreeMenu();
	
	void CreateStructTab();
	void DestroyStructTab();
	void CreateMeshTab();
	void CreateEBeamTab();
	void CreateStripTab();
	void CreateDropPanelTab();
	void CreateColmCapTab();
	void CreateColmLocTab();
	void CreateSeisInfillTab();
	void CreateSeisTab();
	void DestroySeisTab();
	void CreateSeisCommonTab();
	void CreateSeisMFactTab();
	void CreateSeisXYFactTab();

	void CreateStructSttlTab();  // Added By BugBoy for creating Settlement tab
															// 2000.8.10
	void CreateStructHydrTab();
	void CreateStructTimeDepTab();
	void CreateStructTHighTemperatureTab();	//Add by tss 2019.9
	void CreateSeisCvlTab();
	void CreateSeisCvlMiscTab();
	void CreateSeisSetTargetTab();
	void CreateSeisSetInvestTab();

	void CreateStructNode();
	void CreateStructElem();
	void CreateStructBndr();
	void CreateStructStld();
	void CreateStructMass();
	void CreateStructMesh();

	void CreateWindPressure(CWnd* pParent, mit::frx::MBaseTabWnd* pTabWnd, int nWhich);
	void CreateStructEBeam();
	void CreateStructStrip();
	void CreateStructDropPanel();
	void CreateStructColmCap();
	void CreateModifyColmLoc();

	void CreateSeisInfill();
	void CreateSeisGenerateStiff();
	void CreateSeisCommonSech();
	void CreateSeisCommonSehk();
	void CreateSeisCommonScmt();
	void CreateSeisCommonSerd();
	void CreateSeisCommonRccm();
	void CreateSeisCommonPmcm();
	void CreateSeisRcMFactor();
	void CreateSeisStlMFactor();
	void CreateSeisMasonryMFactor();
	void CreateSeisXYFactor();
	void CreateSeisCvl();
	void CreateSeisCvlMisc();
	void CreateSeisSetTarget_CvlJP();
	void CreateSeisSetting_CvlJP();

	void CreateTendon();
	void CreateProfile();
	void CreateDesignStrip();
	void CreateTendonTab();
	void DestroyTendonTab();

	//void CreateJUDTab();
	//void CreateJUDSlab();

	BOOL IsSameTab(int nMode);
	BOOL IsSameDgnTab(int nMode);
	int IsGPSMultiTab(int nMode);
 
	// Creation And Destruction of GPS Tab Window by mjpark
	void CreateGpsTab(int mode);
	void CreateGpsTab_Move(int mode);
	void DestroyGpsTab();

	//SECShortcutBar m_wndShortcutBar;
	void _ChildLayout();
	CWnd* _GetCurWnd();
	void _SetCurWnd(CWnd* pCurWnd);
	void _FaceEdgeSelModeMan(int nCurMode);
	BOOL _ChangeMode(int  nCurMode);
	CWnd* m_pTestView;
	CWnd* m_pDlgNodeElem;
	CWnd* _CreateChildWindow( CRuntimeClass* pWndClass, CCreateContext* pContext, UINT nID );

	void _ShowTreeMenuTabByID(int nCurMode);
	int  RefreshCurTreeMenuMode(int nNewMode);

	
	//{{AFX_MSG(CBarTreeMenu)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void ToggleWorktab();
protected:
	void SavePrevMode(int nCurMode);

	void ForceTabLocationToTop();

protected:
	COLORREF m_TreeCtrlBkColor;
public:
	virtual void SetBkGroundColor(COLORREF BkColor);

	virtual BOOL OnShowControlBarMenu (CPoint point);

	virtual void OnAfterDock(CBCGPBaseControlBar* pBar, LPCRECT lpRect, BCGP_DOCK_METHOD dockMethod);
};


#include "HeaderPost.h"
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__BARTREEMENU_H__)
