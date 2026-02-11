//////////////////////////////////////////////////////////////////////////////
// BarTreeMenu.cpp : implementation file

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "wg_treemenu.h"
#include "BarTreeMenu.h"
#include "TreeMenuCtrl.h"
#include "TreeTableCtrl.h"
#include "TreeGroupDlg.h"
#include "TreeGroupCtrl.h"
#include "TreeWorkCtrl.h"
#include "CreateNodeElem.h"
#include "StructNodeDlg.h"
#include "StructElementDlg.h"
#include "StructBndrDlg.h"
#include "StructStldDlg.h"
#include "StructMassDlg.h"
#include "StructSttlDlg.h"
#include "StructHydrDlg.h"
#include "StructTimeDepMatLinkDlg.h"
#include "StructMeshDlg.h"
#include "StructEBeamDlg.h"
#include "StructStripDlg.h"
#include "StructDropPanelDlg.h"
#include "StructColmCapDlg.h"
#include "ModifyColmLocDlg.h"
#include "SeisCommonDlg.h"
#include "TreeReportCtrl.h"
#include "TreeJUDCtrl.h"
#include "TreeSeisCtrl.h"
#include "StldWindPressureRootDlg.h"
#include "StldEarthPressureRootDlg.h"
#include "SeisInfillWallDlg.h"
#include "EffectStiffness.h"
#include "EffectStiffnessGenerate.h"
#include "SeisMFactorDlg.h"
#include "SeisXYFactorDlg.h"
#include "SeisCvlDlg.h"
#include "SeisCvlMiscDlg.h"
#include "StructHighTemperatureMatLinkDlg.h"
#include "SeisCvlJpSetTargetDlg.h"
#include "SeisCvlJpSetInvestDlg.h"
#include "PostTensionDrawTendonLineDlg.h"
#include "PostTensionDesignStripDlg.h"
#include "PostTensionTendonProfileDlg.h"
#include "PostTensionTendonProfileMatchDlg.h"

#include "..\wg_base\wg_baseAll.h"
#include "..\wg_base\wg_baseAllTemp.h"
#include "..\wg_gr\wg_gr_TipView.h"

#include "..\wg_common\wg_commonAll.h"
#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd2\wg_cmd2All.h"
#include "..\wg_dgn\wg_dgnAll.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_dbLock\LockMgr.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_base\TestEnvMgr.h"

#include "..\wg_gps\wg_gpsAll.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\I_GENStateDefine.h"

#include "..\wg_jud\wg_judAll.h"
#include "..\wg_gr\ILabelMgr_CH.h"

#include "..\MIT_frx\MLayeredTabWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CBarTreeMenu
IMPLEMENT_DYNAMIC(CBarTreeMenu, CTreeMenuBarBase)

//pColorObj->Initialize("Message Bar\tBackground", rgbDefColor, rgbBlackBg, rgbWhiteBg,rgbPrint);
//pColorObj->Initialize("Treemenu\tBackground", rgbDefColor, rgbBlackBg, rgbWhiteBg,rgbPrint);
// #define _D_DOPT_COLOR_SKEY_VALUE ":Value"

// COLORREF ReadBkColor()
// {
//   CWinApp* pApp = AfxGetApp();
// 
//   CString csSection, csEntry, csValue;
//   csSection = "DispOpt-Color";
//   csEntry = "Treemenu\tBackground";
// 
//   csValue = pApp->GetProfileString(csSection, csEntry+_D_DOPT_COLOR_SKEY_VALUE, NULL);
//   if (!csValue.IsEmpty())
//     return (COLORREF)atoi(csValue);
//   else
//   {
//     //!@#$ MNET:XXXX 20060220 LBI
//     int nColorName = GetThemeColorName();
// 
//     if(1 == nColorName)   // NormalColor
//     {
//       return RGB(211,226,249);
//     }
//     else if(2 == nColorName)  // Metallic
//     {
//       return RGB(235,233,239);
//     }
//     else if(3 == nColorName)  // HomeStead
//     {
//       return RGB(228,230,192);
//     }
//     else
//     {
//       return RGB(237,234,228); 
//     }
//   }  
// }

CBarTreeMenu::CBarTreeMenu()
{
	//m_eCurMode        = MODE_MENU; 
	//m_nCurMode        = __TMMODE_TREEMENU__;
	m_nCurMode        = __TMMODE_TREEMENU_MENU__;
	m_nPrevMode       = m_nCurMode;
	m_pTreeMenuCtrl   = new CTreeMenuCtrl;
	m_pTreeTableCtrl  = new CTreeTableCtrl;
	m_pDlgNodeElem    = new CreateNodeElemDlg;
	m_pCurWnd         = NULL; 
//  m_pTreeGroupDlg   = NULL;
	m_pTreeWorkCtrl   = NULL;
	m_pTreeGroupCtrl  = NULL;
	m_pTreeReportCtrl = NULL;
	m_pTreeJUDCtrl    = NULL;
	m_pTreeSeisCtrl   = NULL;
	//m_pExtraView에 대한 정리는 FrameWork에 의해 자동으로 이루어 진다. 
	//즉 명시적인 delete가 필요 없다. _CreateChildWindow Routine 참조할것 
	m_TreeCtrlBkColor = ReadBkColor();
}

CBarTreeMenu::~CBarTreeMenu()
{
	if(m_pTreeMenuCtrl)
	  delete m_pTreeMenuCtrl;
	if(m_pTreeTableCtrl)
		delete m_pTreeTableCtrl;
	// Do not delete following object
	// CFormViewInBar deletes object in PostNcDestroy function.
	//delete m_pTreeGroupDlg;  // 이전 부터 안하던거..냥냥.. 
	if(m_pDlgNodeElem)
		delete m_pDlgNodeElem;
	if(m_pTreeGroupCtrl)
		delete m_pTreeGroupCtrl;
	if(m_pTreeSeisCtrl)
		delete m_pTreeSeisCtrl;
}

BEGIN_MESSAGE_MAP(CBarTreeMenu, CTreeMenuBarBase)
 //{{AFX_MSG_MAP(CBarTreeMenu)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBarTreeMenu message handlers
BOOL CBarTreeMenu::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.dwExStyle &= ~(LONG)CBRS_EX_GRIPPER_CLOSE;
	return CTreeMenuBarBase::PreCreateWindow(cs);
}

void CBarTreeMenu::OnSize(UINT nType, int cx, int cy) 
{
	_ChildLayout();
	CTreeMenuBarBase::OnSize(nType, cx, cy);
}

BOOL CBarTreeMenu::OnEraseBkgnd(CDC* pDC) 
{
	ASSERT_VALID(pDC);
	/*	
	if(IsMDIChild())
	{
		CRect rect;
		
		pDC->GetClipBox(&rect);
		CBrush *pOldBrush;
		CBrush ClientBrush(LIGHT_YELLOW);
		pOldBrush = pDC->SelectObject(&ClientBrush);
		pDC->PatBlt(rect.left,rect.top,rect.Width(),rect.Height(), PATCOPY);
		
		pDC->DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);
		
		pDC->SelectObject(pOldBrush);
	}
	else
	{
		// Fill background with APPWORKSPACE
		CBrush backBrush(GetSysColor(COLOR_BTNFACE));
		CBrush* pOldBrush = pDC->SelectObject(&backBrush);
		CRect rect;
		pDC->GetClipBox(&rect);     // Erase the area needed

		CRect rectInside;
		GetInsideRect(rectInside);
		pDC->PatBlt(rect.left, rect.top, rect.Width(), rectInside.top - rect.top, PATCOPY);
		pDC->PatBlt(rect.left, rectInside.bottom, rect.Width(), rect.bottom - rectInside.bottom, PATCOPY);
		pDC->PatBlt(rect.left, rect.top, rectInside.left - rect.left, rect.Height(), PATCOPY);
		pDC->PatBlt(rectInside.right, rect.top, rect.right - rectInside.right, rect.Height(), PATCOPY);
		
		CBrush ClientBrush(LIGHT_YELLOW);
		pDC->SelectObject(&ClientBrush);
		pDC->PatBlt(rectInside.left,rectInside.top, rectInside.Width(), rectInside.Height(), PATCOPY);
		pDC->DrawEdge(&rectInside,EDGE_SUNKEN,BF_RECT);
		
		pDC->SelectObject(pOldBrush);
	}
	return TRUE;
	*/
	return CTreeMenuBarBase::OnEraseBkgnd(pDC);
	//return TRUE;
}

int CBarTreeMenu::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeMenuBarBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/*
	MySEC3DTabWnd		      m_wndTab;
	MySEC3DTabWnd         m_wndDgnTab;         // Tab Widnow member for Design
	MySEC3DTabWnd         m_wndGPSTab;		     // Tab Window member for GPS
	MySEC3DTabWnd         m_wndCmdTab;
	MySEC3DTabWnd         m_wndNodeElemTab;
	MySEC3DTabWnd         m_wndSettlementTab;  // Tab Window Member for Settlement
	MySEC3DTabWnd         m_wndHydrationTab;
	MySEC3DTabWnd         m_wndSvTab;
	*/

	m_wndTab          ;
	m_wndDgnTab       ;  // Tab Widnow member for Design
	m_wndGPSTab       ;  // Tab Window member for GPS
	m_wndCmdTab       ;
	m_wndNodeElemTab  ;
	m_wndSettlementTab;  // Tab Window Member for Settlement
	m_wndHydrationTab ;
	m_wndSvTab        ;
	m_wndWindPTab     ;
	m_wndEBeamTab     ;
	m_wndStripTab     ;
	m_wndDropPanelTab ;
	m_wndColmCapTab   ;
	m_wndColmLocTab   ;

	//m_wndTab.










	CreateDefaultTreeMenu();	
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
////  Form View Creation ....
CWnd* CBarTreeMenu::_CreateChildWindow(CRuntimeClass * pWndClass, CCreateContext * pContext, UINT nID)
{
	ASSERT_VALID(this);
	ASSERT(pWndClass != NULL);
	ASSERT(pWndClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pWndClass, sizeof(CRuntimeClass), FALSE));

	CWnd* pWnd = NULL;
	try
	{
	  pWnd = (CWnd*)pWndClass->CreateObject();
		if(pWnd == NULL)
			AfxThrowMemoryException();
	}
	
	catch( CException* pEx )
	{
	  pEx->Delete();
		TRACE(_LS(IDS_WG_TREEMENU_Out_of_memory_creating_a_view_windo));
	  return pWnd;
	}
			
	ASSERT(pWnd->IsKindOf(RUNTIME_CLASS(CWnd)));
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	
	if (!pWnd->Create(NULL, NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0), this, nID, pContext))
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning__couldn_t_create_the_view_w));
		return NULL;
	}
	return pWnd;
}

int  CBarTreeMenu::GetCurMode()
{
	return m_nCurMode;
}

void CBarTreeMenu::_SetCurWnd(CWnd * pCurWnd)
{
	m_pCurWnd = pCurWnd;
}

CWnd* CBarTreeMenu::_GetCurWnd()
{
	return m_pCurWnd;
}

/////////////////////////////////////////////////////////////////////////////
//Tree Menu Mode를 설정한다. 
//실제 Mode설정은 TreeMenuBarBase::SetModeST(int Mode)함수를 이용한다. 
//SetMode()는 SetModeST에서 호출된다. 
//Tree Menu Bar에 현재 Mode를 설정한다. 

void CBarTreeMenu::SavePrevMode(int nCurMode)
{
	if(m_nPrevMode == __TMMODE_STRUCT_STLD_PRES_QUERY__ && m_nPrevMode!= nCurMode)
		ILabelMgr_CH::instance()->UnDisplayPresTypeColor();
	//
	if (nCurMode != __TMMODE_TREEMENU__       &&
			nCurMode != __TMMODE_TREEMENU_MENU__  &&
			nCurMode != __TMMODE_TREEMENU_TABLE__ &&
			nCurMode != __TMMODE_TREEMENU_GROUP__ &&
			nCurMode != __TMMODE_TREEMENU_WORK__  &&
			nCurMode != __TMMODE_TREEMENU_REPORT__&&
			nCurMode != __TMMODE_TREEMENU_JUD__   &&
			nCurMode != __TMMODE_TREEMENU_SEIS__)
		m_nPrevMode = nCurMode; // 현재 모드 저장
}

void CBarTreeMenu::ForceTabLocationToTop()
{
	// 부모 윈도우가 탭 컨테이너인지 확인
	auto pParentTabbedBar = GetParentTabbedBar();
	if (!pParentTabbedBar)
		return;

	CBCGPTabbedControlBar* pTabbedControlBar = dynamic_cast<CBCGPTabbedControlBar*>(pParentTabbedBar);
	if (pTabbedControlBar == nullptr)
		return;

	// 탭 위치를 상단으로 강제 설정
	CBCGPBaseTabWnd* pBaseTabWnd = pTabbedControlBar->GetTabWnd();
	if (pBaseTabWnd != nullptr)
	{
		pBaseTabWnd->SetLocation(CBCGPBaseTabWnd::LOCATION_TOP);
	}

	// MLayeredTabWnd에도 동일하게 적용
	mit::frx::MLayeredTabWnd* pTabbedWnd = dynamic_cast<mit::frx::MLayeredTabWnd*>(pTabbedControlBar->GetTabWnd());
	if (pTabbedWnd != nullptr)
	{
		pTabbedWnd->SetLocation(CBCGPBaseTabWnd::LOCATION_TOP);
		pTabbedWnd->RecalcLayout();
	}

	// Gripper 활성화 확인
	pTabbedControlBar->EnableGripper(TRUE);

	// 레이아웃 재계산
	pTabbedControlBar->RecalcLayout();
}

void CBarTreeMenu::SetMode(int Mode)
{
	if(_ChangeMode(Mode))
	{
		SavePrevMode(Mode);

		_ChildLayout();
		if(this->IsAutoHideMode() && !this->IsVisible())
		{
			//!@#$ MNET:XXXX 20060304 LBI
			if( Mode == __TMMODE_TREEMENU__         ||
					Mode == __TMMODE_TREEMENU_MENU__    ||
					Mode == __TMMODE_TREEMENU_TABLE__   ||
					Mode == __TMMODE_TREEMENU_GROUP__   ||
					Mode == __TMMODE_TREEMENU_WORK__    ||
					Mode == __TMMODE_TREEMENU_REPORT__  ||
					Mode == __TMMODE_TREEMENU_JUD__     ||
					Mode == __TMMODE_TREEMENU_SEIS__
				)
			{
				// TreeMenu가 AutoHide인 경우 Show 하지 않도록 한다.
			}
			else
				this->Slide(TRUE);    
		}
	}
}


int CBarTreeMenu::GetMode()
{
	return m_nCurMode;
}

void CBarTreeMenu::SetModeOnly(int Mode)
{
	m_nCurMode = Mode;
	SavePrevMode(Mode);
}

void CBarTreeMenu::_FaceEdgeSelModeMan(int nCurMode)
{
	
	switch(nCurMode)
	{
	case __TMMODE_STRUCT_ELEM_DIVIDE__:   
		break;
	case __TMMODE_STRUCT_STLD_PRES__:
		break;
	case __TMMODE_STRUCT_STLD_HPRS__:
		break;
	case __TMMODE_STRUCT_HYDR_CONVECT__:
		break;
	default:
		{
			CDBDoc* pDoc = CDBDoc::GetDocPoint();
			if(pDoc)
			{
				pDoc->m_pViewCtrl->SetDivPatternMode    (FALSE);
				pDoc->m_pViewCtrl->SetPressureInputMode (FALSE);
				pDoc->m_pViewCtrl->SetPressureInputParam(-1,-1);	
				pDoc->m_pViewCtrl->SetHydratiorInputMode(FALSE);
			}
		}
		break;
	}
}

