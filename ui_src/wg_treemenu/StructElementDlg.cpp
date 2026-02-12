// StructElementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StructElementDlg.h"

#include "..\wg_base\TreeMenuMode.h"

// Dialog추가시 3곳 수정 : '(수정)'이라고 코멘트 친 곳
// 주의 : 순서 맞추어 줄 것
// #ifdef ???? 많아지면 StructBndrDlg나 StructStldDlg처럼 변경할 것

// (수정)
#include "ElemCreateDlg.h"
#include "ElemCreateConvLineDlg.h"
#include "ElemCurveDlg.h"
#include "ElemRotateDlg.h"
#include "ElemExtrudeDlg2.h"
#include "ElemTranslateDlg.h"
#include "ElemMirrorDlg.h"
#include "ElemDivideDlg.h"
#include "ElemMergeDlg.h"
#include "ElemIntersectDlg.h"
#include "ElemDeleteDlg.h"
#include "ElemCompactDlg.h"
#include "ElemRenumberDlg.h"      
#include "ElemParamChangeDlg.h"
#include "ElemDepMatChange.h"
#include "ElemPlateReduFactorDlg.h"
#include "TpscGroupDlg.h"
#include "ElemCreateWallOpenDlg.h"
#include "ElemStiffScaleFactorDlg.h"
#include "ElemWallReduFactorDlg.h"
#include "ElemEccenDlg.h"
#include "BndrEwsfDlg.h"
#include "SectForResultantForceDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\mit_frx\MBarBaseDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStructElementDlg

IMPLEMENT_DYNCREATE(CStructElementDlg, MChildFormView)

CStructElementDlg::CStructElementDlg()
	: MChildFormView(CStructElementDlg::IDD)
{
	//{{AFX_DATA_INIT(CStructElementDlg)
	//}}AFX_DATA_INIT

	struct T_COMBO_ITEM
	{
		int nMask;
		CString strItem;
	};
// (수정) // redesigned by J.B.Seon 2002.05.22
#if defined(_CIVIL)
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Create_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Translate_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Divide_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Merge_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Delete_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Rotate_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Mirror_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Renumbering) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Compact_Element_Numbers) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Extrude_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Intersect_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Create_Line_Elements_on_Curve) },
		{ D_PROD_CVL                       , _LS(IDS_WG_TREEMENU_Eccentric_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Create_Conv_Line_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Change_Element_Parameters) },
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_WG_TREEMENU_Change_Element_Dependent_Mat) },
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_WG_TREEMENU_TaperedSection_Group) },
		{ D_PROD_CVL						   , _LS(IDS_WG_TREEMENU_Effective_Width_Scale_Factor)},
		{ D_PROD_CVL                           , _LS(IDS_WG_TREEMENU_Section_for_Resultant_Forces) },   // if (CProduct::GetTestEnvValue(_ULS(TestLDFS)) == _ULS(yes))  bEnableRForc = TRUE;
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_TM_WORKTREE_Element_Stiffness_Scale_Factor) },
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_WG_TREEMENU_Plate_Shear_Stiffness_Reduction_Factor) },
		{ D_PROD_GEN                           , _LS(IDS_WG_TREEMENU_Create_Wall_Open_Elements) },
		{ D_PROD_GEN                           , _LS(IDS_WG_TREEMENU_Wall_Shear_Stiffness_Reduction_Factor) },
	};
#else
	T_COMBO_ITEM aItem[] = {
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Create_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Create_Line_Elements_on_Curve) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Create_Conv_Line_Elements) },
		{ D_PROD_GEN                           , _LS(IDS_WG_TREEMENU_Create_Wall_Open_Elements) },
		{ D_PROD_CVL                           , _LS(IDS_WG_TREEMENU_Eccentric_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Delete_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Translate_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Rotate_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Extrude_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Mirror_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Divide_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Merge_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Intersect_Elements) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Change_Element_Parameters) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Compact_Element_Numbers) },
		{ D_PROD_GEN + D_PROD_CVL + D_PROD_TOW , _LS(IDS_WG_TREEMENU_Renumbering) + _T(" ") + _LS(IDS_WG_TREEMENU_Element) },
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_WG_TREEMENU_Change_Element_Dependent_Mat) },
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_WG_TREEMENU_TaperedSection_Group) },
		{ D_PROD_CVL                           , _LS(IDS_WG_TREEMENU_Section_for_Resultant_Forces) },   // if (CProduct::GetTestEnvValue(_ULS(TestLDFS)) == _ULS(yes))  bEnableRForc = TRUE;
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_TM_WORKTREE_Element_Stiffness_Scale_Factor) },
		{ D_PROD_GEN                           , _LS(IDS_WG_TREEMENU_Wall_Shear_Stiffness_Reduction_Factor) },
		{ D_PROD_GEN + D_PROD_CVL              , _LS(IDS_WG_TREEMENU_Plate_Shear_Stiffness_Reduction_Factor) },
	};