void CBarTreeMenu::_ChildLayout()
{
	/*****************************************************************************
	@@	Control Bar에 위치할 Child Window들의 Sizing을 행한다. 
	@@	GetInsideRect() ==> CControlBarBase에서 Child Window가 위치할 영역에 대한 
	@@	Rectangle을 제공한다. 
	*/
	CRect rectInside;
	
	/*
	if(IsMDIChild())
	{
		GetClientRect(rectInside);
		rectInside.top += 3;
	}
	else
	{
		GetInsideRect(rectInside);
		rectInside.bottom -= 4;
	}
	*/

	/*!@#$*/GetClientRect(rectInside);
	
	//GetClientRect(&rectInside);
		
	switch(GetCurMode())
	{
	case __TMMODE_TREEMENU__:
	case __TMMODE_TREEMENU_MENU__:
	case __TMMODE_TREEMENU_TABLE__:
	case __TMMODE_TREEMENU_GROUP__:
	case __TMMODE_TREEMENU_WORK__:
	case __TMMODE_TREEMENU_REPORT__:
	case __TMMODE_TREEMENU_JUD__:
	case __TMMODE_TREEMENU_SEIS__:
		//::SetWindowPos(m_wndNodeElemTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		::SetWindowPos(m_wndTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	case __TMMODE_STRUCT_NODE__:
	case __TMMODE_STRUCT_NODE_COMPACT__:
	case __TMMODE_STRUCT_NODE_CREATE__:
	case __TMMODE_STRUCT_NODE_DELETE__:
	case __TMMODE_STRUCT_NODE_TRANSLATE__:
	case __TMMODE_STRUCT_NODE_MIRROR__:
	case __TMMODE_STRUCT_NODE_ROTATE__:
	case __TMMODE_STRUCT_NODE_PROJECT__:
	case __TMMODE_STRUCT_NODE_DIVIDE__:
	case __TMMODE_STRUCT_NODE_MERGE__:
	case __TMMODE_STRUCT_NODE_SCALE__:
	case __TMMODE_STRUCT_NODE_INTSECT__:
	case __TMMODE_STRUCT_NODE_RENUMBER__:  

	case __TMMODE_STRUCT_ELEM__:
	case __TMMODE_STRUCT_ELEM_CREATE__:   
	case __TMMODE_STRUCT_ELEM_CURVE__:
	case __TMMODE_STRUCT_ELEM_CONV_LINE_CREATE__: 
	case __TMMODE_STRUCT_ELEM_WALL_OPEN_CREATE__:
	case __TMMODE_STRUCT_ELEM_DELETE__:   
	case __TMMODE_STRUCT_ELEM_TRANSLATE__:
	case __TMMODE_STRUCT_ELEM_MIRROR__:   
	case __TMMODE_STRUCT_ELEM_ROTATE__:   
	case __TMMODE_STRUCT_ELEM_EXTRUDE__:   
	case __TMMODE_STRUCT_ELEM_DIVIDE__:   
	case __TMMODE_STRUCT_ELEM_MERGE__:   
	case __TMMODE_STRUCT_ELEM_CHANGE__:
	case __TMMODE_STRUCT_ELEM_COMPACT__:
	case __TMMODE_STRUCT_ELEM_RENUMBER__:  
	case __TMMODE_STRUCT_ELEM_INTSECT__:
	case __TMMODE_STRUCT_ELEM_DEP_MAT_CHANGE__:
	case __TMMODE_STRUCT_TPSC_GROUP__:
	case __TMMODE_STRUCT_SECT_FOR_RFOC__:
	case __TMMODE_STRUCT_ELEM_ESSF__:
	case __TMMODE_STRUCT_ELEM_WSSF__:
	case __TMMODE_STRUCT_ELEM_ECCEN__:
	case __TMMODE_STRUCT_ELEM_PSSF__:

	case __TMMODE_STRUCT_BNDR__:
	case __TMMODE_STRUCT_BNDR_SKEW__:
	case __TMMODE_STRUCT_BNDR_CONS__:
	case __TMMODE_STRUCT_BNDR_NSPR__:
	case __TMMODE_STRUCT_BNDR_ELNK__:
	case __TMMODE_STRUCT_BNDR_FRLS__:
	case __TMMODE_STRUCT_BNDR_OFFS__:
	case __TMMODE_STRUCT_BNDR_PRLS__:
	case __TMMODE_STRUCT_BNDR_RIGD__:
	case __TMMODE_STRUCT_BNDR_MCON__:
	case __TMMODE_STRUCT_BNDR_DRLS__:
	case __TMMODE_STRUCT_BNDR_IELC__:
	case __TMMODE_STRUCT_BNDR_GSPR__:
	case __TMMODE_STRUCT_BNDR_SSPR__:
	case __TMMODE_STRUCT_BNDR_SOSP__:
	case __TMMODE_STRUCT_BNGR_CHANGE__:
	case __TMMODE_STRUCT_BNDR_STORY__:
	case __TMMODE_STRUCT_BNDR_EWSF__:
	case __TMMODE_STRUCT_BNDR_CGLP__:
	case __TMMODE_STRUCT_BNDR_NLLINK__:
	case __TMMODE_STRUCT_BNDR_CLDR__:
	case __TMMODE_STRUCT_BNDR_LINK_COMPACT__:
	case __TMMODE_STRUCT_BNDR_LINK_RENUMBER__:
	case __TMMODE_STRUCT_BNDR_RELT__:

	case __TMMODE_STRUCT_STLD__:
	case __TMMODE_STRUCT_STLD_CNLD__:
	case __TMMODE_STRUCT_STLD_CNLD_TYPE__:
	case __TMMODE_STRUCT_STLD_SDSP__:
	case __TMMODE_STRUCT_STLD_BODF__:
	case __TMMODE_STRUCT_RESPOSNS_DISP__:
	case __TMMODE_STRUCT_STLD_NBOF__:
	case __TMMODE_STRUCT_STLD_BMLD__:
	case __TMMODE_STRUCT_STLD_LBLD__:
	case __TMMODE_STRUCT_STLD_FBLD__:
	case __TMMODE_STRUCT_STLD_TBLD__:
	case __TMMODE_STRUCT_STLD_PRST__:
	case __TMMODE_STRUCT_STLD_PRTS__:
	case __TMMODE_STRUCT_STLD_PRES__:
	case __TMMODE_STRUCT_STLD_HPRS__:
	case __TMMODE_STRUCT_STLD_STMP__:
	case __TMMODE_STRUCT_STLD_SSEI__:
	case __TMMODE_STRUCT_STLD_NTMP__:
	case __TMMODE_STRUCT_STLD_ETMP__:
	case __TMMODE_STRUCT_STLD_FIPA__:
	case __TMMODE_STRUCT_STLD_FIRE__:
	case __TMMODE_STRUCT_STLD_FSSF__:
	case __TMMODE_STRUCT_STLD_GTMP__:
	case __TMMODE_STRUCT_STLD_IFGS__:
	case __TMMODE_STRUCT_STLD_SWIB__:
	case __TMMODE_STRUCT_STLD_TENDON__:
	case __TMMODE_STRUCT_STLD_TENDON_PROP__:
	case __TMMODE_STRUCT_STLD_TENDON_PRST__:
	case __TMMODE_STRUCT_STLD_TLCS__:
	case __TMMODE_STRUCT_STLD_CRPC__:
	case __TMMODE_STRUCT_STLD_STBK__:
	case __TMMODE_STRUCT_STLD_ULCB__:
	case __TMMODE_STRUCT_STLD_CMCS__:
	case __TMMODE_STRUCT_LDGR_CHANGE__:
	case __TMMODE_STRUCT_STLD_PNLD__:
	case __TMMODE_STRUCT_STLD_BTMP__:	
	case __TMMODE_STRUCT_STLD_PTMP__:
	case __TMMODE_STRUCT_STLD_FMLD__:    //2 added by KYM
	case __TMMODE_STRUCT_MASS__:
	case __TMMODE_STRUCT_MASS_NMAS__:
	case __TMMODE_STRUCT_MASS_DMAS__:
	case __TMMODE_STRUCT_STLD_LAPL__:
	case __TMMODE_STRUCT_STLD_PRES_AREA__:
	case __TMMODE_STRUCT_STLD_EPSE__:
	case __TMMODE_STRUCT_STLD_EPST__:
	case __TMMODE_STRUCT_STLD_PRES_QUERY__:
		::SetWindowPos(m_wndNodeElemTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	// Added by BugBoy for Settlement Tab
	// 2000.8.10
	case __TMMODE_STRUCT_STTL__:
	case __TMMODE_STRUCT_STTL_GROUP__:
	case __TMMODE_STRUCT_STTL_LOADCASE__:
		::SetWindowPos(m_wndSettlementTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	case __TMMODE_STRUCT_HYDR__:
	case __TMMODE_STRUCT_HYDR_TEMPER__:
	case __TMMODE_STRUCT_HYDR_HEAT__:
	case __TMMODE_STRUCT_HYDR_CONVECT__:
	case __TMMODE_STRUCT_HYDR_MATERIAL__:
			::SetWindowPos(m_wndHydrationTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	case __TMMODE_STRUCT_HYDR_TIME_DEP_MAT__:
			::SetWindowPos(m_wndTimeDepTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
		//2019.9.20
	case __TMMODE_STRUCT_MHTP_MAT_LINK_CH__:
		::SetWindowPos(m_wndHighTemperatureTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	case __TMMODE_CMD_THGA__:
	case __TMMODE_CMD_THNL__:
	case __TMMODE_CMD_THSP__:
	case __TMMODE_CMD_THMS__:
	case __TMMODE_CMD_THRD__:
	case __TMMODE_CMD_THRD_DISP__:
	case __TMMODE_CMD_THRD_TRUSS__:
	case __TMMODE_CMD_THRD_BEAM__:
	case __TMMODE_CMD_THRD_PLANE_STRESS__:
	case __TMMODE_CMD_THRD_PLANE_STRAIN__:
	case __TMMODE_CMD_THRD_PLATE__:
	case __TMMODE_CMD_THRD_SOLID__:
	case __TMMODE_CMD_THRD_GENERAL_LINK__:
	case __TMMODE_CMD_THRD_INELASTIC_HINGE__:
	case __TMMODE_CMD_THRC_TIME_INCREMENT__:
	case __TMMODE_CMD_THRD_STEP_TIMESTEP__:
	case __TMMODE_CMD_THRD_STEP_DISPVELACC__:
	case __TMMODE_CMD_THRD_STEP_TRUSS__:
	case __TMMODE_CMD_THRD_STEP_BEAM__:
	case __TMMODE_CMD_THRD_STEP_DESIGDOF__:
	case __TMMODE_CMD_THAT__:
	case __TMMODE_CMD_THCR__:
	case __TMMODE_CMD_SPEC__:
	case __TMMODE_CMD_NPLN__:
	case __TMMODE_CMD_MLSP__:
	case __TMMODE_CMD_MLSR__:
	case __TMMODE_CMD_MLTT__:
	case __TMMODE_CMD_JIMP__:
	case __TMMODE_CMD_SINF__:
	case __TMMODE_CMD_BGEN__:
	case __TMMODE_CMD_POHA__:
	case __TMMODE_CMD_IEPI__:
	case __TMMODE_CMD_IEHA__:
	case __TMMODE_CMD_BSPN__:
	case __TMMODE_CMD_GIMP__:
	case __TMMODE_CMD_P1LA__:
	case __TMMODE_CMD_CREF__:
	case __TMMODE_CMD_GLTM__:
	case __TMMODE_CMD_NMRD__:
	case __TMMODE_CMD_LNKF__:
	case __TMMODE_CMD_GSTY__:
	case __TMMODE_CMD_ADD_IMPF__:  
	case __TMMODE_CMD_DYFG__:
	case __TMMODE_CMD_DYNF__:    
	case __TMMODE_CMD_SIHO_ELEM__	:
	case __TMMODE_CMD_SIHO_GLINK__	:
	case __TMMODE_CMD_SIHO_PSSPOT__:
	case __TMMODE_CMD_SPHO_ELEM__	:
	case __TMMODE_CMD_SPHO_GLINK__	:
		::SetWindowPos(m_wndCmdTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_CMD_WIND_PRESSURE_AREA__:
	case __TMMODE_CMD_WIND_PRESSURE_BEAM__:
	case __TMMODE_CMD_WIND_PRESSURE_NODAL__:
	case __TMMODE_CMD_WIND_PRESSURE_FUNC__:
		::SetWindowPos(m_wndWindPTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	// mesh
	case __TMMODE_STRUCT_MESH_AUTO__  : 
	case __TMMODE_STRUCT_MESH_MAP_KE__:
	case __TMMODE_STRUCT_MESH_MAP__   :    
		::SetWindowPos(m_wndMeshTab.m_hWnd, NULL, rectInside.left, rectInside.top,
		rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	break;

	case __TMMODE_STRUCT_EBEAM_CREATE__  : 
		::SetWindowPos(m_wndEBeamTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	case __TMMODE_STRUCT_STRIP_ASSIGN__  : 
		::SetWindowPos(m_wndStripTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_STRUCT_DROPPANEL_ASSIGN__  : 
		::SetWindowPos(m_wndDropPanelTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_STRUCT_COLMCAP_ASSIGN__  : 
		::SetWindowPos(m_wndColmCapTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_MESHDGN_SLABSHEAR_REINFORCE__:
	case __TMMODE_MESHDGN_COLUMN_LOCATION__  : 
		::SetWindowPos(m_wndColmLocTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	// Seismic Tab
	case __TMMODE_STRUCT_SEIS_CLEAR_HGT_COL__: 
	case __TMMODE_STRUCT_SEIS_SEISMIC_HOOK__:  
	case __TMMODE_STRUCT_SEIS_COMP_TYPE__:  
	case __TMMODE_STRUCT_SEIS_REINF_DETAILS__:
	case __TMMODE_STRUCT_SEIS_RC_COLUMN_M__:
	case __TMMODE_STRUCT_SEIS_PM_CURVE_METHOD__:
	case __TMMODE_STRUCT_SEIS_PBD_SEBE__:
	case __TMMODE_STRUCT_SEIS_PBD_SECO__:
	case __TMMODE_STRUCT_SEIS_PBD_SEWA__:
	case __TMMODE_STRUCT_SEIS_PBD_HLFW__:
	case __TMMODE_STRUCT_SEIS_PBD_SESP__:
		::SetWindowPos(m_wndSeisCommonTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	case __TMMODE_STRUCT_SEIS_XY_FACTOR__:
		::SetWindowPos(m_wndSeisXYFactTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;

	//	General Tab
	case __TMMODE_DGN_GEN_MEMB__:
	case __TMMODE_DGN_GEN_MEMB_CHG__:
	case __TMMODE_DGN_GEN_LENG__:
	case __TMMODE_DGN_GEN_SPLN__:
	case __TMMODE_DGN_GEN_KFAC__:
	case __TMMODE_DGN_GEN_LTSR__:
	case __TMMODE_DGN_GEN_CMFT__:
	case __TMMODE_DGN_GEN_FMAG__:
	case __TMMODE_DGN_GEN_BCIF__:
	case __TMMODE_DGN_GEN_PMCR__:
	case __TMMODE_DGN_GEN_REDU__:
	case __TMMODE_DGN_GEN_SUEQ__:
	case __TMMODE_DGN_GEN_GREQ__:
	case __TMMODE_DGN_GEN_MBTP__:
	case __TMMODE_DGN_TWR_MBTP__:
	case __TMMODE_DGN_GEN_SEIS__:
	case __TMMODE_DGN_GEN_HCBM__:
	case __TMMODE_DGN_GEN_EWSD__:// Add by sshan. MNET:2729.('20070514)
	case __TMMODE_DGN_GEN_SEIS_DGN__:
	case __TMMODE_DGN_GEN_ULCT__:
	case __TMMODE_DGN_GEN_DSST__:
	case __TMMODE_DGN_GEN_ARST__:
	  //	Steel Tab
	case __TMMODE_DGN_STL_SERV__: // Coded by Seungjun MNet:No.2393 ('20061011)
	case __TMMODE_DGN_STL_CBFT__:
	case __TMMODE_DGN_STL_CVFT__:
	case __TMMODE_DGN_STL_DALW__:
	case __TMMODE_DGN_STL_UCFA__:
	case __TMMODE_DGN_STL_STFN__:
	case __TMMODE_DGN_STL_SLRS__:
	case __TMMODE_DGN_STL_SMSP__:
	case __TMMODE_DGN_STL_JP_POSITION__:
	case __TMMODE_DGN_STL_STBD__:
	case __TMMODE_DGN_STL_STUS__:
	case __TMMODE_DGN_STL_STRE__:
	case __TMMODE_DGN_STL_PHIB__://add by maxiao 2015-10-12
	case __TMMODE_DGN_STL_SETY__:	//Add by tss 2020/02/12
	case __TMMODE_DGN_STL_BMRO__:	//add by tss 2021/01/13
	case __TMMODE_DGN_STL_SPDF__:	//Add by tss 2024/02/29
	case __TMMODE_DGN_STL_IMEL__:	//Add by tss
	case __TMMODE_DGN_STL_INDF__:
	case __TMMODE_DGN_STL_DSAU__:
	case __TMMODE_DGN_STL_DSAS__:
    case __TMMODE_DGN_STL_SDSW__://add by maxiao 2015-10-22
	case __TMMODE_DGN_STL_PFMC__:
	case __TMMODE_DGN_STL_PFCM__:
	case __TMMODE_DGN_STL_PFDL__:
	case __TMMODE_DGN_STL_PFMP__:
	case __TMMODE_DGN_STL_PFNP__:
	case __TMMODE_DGN_STL_SPSC__:
	case __TMMODE_DGN_STL_SPCS__:
	case __TMMODE_DGN_STL_SPLS__:
	case __TMMODE_DGN_STL_SPPB__:
	case __TMMODE_DGN_STL_SPLB__:
	case __TMMODE_DGN_STL_SP14SeisMembParams__:
	case __TMMODE_DGN_STL_SP16SafetyMembParams__:
	case __TMMODE_DGN_STL_SP16CombinedSectParams__:
		//ALU
	case __TMMODE_DGN_ALU_ARIB__:
	case __TMMODE_DGN_ALU_AWED__:
	case __TMMODE_DGN_ALU_AHTR__:
	case __TMMODE_DGN_ALU_APHI__:

	  //	RC Tab
	case __TMMODE_DGN_CON_REBB__:
	case __TMMODE_DGN_CON_REBC__:
	case __TMMODE_DGN_CON_REBR__:
	case __TMMODE_DGN_CON_REBW__:
	case __TMMODE_DGN_CON_WMAK__:
	case __TMMODE_DGN_CON_BEMW__:
	case __TMMODE_DGN_CON_EBMW__:
	case __TMMODE_DGN_CON_CVL_SUFC__:
	case __TMMODE_DGN_CON_CVL_RMFR__:
	case __TMMODE_DGN_CON_CVL_ULSP__:
	case __TMMODE_DGN_CON_CVL_RDGN_BEAM__:
	case __TMMODE_DGN_CON_CVL_RDGN_COLU__:
	case __TMMODE_DGN_CON_JSFT__: //Add By RSH 2002.12.26
	case __TMMODE_DGN_CON_PSAS__: //Add By RSH 2002.12.02
	case __TMMODE_DGN_CON_MRFT__: //Add By RSH 2003.07.04	
	case __TMMODE_DGN_CON_MCMB__:
	case __TMMODE_DGN_CON_DFBA__:
	case __TMMODE_DGN_CON_SERV__: // Coded by Seungjun MNet:No.2393 ('20060828)
	case __TMMODE_DGN_CON_SPVAREMA__: // Coded by Seungjun MNet:No.2393 ('20060828)
	case __TMMODE_DGN_CON_SERV_SLAB__: 
	case __TMMODE_DGN_CON_REIN_SHEAR__:
	case __TMMODE_DGN_GEN_MEMB_PLATE__:
	case __TMMODE_DGN_CON_ELEM_BAR__: // Coded by sshan MNet:No.2484 ('20061109)
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM: 
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_COL:  
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC: 
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL: 
	case __TMMODE_DGN_CON_BEAM_BAR__:
	case __TMMODE_DGN_CON_DORC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_PORC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_CWRC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_LEKF__:// Coded by sshan MNet:No.2607 ('20061226)		
	case __TMMODE_DGN_CON_RSDM__:// Coded by bjshin MNet:No.3992 ('20090517)
	case __TMMODE_DGN_CON_TRFT__:// Coded by sshan MNet:No.3780 ('20090320)
	case __TMMODE_DGN_CON_UCCF__:// Add by GAY. PMS:4267. ('11.12.27). 중국신기준-불확정성 분항계수.
	case __TMMODE_DGN_CON_REXC__:
	case __TMMODE_DGN_CON_PMDM__:
	case __TMMODE_DGN_CON_SCOL__:
	case __TMMODE_DGN_CON_OCDF__:
	case __TMMODE_DGN_CON_PBDC__:
	case __TMMODE_DGN_CON_PCDC__:
	case __TMMODE_DGN_CON_PBDD__:
	case __TMMODE_DGN_CON_PCDD__:
	case __TMMODE_DGN_CON_STAN__:
	case __TMMODE_DGN_CON_MNDC__:
	case __TMMODE_DGN_CON_SAFM__:
	case __TMMODE_DGN_CON_RRTR__:
	case __TMMODE_DGN_CON_EXPOSURE_CLASS__:
	  //	SRC Tab
	case __TMMODE_DGN_SRC_RBSB__:
	case __TMMODE_DGN_SRC_REBS__:
		//	Cold Formed Steel Tab
	case __TMMODE_DGN_CFS_SERV__:
	case __TMMODE_DGN_CFS_CBFT__:
	case __TMMODE_DGN_CFS_CSOS__:
	case __TMMODE_DGN_CFS_CSMS__:
	case __TMMODE_DGN_CFS_PLIN__:
	case __TMMODE_DGN_CFS_SPHI__:
	case __TMMODE_DGN_CFS_PFMC__:
	case __TMMODE_DGN_CFS_PFCM__:
	case __TMMODE_DGN_CFS_PFDL__:
	case __TMMODE_DGN_CFS_PFMP__:
	case __TMMODE_DGN_CFS_PFNP__:
	  // PSC Tab
	case __TMMODE_DGN_CRC_DGN_POSC__:
	case __TMMODE_DGN_CRC_DGN_POSX__:
	case __TMMODE_DGN_PSC_CRACK_PSCW__:
	case __TMMODE_DGN_PSC_SEGMENT__:
	case __TMMODE_DGN_PSC_EXPOSURE_CLASS__:
	case __TMMODE_DGN_PSC_OCDF__:
	case __TMMODE_DGN_PSC_SHEAR_CONNECTOR__:
	case __TMMODE_DGN_PSC_INTERFACE_SHEAR__:
	case __TMMODE_DGN_PSC_FATIGUE_CHECK__:
	  // CPG Tab
	case __TMMODE_DGN_CPG_STIF__:
	case __TMMODE_DGN_CPG_SHEAR__:
	case __TMMODE_DGN_CPG_POSITION__:
	case __TMMODE_DGN_CPG_POSI_DGN_OUT__:
	case __TMMODE_DGN_SOD_FLEXBUCK_INPUT__:
	case __TMMODE_DGN_SOD_POSI_DGN_OUT__:
	case __TMMODE_DGN_CPG_LOAD_APPLICATION__:
	case __TMMODE_DGN_CPG_TORSION__:
	case __TMMODE_DGN_CPG_DAMAGE__:
	case __TMMODE_DGN_CPG_SHEAR_CONN__://PMS:LRFD12 BSC-20131231
	case __TMMODE_DGN_CPG_SHEAR_CONN_CSA__:
	case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
	  //case __TMMODE_DGN_CPG_UNBRA_LENG__:
	case __TMMODE_DGN_CPG_CURVE_BRID__:
	case __TMMODE_DGN_CPG_DECK_OVERHANG__:
	case __TMMODE_DGN_CPG_SHEAR_CONN_GROUP__:
	case __TMMODE_DGN_CPG_SERV__:
	case __TMMODE_DGN_CPG_LENG__:
	case __TMMODE_DGN_CPG_WEB_PANEL__:
	case __TMMODE_DGN_CPG_RESIDUAL_STRESS__:
	  // Rating Tab
	case __TMMODE_DGN_RATING_OPTION__:
	case __TMMODE_DGN_RATING_PRINT__:
	case __TMMODE_DGN_PSC_ASSESSMENT_OPTION__:
	case __TMMODE_DGN_RATING_PSC_ENVIRONMENT_CLASS__:
	case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:
	case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:
	case __TMMODE_DGN_STEEL_RATING_OPTION__:
	case __TMMODE_DGN_STEEL_RATING_PRINT__:
	case __TMMODE_DGN_STEEL_RATING_WEB_PANEL__:
	case __TMMODE_DGN_STEEL_RATING_TORSION__:
	case __TMMODE_DGN_STEEL_RATING_SHEAR_CONN__:
	case __TMMODE_DGN_STEEL_RATING_SERVICE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_ASSESS_CHECK__:
	case __TMMODE_DGN_STEEL_RATING_ASSESS_REPORT__:
	case __TMMODE_DGN_STEEL_RATING_EFFECT_LENGTH_U_FRAME__:
	case __TMMODE_DGN_STEEL_RATING_MOMENT_DIST_FACT_SLENDER__:
	case __TMMODE_DGN_STEEL_RATING_FASTENER_RIVET__:
	case __TMMODE_DGN_STEEL_RATING_ASSIGN_MEMB_FACTOR__:

	  // RC Rating Tab
	case __TMMODE_DGN_RC_RATING_OPTION_BEAM__:
	case __TMMODE_DGN_RC_RATING_OPTION_PLATE__:
	case __TMMODE_DGN_RC_RATING_DGN_OPTION_PLATE__:
	case __TMMODE_DGN_RC_RATING_PRINT_BEAM__:
	case __TMMODE_DGN_RC_RATING_PRINT_PLATE__:	 
	{
		::SetWindowPos(m_wndDgnTab.m_hWnd, NULL, rectInside.left, rectInside.top,
					   rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}break;

/////////////////////////////////////////////////////////////////////////////
// GPS 모드의 경우 시작 by mjpark                     ///////////////////////
/////////////////////////////////////////////////////////////////////////////
	case __TMMODE_GPS_DISP_SRHDSP__:
	case __TMMODE_GPS_DISP_DEFORM__:
	case __TMMODE_GPS_DISP_DCONTR__:
	case __TMMODE_GPS_ISOLATION_DISP__:
	case __TMMODE_GPS_DUTL_FACT__:
	case __TMMODE_GPS_PSOV_DUTL_FACT__:
	case __TMMODE_GPS_FORC_TFORC__:
	case __TMMODE_GPS_FORC_BFORC__:
	case __TMMODE_GPS_ISOLATION_FORCE__:
	case __TMMODE_GPS_FORC_SFDBMD__:
	case __TMMODE_GPS_FORC_SFDBTF__:
	case __TMMODE_GPS_FORC_BDGRM__:
	case __TMMODE_GPS_FORC_WFORC__:
	case __TMMODE_GPS_FORC_WSFBM__:
	case __TMMODE_GPS_FORC_PFORC__:
	case __TMMODE_GPS_FORC_PCLPD__:
	case __TMMODE_GPS_FORC_ESFBM__:
	case __TMMODE_GPS_FORC_RFRCD__:
	case __TMMODE_GPS_PTDN_FORCE__:
	case __TMMODE_GPS_STRESS_SECV__:
	case __TMMODE_GPS_STRS_TSTRS__:
	case __TMMODE_GPS_STRS_BSTRSDGRM__:
	case __TMMODE_GPS_STRS_BSTRS__:
	case __TMMODE_GPS_STRS_BSTRSDETAIL__:
	case __TMMODE_GPS_STRS_BSTRSDETAILDGRM__:
	case __TMMODE_GPS_STRS_BSTRSINC__:
	case __TMMODE_GPS_PTDN_STRESS__:
	case __TMMODE_GPS_STRS_PSTRS__:
	case __TMMODE_GPS_STRS_PNSTRS__:
	case __TMMODE_GPS_STRS_AXSTRS__:
	case __TMMODE_GPS_STRS_SSTRS__:
	case __TMMODE_GPS_ISOLATION_STRS__:
	case __TMMODE_GPS_REAC_SRHRCF__:
	case __TMMODE_GPS_REAC_TARGET__:
	case __TMMODE_GPS_REAC_REACF__:
	case __TMMODE_GPS_SOIL_PRESS__:
	case __TMMODE_GPS_DYNA_EGMDSHP__:
	case __TMMODE_GPS_DYNA_BKMDSHP__:
	case __TMMODE_GPS_INFL_REAC__:
	case __TMMODE_GPS_INFL_DISP__:
	case __TMMODE_GPS_INFL_TFORC__:
	case __TMMODE_GPS_INFL_BFORC__:
	case __TMMODE_GPS_INFL_ELNKFORC__:
	case __TMMODE_GPS_INFL_NLNKFORC__:
	case __TMMODE_GPS_INFL_PFORC__:
	case __TMMODE_GPS_INFL_BSTRS__:
	case __TMMODE_GPS_INFL_SSTRS__:

	case __TMMODE_GPS_INFG_REAC__:
	case __TMMODE_GPS_INFG_DISP__:
	case __TMMODE_GPS_INFG_TFORC__:
	case __TMMODE_GPS_INFG_BFORC__:
	case __TMMODE_GPS_INFG_BSTRS__:
	case __TMMODE_GPS_INFG_ELNK__:
	case __TMMODE_GPS_INFG_NLNK__:

	case __TMMODE_GPS_GRIDTRC_REAC__:
	case __TMMODE_GPS_GRIDTRC_DISP__:
	case __TMMODE_GPS_GRIDTRC_TFORC__:
	case __TMMODE_GPS_GRIDTRC_BFORC__:
	case __TMMODE_GPS_GRIDTRC_ELNK__:
	case __TMMODE_GPS_GRIDTRC_NLNK__:

	case __TMMODE_GPS_MVLTRC_REAC__:
	case __TMMODE_GPS_MVLTRC_DISP__:
	case __TMMODE_GPS_MVLTRC_TFORC__:
	case __TMMODE_GPS_MVLTRC_BFORC__:
	case __TMMODE_GPS_MVLTRC_ELNKFORC__:
	case __TMMODE_GPS_MVLTRC_NLNKFORC__:
	case __TMMODE_GPS_MVLTRC_PFORC__:
	case __TMMODE_GPS_MVLTRC_BSTRS__:
	case __TMMODE_GPS_INFS_REAC__:
	case __TMMODE_GPS_INFS_DISP__:
	case __TMMODE_GPS_INFS_TFORC__:
	case __TMMODE_GPS_INFS_BFORC__:
	case __TMMODE_GPS_INFS_ELNKFORC__:
	case __TMMODE_GPS_INFS_NLNKFORC__:
	case __TMMODE_GPS_INFS_PFORC__:
	case __TMMODE_GPS_INFS_BSTRS__:
	case __TMMODE_GPS_INFS_SSTRS__:
	case __TMMODE_GPS_DSGN_STEEL__:
	case __TMMODE_GPS_DSGN_STEEL_LSD__:
	case __TMMODE_SEIS_CVL_JP_RESULT_3D_INVESTIGATE__:
	case __TMMODE_GPS_DSGN_CFSTL__:
	case __TMMODE_GPS_DSGN_ALU__:
	case __TMMODE_GPS_DSGN_FIRE__:
	case __TMMODE_GPS_DSGN_PFDN__:
	case __TMMODE_GPS_DSGN_PFDNCFS__:
	case __TMMODE_GPS_DGN_DAMP_STEEL__:
	case __TMMODE_GPS_DSGN_CNCRT__:
	case __TMMODE_GPS_DSGN_SRC__:
	case __TMMODE_GPS_TIMEHISTORY_GRAPH__:
	case __TMMODE_GPS_TIMEHISTORY_TEXT__:
	case __TMMODE_GPS_TIMEHISTORY_SSGP__:
	case __TMMODE_GPS_HYDRAHEAT_SSTRS__	:
	case __TMMODE_GPS_HYDRAHEAT_TEMPER__:
	case __TMMODE_GPS_HYDRAHEAT_DISPL__	 :
	case __TMMODE_GPS_HYDRAHEAT_ALTENSSTRS__:
	case __TMMODE_GPS_HYDRAHEAT_CRACKRATIO__:
	case __TMMODE_GPS_HYDRAHEAT_GRAPH__:
	case __TMMODE_GPS_STAGESTEP_GRAPH__:
	case __TMMODE_GPS_GIRDERSTRS_DGRM__:
	case __TMMODE_GPS_COLUMNSHORT_GRP__: //KYE-HONG-20020327
	case __TMMODE_GPS_COLUMNSHORT_TXT__: //KYE-HONG-20020327
	case __TMMODE_GPS_MVLTRC_BATCH__:
	case __TMMODE_GPS_DISPPART_FACT:
	case __TMMODE_GPS_LOCAL_AXIS:
	case __TMMODE_GPS_SSFR__:
	case __TMMODE_GPS_THIS_DISP__:
	case __TMMODE_GPS_THIS_BFORC__:
	case __TMMODE_GPS_THIS_BDGRM__:
	case __TMMODE_GPS_THIS_BSTRS__:
	case __TMMODE_GPS_PSOV_SSDF__:
	case __TMMODE_GPS_PSOV_MMDS__:
	case __TMMODE_GPS_PSOV_SSDR__:
	case __TMMODE_GPS_DEFN_EDGRM__:
	case __TMMODE_GPS_DRAW_EDGRM__:
	case __TMMODE_GPS_PSC_DSGN_DGRM__:
	case __TMMODE_GPS_BRDG_LOAD_DGRM__:
	case __TMMODE_GPS_STEEL_RATING_DGRM__:
	case __TMMODE_GPS_STEEL_RATING_AASHTO_DGRM__:	
	case __TMMODE_GPS_PSC_RATING_DGRM__:
	case __TMMODE_GPS_RC_RATING_DGRM__:
	case __TMMODE_GPS_SOD_DGRM__:
	case __TMMODE_GPS_NRRS__:
	case __TMMODE_GPS_BCCR__:
	case __TMMODE_GPS_RC_DSGN_DGRM__:
	case __TMMODE_GPS_RAIL_PSC_DSGN_DGRM__:
	case __TMMODE_GPS_RAIL_RC_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_COMP_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_COMP_DSGN_AS5100_DGRM__:
	case __TMMODE_GPS_PUSHOVER_GRAPH__:
	case __TMMODE_GPS_PUSHOVER_SSGP__:
	case __TMMODE_GPS_PUSHOVER_TEXT__:
	case __TMMODE_GPS_PSOV_FUNCTION__:
	case __TMMODE_GPS_DGN_SLAB_RESULT__:
	case __TMMODE_GPS_DGN_SLAB_CHECK__:
	case __TMMODE_GPS_DGN_SHEAR_CHECK__:
	case __TMMODE_GPS_DGN_SLABSERV_CHECK__:
	case __TMMODE_GPS_DGN_WALL_RESULT__:
	case __TMMODE_GPS_DGN_WALL_CHECK__:
	case __TMMODE_GPS_DGN_STRIP_RESULT__:
	case __TMMODE_GPS_DGN_STRIP_CHECK__:
	case __TMMODE_GPS_CPG_RUS_SHEAR_COON_FORCE_DGRM__:
	case __TMMODE_GPS_CPG_RUS_RESULT_DGRM__: //KHD
	case __TMMODE_GPS_DGN_SHELL_RESULT__:
	case __TMMODE_GPS_DGN_SHELL_CHKECK__:
	case __TMMODE_GPS_DGN_SHELL_SHEAR_CHKECK__:
	case __TMMODE_GPS_DGN_BATCH_REPORT__:
	case __TMMODE_GPS_SEIS_1ST_RC_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_RC_PERFORM__:
	case __TMMODE_GPS_SEIS_1ST_STL_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_STL_PERFORM__:
	case __TMMODE_GPS_SEIS_1ST_MASONRY_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_MASONRY_PERFORM__:
	case __TMMODE_GPS_THIS_ENRG_GRAPH__:
	case __TMMODE_GPS_THIS_ENRG_GROUP_GRAPH__:
	case __TMMODE_GPS_PLATE_STRAIN__:
	case __TMMODE_GPS_SOLID_STRAIN__:
	case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_PO__:
	case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_TH__:
	case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_STORY_GRAPH_PFRM_TH__:
		::SetWindowPos(m_wndGPSTab.m_hWnd, NULL, rectInside.left, rectInside.top,
					   rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		if(CViewBase::GetCurView_ST())
			CViewBase::GetCurView_ST()->GetIPM()->m_bBarClosed = FALSE; // Added By LSS
		break;

/////////////////////////////////////////////////////////////////////////////
// GPS 모드의 경우 끝 by mjpark                       ///////////////////////
/////////////////////////////////////////////////////////////////////////////

	// 2차 설계
	case __TMMODE_JUD_SLAB__:   
		::SetWindowPos(m_wndJUDTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	break;
	case __TMMODE_SEIS_INFILL_WALL_CREATE__:
	case __TMMODE_SEIS_INFILL_WALL_REDUCE__:   
		::SetWindowPos(m_wndInfillTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_SEIS_EFFECT_STIFF__:
	case __TMMODE_SEIS_GENERATE_EFFECT_STIFF__:
		::SetWindowPos(m_wndSeisTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_SEIS_1ST_RC_M_FACTOR__:
	case __TMMODE_SEIS_1ST_STL_M_FACTOR__:
	case __TMMODE_SEIS_1ST_MASONRY_M_FACTOR__:
		::SetWindowPos(m_wndSeisMFactTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_SEIS_CVL_EVAL_GROUP__:
	case __TMMODE_SEIS_CVL_SECT_EFFECT_STIFF__:
		::SetWindowPos(m_wndSeisCvlTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_SEIS_CVL_RESULT_EVAL_PARAM__:
		::SetWindowPos(m_wndSeisCvlMiscTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
		break;
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_UPPER__:
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_LOWER__:
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_USER__:
		::SetWindowPos(m_wndSeisSetTargetTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		break;
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIMD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIRD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIBD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIDP__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SISR__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIET__:
		::SetWindowPos(m_wndSeisSetInvestTab.m_hWnd, NULL, rectInside.left, rectInside.top,
			rectInside.Width(), rectInside.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		break;

	// Post Tension
	case __TMMODE_PT_DRAW_TENDON_LINE_BEAM__:
	case __TMMODE_PT_DRAW_TENDON_LINE_SLAB__:
	case __TMMODE_PT_DRAW_TRANS_TENDON__:
	case __TMMODE_PT_DRAW_MOD_TENDON__:
	case __TMMODE_PT_DRAW_DEL_TENDON__:
	case __TMMODE_PT_DRAW_RENUM_TENDON__:
	case __TMMODE_PT_TENDON_MATCH_PROFILE__:
	case __TMMODE_PT_SUPPORT_LINE_CREATE__:
	case __TMMODE_PT_DESIGN_STRIP_CREATE__:
	case __TMMODE_PT_DESIGN_STRIP_AUTO__:
	case __TMMODE_PT_DESIGN_STRIP_MODIFY_SECTION__:
		{
			::SetWindowPos(m_wndPostTensionTab.m_hWnd, NULL, rectInside.left, rectInside.top,
				rectInside.Width(), rectInside.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	default:
		break;
	}
}

BOOL  CBarTreeMenu::_ChangeMode(int nCurMode)
{
	MInitCombo initCombo;
	SetRedraw(FALSE);

	// __TMMODE_TREEMENU__는 현재 TAB 모드로 변환
	nCurMode = RefreshCurTreeMenuMode(nCurMode);

	if (m_nCurMode == nCurMode)
	{
		SetRedraw(TRUE);
		return FALSE;
	}

	_FaceEdgeSelModeMan(nCurMode);

	m_ThisMode = nCurMode;


	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_MVCD_D MvcdD;
	if(!pDoc->m_pAttrCtrl->GetMvcd(MvcdD))
	{
		MvcdD.Initialize();
	}
	int nMCodeType = MvcdD.nCodeType;
		
	switch(m_nCurMode)
	{
	case __TMMODE_TREEMENU__:
	case __TMMODE_TREEMENU_MENU__:
	case __TMMODE_TREEMENU_TABLE__:
	case __TMMODE_TREEMENU_GROUP__:
	case __TMMODE_TREEMENU_WORK__:
	case __TMMODE_TREEMENU_REPORT__:
	case __TMMODE_TREEMENU_JUD__:  
	case __TMMODE_TREEMENU_SEIS__:
		_GetCurWnd()->ShowWindow(SW_HIDE);
		break;

	case __TMMODE_STRUCT_NODE__:
	case __TMMODE_STRUCT_NODE_COMPACT__:
	case __TMMODE_STRUCT_NODE_CREATE__:
	case __TMMODE_STRUCT_NODE_DELETE__:
	case __TMMODE_STRUCT_NODE_TRANSLATE__:
	case __TMMODE_STRUCT_NODE_MIRROR__:
	case __TMMODE_STRUCT_NODE_ROTATE__:
	case __TMMODE_STRUCT_NODE_PROJECT__:
	case __TMMODE_STRUCT_NODE_DIVIDE__:
	case __TMMODE_STRUCT_NODE_MERGE__:
	case __TMMODE_STRUCT_NODE_SCALE__:
	case __TMMODE_STRUCT_NODE_INTSECT__:
	case __TMMODE_STRUCT_NODE_RENUMBER__: 

	case __TMMODE_STRUCT_ELEM__:
	case __TMMODE_STRUCT_ELEM_CREATE__:   
	case __TMMODE_STRUCT_ELEM_CURVE__:
	case __TMMODE_STRUCT_ELEM_CONV_LINE_CREATE__:   
	case __TMMODE_STRUCT_ELEM_WALL_OPEN_CREATE__:
	case __TMMODE_STRUCT_ELEM_DELETE__:   
	case __TMMODE_STRUCT_ELEM_TRANSLATE__:
	case __TMMODE_STRUCT_ELEM_MIRROR__:   
	case __TMMODE_STRUCT_ELEM_ROTATE__:   
	case __TMMODE_STRUCT_ELEM_EXTRUDE__:   
	case __TMMODE_STRUCT_ELEM_DIVIDE__:   
	case __TMMODE_STRUCT_ELEM_MERGE__:   
	case __TMMODE_STRUCT_ELEM_CHANGE__:
	case __TMMODE_STRUCT_ELEM_COMPACT__:
	case __TMMODE_STRUCT_ELEM_RENUMBER__: 
	case __TMMODE_STRUCT_ELEM_INTSECT__:
	case __TMMODE_STRUCT_ELEM_DEP_MAT_CHANGE__:
	case __TMMODE_STRUCT_TPSC_GROUP__:
	case __TMMODE_STRUCT_SECT_FOR_RFOC__:
	case __TMMODE_STRUCT_ELEM_ESSF__:
	case __TMMODE_STRUCT_ELEM_WSSF__:
	case __TMMODE_STRUCT_ELEM_ECCEN__:
	case __TMMODE_STRUCT_ELEM_PSSF__:

	case __TMMODE_STRUCT_BNDR__:
	case __TMMODE_STRUCT_BNDR_SKEW__:
	case __TMMODE_STRUCT_BNDR_CONS__:   
	case __TMMODE_STRUCT_BNDR_NSPR__:
	case __TMMODE_STRUCT_BNDR_ELNK__:
	case __TMMODE_STRUCT_BNDR_FRLS__:
	case __TMMODE_STRUCT_BNDR_OFFS__:
	case __TMMODE_STRUCT_BNDR_PRLS__:
	case __TMMODE_STRUCT_BNDR_RIGD__:
	case __TMMODE_STRUCT_BNDR_MCON__:
	case __TMMODE_STRUCT_BNDR_DRLS__:
	case __TMMODE_STRUCT_BNDR_IELC__:
	case __TMMODE_STRUCT_BNDR_GSPR__:
	case __TMMODE_STRUCT_BNDR_SSPR__:
	case __TMMODE_STRUCT_BNDR_SOSP__:
	case __TMMODE_STRUCT_BNGR_CHANGE__:
	case __TMMODE_STRUCT_BNDR_STORY__:
	case __TMMODE_STRUCT_BNDR_EWSF__:
	case __TMMODE_STRUCT_BNDR_CGLP__:
	case __TMMODE_STRUCT_BNDR_NLLINK__:
	case __TMMODE_STRUCT_BNDR_CLDR__:
	case __TMMODE_STRUCT_BNDR_LINK_COMPACT__:
	case __TMMODE_STRUCT_BNDR_LINK_RENUMBER__:
	case __TMMODE_STRUCT_BNDR_RELT__:

	case __TMMODE_STRUCT_STLD__:
	case __TMMODE_STRUCT_STLD_CNLD__:
	case __TMMODE_STRUCT_STLD_CNLD_TYPE__:
	case __TMMODE_STRUCT_STLD_SDSP__:
	case __TMMODE_STRUCT_STLD_BODF__:
	case __TMMODE_STRUCT_RESPOSNS_DISP__:
	case __TMMODE_STRUCT_STLD_NBOF__:
	case __TMMODE_STRUCT_STLD_BMLD__:
	case __TMMODE_STRUCT_STLD_LBLD__:
	case __TMMODE_STRUCT_STLD_FBLD__:
	case __TMMODE_STRUCT_STLD_TBLD__:
	case __TMMODE_STRUCT_STLD_PRST__:
	case __TMMODE_STRUCT_STLD_PRTS__:
	case __TMMODE_STRUCT_STLD_PRES__:
	case __TMMODE_STRUCT_STLD_HPRS__:
	case __TMMODE_STRUCT_STLD_STMP__:
	case __TMMODE_STRUCT_STLD_SSEI__: // 귀남 
	case __TMMODE_STRUCT_STLD_NTMP__:
	case __TMMODE_STRUCT_STLD_ETMP__:
	case __TMMODE_STRUCT_STLD_FIPA__:
	case __TMMODE_STRUCT_STLD_FIRE__:
	case __TMMODE_STRUCT_STLD_FSSF__:
	case __TMMODE_STRUCT_STLD_GTMP__:
	case __TMMODE_STRUCT_STLD_IFGS__:
	case __TMMODE_STRUCT_STLD_SWIB__:
	case __TMMODE_STRUCT_STLD_TENDON__:
	case __TMMODE_STRUCT_STLD_TENDON_PROP__:
	case __TMMODE_STRUCT_STLD_TENDON_PRST__:
	case __TMMODE_STRUCT_STLD_TLCS__: 
	case __TMMODE_STRUCT_STLD_CMCS__:
	case __TMMODE_STRUCT_STLD_CRPC__: 
	case __TMMODE_STRUCT_STLD_STBK__: 
	case __TMMODE_STRUCT_STLD_ULCB__:
	case __TMMODE_STRUCT_LDGR_CHANGE__:
	case __TMMODE_STRUCT_STLD_PNLD__:	
	case __TMMODE_STRUCT_STLD_BTMP__:	
	case __TMMODE_STRUCT_STLD_PTMP__:
	case __TMMODE_STRUCT_STLD_FMLD__:    //2 added by KYM
	case __TMMODE_STRUCT_MASS__:
	case __TMMODE_STRUCT_MASS_NMAS__:
	case __TMMODE_STRUCT_MASS_DMAS__:
	case __TMMODE_STRUCT_STLD_LAPL__:
	case __TMMODE_STRUCT_STLD_PRES_AREA__:
	case __TMMODE_STRUCT_STLD_PRES_QUERY__:
	case __TMMODE_STRUCT_STLD_EPSE__:
	case __TMMODE_STRUCT_STLD_EPST__:
		if (!IsSameTab(nCurMode))
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyStructTab();
			I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		}
		break;
	
	// Added by BugBoy for Settlement Tab
	// 2000.8.10
	case __TMMODE_STRUCT_STTL__:
	case __TMMODE_STRUCT_STTL_GROUP__:
	case __TMMODE_STRUCT_STTL_LOADCASE__:
		if (nCurMode < __TMMODE_STRUCT_STTL_START__ || nCurMode > __TMMODE_STRUCT_STTL_END__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndSettlementTab);
		}
		break;

	case __TMMODE_STRUCT_HYDR__:
	case __TMMODE_STRUCT_HYDR_TEMPER__:
	case __TMMODE_STRUCT_HYDR_HEAT__:
	case __TMMODE_STRUCT_HYDR_CONVECT__:
	case __TMMODE_STRUCT_HYDR_MATERIAL__:
		if (nCurMode < __TMMODE_STRUCT_HYDR_START__ || nCurMode > __TMMODE_STRUCT_HYDR_END__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndHydrationTab);
		}
		break;
	case __TMMODE_STRUCT_HYDR_TIME_DEP_MAT__:

		if (nCurMode < __TMMODE_STRUCT_HYDR_TIME_DEP_MAT_START__ || nCurMode > __TMMODE_STRUCT_HYDR_TIME_DEP_MAT_END__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndTimeDepTab);
		}
		break;
	case __TMMODE_STRUCT_MHTP_MAT_LINK_CH__:

		if (nCurMode < __TMMODE_STRUCT_MHTP_MAT_LINK_CH_START__ || nCurMode > __TMMODE_STRUCT_MHTP_MAT_LINK_CH_END__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndHighTemperatureTab);
		}
		break;
	case __TMMODE_CMD_THGA__:
	case __TMMODE_CMD_THNL__:
	case __TMMODE_CMD_THSP__:
	case __TMMODE_CMD_THMS__:
	case __TMMODE_CMD_THRD__:
	case __TMMODE_CMD_THRD_DISP__:
	case __TMMODE_CMD_THRD_TRUSS__:
	case __TMMODE_CMD_THRD_BEAM__:
	case __TMMODE_CMD_THRD_PLANE_STRESS__:
	case __TMMODE_CMD_THRD_PLANE_STRAIN__:
	case __TMMODE_CMD_THRD_PLATE__:
	case __TMMODE_CMD_THRD_SOLID__:
	case __TMMODE_CMD_THRD_GENERAL_LINK__:
	case __TMMODE_CMD_THRD_INELASTIC_HINGE__:
	case __TMMODE_CMD_THRC_TIME_INCREMENT__:
	case __TMMODE_CMD_THRD_STEP_TIMESTEP__:
	case __TMMODE_CMD_THRD_STEP_DISPVELACC__:
	case __TMMODE_CMD_THRD_STEP_TRUSS__:
	case __TMMODE_CMD_THRD_STEP_BEAM__:
	case __TMMODE_CMD_THRD_STEP_DESIGDOF__:
	case __TMMODE_CMD_THAT__:
	case __TMMODE_CMD_THCR__:
		if(nCurMode != __TMMODE_CMD_THGA__ &&
			 nCurMode != __TMMODE_CMD_THNL__ &&
			 nCurMode != __TMMODE_CMD_THSP__ &&
			 nCurMode != __TMMODE_CMD_THMS__ &&
			 nCurMode != __TMMODE_CMD_THRD__ &&
			 nCurMode != __TMMODE_CMD_THRD_DISP__ &&
			 nCurMode != __TMMODE_CMD_THRD_TRUSS__ &&
			 nCurMode != __TMMODE_CMD_THRD_BEAM__ &&
			 nCurMode != __TMMODE_CMD_THRD_PLANE_STRESS__ &&
			 nCurMode != __TMMODE_CMD_THRD_PLANE_STRAIN__ &&
			 nCurMode != __TMMODE_CMD_THRD_PLATE__ &&
			 nCurMode != __TMMODE_CMD_THRD_SOLID__ &&
			 nCurMode != __TMMODE_CMD_THRD_GENERAL_LINK__ &&
			 nCurMode != __TMMODE_CMD_THRD_INELASTIC_HINGE__ &&
			 nCurMode != __TMMODE_CMD_THRC_TIME_INCREMENT__ &&
			 nCurMode != __TMMODE_CMD_THRD_STEP_TIMESTEP__ &&
			 nCurMode != __TMMODE_CMD_THRD_STEP_DISPVELACC__ &&
			 nCurMode != __TMMODE_CMD_THRD_STEP_TRUSS__ &&
			 nCurMode != __TMMODE_CMD_THRD_STEP_BEAM__ &&
			 nCurMode != __TMMODE_CMD_THRD_STEP_DESIGDOF__ &&
			 nCurMode != __TMMODE_CMD_THCR__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndCmdTab);
		}
		break;
	case __TMMODE_CMD_MLSP__:
	case __TMMODE_CMD_MLSR__:
	case __TMMODE_CMD_MLTT__:
	case __TMMODE_CMD_ADD_IMPF__: 
	case __TMMODE_CMD_SINF__:
	case __TMMODE_CMD_DYNF__:
	case __TMMODE_CMD_JIMP__:
		if (nCurMode != __TMMODE_CMD_MLSP__ &&
			nCurMode != __TMMODE_CMD_MLSR__ &&
			nCurMode != __TMMODE_CMD_MLTT__ &&
			nCurMode != __TMMODE_CMD_ADD_IMPF__ &&
			nCurMode != __TMMODE_CMD_SINF__ &&
			nCurMode != __TMMODE_CMD_DYNF__ &&
			nCurMode != __TMMODE_CMD_JIMP__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTabWnd(&m_wndCmdTab);
		}
		break;
	case __TMMODE_CMD_SIHO_ELEM__	:
	case __TMMODE_CMD_SIHO_GLINK__	:
	case __TMMODE_CMD_SIHO_PSSPOT__:
		if (nCurMode != __TMMODE_CMD_SIHO_ELEM__ && nCurMode != __TMMODE_CMD_SIHO_GLINK__ && nCurMode != __TMMODE_CMD_SIHO_PSSPOT__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTabWnd(&m_wndCmdTab);
		}
		break;
	case __TMMODE_CMD_SPHO_ELEM__	:
	case __TMMODE_CMD_SPHO_GLINK__	:
		if (nCurMode != __TMMODE_CMD_SPHO_ELEM__ && nCurMode != __TMMODE_CMD_SPHO_GLINK__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTabWnd(&m_wndCmdTab);
		}
		break;
	case __TMMODE_CMD_SPEC__:
	case __TMMODE_CMD_NPLN__:
	case __TMMODE_CMD_BGEN__:
	case __TMMODE_CMD_POHA__:
	case __TMMODE_CMD_IEHA__:
	case __TMMODE_CMD_IEPI__:
	case __TMMODE_CMD_NMRD__:
	case __TMMODE_CMD_DYFG__:
	{
		_GetCurWnd()->ShowWindow(SW_HIDE);
		DestroyTabWnd(&m_wndCmdTab);
	}
	break;

	case __TMMODE_STRUCT_MESH_AUTO__  : 
	case __TMMODE_STRUCT_MESH_MAP_KE__:
	case __TMMODE_STRUCT_MESH_MAP__   :
	{
#ifdef _MGEN_CH
		//I think this should determine the condition, confirm and remove the comment;by xuezc 2023/4/14
		if (nCurMode < __TMMODE_STRUCT_MESH_START__ || nCurMode > __TMMODE_STRUCT_MESH_END__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTabWnd(&m_wndMeshTab);
		}
#else
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
		pIGM->ResetTempElem();
		_GetCurWnd()->ShowWindow(SW_HIDE);
		DestroyTabWnd(&m_wndMeshTab);
#endif
	}
		break;
	case __TMMODE_STRUCT_EBEAM_CREATE__:
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyTabWnd(&m_wndEBeamTab);
		I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		break;
	case __TMMODE_STRUCT_STRIP_ASSIGN__:
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyTabWnd(&m_wndStripTab);
		I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		break;
	case __TMMODE_STRUCT_DROPPANEL_ASSIGN__:
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyTabWnd(&m_wndDropPanelTab);
		I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		break;
	case __TMMODE_STRUCT_COLMCAP_ASSIGN__:
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyTabWnd(&m_wndColmCapTab);
		I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		break;
	case __TMMODE_MESHDGN_SLABSHEAR_REINFORCE__:
	case __TMMODE_MESHDGN_COLUMN_LOCATION__:
		_GetCurWnd()->ShowWindow(SW_HIDE);  
//     DestroyTabWnd(&m_wndColmLocTab);
//     I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		break;
	case __TMMODE_CMD_BSPN__:
	case __TMMODE_CMD_GIMP__:
	case __TMMODE_CMD_P1LA__:
	case __TMMODE_CMD_CREF__:
	case __TMMODE_CMD_GLTM__:
	case __TMMODE_CMD_LNKF__:
	case __TMMODE_CMD_GSTY__:
		if(nCurMode != __TMMODE_CMD_BSPN__ &&
			 nCurMode != __TMMODE_CMD_GIMP__ &&
			 nCurMode != __TMMODE_CMD_P1LA__ &&
			 nCurMode != __TMMODE_CMD_CREF__ &&
			 nCurMode != __TMMODE_CMD_GLTM__ &&
			 nCurMode != __TMMODE_CMD_LNKF__ &&
			 nCurMode != __TMMODE_CMD_GSTY__)
		{
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyTabWnd(&m_wndCmdTab);
		}
		break;

	// Seismic Tab
	case __TMMODE_STRUCT_SEIS_CLEAR_HGT_COL__: 
	case __TMMODE_STRUCT_SEIS_SEISMIC_HOOK__: 
	case __TMMODE_STRUCT_SEIS_COMP_TYPE__:
	case __TMMODE_STRUCT_SEIS_REINF_DETAILS__:
	case __TMMODE_STRUCT_SEIS_RC_COLUMN_M__:
	case __TMMODE_STRUCT_SEIS_PM_CURVE_METHOD__:
	case __TMMODE_STRUCT_SEIS_PBD_SEBE__:
	case __TMMODE_STRUCT_SEIS_PBD_SECO__:
	case __TMMODE_STRUCT_SEIS_PBD_SEWA__:
	case __TMMODE_STRUCT_SEIS_PBD_HLFW__:
	case __TMMODE_STRUCT_SEIS_PBD_SESP__:
		if ( nCurMode != __TMMODE_STRUCT_SEIS_CLEAR_HGT_COL__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_SEISMIC_HOOK__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_COMP_TYPE__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_REINF_DETAILS__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_RC_COLUMN_M__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_PM_CURVE_METHOD__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_PBD_SEBE__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_PBD_SECO__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_PBD_SEWA__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_PBD_HLFW__ &&
			nCurMode != __TMMODE_STRUCT_SEIS_PBD_SESP__ )
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTabWnd(&m_wndSeisCommonTab);
		}
		break;
	case __TMMODE_SEIS_1ST_RC_M_FACTOR__:
	case __TMMODE_SEIS_1ST_STL_M_FACTOR__:
	case __TMMODE_SEIS_1ST_MASONRY_M_FACTOR__:
		if(nCurMode != __TMMODE_SEIS_1ST_RC_M_FACTOR__ &&
			 nCurMode != __TMMODE_SEIS_1ST_STL_M_FACTOR__ &&
			 nCurMode != __TMMODE_SEIS_1ST_MASONRY_M_FACTOR__)
		{

			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndSeisMFactTab);
		}
		break;
	case __TMMODE_STRUCT_SEIS_XY_FACTOR__:
		if(nCurMode != __TMMODE_STRUCT_SEIS_XY_FACTOR__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTabWnd(&m_wndSeisXYFactTab);
		}
		break;
	//	General Tab
	case __TMMODE_DGN_GEN_MEMB__:
	case __TMMODE_DGN_GEN_MEMB_CHG__:
	case __TMMODE_DGN_GEN_LENG__:
	case __TMMODE_DGN_GEN_SPLN__:
	case __TMMODE_DGN_GEN_KFAC__:
	case __TMMODE_DGN_GEN_LTSR__:
	case __TMMODE_DGN_GEN_CMFT__:
	case __TMMODE_DGN_GEN_FMAG__:
	case __TMMODE_DGN_GEN_BCIF__:
	case __TMMODE_DGN_GEN_PMCR__:
	case __TMMODE_DGN_GEN_REDU__:
	case __TMMODE_DGN_GEN_SUEQ__:
	case __TMMODE_DGN_GEN_GREQ__:
	case __TMMODE_DGN_GEN_MBTP__:
	case __TMMODE_DGN_TWR_MBTP__:
	case __TMMODE_DGN_GEN_SEIS__:
	case __TMMODE_DGN_GEN_HCBM__:
	case __TMMODE_DGN_GEN_EWSD__:// Add by sshan. MNET:2729.('20070514)
	case __TMMODE_DGN_GEN_SEIS_DGN__:
	case __TMMODE_DGN_GEN_ULCT__:
	case __TMMODE_DGN_GEN_DSST__:
	case __TMMODE_DGN_GEN_ARST__:
		//	Steel Tab
	case __TMMODE_DGN_STL_SERV__: // Coded by Seungjun MNet:No.2393 ('20061011)
	case __TMMODE_DGN_STL_CBFT__:
	case __TMMODE_DGN_STL_CVFT__:
	case __TMMODE_DGN_STL_DALW__:
	case __TMMODE_DGN_STL_UCFA__:
	case __TMMODE_DGN_STL_STFN__:
	case __TMMODE_DGN_STL_SLRS__:
	case __TMMODE_DGN_STL_PHIB__://add by maxiao 2015-10-12
	case __TMMODE_DGN_STL_SETY__:	//Add by tss 2020/02/12
	case __TMMODE_DGN_STL_BMRO__:	//add by tss 2021/01/13
	case __TMMODE_DGN_STL_SPDF__:	//Add by tss 2024/02/29
	case __TMMODE_DGN_STL_IMEL__:	//Add by tss
	case __TMMODE_DGN_STL_INDF__:
	case __TMMODE_DGN_STL_DSAU__:
	case __TMMODE_DGN_STL_DSAS__:
    case __TMMODE_DGN_STL_SDSW__://add by maxiao 2015-10-22
	case __TMMODE_DGN_STL_PFMC__:
	case	__TMMODE_DGN_STL_PFCM__:
	case	__TMMODE_DGN_STL_PFDL__:
	case	__TMMODE_DGN_STL_PFMP__:
	case	__TMMODE_DGN_STL_PFNP__:
	case __TMMODE_DGN_STL_SMSP__:
	case __TMMODE_DGN_STL_JP_POSITION__:
	case __TMMODE_DGN_STL_STBD__:
	case __TMMODE_DGN_STL_STUS__:
	case __TMMODE_DGN_STL_STRE__:
	case __TMMODE_DGN_STL_SPSC__:
	case __TMMODE_DGN_STL_SPCS__:
	case __TMMODE_DGN_STL_SPLS__:
	case __TMMODE_DGN_STL_SPPB__:
	case __TMMODE_DGN_STL_SPLB__:
	case __TMMODE_DGN_STL_SP14SeisMembParams__:
	case __TMMODE_DGN_STL_SP16SafetyMembParams__:
	case __TMMODE_DGN_STL_SP16CombinedSectParams__:

	//ALU
	case __TMMODE_DGN_ALU_ARIB__:
	case __TMMODE_DGN_ALU_AWED__:
	case __TMMODE_DGN_ALU_AHTR__:
	case __TMMODE_DGN_ALU_APHI__:
	  //	RC Tab
	case __TMMODE_DGN_CON_REBB__:
	case __TMMODE_DGN_CON_REBC__:
	case __TMMODE_DGN_CON_REBR__:
	case __TMMODE_DGN_CON_REBW__:
	case __TMMODE_DGN_CON_WMAK__:
	case __TMMODE_DGN_CON_BEMW__:
	case __TMMODE_DGN_CON_EBMW__:
	case __TMMODE_DGN_CON_CVL_SUFC__:
	case __TMMODE_DGN_CON_CVL_RMFR__:
	case __TMMODE_DGN_CON_CVL_ULSP__:
	case __TMMODE_DGN_CON_CVL_RDGN_BEAM__:
	case __TMMODE_DGN_CON_CVL_RDGN_COLU__:
	case __TMMODE_DGN_CON_JSFT__: //Add By RSH 2002.12.26
	case __TMMODE_DGN_CON_PSAS__: //Add By RSH 2002.12.02
	case __TMMODE_DGN_CON_MRFT__: //Add By RSH 2003.07.04
	case __TMMODE_DGN_CON_MCMB__:
	case __TMMODE_DGN_CON_DFBA__:
	case __TMMODE_DGN_CON_SERV__: // Coded by Seungjun MNet:No.2393 ('20060828)
	case __TMMODE_DGN_CON_SPVAREMA__: // Coded by Seungjun MNet:No.2393 ('20060828)
	case __TMMODE_DGN_CON_SERV_SLAB__: 
	case __TMMODE_DGN_CON_REIN_SHEAR__:
	case __TMMODE_DGN_GEN_MEMB_PLATE__:
	case __TMMODE_DGN_CON_ELEM_BAR__: // Coded by sshan MNet:No.2484 ('20061109)
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM: 
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_COL:  
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC: 
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL: 
	case __TMMODE_DGN_CON_BEAM_BAR__:
	case __TMMODE_DGN_CON_DORC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_PORC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_CWRC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_LEKF__:// Coded by sshan MNet:No.2607 ('20061226)
	case __TMMODE_DGN_CON_RSDM__:// Coded by bjshin MNet:No.3992 ('20090517)
	case __TMMODE_DGN_CON_TRFT__:// Coded by sshan MNet:No.3780 ('20090320)
	case __TMMODE_DGN_CON_UCCF__:// Add by GAY. PMS:4267. ('11.12.27). 중국신기준-불확정성 분항계수.
	case __TMMODE_DGN_CON_REXC__:
	case __TMMODE_DGN_CON_PMDM__:
	case __TMMODE_DGN_CON_SCOL__:
	case __TMMODE_DGN_CON_OCDF__:
	case __TMMODE_DGN_CON_PBDC__:
	case __TMMODE_DGN_CON_PCDC__:
	case __TMMODE_DGN_CON_PBDD__:
	case __TMMODE_DGN_CON_PCDD__:
	case __TMMODE_DGN_CON_STAN__:
	case __TMMODE_DGN_CON_MNDC__:
	case __TMMODE_DGN_CON_SAFM__:
	case __TMMODE_DGN_CON_RRTR__:
	case __TMMODE_DGN_CON_EXPOSURE_CLASS__:
	  //	SRC Tab
	case __TMMODE_DGN_SRC_RBSB__:
	case __TMMODE_DGN_SRC_REBS__:
		//	Cold Formed Steel Tab
	case __TMMODE_DGN_CFS_SERV__:
	case __TMMODE_DGN_CFS_CBFT__:
	case __TMMODE_DGN_CFS_CSOS__:
	case __TMMODE_DGN_CFS_CSMS__:
	case __TMMODE_DGN_CFS_PLIN__:
	case __TMMODE_DGN_CFS_SPHI__:
	case __TMMODE_DGN_CFS_PFMC__:
	case __TMMODE_DGN_CFS_PFCM__:
	case __TMMODE_DGN_CFS_PFDL__:
	case __TMMODE_DGN_CFS_PFMP__:
	case __TMMODE_DGN_CFS_PFNP__:
	  // PSC Tab
	case __TMMODE_DGN_CRC_DGN_POSC__:
	case __TMMODE_DGN_CRC_DGN_POSX__:
	case __TMMODE_DGN_PSC_CRACK_PSCW__:
	case __TMMODE_DGN_PSC_SEGMENT__:
	case __TMMODE_DGN_PSC_EXPOSURE_CLASS__:
	case __TMMODE_DGN_PSC_OCDF__:
	case __TMMODE_DGN_PSC_SHEAR_CONNECTOR__:
	case __TMMODE_DGN_PSC_INTERFACE_SHEAR__:
	case __TMMODE_DGN_PSC_FATIGUE_CHECK__:
	  // CPG Tab
	case __TMMODE_DGN_CPG_STIF__:
	case __TMMODE_DGN_CPG_SHEAR__:
	case __TMMODE_DGN_CPG_POSITION__:
	case __TMMODE_DGN_CPG_POSI_DGN_OUT__:
	case __TMMODE_DGN_CPG_LOAD_APPLICATION__:
	case __TMMODE_DGN_CPG_TORSION__:
	case __TMMODE_DGN_CPG_DAMAGE__:
	case __TMMODE_DGN_CPG_SHEAR_CONN__://PMS:LRFD12 BSC-20131231
	case __TMMODE_DGN_CPG_SHEAR_CONN_CSA__:
	case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
	  //case __TMMODE_DGN_CPG_UNBRA_LENG__:
	case __TMMODE_DGN_CPG_CURVE_BRID__:
	case __TMMODE_DGN_CPG_DECK_OVERHANG__:
	case __TMMODE_DGN_CPG_SHEAR_CONN_GROUP__:
	case __TMMODE_DGN_CPG_SERV__:
	case __TMMODE_DGN_CPG_LENG__:
	case __TMMODE_DGN_CPG_WEB_PANEL__:
	case __TMMODE_DGN_CPG_RESIDUAL_STRESS__:
	  // Rating Tab
	case __TMMODE_DGN_RATING_OPTION__:
	case __TMMODE_DGN_RATING_PRINT__:
	case __TMMODE_DGN_RATING_PSC_ENVIRONMENT_CLASS__:
	case __TMMODE_DGN_PSC_ASSESSMENT_OPTION__:
	case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:  
	case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:
	case __TMMODE_DGN_STEEL_RATING_OPTION__:
	case __TMMODE_DGN_STEEL_RATING_PRINT__:   
	case __TMMODE_DGN_STEEL_RATING_WEB_PANEL__:   
	case __TMMODE_DGN_STEEL_RATING_TORSION__:
	case __TMMODE_DGN_STEEL_RATING_SHEAR_CONN__:
	case __TMMODE_DGN_STEEL_RATING_SERVICE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_EFFECT_LENGTH_U_FRAME__:
	case __TMMODE_DGN_STEEL_RATING_MOMENT_DIST_FACT_SLENDER__:
	case __TMMODE_DGN_STEEL_RATING_FASTENER_RIVET__:
	case __TMMODE_DGN_STEEL_RATING_ASSIGN_MEMB_FACTOR__:
	case __TMMODE_DGN_STEEL_RATING_ASSESS_CHECK__:
	case __TMMODE_DGN_STEEL_RATING_ASSESS_REPORT__:
	case __TMMODE_DGN_RC_RATING_OPTION_BEAM__:
	case __TMMODE_DGN_RC_RATING_OPTION_PLATE__:
	case __TMMODE_DGN_RC_RATING_DGN_OPTION_PLATE__:
	case __TMMODE_DGN_RC_RATING_PRINT_BEAM__:
	case __TMMODE_DGN_RC_RATING_PRINT_PLATE__:
	  // SOD Tab
	case __TMMODE_DGN_SOD_FLEXBUCK_INPUT__:
	case __TMMODE_DGN_SOD_POSI_DGN_OUT__:
	{
		if (!IsSameDgnTab(nCurMode))
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndDgnTab);
			I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);
		}
	}break;

/////////////////////////////////////////////////////////////////////////////
// GPS 모드의 경우 시작 by mjpark                     ///////////////////////
/////////////////////////////////////////////////////////////////////////////
	case __TMMODE_GPS_DISP_SRHDSP__:
	case __TMMODE_GPS_DISP_DEFORM__:
	case __TMMODE_GPS_DISP_DCONTR__:
	case __TMMODE_GPS_ISOLATION_DISP__:
	//case __TMMODE_GPS_DUTL_FACT__:
	case __TMMODE_GPS_FORC_TFORC__:
	case __TMMODE_GPS_FORC_BFORC__:
	case __TMMODE_GPS_FORC_BDGRM__:
	case __TMMODE_GPS_PTDN_FORCE__:
	case __TMMODE_GPS_FORC_WFORC__:
	case __TMMODE_GPS_FORC_WSFBM__:
	case __TMMODE_GPS_FORC_PFORC__:
	case __TMMODE_GPS_FORC_PCLPD__:
	case __TMMODE_GPS_FORC_ESFBM__:
	case __TMMODE_GPS_FORC_RFRCD__:
	case __TMMODE_GPS_STRESS_SECV__:
	case __TMMODE_GPS_STRS_TSTRS__:
	case __TMMODE_GPS_STRS_BSTRS__:
	case __TMMODE_GPS_STRS_BSTRSDETAIL__:
	case __TMMODE_GPS_STRS_BSTRSDETAILDGRM__:
	case __TMMODE_GPS_STRS_BSTRSDGRM__:
	case __TMMODE_GPS_STRS_BSTRSINC__:
	case __TMMODE_GPS_PTDN_STRESS__:
	case __TMMODE_GPS_STRS_PSTRS__:
	case __TMMODE_GPS_STRS_PNSTRS__:
	case __TMMODE_GPS_STRS_AXSTRS__:
	case __TMMODE_GPS_STRS_SSTRS__:
	case __TMMODE_GPS_REAC_SRHRCF__:
	case __TMMODE_GPS_REAC_TARGET__:
	case __TMMODE_GPS_REAC_REACF__:
	case __TMMODE_GPS_ISOLATION_FORCE__:
	case __TMMODE_GPS_SOIL_PRESS__:
	case __TMMODE_GPS_PLATE_STRAIN__:
	case __TMMODE_GPS_SOLID_STRAIN__:
	case __TMMODE_GPS_ISOLATION_STRS__:
		// 지금 Multi-Tab 모드이다. 바뀌게 될 모드가 또한 같은 Multi-Tab 모드이면

		// Tab을 Destroy하지 않고 현재 탭을 감추기만 한다.
		if (IsGPSMultiTab(nCurMode)==1 && ::IsWindow(m_wndGPSTab.GetSafeHwnd()))
			_GetCurWnd()->ShowWindow(SW_HIDE);
		// 지금 멀티탭 모드이고, 바뀔 모드는 멀티탭 모드가 아닌경우

		// 탭을 Destroy
		else
			DestroyGpsTab();
		break;

	case __TMMODE_GPS_INFL_REAC__:
	case __TMMODE_GPS_INFL_DISP__:
	case __TMMODE_GPS_INFL_TFORC__:
	case __TMMODE_GPS_INFL_BFORC__:
	case __TMMODE_GPS_INFL_ELNKFORC__:
	case __TMMODE_GPS_INFL_NLNKFORC__:
	case __TMMODE_GPS_INFL_PFORC__:
	case __TMMODE_GPS_INFL_BSTRS__:
	case __TMMODE_GPS_INFL_SSTRS__:
	case __TMMODE_GPS_MVLTRC_REAC__:
	case __TMMODE_GPS_MVLTRC_DISP__:
	case __TMMODE_GPS_MVLTRC_TFORC__:
	case __TMMODE_GPS_MVLTRC_BFORC__:
	case __TMMODE_GPS_MVLTRC_ELNKFORC__:
	case __TMMODE_GPS_MVLTRC_NLNKFORC__:
	case __TMMODE_GPS_MVLTRC_PFORC__:
	case __TMMODE_GPS_MVLTRC_BSTRS__:
	case __TMMODE_GPS_INFS_REAC__:
	case __TMMODE_GPS_INFS_DISP__:
	case __TMMODE_GPS_INFS_TFORC__:
	case __TMMODE_GPS_INFS_BFORC__:
	case __TMMODE_GPS_INFS_ELNKFORC__:
	case __TMMODE_GPS_INFS_NLNKFORC__:
	case __TMMODE_GPS_INFS_PFORC__:
	case __TMMODE_GPS_INFS_BSTRS__:
	case __TMMODE_GPS_INFS_SSTRS__:
	case __TMMODE_GPS_MVLTRC_BATCH__:
		{
			// 지금 Multi-Tab 모드이다. 바뀌게 될 모드가 또한 같은 Multi-Tab 모드이면
			// Tab을 Destroy하지 않고 현재 탭을 감추기만 한다.
		  if (IsGPSMultiTab(nCurMode)==2 && ::IsWindow(m_wndGPSTab.GetSafeHwnd()))
			  _GetCurWnd()->ShowWindow(SW_HIDE);
		  // 지금 멀티탭 모드이고, 바뀔 모드는 멀티탭 모드가 아닌경우
		  // 탭을 Destroy
		  else
			  DestroyGpsTab();
		}
		break;

	case __TMMODE_GPS_TIMEHISTORY_GRAPH__:
	case __TMMODE_GPS_TIMEHISTORY_TEXT__:
	case __TMMODE_GPS_TIMEHISTORY_SSGP__:
	  // 지금 Multi-Tab 모드이다. 바뀌게 될 모드가 또한 같은 Multi-Tab 모드이면
		// Tab을 Destroy하지 않고 현재 탭을 감추기만 한다.
		if(IsGPSMultiTab(nCurMode)==3 && ::IsWindow(m_wndGPSTab.GetSafeHwnd()))
			_GetCurWnd()->ShowWindow(SW_HIDE);
		// 지금 멀티탭 모드이고, 바뀔 모드는 멀티탭 모드가 아닌경우
		// 탭을 Destroy
		else
			DestroyGpsTab();
		break;

	case __TMMODE_GPS_PUSHOVER_GRAPH__:
	case __TMMODE_GPS_PUSHOVER_TEXT__:
	case __TMMODE_GPS_PUSHOVER_SSGP__:
	  // 지금 Multi-Tab 모드이다. 바뀌게 될 모드가 또한 같은 Multi-Tab 모드이면
		// Tab을 Destroy하지 않고 현재 탭을 감추기만 한다.
		if(IsGPSMultiTab(nCurMode)==4 && ::IsWindow(m_wndGPSTab.GetSafeHwnd()))
			_GetCurWnd()->ShowWindow(SW_HIDE);
		// 지금 멀티탭 모드이고, 바뀔 모드는 멀티탭 모드가 아닌경우
		// 탭을 Destroy
		else
			DestroyGpsTab();
		break;

	case __TMMODE_GPS_GRIDTRC_REAC__:
	case __TMMODE_GPS_GRIDTRC_DISP__:
	case __TMMODE_GPS_GRIDTRC_TFORC__:
	case __TMMODE_GPS_GRIDTRC_BFORC__:
	case __TMMODE_GPS_GRIDTRC_ELNK__:
	case __TMMODE_GPS_GRIDTRC_NLNK__:
	case __TMMODE_GPS_INFG_REAC__:
	case __TMMODE_GPS_INFG_DISP__:
	case __TMMODE_GPS_INFG_TFORC__:
	case __TMMODE_GPS_INFG_BFORC__:
	case __TMMODE_GPS_INFG_BSTRS__:
	case __TMMODE_GPS_INFG_ELNK__:
	case __TMMODE_GPS_INFG_NLNK__:
		// 지금 Multi-Tab 모드이다. 바뀌게 될 모드가 또한 같은 Multi-Tab 모드이면
		// Tab을 Destroy하지 않고 현재 탭을 감추기만 한다.
		if(IsGPSMultiTab(nCurMode)==5 && ::IsWindow(m_wndGPSTab.GetSafeHwnd()))
			_GetCurWnd()->ShowWindow(SW_HIDE);
		// 지금 멀티탭 모드이고, 바뀔 모드는 멀티탭 모드가 아닌경우
		// 탭을 Destroy
		else
			DestroyGpsTab();
		break;

	case __TMMODE_GPS_DYNA_EGMDSHP__:
	case __TMMODE_GPS_DYNA_BKMDSHP__:
	case __TMMODE_GPS_DSGN_STEEL__:
	case __TMMODE_GPS_DSGN_STEEL_LSD__:
	case __TMMODE_SEIS_CVL_JP_RESULT_3D_INVESTIGATE__:
	case __TMMODE_GPS_DSGN_CFSTL__:
	case __TMMODE_GPS_DSGN_ALU__:
	case __TMMODE_GPS_DSGN_FIRE__:
	case __TMMODE_GPS_DSGN_PFDN__:
	case __TMMODE_GPS_DSGN_PFDNCFS__:
	case __TMMODE_GPS_DSGN_CNCRT__:
	case __TMMODE_GPS_DSGN_SRC__:
	case __TMMODE_GPS_FORC_SFDBMD__:
	case __TMMODE_GPS_FORC_SFDBTF__:
	case __TMMODE_GPS_GIRDERSTRS_DGRM__:
	case __TMMODE_GPS_STAGESTEP_GRAPH__:
	case __TMMODE_GPS_HYDRAHEAT_SSTRS__	:
	case __TMMODE_GPS_HYDRAHEAT_TEMPER__:
	case __TMMODE_GPS_HYDRAHEAT_DISPL__	 :
	case __TMMODE_GPS_HYDRAHEAT_ALTENSSTRS__:
	case __TMMODE_GPS_HYDRAHEAT_CRACKRATIO__:
	case __TMMODE_GPS_HYDRAHEAT_GRAPH__:
	case __TMMODE_GPS_COLUMNSHORT_GRP__: //KYE-HONG-20020327 
	case __TMMODE_GPS_COLUMNSHORT_TXT__:
	case __TMMODE_GPS_DISPPART_FACT:
	case __TMMODE_GPS_LOCAL_AXIS:
	case __TMMODE_GPS_SSFR__:
	case __TMMODE_GPS_DUTL_FACT__:
	case __TMMODE_GPS_THIS_DISP__:
	case __TMMODE_GPS_THIS_BFORC__:
	case __TMMODE_GPS_THIS_BDGRM__:
	case __TMMODE_GPS_THIS_BSTRS__:
	case __TMMODE_GPS_PSOV_SSDF__:
	case __TMMODE_GPS_PSOV_SSDR__:
	case __TMMODE_GPS_PSOV_MMDS__:
//   case __TMMODE_GPS_INFG_REAC__:
// 	case __TMMODE_GPS_INFG_DISP__:
// 	case __TMMODE_GPS_INFG_TFORC__:
// 	case __TMMODE_GPS_INFG_BFORC__:
// 	case __TMMODE_GPS_INFG_BSTRS__:
//   case __TMMODE_GPS_INFG_ELNK__:
//   case __TMMODE_GPS_INFG_NLNK__:
	case __TMMODE_GPS_DRAW_EDGRM__:
	case __TMMODE_GPS_DEFN_EDGRM__:
	case __TMMODE_GPS_PSC_DSGN_DGRM__:
	case __TMMODE_GPS_BRDG_LOAD_DGRM__:
	case __TMMODE_GPS_STEEL_RATING_DGRM__:
	case __TMMODE_GPS_STEEL_RATING_AASHTO_DGRM__:
	case __TMMODE_GPS_PSC_RATING_DGRM__:
	case __TMMODE_GPS_RC_RATING_DGRM__:
	case __TMMODE_GPS_SOD_DGRM__:
	case __TMMODE_GPS_NRRS__:  
	case __TMMODE_GPS_BCCR__:
	case __TMMODE_GPS_RC_DSGN_DGRM__:
	case __TMMODE_GPS_RAIL_PSC_DSGN_DGRM__:
	case __TMMODE_GPS_RAIL_RC_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_COMP_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_COMP_DSGN_AS5100_DGRM__:
	case __TMMODE_GPS_PSOV_DUTL_FACT__:
	case __TMMODE_GPS_PSOV_FUNCTION__:
	case __TMMODE_GPS_DGN_SLAB_RESULT__:
	case __TMMODE_GPS_DGN_SLAB_CHECK__:
	case __TMMODE_GPS_DGN_SHEAR_CHECK__:
	case __TMMODE_GPS_DGN_SLABSERV_CHECK__:
	case __TMMODE_GPS_DGN_WALL_RESULT__:
	case __TMMODE_GPS_DGN_WALL_CHECK__:
	case __TMMODE_GPS_DGN_STRIP_RESULT__:
	case __TMMODE_GPS_DGN_STRIP_CHECK__:
	case __TMMODE_GPS_CPG_RUS_SHEAR_COON_FORCE_DGRM__:
	case __TMMODE_GPS_CPG_RUS_RESULT_DGRM__:
	case __TMMODE_GPS_DGN_SHELL_RESULT__:
	case __TMMODE_GPS_DGN_SHELL_CHKECK__:
	case __TMMODE_GPS_DGN_SHELL_SHEAR_CHKECK__:
	case __TMMODE_GPS_DGN_BATCH_REPORT__:
	case __TMMODE_GPS_SEIS_1ST_RC_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_RC_PERFORM__:
	case __TMMODE_GPS_SEIS_1ST_STL_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_STL_PERFORM__:
	case __TMMODE_GPS_SEIS_1ST_MASONRY_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_MASONRY_PERFORM__:
	case __TMMODE_GPS_THIS_ENRG_GRAPH__:
	case __TMMODE_GPS_THIS_ENRG_GROUP_GRAPH__:
	case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_PO__:
	case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_TH__:
	case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_STORY_GRAPH_PFRM_TH__:
	case __TMMODE_GPS_DGN_DAMP_STEEL__:
		// 지금 멀티탭이 아니면 무조건 Destroy
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyGpsTab();
		break;
	case __TMMODE_CMD_WIND_PRESSURE_AREA__:
	case __TMMODE_CMD_WIND_PRESSURE_BEAM__:
	case __TMMODE_CMD_WIND_PRESSURE_NODAL__:
	case __TMMODE_CMD_WIND_PRESSURE_FUNC__:
		_GetCurWnd()->ShowWindow(SW_HIDE);  
		DestroyTabWnd(&m_wndWindPTab);
		break;

/////////////////////////////////////////////////////////////////////////////
// GPS 모드의 경우 끝 by mjpark                       ///////////////////////
/////////////////////////////////////////////////////////////////////////////
		case __TMMODE_SEIS_INFILL_WALL_CREATE__:
		case __TMMODE_SEIS_INFILL_WALL_REDUCE__:
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndInfillTab);
		}
		break;
	case __TMMODE_SEIS_EFFECT_STIFF__:
	case __TMMODE_SEIS_GENERATE_EFFECT_STIFF__:
		if (nCurMode != __TMMODE_SEIS_GENERATE_EFFECT_STIFF__ &&
				nCurMode != __TMMODE_SEIS_EFFECT_STIFF__)
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroySeisTab();
		}
		break;
	case __TMMODE_JUD_SLAB__:   
		{
			I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
			pIGM->ResetTempElem();
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndJUDTab);
		}
		break;
	case __TMMODE_SEIS_CVL_EVAL_GROUP__:
	case __TMMODE_SEIS_CVL_SECT_EFFECT_STIFF__:
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndSeisCvlTab);
		}
		break;
	case __TMMODE_SEIS_CVL_RESULT_EVAL_PARAM__:
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);  
			DestroyTabWnd(&m_wndSeisCvlMiscTab);
		}
		break;

	// seismic investigation Civil JP
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_UPPER__:
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_LOWER__:
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_USER__:
	{
		_GetCurWnd()->ShowWindow(SW_HIDE);
		DestroyTabWnd(&m_wndSeisSetTargetTab);
	}
	break;
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIMD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIRD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIBD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIDP__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SISR__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIET__:
	//case __TMMODE_SEIS_CVL_JP_RESULT_3D_INVESTIGATE__:
	{
		_GetCurWnd()->ShowWindow(SW_HIDE);
		DestroyTabWnd(&m_wndSeisSetInvestTab);
	}
	break;

	// Post Tension
	case __TMMODE_PT_DRAW_TENDON_LINE_BEAM__:
	case __TMMODE_PT_DRAW_TENDON_LINE_SLAB__:
	case __TMMODE_PT_DRAW_TRANS_TENDON__:
	case __TMMODE_PT_DRAW_MOD_TENDON__:
	case __TMMODE_PT_DRAW_DEL_TENDON__:
	case __TMMODE_PT_DRAW_RENUM_TENDON__:
	case __TMMODE_PT_TENDON_MATCH_PROFILE__:
	case __TMMODE_PT_SUPPORT_LINE_CREATE__:
	case __TMMODE_PT_DESIGN_STRIP_CREATE__:
	case __TMMODE_PT_DESIGN_STRIP_AUTO__:
	case __TMMODE_PT_DESIGN_STRIP_MODIFY_SECTION__:
		{
			_GetCurWnd()->ShowWindow(SW_HIDE);
			DestroyTendonTab();
		}
		break;
	default:
		break;
	}

	CurString = L"";
	CWnd * pWnd = NULL;
	switch(nCurMode)
	{
	case __TMMODE_TREEMENU__:
	case __TMMODE_TREEMENU_MENU__:
	case __TMMODE_TREEMENU_TABLE__:
	case __TMMODE_TREEMENU_GROUP__:
	case __TMMODE_TREEMENU_WORK__:
	case __TMMODE_TREEMENU_REPORT__:
	case __TMMODE_TREEMENU_JUD__:   
	case __TMMODE_TREEMENU_SEIS__:
		_SetCurWnd(&m_wndTab);
		_ShowTreeMenuTabByID(nCurMode);
		break;
	case __TMMODE_STRUCT_NODE__:
	case __TMMODE_STRUCT_NODE_COMPACT__:
	case __TMMODE_STRUCT_NODE_CREATE__:
	case __TMMODE_STRUCT_NODE_DELETE__:
	case __TMMODE_STRUCT_NODE_TRANSLATE__:
	case __TMMODE_STRUCT_NODE_MIRROR__:
	case __TMMODE_STRUCT_NODE_ROTATE__:
	case __TMMODE_STRUCT_NODE_PROJECT__:
	case __TMMODE_STRUCT_NODE_DIVIDE__:
	case __TMMODE_STRUCT_NODE_MERGE__:
	case __TMMODE_STRUCT_NODE_SCALE__:
	case __TMMODE_STRUCT_NODE_INTSECT__:
	case __TMMODE_STRUCT_NODE_RENUMBER__:  //2000.6.7 최수춘
	case __TMMODE_STRUCT_BNDR_SKEW__:

		{
		CreateStructNode();
		_SetCurWnd(&m_wndNodeElemTab);
			/*!@#$*/if(m_wndNodeElemTab.GetActiveTab(pWnd))
			{
				CStructNodeDlg* pNodeDlgView = (CStructNodeDlg*)pWnd;
				pNodeDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pNodeDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pNodeDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_ELEM__:
	case __TMMODE_STRUCT_ELEM_CREATE__:   
	case __TMMODE_STRUCT_ELEM_CURVE__:
	case __TMMODE_STRUCT_ELEM_CONV_LINE_CREATE__:  
	case __TMMODE_STRUCT_ELEM_WALL_OPEN_CREATE__:
	case __TMMODE_STRUCT_ELEM_DELETE__:   
	case __TMMODE_STRUCT_ELEM_TRANSLATE__:
	case __TMMODE_STRUCT_ELEM_MIRROR__:   
	case __TMMODE_STRUCT_ELEM_ROTATE__:   
	case __TMMODE_STRUCT_ELEM_EXTRUDE__:   
	case __TMMODE_STRUCT_ELEM_DIVIDE__: 
	case __TMMODE_STRUCT_ELEM_MERGE__:   
	case __TMMODE_STRUCT_ELEM_CHANGE__:
	case __TMMODE_STRUCT_ELEM_COMPACT__:
	case __TMMODE_STRUCT_ELEM_RENUMBER__:  
	case __TMMODE_STRUCT_ELEM_INTSECT__:
	case __TMMODE_STRUCT_ELEM_DEP_MAT_CHANGE__:
	case __TMMODE_STRUCT_TPSC_GROUP__:
	case __TMMODE_STRUCT_SECT_FOR_RFOC__:
	case __TMMODE_STRUCT_ELEM_ESSF__:
	case __TMMODE_STRUCT_ELEM_WSSF__:
	case __TMMODE_STRUCT_ELEM_ECCEN__:
	case __TMMODE_STRUCT_ELEM_PSSF__:
	case __TMMODE_STRUCT_BNDR_EWSF__:
		{
			CreateStructElem();
			_SetCurWnd(&m_wndNodeElemTab);
			/*!@#$*/if(m_wndNodeElemTab.GetActiveTab(pWnd))
			{
				CStructElementDlg* pElemDlgView = (CStructElementDlg*)pWnd;
				pElemDlgView->ShowChildDlgByID(nCurMode);
				if(nCurMode == __TMMODE_STRUCT_TPSC_GROUP__)
				{
					pElemDlgView->ShowExecuteButton(FALSE);
				}
				else if(nCurMode == __TMMODE_STRUCT_SECT_FOR_RFOC__)
				{
					pElemDlgView->ShowExecuteButton(FALSE);
				}
				else if(nCurMode == __TMMODE_STRUCT_ELEM_EXTRUDE__ || nCurMode == __TMMODE_STRUCT_ELEM_RENUMBER__)
				{
				}
				else
				{
					pElemDlgView->ShowExecuteButton(TRUE);
				}

				int nSel = pElemDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pElemDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_BNDR__:
	case __TMMODE_STRUCT_BNDR_CONS__:
	case __TMMODE_STRUCT_BNDR_NSPR__:
	case __TMMODE_STRUCT_BNDR_ELNK__:
	case __TMMODE_STRUCT_BNDR_FRLS__:
	case __TMMODE_STRUCT_BNDR_OFFS__:
	case __TMMODE_STRUCT_BNDR_PRLS__:
	case __TMMODE_STRUCT_BNDR_RIGD__:
	case __TMMODE_STRUCT_BNDR_MCON__:
	case __TMMODE_STRUCT_BNDR_DRLS__:
	case __TMMODE_STRUCT_BNDR_GSPR__:
	case __TMMODE_STRUCT_BNDR_SSPR__:
	case __TMMODE_STRUCT_BNDR_SOSP__:
	case __TMMODE_STRUCT_BNGR_CHANGE__:
	case __TMMODE_STRUCT_BNDR_STORY__:
	case __TMMODE_STRUCT_BNDR_CGLP__:
	case __TMMODE_STRUCT_BNDR_NLLINK__:
	case __TMMODE_STRUCT_BNDR_CLDR__:
	case __TMMODE_STRUCT_BNDR_LINK_COMPACT__:
	case __TMMODE_STRUCT_BNDR_LINK_RENUMBER__:
	case __TMMODE_STRUCT_BNDR_RELT__:
		{
		CreateStructBndr();
		_SetCurWnd(&m_wndNodeElemTab);
			/*!@#$*/ if(m_wndNodeElemTab.GetActiveTab(pWnd))
			{
				CStructBndrDlg* pBndrDlgView = (CStructBndrDlg*)pWnd;
				pBndrDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pBndrDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pBndrDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_STLD__:
	case __TMMODE_STRUCT_STLD_CNLD__:
	case __TMMODE_STRUCT_STLD_CNLD_TYPE__:
	case __TMMODE_STRUCT_STLD_SDSP__:
	case __TMMODE_STRUCT_STLD_BODF__:
	case __TMMODE_STRUCT_RESPOSNS_DISP__:
	case __TMMODE_STRUCT_STLD_NBOF__:
	case __TMMODE_STRUCT_STLD_BMLD__:
	case __TMMODE_STRUCT_STLD_LBLD__:
	case __TMMODE_STRUCT_STLD_FBLD__:
	case __TMMODE_STRUCT_STLD_TBLD__:
	case __TMMODE_STRUCT_STLD_PRST__:
	case __TMMODE_STRUCT_STLD_PRTS__:
	case __TMMODE_STRUCT_STLD_PRES__:
	case __TMMODE_STRUCT_STLD_HPRS__:
	case __TMMODE_STRUCT_STLD_STMP__:
	case __TMMODE_STRUCT_STLD_SSEI__:
	case __TMMODE_STRUCT_STLD_NTMP__:
	case __TMMODE_STRUCT_STLD_ETMP__:
	case __TMMODE_STRUCT_STLD_GTMP__:
	case __TMMODE_STRUCT_STLD_IFGS__:
	case __TMMODE_STRUCT_STLD_SWIB__:
	case __TMMODE_STRUCT_STLD_TENDON__:
	case __TMMODE_STRUCT_STLD_TENDON_PROP__:
	case __TMMODE_STRUCT_STLD_TENDON_PRST__:
	case __TMMODE_STRUCT_STLD_TLCS__:
	case __TMMODE_STRUCT_STLD_CMCS__:
	case __TMMODE_STRUCT_STLD_CRPC__:
	case __TMMODE_STRUCT_STLD_STBK__:
	case __TMMODE_STRUCT_STLD_ULCB__:
	case __TMMODE_STRUCT_LDGR_CHANGE__:
	case __TMMODE_STRUCT_STLD_PNLD__:
	case __TMMODE_STRUCT_STLD_BTMP__:
	case __TMMODE_STRUCT_STLD_PTMP__:
	case __TMMODE_STRUCT_STLD_FMLD__:    //2 added by KYM 
	case __TMMODE_STRUCT_STLD_LAPL__:
	case __TMMODE_STRUCT_STLD_PRES_AREA__:
	case __TMMODE_STRUCT_STLD_FIPA__:
	case __TMMODE_STRUCT_STLD_FIRE__:
	case __TMMODE_STRUCT_STLD_FSSF__:
	case __TMMODE_STRUCT_STLD_EPSE__:
	case __TMMODE_STRUCT_STLD_EPST__:
	case __TMMODE_STRUCT_STLD_PRES_QUERY__:
	case __TMMODE_STRUCT_BNDR_IELC__:
		{
		  CreateStructStld();
			_SetCurWnd(&m_wndNodeElemTab);
			/*!@#$*/if(m_wndNodeElemTab.GetActiveTab(pWnd))
			{
				CStructStldDlg* pStldDlgView = (CStructStldDlg*)pWnd;
				pStldDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pStldDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pStldDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_MASS__:
	case __TMMODE_STRUCT_MASS_NMAS__:
	case __TMMODE_STRUCT_MASS_DMAS__:
		{
		CreateStructMass();
		_SetCurWnd(&m_wndNodeElemTab);
			/*!@#$*/if(m_wndNodeElemTab.GetActiveTab(pWnd))
			{
			CStructMassDlg* pMassDlgView = (CStructMassDlg*)pWnd;
			pMassDlgView->ShowChildDlgByID(nCurMode);

			int nSel = pMassDlgView->m_ComboMenu.GetCurSel();
			CString strSelected;
			pMassDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
			CurString = strSelected;
			}
		}
		break;

	// Added by BugBoy for Settlement Tab
	// 2000.8.10
	case __TMMODE_STRUCT_STTL__:
	case __TMMODE_STRUCT_STTL_GROUP__:
	case __TMMODE_STRUCT_STTL_LOADCASE__:
		{
		CreateStructSttlTab();
		_SetCurWnd(&m_wndSettlementTab);
		/*!@#$*/if(m_wndSettlementTab.GetActiveTab(pWnd))
			{
			CStructSttlDlg* pSttlDlgView = (CStructSttlDlg*)pWnd;
			pSttlDlgView->ShowChildDlgByID(nCurMode);

			int nSel = pSttlDlgView->m_ComboMenu.GetCurSel();
			CString strSelected;
			pSttlDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
			CurString = strSelected;
			}
		}
		break;

	case __TMMODE_STRUCT_HYDR__:
	case __TMMODE_STRUCT_HYDR_TEMPER__:
	case __TMMODE_STRUCT_HYDR_HEAT__:
	case __TMMODE_STRUCT_HYDR_CONVECT__:
	case __TMMODE_STRUCT_HYDR_MATERIAL__:
	
		{
		CreateStructHydrTab();
		_SetCurWnd(&m_wndHydrationTab);
		/*!@#$*/if(m_wndHydrationTab.GetActiveTab(pWnd))
			{
			CStructHydrDlg* pHydrDlgView = (CStructHydrDlg*)pWnd;
			pHydrDlgView->ShowChildDlgByID(nCurMode);

			int nSel = pHydrDlgView->m_ComboMenu.GetCurSel();
			CString strSelected;
			pHydrDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
			CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_HYDR_TIME_DEP_MAT__:
		{
		CreateStructTimeDepTab();
		_SetCurWnd(&m_wndTimeDepTab);
		/*!@#$*/if(m_wndTimeDepTab.GetActiveTab(pWnd))
			{
			CStructTimeDepMatLinkDlg* pTDepDlgView = (CStructTimeDepMatLinkDlg*)pWnd;
			pTDepDlgView->ShowChildDlgByID(nCurMode);
			}
		}
		break;
	case __TMMODE_STRUCT_MHTP_MAT_LINK_CH__:
		{
			CreateStructTHighTemperatureTab();
			_SetCurWnd(&m_wndHighTemperatureTab);
			if(m_wndHighTemperatureTab.GetActiveTab(pWnd))
			{
				CStructHighTemperatureMatLinkDlg* pMhtpDlgView = (CStructHighTemperatureMatLinkDlg*)pWnd;
				pMhtpDlgView->ShowChildDlgByID(nCurMode);
			}
		}
		break;
	case __TMMODE_CMD_THGA__:
	case __TMMODE_CMD_THNL__:
	case __TMMODE_CMD_THSP__:
	case __TMMODE_CMD_THMS__:
	case __TMMODE_CMD_THRD__:
	case __TMMODE_CMD_THRD_DISP__:
	case __TMMODE_CMD_THRD_TRUSS__:
	case __TMMODE_CMD_THRD_BEAM__:
	case __TMMODE_CMD_THRD_PLANE_STRESS__:
	case __TMMODE_CMD_THRD_PLANE_STRAIN__:
	case __TMMODE_CMD_THRD_PLATE__:
	case __TMMODE_CMD_THRD_SOLID__:
	case __TMMODE_CMD_THRD_GENERAL_LINK__:
	case __TMMODE_CMD_THRD_INELASTIC_HINGE__:
	case __TMMODE_CMD_THRC_TIME_INCREMENT__:
	case __TMMODE_CMD_THRD_STEP_TIMESTEP__:
	case __TMMODE_CMD_THRD_STEP_DISPVELACC__:
	case __TMMODE_CMD_THRD_STEP_TRUSS__:
	case __TMMODE_CMD_THRD_STEP_BEAM__:
	case __TMMODE_CMD_THRD_STEP_DESIGDOF__:
	case __TMMODE_CMD_THAT__:
	case __TMMODE_CMD_THCR__:
		CMD_CreateBarTreeDlg(this, &m_wndCmdTab, nCurMode, CurString);
		_SetCurWnd(&m_wndCmdTab);
		break;
	case __TMMODE_CMD_SPEC__:
	case __TMMODE_CMD_NPLN__:
	case __TMMODE_CMD_MLSP__:
	case __TMMODE_CMD_MLSR__:
	case __TMMODE_CMD_MLTT__:
	case __TMMODE_CMD_JIMP__:
	case __TMMODE_CMD_SINF__:
	case __TMMODE_CMD_BGEN__:
	case __TMMODE_CMD_POHA__:
	case __TMMODE_CMD_IEHA__:
	case __TMMODE_CMD_IEPI__:
	case __TMMODE_CMD_BSPN__:
	case __TMMODE_CMD_GIMP__:
	case __TMMODE_CMD_P1LA__:
	case __TMMODE_CMD_CREF__:
	case __TMMODE_CMD_GLTM__:
	case __TMMODE_CMD_NMRD__:
	case __TMMODE_CMD_LNKF__:
	case __TMMODE_CMD_GSTY__:
	case __TMMODE_CMD_ADD_IMPF__:
	case __TMMODE_CMD_DYFG__:
	case __TMMODE_CMD_DYNF__:
	case __TMMODE_CMD_SIHO_ELEM__	:
	case __TMMODE_CMD_SIHO_GLINK__	:
	case __TMMODE_CMD_SIHO_PSSPOT__ :
	case __TMMODE_CMD_SPHO_ELEM__	:
	case __TMMODE_CMD_SPHO_GLINK__	:
		CMD_CreateBarTreeDlg(this, &m_wndCmdTab, nCurMode, CurString);
		_SetCurWnd(&m_wndCmdTab);
		break;
	case __TMMODE_CMD_WIND_PRESSURE_AREA__:
	case __TMMODE_CMD_WIND_PRESSURE_BEAM__:
	case __TMMODE_CMD_WIND_PRESSURE_NODAL__:
	case __TMMODE_CMD_WIND_PRESSURE_FUNC__:
		CreateWindPressure(this, &m_wndWindPTab, nCurMode);
		_SetCurWnd(&m_wndWindPTab);
		break;

	//	General Tab
	case __TMMODE_DGN_GEN_MEMB__:
	case __TMMODE_DGN_GEN_MEMB_CHG__:
	case __TMMODE_DGN_GEN_LENG__:
	case __TMMODE_DGN_GEN_SPLN__:
	case __TMMODE_DGN_GEN_KFAC__:
	case __TMMODE_DGN_GEN_LTSR__:
	case __TMMODE_DGN_GEN_CMFT__:
	case __TMMODE_DGN_GEN_FMAG__:
	case __TMMODE_DGN_GEN_BCIF__:
	case __TMMODE_DGN_GEN_PMCR__:
	case __TMMODE_DGN_GEN_REDU__:
	case __TMMODE_DGN_GEN_SUEQ__:
	case __TMMODE_DGN_GEN_GREQ__:
	case __TMMODE_DGN_GEN_MBTP__:
	case __TMMODE_DGN_TWR_MBTP__:
	case __TMMODE_DGN_GEN_SEIS__:
	case __TMMODE_DGN_GEN_HCBM__:
	case __TMMODE_DGN_GEN_EWSD__:// Add by sshan. MNET:2729.('20070514)
	case __TMMODE_DGN_GEN_SEIS_DGN__:
	case __TMMODE_DGN_GEN_ULCT__:
	case __TMMODE_DGN_GEN_DSST__:
	case __TMMODE_DGN_GEN_ARST__:
		//	Steel Tab
	case __TMMODE_DGN_STL_SERV__: // Coded by Seungjun MNet:No.2393 ('20061011)
	case __TMMODE_DGN_STL_CBFT__:
	case __TMMODE_DGN_STL_CVFT__:
	case __TMMODE_DGN_STL_DALW__:
	case __TMMODE_DGN_STL_UCFA__:
	case __TMMODE_DGN_STL_STFN__:
	case __TMMODE_DGN_STL_SLRS__:
	case __TMMODE_DGN_STL_PHIB__://add by maxiao 2015-10-12
	case __TMMODE_DGN_STL_SETY__:	//Add by tss 2020/02/12
	case __TMMODE_DGN_STL_BMRO__:	//add by tss 2021/01/13
	case __TMMODE_DGN_STL_SPDF__:	//Add by tss 2024/02/29
	case __TMMODE_DGN_STL_IMEL__:	//Add by tss
	case __TMMODE_DGN_STL_INDF__:
	case __TMMODE_DGN_STL_DSAU__:
	case __TMMODE_DGN_STL_DSAS__:
    case __TMMODE_DGN_STL_SDSW__://add by maxiao 2015-10-22
	case __TMMODE_DGN_STL_PFMC__:
	case	__TMMODE_DGN_STL_PFCM__:
	case	__TMMODE_DGN_STL_PFDL__:
	case	__TMMODE_DGN_STL_PFMP__:
	case	__TMMODE_DGN_STL_PFNP__:
	case __TMMODE_DGN_STL_SMSP__:
	case __TMMODE_DGN_STL_JP_POSITION__:
	case __TMMODE_DGN_STL_STBD__:
	case __TMMODE_DGN_STL_STUS__:
	case __TMMODE_DGN_STL_STRE__:
	case __TMMODE_DGN_STL_SPSC__:
	case __TMMODE_DGN_STL_SPCS__:
	case __TMMODE_DGN_STL_SPLS__:
	case __TMMODE_DGN_STL_SPPB__:
	case __TMMODE_DGN_STL_SPLB__:
	case __TMMODE_DGN_STL_SP14SeisMembParams__:
	case __TMMODE_DGN_STL_SP16SafetyMembParams__:
	case __TMMODE_DGN_STL_SP16CombinedSectParams__:

		//	RC Tab
	case __TMMODE_DGN_CON_REBB__:
	case __TMMODE_DGN_CON_REBC__:
	case __TMMODE_DGN_CON_REBR__:
	case __TMMODE_DGN_CON_REBW__:
	case __TMMODE_DGN_CON_WMAK__:
	case __TMMODE_DGN_CON_BEMW__:
	case __TMMODE_DGN_CON_EBMW__:
	case __TMMODE_DGN_CON_CVL_SUFC__:
	case __TMMODE_DGN_CON_CVL_RMFR__:
	case __TMMODE_DGN_CON_CVL_ULSP__:
	case __TMMODE_DGN_CON_CVL_RDGN_BEAM__:
	case __TMMODE_DGN_CON_CVL_RDGN_COLU__:
	case __TMMODE_DGN_CON_JSFT__: //Add By RSH 2002.12.26
	case __TMMODE_DGN_CON_PSAS__: //Add By RSH 2002.12.02
	case __TMMODE_DGN_CON_MRFT__: //Add By RSH 2003.07.04
	case __TMMODE_DGN_CON_MCMB__:
	case __TMMODE_DGN_CON_DFBA__:
	case __TMMODE_DGN_CON_SERV__: // Coded b-------------------------------------------------------------------------------------y Seungjun MNet:No.2393 ('20060828)
	case __TMMODE_DGN_CON_SPVAREMA__:
	case __TMMODE_DGN_CON_SERV_SLAB__: 
	case __TMMODE_DGN_CON_REIN_SHEAR__:
	case __TMMODE_DGN_GEN_MEMB_PLATE__:
	case __TMMODE_DGN_CON_ELEM_BAR__: // Coded by sshan MNet:No.2484 ('20061109)
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_BEAM:
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_COL :
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_BRAC:
	case __TMMODE_DGN_CON_ELEM_BAR__SUB_WALL:
	case __TMMODE_DGN_CON_BEAM_BAR__:
	case __TMMODE_DGN_CON_DORC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_PORC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_CWRC__:// Coded by sshan MNet:No.2607 ('20061220)
	case __TMMODE_DGN_CON_LEKF__:// Coded by sshan MNet:No.2607 ('20061226)
	case __TMMODE_DGN_CON_RSDM__:// Coded by bjshin MNet:No.3992 ('20090517)
	case __TMMODE_DGN_CON_TRFT__:// Coded by sshan MNet:No.3780 ('20090320)
	case __TMMODE_DGN_CON_UCCF__:// Add by GAY. PMS:4267. ('11.12.27). 중국신기준-불확정성 분항계수.
	case __TMMODE_DGN_CON_REXC__:
	case __TMMODE_DGN_CON_PMDM__:
	case __TMMODE_DGN_CON_SCOL__:
	case __TMMODE_DGN_CON_OCDF__:
	case __TMMODE_DGN_CON_PBDC__:
	case __TMMODE_DGN_CON_PCDC__:
	case __TMMODE_DGN_CON_PBDD__:
	case __TMMODE_DGN_CON_PCDD__:
	case __TMMODE_DGN_CON_STAN__:
	case __TMMODE_DGN_CON_MNDC__:
	case __TMMODE_DGN_CON_SAFM__:
	case __TMMODE_DGN_CON_RRTR__:
	case __TMMODE_DGN_CON_EXPOSURE_CLASS__:
	  //	SRC Tab
	case __TMMODE_DGN_SRC_RBSB__:
	case __TMMODE_DGN_SRC_REBS__:
		//	Cold Formed Steel Tab
	case __TMMODE_DGN_CFS_SERV__:
	case __TMMODE_DGN_CFS_CBFT__:
	case __TMMODE_DGN_CFS_CSOS__:
	case __TMMODE_DGN_CFS_CSMS__:
	case __TMMODE_DGN_CFS_PLIN__:
	case __TMMODE_DGN_CFS_SPHI__:
	case __TMMODE_DGN_CFS_PFMC__:
	case __TMMODE_DGN_CFS_PFCM__:
	case __TMMODE_DGN_CFS_PFDL__:
	case __TMMODE_DGN_CFS_PFMP__:
	case __TMMODE_DGN_CFS_PFNP__:
	  //  PSC Tab
	case __TMMODE_DGN_CRC_DGN_POSC__:
	case __TMMODE_DGN_CRC_DGN_POSX__:
	case __TMMODE_DGN_PSC_CRACK_PSCW__:
	case __TMMODE_DGN_PSC_SEGMENT__:
	case __TMMODE_DGN_PSC_EXPOSURE_CLASS__:
	case __TMMODE_DGN_PSC_OCDF__:
	case __TMMODE_DGN_PSC_SHEAR_CONNECTOR__:
	case __TMMODE_DGN_PSC_INTERFACE_SHEAR__:
	case __TMMODE_DGN_PSC_FATIGUE_CHECK__:
	  // CPG Tab
	case __TMMODE_DGN_CPG_STIF__:
	case __TMMODE_DGN_CPG_SHEAR__:
	case __TMMODE_DGN_CPG_POSITION__:
	case __TMMODE_DGN_CPG_POSI_DGN_OUT__:
	case __TMMODE_DGN_CPG_LOAD_APPLICATION__:
	case __TMMODE_DGN_CPG_TORSION__:
	case __TMMODE_DGN_CPG_DAMAGE__:
	case __TMMODE_DGN_CPG_SHEAR_CONN__://PMS:LRFD12 BSC-20131231
	case __TMMODE_DGN_CPG_SHEAR_CONN_CSA__:
	case __TMMODE_DGN_CPG_FATIGUE_PARAM__:
	  //case __TMMODE_DGN_CPG_UNBRA_LENG__:
	case __TMMODE_DGN_CPG_CURVE_BRID__:
	case __TMMODE_DGN_CPG_DECK_OVERHANG__:
	case __TMMODE_DGN_CPG_SHEAR_CONN_GROUP__:
	case __TMMODE_DGN_CPG_SERV__:
	case __TMMODE_DGN_CPG_LENG__:
	case __TMMODE_DGN_CPG_WEB_PANEL__:
	case __TMMODE_DGN_CPG_RESIDUAL_STRESS__:
	  //  Rating Tab
	case __TMMODE_DGN_RATING_OPTION__:
	case __TMMODE_DGN_RATING_PRINT__:
	case __TMMODE_DGN_RATING_PSC_ENVIRONMENT_CLASS__:
	case __TMMODE_DGN_PSC_ASSESSMENT_OPTION__:
	case __TMMODE_DGN_STEEL_RATING_FATIGUE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_CURVE_BRID__:
	case __TMMODE_DGN_STEEL_RATING_DECK_OVERHANG__:
	case __TMMODE_DGN_STEEL_RATING_OPTION__:
	case __TMMODE_DGN_STEEL_RATING_PRINT__:
	case __TMMODE_DGN_STEEL_RATING_WEB_PANEL__:
	case __TMMODE_DGN_STEEL_RATING_TORSION__:
	case __TMMODE_DGN_STEEL_RATING_SHEAR_CONN__:
	case __TMMODE_DGN_STEEL_RATING_SERVICE_PARAM__:
	case __TMMODE_DGN_STEEL_RATING_EFFECT_LENGTH_U_FRAME__:
	case __TMMODE_DGN_STEEL_RATING_MOMENT_DIST_FACT_SLENDER__:
	case __TMMODE_DGN_STEEL_RATING_FASTENER_RIVET__:
	case __TMMODE_DGN_STEEL_RATING_ASSIGN_MEMB_FACTOR__:
	case __TMMODE_DGN_STEEL_RATING_ASSESS_CHECK__:
	case __TMMODE_DGN_STEEL_RATING_ASSESS_REPORT__:
	case __TMMODE_DGN_RC_RATING_OPTION_BEAM__:
	case __TMMODE_DGN_RC_RATING_OPTION_PLATE__:
	case __TMMODE_DGN_RC_RATING_DGN_OPTION_PLATE__:
	case __TMMODE_DGN_RC_RATING_PRINT_BEAM__:
	case __TMMODE_DGN_RC_RATING_PRINT_PLATE__:
	// SOD Tab
	case __TMMODE_DGN_SOD_FLEXBUCK_INPUT__:
	case __TMMODE_DGN_SOD_POSI_DGN_OUT__:
	//Alu Table
	case __TMMODE_DGN_ALU_ARIB__:
	case __TMMODE_DGN_ALU_AWED__:
	case __TMMODE_DGN_ALU_AHTR__:
	case __TMMODE_DGN_ALU_APHI__:
	{
		DGN_CreateBarTreeDlg(this, &m_wndDgnTab, nCurMode);
		_SetCurWnd(&m_wndDgnTab);
		DGN_ShowChildDlgByID(&m_wndDgnTab, nCurMode, CurString);
	}
	break;

	case __TMMODE_STRUCT_MESH_AUTO__  : 
	case __TMMODE_STRUCT_MESH_MAP_KE__:
	case __TMMODE_STRUCT_MESH_MAP__   :
	{
		CreateStructMesh();
		_SetCurWnd(&m_wndMeshTab);
		if(m_wndMeshTab.GetActiveTab(pWnd))
		{
			CStructMeshDlg* pMeshDlgView = (CStructMeshDlg*)pWnd;
			pMeshDlgView->ShowChildDlgByID(nCurMode);

			int nSel = pMeshDlgView->m_ComboMenu.GetCurSel();
			CString strSelected;
			pMeshDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
			CurString = strSelected;
		}
	}
	break;
	case __TMMODE_STRUCT_EBEAM_CREATE__:
		{
			CreateStructEBeam();
			_SetCurWnd(&m_wndEBeamTab);
			if(m_wndEBeamTab.GetActiveTab(pWnd))
			{
				CStructEBeamDlg* pEBeamDlgView = (CStructEBeamDlg*)pWnd;
				pEBeamDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pEBeamDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pEBeamDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_STRIP_ASSIGN__:
		{
			CreateStructStrip();
			_SetCurWnd(&m_wndStripTab);
			if(m_wndStripTab.GetActiveTab(pWnd))
			{
				CStructStripDlg* pStripDlgView = (CStructStripDlg*)pWnd;
				pStripDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pStripDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pStripDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_DROPPANEL_ASSIGN__:
		{
			CreateStructDropPanel();
			_SetCurWnd(&m_wndDropPanelTab);
			if(m_wndDropPanelTab.GetActiveTab(pWnd))
			{
				CStructDropPanelDlg* pStripDlgView = (CStructDropPanelDlg*)pWnd;
				pStripDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pStripDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pStripDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_COLMCAP_ASSIGN__:
		{
			CreateStructColmCap();
			_SetCurWnd(&m_wndColmCapTab);
			if(m_wndColmCapTab.GetActiveTab(pWnd))
			{
				CStructColmCapDlg* pStripDlgView = (CStructColmCapDlg*)pWnd;
				pStripDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pStripDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pStripDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_MESHDGN_SLABSHEAR_REINFORCE__:
	case __TMMODE_MESHDGN_COLUMN_LOCATION__:
		{
			CreateModifyColmLoc();
			_SetCurWnd(&m_wndColmLocTab);
			if(m_wndColmLocTab.GetActiveTab(pWnd))
			{
				CModifyColmLocDlg* pColmLocDlgView = (CModifyColmLocDlg*)pWnd;
				pColmLocDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pColmLocDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pColmLocDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_STRUCT_SEIS_CLEAR_HGT_COL__:
	case __TMMODE_STRUCT_SEIS_SEISMIC_HOOK__:
	case __TMMODE_STRUCT_SEIS_COMP_TYPE__:
	case __TMMODE_STRUCT_SEIS_REINF_DETAILS__:
	case __TMMODE_STRUCT_SEIS_RC_COLUMN_M__:
	case __TMMODE_STRUCT_SEIS_PM_CURVE_METHOD__:
	case __TMMODE_STRUCT_SEIS_PBD_SEBE__:
	case __TMMODE_STRUCT_SEIS_PBD_SECO__:
	case __TMMODE_STRUCT_SEIS_PBD_SEWA__:
	case __TMMODE_STRUCT_SEIS_PBD_HLFW__:
	case __TMMODE_STRUCT_SEIS_PBD_SESP__:
		{
			CreateSeisCommonSech();
			_SetCurWnd(&m_wndSeisCommonTab);
			if ( m_wndSeisCommonTab.GetActiveTab(pWnd) )
			{
				CSeisCommonDlg* pDlgView = ( CSeisCommonDlg* ) pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);
			}
		}
		break;
	case __TMMODE_STRUCT_SEIS_XY_FACTOR__:
		{
			CreateSeisXYFactor();
			_SetCurWnd(&m_wndSeisXYFactTab);
			if(m_wndSeisXYFactTab.GetActiveTab(pWnd))
			{
				CSeisXYFactorDlg* pDlgView = (CSeisXYFactorDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
/////////////////////////////////////////////////////////////////////////////
// GPS 모드의 경우 시작 by mjpark                    ////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if defined(_TOWER)
	case __TMMODE_GPS_DISP_SRHDSP__:
	case __TMMODE_GPS_DISP_DEFORM__:
	case __TMMODE_GPS_DISP_DCONTR__:
	case __TMMODE_GPS_REAC_SRHRCF__:
	case __TMMODE_GPS_REAC_REACF__:
	case __TMMODE_GPS_SOIL_PRESS__:
	case __TMMODE_GPS_FORC_TFORC__:
	case __TMMODE_GPS_STRS_TSTRS__:
	case __TMMODE_GPS_DSGN_STEEL__:
	case __TMMODE_GPS_DSGN_CFSTL__:
	case __TMMODE_GPS_DSGN_ALU__:
		CreateGpsTab(nCurMode);
		_SetCurWnd(&m_wndGPSTab);
		break;
#else
	case __TMMODE_GPS_DISP_SRHDSP__:
	case __TMMODE_GPS_DISP_DEFORM__:
	case __TMMODE_GPS_DISP_DCONTR__:
	case __TMMODE_GPS_ISOLATION_DISP__:
	//case __TMMODE_GPS_DUTL_FACT__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(DISP_TAB%4);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(DISP_TAB-1);
			m_wndGPSTab.ScrollToTab(DISP_TAB-1);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;

	case __TMMODE_GPS_FORC_TFORC__:
	case __TMMODE_GPS_FORC_BFORC__:
	case __TMMODE_GPS_FORC_BDGRM__:
	case __TMMODE_GPS_PTDN_FORCE__:
	case __TMMODE_GPS_FORC_WFORC__:
	case __TMMODE_GPS_FORC_WSFBM__:
	case __TMMODE_GPS_FORC_PFORC__:
	case __TMMODE_GPS_FORC_PCLPD__:
	case __TMMODE_GPS_FORC_ESFBM__:
	case __TMMODE_GPS_FORC_RFRCD__:
	case __TMMODE_GPS_ISOLATION_FORCE__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
	  // 새로 만들지 않고 해당되는 탭을 Activate
		if (::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) {
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(FORC_TAB%4);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(FORC_TAB-1);
			m_wndGPSTab.ScrollToTab(FORC_TAB-1);
		}
		else {
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	case __TMMODE_GPS_STRS_TSTRS__:
	case __TMMODE_GPS_STRS_BSTRS__:
	case __TMMODE_GPS_STRS_BSTRSDETAIL__:
	case __TMMODE_GPS_STRS_BSTRSDETAILDGRM__:
	case __TMMODE_GPS_STRS_BSTRSDGRM__:
	case __TMMODE_GPS_STRS_BSTRSINC__:
	case __TMMODE_GPS_PTDN_STRESS__:
	case __TMMODE_GPS_STRS_PSTRS__:
	case __TMMODE_GPS_STRS_PNSTRS__:
	case __TMMODE_GPS_STRS_AXSTRS__:
	case __TMMODE_GPS_STRS_SSTRS__:
	case __TMMODE_GPS_ISOLATION_STRS__:
	case __TMMODE_GPS_STRESS_SECV__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면

		// 새로 만들지 않고 해당되는 탭을 Activate
		if (::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) {
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(STRS_TAB%4);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(STRS_TAB-1);
			m_wndGPSTab.ScrollToTab(STRS_TAB-1);
		}
		else {
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	case __TMMODE_GPS_PLATE_STRAIN__:
	case __TMMODE_GPS_SOLID_STRAIN__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면

		// 새로 만들지 않고 해당되는 탭을 Activate
		if (::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) {
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(0);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(4);
			m_wndGPSTab.ScrollToTab(4);
		}
		else {
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	case __TMMODE_GPS_REAC_SRHRCF__:
	case __TMMODE_GPS_REAC_REACF__:
	case __TMMODE_GPS_REAC_TARGET__:
	case __TMMODE_GPS_SOIL_PRESS__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면

		// 새로 만들지 않고 해당되는 탭을 Activate
		if (::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) {
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(REAC_TAB%4);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(REAC_TAB-1);
			m_wndGPSTab.ScrollToTab(REAC_TAB-1);
		}
		else {
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	
	
	
	case __TMMODE_GPS_INFL_REAC__:
	case __TMMODE_GPS_INFL_DISP__:
	case __TMMODE_GPS_INFL_TFORC__:
	case __TMMODE_GPS_INFL_BFORC__:
	case __TMMODE_GPS_INFL_ELNKFORC__:
	case __TMMODE_GPS_INFL_NLNKFORC__:
	case __TMMODE_GPS_INFL_PFORC__:
	case __TMMODE_GPS_INFL_BSTRS__:  
	case __TMMODE_GPS_INFL_SSTRS__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			//MvlTrc Batch Tab이 추가됨
			//if(nMCodeType == D_MOVE_CODE_BS)
			//{
				//((CGPSBarContainer*)CGPSBarContainer::m_pMyself)->CallOnActivateView(TRUE);
			//  DestroyGpsTab(); 
			//  CreateGpsTab(nCurMode);
			//}
			//else 
			{
				BOOL	bak=CGPSBarContainer::m_bRemove;
			  CGPSBarContainer::m_bRemove = TRUE;
				m_wndGPSTab.SetActiveTab(1); 
			  CGPSBarContainer::m_bRemove = bak;
			  m_wndGPSTab.SetActiveTab((INFL_TAB-STRS_TAB)-1);
			  m_wndGPSTab.ScrollToTab((INFL_TAB-STRS_TAB)-1);
			}
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	
	case __TMMODE_GPS_MVLTRC_REAC__:
	case __TMMODE_GPS_MVLTRC_DISP__:
	case __TMMODE_GPS_MVLTRC_TFORC__:
	case __TMMODE_GPS_MVLTRC_BFORC__:
	case __TMMODE_GPS_MVLTRC_ELNKFORC__:
	case __TMMODE_GPS_MVLTRC_NLNKFORC__:
	case __TMMODE_GPS_MVLTRC_PFORC__:
	case __TMMODE_GPS_MVLTRC_BSTRS__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if (::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
				  
			//KYE-HONG-20020602
			//MvlTrc Batch Tab이 추가됨으로 다음 Tab을 
			//새로운 Index로 Setting
			
			//if(CProduct::GetMovingType()==D_PRODUCT_MOVING_US || 
			//   CProduct::GetMovingType()==D_PRODUCT_MOVING_ORG )
			if(nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
				 nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
				 nMCodeType == D_MOVE_CODE_PENDOT      || 
				 nMCodeType == D_MOVE_CODE_KOREA       || 
				 nMCodeType == D_MOVE_CODE_CANADA      || 
				 nMCodeType == D_MOVE_CODE_TAIWAN      || 
				 nMCodeType == D_MOVE_CODE_CHINA       || 
				 nMCodeType == D_MOVE_CODE_EURO_BS     || 
				 nMCodeType == D_MOVE_CODE_RUSSIA      ||
				 nMCodeType == D_MOVE_CODE_BS          ||
				 nMCodeType == D_MOVE_CODE_INDIA       ||
				 nMCodeType == D_MOVE_CODE_KOREA_LRFD_2011 ||
				 nMCodeType == D_MOVE_CODE_AUSTRALIA   ||
				 nMCodeType == D_MOVE_CODE_POLAND      ||
				 nMCodeType == D_MOVE_CODE_SOUTH_AFRICA||
				 nMCodeType == D_MOVE_CODE_FRANCE      ||
				 nMCodeType == D_MOVE_CODE_NEWZEALAND  ||
				 nMCodeType == D_MOVE_CODE_BRAZIL)
				 //nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
			{
				m_wndGPSTab.SetActiveTab(3); 
			}
			else if (nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
			{
				m_wndGPSTab.SetActiveTab(2);
			}
			else if(nMCodeType == D_MOVE_CODE_TRANS)
			{
				m_wndGPSTab.SetActiveTab(2); 
			}
			else
			{
				m_wndGPSTab.SetActiveTab(0); 
			}

			CGPSBarContainer::m_bRemove = bak;

			//if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP )
			if( nMCodeType == D_MOVE_CODE_JAPAN || 
					nMCodeType == D_MOVE_CODE_TRANS )
			{
				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);
			}
			else if (nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
			{
				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);
			}
			else
			{
				m_wndGPSTab.SetActiveTab((MVLTRC_TAB-STRS_TAB)-1);
				m_wndGPSTab.ScrollToTab((MVLTRC_TAB-STRS_TAB)-1);
			}
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	
	case __TMMODE_GPS_INFS_REAC__:
	case __TMMODE_GPS_INFS_DISP__:
	case __TMMODE_GPS_INFS_TFORC__:
	case __TMMODE_GPS_INFS_BFORC__:
	case __TMMODE_GPS_INFS_ELNKFORC__:
	case __TMMODE_GPS_INFS_NLNKFORC__:
	case __TMMODE_GPS_INFS_PFORC__:
	case __TMMODE_GPS_INFS_BSTRS__:
	case __TMMODE_GPS_INFS_SSTRS__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			
			//KYE-HONG-20020602
			//MvlTrc Batch Tab이 추가됨으로 다음 Tab을 
			//새로운 Index로 Setting
			m_wndGPSTab.SetActiveTab(2);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab((INFS_TAB-STRS_TAB)-1);
			m_wndGPSTab.ScrollToTab((INFS_TAB-STRS_TAB)-1);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;

	case __TMMODE_GPS_MVLTRC_BATCH__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if (::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(0); // 다음 Tab을 Active
			CGPSBarContainer::m_bRemove = bak;

			// Transverse인 경우 
			if( nMCodeType == D_MOVE_CODE_TRANS)
			{
				m_wndGPSTab.SetActiveTab(2);
				m_wndGPSTab.ScrollToTab(2);
			}
			else if (nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
			{
				m_wndGPSTab.SetActiveTab(2);
				m_wndGPSTab.ScrollToTab(2);
			}
			else
			{
				m_wndGPSTab.SetActiveTab(3); // MvlTrc Batch Tab Active
				m_wndGPSTab.ScrollToTab(3);
			}
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break; 
	
	case __TMMODE_GPS_INFG_REAC__:
	case __TMMODE_GPS_INFG_DISP__:
	case __TMMODE_GPS_INFG_TFORC__:
	case __TMMODE_GPS_INFG_BFORC__:
	case __TMMODE_GPS_INFG_BSTRS__:
	case __TMMODE_GPS_INFG_ELNK__:
	case __TMMODE_GPS_INFG_NLNK__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;

			//다음 Tab을  새로운 Index로 Setting
			m_wndGPSTab.SetActiveTab(1);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(0);
			m_wndGPSTab.ScrollToTab(0);
//       m_wndGPSTab.SetActiveTab((GRID_INF_TAB-STRS_TAB)-1);
//       m_wndGPSTab.ScrollToTab((GRID_INF_TAB-STRS_TAB)-1);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;

	case __TMMODE_GPS_GRIDTRC_REAC__:
	case __TMMODE_GPS_GRIDTRC_DISP__:
	case __TMMODE_GPS_GRIDTRC_TFORC__:
	case __TMMODE_GPS_GRIDTRC_BFORC__:
	case __TMMODE_GPS_GRIDTRC_ELNK__:
	case __TMMODE_GPS_GRIDTRC_NLNK__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;

			//다음 Tab을  새로운 Index로 Setting
			m_wndGPSTab.SetActiveTab(0);
			CGPSBarContainer::m_bRemove = bak;

			m_wndGPSTab.SetActiveTab(1);
			m_wndGPSTab.ScrollToTab(1);
//       m_wndGPSTab.SetActiveTab((GRID_INF_TAB-STRS_TAB)-1);
//       m_wndGPSTab.ScrollToTab((GRID_INF_TAB-STRS_TAB)-1);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;


	case __TMMODE_GPS_TIMEHISTORY_GRAPH__:
	case __TMMODE_GPS_PUSHOVER_GRAPH__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(1);
			CGPSBarContainer::m_bRemove = bak;
			m_wndGPSTab.SetActiveTab(0);
			m_wndGPSTab.ScrollToTab(0);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;
	
	case __TMMODE_GPS_TIMEHISTORY_SSGP__:
	case __TMMODE_GPS_PUSHOVER_SSGP__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(2);
			CGPSBarContainer::m_bRemove = bak;
			m_wndGPSTab.SetActiveTab(1);
			m_wndGPSTab.ScrollToTab(1);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break; 
	
	case __TMMODE_GPS_TIMEHISTORY_TEXT__:
//	case __TMMODE_GPS_PUSHOVER_TEXT__:
		// 멀티탭 모드이고 탭이 이미 만들어져 있으면
		// 새로 만들지 않고 해당되는 탭을 Activate
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(0);
			CGPSBarContainer::m_bRemove = bak;
#ifdef _CIVIL
			m_wndGPSTab.SetActiveTab(1);
			m_wndGPSTab.ScrollToTab(1);
#else
			m_wndGPSTab.SetActiveTab(2);
			m_wndGPSTab.ScrollToTab(2);
#endif
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;

	case __TMMODE_GPS_PUSHOVER_TEXT__:
		if(::IsWindow(m_wndGPSTab.GetSafeHwnd()) && (m_wndGPSTab.GetTabCount()>0)) 
		{
			BOOL	bak=CGPSBarContainer::m_bRemove;
			CGPSBarContainer::m_bRemove = TRUE;
			m_wndGPSTab.SetActiveTab(0);
			CGPSBarContainer::m_bRemove = bak;
			m_wndGPSTab.SetActiveTab(1);
			m_wndGPSTab.ScrollToTab(1);
		}
		else 
		{
			CreateGpsTab(nCurMode);
		}
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;

	case __TMMODE_GPS_DYNA_EGMDSHP__:
	case __TMMODE_GPS_DYNA_BKMDSHP__:
	case __TMMODE_GPS_DSGN_STEEL__:
	case __TMMODE_GPS_DSGN_STEEL_LSD__:
	case __TMMODE_SEIS_CVL_JP_RESULT_3D_INVESTIGATE__:
	case __TMMODE_GPS_DSGN_CFSTL__:
	case __TMMODE_GPS_DSGN_ALU__:
	case __TMMODE_GPS_DSGN_PFDN__:
	case __TMMODE_GPS_DSGN_PFDNCFS__:
	case __TMMODE_GPS_DSGN_FIRE__:
	case __TMMODE_GPS_DSGN_CNCRT__:
	case __TMMODE_GPS_DSGN_SRC__:
	case __TMMODE_GPS_FORC_SFDBMD__:
	case __TMMODE_GPS_FORC_SFDBTF__:
	case __TMMODE_GPS_GIRDERSTRS_DGRM__:
	case __TMMODE_GPS_HYDRAHEAT_SSTRS__	:
	case __TMMODE_GPS_HYDRAHEAT_TEMPER__:
	case __TMMODE_GPS_HYDRAHEAT_DISPL__	 :
	case __TMMODE_GPS_HYDRAHEAT_ALTENSSTRS__:
	case __TMMODE_GPS_HYDRAHEAT_CRACKRATIO__:
	case __TMMODE_GPS_HYDRAHEAT_GRAPH__:
	case __TMMODE_GPS_STAGESTEP_GRAPH__:
	case __TMMODE_GPS_COLUMNSHORT_GRP__:  //KYE-HONG-20020327
	case __TMMODE_GPS_COLUMNSHORT_TXT__:  
	case __TMMODE_GPS_DISPPART_FACT:
	case __TMMODE_GPS_LOCAL_AXIS:
	case __TMMODE_GPS_SSFR__:
	case __TMMODE_GPS_DUTL_FACT__:
	case __TMMODE_GPS_THIS_DISP__:
	case __TMMODE_GPS_THIS_BFORC__:
	case __TMMODE_GPS_THIS_BDGRM__:
	case __TMMODE_GPS_THIS_BSTRS__:
	case __TMMODE_GPS_PSOV_SSDF__:
	case __TMMODE_GPS_PSOV_SSDR__:
	case __TMMODE_GPS_PSOV_MMDS__:
//   case __TMMODE_GPS_INFG_REAC__:
// 	case __TMMODE_GPS_INFG_DISP__:
// 	case __TMMODE_GPS_INFG_TFORC__:
// 	case __TMMODE_GPS_INFG_BFORC__:
// 	case __TMMODE_GPS_INFG_BSTRS__:  
//   case __TMMODE_GPS_INFG_ELNK__:
//   case __TMMODE_GPS_INFG_NLNK__:
	case __TMMODE_GPS_DEFN_EDGRM__:
	case __TMMODE_GPS_DRAW_EDGRM__:
	case __TMMODE_GPS_PSC_DSGN_DGRM__:
	case __TMMODE_GPS_BRDG_LOAD_DGRM__:
	case __TMMODE_GPS_STEEL_RATING_DGRM__:
	case __TMMODE_GPS_STEEL_RATING_AASHTO_DGRM__:
	case __TMMODE_GPS_PSC_RATING_DGRM__:
	case __TMMODE_GPS_RC_RATING_DGRM__:
	case __TMMODE_GPS_SOD_DGRM__:
	case __TMMODE_GPS_NRRS__:  
	case __TMMODE_GPS_BCCR__:  
	case __TMMODE_GPS_RC_DSGN_DGRM__:  
	case __TMMODE_GPS_RAIL_PSC_DSGN_DGRM__:
	case __TMMODE_GPS_RAIL_RC_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_COMP_DSGN_DGRM__:
	case __TMMODE_GPS_STEEL_COMP_DSGN_AS5100_DGRM__:
	case __TMMODE_GPS_PSOV_DUTL_FACT__:
	case __TMMODE_GPS_PSOV_FUNCTION__:
	case __TMMODE_GPS_DGN_SLAB_RESULT__:
	case __TMMODE_GPS_DGN_SLAB_CHECK__:
	case __TMMODE_GPS_DGN_SHEAR_CHECK__:
	case __TMMODE_GPS_DGN_SLABSERV_CHECK__:
	case __TMMODE_GPS_DGN_WALL_RESULT__:
	case __TMMODE_GPS_DGN_WALL_CHECK__:
	case __TMMODE_GPS_DGN_STRIP_RESULT__:
	case __TMMODE_GPS_DGN_STRIP_CHECK__:
	case __TMMODE_GPS_CPG_RUS_SHEAR_COON_FORCE_DGRM__:
	case __TMMODE_GPS_CPG_RUS_RESULT_DGRM__:
	case __TMMODE_GPS_DGN_SHELL_RESULT__:
	case __TMMODE_GPS_DGN_SHELL_CHKECK__:
	case __TMMODE_GPS_DGN_SHELL_SHEAR_CHKECK__:
	case __TMMODE_GPS_DGN_BATCH_REPORT__:
	case __TMMODE_GPS_SEIS_1ST_RC_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_RC_PERFORM__:
	case __TMMODE_GPS_SEIS_1ST_STL_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_STL_PERFORM__:
	case __TMMODE_GPS_SEIS_1ST_MASONRY_PERFORM__:
	case __TMMODE_GPS_SEIS_2ND_MASONRY_PERFORM__:
	case __TMMODE_GPS_THIS_ENRG_GRAPH__:
	case __TMMODE_GPS_THIS_ENRG_GROUP_GRAPH__:
	case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_PO__:
	case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_PO__:
	case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_TH__:
	case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_TH__:
	case __TMMODE_GPS_SEIS_STORY_GRAPH_PFRM_TH__:
	case __TMMODE_GPS_DGN_DAMP_STEEL__:
		CreateGpsTab(nCurMode);
		_SetCurWnd(&m_wndGPSTab);
		CurString = CGPSBarContainer::m_CurString;
		break;

#endif
/////////////////////////////////////////////////////////////////////////////
// GPS 모드의 경우 끝 by mjpark                       ///////////////////////
/////////////////////////////////////////////////////////////////////////////

	case __TMMODE_JUD_SLAB__  :  
		{
			JUD_CreateBarTreeDlg(this, &m_wndJUDTab, nCurMode);
			_SetCurWnd(&m_wndJUDTab);
			break;
		}
	case __TMMODE_SEIS_INFILL_WALL_CREATE__  :
	case __TMMODE_SEIS_INFILL_WALL_REDUCE__  :  
		{
			CreateSeisInfill();
			_SetCurWnd(&m_wndInfillTab);
			/*!@#$*/if(m_wndInfillTab.GetActiveTab(pWnd))
			{
				CSeisInfillWallDlg* pInfillWall = (CSeisInfillWallDlg*)pWnd;
				pInfillWall->ShowChildDlgByID(nCurMode);

				int nSel = pInfillWall->m_ComboMenu.GetCurSel();
				CString strSelected;
				pInfillWall->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;

			}
			break;
		}
	case __TMMODE_SEIS_EFFECT_STIFF__:
	case __TMMODE_SEIS_GENERATE_EFFECT_STIFF__:
		{
			CreateSeisGenerateStiff();
			_SetCurWnd(&m_wndSeisTab);
			/*!@#$*/if(m_wndSeisTab.GetActiveTab(pWnd))
			{
				CEffectStiffness* pStffDlgView = (CEffectStiffness*)pWnd;
				pStffDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pStffDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pStffDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;

			}
		}
		break;

	case __TMMODE_SEIS_1ST_RC_M_FACTOR__:
		{
			CreateSeisRcMFactor();
			_SetCurWnd(&m_wndSeisMFactTab);
			/*!@#$*/if(m_wndSeisMFactTab.GetActiveTab(pWnd))
			{
				CSeisMFactorDlg* pDlgView = (CSeisMFactorDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;

			}
		}
		break;

	case __TMMODE_SEIS_1ST_STL_M_FACTOR__:
		{
			CreateSeisStlMFactor();
			_SetCurWnd(&m_wndSeisMFactTab);
			/*!@#$*/if(m_wndSeisMFactTab.GetActiveTab(pWnd))
			{
				CSeisMFactorDlg* pDlgView = (CSeisMFactorDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_SEIS_1ST_MASONRY_M_FACTOR__:
		{
			CreateSeisMasonryMFactor();
			_SetCurWnd(&m_wndSeisMFactTab);
			/*!@#$*/if(m_wndSeisMFactTab.GetActiveTab(pWnd))
			{
				CSeisMFactorDlg* pDlgView = (CSeisMFactorDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pDlgView->m_ComboMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_ComboMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;

	case __TMMODE_SEIS_CVL_EVAL_GROUP__:
	case __TMMODE_SEIS_CVL_SECT_EFFECT_STIFF__:
		{
			CreateSeisCvl();
			_SetCurWnd(&m_wndSeisCvlTab);
			if(m_wndSeisCvlTab.GetActiveTab(pWnd))
			{
				CSeisCvlDlg* pDlgView = (CSeisCvlDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pDlgView->m_cmbMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_cmbMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_SEIS_CVL_RESULT_EVAL_PARAM__:
		{
			CreateSeisCvlMisc();
			_SetCurWnd(&m_wndSeisCvlMiscTab);
			if(m_wndSeisCvlMiscTab.GetActiveTab(pWnd))
			{
				CSeisCvlMiscDlg* pDlgView = (CSeisCvlMiscDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

				int nSel = pDlgView->m_cmbMenu.GetCurSel();
				CString strSelected;
				pDlgView->m_cmbMenu.GetLBText(nSel, strSelected);
				CurString = strSelected;
			}
		}
		break;

	// seismic investigation Civil JP
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_UPPER__:
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_LOWER__:
	case __TMMODE_SEIS_CVL_JP_SETTING_TARGET_USER__:
		{
			CreateSeisSetTarget_CvlJP();
			_SetCurWnd(&m_wndSeisSetTargetTab);
			if (m_wndSeisSetTargetTab.GetActiveTab(pWnd))
			{
				CSeisCvlJpSetTargetDlg* pDlgView = (CSeisCvlJpSetTargetDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

// 				int nSel = pDlgView->m_cmbMenu.GetCurSel();
// 				CString strSelected;
// 				pDlgView->m_cmbMenu.GetLBText(nSel, strSelected);
// 				CurString = strSelected;
			}
		}
		break;
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIMD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIRD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIBD__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIDP__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SISR__:
	case __TMMODE_SEIS_CVL_JP_SETTING_INVESTIGATE_SIET__:
		{
			CreateSeisSetting_CvlJP();
			_SetCurWnd(&m_wndSeisSetInvestTab);
			if (m_wndSeisSetInvestTab.GetActiveTab(pWnd))
			{
				CSeisCvlJpSetInvestDlg* pDlgView = (CSeisCvlJpSetInvestDlg*)pWnd;
				pDlgView->ShowChildDlgByID(nCurMode);

// 				int nSel = pDlgView->m_cmbMenu.GetCurSel();
// 				CString strSelected;
// 				pDlgView->m_cmbMenu.GetLBText(nSel, strSelected);
// 				CurString = strSelected;
			}
		}
	break;
// 	case __TMMODE_SEIS_CVL_JP_RESULT_3D_INVESTIGATE__:
// 	{
// // 		CreateCvlSeisSetTargetTab();
// // 		_SetCurWnd(&m_wndCvlSeisSetTargetTab);
// // 		if (m_wndCvlSeisSetTargetTab.GetActiveTab(pWnd))
// // 		{
// // 
// // 		}
// 	}
// 	break;

	// Post Tension
	case __TMMODE_PT_DRAW_TENDON_LINE_BEAM__:
	case __TMMODE_PT_DRAW_TENDON_LINE_SLAB__:
	case __TMMODE_PT_DRAW_TRANS_TENDON__:
	case __TMMODE_PT_DRAW_MOD_TENDON__:
	case __TMMODE_PT_DRAW_DEL_TENDON__:
	case __TMMODE_PT_DRAW_RENUM_TENDON__:
		{
			CreateTendon();
			_SetCurWnd(&m_wndPostTensionTab);
			if (m_wndPostTensionTab.GetActiveTab(pWnd))
			{
				CPostTensionDrawTendonLineDlg* pStffDlgView = (CPostTensionDrawTendonLineDlg*)pWnd;
				pStffDlgView->ShowChildDlgByID(nCurMode);
			}
		}
		break;
	case __TMMODE_PT_TENDON_MATCH_PROFILE__:
		{
			CreateProfile();
			_SetCurWnd(&m_wndPostTensionTab);
			if ( m_wndPostTensionTab.GetActiveTab(pWnd))
			{
				CPostTensionTendonProfileDlg* pStffDlgView = (CPostTensionTendonProfileDlg*)pWnd;
				pStffDlgView->ShowChildDlgByID(nCurMode);
			}
		}
		break;
	case __TMMODE_PT_SUPPORT_LINE_CREATE__:
	case __TMMODE_PT_DESIGN_STRIP_CREATE__:
	case __TMMODE_PT_DESIGN_STRIP_AUTO__:
	case __TMMODE_PT_DESIGN_STRIP_MODIFY_SECTION__:
		{
			CreateDesignStrip();
			_SetCurWnd(&m_wndPostTensionTab);
			if (m_wndPostTensionTab.GetActiveTab(pWnd))
			{
				CPostTensionDesignStripCommonDlg* pStffDlgView = (CPostTensionDesignStripCommonDlg*)pWnd;
				pStffDlgView->ShowChildDlgByID(nCurMode);
			}
		}
		break;

		default:
			break;
	}
	
	_GetCurWnd()->ShowWindow(SW_SHOW);
 
	m_nCurMode = nCurMode;

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	
	return TRUE;
}


void CBarTreeMenu::CreateDefaultTreeMenu()
{
#if !defined(_CIVIL)
	m_pTreeMenuCtrl->SetBkGroundColor (m_TreeCtrlBkColor);
#endif
	m_pTreeTableCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	
	if(m_pTreeWorkCtrl)
		m_pTreeWorkCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
		
	if(m_pTreeGroupCtrl)
		m_pTreeGroupCtrl->SetBkGroundColor(m_TreeCtrlBkColor);  

	if(m_pTreeSeisCtrl)
		m_pTreeSeisCtrl->SetBkGroundColor(m_TreeCtrlBkColor);

#if defined(_MGEN) || defined(_CIVIL)
	if(m_pTreeReportCtrl)
		m_pTreeReportCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
#endif

	// TODO: Add your specialized creation code here
	m_wndTab.Create(this, 1000);
	
#if !defined(_CIVIL)
	m_pTreeMenuCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x100);
	m_pTreeMenuCtrl->EnableToolTips(TRUE); 
	m_wndTab.AddTab(m_pTreeMenuCtrl, _LS(IDS_WG_TREEMENU_Menu));
#endif

	m_pTreeTableCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x101);
	m_pTreeTableCtrl->EnableToolTips(TRUE);
	m_pTreeTableCtrl->EnableFilterBar(TRUE, *m_pfilterBarOptions, FALSE);
	m_wndTab.AddTab(m_pTreeTableCtrl, _LS(IDS_WG_TREEMENU_Tables));

	//m_wndTab.AddTab(RUNTIME_CLASS(CTestView),_LS(IDS_WG_TREEMENU_Node)));
	//m_wndTab.AddTab(RUNTIME_CLASS(CTestView),_LS(IDS_WG_TREEMENU_Element)));
	
	//((CDialog*)m_pDlgNodeElem)->Create(CreateNodeElemDlg::IDD,&m_wndTab);
	//m_wndTab.AddTab(m_pDlgNodeElem,_T("EXT"));
		
	m_wndTab.SetActiveTab(0);
	m_wndTab.ScrollToTab(0);
	_SetCurWnd(&m_wndTab);

#if defined(_MGEN)
	if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP)
	{
		if(m_pTreeJUDCtrl)
			m_pTreeJUDCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	}  
#endif

}

void CBarTreeMenu::CreateStructTab()
{

	m_wndNodeElemTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndNodeElemTab.AddTab(RUNTIME_CLASS(CStructNodeDlg),_LS(IDS_WG_TREEMENU_Node),NULL,CStructNodeDlg::IDD);
	m_wndNodeElemTab.AddTab(RUNTIME_CLASS(CStructElementDlg),_LS(IDS_WG_TREEMENU_Element),NULL,CStructElementDlg::IDD);
	m_wndNodeElemTab.AddTab(RUNTIME_CLASS(CStructBndrDlg), _LS(IDS_WG_TREEMENU_Boundary),NULL,CStructBndrDlg::IDD);
#ifndef _MVIL
#ifndef _TOWER
	m_wndNodeElemTab.AddTab(RUNTIME_CLASS(CStructMassDlg), _LS(IDS_WG_TREEMENU_Mass),NULL,CStructMassDlg::IDD);
#endif
#endif
	m_wndNodeElemTab.AddTab(RUNTIME_CLASS(CStructStldDlg), _LS(IDS_WG_TREEMENU_Load),NULL,CStructStldDlg::IDD);
}

void CBarTreeMenu::CreateMeshTab()
{
	m_wndMeshTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndMeshTab.AddTab(RUNTIME_CLASS(CStructMeshDlg),_LS(IDS_TM_Mesh),NULL,CStructMeshDlg::IDD);
}

void CBarTreeMenu::CreateEBeamTab()
{
	m_wndEBeamTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndEBeamTab.AddTab(RUNTIME_CLASS(CStructEBeamDlg),_LS(IDS_WG_TREEMENU_Create_EBeam),NULL,CStructEBeamDlg::IDD);
}

void CBarTreeMenu::CreateStripTab()
{
	m_wndStripTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndStripTab.AddTab(RUNTIME_CLASS(CStructStripDlg),_LS(IDS_TM_Assign_Strip),NULL,CStructStripDlg::IDD);
}

void CBarTreeMenu::CreateDropPanelTab()
{
	m_wndDropPanelTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndDropPanelTab.AddTab(RUNTIME_CLASS(CStructDropPanelDlg),_LS(IDS_TM_Assign_DropPanel),NULL,CStructDropPanelDlg::IDD);
}

void CBarTreeMenu::CreateColmCapTab()
{
	m_wndColmCapTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndColmCapTab.AddTab(RUNTIME_CLASS(CStructColmCapDlg),_LS(IDS_TM_Assign_ColmCapital),NULL,CStructColmCapDlg::IDD);
}

void CBarTreeMenu::CreateColmLocTab()
{
	m_wndColmLocTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndColmLocTab.AddTab(RUNTIME_CLASS(CModifyColmLocDlg),_LS(IDS_TM_MESHED_DESIGN),NULL,CModifyColmLocDlg::IDD);
}

void CBarTreeMenu::CreateSeisInfillTab()
{
	m_wndInfillTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndInfillTab.AddTab(RUNTIME_CLASS(CSeisInfillWallDlg),_LS(IDS_TREE_SEIS_INFILL_WALL),NULL, CSeisInfillWallDlg::IDD);
}
void CBarTreeMenu::CreateSeisTab()
{
	m_wndSeisTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndSeisTab.AddTab(RUNTIME_CLASS(CEffectStiffness),_LS(IDS_TREE_SEIS_EFFECT_STIFF),NULL, CEffectStiffness::IDD);
}

void CBarTreeMenu::DestroySeisTab()
{
	DestroyTabWnd(&m_wndSeisTab);
}

void CBarTreeMenu::CreateSeisCommonTab()
{
	m_wndSeisCommonTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndSeisCommonTab.AddTab(RUNTIME_CLASS(CSeisCommonDlg),_LS(IDS_TM_SEIS_COMMON),NULL,CSeisCommonDlg::IDD);

	CurString = _LS(IDS_TM_SEIS_COMMON);
}

void CBarTreeMenu::CreateSeisMFactTab()
{
	m_wndSeisMFactTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndSeisMFactTab.AddTab(RUNTIME_CLASS(CSeisMFactorDlg),_LS(IDS_WG_TREEMENU_SEIS_MFACT),NULL, CSeisMFactorDlg::IDD);
}

void CBarTreeMenu::CreateSeisXYFactTab()
{
	m_wndSeisXYFactTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndSeisXYFactTab.AddTab(RUNTIME_CLASS(CSeisXYFactorDlg),_LS(IDS_WG_TREEMENU_SEIS_XYFACT),NULL, CSeisXYFactorDlg::IDD);
}

void CBarTreeMenu::DestroyStructTab()
{
	/*
	if(::IsWindow(m_wndNodeElemTab.GetSafeHwnd()))
	{
		int nTabCount = m_wndNodeElemTab.GetTabCount();
		for (int i = 0; i < nTabCount; i++)
			m_wndNodeElemTab.RemoveTab(0);
		m_wndNodeElemTab.DestroyWindow();
	}
	*/
	DestroyTabWnd(&m_wndNodeElemTab);
}

void CBarTreeMenu::CreateStructNode()
{
	if(!::IsWindow(m_wndNodeElemTab.GetSafeHwnd()))
		CreateStructTab();
	
	m_wndNodeElemTab.SetActiveTab(0);
	m_wndNodeElemTab.ScrollToTab(0);
	
	//_SetCurWnd(&m_wndNodeElemTab);
}

void CBarTreeMenu::CreateStructElem()
{
	if(!::IsWindow(m_wndNodeElemTab.GetSafeHwnd()))
		CreateStructTab();
	
	m_wndNodeElemTab.SetActiveTab(1);
	m_wndNodeElemTab.ScrollToTab(1);
	//_SetCurWnd(&m_wndNodeElemTab);
}

void CBarTreeMenu::CreateStructBndr()
{
	if(!::IsWindow(m_wndNodeElemTab.GetSafeHwnd()))
		CreateStructTab();
	
	m_wndNodeElemTab.SetActiveTab(2);
	m_wndNodeElemTab.ScrollToTab(2);
}

void CBarTreeMenu::CreateStructMesh()
{
	if(!::IsWindow(m_wndMeshTab.GetSafeHwnd()))
		CreateMeshTab();
	
	m_wndMeshTab.SetActiveTab(0);
	m_wndMeshTab.ScrollToTab(0);
}


void CBarTreeMenu::CreateStructEBeam()
{
	if(!::IsWindow(m_wndEBeamTab.GetSafeHwnd()))
		CreateEBeamTab();

	m_wndEBeamTab.SetActiveTab(0);
	m_wndEBeamTab.ScrollToTab(0);
}


void CBarTreeMenu::CreateStructStrip()
{
	if(!::IsWindow(m_wndStripTab.GetSafeHwnd()))
		CreateStripTab();

	m_wndStripTab.SetActiveTab(0);
	m_wndStripTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateStructDropPanel()
{
	if(!::IsWindow(m_wndDropPanelTab.GetSafeHwnd()))
		CreateDropPanelTab();

	m_wndDropPanelTab.SetActiveTab(0);
	m_wndDropPanelTab.ScrollToTab(0);
}
void CBarTreeMenu::CreateStructColmCap()
{
	if(!::IsWindow(m_wndColmCapTab.GetSafeHwnd()))
		CreateColmCapTab();

	m_wndColmCapTab.SetActiveTab(0);
	m_wndColmCapTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateModifyColmLoc()
{
	if(!::IsWindow(m_wndColmLocTab.GetSafeHwnd()))
		CreateColmLocTab();

	m_wndColmLocTab.SetActiveTab(0);
	m_wndColmLocTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateSeisInfill()
{
	if(!::IsWindow(m_wndInfillTab.GetSafeHwnd()))
		CreateSeisInfillTab();

	m_wndInfillTab.SetActiveTab(0);
	m_wndInfillTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateSeisGenerateStiff()
{
	if(!::IsWindow(m_wndSeisTab.GetSafeHwnd()))
		CreateSeisTab();

	m_wndSeisTab.SetActiveTab(0);
	m_wndSeisTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateStructStld()
{
	if(!::IsWindow(m_wndNodeElemTab.GetSafeHwnd()))
		CreateStructTab();
#ifndef _TOWER
	m_wndNodeElemTab.SetActiveTab(4);
	m_wndNodeElemTab.ScrollToTab(4);
#else
	m_wndNodeElemTab.SetActiveTab(3);
	m_wndNodeElemTab.ScrollToTab(3);
#endif
}

void CBarTreeMenu::CreateStructMass()
{
#ifndef _TOWER
	if(!::IsWindow(m_wndNodeElemTab.GetSafeHwnd()))
		CreateStructTab();
	
	m_wndNodeElemTab.SetActiveTab(3);
	m_wndNodeElemTab.ScrollToTab(3);
#endif
}

void CBarTreeMenu::CreateSeisCommonSech()
{
	if(!::IsWindow(m_wndSeisCommonTab.GetSafeHwnd()))
		CreateSeisCommonTab();

	m_wndSeisCommonTab.SetActiveTab(0);
	m_wndSeisCommonTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateSeisCommonSehk()
{
	if(!::IsWindow(m_wndSeisCommonTab.GetSafeHwnd()))
		CreateSeisCommonTab();

	m_wndSeisCommonTab.SetActiveTab(1);
	m_wndSeisCommonTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateSeisCommonScmt()
{
	if(!::IsWindow(m_wndSeisCommonTab.GetSafeHwnd()))
		CreateSeisCommonTab();

	m_wndSeisCommonTab.SetActiveTab(2);
	m_wndSeisCommonTab.ScrollToTab(2);
}

void CBarTreeMenu::CreateSeisCommonSerd()
{
	if(!::IsWindow(m_wndSeisCommonTab.GetSafeHwnd()))
		CreateSeisCommonTab();

	m_wndSeisCommonTab.SetActiveTab(3);
	m_wndSeisCommonTab.ScrollToTab(3);
}

void CBarTreeMenu::CreateSeisCommonRccm()
{
	if(!::IsWindow(m_wndSeisCommonTab.GetSafeHwnd()))
		CreateSeisCommonTab();

	m_wndSeisCommonTab.SetActiveTab(4);
	m_wndSeisCommonTab.ScrollToTab(4);
}

void CBarTreeMenu::CreateSeisCommonPmcm()
{
	if(!::IsWindow(m_wndSeisCommonTab.GetSafeHwnd()))
		CreateSeisCommonTab();

	m_wndSeisCommonTab.SetActiveTab(5);
	m_wndSeisCommonTab.ScrollToTab(5);
}

void CBarTreeMenu::CreateSeisRcMFactor()
{
	if(!::IsWindow(m_wndSeisMFactTab.GetSafeHwnd()))
		CreateSeisMFactTab();

	m_wndSeisMFactTab.SetActiveTab(0);
	m_wndSeisMFactTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateSeisStlMFactor()
{
	if(!::IsWindow(m_wndSeisMFactTab.GetSafeHwnd()))
		CreateSeisMFactTab();

	m_wndSeisMFactTab.SetActiveTab(1);
	m_wndSeisMFactTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateSeisMasonryMFactor()
{
	if(!::IsWindow(m_wndSeisMFactTab.GetSafeHwnd()))
		CreateSeisMFactTab();

	m_wndSeisMFactTab.SetActiveTab(1);
	m_wndSeisMFactTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateSeisXYFactor()
{
	if(!::IsWindow(m_wndSeisXYFactTab.GetSafeHwnd()))
		CreateSeisXYFactTab();

	m_wndSeisXYFactTab.SetActiveTab(0);
	m_wndSeisXYFactTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateSeisCvl()
{
	if(!::IsWindow(m_wndSeisCvlTab.GetSafeHwnd()))
		CreateSeisCvlTab();

	m_wndSeisCvlTab.SetActiveTab(1);
	m_wndSeisCvlTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateSeisCvlMisc()
{
	if(!::IsWindow(m_wndSeisCvlMiscTab.GetSafeHwnd()))
		CreateSeisCvlMiscTab();

	m_wndSeisCvlMiscTab.SetActiveTab(1);
	m_wndSeisCvlMiscTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateSeisSetTarget_CvlJP()
{
	if (!::IsWindow(m_wndSeisSetTargetTab.GetSafeHwnd()))
		CreateSeisSetTargetTab();

	m_wndSeisSetTargetTab.SetActiveTab(1);
	m_wndSeisSetTargetTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateSeisSetting_CvlJP()
{
	if (!::IsWindow(m_wndSeisSetInvestTab.GetSafeHwnd()))
		CreateSeisSetInvestTab();

	m_wndSeisSetInvestTab.SetActiveTab(1);
	m_wndSeisSetInvestTab.ScrollToTab(1);
}

/*
void CBarTreeMenu::CreateJUDTab()
{
	m_wndJUDTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
	m_wndJUDTab.AddTab(RUNTIME_CLASS(CJUDBarDlg),_LS(IDS_TREEMENU_DEFINE_SLAB_AREA)));
}

void CBarTreeMenu::CreateJUDSlab()
{
	if(!::IsWindow(m_wndJUDTab.GetSafeHwnd()))
		CreateJUDTab();
	
	m_wndJUDTab.ActivateTab(0);
	m_wndJUDTab.ScrollToTab(0);
}
*/

void CBarTreeMenu::CreateTendon()
{
	if (!::IsWindow(m_wndPostTensionTab.GetSafeHwnd()))
		CreateTendonTab();

	m_wndPostTensionTab.SetActiveTab(0);
	m_wndPostTensionTab.ScrollToTab(0);
}

void CBarTreeMenu::CreateProfile()
{
	if (!::IsWindow(m_wndPostTensionTab.GetSafeHwnd()))
		CreateTendonTab();

	m_wndPostTensionTab.SetActiveTab(1);
	m_wndPostTensionTab.ScrollToTab(1);
}

void CBarTreeMenu::CreateDesignStrip()
{
	if (!::IsWindow(m_wndPostTensionTab.GetSafeHwnd()))
		CreateTendonTab();

	m_wndPostTensionTab.SetActiveTab(2);
	m_wndPostTensionTab.ScrollToTab(2);
}

void CBarTreeMenu::CreateTendonTab()
{
	m_wndPostTensionTab.Create(this, WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP, 1200);
	m_wndPostTensionTab.AddTab(RUNTIME_CLASS(CPostTensionDrawTendonLineDlg), _LS(IDS_WG_TREEMENU_TENDON), NULL, CPostTensionDrawTendonLineDlg::IDD);
	m_wndPostTensionTab.AddTab(RUNTIME_CLASS(CPostTensionTendonProfileDlg), _LS(IDS_WG_TREEMENU_TENDON_PROFILE), NULL, CPostTensionTendonProfileDlg::IDD);
	m_wndPostTensionTab.AddTab(RUNTIME_CLASS(CPostTensionDesignStripCommonDlg), _LS(IDS_WG_TREEMENU_DESIGN_STRIP), NULL, CPostTensionDesignStripCommonDlg::IDD);
}

void CBarTreeMenu::DestroyTendonTab()
{
	DestroyTabWnd(&m_wndPostTensionTab);
}

BOOL CBarTreeMenu::IsSameTab(int nMode)
{
	return (
		(__TMMODE_STRUCT_NODE_START__       < nMode && __TMMODE_STRUCT_NODE_END__       > nMode) ||
		(__TMMODE_STRUCT_ELEM_START__       < nMode && __TMMODE_STRUCT_ELEM_END__       > nMode) ||
		(__TMMODE_STRUCT_BNDR_START__       < nMode && __TMMODE_STRUCT_BNDR_END__       > nMode) ||
		(__TMMODE_STRUCT_STLD_START__       < nMode && __TMMODE_STRUCT_STLD_END__       > nMode) ||
		(__TMMODE_STRUCT_MASS_START__       < nMode && __TMMODE_STRUCT_MASS_END__       > nMode) /*||*/
//     (__TMMODE_SEIS_INFILL_WALL_START__  < nMode && __TMMODE_SEIS_INFILL_WALL_END__  > nMode) ||
//     (__TMMODE_SEIS_EFFECT_STIFF_START__ < nMode && __TMMODE_SEIS_EFFECT_STIFF_END__ > nMode)
	);
}

// 인영근 : 추가시 고칠곳 : 탭 추가시에 해당

BOOL CBarTreeMenu::IsSameDgnTab(int nMode)
{
	return (
		(__TMMODE_DGN_GEN_START__						< nMode && __TMMODE_DGN_GEN_END__					> nMode) ||
		(__TMMODE_DGN_STL_START__						< nMode && __TMMODE_DGN_STL_END__					> nMode) ||
		(__TMMODE_DGN_ALU_START__						< nMode && __TMMODE_DGN_ALU_END__					> nMode) ||
		(__TMMODE_DGN_CON_START__						< nMode && __TMMODE_DGN_CON_END__					> nMode) ||
		(__TMMODE_DGN_SRC_START__		 				< nMode && __TMMODE_DGN_SRC_END__					> nMode) ||
		(__TMMODE_DGN_CFS_START__		 				< nMode && __TMMODE_DGN_CFS_END__					> nMode) ||
		(__TMMODE_DGN_PSC_START__						< nMode && __TMMODE_DGN_PSC_END__					> nMode) ||
		(__TMMODE_DGN_CPG_START__						< nMode && __TMMODE_DGN_CPG_END__					> nMode) ||
		(__TMMODE_DGN_RATING_START__					< nMode && __TMMODE_DGN_RATING_END__				> nMode) ||
		(__TMMODE_DGN_STEEL_RATING_START__				< nMode && __TMMODE_DGN_STEEL_RATING_END__			> nMode) ||
		(__TMMODE_DGN_RC_RATING_START__					< nMode && __TMMODE_DGN_RC_RATING_END__				> nMode) ||
		(__TMMODE_DGN_SOD_START__						< nMode && __TMMODE_DGN_SOD_END__					> nMode)
	);
}

void CBarTreeMenu::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTreeMenuBarBase::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	/*
	if(!bShow)
	{
		CWnd * pWnd = AfxGetMainWnd();
		if(pWnd)
			pWnd->SetFocus();
	}
	*/
}

////////////////////////////////////////////////////////////////////////////////////
// by jbseon
//!@#$void CBarTreeMenu::DestroyTabWnd(SEC3DTabWnd* pTabWnd)
void CBarTreeMenu::DestroyTabWnd(mit::frx::MBaseTabWnd* pTabWnd)
{
	if(::IsWindow(pTabWnd->GetSafeHwnd()))
	{
		/*!@#$
	  int nCur=0, nDelete = 0;
	  BOOL bCurrent = pTabWnd->GetActiveTab(nCur);
		//nCur = pTabWnd->GetActiveTab();
		//BOOL bCurrent = (nCur != -1);
		int nCount = pTabWnd->GetTabCount();
		for (int i = 0; i < nCount; i++)
	  {
		if (i == nCur) { nDelete = 1; continue; }
			pTabWnd->RemoveTab(nDelete);
	  }
	  if (bCurrent) pTabWnd->RemoveTab(0);
		*/
		pTabWnd->RemoveAllTabs();

		pTabWnd->SetFocus();
		pTabWnd->DestroyWindow();
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Creation And Destruction of GPS Tab Window Begin, by mjpark       ///////////////
////////////////////////////////////////////////////////////////////////////////////
int CBarTreeMenu::IsGPSMultiTab(int nMode)
{
#if defined(_TOWER)
	return 0;
#endif

	switch (nMode) 
	{
		case __TMMODE_GPS_DISP_SRHDSP__:
		case __TMMODE_GPS_DISP_DEFORM__:
		case __TMMODE_GPS_DISP_DCONTR__:
		//case __TMMODE_GPS_DUTL_FACT__:
		case __TMMODE_GPS_FORC_TFORC__:
		case __TMMODE_GPS_FORC_BFORC__:
		case __TMMODE_GPS_FORC_BDGRM__:
		case __TMMODE_GPS_PTDN_FORCE__:
		case __TMMODE_GPS_FORC_WFORC__:
		case __TMMODE_GPS_FORC_WSFBM__:
		case __TMMODE_GPS_FORC_PFORC__:
		case __TMMODE_GPS_FORC_PCLPD__:
		case __TMMODE_GPS_FORC_ESFBM__:
		case __TMMODE_GPS_FORC_RFRCD__:
		case __TMMODE_GPS_STRESS_SECV__:
		case __TMMODE_GPS_ISOLATION_FORCE__:
		case __TMMODE_GPS_ISOLATION_DISP__:
		case __TMMODE_GPS_ISOLATION_STRS__:
		case __TMMODE_GPS_STRS_TSTRS__:
		case __TMMODE_GPS_STRS_BSTRS__:
		case __TMMODE_GPS_STRS_BSTRSDETAIL__:
		case __TMMODE_GPS_STRS_BSTRSDETAILDGRM__:
		case __TMMODE_GPS_STRS_BSTRSDGRM__:
		case __TMMODE_GPS_STRS_BSTRSINC__:
		case __TMMODE_GPS_PTDN_STRESS__:
		case __TMMODE_GPS_STRS_PSTRS__:
		case __TMMODE_GPS_STRS_PNSTRS__:
		case __TMMODE_GPS_STRS_AXSTRS__:
		case __TMMODE_GPS_STRS_SSTRS__:
		case __TMMODE_GPS_REAC_SRHRCF__:
		case __TMMODE_GPS_REAC_REACF__: //!@#$
		case __TMMODE_GPS_REAC_TARGET__:
		case __TMMODE_GPS_SOIL_PRESS__: //!@#$
		case __TMMODE_GPS_PLATE_STRAIN__:
		case __TMMODE_GPS_SOLID_STRAIN__:
		
		  return 1;
			break;

		case __TMMODE_GPS_INFL_REAC__:
		case __TMMODE_GPS_INFL_DISP__:
		case __TMMODE_GPS_INFL_TFORC__:
		case __TMMODE_GPS_INFL_BFORC__:
		case __TMMODE_GPS_INFL_ELNKFORC__:
		case __TMMODE_GPS_INFL_NLNKFORC__:
		case __TMMODE_GPS_INFL_PFORC__:
		case __TMMODE_GPS_INFL_BSTRS__:
		case __TMMODE_GPS_INFL_SSTRS__:
		case __TMMODE_GPS_MVLTRC_REAC__:
		case __TMMODE_GPS_MVLTRC_DISP__:
		case __TMMODE_GPS_MVLTRC_TFORC__:
		case __TMMODE_GPS_MVLTRC_BFORC__:
		case __TMMODE_GPS_MVLTRC_ELNKFORC__:
		case __TMMODE_GPS_MVLTRC_NLNKFORC__:
		case __TMMODE_GPS_MVLTRC_PFORC__:
		case __TMMODE_GPS_MVLTRC_BSTRS__:
		case __TMMODE_GPS_INFS_REAC__:
		case __TMMODE_GPS_INFS_DISP__:
		case __TMMODE_GPS_INFS_TFORC__:
		case __TMMODE_GPS_INFS_BFORC__:
		case __TMMODE_GPS_INFS_ELNKFORC__:
		case __TMMODE_GPS_INFS_NLNKFORC__:
		case __TMMODE_GPS_INFS_PFORC__:
		case __TMMODE_GPS_INFS_BSTRS__:
		case __TMMODE_GPS_INFS_SSTRS__:
		case __TMMODE_GPS_MVLTRC_BATCH__:
		
		  return 2;
			break;

		case __TMMODE_GPS_TIMEHISTORY_GRAPH__:
		case __TMMODE_GPS_TIMEHISTORY_TEXT__:
		case __TMMODE_GPS_TIMEHISTORY_SSGP__:
			return 3;
			break;
		
		case __TMMODE_GPS_PUSHOVER_GRAPH__:
		case __TMMODE_GPS_PUSHOVER_TEXT__:
		case __TMMODE_GPS_PUSHOVER_SSGP__:

			return 4;
			break;

		case __TMMODE_GPS_GRIDTRC_REAC__:
		case __TMMODE_GPS_GRIDTRC_DISP__:
		case __TMMODE_GPS_GRIDTRC_TFORC__:
		case __TMMODE_GPS_GRIDTRC_BFORC__:
		case __TMMODE_GPS_GRIDTRC_ELNK__:
		case __TMMODE_GPS_GRIDTRC_NLNK__:
		case __TMMODE_GPS_INFG_REAC__:
		case __TMMODE_GPS_INFG_DISP__:
		case __TMMODE_GPS_INFG_TFORC__:
		case __TMMODE_GPS_INFG_BFORC__:
		case __TMMODE_GPS_INFG_BSTRS__:
		case __TMMODE_GPS_INFG_ELNK__:
		case __TMMODE_GPS_INFG_NLNK__:
			return 5;
			break;

		default:
			return 0;
			break;
	}
}

void CBarTreeMenu::CreateGpsTab(int mode)
{
	//!@#$SECTab	*pTab;
	CWnd* pTab = NULL;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	
	T_MVCD_D MvcdD;
	if(!pDoc->m_pAttrCtrl->GetMvcd(MvcdD))MvcdD.Initialize();
	int nMCodeType = MvcdD.nCodeType;

#if defined(_TOWER)

	if(!::IsWindow(m_wndGPSTab.GetSafeHwnd())) 
	{
		m_wndGPSTab.Create(this, WS_CHILD|WS_VISIBLE|TWS_TABS_ON_TOP, 1200);
		pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _T("Results"));
		((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
		m_wndGPSTab.ActivateTab(0);
		m_wndGPSTab.ScrollToTab(0);
	}
	return;
#endif

	if (!::IsWindow(m_wndGPSTab.GetSafeHwnd())) {
		m_wndGPSTab.Create(this, WS_CHILD|WS_VISIBLE|TWS_TABS_ON_TOP, 1200);
		switch (mode) {
			case __TMMODE_GPS_DISP_SRHDSP__:
			case __TMMODE_GPS_DISP_DEFORM__:
			case __TMMODE_GPS_DISP_DCONTR__:
			//case __TMMODE_GPS_DUTL_FACT__:
			case __TMMODE_GPS_ISOLATION_DISP__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Reactions));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(REAC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Deformations));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DISP_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Forces));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(FORC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Stresses));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRS_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Strains));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRN_TAB, mode);

				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_FORC_TFORC__:
			case __TMMODE_GPS_FORC_BFORC__:
			case __TMMODE_GPS_FORC_BDGRM__:
			case __TMMODE_GPS_PTDN_FORCE__:
			case __TMMODE_GPS_FORC_WFORC__:
			case __TMMODE_GPS_FORC_WSFBM__:
			case __TMMODE_GPS_FORC_PFORC__:
			case __TMMODE_GPS_FORC_PCLPD__:
			case __TMMODE_GPS_FORC_ESFBM__:
			case __TMMODE_GPS_FORC_RFRCD__:
			case __TMMODE_GPS_ISOLATION_FORCE__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Reactions));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(REAC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Deformations));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DISP_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Forces));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(FORC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Stresses));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRS_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Strains));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRN_TAB, mode);

				m_wndGPSTab.SetActiveTab(2);
				m_wndGPSTab.ScrollToTab(2);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_STRS_TSTRS__:
			case __TMMODE_GPS_STRS_BSTRS__:
			case __TMMODE_GPS_STRS_BSTRSDETAIL__:
			case __TMMODE_GPS_STRS_BSTRSDETAILDGRM__:
			case __TMMODE_GPS_STRS_BSTRSDGRM__:
			case __TMMODE_GPS_STRS_BSTRSINC__:
			case __TMMODE_GPS_PTDN_STRESS__:
			case __TMMODE_GPS_STRS_PSTRS__:
			case __TMMODE_GPS_STRS_PNSTRS__:
			case __TMMODE_GPS_STRS_AXSTRS__:
			case __TMMODE_GPS_STRS_SSTRS__:
			case __TMMODE_GPS_ISOLATION_STRS__:
			case __TMMODE_GPS_STRESS_SECV__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Reactions));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(REAC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Deformations));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DISP_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Forces));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(FORC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Stresses));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRS_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Strains));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRN_TAB, mode);

				m_wndGPSTab.SetActiveTab(3);
				m_wndGPSTab.ScrollToTab(3);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_REAC_SRHRCF__:
			case __TMMODE_GPS_REAC_REACF__:
			case __TMMODE_GPS_REAC_TARGET__:
			case __TMMODE_GPS_SOIL_PRESS__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Reactions));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(REAC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Deformations));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DISP_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Forces));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(FORC_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Stresses));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRS_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Strains));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRN_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);

				break;
			case __TMMODE_GPS_PLATE_STRAIN__:
			case __TMMODE_GPS_SOLID_STRAIN__:
				{
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Reactions));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(REAC_TAB, mode);
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Deformations));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DISP_TAB, mode);
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Forces));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(FORC_TAB, mode);
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Stresses));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRS_TAB, mode);
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Strains));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(STRN_TAB, mode);

					m_wndGPSTab.SetActiveTab(4);
					m_wndGPSTab.ScrollToTab(4);

					// Update
					m_wndGPSTab.GetActiveTab(pTab);
					if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
					break;
				}
			case __TMMODE_GPS_DYNA_EGMDSHP__:
			case __TMMODE_GPS_DYNA_BKMDSHP__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Mode_Shapes));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_INFL_REAC__:
			case __TMMODE_GPS_INFL_DISP__:
			case __TMMODE_GPS_INFL_TFORC__:
			case __TMMODE_GPS_INFL_BFORC__:
			case __TMMODE_GPS_INFL_ELNKFORC__:
			case __TMMODE_GPS_INFL_NLNKFORC__:
			case __TMMODE_GPS_INFL_PFORC__:
			case __TMMODE_GPS_INFL_BSTRS__:  
			case __TMMODE_GPS_INFL_SSTRS__:
				{
					CreateGpsTab_Move(mode);
					/*
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Lines)));
					((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFL_TAB, mode);
					
					// Infl Surface : Japan이 아니고, India도 아닐때 보여줌 
					if(nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
						 nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
						 nMCodeType == D_MOVE_CODE_PENDOT      || 
						 nMCodeType == D_MOVE_CODE_KOREA       || 
						 nMCodeType == D_MOVE_CODE_CANADA      || 
						 nMCodeType == D_MOVE_CODE_TAIWAN      || 
						 nMCodeType == D_MOVE_CODE_CHINA  )     
					{
						pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Surf_)));
						((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFS_TAB, mode);
					}
					
					// BS일때는 없음 
					if(nMCodeType != D_MOVE_CODE_BS)
					{
						pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_MVL_Tracer)));
						((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRC_TAB, mode);
					}
										
					// Batch Moving Load Tracer : US이면서 India가 아닐때 이거나 ORG에서만 보여줌 
					//if( (CProduct::GetMovingType()==D_PRODUCT_MOVING_US && !bIndia) || 
					//  CProduct::GetMovingType()==D_PRODUCT_MOVING_ORG)
					if(nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
						 nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
						 nMCodeType == D_MOVE_CODE_PENDOT      || 
						 nMCodeType == D_MOVE_CODE_KOREA       || 
						 nMCodeType == D_MOVE_CODE_CANADA      || 
						 nMCodeType == D_MOVE_CODE_TAIWAN      || 
						 nMCodeType == D_MOVE_CODE_CHINA       )
					{
						pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Batch_Conversion_MVLTRC_to_Static_Load));
						((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRCBATCH_TAB, mode);
					}
					*/
					m_wndGPSTab.SetActiveTab(0);
					m_wndGPSTab.ScrollToTab(0);

					// Update
					m_wndGPSTab.GetActiveTab(pTab);
					if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				}
				break;
			case __TMMODE_GPS_INFS_REAC__:
			case __TMMODE_GPS_INFS_DISP__:
			case __TMMODE_GPS_INFS_TFORC__:
			case __TMMODE_GPS_INFS_BFORC__:
			case __TMMODE_GPS_INFS_ELNKFORC__:
			case __TMMODE_GPS_INFS_NLNKFORC__:
			case __TMMODE_GPS_INFS_PFORC__:
			case __TMMODE_GPS_INFS_BSTRS__:
			case __TMMODE_GPS_INFS_SSTRS__:
				{
					CreateGpsTab_Move(mode);
					/*
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Lines));
				  ((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFL_TAB, mode);
				
					// Japan에서는 Infl surface제외 
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Surf_));
					((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFS_TAB, mode);
									
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_MVL_Tracer));
				  ((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRC_TAB, mode);

					// Batch Conversion from MVLTRC to Static Load 탭추가 
					//if(CProduct::GetMovingType()==D_PRODUCT_MOVING_US || 
					//   CProduct::GetMovingType()==D_PRODUCT_MOVING_ORG)
					if( nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
							nMCodeType == D_MOVE_CODE_AASHTO_LRFD ||
							nMCodeType == D_MOVE_CODE_PENDOT      ||               
							nMCodeType == D_MOVE_CODE_KOREA       || 
							nMCodeType == D_MOVE_CODE_CANADA      || 
							nMCodeType == D_MOVE_CODE_TAIWAN      || 
							nMCodeType == D_MOVE_CODE_CHINA )
					{
						pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Batch_Conversion_MVLTRC_to_Static_Load));
						((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRCBATCH_TAB, mode);
					}
					*/
					m_wndGPSTab.SetActiveTab(1);
				  m_wndGPSTab.ScrollToTab(1);
				}
				
				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
			  break;

			case __TMMODE_GPS_MVLTRC_REAC__:
			case __TMMODE_GPS_MVLTRC_DISP__:
			case __TMMODE_GPS_MVLTRC_TFORC__:
			case __TMMODE_GPS_MVLTRC_BFORC__:
			case __TMMODE_GPS_MVLTRC_ELNKFORC__:
			case __TMMODE_GPS_MVLTRC_NLNKFORC__:
			case __TMMODE_GPS_MVLTRC_PFORC__:
			case __TMMODE_GPS_MVLTRC_BSTRS__:
				{
					/*
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Lines));
				  ((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFL_TAB, mode);

					// Japan에서는 Infl surface제외 
					// if(CProduct::GetMovingType()!=D_PRODUCT_MOVING_JP)
					if(nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
						 nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
						 nMCodeType == D_MOVE_CODE_PENDOT      || 
						 nMCodeType == D_MOVE_CODE_KOREA       || 
						 nMCodeType == D_MOVE_CODE_CHINA       || 
						 nMCodeType == D_MOVE_CODE_CANADA      || 
						 nMCodeType == D_MOVE_CODE_TAIWAN      )
					{
						pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Surf_));
						((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFS_TAB, mode);
					}

					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_MVL_Tracer));
				  ((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRC_TAB, mode);

					// Batch Conversion from MVLTRC to Static Load 탭추가 
					if( nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
							nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
							nMCodeType == D_MOVE_CODE_PENDOT      || 
							nMCodeType == D_MOVE_CODE_KOREA       || 
							nMCodeType == D_MOVE_CODE_CANADA      || 
							nMCodeType == D_MOVE_CODE_TAIWAN      || 
							nMCodeType == D_MOVE_CODE_CHINA )
					{
						pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Batch_Conversion_MVLTRC_to_Static_Load));
						((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRCBATCH_TAB, mode);
					}
					*/
					CreateGpsTab_Move(mode);
				
					// Japan에서는 Infl surface제외 
					if(nMCodeType == D_MOVE_CODE_JAPAN ||
						 nMCodeType == D_MOVE_CODE_TRANS )
					{
						m_wndGPSTab.SetActiveTab(1);
						m_wndGPSTab.ScrollToTab(1);
					}
					else if (nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
					{
						m_wndGPSTab.SetActiveTab(1);
						m_wndGPSTab.ScrollToTab(1);
					}
					else
					{
						m_wndGPSTab.SetActiveTab(2);
						m_wndGPSTab.ScrollToTab(2);
					}
				}
				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);

				break;
				
			case __TMMODE_GPS_MVLTRC_BATCH__:
				/*
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Lines));
				((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFL_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Surf_));
				((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(INFS_TAB, mode);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_MVL_Tracer));
				((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRC_TAB, mode);

				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Batch_Conversion_MVLTRC_to_Static_Load));
				((CGPSBarContainer *) pTab/*->m_pClient* /)->InitAfterCreate(MVLTRCBATCH_TAB, mode);
				*/
				CreateGpsTab_Move(mode);
				if(nMCodeType == D_MOVE_CODE_TRANS)
				{
					m_wndGPSTab.SetActiveTab(2);
					m_wndGPSTab.ScrollToTab(2);
				}
				else if (nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
				{
					m_wndGPSTab.SetActiveTab(2);
					m_wndGPSTab.ScrollToTab(2);
				}
				else
				{
					m_wndGPSTab.SetActiveTab(3);
					m_wndGPSTab.ScrollToTab(3);
				}

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break; 
			 

			case __TMMODE_GPS_DSGN_STEEL__:
			case __TMMODE_GPS_DSGN_STEEL_LSD__:
			case __TMMODE_GPS_DSGN_CFSTL__:
			case __TMMODE_GPS_DSGN_ALU__:
			case __TMMODE_GPS_DSGN_FIRE__:
			case __TMMODE_GPS_DSGN_PFDN__:
			case __TMMODE_GPS_DSGN_PFDNCFS__:
			case __TMMODE_GPS_DSGN_CNCRT__:
			case __TMMODE_GPS_DSGN_SRC__:
			case __TMMODE_GPS_DGN_DAMP_STEEL__:
			{
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Design_Results));
				((CGPSBarContainer *)pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if (pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			}
			case __TMMODE_SEIS_CVL_JP_RESULT_3D_INVESTIGATE__:
			{
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Seis_CVLJP_Results));
				((CGPSBarContainer*)pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if (pTab) ((CGPSBarContainer*)pTab)->CallOnActivateView(TRUE);
				break;
			}
			case __TMMODE_GPS_INFG_REAC__:
			case __TMMODE_GPS_INFG_DISP__:
			case __TMMODE_GPS_INFG_TFORC__:
			case __TMMODE_GPS_INFG_BFORC__:
			case __TMMODE_GPS_INFG_BSTRS__:
			case __TMMODE_GPS_INFG_ELNK__:
			case __TMMODE_GPS_INFG_NLNK__:
			{
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Influence_Grid));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(GRID_INF_TAB, mode);

				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Tracer_Grid));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(GRID_TRC_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			}
			case __TMMODE_GPS_GRIDTRC_REAC__:
			case __TMMODE_GPS_GRIDTRC_DISP__:
			case __TMMODE_GPS_GRIDTRC_TFORC__:
			case __TMMODE_GPS_GRIDTRC_BFORC__:
			case __TMMODE_GPS_GRIDTRC_ELNK__:
			case __TMMODE_GPS_GRIDTRC_NLNK__:
				{
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Influence_Grid));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(GRID_INF_TAB, mode);

					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Tracer_Grid));
					((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(GRID_TRC_TAB, mode);

					m_wndGPSTab.SetActiveTab(1);
					m_wndGPSTab.ScrollToTab(1);

					// Update
					m_wndGPSTab.GetActiveTab(pTab);
					if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
					break;
				}
			case __TMMODE_GPS_DISPPART_FACT:
			  pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Displacement_Participation_Factor));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_LOCAL_AXIS:
			  pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Local_Axis));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_DUTL_FACT__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Inelastic_Hinge_Status));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break; 
				
			case __TMMODE_GPS_PSOV_DUTL_FACT__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Pushover_Hinge_Status));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break; 

			case __TMMODE_GPS_THIS_DISP__:
			case __TMMODE_GPS_THIS_BFORC__:
			case __TMMODE_GPS_THIS_BDGRM__:
			case __TMMODE_GPS_THIS_BSTRS__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Time_History_Results));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break; 

			case __TMMODE_GPS_FORC_SFDBMD__:
			case __TMMODE_GPS_FORC_SFDBTF__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Beam_Detail));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_COLUMNSHORT_GRP__:  //KYE-HONG-20020327
			case __TMMODE_GPS_COLUMNSHORT_TXT__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Story_Column_Shortening_Graph));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			
			case __TMMODE_GPS_SSFR__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Story_Shear_Force_Ratio));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_GIRDERSTRS_DGRM__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Girder_Stress_Diagram));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;


			case __TMMODE_GPS_TIMEHISTORY_GRAPH__:
				{
#ifdef _CIVIL
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHContainer), _LS(IDS_WG_TREEMENU_Time_History_Graph));
					((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
					((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_nContainerMode = 0;
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHTextContainer), _LS(IDS_WG_TREEMENU_Time_History_Text));
					((CGPSBarTHTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;

					m_wndGPSTab.SetActiveTab(0);
					m_wndGPSTab.ScrollToTab(0);

					// Update
					m_wndGPSTab.GetActiveTab(pTab);
					if(pTab) ((CGPSBarTHContainer *)pTab)->CallOnActivateView(TRUE);

#else
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHContainer), _LS(IDS_WG_TREEMENU_Time_History_Graph));
					((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
					((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_nContainerMode = 0;
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHSSGPContainer), _LS(IDS_WG_TREEMENU_Story_Shear_Graph));
					((CGPSBarTHSSGPContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
					pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHTextContainer), _LS(IDS_WG_TREEMENU_Time_History_Text));
					((CGPSBarTHTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;

					m_wndGPSTab.SetActiveTab(0);
					m_wndGPSTab.ScrollToTab(0);

					// Update
					m_wndGPSTab.GetActiveTab(pTab);
					if(pTab) ((CGPSBarTHContainer *)pTab)->CallOnActivateView(TRUE);
#endif
				}
				break;
			case __TMMODE_GPS_TIMEHISTORY_SSGP__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHContainer), _LS(IDS_WG_TREEMENU_Time_History_Graph));
				((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHSSGPContainer), _LS(IDS_WG_TREEMENU_Story_Shear_Graph));
				((CGPSBarTHSSGPContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHTextContainer), _LS(IDS_WG_TREEMENU_Time_History_Text));
				((CGPSBarTHTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;

				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarTHSSGPContainer *)pTab)->CallOnActivateView(TRUE);

				break; 

			case __TMMODE_GPS_TIMEHISTORY_TEXT__:
#ifdef _CIVIL
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHContainer), _LS(IDS_WG_TREEMENU_Time_History_Graph));
				((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHTextContainer), _LS(IDS_WG_TREEMENU_Time_History_Text));
				((CGPSBarTHTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;

				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarTHTextContainer *)pTab)->CallOnActivateView(TRUE);
#else
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHContainer), _LS(IDS_WG_TREEMENU_Time_History_Graph));
				((CGPSBarTHContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHSSGPContainer), _LS(IDS_WG_TREEMENU_Story_Shear_Graph));
				((CGPSBarTHSSGPContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHTextContainer), _LS(IDS_WG_TREEMENU_Time_History_Text));
				((CGPSBarTHTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;

				m_wndGPSTab.SetActiveTab(2);
				m_wndGPSTab.ScrollToTab(2);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarTHTextContainer *)pTab)->CallOnActivateView(TRUE);
#endif
				break;
			case __TMMODE_GPS_THIS_ENRG_GRAPH__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHEnrgGrphContainer), _LS(IDS_WG_GPS_TH_ENRG_GRPH));
				((CGPSBarTHEnrgGrphContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);
				
				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarTHEnrgGrphContainer *)pTab)->CallOnActivateView(TRUE);
				break; 
			case __TMMODE_GPS_THIS_ENRG_GROUP_GRAPH__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHEnrgGrphContainer), _LS(IDS_WG_GPS_TH_ENRG_GRPH));
				((CGPSBarTHEnrgGrphContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) 
				{
					((CGPSBarTHEnrgGrphContainer *)pTab)->CallOnActivateView(TRUE);
					((CGPSBarTHEnrgGrphContainer *)pTab)->ShowCurChildDlg(1);
				}
				break; 

			case __TMMODE_GPS_STAGESTEP_GRAPH__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarStgStpContainer), _LS(IDS_WG_TREEMENU_Nonlinear_Graph));
				((CGPSBarStgStpContainer*) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				((CGPSBarStgStpContainer*) pTab/*->m_pClient*/)->m_nContainerMode = 1;
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarStgStpContainer *)pTab)->CallOnActivateView(TRUE);
				break;
		
			case __TMMODE_GPS_HYDRAHEAT_SSTRS__	:
			case __TMMODE_GPS_HYDRAHEAT_TEMPER__:
			case __TMMODE_GPS_HYDRAHEAT_DISPL__	 :
			case __TMMODE_GPS_HYDRAHEAT_ALTENSSTRS__:
			case __TMMODE_GPS_HYDRAHEAT_CRACKRATIO__:
			case __TMMODE_GPS_HYDRAHEAT_GRAPH__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Hydration_Heat_Analysis));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_PSOV_SSDF__:
			case __TMMODE_GPS_PSOV_MMDS__:
			case __TMMODE_GPS_PSOV_SSDR__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Push_Over_Graph));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);
				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break; 
			case __TMMODE_GPS_DRAW_EDGRM__:
			case __TMMODE_GPS_DEFN_EDGRM__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_Truss_Beam_Diagram));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;   
			case __TMMODE_GPS_DGN_SLAB_RESULT__:
			case __TMMODE_GPS_DGN_SLAB_CHECK__:
			case __TMMODE_GPS_DGN_SHEAR_CHECK__:
			case __TMMODE_GPS_DGN_SLABSERV_CHECK__:
			case __TMMODE_GPS_DGN_WALL_RESULT__:
			case __TMMODE_GPS_DGN_WALL_CHECK__:
			case __TMMODE_GPS_DGN_STRIP_RESULT__:
			case __TMMODE_GPS_DGN_STRIP_CHECK__:
			case __TMMODE_GPS_DGN_SHELL_RESULT__:
			case __TMMODE_GPS_DGN_SHELL_CHKECK__:
			case __TMMODE_GPS_DGN_SHELL_SHEAR_CHKECK__:
			case __TMMODE_GPS_DGN_BATCH_REPORT__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_TM_Design_Result));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;   

			case __TMMODE_GPS_PSC_DSGN_DGRM__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Design_Results ));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_BRDG_LOAD_DGRM__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LSX(Bridge Load Rating Result Diagram));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_STEEL_RATING_DGRM__:			
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_GPS_STEEL_RATING_DESIGN_RESULT_DIAGRAM));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_STEEL_RATING_AASHTO_DGRM__:			
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_GPS_STEEL_RATING_DESIGN_RESULT_DIAGRAM));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_PSC_RATING_DGRM__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_GPS_PSC_RATING_DESIGN_RESULT_DIAGRAM));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_RC_RATING_DGRM__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_GPS_RC_RATING_DESIGN_RESULT_DIAGRAM));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_SOD_DGRM__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_GPS_SOD_RESULT_DIAGRAM));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_CPG_RUS_SHEAR_COON_FORCE_DGRM__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_GPS_CPG_SHEAR_CONN_FORCE_DIAGRAM));//KHD
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_CPG_RUS_RESULT_DGRM__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_GPS_CPG_RUS_RESULT_DIAGRAM));//KHD
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_NRRS__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Nodal_Results_of_RS));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			case __TMMODE_GPS_BCCR__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_RC_Beam_Column_Flexural_Capacity_Ratio));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_RC_DSGN_DGRM__:  
			case __TMMODE_GPS_RAIL_PSC_DSGN_DGRM__:  
			case __TMMODE_GPS_RAIL_RC_DSGN_DGRM__: 
			case __TMMODE_GPS_STEEL_DSGN_DGRM__:
			case __TMMODE_GPS_STEEL_COMP_DSGN_DGRM__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Design_Results ));
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;

			case __TMMODE_GPS_STEEL_COMP_DSGN_AS5100_DGRM__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_GPS_CPG_RUS_RESULT_DIAGRAM));
				((CGPSBarContainer*)pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if (pTab) ((CGPSBarContainer*)pTab)->CallOnActivateView(TRUE);
				break;
				
			case __TMMODE_GPS_PUSHOVER_GRAPH__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovContainer), _LS(IDS_WG_TREEMENU_Pushover_Graph));
				((CGPSBarPsovContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				((CGPSBarPsovContainer *) pTab/*->m_pClient*/)->m_nContainerMode = 0;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovTextContainer), _LS(IDS_WG_TREEMENU_Pushover_Text));
				((CGPSBarPsovTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarPsovContainer *)pTab)->CallOnActivateView(TRUE);

				break;

			case __TMMODE_GPS_PUSHOVER_SSGP__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovContainer), _LS(IDS_WG_TREEMENU_Pushover_Graph));
				((CGPSBarPsovContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovSSGPContainer), _LS(IDS_WG_TREEMENU_Story_Shear_Graph));
				((CGPSBarPsovSSGPContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovTextContainer), _LS(IDS_WG_TREEMENU_Pushover_Text));
				((CGPSBarPsovTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarPsovSSGPContainer *)pTab)->CallOnActivateView(TRUE);

				break; 

			case __TMMODE_GPS_PUSHOVER_TEXT__:
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovContainer), _LS(IDS_WG_TREEMENU_Pushover_Graph));
				((CGPSBarPsovContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovTextContainer), _LS(IDS_WG_TREEMENU_Pushover_Text));
				((CGPSBarPsovTextContainer *) pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;
				m_wndGPSTab.SetActiveTab(1);
				m_wndGPSTab.ScrollToTab(1);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarPsovTextContainer *)pTab)->CallOnActivateView(TRUE);

				break;

			case __TMMODE_GPS_PSOV_FUNCTION__:  
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarPsovMainDlg), _LS(IDS_WG_TREEMENU_Pushover_Analysis_Data));

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarPsovMainDlg*)pTab)->ShowChildDlgByID(mode);
				break;

			case __TMMODE_GPS_SEIS_1ST_RC_PERFORM__:
			case __TMMODE_GPS_SEIS_2ND_RC_PERFORM__:
		  {
				CDBDoc *pDoc = CDBDoc::GetDocPoint();
				int nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();

				CString strTitle = mode==__TMMODE_GPS_SEIS_1ST_RC_PERFORM__ ? _LS(IDS_TREEMENU_SEIS_1ST_RC_PERFORM) : _LS(IDS_TREEMENU_SEIS_2ND_RC_PERFORM);
				if(nMethod==D_SEME_METHOD_MOE2018 || nMethod==D_SEME_METHOD_MOE2019) 
					strTitle = mode==__TMMODE_GPS_SEIS_1ST_RC_PERFORM__ ? _LS(IDS_TREEMENU_SEIS_MFACT_RC_PERFORM) : _LS(IDS_TREEMENU_SEIS_PSOV_RC_PERFORM);

			  pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), strTitle);
			  ((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

			  m_wndGPSTab.SetActiveTab(0);
			  m_wndGPSTab.ScrollToTab(0);

			  // Update
			  m_wndGPSTab.GetActiveTab(pTab);
			  if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
			  break;
		  }

			case __TMMODE_GPS_SEIS_1ST_STL_PERFORM__:
			case __TMMODE_GPS_SEIS_2ND_STL_PERFORM__:
			{
				CDBDoc *pDoc = CDBDoc::GetDocPoint();
				int nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();

				CString strTitle = mode==__TMMODE_GPS_SEIS_1ST_STL_PERFORM__ ? _LS(IDS_TREEMENU_SEIS_1ST_STL_PERFORM) : _LS(IDS_TREEMENU_SEIS_2ND_STL_PERFORM);
				if(nMethod==D_SEME_METHOD_MOE2018 || nMethod==D_SEME_METHOD_MOE2019) 
					strTitle = mode==__TMMODE_GPS_SEIS_1ST_STL_PERFORM__ ? _LS(IDS_TREEMENU_SEIS_MFACT_STL_PERFORM) : _LS(IDS_TREEMENU_SEIS_PSOV_STL_PERFORM);
				
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), strTitle);
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			}

			case __TMMODE_GPS_SEIS_1ST_MASONRY_PERFORM__:
			case __TMMODE_GPS_SEIS_2ND_MASONRY_PERFORM__:
			{
				CString strTitle = mode==__TMMODE_GPS_SEIS_1ST_MASONRY_PERFORM__ ? _LS(IDS_TREEMENU_SEIS_MFACT_MASONRY_PERFORM) : _LS(IDS_TREEMENU_SEIS_PSOV_MASONRY_PERFORM);
				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), strTitle);
				((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
				break;
			}
			case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_PO__:
			case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_PO__:
			case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_PO__:
			case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_PO__:
			case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_TH__:
			case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_TH__:
			case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_TH__:
			case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_TH__:
			{
				CDBDoc* pDoc = CDBDoc::GetDocPoint();
				int nMethod = pDoc->m_pAttrCtrl2->GetSemeMethod();

				CString strTitle;
				switch (mode)
				{
				case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_PO__:
				case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_PO__:
				case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_PO__:
				case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_PO__:
					strTitle = _LS(IDS_DB_ANALCTRL_____AnalysisName_Pushover);
					break;
				case __TMMODE_GPS_SEIS_LUMPED_HINGE_PFRM_TH__:
				case __TMMODE_GPS_SEIS_FIBER_HINGE_PFRM_TH__:
				case __TMMODE_GPS_SEIS_AXIAL_STRAIN_PFRM_TH__:
				case __TMMODE_GPS_SEIS_SHEAR_FORCE_PFRM_TH__:
				case __TMMODE_GPS_SEIS_STORY_GRAPH_PFRM_TH__:
					strTitle = _LS(IDS_DB_ANALCTRL_____AnalysisName_History);
					break;
				}

				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), strTitle);
				((CGPSBarContainer*)pTab/*->m_pClient*/)->InitAfterCreate(DFLT_TAB, mode);

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if (pTab) ((CGPSBarContainer*)pTab)->CallOnActivateView(TRUE);
				break;
			}
			case __TMMODE_GPS_SEIS_STORY_GRAPH_PFRM_TH__:
			{
				CString strTitle = _LS(IDS_DB_ANALCTRL_____AnalysisName_History);

				pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarTHPFMCContainer), strTitle);
				((CGPSBarTHPFMCContainer*)pTab/*->m_pClient*/)->m_pTabWnd = &m_wndGPSTab;

				m_wndGPSTab.SetActiveTab(0);
				m_wndGPSTab.ScrollToTab(0);

				// Update
				m_wndGPSTab.GetActiveTab(pTab);
				if (pTab) ((CGPSBarTHPFMCContainer*)pTab)->CallOnActivateView(TRUE);

				break;
			}
		}

		//!@#$
		//m_wndGPSTab.GetActiveTab(pTab);
		//if(pTab) ((CGPSBarContainer *)pTab)->CallOnActivateView(TRUE);
	}
}