#endif
	int nMask = 0;
#if defined(_MGEN)
	nMask = D_PROD_GEN;
#elif defined(_CIVIL)
	nMask = D_PROD_CVL;
#elif defined(_TOWER)
	nMask = D_PROD_TOW;
#else
	#error _T("Unknown Product Definition!")
#endif

	int i, nCount;
	nCount = sizeof(aItem)/sizeof(T_COMBO_ITEM);
	for(i=0; i < nCount; i++)
	{
		if ((nMask & aItem[i].nMask) == 0) continue;
		m_ComboMenuStr.Add(aItem[i].strItem);
	}

	CreateChildDlgs();  
}

CStructElementDlg::~CStructElementDlg()
{
	DeleteChildDlgs();
}

void CStructElementDlg::DoDataExchange(CDataExchange* pDX)
{
	MChildFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStructElementDlg)
	DDX_Control(pDX, IDC_TM_ELEM_TABLE_BTN, m_wndTableBtn);
	DDX_Control(pDX, IDC_START_NODE_NUM, m_EditNode);
	DDX_Control(pDX, IDC_START_ELEM_NUM, m_EditElem);
	DDX_Control(pDX, IDC_TM_PLACEHOLDER, m_wndPlaceHolder);
	DDX_Control(pDX, IDC_ELEM_MENU_COMBO, m_ComboMenu);
	//DDX_Control(pDX, IDC_TM_START_STC, m_gbStartNum);
	DDX_Control(pDX, IDC_TM_STATIC1, m_stNodeNum);
	DDX_Control(pDX, IDC_TM_STATIC2, m_stElemNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStructElementDlg, MChildFormView)
	//{{AFX_MSG_MAP(CStructElementDlg)
	ON_CBN_SELCHANGE(IDC_ELEM_MENU_COMBO, OnComboMenuSelect)
	ON_BN_CLICKED(IDC_VIEW_START_NODE_NUM, OnViewStartNodeNum)	
	ON_BN_CLICKED(IDC_VIEW_START_ELEM_NUM, OnViewStartElemNum)
	ON_BN_CLICKED(IDC_TM_ELEM_TABLE_BTN, OnTmTableBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStructElementDlg diagnostics

#ifdef _DEBUG
void CStructElementDlg::AssertValid() const
{
	MChildFormView::AssertValid();
}

void CStructElementDlg::Dump(CDumpContext& dc) const
{
	MChildFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStructElementDlg message handlers

void CStructElementDlg::OnViewStartNodeNum() 
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->NodeNumberDlg();
}

void CStructElementDlg::OnViewStartElemNum() 
{
	((CDBDoc*)CTreeMenuBarBase::GetDocST())->ElemNumberDlg();
}

void CStructElementDlg::OnInitialUpdate() 
{
	MInitCombo initCombo;
	MChildFormView::OnInitialUpdate();
	InitContent();
	InitComboMenu();
	m_wndPlaceHolder.GetWindowRect(&m_RectPlaceHolder);
	CMouseEdit::ImAssocWindowST();
	//m_gbStartNum.InitControl(this, CStructElementDlg::IDD, IDC_TM_START_STC,TRUE,FALSE,3,4);
}

bool CStructElementDlg::ShowApplyBtn()
{
	if ( m_nCurChildDlg < 0 || m_ChildDlgs.GetSize() - 1 < m_nCurChildDlg )
	{
		ASSERT(0);  return true;
	}

	switch ( m_DlgIDs.GetAt(m_nCurChildDlg) )
	{
	case __TMMODE_STRUCT_TPSC_GROUP__:
	case __TMMODE_STRUCT_SECT_FOR_RFOC__:
		return false;
	default:
		return true;
	}
}

// (수정)
#if defined(_CIVIL)
void CStructElementDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1;

	m_ChildDlgs.Add(new CElemCreateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CREATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemTranslateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_TRANSLATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemDivideDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_DIVIDE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemMergeDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_MERGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemDeleteDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_DELETE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemRotateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_ROTATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemMirrorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_MIRROR__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemRenumberDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_RENUMBER__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemCompactDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_COMPACT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemExtrudeDlg2());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_EXTRUDE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemIntersectDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_INTSECT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemCurveDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CURVE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemEccenDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_ECCEN__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);
	// Added by Water Tree .. Prepare for WGen ver 4.1.3 (2000.7.24)
	// 다시 메뉴를 살릴때는 OnTabSelect 와 OnTabUnSelect 의 Dialog 번호를 수정해주어야 한다.(6->7)

	m_ChildDlgs.Add(new CElemCreateConvLineDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CONV_LINE_CREATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemParamChangeDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CHANGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemDepMatChange());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_DEP_MAT_CHANGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_EDMP);

	m_ChildDlgs.Add(new CTpscGroupDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_TPSC_GROUP__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CBndrEwsfDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_BNDR_EWSF__);
	m_TableIDs.Add(ID_QUERY_STRUCT_EWSF);

	m_ChildDlgs.Add(new CSectForResultantForceDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_SECT_FOR_RFOC__);
	m_TableIDs.Add(0);

	m_ChildDlgs.Add(new CElemStiffScaleFactorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_ESSF__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ESSF);

	m_ChildDlgs.Add(new CElemPlateReduFactorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_PSSF__);
	m_TableIDs.Add(ID_QUERY_STRUCT_PSSF);
}
#else
void CStructElementDlg::CreateChildDlgs()
{
	m_nCurChildDlg = -1;

	m_ChildDlgs.Add(new CElemCreateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CREATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemCurveDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CURVE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	// Added by Water Tree .. Prepare for WGen ver 4.1.3 (2000.7.24)
	// 다시 메뉴를 살릴때는 OnTabSelect 와 OnTabUnSelect 의 Dialog 번호를 수정해주어야 한다.(6->7)

	m_ChildDlgs.Add(new CElemCreateConvLineDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CONV_LINE_CREATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

#if defined(_MGEN)
	m_ChildDlgs.Add(new CElemCreateWallOpenDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_WALL_OPEN_CREATE__);
	m_TableIDs.Add(0);
#endif

#if defined(_CIVIL)
	m_ChildDlgs.Add(new CElemEccenDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_ECCEN__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);
#endif

	m_ChildDlgs.Add(new CElemDeleteDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_DELETE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemTranslateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_TRANSLATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemRotateDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_ROTATE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	/*  m_ChildDlgs.Add(new CElemExtrudeDlg());
		m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_EXTRUDE__);
		m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);  */  // Added by BugBoy Extrude Element에 Project관련 그림 추가.
		// CElemExtrudeDlg2가 대신 사용됨.

	m_ChildDlgs.Add(new CElemExtrudeDlg2());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_EXTRUDE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemMirrorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_MIRROR__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemDivideDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_DIVIDE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemMergeDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_MERGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemIntersectDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_INTSECT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemParamChangeDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_CHANGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemCompactDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_COMPACT__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemRenumberDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_RENUMBER__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ELEM);

	m_ChildDlgs.Add(new CElemDepMatChange());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_DEP_MAT_CHANGE__);
	m_TableIDs.Add(ID_QUERY_STRUCT_EDMP);

	m_ChildDlgs.Add(new CTpscGroupDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_TPSC_GROUP__);
	m_TableIDs.Add(0);

#if defined(_CIVIL)
	//   BOOL bEnableRForc = FALSE;
	//   if (CProduct::GetTestEnvValue(_ULS(TestLDFS)) == _ULS(yes))  bEnableRForc = TRUE;
	//   if (bEnableRForc)
	//   {
	//     m_ChildDlgs.Add(new CSectForResultantForceDlg());
	//     m_DlgIDs.Add(__TMMODE_STRUCT_SECT_FOR_RFOC__);
	//     m_TableIDs.Add(0);            
	//   }
	m_ChildDlgs.Add(new CSectForResultantForceDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_SECT_FOR_RFOC__);
	m_TableIDs.Add(0);
#endif

#if defined(_MGEN)
	m_ChildDlgs.Add(new CElemStiffScaleFactorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_ESSF__);
	m_TableIDs.Add(ID_QUERY_STRUCT_ESSF);

	m_ChildDlgs.Add(new CElemWallReduFactorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_WSSF__);
	m_TableIDs.Add(ID_QUERY_STRUCT_WSSF);
#endif

	m_ChildDlgs.Add(new CElemPlateReduFactorDlg());
	m_DlgIDs.Add(__TMMODE_STRUCT_ELEM_PSSF__);
	m_TableIDs.Add(ID_QUERY_STRUCT_PSSF);
}
#endif


void CStructElementDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_ComboMenu.GetDC();
	for (int i=0;i < m_ComboMenu.GetCount();i++)
	{
		m_ComboMenu.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_ComboMenu.ReleaseDC(pDC);

//  dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2*::GetSystemMetrics(SM_CXEDGE);

	if (m_ComboMenu.GetDroppedWidth() < dx)
	{
		m_ComboMenu.SetDroppedWidth(dx);
		ASSERT(m_ComboMenu.GetDroppedWidth() == dx);
	}

	//m_ComboMenu.SetItemHeight(-1, 28);
}

void CStructElementDlg::InitComboMenu()
{
	int wHeight;
	wHeight = (m_ComboMenuStr.GetSize()+1) * GetHeightFromFont();
	
	CRect rectWnd;
	m_ComboMenu.GetWindowRect(rectWnd);
	m_ComboMenu.SetWindowPos(NULL, 0, 0,rectWnd.Width(),wHeight,
				         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	for( int i=0;  i<m_ComboMenuStr.GetSize(); ++i )
	{
		m_ComboMenu.AddString(m_ComboMenuStr[i]);
	}
	m_ComboMenu.SetCurSel(0);

	AdjustComboListBox();
}

// Combo Menu선택에 따른 상황반영을 행한다. 
void CStructElementDlg::OnComboMenuSelect() 
{
	// TODO: Add your control notification handler code here
	int nCurI = m_ComboMenu.GetCurSel();
	
	//I_GENModelBase::SetModelingModeST(IG_STATE_CUROP_END);
	I_GENModelBase::SetModelingModeST(IG_STATE_INITALL);

	// ShowCurChildDlg를 직접 부르면 CBarTreeMenu의 m_nCurMode가 Update
	// 되지 않아 Menu에서 Select해도 해당 Dialog가 뜨지 않는 경우가 발생
	CTreeMenuBarBase::SetModeST(m_DlgIDs.GetAt(nCurI));
	//ShowCurChildDlg(nCurI);	
	//14
	//CWnd * pWnd;
	if( m_DlgIDs[nCurI]== __TMMODE_STRUCT_TPSC_GROUP__) 
	{
		//pWnd = GetDlgItem(IDC_TM_EXECUTE);
		//pWnd->ShowWindow(SW_HIDE);
	}
	else if( m_DlgIDs[nCurI]== __TMMODE_STRUCT_SECT_FOR_RFOC__) 
	{
		//pWnd = GetDlgItem(IDC_TM_EXECUTE);
		//pWnd->ShowWindow(SW_HIDE);
	}
	else if( m_DlgIDs[nCurI] == __TMMODE_STRUCT_ELEM_EXTRUDE__ || m_DlgIDs[nCurI] == __TMMODE_STRUCT_ELEM_RENUMBER__)
	{
	}
	else
	{
		//pWnd = GetDlgItem(IDC_TM_EXECUTE);
		//pWnd->ShowWindow(SW_SHOW);
	}
	
		
	/*
	CString str;
	m_ComboMenu.GetLBText(nCurI,str);
	GSaveHistoryNF(str);
	*/
}

void CStructElementDlg::ShowExecuteButton(UINT nShowHide)
{
	CWnd * pWnd;
	pWnd = GetDlgItem(IDC_TM_EXECUTE);
	if(pWnd)
		pWnd->ShowWindow(nShowHide);
}

void CStructElementDlg::OnTmExecute() 
{
	if(m_nCurChildDlg < m_ChildDlgs.GetSize())
		m_ChildDlgs[m_nCurChildDlg]->Execute();
}

void CStructElementDlg::OnTmClose() 
{
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode(FALSE);
	}
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);  	
}