//-------------------------------------------------------------------------------------------------
// INIT : 2003.1.3. by KYE-HONG
// FUNC : Moving Load 관련 명령의 Tab생성에 관해 
//-------------------------------------------------------------------------------------------------
void CBarTreeMenu::CreateGpsTab_Move(int mode)
{
	CDBDoc   *pDoc = CDBDoc::GetDocPoint();
	//!@#$SECTab	 *pTab;
	CWnd* pTab = NULL;
	T_MVCD_D MvcdD;
	if(!pDoc->m_pAttrCtrl->GetMvcd(MvcdD))MvcdD.Initialize();
	int nMCodeType = MvcdD.nCodeType;
	
	// Infl Line은 모든버전에서 보임 
	pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Lines));
	((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(INFL_TAB, mode);
	
	// Infl Surface : Japan이 아니고, India, 횡방향 moving load도 아닐때 보여줌 
	if(nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
		 nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
		 nMCodeType == D_MOVE_CODE_PENDOT      || 
		 nMCodeType == D_MOVE_CODE_KOREA       || 
		 nMCodeType == D_MOVE_CODE_CANADA      || 
		 nMCodeType == D_MOVE_CODE_TAIWAN      || 
		 nMCodeType == D_MOVE_CODE_CHINA       || 
		 nMCodeType == D_MOVE_CODE_EURO_BS     || 
		 nMCodeType == D_MOVE_CODE_RUSSIA      ||
		 nMCodeType == D_MOVE_CODE_BS          ||
		 nMCodeType == D_MOVE_CODE_INDIA       ||
		 nMCodeType == D_MOVE_CODE_KOREA_LRFD_2011 ||
		 nMCodeType == D_MOVE_CODE_AUSTRALIA   ||
		 nMCodeType == D_MOVE_CODE_POLAND      ||
		 nMCodeType == D_MOVE_CODE_SOUTH_AFRICA||
		 nMCodeType == D_MOVE_CODE_FRANCE      ||
		 nMCodeType == D_MOVE_CODE_NEWZEALAND ||
		 nMCodeType == D_MOVE_CODE_BRAZIL)
		 //nMCodeType == D_MOVE_CODE_JAPAN_RAIL)
	{
		pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Infl__Surf_));
		((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(INFS_TAB, mode);
	}
	
	// BS일때는 없음 
	pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_MVL_Tracer));
	((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(MVLTRC_TAB, mode);
		
						
	// Batch Moving Load Tracer 
	if(nMCodeType == D_MOVE_CODE_AASHTO_STAN || 
		 nMCodeType == D_MOVE_CODE_AASHTO_LRFD || 
		 nMCodeType == D_MOVE_CODE_PENDOT      || 
		 nMCodeType == D_MOVE_CODE_KOREA       || 
		 nMCodeType == D_MOVE_CODE_CANADA      || 
		 nMCodeType == D_MOVE_CODE_TAIWAN      || 
		 nMCodeType == D_MOVE_CODE_CHINA       || 
		 nMCodeType == D_MOVE_CODE_BS          ||
		 nMCodeType == D_MOVE_CODE_EURO_BS     ||
		 nMCodeType == D_MOVE_CODE_RUSSIA      ||
		 nMCodeType == D_MOVE_CODE_TRANS       ||
		 nMCodeType == D_MOVE_CODE_INDIA       ||
		 nMCodeType == D_MOVE_CODE_KOREA_LRFD_2011 ||
		 nMCodeType == D_MOVE_CODE_AUSTRALIA   ||
		 nMCodeType == D_MOVE_CODE_POLAND      ||
		 nMCodeType == D_MOVE_CODE_SOUTH_AFRICA||
		 nMCodeType == D_MOVE_CODE_FRANCE      ||
		 nMCodeType == D_MOVE_CODE_JAPAN_RAIL  ||
		 nMCodeType == D_MOVE_CODE_NEWZEALAND  ||
		 nMCodeType == D_MOVE_CODE_BRAZIL)
	{
		pTab = m_wndGPSTab.AddTab(RUNTIME_CLASS(CGPSBarContainer), _LS(IDS_WG_TREEMENU_Batch_Conversion_MVLTRC_to_Static_Load));
		((CGPSBarContainer *) pTab/*->m_pClient*/)->InitAfterCreate(MVLTRCBATCH_TAB, mode);
	}
}

void CBarTreeMenu::DestroyGpsTab()
{
	CGPSBarContainer::m_bRemove = TRUE;
	DestroyTabWnd(&m_wndGPSTab);
}

// Work Tree에서 선택된 항목을 처리하는 것과 Select by Label로 처리하는 것을 
// 구분해서 처리한다. (대상에 따라서 상황이 다양하게 나타 날 수 있음 ) 
BOOL CBarTreeMenu::ShowDlgWithInit_by_SelectLabel(int Mode, UINT Key)
{
	SetModeST(Mode);

	MySEC3DTabWnd* pTabWnd = (MySEC3DTabWnd*)m_pCurWnd;
	CWnd* pView = NULL;
	if (!pTabWnd->GetActiveTab(pView)) return FALSE;
	if (!pView->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CFormViewInBar))) return FALSE;

	CFormViewInBar* pFormView = (CFormViewInBar*)pView;
	CMenuBarChildDlg* pChildDlg = (CMenuBarChildDlg*)pFormView->GetDlgByID(Mode);
	if (pChildDlg == 0) return FALSE;
	if (!pChildDlg->GetSafeHwnd() || !IsWindow(pChildDlg->GetSafeHwnd())) return FALSE;

	//pChildDlg->ExternalInit(Key);
	pChildDlg->ExternalInit_by_SelectLabel(Key);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Creation And Destruction of GPS Tab Window Ended, by mjpark       ///////////////
////////////////////////////////////////////////////////////////////////////////////

BOOL CBarTreeMenu::ShowDlgWithInit(int Mode, UINT key)
{
	SetModeST(Mode);

	MySEC3DTabWnd* pTabWnd = (MySEC3DTabWnd*)m_pCurWnd;
	CWnd* pView = NULL;
	if (!pTabWnd->GetActiveTab(pView)) return FALSE;
	if (!pView->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CFormViewInBar))) return FALSE;

	CFormViewInBar* pFormView = (CFormViewInBar*)pView;
	CMenuBarChildDlg* pChildDlg = (CMenuBarChildDlg*)pFormView->GetDlgByID(Mode);
	if (pChildDlg == 0) return FALSE;
	if (!pChildDlg->GetSafeHwnd() || !IsWindow(pChildDlg->GetSafeHwnd())) return FALSE;

	pChildDlg->ExternalInit(key);

	return TRUE;
}

int CBarTreeMenu::GetThisMode()
{
	return m_ThisMode;
}

/*
void CBarTreeMenu::AddGroupTab()
{
//#ifdef _MDEMO
//  return; // *^^* 데모 버전에서 뺀다.
//#endif
#ifdef _MVIL
	return; // *^^* 빌라 버전에서 뺀다.
#endif

	if (m_pTreeGroupDlg != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_already_exist_));
		return;
	}

	m_pTreeGroupDlg   = new CTreeGroupDlg;
	m_pTreeGroupDlg->Create(_LS(IDS_WG_TREEMENU_CTreeGroupDlg), _LS(IDS_WG_TREEMENU_Group), WS_VISIBLE|WS_CHILD,
		CRect(0, 0, 0, 0), &m_wndTab, 0x102, NULL);
	m_pTreeGroupDlg->EnableToolTips(TRUE); 
	SECTab* pTab3 = m_wndTab.AddTab(m_pTreeGroupDlg, _LS(IDS_WG_TREEMENU_Group));

	//추가된 탭이 Tab Control에 이름이 나타나게 하기(졸라 헤딩 했음)
	SECTabControlBase* pTabCtrl = (SECTabControlBase*)m_wndTab.GetTabControl();
	pTabCtrl->RecalcLayout();
	pTabCtrl->Invalidate();
}

void CBarTreeMenu::RemoveGroupTab()
{
//#ifdef _MDEMO
//  return; // *^^* 데모 버전에서 뺀다.
//#endif
#ifdef _MVIL
	return; // *^^* 빌라 버전에서 뺀다.
#endif

	if (m_pTreeGroupDlg == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	m_wndTab.RemoveTab(m_pTreeGroupDlg);
	if (m_pTreeGroupDlg->GetSafeHwnd() && IsWindow(m_pTreeGroupDlg->GetSafeHwnd()))
		m_pTreeGroupDlg->DestroyWindow();
	//delete m_pTreeGroupDlg;
	m_pTreeGroupDlg = 0;
}
*/