void CStructElementDlg::OnTmExecAndClose(BOOL bExecute)
{
	if (bExecute)
		OnTmExecute();
	else
		OnTmClose();
}

int  CStructElementDlg::_GetHeightFromFont()
{
	 CFont * pNewFont  = GetFont();
	 CFont *pSysFont,*pOldFont,TempFont;
	 CDC   *pCdc;
	 TEXTMETRIC  tmNew,tmSys;
	 int     nTemp,nCorrectVal;
 
	 //get the DC for the edit control
	 pCdc = GetDC();
 
	 //get the metrics for the system font
	 pSysFont = TempFont.FromHandle(((HFONT)GetStockObject(SYSTEM_FONT)));
	 pOldFont  = pCdc->SelectObject( pSysFont);
	 pCdc->GetTextMetrics(&tmSys);
 
	 //get the metrics for the new font
	 pCdc->SelectObject(pNewFont);
	 pCdc->GetTextMetrics(&tmNew);
 
	 //select the original font back into the DC and release the DC
	 pCdc->SelectObject(pOldFont);
	 ReleaseDC(pCdc);
	 
	 nCorrectVal = (min(tmNew.tmHeight, tmSys.tmHeight)/2);
	 //calculate the new height for the edit control
	 nTemp = tmNew.tmHeight + nCorrectVal;
	 //nTemp = tmNew.tmHeight + tmNew.tmInternalLeading;
	 return nTemp;
}