void CBarTreeMenu::AddGroupTab()
{
//#ifdef _MDEMO
//  return; // *^^* 데모 버전에서 뺀다.
//#endif
#ifdef _MVIL
	return; // *^^* 빌라 버전에서 뺀다.
#endif

	if (m_pTreeGroupCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_already_exist_));
		return;
	}

	m_pTreeGroupCtrl   = new CTreeGroupCtrl;

	if(m_pTreeGroupCtrl)
		m_pTreeGroupCtrl->SetBkGroundColor(m_TreeCtrlBkColor);  

	m_pTreeGroupCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT|TVS_EDITLABELS,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeGroupCtrl->EnableToolTips(TRUE);
	m_pTreeGroupCtrl->EnableFilterBar(TRUE, *m_pfilterBarOptions, FALSE);
	/*!@#$
	SECTab* pTab3 = m_wndTab.AddTab(m_pTreeGroupCtrl, _LS(IDS_WG_TREEMENU_Group)));
	ASSERT(pTab3);
	*/
	m_wndTab.AddTab(m_pTreeGroupCtrl, _LS(IDS_WG_TREEMENU_Group));

	//추가된 탭이 Tab Control에 이름이 나타나게 하기(졸라 헤딩 했음)
	/*!@#$
	SECTabControlBase* pTabCtrl = (SECTabControlBase*)m_wndTab.GetTabControl();
	pTabCtrl->RecalcLayout();
	pTabCtrl->Invalidate();
	*/