BOOL CStructElementDlg::ShowChildDlgByID(int nID)
{
	if(nID == __TMMODE_STRUCT_ELEM__)
	{
		ShowCurChildDlg(0);  
		m_ComboMenu.SetCurSel(0);
		return TRUE;
	}

	int nIDCount = m_DlgIDs.GetSize();
	for(int i = 0 ; i < nIDCount ; i++)
	{
		if(nID == m_DlgIDs[i])
		{
			ShowCurChildDlg(i);  
			m_ComboMenu.SetCurSel(i);
			if (m_TableIDs[i] > 0) m_wndTableBtn.EnableWindow(TRUE);
			else m_wndTableBtn.EnableWindow(FALSE);
			return TRUE;
		}
	}
	return FALSE;
}

void CStructElementDlg::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex)
		return;
		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1 )
	{
		m_nCurChildDlg = nDlgIndex;
		return;
	}
	
	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();
		
	m_ChildDlgs[nDlgIndex]->CreateInit(&m_wndPlaceHolder,0,0);
	//m_ChildDlgs[nDlgIndex]->CreateInit(this,0,0);
	m_nCurChildDlg = nDlgIndex;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...
	CRect DlgRect,ParentRect,PHRect;
	
	GetClientRect(&ParentRect);
	m_ChildDlgs[nDlgIndex]->GetWindowRect(&DlgRect);
	//m_ChildDlgs[nDlgIndex]->SetExecButton(&m_wndExecute);
	ScreenToClient(&DlgRect);
	
	int Sx,Sy,Wx,Wy;
	Sx = (ParentRect.Width() - DlgRect.Width()) / 2 ;
	Sy = DlgRect.top;

	Wx = DlgRect.Width();
	Wy = DlgRect.Height();
	
	m_wndPlaceHolder.SetWindowPos(NULL, Sx,Sy,Wx,Wy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	m_wndPlaceHolder.GetWindowRect(&PHRect);

	ScreenToClient(&PHRect);

	CRect CBRect,EBRect;
	int SCX,SEX,BY,CW,EW;
	
	if( m_DlgIDs[nDlgIndex] == __TMMODE_STRUCT_ELEM_EXTRUDE__ || m_DlgIDs[nDlgIndex] == __TMMODE_STRUCT_ELEM_RENUMBER__)
	{
	}
	else
	{
		if( m_DlgIDs[nDlgIndex] == __TMMODE_STRUCT_TPSC_GROUP__) 
		{  
		}
		else if( m_DlgIDs[nDlgIndex] == __TMMODE_STRUCT_SECT_FOR_RFOC__) 
		{  
		}
		else
		{
		}

		BY = Sy+Wy+5;
		CW = CBRect.Width();
		EW = EBRect.Width();
		
#ifdef _RUS    
		SEX = PHRect.right - CW - EW - 2;
		SCX = SEX + CW + 15;
#else
		SEX = PHRect.right - CW - EW - 10;
		SCX = SEX + CW + 5;
#endif

	}

	/////////////////////////////////////////////////////////////
	// Scroll Size를 조정한다. 
	CSize ScrSize,TSize;
	ScrSize = GetTotalSize();
	
	TSize.cx = PHRect.Width()  - m_RectPlaceHolder.Width();  
	TSize.cy = PHRect.Height() - m_RectPlaceHolder.Height();
 
	ScrSize += TSize;
		
	m_RectPlaceHolder = PHRect;

	SetScrollSizes(MM_TEXT,ScrSize);

	//__TMMODE_STRUCT_ELEM_EXTRUDE__

		// 예외처리. Apply를 숨겨야하는 경우
	// Child Dlg 가 여러개인 경우
	mit::frx::MBarBaseDlg* pParent = (mit::frx::MBarBaseDlg*)GetParent();
	pParent->SetApplyBtnShowHide(ShowApplyBtn() ? SW_SHOW : SW_HIDE);
}