/*

	if (m_pTreeGroupDlg != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_already_exist_));
		return;
	}

	m_pTreeGroupDlg   = new CTreeGroupDlg;
	m_pTreeGroupDlg->Create(_LS(IDS_WG_TREEMENU_CTreeGroupDlg), _LS(IDS_WG_TREEMENU_Group), WS_VISIBLE|WS_CHILD,
		CRect(0, 0, 0, 0), &m_wndTab, 0x102, NULL);
	m_pTreeGroupDlg->EnableToolTips(TRUE); 
	SECTab* pTab3 = m_wndTab.AddTab(m_pTreeGroupDlg, _LS(IDS_WG_TREEMENU_Group)));

	//추가된 탭이 Tab Control에 이름이 나타나게 하기(졸라 헤딩 했음)
	SECTabControlBase* pTabCtrl = (SECTabControlBase*)m_wndTab.GetTabControl();
	pTabCtrl->RecalcLayout();
	pTabCtrl->Invalidate();
	*/
}

void CBarTreeMenu::RemoveGroupTab()
{
//#ifdef _MDEMO
//  return; // *^^* 데모 버전에서 뺀다.
//#endif
#ifdef _MVIL
	return; // *^^* 빌라 버전에서 뺀다.
#endif

	if (m_pTreeGroupCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_not_exist_));
		return;
	}

	m_wndTab.SetActiveTab(0);
	m_wndTab.ScrollToTab(0);
	
	m_wndTab.RemoveTab(m_pTreeGroupCtrl);
	if (m_pTreeGroupCtrl->GetSafeHwnd() && IsWindow(m_pTreeGroupCtrl->GetSafeHwnd()))
	{
		m_pTreeGroupCtrl->DestroyWindow();
		
	}
	//--------------------------------------------------
	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. 
	// Changkeun 
	delete m_pTreeGroupCtrl;
	m_pTreeGroupCtrl = 0;

	/*

	if (m_pTreeGroupDlg == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	m_wndTab.RemoveTab(m_pTreeGroupDlg);
	if (m_pTreeGroupDlg->GetSafeHwnd() && IsWindow(m_pTreeGroupDlg->GetSafeHwnd()))
		m_pTreeGroupDlg->DestroyWindow();
	//delete m_pTreeGroupDlg;
	m_pTreeGroupDlg = 0;

	if (m_pTreeGroupDlg == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Group_tab_not_exist_));
		return;
	}
	*/


}

void CBarTreeMenu::AddWorkTab()
{
	if (m_pTreeWorkCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
		return;
	}

	m_pTreeWorkCtrl = new CTreeWorkCtrl;

	if (m_pTreeWorkCtrl)
		m_pTreeWorkCtrl->SetBkGroundColor(m_TreeCtrlBkColor);

	m_pTreeWorkCtrl->Create(TVS_HASLINES | TVS_HASBUTTONS | WS_VISIBLE | WS_CHILD | TVS_SHOWSELALWAYS | TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeWorkCtrl->EnableToolTips(TRUE);
	m_pTreeWorkCtrl->EnableFilterBar(TRUE, *m_pfilterBarOptions, FALSE);

	/*!@#$
	SECTab* pTab4 = m_wndTab.AddTab(m_pTreeWorkCtrl, _LS(IDS_WG_TREEMENU_Works)));
	ASSERT(pTab4);
	*/
	m_wndTab.AddTab(m_pTreeWorkCtrl, _LS(IDS_WG_TREEMENU_Works));

	/*!@#$
	//추가된 탭이 Tab Control에 이름이 나타나게 하기(졸라 헤딩 했음)
	SECTabControlBase* pTabCtrl = (SECTabControlBase*)m_wndTab.GetTabControl();
	pTabCtrl->RecalcLayout();
	pTabCtrl->Invalidate();
	*/

}

void CBarTreeMenu::RemoveWorkTab()
{
	if (m_pTreeWorkCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	
	m_wndTab.RemoveTab(m_pTreeWorkCtrl);
	if (m_pTreeWorkCtrl->GetSafeHwnd() && IsWindow(m_pTreeWorkCtrl->GetSafeHwnd()))
	{
		m_pTreeWorkCtrl->DestroyWindow();
	}
	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
	delete m_pTreeWorkCtrl;
	m_pTreeWorkCtrl = 0;
}

void CBarTreeMenu::AddReportTab()
{
#if defined(_MGEN) || defined(_CIVIL)
//   if (m_pTreeReportCtrl != NULL)
//   {
//     TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
//     return;
//   }
// 
//   m_pTreeReportCtrl   = new CTreeReportCtrl;
	m_pTreeReportCtrl   = CTreeReportCtrlInstance::GetTreeReportCtrlInstance();
	
	if(m_pTreeReportCtrl)
		m_pTreeReportCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
	
	m_pTreeReportCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeReportCtrl->EnableToolTips(TRUE); 

	m_wndTab.AddTab(m_pTreeReportCtrl, _LS(IDS_TM_WORK_Report));
#endif
}

void CBarTreeMenu::RemoveReportTab()
{
#if defined(_MGEN) || defined(_CIVIL)
	if (m_pTreeReportCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);
	
	m_wndTab.RemoveTab(m_pTreeReportCtrl);
	if (m_pTreeReportCtrl->GetSafeHwnd() && IsWindow(m_pTreeReportCtrl->GetSafeHwnd()))
	{
		m_pTreeReportCtrl->DestroyWindow();

	}
	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
	//delete m_pTreeReportCtrl;
	//m_pTreeReportCtrl = 0;
#endif
}

void CBarTreeMenu::AddJUDTab()
{   
#if defined(_MGEN)
	//if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP)
	{
		if (m_pTreeJUDCtrl != NULL)
		{
			TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
			return;
		}
		
		m_pTreeJUDCtrl   = new CTreeJUDCtrl;
		
		if(m_pTreeJUDCtrl)
			m_pTreeJUDCtrl->SetBkGroundColor(m_TreeCtrlBkColor);
		
		m_pTreeJUDCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
			CRect(0, 0, 0, 0), &m_wndTab, 0x103);
		m_pTreeJUDCtrl->EnableToolTips(TRUE); 
		
		m_wndTab.AddTab(m_pTreeJUDCtrl, _LS(IDS_TREE_JUD));
	}
#endif
}

void CBarTreeMenu::RemoveJUDTab()
{ 
#if defined(_MGEN)
	//if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP)
	{
		if (m_pTreeJUDCtrl == NULL)
		{
			TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
			return;
		}
		
		m_wndTab.ActivateTab(0);
		m_wndTab.ScrollToTab(0);
		
		m_wndTab.RemoveTab(m_pTreeJUDCtrl);
		if (m_pTreeJUDCtrl->GetSafeHwnd() && IsWindow(m_pTreeJUDCtrl->GetSafeHwnd()))  
			m_pTreeJUDCtrl->DestroyWindow();    
		
		//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
		//--- Object를 Delete 하지 않아서 Memory Leak발생 
		//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
		delete m_pTreeJUDCtrl;
		m_pTreeJUDCtrl = 0;
	}  
#endif
}

void CBarTreeMenu::AddSeisTab()
{
	static BOOL bSeisEvalLockOption = CTestEnvMgr::IsTestEnvST(_LSX(SeisEvalLockOption), _LSX(yes));
	if (!bSeisEvalLockOption && !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_SEIS_EVAL) && !CDBDoc::GetDocPoint()->AllowCommand(D_OPTN_ID_SPD))
		return;

	if (!CProduct::IsSeisEval()) return;

	if (m_pTreeSeisCtrl != NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_already_exist_));
		return;
	}

	m_pTreeSeisCtrl = new CTreeSeisCtrl;

	if(m_pTreeSeisCtrl)
		m_pTreeSeisCtrl->SetBkGroundColor(m_TreeCtrlBkColor);

	m_pTreeSeisCtrl->Create(TVS_HASLINES|TVS_HASBUTTONS|WS_VISIBLE|WS_CHILD|TVS_SHOWSELALWAYS|TVS_LINESATROOT,
		CRect(0, 0, 0, 0), &m_wndTab, 0x103);
	m_pTreeSeisCtrl->EnableToolTips(TRUE); 

	m_wndTab.AddTab(m_pTreeSeisCtrl, _LS(IDS_WG_TREEMENU_Seismic));
}

void CBarTreeMenu::RemoveSeisTab()
{
	if (!CProduct::IsSeisEval()) return;

	if (m_pTreeSeisCtrl == NULL)
	{
		TRACE(_LS(IDS_WG_TREEMENU_Warning___Work_tab_not_exist_));
		return;
	}

	m_wndTab.ActivateTab(0);
	m_wndTab.ScrollToTab(0);

	m_wndTab.RemoveTab(m_pTreeSeisCtrl);
	if (m_pTreeSeisCtrl->GetSafeHwnd() && IsWindow(m_pTreeSeisCtrl->GetSafeHwnd()))  
		m_pTreeSeisCtrl->DestroyWindow();    

	//--- RemoveWorkTab 호출전에 윈도우가 삭제 되었을때 
	//--- Object를 Delete 하지 않아서 Memory Leak발생 
	//--- BCG로 바뀌면서 변경된 내용이 있나보다. Changkeun 
	delete m_pTreeSeisCtrl;
	m_pTreeSeisCtrl = 0;
}

// Tree Menu Tab 조정 (by J. B. Seon)
void CBarTreeMenu::_ShowTreeMenuTabByID(int nCurMode)
{
	int nTabIndex = 0;
	switch(nCurMode)
	{
	case __TMMODE_TREEMENU_MENU__:
	case __TMMODE_TREEMENU_TABLE__:
		nTabIndex = nCurMode-1; break;
	case __TMMODE_TREEMENU_GROUP__:
	case __TMMODE_TREEMENU_WORK__:
	case __TMMODE_TREEMENU_REPORT__:
	case __TMMODE_TREEMENU_JUD__:
	case __TMMODE_TREEMENU_SEIS__:
		if (m_wndTab.GetTabCount() < nCurMode) 
		{
			TRACE(_LS(IDS_WG_TREEMENU_Error___Work_tab_is_requested_but_n));
			return;
		}
		nTabIndex = nCurMode-1; break;
	default:
		TRACE(_LS(IDS_WG_TREEMENU_Error___undefined_tree_menu_tab_));
		return;
	}
	m_wndTab.SetActiveTab(nTabIndex);
	m_wndTab.ScrollToTab(nTabIndex);
}

int CBarTreeMenu::RefreshCurTreeMenuMode(int nNewMode)
{
	int nIndex = 0;
	if ( m_nCurMode == __TMMODE_TREEMENU__
		|| m_nCurMode == __TMMODE_TREEMENU_MENU__
		|| m_nCurMode == __TMMODE_TREEMENU_TABLE__
		|| m_nCurMode == __TMMODE_TREEMENU_GROUP__
		|| m_nCurMode == __TMMODE_TREEMENU_WORK__
		|| m_nCurMode == __TMMODE_TREEMENU_REPORT__
		|| m_nCurMode == __TMMODE_TREEMENU_JUD__
		|| m_nCurMode == __TMMODE_TREEMENU_SEIS__
		 )
	{
		if (m_wndTab.GetActiveTab(nIndex))
		{
#if defined (_CIVIL)
			if      (nIndex == 0) m_nCurMode = __TMMODE_TREEMENU_TABLE__  ;
			else if (nIndex == 1) m_nCurMode = __TMMODE_TREEMENU_WORK__   ;
			else if (nIndex == 2) m_nCurMode = __TMMODE_TREEMENU_GROUP__  ;
			else if (nIndex == 3) m_nCurMode = __TMMODE_TREEMENU_REPORT__ ;
			//else if (nIndex == 4) m_nCurMode = __TMMODE_TREEMENU_JUD__    ;
			//else if (nIndex == 5) m_nCurMode = __TMMODE_TREEMENU_MENU__   ;
			else
			{
				TRACE(_LS(IDS_WG_TREEMENU_Error___Undefined_treemenu_tab_));
				m_nCurMode = __TMMODE_TREEMENU_MENU__;
			}
#else
			if      (nIndex == 0) m_nCurMode = __TMMODE_TREEMENU_MENU__   ;
			else if (nIndex == 1) m_nCurMode = __TMMODE_TREEMENU_TABLE__  ;
			else if (nIndex == 2) m_nCurMode = __TMMODE_TREEMENU_GROUP__  ;
			else if (nIndex == 3) m_nCurMode = __TMMODE_TREEMENU_WORK__   ;
			else if (nIndex == 4) m_nCurMode = __TMMODE_TREEMENU_REPORT__ ;
			else if (nIndex == 5) m_nCurMode = __TMMODE_TREEMENU_JUD__    ;
			else if (nIndex == 6) m_nCurMode = __TMMODE_TREEMENU_SEIS__   ;
			else
			{
				TRACE(_LS(IDS_WG_TREEMENU_Error___Undefined_treemenu_tab_));
				m_nCurMode = __TMMODE_TREEMENU_MENU__;
			}
#endif
		}
	}

	if (nNewMode == __TMMODE_TREEMENU__)
	{
		if (m_wndTab.GetActiveTab(nIndex))
		{
#if defined (_CIVIL)
			if      (nIndex == 0) nNewMode = __TMMODE_TREEMENU_TABLE__  ;
			else if (nIndex == 1) nNewMode = __TMMODE_TREEMENU_WORK__   ;
			else if (nIndex == 2) nNewMode = __TMMODE_TREEMENU_GROUP__  ;
			else if (nIndex == 3) nNewMode = __TMMODE_TREEMENU_REPORT__ ;
			//else if (nIndex == 4) nNewMode = __TMMODE_TREEMENU_JUD__    ;
			//else if (nIndex == 5) nNewMode = __TMMODE_TREEMENU_MENU__   ;
			else
			{
				TRACE(_LS(IDS_WG_TREEMENU_Error___Undefined_treemenu_tab_));
				nNewMode = __TMMODE_TREEMENU_MENU__;
			}
#else
			if      (nIndex == 0) nNewMode = __TMMODE_TREEMENU_MENU__   ;
			else if (nIndex == 1) nNewMode = __TMMODE_TREEMENU_TABLE__  ;
			else if (nIndex == 2) nNewMode = __TMMODE_TREEMENU_GROUP__  ;
			else if (nIndex == 3) nNewMode = __TMMODE_TREEMENU_WORK__   ;
			else if (nIndex == 4) nNewMode = __TMMODE_TREEMENU_REPORT__ ;
			else if (nIndex == 5) nNewMode = __TMMODE_TREEMENU_JUD__    ;
			else if (nIndex == 6) nNewMode = __TMMODE_TREEMENU_SEIS__   ;
			else
			{
				TRACE(_LS(IDS_WG_TREEMENU_Error___Undefined_treemenu_tab_));
				nNewMode = __TMMODE_TREEMENU_MENU__;
			}
#endif
		}
	}
	return nNewMode;
}

void CBarTreeMenu::CreateStructSttlTab()
{
	if(!::IsWindow(m_wndSettlementTab.GetSafeHwnd()))
	{
		m_wndSettlementTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		m_wndSettlementTab.AddTab(RUNTIME_CLASS(CStructSttlDlg),_LS(IDS_WG_TREEMENU_Settlement_Analysis_Data),NULL,CStructSttlDlg::IDD);
	}
	m_wndSettlementTab.SetActiveTab(0);
}

void CBarTreeMenu::CreateStructHydrTab()
{
	if(!::IsWindow(m_wndHydrationTab.GetSafeHwnd()))
	{
		m_wndHydrationTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		m_wndHydrationTab.AddTab(RUNTIME_CLASS(CStructHydrDlg),_LS(IDS_WG_TREEMENU_Hydration_Heat_Analysis_Data),NULL,CStructHydrDlg::IDD);
	}
	m_wndHydrationTab.SetActiveTab(0);
}


void CBarTreeMenu::CreateStructTimeDepTab()
{
	if(!::IsWindow(m_wndTimeDepTab.GetSafeHwnd()))
	{
		m_wndTimeDepTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		m_wndTimeDepTab.AddTab(RUNTIME_CLASS(CStructTimeDepMatLinkDlg),_LS(IDS_WG_TREEMENU_ADD3_Hydration_Time_Dependent_Mater_MSG1),NULL,CStructTimeDepMatLinkDlg::IDD);
		CurString = _LS(IDS_WG_TREEMENU_ADD3_Hydration_Time_Dependent_Mater_MSG1);
	}
	m_wndTimeDepTab.SetActiveTab(0);
}

void CBarTreeMenu::CreateStructTHighTemperatureTab()
{
	if(!::IsWindow(m_wndHighTemperatureTab.GetSafeHwnd()))
	{
		m_wndHighTemperatureTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		m_wndHighTemperatureTab.AddTab(RUNTIME_CLASS(CStructHighTemperatureMatLinkDlg),_LS(IDS_WG_TREEMENU_ADD3_High_Temperature_Mater_MSG1),NULL,CStructHighTemperatureMatLinkDlg::IDD);
		CurString = _LS(IDS_WG_TREEMENU_ADD3_High_Temperature_Mater_MSG1);
	}
	m_wndHighTemperatureTab.SetActiveTab(0);
}

void CBarTreeMenu::CreateSeisCvlTab()
{
	if(!::IsWindow(m_wndSeisCvlTab.GetSafeHwnd()))
	{
		m_wndSeisCvlTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		m_wndSeisCvlTab.AddTab(RUNTIME_CLASS(CSeisCvlDlg), _LS(IDS_WG_TREEMENU_SEIS_CVL_SEISMIC), NULL, CSeisCvlDlg::IDD);
	}
	m_wndSeisCvlTab.SetActiveTab(0);
}

void CBarTreeMenu::CreateSeisCvlMiscTab()
{
	if(!::IsWindow(m_wndSeisCvlMiscTab.GetSafeHwnd()))
	{
		m_wndSeisCvlMiscTab.Create(this,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		m_wndSeisCvlMiscTab.AddTab(RUNTIME_CLASS(CSeisCvlMiscDlg), _LS(IDS_WG_TREEMENU_SEIS_CVL_MISC), NULL, CSeisCvlMiscDlg::IDD);
	}
	m_wndSeisCvlMiscTab.SetActiveTab(0);
}

void CBarTreeMenu::CreateSeisSetTargetTab()
{
	if (!::IsWindow(m_wndSeisSetTargetTab.GetSafeHwnd()))
	{
		m_wndSeisSetTargetTab.Create(this, WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP, 1200);
		m_wndSeisSetTargetTab.AddTab(RUNTIME_CLASS(CSeisCvlJpSetTargetDlg), _LS(IDS_WG_TREEMENU_SEIS_SET_TARGET), NULL, CSeisCvlJpSetTargetDlg::IDD);
	}
	m_wndSeisSetTargetTab.SetActiveTab(0);
}

void CBarTreeMenu::CreateSeisSetInvestTab()
{
	if (!::IsWindow(m_wndSeisSetInvestTab.GetSafeHwnd()))
	{
		m_wndSeisSetInvestTab.Create(this, WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP, 1200);
		m_wndSeisSetInvestTab.AddTab(RUNTIME_CLASS(CSeisCvlJpSetInvestDlg), _LS(IDS_WG_TREEMENU_SEIS_SET_INVEST), NULL, CSeisCvlJpSetInvestDlg::IDD);
	}
	m_wndSeisSetInvestTab.SetActiveTab(0);
}

void CBarTreeMenu::ToggleWorktab()
{
	if (GetMode() == __TMMODE_TREEMENU_WORK__)  // Previous로
	{
		if (m_nPrevMode == __TMMODE_TREEMENU_WORK__) return;
		CTreeMenuBarBase::SetModeST(m_nPrevMode);
	}
	else  // Worktab으로
	{
		int nCurMode = GetMode();
		SavePrevMode(nCurMode);
		CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU_WORK__);
	}
}

void CBarTreeMenu::SetBkGroundColor(COLORREF BkColor)
{
	m_TreeCtrlBkColor = BkColor;
	
#if !defined(_CIVIL)
	m_pTreeMenuCtrl->SetBkGroundColor(BkColor);
#endif
	m_pTreeTableCtrl->SetBkGroundColor(BkColor);
	if(m_pTreeWorkCtrl)
		m_pTreeWorkCtrl->SetBkGroundColor(BkColor);
	
	if(m_pTreeGroupCtrl)
		m_pTreeGroupCtrl->SetBkGroundColor(BkColor);

#if defined(_MGEN) || defined(_CIVIL)
	if(m_pTreeReportCtrl)
		m_pTreeReportCtrl->SetBkGroundColor(BkColor);
#endif
	
#if defined(_MGEN)
	if(CProduct::GetMovingType()==D_PRODUCT_MOVING_JP)
	{
		if(m_pTreeJUDCtrl)
			m_pTreeJUDCtrl->SetBkGroundColor(BkColor);
	}
#endif

	if(CProduct::IsSeisEval() && m_pTreeSeisCtrl)
	{
		m_pTreeSeisCtrl->SetBkGroundColor(BkColor);
	}
}

//!@#$ MNET:XXXX 20051028 LBI : Floating 항목을 없앰... 
BOOL CBarTreeMenu::OnShowControlBarMenu (CPoint point)
{
	if (g_pContextMenuManager == NULL)
	{
		return FALSE;
	}

	const UINT idAutoHide = (UINT)-104;
	const UINT idHide = (UINT)-105;

	CMenu menu;
	menu.CreatePopupMenu();

	{
		CString strItem;

		strItem.Format(_LS(IDS_TM_AUTOHIDE));
		menu.AppendMenu(MF_STRING, idAutoHide, strItem);

		strItem.Format(_LS(IDS_TM_HIDE));
		menu.AppendMenu(MF_STRING, idHide, strItem);
	}

	CBCGPBaseTabbedBar* pBar = GetParentTabbedBar();
	if(pBar == NULL) return FALSE;
	if (pBar->IsAutoHideMode())
	{
		menu.CheckMenuItem(idAutoHide, MF_CHECKED);
	}

	int nMenuResult = g_pContextMenuManager->TrackPopupMenu(
		menu, point.x, point.y, this);

	switch (nMenuResult)
	{
	case idAutoHide:
		pBar->ToggleAutoHide();
		break;

	case idHide:
		BOOL bIsAutoHide = pBar->IsAutoHideMode();
		pBar->ShowControlBar(!IsVisible(), FALSE, bIsAutoHide);
		break;
	}

	return TRUE;
}

void CBarTreeMenu::OnAfterDock(CBCGPBaseControlBar* pBar, LPCRECT lpRect, BCGP_DOCK_METHOD dockMethod)
{
	CTreeMenuBarBase::OnAfterDock(pBar, lpRect, dockMethod);

	ForceTabLocationToTop();
}

// Docking Bar의 Close 버튼을 눌렀을 때 customize...
void CBarTreeMenu::OnPressCloseButton ()
{
	CBCGPDockingControlBar::OnPressCloseButton();
}

CWnd* CBarTreeMenu::GetCurTreeWnd()
{
	if (m_wndTab.GetSafeHwnd() == NULL)
		return nullptr;

	return m_wndTab.GetActiveWnd();
}

void CBarTreeMenu::CreateWindPressure(CWnd* pParent, mit::frx::MBaseTabWnd* pTabWnd, int nWhich)
{
	if(!::IsWindow(pTabWnd->GetSafeHwnd()))
	{
		pTabWnd->Create(pParent,WS_CHILD | WS_VISIBLE | TWS_TABS_ON_TOP,1200);
		switch(nWhich)
		{
		case __TMMODE_CMD_WIND_PRESSURE_AREA__:
		case __TMMODE_CMD_WIND_PRESSURE_BEAM__:
		case __TMMODE_CMD_WIND_PRESSURE_NODAL__:
		case __TMMODE_CMD_WIND_PRESSURE_FUNC__:
			pTabWnd->AddTab(RUNTIME_CLASS(CStldWindPressureRootDlg),_LS(IDS_CMD_WINDP_WINDPRESSURE));
			break;      
		}
	}
	pTabWnd->SetActiveTab(0);

	switch(nWhich)
	{
	case __TMMODE_CMD_WIND_PRESSURE_AREA__:
	case __TMMODE_CMD_WIND_PRESSURE_BEAM__:
	case __TMMODE_CMD_WIND_PRESSURE_NODAL__:
	case __TMMODE_CMD_WIND_PRESSURE_FUNC__:
		{
			CWnd * pWnd = NULL;
			pTabWnd->GetActiveTab(pWnd);
			if(pWnd) ((CStldWindPressureRootDlg*)pWnd)->ShowChildDlgByID(nWhich);
			break;
		}
	} 
	
}