void CStructElementDlg::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}


void CStructElementDlg::InitContent()
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
}

void CStructElementDlg::OnTmTableBtn() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (m_TableIDs[nCurSel] <= 0) return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(m_TableIDs[nCurSel], 0));	
}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructElementDlg::OnTabSelect(WPARAM wParam,LPARAM lParam)
{
	//m_ChildDlgs[m_nCurChildDlg]->OnTabSelect(NULL,NULL);
	if(CDBDoc::GetDocPoint())
	{
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetDivPatternMode    (FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode (FALSE); 
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputParam(-1,-1);
	}
	//if(bActivate)
	{
		int nCurID;
		if(m_nCurChildDlg != -1) nCurID = m_nCurChildDlg;
		else nCurID = 0;

		ShowCurChildDlg(nCurID);
		CTreeMenuBarBase::SetModeOnlyST(m_DlgIDs[nCurID]);

		// 테이블 없는 항목인 경우 Table 버튼 Disable
		if (m_TableIDs[nCurID] > 0) m_wndTableBtn.EnableWindow(TRUE);
		else m_wndTableBtn.EnableWindow(FALSE);
	}

	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;

	//if(m_nCurChildDlg == 7)
	if(m_DlgIDs[m_nCurChildDlg] ==__TMMODE_STRUCT_ELEM_DIVIDE__)
		((CElemDivideDlg *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(TRUE);

	//if(m_nCurChildDlg == 0)
	if(m_DlgIDs[m_nCurChildDlg] ==__TMMODE_STRUCT_ELEM_CREATE__)
	{
		m_ChildDlgs[m_nCurChildDlg]->OnTabSelect(0L,0L);
	}
	//if(I_GENModelBase::GetModelingModeST() != IG_STATE_ELEM_ADD)
	//{
		//CMouseEdit::ImNotAssocWindowST();
		//CMouseEdit::DisConnectAssocWindowST();
	//}

}
/////////////////////////////////////////////////////////////////////
// By L.C.G
void CStructElementDlg::OnTabUnSelect(WPARAM wParam,LPARAM lParam)
{
	int nCurSel = m_ComboMenu.GetCurSel();
	if (nCurSel < 0 || nCurSel >= m_ComboMenu.GetCount()) return;
	if (0 > m_nCurChildDlg || m_nCurChildDlg >= m_ComboMenu.GetCount()) return;
	//m_ChildDlgs[m_nCurChildDlg]->OnTabUnSelect(NULL,NULL);
	
	//if(m_nCurChildDlg == 7)
	if(m_DlgIDs[m_nCurChildDlg] ==__TMMODE_STRUCT_ELEM_DIVIDE__)
		((CElemDivideDlg *)m_ChildDlgs[m_nCurChildDlg])->CheckMode(FALSE);

	//if(m_nCurChildDlg == 1)
	//  ((CElemCurveDlg *)m_ChildDlgs[m_nCurChildDlg])->EndPlugInParam(); 
}

CWnd* CStructElementDlg::GetDlgByID(int Mode)
{
	int nSize = m_DlgIDs.GetSize();
	for (int i = 0; i < nSize; i++)
		if (Mode == m_DlgIDs[i]) return m_ChildDlgs[i];
	return 0;
}

void CStructElementDlg::SetHolderSize(int nHeight)
{
	CWnd *pWndHolder/*, *pWndExecute, *pWndClose*/; 
	CRect rectHolder;
	pWndHolder = GetDlgItem(IDC_TM_PLACEHOLDER);
	pWndHolder->GetWindowRect(rectHolder);
	int nPrevHeight = rectHolder.Height();

	rectHolder.bottom = rectHolder.top + nHeight;
	pWndHolder->GetParent()->ScreenToClient(rectHolder);
	pWndHolder->MoveWindow(rectHolder);
	
	CRect PHRect;
	m_wndPlaceHolder.GetWindowRect(&PHRect);
	ScreenToClient(&PHRect);
	m_RectPlaceHolder = PHRect;

	/////////////////////////////////////////////////////////////////////////
	// Layout Child Dialog...

	CSize ScrSize, TSize;
	ScrSize = GetTotalSize();
	ScrSize.cy += nHeight - nPrevHeight;

	SetScrollSizes(MM_TEXT, ScrSize);
}

