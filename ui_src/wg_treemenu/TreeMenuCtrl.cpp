// BarTreeMenuCtrl.cpp : implementation file
// 
// 항목 추가 및 수정시 주의사항!!
//
// 1. 꼭 순서를 지켜 주세요 (메인메뉴의 같은 순서로)
// 2. 꼭 아이콘(비트맵)을 만들어 같이 붙여 주세요 
//    -> 안 만드시는 분은 나중에 찾아서 응징할 겁니다!! *^^*
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeMenuCtrl.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_main\wg_mainres2.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\AluDgnTool.h"
#include "..\wg_db\DBCodeDef.h"

const unsigned int cDGN_PSC_DGN = 9999;
const unsigned int cDGN_RATING_DGN = 9998;
const unsigned int cSTRUCT_GEOMETRY = 9997;

/* 아이콘 갯수 변경시 수정해주세요 !!! pyj0827 */
const int IMAGE1_SIZE = 64;
const int IMAGE2_SIZE = IMAGE1_SIZE + 64;
const int IMAGE3_SIZE = IMAGE2_SIZE + 64;
const int IMAGE4_SIZE = IMAGE3_SIZE + 64;
const int IMAGE5_SIZE = IMAGE4_SIZE + 64;
const int IMAGE6_SIZE = IMAGE5_SIZE + 83;
const int IMAGE7_SIZE = IMAGE6_SIZE + 64;
const int IMAGE8_SIZE = IMAGE7_SIZE + 64;

using namespace mit::frx;
IMPLEMENT_DYNAMIC(CTreeMenuCtrl, MBaseTreeCtrl);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#314  Recently Bitmap ID 
/////////////////////////////////////////////////////////////////////////////
// CTreeMenuCtrl

CTreeMenuCtrl::CTreeMenuCtrl()
{
	m_BkColor = RGB( 192, 220, 192);
	m_strSvgPath = _T("TreeMenu\\menu");
}

CTreeMenuCtrl::~CTreeMenuCtrl()
{
}

#define CTreeCtrl MBaseTreeCtrl
BEGIN_MESSAGE_MAP(CTreeMenuCtrl, CTreeCtrl)
#undef CTreeCtrl
	//{{AFX_MSG_MAP(CTreeMenuCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Local Helper Functions

HTREEITEM InsertTreeItem(MBaseTreeCtrl* ctrl,LPCTSTR txt,int imgind,int selimgind)
{
	TV_INSERTSTRUCT  TreeCtrlItem;  	

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;

	TreeCtrlItem.item.iImage = imgind;
	TreeCtrlItem.item.iSelectedImage = selimgind;
	TreeCtrlItem.item.lParam = 0;
	TreeCtrlItem.item.pszText = (LPTSTR)txt;
	
	HTREEITEM hTreeItem = ctrl->InsertItem(&TreeCtrlItem);
	return(hTreeItem);
}

HTREEITEM InsertTreeItem(MBaseTreeCtrl* ctrl,HTREEITEM hTreeItem,UINT CommandID,LPCTSTR txt,int imgind,int selimgind,HTREEITEM hInsertAfter/* = TVI_LAST*/)
{
	TV_INSERTSTRUCT  TreeCtrlItem;  	

	TreeCtrlItem.hParent = hTreeItem;
	TreeCtrlItem.hInsertAfter = hInsertAfter;
	TreeCtrlItem.item.mask = TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	TreeCtrlItem.item.lParam = CommandID;
	TreeCtrlItem.item.iImage = imgind;
	TreeCtrlItem.item.iSelectedImage = selimgind;
	TreeCtrlItem.item.pszText = (LPTSTR)txt;

	HTREEITEM hnTreeItem = ctrl->InsertItem(&TreeCtrlItem);
	return(hnTreeItem);
}

/////////////////////////////////////////////////////////////////////////////
// CTreeMenuCtrl message handlers

void CTreeMenuCtrl::CreateTree()
{
	return;
}

void CTreeMenuCtrl::LoadMenu()
{
	HTREEITEM	hTI[20];
	HTREEITEM   RoothTi,ThTi;
	// Load the tree with some items to make the demo interesting
	RoothTi = InsertTreeItem(_LS(IDS_TM_Structure_Analysis), _T(""));
	//RoothTi = NULL;
	int nRootIndex=0;
	int nMovingType = CProduct::GetMovingType();

//------------------------------------------------------------------------------------------------
// Configuration 
//------------------------------------------------------------------------------------------------
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Configuration), _T("ico24_temp"), m_strSvgPath);
	{
		InsertTreeItem(hTI[nRootIndex], ID_FILE_PJINFO, _LS(IDS_TM_Project_Information), _T("ico24_burger_projectinformation"), m_strSvgPath);
#ifndef _TOWER		//수정(김영민)
#ifdef _IGEN
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_CONFIG, _LS(IDS_TM_Structure_Type), _T("ico24__structure_type_structuretype"), m_strSvgPath);
#else  //_IGEN
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_CONFIG, _LS(IDS_TM_Structure_Type), _T("ico24__structure_type_structuretype"), m_strSvgPath);
#endif  //_IGEN
#endif						//수정(김영민)
	}
//------------------------------------------------------------------------------------------------
// Geometry
//------------------------------------------------------------------------------------------------
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Geometry), _T("ico24_temp"), m_strSvgPath);
	{
		//-----------------------------
		// Wizard
		ThTi = InsertTreeItem(hTI[nRootIndex], cSTRUCT_GEOMETRY, _LS(IDS_TM_Structure_Wizard), _T("ico24_temp"), m_strSvgPath);
		{
#ifndef _TOWER
			InsertTreeItem(ThTi, ID_STRUCT_SW_BEAM, _LS(IDS_TM_Beam), _T("ico24_structure_wizard_beam"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_COLUMN, _LS(IDS_TM_Column), _T("ico24_structure_wizard_column"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_ARCH, _LS(IDS_TM_Arch), _T("ico24_structure_wizard_arch"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_FRAME, _LS(IDS_TM_Frame), _T("ico24_structure_wizard_frame"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_TRUSS, _LS(IDS_TM_Truss), _T("ico24_structure_wizard_truss"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_PLATE, _LS(IDS_TM_Plate), _T("ico24_structure_wizard_plate"), m_strSvgPath);
#endif
#ifdef _TOWER
			InsertTreeItem(ThTi, ID_STRUCT_SW_TOWER_LEG, _LS(IDS_TM_Tower_Leg ), _T("ico24_structure_wizard_towerleg"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_TOWER_BODY, _LS(IDS_TM_Tower_Body), _T("ico24_structure_wizard_towerbody"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_TOWER_ARM, _LS(IDS_TM_Tower_Arm ), _T("ico24_structure_wizard_towerarm"), m_strSvgPath);
#endif

#if defined(_MGEN) || defined(_CIVIL)
			InsertTreeItem(ThTi, ID_STRUCT_SW_SHELL, _LS(IDS_TM_Shell), _T("ico24_structure_wizard_shell"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_STRUCT_SW_PIPE_CONNECTION, _LS(IDS_TM_Pipe_Connection), _T("ico24_temp"), m_strSvgPath);
#endif
#ifdef _CIVIL
			InsertTreeItem(ThTi, ID_STRUCT_SW_SUSBRG, _LS(IDS_TM_Suspension_Bridge), _T("ico24_structure_wizard_suspensionbridge"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_STAYEDBRG, _LS(IDS_TM_Cable_Stayed_Bridge), _T("ico24_structure_wizard_cablestayedbridge"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_ILMMODEL, _LS(IDS_TM1_ILM_Bridge_Model___), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_ILMSTAGE, _LS(IDS_TM1_ILM_Bridge_Stage___), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_FCM, _LS(IDS_TM1_FCM_Bridge___), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_MSS, _LS(IDS_TM1_MSS_FSM_Bridge___), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_FSM, _LS(IDS_TM1_FSM_Bridge___), _T("ico24_temp"), m_strSvgPath);
			
			// MNET:XXXX-CJJEONG-20080225
			HTREEITEM ThTi2 = InsertTreeItem(ThTi, 0, _LS(IDS_TM_PSC), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi2, ID_STRUCT_SW_PSC_SPAN, _LS(IDS_TM_PSC_SPAN), _T("ico24_structure_wizard_spaninformation"), m_strSvgPath);
			InsertTreeItem(ThTi2, ID_STRUCT_SW_PSC_SECT, _LS(IDS_TM_PSC_SECT), _T("ico24_structure_wizard_sectionreinforcement"), m_strSvgPath);
			InsertTreeItem(ThTi2, ID_STRUCT_SW_PSC_EFFECT, _LS(IDS_TM_PSC_EFFECT), _T("ico24_structure_wizard_effectivewidth"), m_strSvgPath);

	#if !defined(_JP)
			InsertTreeItem(ThTi, ID_STRUCT_SW_CROSS, _LS(IDS_TM_PSC_CROSS), _T("ico24_temp"), m_strSvgPath);
	#endif
			 
			InsertTreeItem(ThTi, ID_STRUCT_SW_GRIL, _LS(IDS_TM_GRIL_MODEL_WIZARD), _T("ico24_temp"), m_strSvgPath);
#if defined(_CIVIL_CH)
			InsertTreeItem(ThTi, ID_STRUCT_SW_GRIL_CH, _LS(IDS_TM_CHGRIL_MODEL_WIZARD), _T("ico24_structure_type_structuretype"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_GIRDER_CH, _LS(IDS_TM_CHGIRDER_MODEL_WIZARD), _T("ico24_structure_type_structuretype"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_STRUCT_SW_FASTTENDONMAKER, _LS(IDS_TM_FASTTENDONMAKET_WIZARD), _T("ico24_structure_type_structuretype"), m_strSvgPath);
#if defined(_CIVIL_CH)
			InsertTreeItem(ThTi, ID_STRUCT_SW_PILEFOUNDATION, _LS(IDS_TM_PILEFOUNDATION_WIZARD), _T("ico24_structure_type_structuretype"), m_strSvgPath);
#endif
			
			InsertTreeItem(ThTi, ID_STRUCT_SW_RCSLAB, _LS(IDS_TM_RCSlab_Brg), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_RCRAHMEN, _LS(IDS_TM_RCRahmen_Brg), _T("ico24_temp"), m_strSvgPath);
	#if defined(_ORG)
			InsertTreeItem(ThTi, ID_STRUCT_SW_SUBWAY, _LS(IDS_TM_SubwayStation), _T("ico24_structure_type_structuretype"), m_strSvgPath);
	#endif
	#if defined(_JP)
			InsertTreeItem(ThTi, ID_STRUCT_SW_GRID, _LS(IDS_TM_GRID_MODEL_WIZARD), _T("structure_category_3"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_SW_GRID_SLAB, _LS(IDS_TM_GRID_SLAB_MODEL_WIZARD), _T("structure_category_4"), m_strSvgPath);
	#endif

#if defined(_CIVIL_CH)
			if(CProduct::GetTestEnvValue(_T("GirderWizTest")) == _T("yes"))
			{
				InsertTreeItem(ThTi, ID_STRUCT_SW_GRIL_CH, _LS(IDS_MAIN_RIBBON_MENU_CHGrillage_Model), _T("ico24_structure_type_structuretype"), m_strSvgPath);
			}
			if(CProduct::GetTestEnvValue(_T("CHGirderWizTest")) == _T("yes"))
			{
				InsertTreeItem(ThTi, ID_STRUCT_SW_GIRDER_CH, _LS(IDS_MAIN_RIBBON_MENU_CHGirder_Model), _T("ico24_structure_type_structuretype"), m_strSvgPath);
			}
#endif
			if (CProduct::GetTestEnvValue(_T("FastTednonMakerTest")) == _T("yes"))
			{
				InsertTreeItem(ThTi, ID_STRUCT_SW_FASTTENDONMAKER, _LS(IDS_MAIN_RIBBON_MENU_FASTTENDONMAKER), _T("ico24_structure_type_structuretype"), m_strSvgPath);
			}

#if defined(_CIVIL_CH)
			if (CProduct::GetTestEnvValue(_T("PileFoundationWizTest")) == _T("yes"))
			{
				InsertTreeItem(ThTi, ID_STRUCT_SW_PILEFOUNDATION, _LS(IDS_MAIN_RIBBON_MENU_PILEFOUNDATION), _T("ico24_structure_type_structuretype"), m_strSvgPath);
			}
#endif
			if(CProduct::GetTestEnvValue(_T("GirderWizTest")) == _T("yes"))
			{
				InsertTreeItem(ThTi, ID_STRUCT_SW_GIRDER, _LS(IDS_TM_GIRDER_BRIDGE_WIZARD), _T("ico24_structure_type_structuretype"), m_strSvgPath);
			}
			else
			{
#if defined(_CIVIL_RUS) || defined (_CIVIL_US) || defined (_CIVIL_ORG)
				InsertTreeItem(ThTi, ID_STRUCT_SW_GIRDER, _LS(IDS_TM_GIRDER_BRIDGE_WIZARD), _T("ico24_structure_type_structuretype"), m_strSvgPath);
#endif
			}

			BOOL bPcGirderWizEnable = FALSE;
#if defined(_CIVIL_RUS) || defined (_CIVIL_US)
			bPcGirderWizEnable = TRUE;
#endif
			if(CProduct::GetTestEnvValue(_T("PCGirderWizTest")) == _T("yes") || bPcGirderWizEnable)
			{
				InsertTreeItem(ThTi, ID_STRUCT_SW_PCGIRDER, _LS(IDS_SW_PC_GIRDER_Wiz), _T("ico24_structure_wizard_prestressedcompositebridge"), m_strSvgPath);
			}
	
#endif

#if defined(_MGEN)
#if !(defined(_JP) || defined(_CH) || defined(_US) || defined(_RUS))  //1 added by KYM
			InsertTreeItem(ThTi, ID_SW_VILLA_WALL, _LS(IDS_TM_Steel_Villa_Wall), _T("ico24_structure_wizard_steelvillawall"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_SW_VILLA_TRUSS, _LS(IDS_TM_Steel_Villa_Truss), _T("ico24_structure_wizard_steelvillatruss"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_SW_VILLA_STAIR, _LS(IDS_TM_Steel_Villa_Stair), _T("ico24_structure_wizard_steelvillastair"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_SW_WIND_STRUCTURE, _STR("Window Frame"), _T("ico24_structure_wizard_windowframe"));
#endif
#endif
		}
		//-----------------------------
		// User Coordinate System
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_User_Coordinate_System), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_STRUCT_UCS_XY, _LS(IDS_TM_X_Y_Plane), _T("ico24__structure_ucsplan_ucs"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_XZ, _LS(IDS_TM_X_Z_Plane), _T("ico24_structure_ucsplan_xzplane"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_YZ, _LS(IDS_TM_Y_Z_Plane), _T("ico24_structure_ucsplan_yzplane"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_COOR, _LS(IDS_TM_Three_Points), _T("ico24_structure_ucsplan_threepoints"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_ANGLE, _LS(IDS_TM_Three_Angles), _T("ico24_structure_ucsplan_threeangles"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_NPLANE, _LS(IDS_TM_Named_Plane), _T("ico24_structure_ucsplan_definenamedplane"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_BY_UCS, _LS(IDS_TM_UCS_by_UCS), _T("ico24_structure_ucsplan_uscbyucs"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_UCS_BY_NAME, _LS(IDS_TM_Named_UCS), _T("ico24_structure_ucsplan_nameducs"), m_strSvgPath);
		}
		//-----------------------------
		// Grid
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Grids), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_GRID_EQUAL, _LS(IDS_TM_Point_Grid), _T("ico24_view_gridsnap_pointgrid"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_GRID_UNEQUAL, _LS(IDS_TM_Line_Grid), _T("ico24_view_gridsnap_linegrid"), m_strSvgPath);
		}
		//-----------------------------
		// Node
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Nodes), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_NODE_CREATE, _LS(IDS_TM_Create), _T("ico24_nodeelement_nodes_createnodes"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_DELETE, _LS(IDS_TM_Delete), _T("ico24_nodeelement_nodes_delete"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_TRANSLATE, _LS(IDS_TM_Translate), _T("ico24_nodeelement_nodes_translate"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_ROTATE, _LS(IDS_TM_Rotate), _T("ico24_nodeelement_nodes_rotate"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_PROJECT, _LS(IDS_TM_Project), _T("ico24_nodeelement_nodes_project"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_MIRROR, _LS(IDS_TM_Mirror), _T("ico24_nodeelement_nodes_mirror"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_DIVIDE, _LS(IDS_TM_Divide), _T("ico24_nodeelement_nodes_divide"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_NODE_INTERSECT, _LS(IDS_TM_Intersect), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_MERGE, _LS(IDS_TM_Merge), _T("ico24_nodeelement_nodes_merge"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_SCALE, _LS(IDS_TM_Scale), _T("ico24_nodeelement_nodes_scale"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_COMPACT, _LS(IDS_TM_Compact_Numbers), _T("ico24_nodeelement_nodes_compactnodenumbers"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_RENUMBER, _LS(IDS_TM_Renumbering), _T("ico24_nodeelement_nodes_renumberrnodeid"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_NODE_STARTNUMBER, _LS(IDS_TM_Node_Numbering), _T("ico24_nodeelement_nodes_startnodenumber"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_SKEWCS, _LS(IDS_TM_Node_Local_Axis), _T("ico24_boundary_etc_nodelocalaxis"), m_strSvgPath);
		}
		//-----------------------------
		// Element
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Elements), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_CREATE, _LS(IDS_TM_Create_Elements), _T("ico24_nodeelement_nodes_createelement"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_CURVE, _LS(IDS_TM_Create_Line_Elements_on_Curve), _T("ico24_nodeelement_elements_createlineelementsoncurve"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_CONV_LINE_CREATE, _LS(IDS_TM_Create_Converted_Line_Elements), _T("ico24_nodeelement_elements_createconvertedlineelements"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_OPEN_WALL_CREATE, _LS(IDS_TM_Create_Opening_Wall_Elements), _T("ico24_nodeelement_elements_wallopening"), m_strSvgPath);
			#if defined(_CIVIL)
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_ECCEN, _LS(IDS_TM_Create_Eccentric_Elements), _T("ico24_nodeelement_elements_createeccentricelements"), m_strSvgPath);
			#endif
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_DELETE, _LS(IDS_TM_Delete), _T("ico24_nodeelement_elements_delete"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_TRANSLATE, _LS(IDS_TM_Translate), _T("ico24_nodeelement_elements_translate"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_ROTATE, _LS(IDS_TM_Rotate), _T("ico24_nodeelement_elements_rotate"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_EXTRUDE, _LS(IDS_TM_Extrude), _T("ico24_nodeelement_elements_extrude"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_MIRROR, _LS(IDS_TM_Mirror), _T("ico24_nodeelement_elements_mirror"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_DIVIDE, _LS(IDS_TM_Divide), _T("ico24_nodeelement_elements_divide"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_MERGE, _LS(IDS_TM_Merge), _T("ico24_nodeelement_elements_merge"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_INTERSECT, _LS(IDS_TM_Intersect), _T("ico24_nodeelement_elements_intersection"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_CHANGE, _LS(IDS_TM_Change_Element_Parameters), _T("ico24_nodeelement_elements_changeparameter"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_ELEM_REVNORMAL, _LS(IDS_TM_Reverse_Element_Normal), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_COMPACT, _LS(IDS_TM_Compact_Numbers), _T("ico24_nodeelement_elements_compactnodenumbers"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_RENUMBER, _LS(IDS_TM_Renumbering), _T("ico24_nodeelement_elements_renumberrnodeid"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ELEM_STARTNUMBER, _LS(IDS_TM_Element_Numbering), _T("ico24_nodeelement_elements_elementnumbering"), m_strSvgPath);
		}
		//-----------------------------
		// Property
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Properties), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_PROP_MATERIAL, _LS(IDS_TM_Material), _T("ico24_properties_material_materialproperties"), m_strSvgPath);
#ifndef _TOWER
			//ID_STRUCT_HYDR_TIME_DEP_MAT_FUNC
			InsertTreeItem(ThTi, ID_STRUCT_HYDR_TIME_DEP_MAT_FUNC, _LS(IDS_WG_TREEMENU_ADD3_Time_Dependent_Material_Function), _T("ico24_properties_tdm_userdefine"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_HYDR_TIME_DEP_MAT_TYPE, _LS(IDS_WG_TREEMENU_ADD3_Hydration_Time_Dependent_Mater), _T("ico24_properties_tdm_creepshrinkage"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_HYDR_TIME_DEP_MATELAST_TYPE, _LS(IDS_TM1_Time_Dependent_Material_Comp__Strength____), _T("ico24_properties_tdm_compstrength"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_HYDR_TIME_DEP_MAT, _LS(IDS_WG_TREEMENU_ADD3_Hydration_Time_Dependent_Mater_MSG1), _T("ico24_properties_tdm_materiallink"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_HYDR_TIME_DEP_MAT_ELMT, _LS(IDS_TM1_Change_Element_Dependent_Material_Property_), _T("ico24_properties_tdm_changeproperty"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PROP_ELS_PLS, _LS(IDS_TM_TREEMENU_MATERIAL_ELS_PLS), _T("ico24_properties_plastic_plasticmaterial"), m_strSvgPath);
	#if defined (_CH)
			// InsertTreeItem(ThTi, ID_STRUCT_TEMPERATURE_RISE_PROP, _LS(IDS_MAIN_RIBBON_MENU_Temperature_Rising_Properties), _T("ico24_view_dynamicview_pan2"), m_strSvgPath);
			// InsertTreeItem(ThTi, ID_STRUCT_TEMPERATURE_MATL_LINK, _LS(IDS_MAIN_RIBBON_MENU_Material_Properties_Link), _T("ico24_view_dynamicview_pan2"), m_strSvgPath);
	#endif
#endif
			InsertTreeItem(ThTi, ID_STRUCT_PROP_SECTION, _LS(IDS_TM_Section), _T("ico24_properties_section_sectionproperties"), m_strSvgPath);

#ifdef _CIVIL
			HTREEITEM ThTi2 = InsertTreeItem(ThTi, 0, _LS(IDS_TREEMENU_SECTION_MANAGER)    , _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi2, ID_STRUCT_PROP_SECTION_SFACTOR, _LS(IDS_TREEMENU_SECTION_MANAGER_STIFFNESS), _T("ico24_properties_section_sectionstiffnessscalefactor"), m_strSvgPath);
				InsertTreeItem(ThTi2, ID_STRUCT_PROP_SECTION_RPSC, _LS(IDS_TREEMENU_SECTION_MANAGER_REINFORCEMENTS), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi2, ID_STRUCT_PROP_STRESS_POINT, _LS(IDS_TREEMENU_SECTION_MANAGER_STRESS_POINTS), _T("ico24_temp"), m_strSvgPath);	
			}
#else
			InsertTreeItem(ThTi, ID_STRUCT_PROP_SECTION_SFACTOR, _LS(IDS_WG_TREEMENU_ADD3_Section_Stiffness_Scale_Factor), _T("ico24_properties_section_sectionstiffnessscalefactor"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PROP_ELEM_SFACTOR, _LS(IDS_TM_WORKTREE_Element_Stiffness_Scale_Factor), _T("ico24_properties_section_elementstiffnessscalefactor"), m_strSvgPath);
#endif

			InsertTreeItem(ThTi, ID_STRUCT_PROP_SECTION_TAPEREDGROUP, _LS(IDS_TM_Tapered_Section_Group), _T("ico24_properties_section_taperedgroupgen"), m_strSvgPath);
#if defined(_CIVIL)
//       BOOL bEnableRForc = FALSE;
//       if (CProduct::GetTestEnvValue(_ULS(TestLDFS)) == _ULS(yes))  bEnableRForc = TRUE;
//       if (bEnableRForc)
//       {
//         InsertTreeItem(ThTi, ID_STRUCT_SECT_FOR_RFOC, _LS(IDS_TM_Section_for_Resultant_Forces), _T("ico24_temp"), m_strSvgPath);
//       }
			InsertTreeItem(ThTi, ID_STRUCT_SECT_FOR_RFOC, _LS(IDS_TM_Section_for_Resultant_Forces), _T("ico24_temp"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_STRUCT_PROP_THICKNESS, _LS(IDS_TM_Thickness), _T("ico24_properties_section_thickness"), m_strSvgPath);

#if defined(_MGEN)
	#if !defined(_JP)
			// PMS 5265 : Gen RC,SRC 단면 철근 고려한 해석
			// 현재 개발이 보류되어 이부분 막았음. (박경식 책임)
// 			if (CProduct::GetTestEnvValue(_ULS(TestRCSRC)) == _ULS(yes))
// 			{
// 				InsertTreeItem(ThTi, ID_STRUCT_PROP_REINFORCEMENT_ANAL, _LS(IDS_TM_Reinforcement_Analysis), _T("ico24_temp"), m_strSvgPath);
// 			}
	#endif
#endif

#ifdef _CIVIL
			if(CProduct::GetMovingType() == D_PRODUCT_MOVING_JP || CProduct::GetMovingType() == D_PRODUCT_MOVING_ALL)
			{
				HTREEITEM ThTi2 = InsertTreeItem(ThTi, 0, _LS(IDS_CMD_MPHI_CALCULATION), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi2, ID_STRUCT_MPHI_PRAMETER, _LS(IDS_CMD_MPHI_PARAMETERS), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi2, ID_STRUCT_MPHI_CALCULATE, _LS(IDS_CMD_MPHI_CALCULATE), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi2, ID_STRUCT_MPHI_SECTION_PARAMETER, _LS(IDS_CMD_MPHI_SECT_PARAMETERS), _T("ico24_temp"), m_strSvgPath);
			}
#endif

#ifdef _MGEN
			InsertTreeItem(ThTi, ID_STRUCT_PROP_WSSF, _LS(IDS_TM_Wall_Shear_Stiffness_Reduction_Factor), _T("ico24_properties_section_wallstiffnessscalefactor"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_STRUCT_PROP_PSSF, _LS(IDS_TM_Plate_Shear_Stiffness_Reduction_Factor), _T("ico24_properties_section_platestiffnessscalefactor"), m_strSvgPath);
#ifndef _TOWER
			InsertTreeItem(ThTi, ID_STRUCT_ITHA_DEFINE_HINGE, _LS(IDS_TM_Inelastic_Hinge_Property), _T("ico24_properties_inelproperties_inelastichingeproperties"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ITHA_ASSIGN_HINGE, _LS(IDS_TM_Inelastic_Hinge), _T("ico24_properties_inelproperties_assigninelastichingeproperties"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_GRDP_GROUP_DAMP, _LS(IDS_TM_Group_Damping), _T("ico24_properties_damping_groupdamping"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_GRDP_STRAIN_ENERGY, _LS(IDS_TM_Group_Damping_Strain), _T("ico24_properties_damping_groupdamping"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PROP_FIBER_PROP, _LS(IDS_TM_FIBER_MATERIAL), _T("ico24_properties_inelproperties_inelasticmaterialproperties"), m_strSvgPath); 
			InsertTreeItem(ThTi, ID_STRUCT_PROP_FIBER_DIVISION, _LS(IDS_TM_FIBER_DIVISION), _T("ico24_properties_inelasticproperties_fiberdivisionofsectionbeamcolumn"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PROP_FIBER_DIVISION_WALL, _LS(IDS_TM_FIBER_DIVISION_WALL), _T("ico24_properties_inelasticproperties_fiberdivisionofsectionmcpmwall"), m_strSvgPath);
			
#endif

#ifdef _CIVIL
	if(CProduct::GetMovingType() == D_PRODUCT_MOVING_ORG || CProduct::GetMovingType() == D_PRODUCT_MOVING_CH || 
		 CProduct::GetMovingType() == D_PRODUCT_MOVING_US  || CProduct::GetMovingType() == D_PRODUCT_MOVING_RUS||
		 CProduct::GetMovingType() == D_PRODUCT_MOVING_ALL)
	//#if defined(_ORG) || defined(_CH) || defined(_US) || defined(_RUS)
		InsertTreeItem(ThTi, ID_STRUCT_PROP_MPHI_CURVE, _LS(IDS_TM_MPHI_CURVE), _T("ico24_temp"), m_strSvgPath);
	//#endif
#endif
		}
		//-----------------------------
		// Boundaries
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Boundaries), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_SUPPORT, _LS(IDS_TM_Supports), _T("ico24_boundary_tables_boundarytables"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_PSPRING, _LS(IDS_TM_Point_Spring_Supports), _T("ico24_boundary_springsupports_pointspring"), m_strSvgPath);
#ifndef _TOWER
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_GENSPRTYPE, _LS(IDS_TM_Define_General_Spring_Type), _T("ico24_boundary_springsupports_definegeneralspringtype"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_BNDR_LSPRING, _LS(IDS_TM_Line_Spring_Supports), _T("ico24_temp"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_BNDR_ASPRING, _LS(IDS_TM_Area_Spring_Supports), _T("ico24_temp"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_BNDR_GENSPRTYPE, _LS(IDS_TM_Define_General_Spring_Type), _T("ico24_boundary_springsupports_definegeneralspringtype"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_GENSPRING, _LS(IDS_TM_General_Spring_Supports), _T("ico24_boundary_springsupports_generalspringsupports"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_SURFACESPRING, _LS(IDS_TM_Surface_Spring_Supports), _T("ico24_boundary_springsupports_surfacespring"), m_strSvgPath);
#ifdef _CIVIL
	//#ifdef _US // 20070822 mylee - 다 푼다
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_SOILSPRING, _LS(IDS_TM_Soil_Spring_Supports), _T("ico24_boundary_springsupports_integralbridge"), m_strSvgPath);
	//#endif
#else
			// 20081008 mylee
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_PILESPRING, _LS(IDS_TM_Pile_Spring_Supports), _T("ico24_boundary_springsupports_pilespringsupports"), m_strSvgPath);
#endif  // end of #ifdef _CIVIL
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_ELLINK, _LS(IDS_TM_Elastic_Link), _T("ico24_boundary_link_elasticlink"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_NLLINK_PROPERTY, _LS(IDS_TM_Nonlinear_Link_Property), _T("ico24_boundary_link_generallinkproperties"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_NLNK, _LS(IDS_WG_TREEMENU_Non_Linear_Link), _T("ico24_boundary_link_generallink2"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_CGLP, _LS(IDS_TM_DT_CGLK), _T("ico24_boundary_link_changegenerallinkproperties"), m_strSvgPath);
			HTREEITEM ThTi_SeisDv = InsertTreeItem(ThTi, NULL, _LS(IDS_TM_MENUCTRL_SEISMIC_PROP), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"), m_strSvgPath);
			InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_SDVP_VISCDAMP, _LS(IDS_TM_MENUCTRL_SDVI), _T("ico24_boundary_link_viscousdamperoildamper"), m_strSvgPath);      
			InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_SDVP_VELADAMP, _LS(IDS_TM_MENUCTRL_SDVE), _T("ico24_boundary_link_viscoelasticdamper"), m_strSvgPath);      
			InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_SDVP_STLDAMP, _LS(IDS_TM_MENUCTRL_SDST), _T("ico24_boundary_link_steeldamper"), m_strSvgPath);      
			InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_SDVP_HYSTDAMP, _LS(IDS_TM_MENUCTRL_SDHY), _T("ico24_boundary_link_hystereticisolator(mss)"), m_strSvgPath);
			InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_SDVP_ISOLATOR, _LS(IDS_TM_MENUCTRL_SDIS), _T("ico24_boundary_link_isolator(mss)"), m_strSvgPath);
#ifdef _MGEN
	#if defined (_CH)
			// image path - temporary
			// InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_ASGB_ISOLATOR, _LS(IDS_TM_MENUCTRL_ASGB), _T("ico24_boundary_link_isolator(mss)"), m_strSvgPath);
			// InsertTreeItem(ThTi_SeisDv, ID_STRUCT_BNDR_DAMPERGB, _LS(IDS_TM_MENUCTRL_DMGB), _T("ico24_boundary_link_isolator(mss)"), m_strSvgPath);
	#endif
#endif
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_BRELEASE, _LS(IDS_TM_Beam_End_Release), _T("ico24_boundary_releaseoffset_beamendrelease"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_BOFFSET, _LS(IDS_TM_Beam_End_Offsets), _T("ico24_boundary_releaseoffset_beamendoffsets"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_PRELEASE, _LS(IDS_TM_Plate_End_Release), _T("ico24_boundary_releaseoffset_plateendrelease"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_RIGIDLINK, _LS(IDS_TM_Rigid_Link), _T("ico24_boundary_link_rigidlink"), m_strSvgPath);
			// MNET:2790-JHLEE-20070911
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_MCON, _LS(IDS_WG_TREEMENU_Linear_Constraints), _T("ico24_boundary_etc_linearconstraints"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_PANELZONE, _LS(IDS_TM_Panel_Zone_Effects), _T("ico24_boundary_etc_panelzoneeffects"), m_strSvgPath);
#endif  // end of #ifndef _TOWER
#ifdef _CIVIL
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_EWSF, _LS(IDS_TM_Eff_Width_Scale_Factor), _T("ico24_temp"), m_strSvgPath);	
#endif  // end of #ifdef _CIVIL
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_CLDR, _LS(IDS_WG_TREEMENU_Define_Constraint_Label_Direction), _T("ico24_boundary_etc_definelabeldir"), m_strSvgPath);	
#ifdef _MGEN
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_DISCONNECT, _LS(IDS_TM_Diaphragm_Disconnect), _T("ico24_boundary_misc_diaphragmdisconnect"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_DIAPHGROUP, _LS(IDS_WG_TREEMENU_Story_Diaphragm_Group), _T("ico24_boundary_misc_storydiaphragmgroup"), m_strSvgPath);
#endif  // end of #ifdef _MGEN
		}
		//-----------------------------
		// Mass
#ifndef _TOWER
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Masses), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_MASS_NMASS, _LS(IDS_TM_Nodal_Masses), _T("ico24_load_structureloadsmasses_nodalmasses"), m_strSvgPath);
#ifdef _MGEN
			InsertTreeItem(ThTi, ID_STRUCT_MASS_FDMASS, _LS(IDS_TM_Floor_Diaphragm_Masses), _T("ico24_load_structureloadsmasses_floordiaphragmmasses"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_STRUCT_MASS_LTMASS, _LS(IDS_TM_Loads_to_Masses), _T("ico24_load_structureloadsmasses_loadstomasses"), m_strSvgPath);
		}

#ifdef _CIVIL
	if(CProduct::GetMovingType() == D_PRODUCT_MOVING_CH  || CProduct::GetMovingType() == D_PRODUCT_MOVING_JP || 
		 CProduct::GetMovingType() == D_PRODUCT_MOVING_US  || CProduct::GetMovingType() == D_PRODUCT_MOVING_RUS||
		 CProduct::GetMovingType() == D_PRODUCT_MOVING_ALL)
	{
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Mesh), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_AUTOMESH, _LS(IDS_TM_Automesh), _T("ico24_mods_mesh_automesh"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_MAPMESH_KE, _LS(IDS_TM_Mapmesh_KE), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_MAPMESH, _LS(IDS_TM_Mapmesh), _T("ico24_mods_mesh_mapmesh"), m_strSvgPath);
		}
	}
	
#endif

		//-----------------------------
		// Building
#ifdef _MGEN    
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Building), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_BDC, _LS(IDS_TM_Control_Data), _T("ico24_structure_building_controldata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STORY, _LS(IDS_TM_Story), _T("ico24_structure_building_story"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BUILDING_GENERATION, _LS(IDS_TM_Building_Generation), _T("ico24_structure_building_buildinggeneration"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BUILDING_AUTOWALLID, _LS(IDS_TM_Automatic_Wall_ID_Generation), _T("ico24_structure_building_autowallidgneration"), m_strSvgPath); // added by KYM
		}

		{
			// Mesh
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Mesh), _T("ico24_temp"), m_strSvgPath);	
			{
				InsertTreeItem(ThTi, ID_STRUCT_AUTOMESH, _LS(IDS_TM_Automesh), _T("ico24_mods_mesh_automesh"), m_strSvgPath);
				//InsertTreeItem(ThTi, ID_STRUCT_MAPMESH_KE, _LS(IDS_TM_Mapmesh_KE), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_STRUCT_MAPMESH, _LS(IDS_TM_Mapmesh), _T("ico24_mods_mesh_mapmesh"), m_strSvgPath);
			}

			// Domain
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Domain), _T("ico24_temp"), m_strSvgPath);	
			{
				InsertTreeItem(ThTi, ID_STRUCT_DOMAIN_DEFINE, _LS(IDS_TM_Domain_Define), _T("ico24_nodeelement_mesh_definedomain"), m_strSvgPath);

				BOOL bDrawSubDomain = FALSE;
			#if defined(_US)
				if(!CProduct::IsIndia() && !CProduct::IsTaiwan()) bDrawSubDomain = TRUE;
			#elif defined(_ORG) 
				bDrawSubDomain = TRUE;
			#else
				bDrawSubDomain = FALSE;
			#endif
				if(bDrawSubDomain) InsertTreeItem(ThTi, ID_STRUCT_SUBDOMAIN_DEFINE, _LS(IDS_TM_SubDomain_Define), _T("ico24_nodeelement_mesh_definedomain"), m_strSvgPath);      
			}
		}
#endif
#endif // _TOWER
		//-----------------------------
		// Named Plan
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_NAMEDPLANE, _LS(IDS_TM_Named_Plane), _T("ico24__structure_ucsplan_choosenamedplane"), m_strSvgPath);	
		 
		// Dimension 
#ifdef _MGEN 
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_DIMENSION, _LS(IDS_TM_Dimension), _T("ico24_structure_ucsplan_dimension"), m_strSvgPath);	
#endif	// end of #ifdef _MGEN	
		 
		//-----------------------------
		// Group
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Group), _T("ico24_temp"), m_strSvgPath);	
		{
			InsertTreeItem(ThTi, ID_STRUCT_GROUP, _LS(IDS_WG_TREEMENU_ADD3_Define_Structure_Group___), _T("ico24_structure_group_structure"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_DEF_BNDR_GROUP, _LS(IDS_WG_TREEMENU_ADD3_Define_Boundary_Group___), _T("ico24_structure_group_defineboundarygroup"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_DEF_LOAD_GROUP, _LS(IDS_WG_TREEMENU_ADD3_Define_Load_Group___), _T("ico24_structure_group_defineloadgroup"), m_strSvgPath);
#ifdef _MGEN 
			InsertTreeItem(ThTi, ID_STRUCT_DEF_LOADINGAREA_GROUP, _LS(IDS_WG_TREEMENU_ADD3_Define_Loading_Area_Group___), _T("ico24_structure_group_defineloadingareagroup"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_STRUCT_DEF_TENDON_GROUP, _LS(IDS_WG_TREEMENU_ADD3_Define_Tendon_Group___), _T("ico24_structure_group_definetendongroup"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_BNGR_CHANGE, _LS(IDS_WG_TREEMENU_ADD3_Change_Boundary_Group), _T("ico24__structure_change_changeboundarygroup"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_LOAD_LDGR_CHANGE, _LS(IDS_WG_TREEMENU_ADD3_Change_Load_Group), _T("ico24__structure_change_changeloadgroup"), m_strSvgPath);
		}
	}

//------------------------------------------------------------------------------------------------
// Static Load
//------------------------------------------------------------------------------------------------  
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Static_Loads), _T("ico24_temp"), m_strSvgPath);
	{

#ifdef _TOWER    //추가(김영민)
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TOWER_LOAD_GENERATOR, _LS(IDS_TM_Tower_Load_Generator), _T("ico24_temp"), m_strSvgPath);  
#endif						//추가(김영민)

		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_LOADCASE, _LS(IDS_TM_Static_Load_Cases), _T("ico24_load_createloadcases_staticloadcases"), m_strSvgPath);
#ifndef _TOWER
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_MAKELOAD, _LS(IDS_TM_Create_Load_Cases_Using_Load_Combinations), _T("ico24_load_createloadcases_usingloadcombinations"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BODYFORCE, _LS(IDS_TM_Self_Weight), _T("ico24_load_structureloadsmasses_selfweight"), m_strSvgPath);
#endif
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_NODALBODF, _LS(IDS_TM_DT_NBOF), _T("ico24_load_structureloadsmasses_nodalbodyforce"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_NODALLOADS, _LS(IDS_TM_Nodal_Loads), _T("ico24_load_structureloadsmasses_nodalloads"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_SPDISP, _LS(IDS_TM_Specified_Displacements_of_Supports), _T("ico24_load_structureloadsmasses_specifieddispl."), m_strSvgPath);
#ifndef _TOWER
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_EBLOAD, _LS(IDS_TM_Element_Beam_Loads), _T("ico24_load_beamload_element"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_LBLOAD, _LS(IDS_TM_Line_Beam_Loads), _T("ico24_load_beamload_line"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_TBLOAD, _LS(IDS_TM_Typical_Beam_Loads), _T("ico24_load_beamload_typical"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_DEFFBL, _LS(IDS_TM_Define_Floor_Load_Type), _T("ico24_load_initialforcesetc_definefloorloadtype"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_FBLOAD, _LS(IDS_TM_Assign_Floor_Loads), _T("ico24_load_initialforcesetc_assignfloorloads2"), m_strSvgPath);
// RUS에서는 마감하중 제외
#if !(defined(_RUS))
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_FMLOAD, _LS(IDS_TM_Finishing_Loads), _T("ico24_load_loadtables_finishingmaterialloads"), m_strSvgPath);
#endif
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_PRESSURE_TYPE, _LS(IDS_TM_Pressure_Load_TYPE), _T("ico24_load_pressureload_definepressureloadtype"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_PRESSURE, _LS(IDS_TM_Pressure_Loads), _T("ico24_load_pressureload_assignpressureloads"), m_strSvgPath);
#ifdef _MGEN 
		InsertTreeItem(hTI[nRootIndex], ID_LOAD_ASSIGN_AREA_PRESLOAD, _LS(IDS_DB_DT_ARPR), _T("ico24_load_pressureload_assignareapressureloads"), m_strSvgPath);
#endif
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_HYDPRESSURE, _LS(IDS_TM_Hydrostatic_Pressure_Loads), _T("ico24_load_pressureload_hydrostaticpressure"), m_strSvgPath);

#ifdef _MGEN_JP
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Earth_Pressure), _T("ico24_temp"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_PARAM_OF_SOIL_PROP, _LS(IDS_TM_Earth_Param_of_Soil_Prop	), _T("ico24_load_pressureload_parametersofsoilproperties"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_EARTHP_STATIC, _LS(IDS_TM_Earth_Pressure_Static		), _T("ico24_load_pressureload_seismicearthpressure"), m_strSvgPath);
#endif
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_DEFPNLD, _LS(IDS_TM_Define_Plane_Load_Type), _T("ico24_load_pressureload_defineplaneloadtype"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_BLOAD_PNLOAD, _LS(IDS_TM_Assign_Plane_Loads), _T("ico24_load_pressureload_assignplaneloads2"), m_strSvgPath);
		//InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_SWIB, _LS(IDS_WG_TREEMENU_ADD3_Self_Weight_of_Inactive_Beam), _T("ico24_temp"), m_strSvgPath);                   
		//-----------------------------
		if(CDBDoc::IsEnableWindPressure())
		{
			InsertTreeItem(hTI[nRootIndex], ID_LOAD_LOAD_AREA_PLANE, _LS(IDS_WG_TREEMENU_Loading_Area_Plane), _T("ico24_load_Initialforcesetc_loadingareaplane"), m_strSvgPath);
		}
		// Temperature Load
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Temperature__Loads), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TEMP_SYSTEM, _LS(IDS_TM_System_Temperature), _T("ico24_load_temperatureloads_systemtemp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TEMP_NODAL, _LS(IDS_TM_Nodal_Temperatures), _T("ico24_load_temperatureloads_nodaltemp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TEMP_ELEM, _LS(IDS_TM_Element_Temperatures), _T("ico24_load_temperatureloads_elementtemp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TEMP_GRADIENT, _LS(IDS_TM_Temperature_Gradient), _T("ico24_load_temperatureloads_tempgradient"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_BEAM_SECT_TEMP, _LS(IDS_TM_Beam_Section_Temperatures), _T("ico24_load_temperatureloads_beamsectiontemp"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_STRUCT_STATIC_PLATE_THICK_TEMP, _LS(IDS_TM_Plate_Thickness_Temperatures), _T("ico24_load_platethicktemperature"), m_strSvgPath);
		}
#endif
		//-----------------------------
		// Prestress Load
#ifndef _TOWER
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Prestress__Loads), _T("ico24_temp"), m_strSvgPath);
		{
		  InsertTreeItem(ThTi, ID_STRUCT_STATIC_BLOAD_PSTRESS, _LS(IDS_TM_Prestress_Beam_Loads), _T("ico24_load_prestressloads_prestressbeamloads"), m_strSvgPath);
		  InsertTreeItem(ThTi, ID_STRUCT_STATIC_PRETENSION, _LS(IDS_TM_Pretension_Loads), _T("ico24_load_prestressloads_pretensionloadsgen"), m_strSvgPath);
#if defined(_CIVIL) || defined(_MGEN)    
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_EXLD, _LS(IDS_TM_EXTERNAL_TYPE_LOADCASE_FOR_PRETENSION), _T("ico24_load_prestressloads_externaltypeloadcase"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TENDON_PROP, _LS(IDS_TM1_Tendon_Property___), _T("ico24_load_prestressloads_tendonproperty"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TENDON_ASSIGN, _LS(IDS_TM1_Tendon_Profile___), _T("ico24_load_prestressloads_tendonprofile"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TENDON_COMP, _LS(IDS_TM1_Tendon_Location_Comp_Sect___), _T("ico24_load_prestressloads_tendonlocationforcompositesection"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TENDON_CHANGE_PROF, _LS(IDS_MAIN_RIBBON_MENU_Change_Tendon_Profile), _T("ico24_load_prestressloads_tendonprofile"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_STATIC_TENDON_PRESTRESS, _LS(IDS_TM1_Tendon_Prestress_Loads___), _T("ico24_load_prestressloads_tendonprestress"), m_strSvgPath);
#endif
		}
		//-----------------------------
		// Construction Stage Load
#if defined(_CIVIL) || defined(_MGEN)
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_TIMELOAD, _LS(IDS_TM1_Time_Loads_For_Construction_Stage___), _T("ico24_load_constructionstagedata_timeloadsforconstructionstage"), m_strSvgPath);
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_CRPC, _LS(IDS_TM1_Creep_Coefficient_for_Construction_Stage___), _T("ico24_load_constructionstagedata_creepcoefficientforconstructionstage"), m_strSvgPath);
#endif
		//-----------------------------
		// Lateral Load
#ifdef _MGEN
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_WIND, _LS(IDS_TM_Wind_Loads), _T("ico24_load_lateral_windloads"), m_strSvgPath);
		if(CDBDoc::IsEnableWindPressure())
		{
			BOOL bCH  = CProduct::IsMovingType(D_PRODUCT_MOVING_CH);
			BOOL bORG = CProduct::IsMovingType(D_PRODUCT_MOVING_ORG);

			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Wind_Pressure), _T("ico24_temp"), m_strSvgPath);
			if(bORG || bCH)
			{
				InsertTreeItem(ThTi, ID_LOAD_WIND_PRESSURE_DEF_VELOCITY, _LS(IDS_TM_Wind_Pressure_Def_Velocity), _T("ico24_load_lateral_windpressure_velocitypressure"), m_strSvgPath);
			}
			InsertTreeItem(ThTi, ID_LOAD_WIND_PRESSURE_DEF_FUNCTION, _LS(IDS_TM_Wind_Pressure_Def_Function), _T("ico24_load_lateral_windpressure_windpressurefunction"), m_strSvgPath);
			if(bORG || bCH)
			{
				InsertTreeItem(ThTi, ID_LOAD_WIND_PRESSURE_AREA, _LS(IDS_TM_Wind_Pressure_Area), _T("ico24_load_lateral_windpressure_areawindpressure"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_LOAD_WIND_PRESSURE_BEAM, _LS(IDS_TM_Wind_Pressure_Beam), _T("ico24_load_lateral_windpressure_beamwindpressure"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_LOAD_WIND_PRESSURE_NODAL, _LS(IDS_TM_Wind_Pressure_Nodal), _T("ico24_load_lateral_windpressure_nodalwindpressure"), m_strSvgPath);
			}
			InsertTreeItem(ThTi, ID_LOAD_WIND_PRESSURE_FUNC, _LS(IDS_TM_Wind_Pressure_Function), _T("ico24_load_lateral_windpressure_functionwindpressure"), m_strSvgPath);
		}
		InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_SEISMIC, _LS(IDS_TM_Static_Seismic_Loads), _T("ico24_seismicperformance_load_pseudoseismicforce"), m_strSvgPath);

#ifndef _JP
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Earth_Pressure), _T("ico24_temp"), m_strSvgPath);
		if(CDBDoc::IsEnableEarthPressureSeis())
		{
			InsertTreeItem(ThTi, ID_STRUCT_PARAM_OF_SEIS_LOAD, _LS(IDS_TM_Earth_Param_of_Seis_Load	), _T("ico24_load_pressureload_parameterofseismicloads"), m_strSvgPath);
		}
		InsertTreeItem(ThTi, ID_STRUCT_PARAM_OF_SOIL_PROP, _LS(IDS_TM_Earth_Param_of_Soil_Prop	), _T("ico24_load_pressureload_parametersofsoilproperties"), m_strSvgPath);
		if(CDBDoc::IsEnableEarthPressureSeis())
		{
			InsertTreeItem(ThTi, ID_STRUCT_EARTHP_SEIS, _LS(IDS_TM_Earth_Pressure_Seismic		), _T("ico24_load_pressureload_seismicearthpressure"), m_strSvgPath);
		}
		InsertTreeItem(ThTi, ID_STRUCT_EARTHP_STATIC, _LS(IDS_TM_Earth_Pressure_Static		), _T("ico24_load_pressureload_seismicearthpressure"), m_strSvgPath);
#endif // !_JP

#endif    
		//-----------------------------
		// Initial Load
#if defined(_MGEN)
		//ThTi = InsertTreeItem(this,hTI[nRootIndex],ID_STRUCT_LOAD_INITIALFORCE_CONTROL,_LS(IDS_TM1_Initial_Forces_Control_Data___),1,1);
		//ThTi = InsertTreeItem(this,hTI[nRootIndex],ID_STRUCT_LOAD_INITIALFORCE,_LS(IDS_TM1_Initial_Forces_for_Geometric_Stiffness___),39,39);
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_INITIAL_FORCE_LARGE), _T("ico24_temp"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_LOAD_INITIALFORCE, _LS(IDS_TM1_Initial_Forces_for_Geometric_Stiffness___), _T("ico24_load_initialforcesetc_initialforcesforgeometricstiffness"), m_strSvgPath);
#ifdef _CH
		InsertTreeItem(ThTi, ID_STRUCT_LOAD_EQMF, _LS(IDS_TM1_EQUILIBRIUM_ELEMENT_FORCE_DATA), _T("ico24_load_initialforcesetc_euiilibriumelementnodalforces"), m_strSvgPath);
#endif
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_INITIAL_FORCE_SMALL), _T("ico24_temp"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_LOAD_INITIALFORCE_CONTROL, _LS(IDS_TM1_Initial_Forces_Control_Data___), _T("ico24_load_initialforcesetc_initialforcescontroldata"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_LOAD_INMF, _LS(IDS_TM_INITIAL_ELEMENT_FORCE), _T("ico24_load_initialforcesetc_initialelementforces"), m_strSvgPath);
#elif defined(_CIVIL)
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_INITIAL_FORCE_LARGE), _T("ico24_temp"), m_strSvgPath);
					 InsertTreeItem(ThTi, ID_STRUCT_LOAD_INITIALFORCE, _LS(IDS_TM1_Initial_Forces_for_Geometric_Stiffness___), _T("ico24_load_initialforcesetc_initialforcesforgeometricstiffness"), m_strSvgPath);
					 InsertTreeItem(ThTi, ID_STRUCT_LOAD_EQMF, _LS(IDS_TM1_EQUILIBRIUM_ELEMENT_FORCE_DATA), _T("ico24_load_initialforcesetc_euiilibriumelementnodalforces"), m_strSvgPath);
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_INITIAL_FORCE_SMALL), _T("ico24_temp"), m_strSvgPath);
					 InsertTreeItem(ThTi, ID_STRUCT_LOAD_INITIALFORCE_CONTROL, _LS(IDS_TM1_Initial_Forces_Control_Data___), _T("ico24_load_initialforcesetc_initialforcescontroldata"), m_strSvgPath);
					 InsertTreeItem(ThTi, ID_STRUCT_LOAD_INMF, _LS(IDS_TM_INITIAL_ELEMENT_FORCE), _T("ico24_load_initialforcesetc_initialelementforces"), m_strSvgPath);
		if (nMovingType == D_PRODUCT_MOVING_JP || nMovingType == D_PRODUCT_MOVING_ALL )
		{
			ThTi = InsertTreeItem(hTI[nRootIndex],  // this, 현재 노드의 바로 엄마 노드 
														ID_STRUCT_LOAD_SUPERSTRUCTURE_SEISMIC,  // 부착 시킬 리소스 아이디 
														_LS(IDS_TM1_Superstructure_Seismic_Load___), // 스트링 _T("Superstructure Seismic Load...") 
														IMAGE2_SIZE+29,IMAGE2_SIZE+29); // LoadBitmap(비트맵 소스 몇번째인지 계산 누적으로)일일이 계산해서 넣어준다. 
		}
#endif

#endif // _TOWER
		InsertTreeItem(ThTi, ID_STRUCT_BNDR_IGNORE, _LS(IDS_MAIN_RIB_MENU_BNDR_IGNORE_ELEMENT), _T("ico24_boundary_etc_ignoreelementsforloadcases"), m_strSvgPath);
	}
		//-----------------------------
		// Response Spectrum Analysis
#ifndef _TOWER  
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Response_Spectrum_Analysis), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_SPEC_DEFINE, _LS(IDS_TM_Response_Spectrum_Functions), _T("ico24_seismicperform_demand_rsfunctions"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_SPEC_ASSIGN, _LS(IDS_TM_Response_Spectrum_Load_Cases), _T("ico24_load_responsespectrumdata_rsloadcases"), m_strSvgPath);
		}

		//-----------------------------
		// Time History Analysis
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Time_History_Analysis), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_TH_GLOBAL_CONTROL, _LS(IDS_MAIN_RIBBON_MENU_Global_Control), _T("ico24_load_timehistoryanalysisdata_globalcontrol"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_BNDR_IEPI, _LS(IDS_DB_DT_IEPI), _T("ico24_pushover_control_ignoreelementsfornlanalysisinitialload"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HISTORY_LOADCASE, _LS(IDS_TM_Time_History_Load_Cases), _T("ico24_load_timehistoryanalysisdata_loadcases"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TIME_TFUNCTION, _LS(IDS_TM_Time_Forcing_Functions), _T("ico24_load_timehistoryanalysisdata_timehistoryfunction"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TIME_GROUND, _LS(IDS_TM_Ground_Acceleration), _T("ico24_load_timehistoryanalysisdata_groundacceleration"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TIME_NODAL, _LS(IDS_TM_Dynamic_Nodal_Loads), _T("ico24_load_timehistoryanalysisdata_dynamicnodalloads"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TIME_DYNSPA_LOAD, _LS(IDS_TM_Dynamic_Spatial_Loads), _T("ico24_load_timehistoryanalysisdata_timevaryingstaticloads"), m_strSvgPath);
			//InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TIME_DYNSPA_LOAD, _T("Dynamic Spatial Loads"), _T("ico24_load_timehistoryanalysisdata_timevaryingstaticloads"));
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_TIME_MULTI, _LS(IDS_TM_Multi_Support_Excitation), _T("ico24_load_timehistoryanalysisdata_multiplesupportexcitation"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_TIME_HISTORY_RESULT, _LS(IDS_TM_HISTORY_RESULT), _T("ico24_load_timehistoryanalysisdata_defineresultfunction"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_DEFINE_ANIMATION_TIME, _LS(IDS_TM_DEFINE_ANIMATION_TIME), _T("ico24_load_timehistoryanalysisdata_defineanimationtime"), m_strSvgPath);
		}

		//-----------------------------
		// Moving Load Analysis
		// GEN   : 국내, 미국, 대만, 인도만 
		// CIVIL : 모든 국가 
		// Moving Load Code만 추가해주고 나머지는 main > CMenuMgr 클래스에서 코드별 동적생성 
		nRootIndex++;
#if defined(_MGEN)
		if (nMovingType == D_PRODUCT_MOVING_ORG || nMovingType == D_PRODUCT_MOVING_ALL ||
				nMovingType == D_PRODUCT_MOVING_US  || nMovingType == D_PRODUCT_MOVING_RUS)
		{
			hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Moving_Load_Analysis), _T("ico24_temp"), m_strSvgPath);
			{
			  InsertTreeItem(hTI[nRootIndex], ID_STRUCT_MOV_LCODE, _LS(IDS_TM_Moving_Load_Code), _T("ico24_load_movingloadanalysisdata_movingloadcases"), m_strSvgPath);
			}
		}
#else
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Moving_Load_Analysis), _T("ico24_temp"), m_strSvgPath);
		{
		  InsertTreeItem(hTI[nRootIndex], ID_STRUCT_MOV_LCODE, _LS(IDS_TM_Moving_Load_Code), _T("ico24_load_movingloadanalysisdata_movingloadcases"), m_strSvgPath);
		}
#endif

		//-----------------------------
		// Crane Load Analysis Data
		// GEN   : 중국
		// CIVIL : .
#if defined(_MGEN)
		if (nMovingType == D_PRODUCT_MOVING_CH || nMovingType == D_PRODUCT_MOVING_ALL)
		{
			nRootIndex++;
			hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_CRAN), _T("ico24_temp"), m_strSvgPath);
			{
			  InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_CRANE, _LS(IDS_TREE_CRAN_SUB), _T("ico24_temp"), m_strSvgPath);
			}
		}
#endif

		//-----------------------------
		// Settlement Analysis 
#if defined(_MGEN)  // GEN은 국내와 일본버전에만 추가 : 2004.02.06 고영현 차장님 지시로 미국 버전에도 추가
		if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG)     || CProduct::GetMovingType() == D_PRODUCT_MOVING_JP ||
			 CProduct::GetMovingType() == D_PRODUCT_MOVING_US || CProduct::GetMovingType() == D_PRODUCT_MOVING_RUS)
		{
			nRootIndex++;
			hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Settlement_Analysis_Data), _T("ico24_temp"), m_strSvgPath);
			{
				InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_SETTLE, _LS(IDS_TM_Settlement_Group), _T("ico24_load_settlementanalysisdata_settlementgroup"), m_strSvgPath);
				InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_SETLCASE, _LS(IDS_TM_Settlement_Load_Cases), _T("ico24_load_settlementanalysisdata_settlementloadcase"), m_strSvgPath);
			}
		}
#else
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Settlement_Analysis_Data), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_SETTLE, _LS(IDS_TM_Settlement_Group), _T("ico24_load_settlementanalysisdata_settlementgroup"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_SETLCASE, _LS(IDS_TM_Settlement_Load_Cases), _T("ico24_load_settlementanalysisdata_settlementloadcase"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_CRGR, _LS(IDS_TM_Concurrent_Reaction_Group), _T("ico24_load_settlementanalysisdata_concurrentreactiongroup"), m_strSvgPath);
		}
#endif

#endif // Tower

#ifndef _TOWER

		//-----------------------------
		// Composite Section Analysis
#if defined(_CIVIL) || defined(_MGEN)
//#if !defined(_JP) || !defined(_MGEN)  //1 added by KYM  : Gen-JP 푼다. 05.08.16
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Composite_Bridge_Analysis_Data), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_COMPBRIDGE, _LS(IDS_TM_Pre_Combined_Load_Cases_for_Composite_Bridge), _T("ico24_load_misc_precompositesection"), m_strSvgPath);
		}
//#endif
#endif
		//-----------------------------
		// Heat of Hydration Analysis
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Hydration_Heat_Analysis_Data), _T("ico24_temp"), m_strSvgPath);
		{
			//InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION, _LS(IDS_TM_Hydration_Heat_Analysis_Control), _T("ico24_analysis_analysiscontrol_heatofhydration"), m_strSvgPath);
			//InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_MATERIAL_TYPE, _LS(IDS_TM_Hydration_Heat_Material_Type), _T("ico24_temp"), m_strSvgPath);
			//InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_MATERIAL, _LS(IDS_TM_Hydration_Heat_Material), _T("ico24_temp"), m_strSvgPath);     
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_ENVTEMP, _LS(IDS_TM_Ambient_Temperature_Functions), _T("ico24_load_heatofhydrationanalysisdata_ambienttemperaturefunctions"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_CCFC, _LS(IDS_TM_Convection_Coefficient_Functions___), _T("ico24_load_heatofhydrationanalysisdata_convectioncoefficientfunctions"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_CONVECTION, _LS(IDS_TM_Element_Convection_Boundary), _T("ico24_load_heatofhydrationanalysisdata_elementconvectionboundary"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_SPTEMP, _LS(IDS_TM_Prescribed_Temperature), _T("ico24_load_heatofhydrationanalysisdata_prescribedtemp"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_HEATSOURCE_DEFINE, _LS(IDS_TM_Heat_Source_Functions), _T("ico24_load_heatofhydrationanalysisdata_heatsourcefunctions"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_HEATSOURCE_ASSIGN, _LS(IDS_TM_Assign_Heat_Source), _T("ico24_load_heatofhydrationanalysisdata_assignheatsource"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_COOLING, _LS(IDS_TM_Pipe_Cooling), _T("ico24_load_heatofhydrationanalysisdata_pipecooling"), m_strSvgPath);
			//InsertTreeItem(hTI[nRootIndex], ID_STRUCT_HYDRATION_CREEP, _LS(IDS_TM_Creep___Shrinkage), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_DEF_STAGE_HYD, _LS(IDS_TM1_Construction_Stage_for_Hydration), _T("ico24_load_heatofhydrationanalysisdata_definecsforhydration"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STAGE_CHANGE, _LS(IDS_TM1_Select_Construction_Stage_for_Display___), _T("ico24_load_constructionstagedata_definecsgen"), m_strSvgPath);
		}
		//-----------------------------
		// Non Linear Analysis
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM1_Non_Linear_Analysis_Data), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_NONLINEAR_LOADSEQ, _LS(IDS_TM1_Loading_Sequence_for_Non_Linear_Analysis___), _T("ico24_load_misc_loadsequencefornonlinear"), m_strSvgPath);
		}

		// Imperfection Data
#ifdef _MGEN
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_IMPERFECTION), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_IMPERFECTION_LOAD_DATA, _LS(IDS_TM_IMPERFECTION_DATA), _T("ico24_load_misc_imperfection_imperfectiondata"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STATIC_CREATE_IMPERFECTION_LOAD, _LS(IDS_TM_CREATE_IMPERFECTION_LOAD), _T("ico24_load_misc_imperfection_createimperfectionload"), m_strSvgPath);
		}
#endif

		//-----------------------------
		// Construction Stage Analysis
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM1_Construction_Stage_Analysis_Data), _T("ico24_temp"), m_strSvgPath);
		{
#ifdef _MGEN
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STAGE_BUILDING, _LS(IDS_TM1_Construction_Stage_Wizard_fro_Building), _T("ico24_load_constructionstagedata_constructionstagewizard"), m_strSvgPath);
#endif
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_DEF_STAGE, _LS(IDS_TM1_Define_Construction_Stage___), _T("ico24_load_constructionstagedata_definecsgen"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STAGE_CHANGE, _LS(IDS_TM1_Select_Construction_Stage_for_Display___), _T("ico24_load_constructionstagedata_definecsgen"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_STAGE_SECTION, _LS(IDS_TM1_Composite_Section_For_Construction_Stage), _T("ico24_load_constructionstagedata_compositesectionforcs"), m_strSvgPath);
		}

/*
		//-----------------------------
		// Wave Load
#if defined(_CIVIL)
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_WAVE_LOAD), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_WAVE_LOAD_PARAM, _LS(IDS_TM_WAVE_LOAD_PARAM), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_WAVE_LOAD_PLOT, _LS(IDS_TM_WAVE_LOAD_PLOT), _T("ico24_temp"), m_strSvgPath);
		}
#endif
		//-----------------------------
*/
#endif// _TOWER

//------------------------------------------------------------------------------------------------
// Grid Model
//------------------------------------------------------------------------------------------------  
// Icon 만들어서 넣어주세요..
#ifdef _CIVIL
	if (CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
	{
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_TREEMENU_GRID_MODEL_ANALYSIS_DATA), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_MAIN_GIRDER, _LS(IDS_TM_TREEMENU_GRID_MAIN_GIRDER), _T("ico24_loads_grid_jpn3"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_CROSS_BEAM, _LS(IDS_TM_TREEMENU_GRID_CROSS_BEAM), _T("ico24_loads_grid_jpn4"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_LOAD_LINE, _LS(IDS_TM_TREEMENU_GRID_LOAD_LINE), _T("ico24_loads_grid_jpn5"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_GANAL_LOAD, _LS(IDS_TM_TREEMENU_GRID_GRID_ANALYSIS_LOAD), _T("ico24_loads_grid_jpn6"), m_strSvgPath);      
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_BRIDGE_TYPE, _LS(IDS_TM_TREEMENU_GRID_BRIDGE_TYPE), _T("ico24_loads_grid_jpn7"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_SPAM_LENGTH, _LS(IDS_TM_TREEMENU_GRID_SPAN_LENGTH), _T("ico24_loads_grid_jpn8"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_ELEMSTRUCT_TYPE, _LS(IDS_TM_TREEMENU_GRID_STRUCTURE_TYPE), _T("ico24_loads_grid_jpn9"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_IMPACT_FACTOR, _LS(IDS_TM_TREEMENU_GRID_IMPACT_FACTOR), _T("ico24_loads_grid_jpn10"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_P1LOAD_FACTOR, _LS(IDS_TM_TREEMENU_GRID_P1LOAD_FACTOR), _T("ico24_loads_grid_jpn11"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_IMPACT_P1LOAD_FACTOR, _LS(IDS_TM_TREEMENU_GRID_IMPACT_N_P1LOAD_FACTOR), _T("ico24_loads_grid_jpn24"), m_strSvgPath);      
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_CROSS_REFERENCE, _LS(IDS_TM_TREEMENU_GRID_CROSS_REFERENCE), _T("ico24_loads_grid_jpn4"), m_strSvgPath);
			InsertTreeItem(hTI[nRootIndex], ID_STRUCT_GRID_LOAD_CONVERTER, _LS(IDS_TM_TREEMENU_GRID_LOAD_CONVERTER), _T("ico24_loads_grid_jpn14"), m_strSvgPath);      
		}
	}
#endif
//------------------------------------------------------------------------------------------------
// Result
//------------------------------------------------------------------------------------------------  
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Results), _T("ico24_temp"), m_strSvgPath);
	{
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_COMBINATION, _LS(IDS_TM_Combinations), _T("ico24_results_combination_loadcombination"), m_strSvgPath);
#if defined(_CIVIL)
	#ifdef _JP
		// MNET:XXXX-CJJEONG-20080317 JP일때만 사용
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_RECIPROCAL_STRESS, _LS(IDS_TM_LOADCASES_FOR_RECIPROCAL_STRESS), _T("ico24_results_detail_reductionmoment"), m_strSvgPath);
	#endif
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_REDUCTION_MOMENT, _LS(IDS_TM_Reduction_Moment), _T("ico24_results_detail_reductionmoment"), m_strSvgPath);
#endif

#if !defined (_MGEN) && !defined (_CIVIL) && !defined (_TOWER)  //수정(김영민)
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_ENVELOPE, _LS(IDS_TM_Envelope), _T("ico24_temp"), m_strSvgPath);

#endif		//수정(김영민)
		///////////////////////
		// Reactions
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Reactions), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_REACTION_REACF, _LS(IDS_TM_Reaction_Forces_Moments), _T("ico24_results_results_reactionforcesmoments"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_REACTION_SRHRCF, _LS(IDS_TM_Search_Reaction_Forces_Moments), _T("ico24_results_results_searchreactionforcesmoments"), m_strSvgPath);
#ifdef _MGEN_CH
			InsertTreeItem(ThTi, ID_RESULT_REACTION_TARGET, _LS(IDS_TM_Target_Reaction_Forces_Moments), _T("ico24_temp"), m_strSvgPath);
#endif
			
			InsertTreeItem(ThTi, ID_RESULT_SOIL_PRESS, _LS(IDS_TM_Soil_Pressure), _T("ico24_results_results_soilpressure"), m_strSvgPath);
		}
		////////////////////////
		// Deformations
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Deformations), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_DEFORM_DEFORM, _LS(IDS_TM_Deformed_Shape), _T("ico24_results_results_deformedshape"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_DEFORM_DCONTR, _LS(IDS_TM_Displacement_Contour), _T("ico24_results_results_displacementcontour"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_DEFORM_SRHDSP, _LS(IDS_TM_Search_Displacements), _T("ico24_results_results_searchdisplacements"), m_strSvgPath);
#ifdef _MGEN
			if(CProduct::IsMovingType(D_PRODUCT_MOVING_US)||CProduct::IsMovingType(D_PRODUCT_MOVING_CH))
			{
				InsertTreeItem(ThTi, ID_RESULT_COLUMN_DISP, _LS(IDS_TM_Deform_Colmdisp), _T("ico24_results_results_deformedshape"), m_strSvgPath);
			}
#endif
		}
		////////////////////////
		// Forces
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Forces), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_FORCES_TFORC, _LS(IDS_TM_Truss_Forces), _T("ico24_results_results_trussforces"), m_strSvgPath);

#ifndef _TOWER
			InsertTreeItem(ThTi, ID_RESULT_FORCES_BFORC, _LS(IDS_TM_Beam_Forces_Moments), _T("ico24_results_results_beamforcesmoments"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_FORCES_SFDBMD, _LS(IDS_TM_Beam_SFD_BMD__Chart_), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_FORCES_BDGRM, _LS(IDS_TM_Beam_Diagrams), _T("ico24_results_results_beamdiagrams"), m_strSvgPath);
			
			InsertTreeItem(ThTi, ID_RESULT_FORCES_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_results_plateforcesmoments"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_FORCES_PFORC_CLWP, _LS(IDS_TM_Plate_Forces_Moments_Cutting_Diagram), _T("ico24_results_results_platecuttinglinediagram"), m_strSvgPath);
#ifdef _MGEN
			InsertTreeItem(ThTi, ID_RESULT_FORCES_WFORC, _LS(IDS_TM_Wall_Forces_Moments), _T("ico24_results_results_wallforcesmoments"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_FORCES_WSFBM, _LS(IDS_TM_Wall_Diagrams), _T("ico24_results_results_walldiagrams"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_FORCES_ESFBM, _LS(IDS_TM_Member_Diagrams), _T("ico24_results_results_memberdiagrams"), m_strSvgPath);
#endif
#endif
		}
		////////////////////////
		// Stresses
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Stresses), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_STRESS_TSTRS, _LS(IDS_TM_Truss_Stresses), _T("ico24_results_results_trussstresses"), m_strSvgPath);
#ifndef _TOWER
			InsertTreeItem(ThTi, ID_RESULT_STRESS_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_results_beamstresses"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_STRESS_BSTRSDGRM, _LS(IDS_TM_Beam_Stresses_Diagram), _T("ico24_results_results_beamstressesdiagram"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_STRESS_BSTRSDETAIL, _LS(IDS_TM_Beam_StressesDetail), _T("ico24_results_results_beamstresses(equivalent)"), m_strSvgPath);
			//#ifdef _DEBUG
			InsertTreeItem(ThTi, ID_RESULT_STRESS_BSTRSDETAILDGRM, _LS(IDS_TM_Beam_StressesDetail_Diagram), _T("ico24_results_results_beamstressesdiagram"), m_strSvgPath);
			//#endif
#ifdef _CIVIL
			InsertTreeItem(ThTi, ID_RESULT_STRESS_BSTRSINC, _LS(IDS_TM_Beam_Stresses_PSC), _T("ico24_results_results_beamstresses(psc)"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_RESULT_STRESS_PSTRS, _LS(IDS_TM_Plane_Stress_Plate_Stresses), _T("ico24_results_results_planestressplatestresses"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_STRESS_PNSTRS, _LS(IDS_TM_Plane_Strain_Stresses), _T("ico24_results_results_planestrainstresses"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_STRESS_AXSTRS, _LS(IDS_TM_Axisymmetric_Stresses), _T("ico24_results_results_axisymmetricstresses"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_STRESS_SSTRS, _LS(IDS_TM_Solid_Stresses), _T("ico24_results_results_solidstresses"), m_strSvgPath);

			if (CDBLib::ActivateVBEM()) // 960_R2
			{
				InsertTreeItem(ThTi, ID_RESULT_STRESS_SECV, _LS(IDS_MAIN_RIBBON_RESULT_STRESS_SECV), _T("ico24_results_results_beamstressesdiagram"), m_strSvgPath);
			}
#endif
		}
//#ifdef _CIVIL

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Strain), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_STRAIN_PLATE, _LS(IDS_TM_Plate_Strain), _T("ico24_results_results_platestrain"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_STRAIN_SOLID, _LS(IDS_TM_Solid_Strain), _T("ico24_results_results_solidstrain"), m_strSvgPath);
		}

		////////////////////////
		// User Defined Diagram

#if defined(_MGEN) && defined(_ORG)
		// 없음 
#else
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_GPS_User_Defined_Diagram), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_UDGRM_DEFN, _LS(IDS_TM_GPS_Define_Diagram), _T("ico24_results_results_definediagram"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_UDGRM_PLOT, _LS(IDS_TM_GPS_Plot_Diagram), _T("ico24_results_results_plotdiagram"), m_strSvgPath);
		}
#endif

		////////////////////////
		// Hydration
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Hydration_heat_Analysis), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_HYDRAHEAT_SSTRS, _LS(IDS_TM_Stress), _T("ico24_results_results_stress"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_HYDRAHEAT_TEMPER, _LS(IDS_TM_Temperature), _T("ico24_results_results_temperature"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_HYDRAHEAT_DISPL, _LS(IDS_TM_Displacement), _T("ico24_results_results_displacements"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_HYDRAHEAT_ALTENSSTRS, _LS(IDS_TM_Allowable_Tensile_Stress), _T("ico24_results_results_allowabletensilestress"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_HYDRAHEAT_CRACKRATIO, _LS(IDS_TM_Crack_Ratio), _T("ico24_results_results_crackratio"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_HYDRAHEAT_GRAPH, _LS(IDS_TM_Graph), _T("ico24_results_results_graph"), m_strSvgPath);
		}
//#endif
#ifndef _TOWER
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_BEAMDETAIL, _LS(IDS_TM_Beam_Detail_Analysis), _T("ico24_results_detail_beamdetailanalysis"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_PEEK, _LS(IDS_TM_Element_Detail_Results), _T("ico24_results_detail_elementdetailresult"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_NORMALFORCSUM, _LS(IDS_TM_Local_Direction_Force_Sum), _T("ico24_results_detail_localdirectionforcesum"), m_strSvgPath);
#ifdef _MGEN_CH
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_BEAMTARGET, _LS(IDS_TM_Beam_Target_Force), _T("ico24_temp"), m_strSvgPath);
#endif
#ifdef _MGEN
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_DISP_PART_FACT, _LS(IDS_WG_TREEMENU_Displacement_Participation_Factor), _T("ico24_results_detail_displacementparticipationfactor"), m_strSvgPath);
#endif
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_DYNAMIC_EGMDSHP, _LS(IDS_TM_Vibration_Mode_Shapes), _T("ico24_results_modeshape_vibrationmodeshapes"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_MODAL_DAMPING_RATIO, _LS(IDS_TM_Modal_Damping_Ratio), _T("ico24_results_modeshape_modaldampingratio"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_DYNAMIC_BKMDSHP, _LS(IDS_TM_Buckling_Mode_Shapes), _T("ico24_results_modeshape_bucklingmodeshapes"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_DYNAMIC_NRRS, _LS(IDS_WG_TREEMENU_Nodal_Results_of_RS), _T("ico24_results_modeshape_nodalresultsofrs"), m_strSvgPath);
		////////////////////////
		// Time History
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_WG_TREEMENU_Time_History_Results), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_TIMEHISTORY_DISP, _LS(IDS_WG_TREEMENU_Time_History_Displacement), _T("ico24_results_timehistory_displvelaccel"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_TIMEHISTORY_BFORC, _LS(IDS_TM_TREEMENU_Force_Beam_Truss), _T("ico24_results_timehistory_force(beamwalltrussgenerallink)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_TIMEHISTORY_BDGRM, _LS(IDS_TM_TREEMENU_Diagram_Beam_Truss), _T("ico24_results_timehistory_forcediagram(beamwalltrussgenerallink)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_TIMEHISTORY_BSTRS, _LS(IDS_TM_TREEMENU_Stress_Beam_Truss), _T("ico24_results_timehistory_stress(beamtruss)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_ITHA_DUCKTILITY_FACT, _LS(IDS_WG_TREEMENU_Inelastic_Hinge_Status), _T("ico24_results_timehistory_inelastichingestatus"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_RESULT_OF_FIBER_SECT, _LS(IDS_TM_TREEMENU_Analysis_Result_Fiber), _T("ico24_results_timehistory_analysisresultoffibersection"), m_strSvgPath);
#ifdef _JP
			HTREEITEM ThTiFiber=InsertTreeItem(ThTi, 0, _LS(IDS_TM_Section_Damage_Check_with_Fiber_Element), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTiFiber, ID_RESULT_FIBER_SECT_CHECK_JCSS, _LS(IDS_TM_Japanese_Concrete_Standard_Specification), _T("ico24_result_damagedsection_residualelasticstiffness"), m_strSvgPath);
			InsertTreeItem(ThTiFiber, ID_RESULT_FIBER_SECT_CHECK_NAGOYA, _LS(IDS_TM_Nagoya_Highway_Corporation), _T("ico24_temp"), m_strSvgPath);
#endif

			InsertTreeItem(ThTi, ID_RESULT_TIMEHISTORY_GRAPH, _LS(IDS_TM_Time_History_Graph), _T("ico24_results_timehistory_timehistorygraph"), m_strSvgPath);
#ifdef _MGEN
			InsertTreeItem(ThTi, ID_RESULT_STORYSHEAR_GRAPH, _LS(IDS_WG_TREEMENU_Story_Shear_Graph), _T("ico24_results_timehistory_storygraph"), m_strSvgPath);
#endif
			InsertTreeItem(ThTi, ID_RESULT_TIMEHISTORY_TEXT, _LS(IDS_TM_Time_History_Text), _T("ico24_results_timehistory_timehistorytext"), m_strSvgPath);
		}
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_STAGESTEP_GRAPH, _LS(IDS_WG_TREEMENU_Stage_Step_Graph), _T("ico24_results_timehistory_stagestepgraph"), m_strSvgPath);

#ifdef _MGEN
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_STORYCOLUMN_GRAPH, _LS(IDS_WG_TREEMENU_Story_Column_Shortening_Graph), _T("ico24_results_misc_columnshorteninggraphforcs"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_RESULT_SSFR, _LS(IDS_TM_Story_Shear_Force_Ratio__), _T("ico24_results_misc_storyshearforceratio"), m_strSvgPath);
#endif
		////////////////////////
		// Moving Load
		/*
		BOOL bIndia = CProduct::GetMovingType()==D_PRODUCT_MOVING_US && CProduct::IsIndia();

		switch(CProduct::GetMovingType())
		{
		case D_PRODUCT_MOVING_JP:   // 일본 (CIVIL만)
			#if defined(_CIVIL)
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Lines), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_INFLLINE_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			}		
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Moving_Load_Tracer), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_MVLTRC_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			} 
			#endif
			break;
		case D_PRODUCT_MOVING_CH:   // 중국 (CIVIL만)
			#if defined(_CIVIL)
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Lines), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_INFLLINE_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			}		
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Surfaces), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
	      InsertTreeItem(ThTi, ID_RESULT_INFLSURF_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
			}
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Moving_Load_Tracer), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_MVLTRC_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			} 
			#endif
			break;
		case D_PRODUCT_MOVING_US:   // 미국 (GEN만 빼고)
			#if !defined(_MGEN)
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Lines), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
		  	InsertTreeItem(ThTi, ID_RESULT_INFLLINE_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			}	
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Moving_Load_Tracer), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_MVLTRC_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			}
			if(!bIndia)
			{
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Surfaces), _T("ico24_temp"), m_strSvgPath);
		    {
					InsertTreeItem(ThTi, ID_RESULT_INFLSURF_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
	        InsertTreeItem(ThTi, ID_RESULT_INFLSURF_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RESULT_INFLSURF_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RESULT_INFLSURF_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RESULT_INFLSURF_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				}
			}
			#endif
			break;
		default:  // 나머지는 국내와 같게
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Lines), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
		  	InsertTreeItem(ThTi, ID_RESULT_INFLLINE_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			}			
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Surfaces), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
	      InsertTreeItem(ThTi, ID_RESULT_INFLSURF_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLSURF_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
			}
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Moving_Load_Tracer), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_REAC, _LS(IDS_TM_Reactions)           ), _T("ico24_results_movingload_reactions"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_MVLTRC_DISP, _LS(IDS_TM_Displacements)       ), _T("ico24_results_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_TFORC, _LS(IDS_TM_Truss_Forces)        ), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BFORC, _LS(IDS_TM_Beam_Forces_Moments) ), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_MVLTRC_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_results_movingload_beamstresses"), m_strSvgPath);
			}
			break;
		}
		*/

		////////////////////////
		// Unknown Load Factor
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_UNKNOWNLOAD, _LS(IDS_TM_Unknown_Load_Factor), _T("ico24_results_misc_unknownloadfactor"), m_strSvgPath);
#if defined(_MGEN) && defined(_CH)
		// Stability Check
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_STABILITY_CHECK, _LS(IDS_TM_Stability_Check), _T("ico24_temp"), m_strSvgPath);
		// Story Yield Strength Factor Check
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_STRYLD_STR_CHECK, _LS(IDS_TM_Story_Yield_Strength_Factor_Check), _T("ico24_temp"), m_strSvgPath);
#endif

#ifdef	_MVIL
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Design), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_DESIGN_VILLA, _LS(IDS_TM_Member_Design), _T("ico24_temp"), m_strSvgPath);
		}
#endif
		
#ifdef _CIVIL
		if(CProduct::IsMovingType(D_PRODUCT_MOVING_JP))
		{
			// Influence Grid
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Grid), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_RESULT_INFLGRID_REAC, _LS(IDS_TM_Reaction_Forces_Moments), _T("ico24_loads_detail_reductionmoment"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_RESULT_INFLGRID_DISP, _LS(IDS_TM_Displacement), _T("ico24_loads_movingload_displacements"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLGRID_TFORC, _LS(IDS_TM_Truss_Forces), _T("ico24_loads_results_trussforces"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLGRID_BFORC, _LS(IDS_TM_Beam_Forces_Moments), _T("ico24_loads_results_forces"), m_strSvgPath);
				//InsertTreeItem(ThTi, ID_RESULT_INFLGRID_BSTRS, _LS(IDS_TM_Beam_Stresses), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLGRID_EFORC, _LS(IDS_TM_Ellk_Forces_Moments), _T("ico24_loads_link_elasticlink2"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_RESULT_INFLGRID_NFORC, _LS(IDS_TM_Nllk_Forces_Moments), _T("ico24_loads_link_generallink"), m_strSvgPath);
	    }

			// MNET:XXXX-CJJEONG-20080317
			InsertTreeItem(hTI[nRootIndex], ID_RESULT_CREF_TEXT, _LS(IDS_TM_CROSS_REFERENCE_TEXT_OUTPUT), _T("ico24_load_movingloadanalysisdata_concurrentreactiongroup"), m_strSvgPath);
		}


		// FCM Camber
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_FCM_Camber), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_CAMBER_CONTROL, _LS(IDS_TM_FCM_Camber_Control), _T("ico24_results_bridge_fcmcambercontrol"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_CAMBER_GRAPH, _LS(IDS_TM_FCM_Camber_Graph_View), _T("ico24_results_bridge_fcmcambergraphview"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_CAMBER_TABLE, _LS(IDS_TM_FCM_Camber_Table), _T("ico24_results_bridge_fcmcambertable"), m_strSvgPath);
	  }
		// General Camber
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_GEN_Camber), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_GENCAMBER_CONTROL, _LS(IDS_TM_GEN_Camber_Control), _T("ico24_results_bridge_generalcambercontrol"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_GENCAMBER_GRAPH, _LS(IDS_TM_GEN_Camber_Graph_View), _T("ico24_results_bridge_generalcambergraphview"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_RESULT_GENCAMBER_TABLE, _LS(IDS_TM_GEN_Camber_Table), _T("ico24_results_bridge_generalcambertable"), m_strSvgPath);
	  }
		////////////////////////
		// ILM Reaction
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_ILM_Reaction), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_RESULT_RPOS_DATA, _LS(IDS_TM_ILM_RPOS_DATA), _T("ico24_results_bridge_reactionbypositiondata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RPOS_L, _LS(IDS_TM_ILM_RPOS_TABLE_LOCAL), _T("ico24_results_bridge_reactionbypositiontable(local)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RPOS, _LS(IDS_TM_ILM_RPOS_TABLE_GLOBAL), _T("ico24_results_bridge_reactionbypositiontable(global)"), m_strSvgPath);
	  }
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_GIRDERSTRS_DGRM, _LS(IDS_WG_TREEMENU_Girder_Stress_Diagram), _T("ico24_results_bridge_bridgegirderdiagram"), m_strSvgPath);
#endif
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_TENDON_TIMEGRAPH, _LS(IDS_TM_Tendon_Time_Dependent_Loss_Graph), _T("ico24_results_misc_tendonlossgraph"), m_strSvgPath);
		
// Mode Shapes
//		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Dynamic_Results), _T("ico24_temp"), m_strSvgPath);
//		{
//			InsertTreeItem(ThTi, ID_RESULT_DYNAMIC_EGMDSHP, _LS(IDS_TM_Vibration_Mode_Shape), _T("ico24_results_modeshape_vibrationmodeshapes"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_DYNAMIC_BKMDSHP, _LS(IDS_TM_Buckling_Mode_Shape), _T("ico24_results_modeshape_bucklingmodeshapes"), m_strSvgPath);
//		}
		// Influence Lines
//		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Lines), _T("ico24_temp"), m_strSvgPath);
//		{
//			InsertTreeItem(ThTi, ID_RESULT_INFLLINE_REAC, _LS(IDS_TM_Reactions), _T("ico24_results_movingload_reactions"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLLINE_DISP, _LS(IDS_TM_Displacements), _T("ico24_results_movingload_displacements"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLLINE_TFORC, _LS(IDS_TM_Truss_Forces_Moments), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLLINE_BFORC, _LS(IDS_TM_Beam_Forces_Moments), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLLINE_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
//		}
		// Influence Surfaces
//		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Influence_Surfaces), _T("ico24_temp"), m_strSvgPath);
//		{
//			InsertTreeItem(ThTi, ID_RESULT_INFLSURF_REAC, _LS(IDS_TM_Reactions), _T("ico24_results_movingload_reactions"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLSURF_DISP, _LS(IDS_TM_Displacements), _T("ico24_results_movingload_displacements"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLSURF_TFORC, _LS(IDS_TM_Truss_Forces_Moments), _T("ico24_results_movingload_trussforces"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLSURF_BFORC, _LS(IDS_TM_Beam_Forces_Moments), _T("ico24_results_movingload_beamforcesmoments"), m_strSvgPath);
//			InsertTreeItem(ThTi, ID_RESULT_INFLSURF_PFORC, _LS(IDS_TM_Plate_Forces_Moments), _T("ico24_results_movingload_plateforcesmoments"), m_strSvgPath);
//		}
	}
#endif

//------------------------------------------------------------------------------------------------
// Design
//------------------------------------------------------------------------------------------------  
#ifndef _CIVIL
	// Change by ZINU.('01.1.5).
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
	{
#ifdef _TOWER
		CString strTitle = _T("");
		BOOL bShow = FALSE;
		// Tower Design.
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_DGN_STL_DSTL, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_DESIGN_TOWER_MATL, _LS(IDS_TM1_Material_for_Design), _T("ico24_temp"), m_strSvgPath);
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_STRUCT_PROP_SECT4DGN, _LS(IDS_TM1_Section_for_Design), _T("ico24_desgin_design_sectionfordesign"), m_strSvgPath);
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_DGN_TOWER_DVMT_ADD, _LS(IDS_TM1_Design_Parameters), _T("ico24_temp"), m_strSvgPath);
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_DGN_TOWER_BOLT_ADD, _LS(IDS_TM1_Bolt_Design_Parameters), _T("ico24_temp"), m_strSvgPath);
#endif
#ifndef _TOWER
		CString strTitle = _T("");
		BOOL bShow = FALSE;
		/////////////////////////
		// General.
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_GEN_PARAM), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_DGN_GEN_DFRM, _LS(IDS_TREE_TEXT_GEN_DEF_FRAME), _T("ico24_rating_bridgeratingdesign_definitionofframe"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_REDU_AUTO, _LS(IDS_TREE_TEXT_GEN_LIVE_REDUCT_FACTOR), _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
			// -------------------------------------------------------------------
			InsertTreeItem(ThTi, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_HCBM, _LS(IDS_TREE_TEXT_GEN_HAUNCH), _T("ico24_design_general_haunchedbeamassignment"), m_strSvgPath);
			strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
			InsertTreeItem(ThTi, ID_DGN_GEN_LENG, strTitle, _T("ico24_rating_bridgeratingdesign_unbracedlength(l,lb)"), m_strSvgPath);
			strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
			InsertTreeItem(ThTi, ID_DGN_GEN_KFAC, strTitle, _T("ico24_rating_bridgeratingdesign_effectivelengthfactor(k)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_rating_bridgeratingdesign_limitingslendernessratio"), m_strSvgPath);
			strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_momentfactor(cm)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_momentmagnifier(b1delta-b,b2delta-s)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_REDU, _LS(IDS_TREE_TEXT_GEN_MOD_L_REDUCT_FACTOR), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
			#if !(defined(_US) || defined(_RUS)) // Change by ZINU.('02.12.20). <- #if defined(_CH).
			InsertTreeItem(ThTi, ID_DGN_GEN_SUEQ, _LS(IDS_TREE_TEXT_GEN_SUEQ), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_GEN_GREQ, _LS(IDS_TREE_TEXT_GEN_GREQ), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
			#endif
			InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);
			#if defined(_JP) || defined(_ORG)
			InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath);
			#endif
			// Coded by Seungjun MNet:No.2393 ('20060919)  
			#if defined(_US) || defined(_RUS) || defined(_ORG)
			InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_mods_design_dropmenu2_serviceabilityloadcombinationtype"), m_strSvgPath);
			#endif

		  InsertTreeItem(ThTi, ID_DGN_GEN_LCTB, _LS(IDS_TM_LOAD_CONTRIBUTION), _T("ico24_mods_design_loadcontributionfornonlinearloadcase"), m_strSvgPath);
			#if defined(_ORG)
			//InsertTreeItem(ThTi, ID_DGN_GEN_SEIS_FACTOR, _LS(IDS_TREE_TEXT_GEN_SEIS_FACTOR), _T("ico24_temp"), m_strSvgPath);
			strTitle = _LS(IDS_TREE_TEXT_GEN_SEIS_TYPE);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SEIS, strTitle, bShow);
			InsertTreeItem(ThTi, ID_DGN_GEN_SEIS, strTitle, _T("ico24_design_general_seismicloadcombinationtype"), m_strSvgPath);

			strTitle = _LS(IDS_TREE_TEXT_GEN_SEIS_DGN_TYPE);
			InsertTreeItem(ThTi, ID_DGN_GEN_SDGN, strTitle, _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
			#endif
			strTitle = _LS(IDS_TREE_TEXT_GEN_ULCT);
			InsertTreeItem(ThTi, ID_DGN_GEN_ULCT, strTitle, _T("ico24_design_general_undergroundloadcombinationtype"), m_strSvgPath);
		}
		/////////////////////////
		// Steel.
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_PARAM), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_DGN_STL_DSTL, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
			strTitle = _LS(IDS_TREE_TEXT_STL_ST_REDUCT_FACTOR_LSD);
			CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DSTL_SRED, strTitle, bShow);
			InsertTreeItem(ThTi, ID_DGN_STL_DSTL_SRED, strTitle, _T("ico24_mods_design_dropmenu1_strengthreductionfactor"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_MATD, _LS(IDS_TREE_TEXT_STL_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifysteelmaterial"), m_strSvgPath);
	 /*
	  //add by maxiao(2015-10-27)GB50017-15
	   CString strStlCode = CRegistry::AppGetProfileString( _T("Settings"), _T("StlCode"), _T("") );
	   if(strStlCode==_T("GB50017-15")) 
	  {
		  InsertTreeItem(ThTi, ID_DGN_STL_SECTD, _LS(IDS_TREE_TEXT_STL_MOD_SECT), _T("ico24_mods_design_dropmenu1_modifysteelmaterial"), m_strSvgPath);
	  }
	   */
			InsertTreeItem(ThTi, ID_DGN_STL_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_dropmenu1_serviceabilityparameters"), m_strSvgPath);
			// -------------------------------------------------------------------
			strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
			CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_SLRS, _LS(IDS_TREE_TEXT_STL_SLRS), _T("ico24_design_design_dropmenu1_seismicloadresistingsystembymember"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_DESIGN_STEEL_UCFA, _LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_temp"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_SMSP, _LS(IDS_MAIN_COMBINED_STRENGTH_METHOD_OF_SOLID_PIPE), _T("ico24_mods_design_dropmenu1_combinedratiocalculationmethodforcircularsection"), m_strSvgPath);

			const CString& strStlCode = CDBLib::GetStlCodeName();
			if ( strStlCode.CompareNoCase(STLCODE_SP_16_13330_2017) == 0 )
			{
				InsertTreeItem(ThTi, ID_DGN_STL_SPSC, _LS(IDS_TREE_TEXT_STL_SPSC), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SPCS, _LS(IDS_TREE_TEXT_STL_SPCS), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SPLS, _LS(IDS_TREE_TEXT_STL_SPLS), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SPPB, _LS(IDS_TREE_TEXT_STL_SPPB), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SPLB, _LS(IDS_TREE_TEXT_STL_SPLB), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SP14_SEISMIC_ACCOUNTING, _LS(IDS_TREE_TEXT_STL_SP14SeisMembParams), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SP16SafetyMembParams, _LS(IDS_TREE_TEXT_STL_SP16SafetyMembParams), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_SP16CombinedSectParams, _LS(IDS_TREE_TEXT_STL_SP16CombinedSectParams), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			}

#if defined(_MGEN_CH)
			InsertTreeItem(ThTi, ID_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);//add by maxiao 2015-10-12
			InsertTreeItem(ThTi, ID_DGN_PERFOR_MIN_COEFFICEN, _LS(IDS_TREE_TEXT_PERFORMANCE_COEFFICIENT_MIN), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_PERFOR_MEMBER_COEFFICIENT, _LS(IDS_TREE_TEXT_PERFORMANCE_MEMBER_COEFFICIENT), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_PERFOR_DUCTILITY_LEVEL, _LS(IDS_TREE_TEXT_PERFORMANCE_DUCTILITY_LEVEL), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_PERFOR_MEMBER_PROPERTIES, _LS(IDS_TREE_TEXT_PERFORMANCE_MEMBER_PROPERTIES), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT, _LS(IDS_TREE_TEXT_PERFORMANCE_NON_PLASTIC_FORCE_COEFFICIENT), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_SECTION_TYPE, _LS(IDS_TREE_TEXT_STL_SECT_CLASSIFY), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_BEND_MODULUS_RTO, _LS(IDS_TREE_TEXT_STL_BEND_MODULUS_RTO), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_SPDF, _LS(IDS_TREE_TEXT_STL_CROSSECT_PLASTIC_DEVELOP_FAC), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_SDSW, _LS(IDS_TREE_TEXT_STL_SIDESWAY_CHARACT), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_STL_APPRAISAL_IMEL, _LS(IDS_TREE_TEXT_STL_APPRASIAL_ASSIGN_IMPORTANT_ELEMENT), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
#endif
		}
		/////////////////////////
		// Concrete.
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_CON_PARAM), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_DGN_CON_DCON, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_designcode"), m_strSvgPath);
			strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
			CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
			InsertTreeItem(ThTi, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_design_design_dropmenu2_modifyconcretematerial"), m_strSvgPath);

			if(CDBDoc::GetDocPoint())
			{
				BOOL bUS = FALSE;
				if(CProduct::IsMovingType(D_PRODUCT_MOVING_US) || CProduct::IsMovingType(D_PRODUCT_MOVING_RUS))
					bUS = TRUE;
				const CString& strCode = CDBLib::GetConCodeName();
				int nCode = CDBLib::GetConSubCode();
				if(bUS && strCode==CONCODE_EC2_04 && nCode==dgn::def::enNationalAnnex::Italy)
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
				else
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
				
                if ( bUS && ((strCode == CONCODE_EC2_04 && nCode == dgn::def::enNationalAnnex::Italy) || strCode == CONCODE_IS456_2000 ||
                    strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 || 
                    strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 || 
                    strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
					strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
                    strCode == CONCODE_NSCP_2015 || strCode == CONCODE_NTC_DCEC_2017 ||
					strCode == CONCODE_NTC_DCEC_2023) ) // Seungjun '151126 PMS.5166 ACI318-08, 11 단면제한 옵션 추가
                    InsertTreeItem(ThTi, ID_DGN_CON_DCON_MIN_SECT_LIMIT, _LS(IDS_TREE_TEXT_CON_MIN_SECT_SIZE), _T("ico24_mods_design_dropmenu2_limitingminimumsectionsize"), m_strSvgPath);
			}
			else
			{
				InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
			}

			BOOL bORG = CProduct::IsMovingType(D_PRODUCT_MOVING_ORG);			
			
			InsertTreeItem(ThTi, ID_DGN_CON_DCRB, _LS(IDS_TREE_TEXT_CON_BAR_CRITERIA), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_REBAR_MEMBER, _LS(IDS_TREE_TEXT_CON_BAR_MEMBER), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_BEAM_REBAR_EQUAL, _LS(IDS_TREE_TEXT_CON_EQUAL_BEAM), _T("ico24_design_design_dropmenu2_samebeamrebaratjoints"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_MRFT, _LS(IDS_TREE_TEXT_CON_MOD_MRFT), _T("ico24_design_design_dropmenu2_momentredistributionfactor"), m_strSvgPath);			
			InsertTreeItem(ThTi, ID_DGN_CON_TRFT, _LS(IDS_TREE_TEXT_CON_MOD_TRFT), _T("ico24_design_design_dropmenu2_torsionreductionfactor"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
#ifndef _IGEN
			InsertTreeItem(ThTi, ID_DGN_CON_UCCF, _LS(IDS_TREE_TEXT_CON_MOD_UCCF), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);// .중국신기준 - 불확정성 분항계수.
#endif
			InsertTreeItem(ThTi, ID_DGN_CON_STAN, _LS(IDS_TREE_TEXT_CON_MOD_STAN), _T("ico24_design_design_dropmenu2_modifymemberstrutangle"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_MCMB, _LS(IDS_TREE_TEXT_CON_MOD_MCMB), _T("ico24_design_design_dropmenu2_momentcalculationmethodforbeam"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_DFBA, _LS(IDS_TREE_TEXT_CON_MOD_DFBA), _T("ico24_design_design_dropmenu2_designforceforbeamassignedasmember"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_REXC, _LS(IDS_TREE_TEXT_CON_MOD_REXC), _T("ico24_design_design_dropmenu2_momentcalculationmethodforbeam"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_PMDM, _LS(IDS_MAIN_RIBBON_MENU_PMCM), _T("ico24_design_design_dropmenu2_pmcurvecalculationmethod"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_SCOL, _LS(IDS_MAIN_RIBBON_MENU_SCOL), _T("ico24_design_design_dropmenu2_seismiccolumntype"), m_strSvgPath);

			if(bORG)
			{
				InsertTreeItem(ThTi, ID_DGN_CON_LOSSFACTOR, _LS(IDS_TREE_TEXT_CON_MOD_LOSSFACTOR), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PSCLCOM, _LS(IDS_TREE_TEXT_CON_MOD_PSCLCOM), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PSCMATD, _LS(IDS_TREE_TEXT_CON_MOD_PSCMATL), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_JSFT, _LS(IDS_TREE_TEXT_CON_MOD_JSFT), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PSAS, _LS(IDS_TREE_TEXT_CON_MOD_PSAS), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
			}
			
			InsertTreeItem(ThTi, ID_DGN_CON_REBB, _LS(IDS_TREE_TEXT_CON_MOD_BEAM_REBAR), _T("ico24_design_design_dropmenu2_modifybeamrebardata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_REBC, _LS(IDS_TREE_TEXT_CON_MOD_COLM_REBAR), _T("ico24_design_design_dropmenu2_modifycolumnrebardata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_REBR, _LS(IDS_TREE_TEXT_CON_MOD_BRCE_REBAR), _T("ico24_design_design_dropmenu2_modifycolumnrebardata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_REBW, _LS(IDS_TREE_TEXT_CON_MOD_WALL_REBAR), _T("ico24_design_design_dropmenu2_modifywallrebardata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_WMAK, _LS(IDS_TREE_TEXT_CON_MOD_WALL_MARK), _T("ico24_design_design_dropmenu2_modifywallmarkdata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_BEMW, _LS(IDS_TREE_TEXT_CON_BEM_WALL_ID), _T("ico24_design_design_dropmenu2_modifywallmarkdata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_CON_EBMW, _LS(IDS_TREE_TEXT_CON_EBM_WALL_ID), _T("ico24_mods_design_dropmenu2_rebarinputforgeneralsection"), m_strSvgPath);
			//InsertTreeItem(ThTi, ID_DGN_CON_FPSW, _LS(IDS_TREE_TEXT_CON_FPS_WALL), _T("ico24_temp"), m_strSvgPath);

		}
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_SRC_PARAM), _T("ico24_temp"), m_strSvgPath);
		{
			InsertTreeItem(ThTi, ID_DGN_SRC_DSRC, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu3_designcode"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_SRC_MATD, _LS(IDS_TREE_TEXT_SRC_MOD_MATL), _T("ico24_mods_design_dropmenu3_modifysrcmaterial"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_SRC_RBSB, _LS(IDS_TREE_TEXT_SRC_MOD_BEAM), _T("ico24_mods_design_dropmenu3_modifysrcsectiondata"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_DGN_SRC_REBS, _LS(IDS_TREE_TEXT_SRC_MOD_COLM), _T("ico24_mods_design_dropmenu3_modifysrcsectiondata"), m_strSvgPath);
		}

		BOOL bUseColdFormed = TRUE;
		if(CTestEnvMgr::IsTestEnvST(_T("UseColdFormed"), _T("yes"))) bUseColdFormed = TRUE;

		if(bUseColdFormed)
		{
			/////////////////////////
			// Cold Formed Steel.
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_CFS_PARAM), _T("ico24_temp"), m_strSvgPath);
			{
				InsertTreeItem(ThTi, ID_DGN_CF_DCFS, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_design_designinputdata_designcodeoption"), m_strSvgPath);
				strTitle = _LS(IDS_MAIN_RIBBON_PARTIAL_FACT);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_DCFS_PSFT, strTitle, bShow);
				InsertTreeItem(ThTi, ID_DGN_CF_DCFS_PSFT, strTitle, _T("ico24_design_design_dropmenu5_partialsafetyfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CF_MATD, _LS(IDS_TREE_TEXT_CFS_MOD_MATL), _T("ico24_design_design_dropmenu5_modifycoldformedsteelmaterial"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CF_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CF_SECD, _LS(IDS_MAIN_MODIFY_CFSTEEL_SECT_DATA), _T("ico24_design_design_dropmenu5_sectiondata"), m_strSvgPath);
				strTitle = _LS(IDS_MAIN_RIBBON_BENDING_COEFF_CB);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_CBFT, strTitle, bShow);
				InsertTreeItem(ThTi, ID_DGN_CF_CBFT, strTitle, _T("ico24_design_design_dropmenu5_bendingcoefficient"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CF_PROP, _LS(IDS_MAIN_MODIFY_CFSTEEL_EFFECTIVE), _T("ico24_design_design_dropmenu5_modifyeffectivesectionproperties"), m_strSvgPath);
#if defined(_MGEN_CH)
				InsertTreeItem(ThTi, ID_DGN_CF_SPHI, _LS(IDS_MAIN_RIBBON_MENU_TABLE_CF_PHI), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_BEND_MODULUS_RTO, _LS(IDS_TREE_TEXT_STL_BEND_MODULUS_RTO), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_PERFOR_MIN_COEFFICEN_CFS, _LS(IDS_TREE_TEXT_PERFORMANCE_COEFFICIENT_MIN), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_PERFOR_MEMBER_COEFFICIENT_CFS, _LS(IDS_TREE_TEXT_PERFORMANCE_MEMBER_COEFFICIENT), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_PERFOR_DUCTILITY_LEVEL_CFS, _LS(IDS_TREE_TEXT_PERFORMANCE_DUCTILITY_LEVEL), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_PERFOR_MEMBER_PROPERTIES_CFS, _LS(IDS_TREE_TEXT_PERFORMANCE_MEMBER_PROPERTIES), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT_CFS, _LS(IDS_TREE_TEXT_PERFORMANCE_NON_PLASTIC_FORCE_COEFFICIENT), _T("ico24_design_design_dropmenu5_serviceabilityparameters"), m_strSvgPath);
#endif
			}
		}
		if(IAluDgnTool::IsUseAluDgn())
		{
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_MAIN_RIBBON_MENU_ALU_DGN), _T("ico24_temp"), m_strSvgPath);
			{
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_CODE       );
				InsertTreeItem(ThTi, ID_DGN_ALU_CODE, strTitle, _T("ico24_temp"), m_strSvgPath);
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_MATD       );
				InsertTreeItem(ThTi, ID_DGN_ALU_MATD, strTitle, _T("ico24_temp"), m_strSvgPath);
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_FORCERIB   );
				InsertTreeItem(ThTi, ID_DGN_ALU_FORCERIB, strTitle, _T("ico24_temp"), m_strSvgPath);
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_HEATRANGE  );
				InsertTreeItem(ThTi, ID_DGN_ALU_HEATRANGE, strTitle, _T("ico24_temp"), m_strSvgPath);
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_WELD_RATIO );
				InsertTreeItem(ThTi, ID_DGN_ALU_WELD_RATIO, strTitle, _T("ico24_temp"), m_strSvgPath);
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_DGN_ALU_BEND_RATIO );     
				InsertTreeItem(ThTi, ID_DGN_ALU_BEND_RATIO, strTitle, _T("ico24_temp"), m_strSvgPath);
			}
		}
#endif
	}

		/////////////////////////
		// Pushover
#if defined(_IGEN)
	  ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PUSHOVER_ANALYSIS_DATA), _T("ico24_temp"), m_strSvgPath);  
#else
	  ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PUSHOVER_ANALYSIS_DATA), _T("ico24_temp"), m_strSvgPath);  
#endif
	  {
#if defined(_IGEN)
			InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_TEXT_POGD), _T("ico24_pushover_control_globalcontrol"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_IEPI, _LS(IDS_DB_DT_IEPI), _T("ico24_pushover_control_ignoreelementsfornlanalysisinitialload"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_TEXT_POLD), _T("ico24_pushover_loadcase_loadcase"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_FUNCTION, _LS(IDS_TREE_TEXT_POFC), _T("ico24_pushover_loadcase_pushoverincrementfunctions"), m_strSvgPath);      
		  InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_TEXT_PHGT), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
		  InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_TEXT_PHGE), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
#else
			InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_TEXT_POGD), _T("ico24_pushover_control_globalcontrol"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_BNDR_IEPI, _LS(IDS_DB_DT_IEPI), _T("ico24_pushover_control_ignoreelementsfornlanalysisinitialload"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_TEXT_POLD), _T("ico24_pushover_loadcase_loadcase"), m_strSvgPath);
			InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_FUNCTION, _LS(IDS_TREE_TEXT_POFC), _T("ico24_pushover_loadcase_pushoverincrementfunctions"), m_strSvgPath);      
		  InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_TEXT_PHGT), _T("ico24_pushover_properties_definepushoverhingeproperties"), m_strSvgPath);
		  InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_TEXT_PHGE), _T("ico24_pushover_properties_assignpushoverhingeproperties"), m_strSvgPath);
#endif

	#if defined(_WGEN)
		  InsertTreeItem(ThTi, ID_PUSHOVER_STSHVSDRIFT, _LS(IDS_TREE_TEXT_PSSG), _T("ico24_pushover_pushoverresults_storysheargraph"), m_strSvgPath);
		  InsertTreeItem(ThTi, ID_PUSHOVER_MMSHVSDISP, _LS(IDS_TREE_TEXT_PMSG), _T("ico24_pushover_pushoverresults_pushovergraph"), m_strSvgPath);
	#endif 
	  }
	#if defined(_MGEN) && defined(_CH)
	  InsertTreeItem(hTI[nRootIndex], ID_DESIGN_CALREPORTS, _LS(IDS_TREE_TEXT_DESIGN_CALREPORTS), _T("ico24_temp"), m_strSvgPath);
	#endif 

#else   // CIVIL이면.. 

	#if defined(_ORG)//한국 버전일경우

		nRootIndex++;
	  hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
	  {
			/*
			/////////////////////////////////////////////
		  // Steel Design
		  // sshan .('07.11.30) 
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_DGN_STL_KR_DSTL, _LS(IDS_TREE_TEXT_STL_DESIGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_MATD, _LS(IDS_TREE_TEXT_STL_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifysteelmaterial"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_STEEL_KR_GEN_SECT, _LS(IDS_TREE_TEXT_DGN_STEEL_GEN_SECT), _T("ico24_rating_bridgeratingdesign_dropmenu1_steelgeneralsectionfordesign"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_STEEL_KR_PARAM, _LS(IDS_TREE_TEXT_STL_PARAM), _T("ico24_mods_design_dropmenu1_steeldesignparameter"), m_strSvgPath);			
		  }
			/////////////////////////////////////////////
		  // RC Design
		  // sshan .('06.03.02) #if defined(_ORG) 일 경우 RC Desing을 Add 한다.    
	    ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_RC_DESIGN), _T("ico24_temp"), m_strSvgPath);
		  {
				CString strTitle = _T("");
				BOOL bShow = FALSE;

				InsertTreeItem(ThTi, ID_DGN_RC_CODE, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_mesheddesign"), m_strSvgPath);
				strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
				CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
				InsertTreeItem(ThTi, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_modifyconcretematerial"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_KCI_USD03, _LS(IDS_TREE_TEXT_RC_DESIGN_PARAM), _T("ico24_mods_design_dropmenu2_rcdesignparameter"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_RC_PLATE_PARAM, _LS(IDS_TREE_TEXT_RC_DESIGN_PARAM_PLATE), _T("ico24_mods_design_dropmenu2_rcdesignparameter(plate)"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_CON_REBB2, _LS(IDS_TREE_TEXT_RC_DESIGN_BEAM_BAR_SECT), _T("ico24_mods_design_dropmenu2_beamsectiondataforchecking"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_CON_REBC2, _LS(IDS_TREE_TEXT_RC_DESIGN_COLUMN_BAR_SECT), _T("ico24_seismicperform_capacity_rebarinputforcolumnsection"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_SECTION_REBAR_INPUT, _LS(IDS_TREE_TEXT_RC_DESIGN_GEN_BAR_SECT), _T("ico24_seismicperform_capacity_rebarinputforgeneralsection"), m_strSvgPath);      
		  }
			*/
		

			/////////////////////////
			// PSC Design 
			ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_PSC_DGN, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);
 	    CStringArray aPscCodeName;
	    CDBLib::GetPscCodeNameList(aPscCodeName);
			if(aPscCodeName.GetSize()>0)      
			{  
		    SetPscDgnTableItemTree();
			}  
			
			/////////////////////////////////////////////
		  // Bridge Load Rating Design
		  // sshan .('06.03.02) #if defined(_US) 일 경우 RC Desing을 Add 한다.				
			// sshan .('08.03.06) #if defined(_US)(_ORG) 일 경우 RC Desing을 Add 한다.				
		  //ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_RATING_DGN, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_temp"), m_strSvgPath);  
			//SetRatingDgnTableItemTree();
			
	  

			//nRootIndex++;
			//International Design
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_DGN_INTERNATIONAL), _T("ico24_temp"), m_strSvgPath);
		  {	    
				CString strTitle = _T("");
		    BOOL bShow = FALSE;
				/////////////////////////
		    // General.
		    HTREEITEM ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_DGN_COMMON_PARAM), _T("ico24_temp"), m_strSvgPath);
		    {
					InsertTreeItem(ThTiDgn, ID_DGN_GEN_DFRM, _LS(IDS_TREE_TEXT_GEN_DEF_FRAME), _T("ico24_rating_bridgeratingdesign_definitionofframe"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_GEN_LCTB, _LS(IDS_TM_LOAD_CONTRIBUTION), _T("ico24_mods_design_loadcontributionfornonlinearloadcase"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
					//(2010.01.12) Modify by Unsang :: Haunched Beam Assignment... Menu는 Gen에서만 사용함.          
					//InsertTreeItem(ThTiDgn, ID_DGN_GEN_HCBM, _LS(IDS_TREE_TEXT_GEN_HAUNCH), _T("ico24_design_general_haunchedbeamassignment"), m_strSvgPath);
			    
					strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
			    CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
			    InsertTreeItem(ThTiDgn, ID_DGN_GEN_LENG, strTitle, _T("ico24_rating_bridgeratingdesign_unbracedlength(l,lb)"), m_strSvgPath);
			    strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
			    CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
			    InsertTreeItem(ThTiDgn, ID_DGN_GEN_KFAC, strTitle, _T("ico24_rating_bridgeratingdesign_effectivelengthfactor(k)"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_rating_bridgeratingdesign_limitingslendernessratio"), m_strSvgPath);
			    strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
			    CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
			    InsertTreeItem(ThTiDgn, ID_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_momentfactor(cm)"), m_strSvgPath);
			    InsertTreeItem(ThTiDgn, ID_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_momentmagnifier(b1delta-b,b2delta-s)"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);		  
		    }
				/////////////////////////
		    // Steel.
		    ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
		    {
					InsertTreeItem(ThTiDgn, ID_DGN_STL_DSTL, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_STL_ST_REDUCT_FACTOR_LSD);
			    CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DSTL_SRED, strTitle, bShow);
					InsertTreeItem(ThTiDgn, ID_DGN_STL_DSTL_SRED, strTitle, _T("ico24_mods_design_dropmenu1_strengthreductionfactor"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_STL_MATD, _LS(IDS_TREE_TEXT_STL_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifysteelmaterial"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_STL_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_dropmenu1_serviceabilityparameters"), m_strSvgPath);
					// -------------------------------------------------------------------
			    strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
			    CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT, strTitle, bShow);
			    InsertTreeItem(ThTiDgn, ID_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
			    InsertTreeItem(ThTiDgn, ID_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)"), m_strSvgPath);
			    InsertTreeItem(ThTiDgn, ID_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress"), m_strSvgPath);
					//InsertTreeItem(ThTiDgn, ID_DESIGN_STEEL_UCFA, _LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_STL_TRAN_STFN, _LS(IDS_TREE_TEXT_STL_TRANSVERSE_STFN), _T("ico24_mods_design_dropmenu1_transversestiffenerofsection"), m_strSvgPath);
			    InsertTreeItem(ThTiDgn, ID_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);  //add by maxiao 2015-10-12
// 			    InsertTreeItem(ThTiDgn, ID_DGN_STL_SDSW, _LS(IDS_TREE_TEXT_STL_SIDESWAY_CHARACT), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);  //add by maxiao 2015-10-22
		    }
				/////////////////////////
		    // Concrete.	    
		    ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_CON_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
		    {
					InsertTreeItem(ThTiDgn, ID_DGN_CON_DCON, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_designcode"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
					CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
					InsertTreeItem(ThTiDgn, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_modifyconcretematerial"), m_strSvgPath);
//        InsertTreeItem(ThTiDgn, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);

					CString strCode = CDBLib::GetConCodeName();
					int nCode = CDBLib::GetConSubCode();
					if(strCode=="Eurocode2-2:05" && nCode==0)
						InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
					else
						InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
					
					if(strCode=="Eurocode2-2:05" && nCode==0)
						InsertTreeItem(ThTi, ID_DGN_CON_DCON_MIN_SECT_LIMIT, _LS(IDS_TREE_TEXT_CON_MIN_SECT_SIZE), _T("ico24_mods_design_dropmenu2_limitingminimumsectionsize"), m_strSvgPath);
					
					if(strCode=="Eurocode2-2:05" && nCode==0)
						InsertTreeItem(ThTi, ID_DESIGN_CONC_CVL_DGN_CRIT_FOR_RBAR, _LS(IDS_TREE_TEXT_CON_DGN_CRIT_FOR_RBAR), _T("ico24_mods_design_dropmenu2_designcriteriaforrebars"), m_strSvgPath);
					
					//
					//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
					/*
					InsertTreeItem(ThTiDgn, ID_DGN_CON_CVL_DCRM, _LS(IDS_TREE_TEXT_CON_MATL_LIMIT), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_CON_CVL_DCRR, _LS(IDS_TREE_TEXT_CON_RBAR_LIMIT), _T("ico24_temp"), m_strSvgPath);
					*/          
					InsertTreeItem(ThTiDgn, ID_DESIGN_CONC_CVL_CTC_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR), _T("ico24_mods_design_dropmenu2_scaleupfactorforcolumn"), m_strSvgPath);
					//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
					/*InsertTreeItem(ThTiDgn, ID_DESIGN_CONC_CVL_ULS_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR), _T("ico24_temp"), m_strSvgPath);*/
					InsertTreeItem(ThTiDgn, ID_DGN_CON_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);

					InsertTreeItem(ThTiDgn, ID_DESIGN_CONC_BEAMDATA, _LS(IDS_TREE_TEXT_CON_DGN_BEAM), _T("ico24_mods_design_dropmenu2_beamsectiondatafordesign"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DESIGN_CONC_COLMDATA, _LS(IDS_TREE_TEXT_CON_DGN_COLM), _T("ico24_mods_design_dropmenu2_columnsectiondatafordesign"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_CON_REBB2, _LS(IDS_TREE_TEXT_CON_CHK_BEAM), _T("ico24_mods_design_dropmenu2_beamsectiondataforchecking"), m_strSvgPath);
			    InsertTreeItem(ThTiDgn, ID_DGN_CON_REBC2, _LS(IDS_TREE_TEXT_CON_CHK_COLM), _T("ico24_seismicperform_capacity_rebarinputforcolumnsection"), m_strSvgPath);
					//InsertTreeItem(ThTiDgn, ID_DGN_CON_REBR, _LS(IDS_TREE_TEXT_CON_MOD_BRCE_SECT), _T("ico24_design_design_dropmenu2_modifycolumnrebardata"), m_strSvgPath);
					//(2010.02.02) Add by Unsang :: Add General Section's Rebar DLG. Only Eurocode2-2:05
					// Modify, Jaeoh. [5/20/2010] 785 출시관련 막음.
					//strTitle = _LS(IDS_TREE_TEXT_CON_CHK_COLM_GEN);
					//CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_SECTION_REBAR_INPUT2, strTitle, bShow, FALSE);
					//InsertTreeItem(ThTiDgn, ID_SECTION_REBAR_INPUT2, strTitle, _T("ico24_mods_design_dropmenu2_rebarinputforgeneralsection"), m_strSvgPath);
		    }

				/////////////////////////
				// SRC     
		    ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_SRC_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
		    {
					InsertTreeItem(ThTiDgn, ID_DGN_SRC_DSRC, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_design_designinputdata_designcodeoption"), m_strSvgPath);
					InsertTreeItem(ThTiDgn, ID_DGN_SRC_MATD, _LS(IDS_TREE_TEXT_SRC_MOD_MATL), _T("ico24_mods_design_dropmenu3_modifysrcmaterial"), m_strSvgPath);
			    InsertTreeItem(ThTiDgn, ID_DGN_SRC_REBS, _LS(IDS_TREE_TEXT_SRC_MOD_COLM), _T("ico24_mods_design_dropmenu3_modifysrcsectiondata"), m_strSvgPath);
		    }
			}

			/////////////////////////
			// Pushover
	    ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PUSHOVER_ANALYSIS_DATA), _T("ico24_temp"), m_strSvgPath);  
	    {
				InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_TEXT_POGD), _T("ico24_pushover_control_globalcontrol"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_TEXT_POLD), _T("ico24_pushover_loadcase_loadcase"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_FUNCTION, _LS(IDS_TREE_TEXT_POFC), _T("ico24_pushover_loadcase_pushoverincrementfunctions"), m_strSvgPath);      
		    InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_TEXT_PHGT), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
		    InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_TEXT_PHGE), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
	    }
		}
	  
	#else//한국 이외의 국가
	  nRootIndex++;
	  hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
	  {

			#if defined _CH	//CH 일경우
				/////////////////////////////////////////////
				// RC Design(CH)
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_RC_DESIGN), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_RC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_RC_CH_PARAM), _T("ico24_temp"), m_strSvgPath);      
					InsertTreeItem(ThTi, ID_RC_DESIGN_MATL, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_REBAR), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_OPTION), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RC_PRINT_OPTION, _LS(IDS_TREE_TEXT_RC_CH_PRINT_OPTION), _T("ico24_temp"), m_strSvgPath);					
					InsertTreeItem(ThTi, ID_RC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_RC_CH_CRACK_WIDTH), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RC_DESIGN_SEISMIC, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_SEISMIC), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_RC_DESIGN_DGRM, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_RESULT_DIAGRAM), _T("ico24_temp"), m_strSvgPath);			
				}

				/////////////////////////
				// PSC Design 
				// Change by ZINU.('04.12.28). 임시수정 by 신미영DR, 개발항목 1103.
				// Change by Jaeoh.('05.07.19) Code Base로 수정.
				// Change by Jaeoh.('05.07.27) Lock 기준으로 수정.
				ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_PSC_DGN, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);
 				CStringArray aPscCodeName;
				CDBLib::GetPscCodeNameList(aPscCodeName);
				if(aPscCodeName.GetSize()>0)      
				{  
					SetPscDgnTableItemTree();
				}  

				CString strTitle = _T("");
				BOOL bShow = FALSE;
				/////////////////////////
				// General.
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_GEN_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_GEN_DFRM, _LS(IDS_TREE_TEXT_GEN_DEF_FRAME), _T("ico24_rating_bridgeratingdesign_definitionofframe"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_LCTB, _LS(IDS_TM_LOAD_CONTRIBUTION), _T("ico24_mods_design_loadcontributionfornonlinearloadcase"), m_strSvgPath);
					// -------------------------------------------------------------------
					strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_LENG, strTitle, _T("ico24_rating_bridgeratingdesign_unbracedlength(l,lb)"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_KFAC, strTitle, _T("ico24_rating_bridgeratingdesign_effectivelengthfactor(k)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_rating_bridgeratingdesign_limitingslendernessratio"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_momentfactor(cm)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_momentmagnifier(b1delta-b,b2delta-s)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);		  
				}
				/////////////////////////
				// Steel.
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_STL_DSTL, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_STL_ST_REDUCT_FACTOR_LSD);
					CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DSTL_SRED, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_STL_DSTL_SRED, strTitle, _T("ico24_mods_design_dropmenu1_strengthreductionfactor"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_MATD, _LS(IDS_TREE_TEXT_STL_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifysteelmaterial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_dropmenu1_serviceabilityparameters"), m_strSvgPath);
					// -------------------------------------------------------------------
					strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
					CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress"), m_strSvgPath);
					//InsertTreeItem(ThTi, ID_DESIGN_STEEL_UCFA, _LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_TRAN_STFN, _LS(IDS_TREE_TEXT_STL_TRANSVERSE_STFN), _T("ico24_mods_design_dropmenu1_transversestiffenerofsection"), m_strSvgPath);
			        InsertTreeItem(ThTi, ID_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);  //add by maxiao 2015-10-12
// 			        InsertTreeItem(ThTi, ID_DGN_STL_SDSW, _LS(IDS_TREE_TEXT_STL_SIDESWAY_CHARACT), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);  //add by maxiao 2015-10-22
				}
				/////////////////////////
				// SRC 
				// Change by sshan .('06.03.02) #if defined(_ORG) 일 경우 Add 하지 않는다				
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_SRC_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_SRC_DSRC, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_design_designinputdata_designcodeoption"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_SRC_MATD, _LS(IDS_TREE_TEXT_SRC_MOD_MATL), _T("ico24_mods_design_dropmenu3_modifysrcmaterial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_SRC_REBS, _LS(IDS_TREE_TEXT_SRC_MOD_COLM), _T("ico24_mods_design_dropmenu3_modifysrcsectiondata"), m_strSvgPath);
				}

				/////////////////////////
				// Pushover
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PUSHOVER_ANALYSIS_DATA), _T("ico24_temp"), m_strSvgPath);  
				{
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_TEXT_POGD), _T("ico24_pushover_control_globalcontrol"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_TEXT_POLD), _T("ico24_pushover_loadcase_loadcase"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_FUNCTION, _LS(IDS_TREE_TEXT_POFC), _T("ico24_pushover_loadcase_pushoverincrementfunctions"), m_strSvgPath);      
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_TEXT_PHGT), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_TEXT_PHGE), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
				}				
				

			#else//CH 아닐경우.

				CString strTitle = _T("");
				BOOL bShow = FALSE;
				/////////////////////////
				// General.
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_GEN_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_GEN_DFRM, _LS(IDS_TREE_TEXT_GEN_DEF_FRAME), _T("ico24_rating_bridgeratingdesign_definitionofframe"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_LCTB, _LS(IDS_TM_LOAD_CONTRIBUTION), _T("ico24_mods_design_loadcontributionfornonlinearloadcase"), m_strSvgPath);
					// -------------------------------------------------------------------
					strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_LENG, strTitle, _T("ico24_rating_bridgeratingdesign_unbracedlength(l,lb)"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_KFAC, strTitle, _T("ico24_rating_bridgeratingdesign_effectivelengthfactor(k)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_rating_bridgeratingdesign_limitingslendernessratio"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_momentfactor(cm)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_momentmagnifier(b1delta-b,b2delta-s)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);		  
				}
				/////////////////////////
				// Steel.
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_STL_DSTL, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_STL_ST_REDUCT_FACTOR_LSD);
					CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DSTL_SRED, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_STL_DSTL_SRED, strTitle, _T("ico24_mods_design_dropmenu1_strengthreductionfactor"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_MATD, _LS(IDS_TREE_TEXT_STL_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifysteelmaterial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_dropmenu1_serviceabilityparameters"), m_strSvgPath);
					// -------------------------------------------------------------------
					strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
					CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress"), m_strSvgPath);
					//InsertTreeItem(ThTi, ID_DESIGN_STEEL_UCFA, _LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_STL_TRAN_STFN, _LS(IDS_TREE_TEXT_STL_TRANSVERSE_STFN), _T("ico24_mods_design_dropmenu1_transversestiffenerofsection"), m_strSvgPath);
			        InsertTreeItem(ThTi, ID_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);  //add by maxiao 2015-10-12
// 			        InsertTreeItem(ThTi, ID_DGN_STL_SDSW, _LS(IDS_TREE_TEXT_STL_SIDESWAY_CHARACT), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)"), m_strSvgPath);  //add by maxiao 2015-10-12
				}
				/////////////////////////
				// Concrete.
				// Change by sshan .('06.03.02) #if defined(_ORG) defined(_CH) 일 경우 Add 하지 않는다
				#if defined(_US) || defined(_RUS) || defined(_JP)
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_CON_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_CON_DCON, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_designcode"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
					CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_modifyconcretematerial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
					
					//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
					/*
					InsertTreeItem(ThTi, ID_DGN_CON_CVL_DCRM, _LS(IDS_TREE_TEXT_CON_MATL_LIMIT), _T("ico24_temp"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_CVL_DCRR, _LS(IDS_TREE_TEXT_CON_RBAR_LIMIT), _T("ico24_temp"), m_strSvgPath);
					*/

					// Change by Jaeoh ('05.07.15) #if define(_ORG) || define(_CH) 삭제
					InsertTreeItem(ThTi, ID_DESIGN_CONC_CVL_CTC_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR), _T("ico24_mods_design_dropmenu2_scaleupfactorforcolumn"), m_strSvgPath);
					//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
					/*InsertTreeItem(ThTi, ID_DESIGN_CONC_CVL_ULS_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR), _T("ico24_temp"), m_strSvgPath);*/
					InsertTreeItem(ThTi, ID_DGN_CON_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);

					InsertTreeItem(ThTi, ID_DESIGN_CONC_BEAMDATA, _LS(IDS_TREE_TEXT_CON_DGN_BEAM), _T("ico24_mods_design_dropmenu2_beamsectiondatafordesign"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DESIGN_CONC_COLMDATA, _LS(IDS_TREE_TEXT_CON_DGN_COLM), _T("ico24_mods_design_dropmenu2_columnsectiondatafordesign"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_REBB2, _LS(IDS_TREE_TEXT_CON_CHK_BEAM), _T("ico24_mods_design_dropmenu2_beamsectiondataforchecking"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_REBC2, _LS(IDS_TREE_TEXT_CON_CHK_COLM), _T("ico24_seismicperform_capacity_rebarinputforcolumnsection"), m_strSvgPath);
					//InsertTreeItem(ThTi, ID_DGN_CON_REBR, _LS(IDS_TREE_TEXT_CON_MOD_BRCE_SECT), _T("ico24_design_design_dropmenu2_modifycolumnrebardata"), m_strSvgPath);
					//(2010.02.02) Add by Unsang :: Add General Section's Rebar DLG. Only Eurocode2-2:05
					// Modify, Jaeoh. [5/20/2010] 785 출시관련 막음.
					//strTitle = _LS(IDS_TREE_TEXT_CON_CHK_COLM_GEN);
					//CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_SECTION_REBAR_INPUT2, strTitle, bShow, FALSE);
					//InsertTreeItem(ThTi, ID_SECTION_REBAR_INPUT2, strTitle, _T("ico24_mods_design_dropmenu2_rebarinputforgeneralsection"), m_strSvgPath);
				}
				#endif

				/////////////////////////////////////////////
				// RC Design
				// sshan .('06.03.02) #if defined(_ORG) 일 경우 RC Desing을 Add 한다.
				#if defined _ORG
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_RC_DESIGN), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_CON_DCON, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_designcode"), m_strSvgPath);
					strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
					CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_modifyconcretematerial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_KCI_USD03, _LS(IDS_TREE_TEXT_RC_DESIGN_PARAM), _T("ico24_mods_design_dropmenu2_rcdesignparameter"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_RC_PLATE_PARAM, _LS(IDS_TREE_TEXT_RC_DESIGN_PARAM_PLATE), _T("ico24_mods_design_dropmenu2_rcdesignparameter(plate)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_REBB2, _LS(IDS_TREE_TEXT_RC_DESIGN_BEAM_BAR_SECT), _T("ico24_mods_design_dropmenu2_beamsectiondataforchecking"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_CON_REBC2, _LS(IDS_TREE_TEXT_RC_DESIGN_COLUMN_BAR_SECT), _T("ico24_seismicperform_capacity_rebarinputforcolumnsection"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_SECTION_REBAR_INPUT, _LS(IDS_TREE_TEXT_RC_DESIGN_GEN_BAR_SECT), _T("ico24_seismicperform_capacity_rebarinputforgeneralsection"), m_strSvgPath);      
				}
				#endif
				
				/////////////////////////
				// SRC 
				// Change by sshan .('06.03.02) #if defined(_ORG) 일 경우 Add 하지 않는다
				#ifndef _ORG
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_SRC_PARAM), _T("ico24_temp"), m_strSvgPath);
				{
					InsertTreeItem(ThTi, ID_DGN_SRC_DSRC, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_design_designinputdata_designcodeoption"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_SRC_MATD, _LS(IDS_TREE_TEXT_SRC_MOD_MATL), _T("ico24_mods_design_dropmenu3_modifysrcmaterial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_DGN_SRC_REBS, _LS(IDS_TREE_TEXT_SRC_MOD_COLM), _T("ico24_mods_design_dropmenu3_modifysrcsectiondata"), m_strSvgPath);
				}
				#endif

				#if !defined(_JP)
				/////////////////////////
				// Pushover
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PUSHOVER_ANALYSIS_DATA), _T("ico24_temp"), m_strSvgPath);  
				{
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_CONTROL, _LS(IDS_TREE_TEXT_POGD), _T("ico24_pushover_control_globalcontrol"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_LOADCASE, _LS(IDS_TREE_TEXT_POLD), _T("ico24_pushover_loadcase_loadcase"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_FUNCTION, _LS(IDS_TREE_TEXT_POFC), _T("ico24_pushover_loadcase_pushoverincrementfunctions"), m_strSvgPath);      
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_DEFINE_HINGE, _LS(IDS_TREE_TEXT_PHGT), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_STRUCT_PUSHOVER_ASSIGN, _LS(IDS_TREE_TEXT_PHGE), _T("ico24_seismicperform_misc_bridgeinformation"), m_strSvgPath);
				}
				#endif
				/////////////////////////
				// PSC Design 
				#if defined _JP || defined _US
				// Change by ZINU.('04.12.28). 임시수정 by 신미영DR, 개발항목 1103.
				// Change by Jaeoh.('05.07.19) Code Base로 수정.
				// Change by Jaeoh.('05.07.27) Lock 기준으로 수정.
				ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_PSC_DGN, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);
 				CStringArray aPscCodeName;
				CDBLib::GetPscCodeNameList(aPscCodeName);
				if(aPscCodeName.GetSize()>0)      
				{  
					SetPscDgnTableItemTree();
				}  
				#endif

				/////////////////////////////////////////////
				// Bridge Load Rating Design
				// sshan .('06.03.02) #if defined(_US) 일 경우 RC Desing을 Add 한다.				
				// sshan .('08.03.06) #if defined(_US)(_ORG) 일 경우 RC Desing을 Add 한다.				
				#if defined(_US) || defined(_RUS) || defined(_ORG)
				ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_RATING_DGN, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_temp"), m_strSvgPath);  
				SetRatingDgnTableItemTree();
				#endif

				/////////////////////////////////////////////
				// Composite Plate Girder Design
				// sshan .('06.12.27) #if defined(_US) 일 경우 CPG을 Add 한다.				
				#if defined(_US) || defined(_RUS)// || _ORG
				ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_DGN_CPG), _T("ico24_temp"), m_strSvgPath);  
				{
					InsertTreeItem(ThTi, ID_CPG_DESIGN_PARAM, _LS(IDS_TREE_TEXT_DGN_CPG_PARAM), _T("ico24_mods_design_dropmenu4_defineflange&web"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_MATERIAL, _LS(IDS_TREE_TEXT_DGN_CPG_MATERIAL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifycompositematerial"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_POSI, _LS(IDS_TREE_TEXT_DGN_CPG_POSI_DGN_OUT), _T("ico24_mods_design_dropmenu4_designposition"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_POSI_DGN_OUT, _LS(IDS_TREE_TEXT_DGN_CPG_POSI), _T("ico24_mods_design_dropmenu4_positionfordesignoutput"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_SHEAR_CONNECT, _LS(IDS_TREE_TEXT_DGN_CPG_SHEAR_CONNECT), _T("ico24_mods_design_dropmenu4_shearconnector"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_LONGIT_REINFORCE, _LS(IDS_TREE_TEXT_DGN_CPG_LONGIT_REINFORCE), _T("ico24_rating_bridgeratingdesign_dropmenu1_longitudinalreinforcement"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_FLANGE_WEB, _LS(IDS_TREE_TEXT_DGN_CPG_FALNGE_WEB), _T("ico24_mods_design_dropmenu4_defineflange&web"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_LONGIT_REINFORCE_STIF, _LS(IDS_TREE_TEXT_DGN_CPG_LONGIT_REINFORCE_STIF), _T("ico24_mods_design_dropmenu4_longitudinalstiffener(plategirderonly)"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_LONGIT_REINFORCE_STIF_SM, _LS(IDS_TREE_TEXT_DGN_CPG_LONGIT_REINFORCE_STIF_SM), _T("ico24_mods_design_dropmenu4_longitudinalstiffener"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_TRAN_STIF, _LS(IDS_TREE_TEXT_DGN_CPG_TRAN_STIF), _T("ico24_mods_design_dropmenu4_transversestiffener"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_END_SUPPORT, _LS(IDS_TREE_TEXT_DGN_CPG_END_SUPPORT), _T("ico24_mods_design_dropmenu4_transversestiffenerofendsupport"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_TYPE_LOAD, _LS(IDS_TREE_TEXT_DGN_CPG_LOAD_APPL), _T("ico24_mods_design_dropmenu4_typesofloadapplication"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_TORSION, _LS(IDS_TREE_TEXT_DGN_CPG_LOAD_LTBD), _T("ico24_mods_design_dropmenu4_lateraltorsionalbucklingdata"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_DAMAGE, _LS(IDS_TREE_TEXT_DGN_CPG_LOAD_DEFC), _T("ico24_mods_design_dropmenu4_fatiguedetail"), m_strSvgPath);
					InsertTreeItem(ThTi, ID_CPG_DESIGN_SERVICE_LCOM_TYPE, _LS(IDS_TREE_TEXT_DGN_CPG_SERV_LCOM_TYPE), _T("ico24_mods_design_dropmenu4_serviceabilityloadcombinationtype"), m_strSvgPath);
				}
				#endif

			#endif//CH 아니면 끝
		}

	#endif//한국이 아니면 끝

#endif

//------------------------------------------------------------------------------------------------
// Query
//------------------------------------------------------------------------------------------------  
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TM_Query), _T("ico24_temp"), m_strSvgPath);
	{
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_PROJECT, _LS(IDS_TM_Project_Status), _T("ico24_query_status_projectstatus"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_PICKNODE, _LS(IDS_TM_Query_Nodes), _T("ico24_query_query_querynodes"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_PICKELEM, _LS(IDS_TM_Query_Elements), _T("ico24_query_query_queryelements"), m_strSvgPath);

/*
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_NODE, _LS(IDS_TM_Node_Detail_Table), _T("ico24_query_detailtable_nodedetailtable"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_ELEMENT, _LS(IDS_TM_Element_Detail_Table), _T("ico24_query_detailtable_elementdetailtable"), m_strSvgPath);
#ifdef _MGEN
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_DESIGN, _LS(IDS_TM_Design_Parameter_Detail_Table), _T("ico24_query_detailtable_designparameterdetailtable"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STOR_WEIGHT, _LS(IDS_TM_Story_Weight_Table), _T("ico24_query_weightmassloadtable_storyweighttable"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STOR_LOAD, _LS(IDS_TM_Story_Load_Table), _T("ico24_query_weightmassloadtable_storyloadtable"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STOR_MASS, _LS(IDS_TM_Story_Mass_Table), _T("ico24_query_weightmassloadtable_storymasstable"), m_strSvgPath);
#endif
#ifndef _TOWER
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_NODAL_MASS, _LS(IDS_TM_Mass_Summary_Table), _T("ico24_query_massloadtable_masssummarytable"), m_strSvgPath);
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_TOTAL_LOAD, _LS(IDS_TM_Load_Summary_Table), _T("ico24_query_massloadtable_loadsummarytable"), m_strSvgPath);
		////Add 김제헌20020621
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_CS_GROUP, _LS(IDS_TM_Group_Activation_Summary_Table), _T("ico24_GroupActivationofCS"), m_strSvgPath);
#endif
		*/
	}	


	// Add, Jaeoh. (08.11.03)
#if defined(_CIVIL)
	#if defined(_ORG)
		nRootIndex++;
		hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_MODS_MODULE), _T("ico24_temp"), m_strSvgPath);
		{
			//InsertTreeItem(hTI[nRootIndex], ID_QUERY_PROJECT, _LS(IDS_TM_Project_Status), _T("ico24_query_status_projectstatus"), m_strSvgPath);
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_Mesh), _T("ico24_temp"), m_strSvgPath);	
			{
				InsertTreeItem(ThTi, ID_STRUCT_AUTOMESH, _LS(IDS_TM_Automesh), _T("ico24_mods_mesh_automesh"), m_strSvgPath);
				//InsertTreeItem(ThTi, ID_STRUCT_MAPMESH_KE, _LS(IDS_TM_Mapmesh_KE), _T("ico24_temp"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_STRUCT_MAPMESH, _LS(IDS_TM_Mapmesh), _T("ico24_mods_mesh_mapmesh"), m_strSvgPath);
			}

			//(2009.12.14) Add by Unsang :: Add Commom Parameters in MODS-Treemeun.
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_DGN_COMMON_PARAM), _T("ico24_temp"), m_strSvgPath);	
			{
				CString strTitle = _T("");
		    BOOL bShow = FALSE;
				// Member Assignment
				InsertTreeItem(ThTi, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
				// Reverse Member Local Direction
				InsertTreeItem(ThTi, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
				// Unbraced Length
				strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
			  CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
			  InsertTreeItem(ThTi, ID_DGN_GEN_LENG, strTitle, _T("ico24_rating_bridgeratingdesign_unbracedlength(l,lb)"), m_strSvgPath);
				// Effective Length Factor
				strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
			  CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
			  InsertTreeItem(ThTi, ID_DGN_GEN_KFAC, strTitle, _T("ico24_rating_bridgeratingdesign_effectivelengthfactor(k)"), m_strSvgPath);
				// Limiting Slenderness ratio
				InsertTreeItem(ThTi, ID_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_rating_bridgeratingdesign_limitingslendernessratio"), m_strSvgPath);
				// Modify Member Type
				InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_LOAD_COMB_TYPE, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_dropmenu1_loadcombinationtype"), m_strSvgPath);
			}

			//(2010.02.19) Modify by Unsang :: Menu와 동일한 순서로 조정함. Steel Design과 Concrete Design의 순서를 변경함.
			/////////////////////////////////////////////
		  // RC Design
		  // sshan .('06.03.02) #if defined(_ORG) 일 경우 RC Desing을 Add 한다.    
	    ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_RC_DESIGN), _T("ico24_temp"), m_strSvgPath);
		  {
				CString strTitle = _T("");
				BOOL bShow = FALSE;

				InsertTreeItem(ThTi, ID_DGN_RC_CODE, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_mesheddesign"), m_strSvgPath);
				strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
				CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
				InsertTreeItem(ThTi, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_modifyconcretematerial"), m_strSvgPath);

				//(2009.12.15) Modify by Unsang :: Common Parameter가 추가되면서 겹치는 항목은 지움. (Asked by JWLEE)
			  //InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);
			  //InsertTreeItem(ThTi, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
			  //InsertTreeItem(ThTi, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_KCI_USD03, _LS(IDS_TREE_TEXT_RC_DESIGN_PARAM), _T("ico24_mods_design_dropmenu2_rcdesignparameter"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_RC_PLATE_PARAM, _LS(IDS_TREE_TEXT_RC_DESIGN_PARAM_PLATE), _T("ico24_mods_design_dropmenu2_rcdesignparameter(plate)"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_CON_REBB2, _LS(IDS_TREE_TEXT_RC_DESIGN_BEAM_BAR_SECT), _T("ico24_mods_design_dropmenu2_beamsectiondataforchecking"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_CON_REBC2, _LS(IDS_TREE_TEXT_RC_DESIGN_COLUMN_BAR_SECT), _T("ico24_seismicperform_capacity_rebarinputforcolumnsection"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_SECTION_REBAR_INPUT, _LS(IDS_TREE_TEXT_RC_DESIGN_GEN_BAR_SECT), _T("ico24_seismicperform_capacity_rebarinputforgeneralsection"), m_strSvgPath);      
		  }

			/////////////////////////////////////////////
		  // Steel Design
		  // sshan .('07.11.30) 
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_temp"), m_strSvgPath);
		  {
				InsertTreeItem(ThTi, ID_DGN_STL_KR_DSTL, _LS(IDS_TREE_TEXT_STL_DESIGN_CODE), _T("ico24_mods_design_dropmenu1_designcode"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_STL_MATD, _LS(IDS_TREE_TEXT_STL_MOD_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu1_modifysteelmaterial"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_STEEL_KR_GEN_SECT, _LS(IDS_TREE_TEXT_DGN_STEEL_GEN_SECT), _T("ico24_rating_bridgeratingdesign_dropmenu1_steelgeneralsectionfordesign"), m_strSvgPath);
			  InsertTreeItem(ThTi, ID_DGN_STEEL_KR_PARAM, _LS(IDS_TREE_TEXT_STL_PARAM), _T("ico24_mods_design_dropmenu1_steeldesignparameter"), m_strSvgPath);			
		  }

			// Unit Member Design
		  InsertTreeItem(hTI[nRootIndex], ID_DGN_RC_UMD, _LS(IDS_TREE_TEXT_MODS_Unit_Member_Design), _T("ico24_mods_umdgsd_midasumd"), m_strSvgPath);
			
			/////////////////////////////////////////////
		  // Bridge Load Rating Design
		  // sshan .('06.03.02) #if defined(_US) 일 경우 RC Desing을 Add 한다.				
			// sshan .('08.03.06) #if defined(_US)(_ORG) 일 경우 RC Desing을 Add 한다.				
		  ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_RATING_DGN, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_temp"), m_strSvgPath);  
			SetRatingDgnTableItemTree();

			// Unit Member Design
			/*
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_MODS_Utility), _T("ico24_temp"), m_strSvgPath)
			{
				InsertTreeItem(ThTi, ID_MODS_UTIL_SECT_CALC, _LS(IDS_TREE_TEXT_MODS_Sectional_Property_Calculator_Pro), _T("ico24_temp"), m_strSvgPath)
				InsertTreeItem(ThTi, ID_MODS_UTIL_TENDON_PROF_GENERATOR, _LS(IDS_TREE_TEXT_MODS_Tendon_Profile_Generator), _T("ico24_temp"), m_strSvgPath)
			}
			*/

		}
	#endif
#endif

		// Dynamic Report Figure - 20090421 KIMJM
#ifdef _MGEN 
#ifdef _JP
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_RESULT_AUTO_MAKE_REPORT_FIGURE, _LS(IDS_TM_WORK_Dynamic_Report_Figure), _T("ico24_mods_smartreport_image"), m_strSvgPath);	
#endif  // end of #ifdef _JP
#endif	// end of #ifdef _MGEN	


	SelectItem(RoothTi);
	Expand(RoothTi,TVE_TOGGLE);
}

void CTreeMenuCtrl::UpdateBuffer(BOOL bAlwaysSet)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc) return;
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;

	LockWindowUpdate();
	SetInUpdateBuffer(TRUE);

	BOOL bPscdItem = FALSE;//PSC
	BOOL bRacdItem = FALSE;//Rating

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
			case(UR_PSCD_ADD): case(UR_PSCD_DEL):
				bPscdItem = TRUE;
				break;
			case(UR_RACD_ADD): case(UR_RACD_DEL):
				bRacdItem = TRUE;
				break;
			default:
				break;
		}
	}
	// Remember Current Position at Tree.
	HTREEITEM   hTIKeep = GetFirstVisibleItem();
	int indx = ItemIndex(hTIKeep);

	Update_InitDataByCode();
	// Redraw Changed Directory - Speed Up.
 	CStringArray aPscCodeName;
	CDBLib::GetPscCodeNameList(aPscCodeName);
	if(aPscCodeName.GetSize()>0)      
	{
		if(bAlwaysSet || bPscdItem) SetPscDgnTableItemTree();
	}

	if(bAlwaysSet || bRacdItem) SetRatingDgnTableItemTree();

#if defined(_MGEN)
	if(bAlwaysSet) SetTowerItemTree();    // PMS:XXXX-JWKWON-20100323 : 일본법인 요청으로 Tower 메뉴 넣음
#endif

	ResetDgnConcreteItemTree();

	UnlockWindowUpdate();
	SetInUpdateBuffer(FALSE);
	// Restore Previous Position at Tree.
	hTIKeep = GetItemAt(indx);
	if(hTIKeep)	SelectSetFirstVisible(hTIKeep);
}

// MNET:4076-SHJUNG-20100611 Gen에서만.. => 사용하려다가 말음. 나중에 사용할 수도 있기에 놔둠.
void CTreeMenuCtrl::ResetDgnGeneralItemTree()
{
#if defined(_MGEN)
	CDBDoc* pDoc=CDBDoc::GetDocPoint();
	if(pDoc==NULL) return;
	
	CString strCode = CDBLib::GetConCodeName();
	BOOL bSeisDgnTypeItemShow = TRUE;
	if(strCode!="KCI-USD07" && strCode!="KCI-USD12")
	{
		T_DCON_D DconD;
		if(!pDoc->m_pAttrCtrl->GetDcon(DconD)) DconD.Initialize();
		if(!DconD.bSpecialSeismic)
			bSeisDgnTypeItemShow = FALSE;
	}

	BOOL bRemake = FALSE;
	HTREEITEM hSeisDgnTypeItem = GetHTreeItemByID(ID_DGN_GEN_SDGN);
	if(bSeisDgnTypeItemShow)
	{
		// "Seismin Design Type" 메뉴가 없으면
		// "Seismin Design Type" 메뉴를 추가하기 위해 워크트리를 다시 구성.
		if(hSeisDgnTypeItem==NULL) bRemake = TRUE;
	}
	else
	{
		// "Seismin Design Type" 메뉴가 있으면
		// "Seismin Design Type" 메뉴를 삭제하기 위해 워크트리를 다시 구성.
		if(bSeisDgnTypeItemShow) bRemake = TRUE;
	}

	if(bRemake==FALSE) return;

	HTREEITEM hParent = GetParentItem(GetHTreeItemByID(ID_DGN_GEN_SEIS));
	if(hParent == NULL) {ASSERT(0); return;}

	if(bRemake)
	{
		HTREEITEM hChildItem = GetChildItem(hParent);	// Design General Level

		while(hChildItem)
		{
			HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
			DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}

		CString strTitle = _T("");
	  BOOL bShow = FALSE;

				// General.
			HTREEITEM ThTi = hParent;
			{
				InsertTreeItem(ThTi, ID_DGN_GEN_DFRM, _LS(IDS_TREE_TEXT_GEN_DEF_FRAME), _T("ico24_rating_bridgeratingdesign_definitionofframe"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_REDU_AUTO, _LS(IDS_TREE_TEXT_GEN_LIVE_REDUCT_FACTOR), _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
				// -------------------------------------------------------------------
				InsertTreeItem(ThTi, ID_DGN_GEN_MEMB, _LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_rating_bridgeratingdesign_memberassignment"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_MEMB_CHANGE, _LS(IDS_TREE_TEXT_GEN_MEMBER_CHANGE_LDIR), _T("ico24_rating_bridgeratingdesign_reversememberlocaldirection"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_HCBM, _LS(IDS_TREE_TEXT_GEN_HAUNCH), _T("ico24_design_general_haunchedbeamassignment"), m_strSvgPath);
				strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
				CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
				InsertTreeItem(ThTi, ID_DGN_GEN_LENG, strTitle, _T("ico24_rating_bridgeratingdesign_unbracedlength(l,lb)"), m_strSvgPath);
				strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
				CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
				InsertTreeItem(ThTi, ID_DGN_GEN_KFAC, strTitle, _T("ico24_rating_bridgeratingdesign_effectivelengthfactor(k)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_rating_bridgeratingdesign_limitingslendernessratio"), m_strSvgPath);
				strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
				CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
				InsertTreeItem(ThTi, ID_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_momentfactor(cm)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_momentmagnifier(b1delta-b,b2delta-s)"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_REDU, _LS(IDS_TREE_TEXT_GEN_MOD_L_REDUCT_FACTOR), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
#if !(defined(_US) || defined(_RUS)) // Change by ZINU.('02.12.20). <- #if defined(_CH).
				InsertTreeItem(ThTi, ID_DGN_GEN_SUEQ, _LS(IDS_TREE_TEXT_GEN_SUEQ), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_GEN_GREQ, _LS(IDS_TREE_TEXT_GEN_GREQ), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
#endif
				InsertTreeItem(ThTi, ID_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_modifymembertype"), m_strSvgPath);
#if defined(_JP) || defined(_ORG)
				InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath);
#endif
				// Coded by Seungjun MNet:No.2393 ('20060919)  
#if defined(_US) || defined(_RUS) || defined(_ORG)
				InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath);
#endif
				
				InsertTreeItem(ThTi, ID_DGN_GEN_LCTB, _LS(IDS_TM_LOAD_CONTRIBUTION), _T("ico24_mods_design_loadcontributionfornonlinearloadcase"), m_strSvgPath);
#if defined(_ORG)
		      //InsertTreeItem(ThTi, ID_DGN_GEN_SEIS_FACTOR, _LS(IDS_TREE_TEXT_GEN_SEIS_FACTOR), _T("ico24_temp"), m_strSvgPath);
		      strTitle = _LS(IDS_TREE_TEXT_GEN_SEIS_TYPE);
					CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_SEIS, strTitle, bShow);
					InsertTreeItem(ThTi, ID_DGN_GEN_SEIS, strTitle, _T("ico24_design_general_seismicloadcombinationtype"), m_strSvgPath);
					
					strTitle = _LS(IDS_TREE_TEXT_GEN_SEIS_DGN_TYPE);
					InsertTreeItem(ThTi, ID_DGN_GEN_SDGN, strTitle, _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
#endif
			}
		}
	
	
#endif
}

// MNET:XXXX-SHJUNG-20100422 Gen에서만..
void CTreeMenuCtrl::ResetDgnConcreteItemTree()
{
#if defined(_MGEN)
	if(!CProduct::IsMovingType(D_PRODUCT_MOVING_US)) return;
	if(CDBDoc::GetDocPoint())
	{
		CString strCode = CDBLib::GetConCodeName();
		int nCode = CDBLib::GetConSubCode();
		
		BOOL bRemake = FALSE;
        if ( strCode=="Eurocode2:04" && nCode==dgn::def::enNationalAnnex::Italy )
		{
			// "Limiting Minimum Section Size" 메뉴 추가
			HTREEITEM hMinSectItem = GetHTreeItemByID(ID_DGN_CON_DCON_MIN_SECT_LIMIT);
			if(hMinSectItem==NULL)
			{
				// "Limiting Minimum Section Size" 메뉴가 없으면
				// "Limiting Minimum Section Size" 메뉴를 추가하기 위해 워크트리를 다시 구성.
				bRemake = TRUE;
			}
			else
			{
				HTREEITEM hRmaxItem = GetHTreeItemByID(ID_DGN_CON_DCON_RMAX);
				// "Limiting Minimum Section Size" 메뉴는 있는데,
				// Rebar 항목의 이름이 "Limiting Rebar Ratio"가 아닌 경우에 항목 이름만 변경.
				if(GetItemText(hRmaxItem) != _LS(IDS_TREE_TEXT_CON_BAR_RATIO))
				{
					SetItemText(hRmaxItem, _LS(IDS_TREE_TEXT_CON_BAR_RATIO));
					return;
				}
			}
		}
        else if ( strCode == CONCODE_IS456_2000 || strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 ||
            strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 ||
            strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
			strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
            strCode == CONCODE_NSCP_2015 || strCode == CONCODE_NTC_DCEC_2017 ||
			strCode == CONCODE_NTC_DCEC_2023 ) // Seungjun '151126 PMS.5166 ACI318-08, 11 단면제한 옵션 추가
		{ // _T("Limiting Minimum Section Size") 메뉴 추가
			HTREEITEM hMinSectItem = GetHTreeItemByID(ID_DGN_CON_DCON_MIN_SECT_LIMIT);
			if(hMinSectItem==NULL)
			{
				// "Limiting Minimum Section Size" 메뉴가 없으면
				// "Limiting Minimum Section Size" 메뉴를 추가하기 위해 워크트리를 다시 구성.
				bRemake = TRUE;
			}
			else
			{
				HTREEITEM hRmaxItem = GetHTreeItemByID(ID_DGN_CON_DCON_RMAX);
				// "Limiting Minimum Section Size" 메뉴는 있는데,
				// Rebar 항목의 이름이 "Limiting Maximum Rebar Ratio"가 아닌 경우에 항목 이름만 변경.
				if(GetItemText(hRmaxItem) != _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO))
				{
					SetItemText(hRmaxItem, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO));
					return;
				}
			}
		}
		else
		{
			HTREEITEM hMinSectItem = GetHTreeItemByID(ID_DGN_CON_DCON_MIN_SECT_LIMIT);
			if(hMinSectItem)
			{
				// "Limiting Minimum Section Size" 메뉴가 있으면
				// "Limiting Minimum Section Size" 메뉴를 삭제하기 위해 워크트리를 다시 구성.
				bRemake = TRUE;
			}
			else
			{
				HTREEITEM hRmaxItem = GetHTreeItemByID(ID_DGN_CON_DCON_RMAX);
				// "Limiting Minimum Section Size" 메뉴는 없는데,
				// Rebar 항목의 이름이 "Limiting Maximum Rebar Ratio"가 아닌 경우에 항목 이름만 변경.
				if(GetItemText(hRmaxItem) != _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO))
				{
					SetItemText(hRmaxItem, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO));
					return;
				}
			}
		}

		if(bRemake==FALSE) return;

		HTREEITEM hParent = GetParentItem(GetHTreeItemByID(ID_DGN_CON_DCON_RMAX));
		if(hParent == NULL) {ASSERT(0); return;}
		if(bRemake)
		{
			HTREEITEM hChildItem = GetChildItem(hParent);	// Design Concrete Lavel

			CString strName = GetItemText(hParent);
			while(hChildItem)
			{
				HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
				DeleteItem(hChildItem);
				hChildItem = hNextItem;
			}
			
			HTREEITEM ThTi = hParent;
			{
				CString strTitle = _T("");
				BOOL bShow = FALSE;
				InsertTreeItem(ThTi, ID_DGN_CON_DCON, _LS(IDS_TREE_TEXT_DGN_CODE), _T("ico24_mods_design_dropmenu2_designcode"), m_strSvgPath);
				strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
				CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
				InsertTreeItem(ThTi, ID_DGN_CON_DCON_SRED, strTitle, _T("ico24_mods_design_dropmenu2_strengthreductionfactor"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_MATD, _LS(IDS_TREE_TEXT_CON_MOD_MATL), _T("ico24_design_design_dropmenu2_modifyconcretematerial"), m_strSvgPath);

				const CString& strCode = CDBLib::GetConCodeName();
				const int nCode = CDBLib::GetConSubCode();
				if(strCode==CONCODE_EC2_04 && nCode==dgn::def::enNationalAnnex::Italy)
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
				else
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_RMAX, _LS(IDS_TREE_TEXT_CON_MAX_BAR_RATIO), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);

                if ( (strCode == CONCODE_EC2_04 && nCode == dgn::def::enNationalAnnex::Italy) || strCode == CONCODE_IS456_2000 ||
                    strCode == CONCODE_ACI318_08 || strCode == CONCODE_ACI318_11 || 
                    strCode == CONCODE_ACI318_14 || strCode == CONCODE_ACI318M_14 || 
                    strCode == CONCODE_ACI318_19 || strCode == CONCODE_ACI318M_19 ||
					strCode == CONCODE_ACI318_25 || strCode == CONCODE_ACI318M_25 ||
                    strCode == CONCODE_NSCP_2015 || strCode == CONCODE_NTC_DCEC_2017 ||
					strCode == CONCODE_NTC_DCEC_2023 ) // Seungjun '151126 PMS.5166 ACI318-08, 11 단면제한 옵션 추가
					InsertTreeItem(ThTi, ID_DGN_CON_DCON_MIN_SECT_LIMIT, _LS(IDS_TREE_TEXT_CON_MIN_SECT_SIZE), _T("ico24_mods_design_dropmenu2_limitingminimumsectionsize"), m_strSvgPath);

				InsertTreeItem(ThTi, ID_DGN_CON_DCRB, _LS(IDS_TREE_TEXT_CON_BAR_CRITERIA), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_REBAR_MEMBER, _LS(IDS_TREE_TEXT_CON_BAR_MEMBER), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_BEAM_REBAR_EQUAL, _LS(IDS_TREE_TEXT_CON_EQUAL_BEAM), _T("ico24_design_design_dropmenu2_samebeamrebaratjoints"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_MRFT, _LS(IDS_TREE_TEXT_CON_MOD_MRFT), _T("ico24_design_design_dropmenu2_momentredistributionfactor"), m_strSvgPath);			
				InsertTreeItem(ThTi, ID_DGN_CON_TRFT, _LS(IDS_TREE_TEXT_CON_MOD_TRFT), _T("ico24_design_design_dropmenu2_torsionreductionfactor"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_SERV, _LS(IDS_TREE_TEXT_CON_MOD_SERV), _T("ico24_mods_design_definitionofframe"), m_strSvgPath);
#ifndef _IGEN
					InsertTreeItem(ThTi, ID_DGN_CON_UCCF, _LS(IDS_TREE_TEXT_CON_MOD_UCCF), _T("ico24_mods_design_dropmenu2_modifyconcretematerials"), m_strSvgPath);//.중국신기준 - 불확정성 분항계수.
#endif
				InsertTreeItem(ThTi, ID_DGN_CON_STAN, _LS(IDS_TREE_TEXT_CON_MOD_STAN), _T("ico24_design_design_dropmenu2_modifymemberstrutangle"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_MCMB, _LS(IDS_TREE_TEXT_CON_MOD_MCMB), _T("ico24_design_design_dropmenu2_momentcalculationmethodforbeam"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_DFBA, _LS(IDS_TREE_TEXT_CON_MOD_DFBA), _T("ico24_design_design_dropmenu2_designforceforbeamassignedasmember"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_REXC, _LS(IDS_TREE_TEXT_CON_MOD_REXC), _T("ico24_design_design_dropmenu2_momentcalculationmethodforbeam"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PMDM, _LS(IDS_MAIN_RIBBON_MENU_PMCM), _T("ico24_design_design_dropmenu2_pmcurvecalculationmethod"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_SCOL, _LS(IDS_MAIN_RIBBON_MENU_SCOL), _T("ico24_design_design_dropmenu2_seismiccolumntype"), m_strSvgPath);

#if defined(_ORG) && defined(_MGEN)
				InsertTreeItem(ThTi, ID_DGN_CON_LOSSFACTOR, _LS(IDS_TREE_TEXT_CON_MOD_LOSSFACTOR), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PSCLCOM, _LS(IDS_TREE_TEXT_CON_MOD_PSCLCOM), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PSCMATD, _LS(IDS_TREE_TEXT_CON_MOD_PSCMATL), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_JSFT, _LS(IDS_TREE_TEXT_CON_MOD_JSFT), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_PSAS, _LS(IDS_TREE_TEXT_CON_MOD_PSAS), _T("ico24_design_design_dropmenu2_modifyprestresslossfactors"), m_strSvgPath);
#endif
				////////////////////////////////////////////////////////////////////////////////////////
				InsertTreeItem(ThTi, ID_DGN_CON_REBB, _LS(IDS_TREE_TEXT_CON_MOD_BEAM_REBAR), _T("ico24_design_design_dropmenu2_modifybeamrebardata"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_REBC, _LS(IDS_TREE_TEXT_CON_MOD_COLM_REBAR), _T("ico24_design_design_dropmenu2_modifycolumnrebardata"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_REBR, _LS(IDS_TREE_TEXT_CON_MOD_BRCE_REBAR), _T("ico24_design_design_dropmenu2_modifycolumnrebardata"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_REBW, _LS(IDS_TREE_TEXT_CON_MOD_WALL_REBAR), _T("ico24_design_design_dropmenu2_modifywallrebardata"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_WMAK, _LS(IDS_TREE_TEXT_CON_MOD_WALL_MARK), _T("ico24_design_design_dropmenu2_modifywallmarkdata"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_BEMW, _LS(IDS_TREE_TEXT_CON_BEM_WALL_ID), _T("ico24_design_design_dropmenu2_modifywallmarkdata"), m_strSvgPath);
				InsertTreeItem(ThTi, ID_DGN_CON_EBMW, _LS(IDS_TREE_TEXT_CON_EBM_WALL_ID), _T("ico24_mods_design_dropmenu2_rebarinputforgeneralsection"), m_strSvgPath);
				//InsertTreeItem(ThTi, ID_DGN_CON_FPSW, _LS(IDS_TREE_TEXT_CON_FPS_WALL), _T("ico24_temp"), m_strSvgPath);
			}
		}
	}

#endif
}

// Add by ZINU.('04.08.20). For PSC Design.
void CTreeMenuCtrl::SetPscDgnTableItemTree()
{
#if defined(_CIVIL)
	T_PSCD_D PscdD;
	PscdD.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(pDoc)	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	HTREEITEM ThTi			 = GetHTreeItemByID(cDGN_PSC_DGN);
	if(ThTi==0) return; // 20080424 mylee
	HTREEITEM hChildItem = GetChildItem(ThTi);	// PSC Design Table Level.
	while(hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}
	
	if(PscdD.iDgnCode==KSCE_USD03 || PscdD.iDgnCode==KSCE_USD05 || PscdD.iDgnCode==KSCE_USD10 ||  
		 PscdD.iDgnCode==KSCE_RAIL_USD04 || PscdD.iDgnCode==KSCE_RAIL_USD11)
	{
		//ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);	
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
	//InsertTreeItem(ThTi, ID_PSC_DESIGN_LCOM, _LS(IDS_TREE_TEXT_PSC_LCOM), _T("ico24_psc_pscdesigndata_transferloadcombination"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
	//InsertTreeItem(ThTi, ID_CMPW_DESIGN_MATL, _LS(IDS_TREE_TEXT_CMPW_MATL), _T("Todo_JP"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_IMPORT_FORCE, _LS(IDS_TREE_TEXT_PSC_IMPORT_FORCE), _T("ico24_psc_pscdesigndata_pscimportforce"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
	
	}
	else if(PscdD.iDgnCode==JSCE02)
	{
		//ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_LCOM, _LS(IDS_TREE_TEXT_PSC_LCOM), _T("ico24_psc_pscdesigndata_transferloadcombination"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_CMPW_DESIGN_MATL, _LS(IDS_TREE_TEXT_CMPW_MATL), _T("Todo_JP"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);    
	//InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_IMPORT_FORCE, _LS(IDS_TREE_TEXT_PSC_IMPORT_FORCE), _T("ico24_psc_pscdesigndata_pscimportforce"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
	}
	else if(PscdD.iDgnCode==JTG_D62_04 || PscdD.iDgnCode==TB_10002_3_05)
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
	//InsertTreeItem(ThTi, ID_PSC_DESIGN_LCOM, _LS(IDS_TREE_TEXT_PSC_LCOM), _T("ico24_psc_pscdesigndata_transferloadcombination"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
	//InsertTreeItem(ThTi, ID_CMPW_DESIGN_MATL, _LS(IDS_TREE_TEXT_CMPW_MATL), _T("Todo_JP"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_PSC_CRACK_WIDTH), _T("ico24_psc_pscdesigndata_psccrackwidthcoefficient"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
	}
	else if(CDBLib::IsPscCodeAASHTO(PscdD.iDgnCode) ||
					CDBLib::IsPscCodeCSA(PscdD.iDgnCode))
	{
		//ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);	
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
	//InsertTreeItem(ThTi, ID_PSC_DESIGN_LCOM, _LS(IDS_TREE_TEXT_PSC_LCOM), _T("ico24_psc_pscdesigndata_transferloadcombination"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
	//InsertTreeItem(ThTi, ID_CMPW_DESIGN_MATL, _LS(IDS_TREE_TEXT_CMPW_MATL), _T("Todo_JP"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_SEGMENT, _LS(IDS_TREE_TEXT_PSC_SEGMENT), _T("ico24_psc_pscdesigndata_pscsegmentassignment"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_PSC_IMPORT_FORCE, _LS(IDS_TREE_TEXT_PSC_IMPORT_FORCE), _T("ico24_psc_pscdesigndata_pscimportforce"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_INTERFACE_SHEAR, _LS(IDS_TREE_TEXT_PSC_INTERFACE_SHEAR), _T("ico24_psc_pscdesigndata_torsion&interfaceshear"), m_strSvgPath); //khd

	}
	else if(PscdD.iDgnCode==EUROCODE2_2_05_PSC)
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
		
		//(2009.11.19) Add by Unsang:: Insert Tree Item : Short/Long term, Serviceability Load Combination Type..
		InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath); // Short/Long term load case... 
		InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath); // Serviceability load combination type...

		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_SHEAR_CONNECTOR, _LS(IDS_TREE_TEXT_PSC_SHEAR_CONNECTOR), _T("ico24_psc_pscdesigndata_shearconnector"), m_strSvgPath);
	}
	else if(PscdD.iDgnCode==ABNT_NBR_6118_2023_PSC)
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
		
		InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath); // Short/Long term load case... 
		InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath); // Serviceability load combination type...

		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_ENVIRONMENTAL_AGGRESSIVENESS_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_SHEAR_CONNECTOR, _LS(IDS_TREE_TEXT_PSC_SHEAR_CONNECTOR), _T("ico24_psc_pscdesigndata_shearconnector"), m_strSvgPath);
	}
	else if(PscdD.iDgnCode==AS_5100_5_17_PSC)
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);

		//(2009.11.19) Add by Unsang:: Insert Tree Item : Short/Long term, Serviceability Load Combination Type..
		InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath); // Short/Long term load case... 
		//InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath); // Serviceability load combination type...

		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_PSC_SHEAR_CONNECTOR, _LS(IDS_TREE_TEXT_PSC_SHEAR_CONNECTOR), _T("ico24_psc_pscdesigndata_shearconnector"), m_strSvgPath);
	}
	else if(PscdD.iDgnCode==KSCE_LSD15_PSC)
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
		
		//(2009.11.19) Add by Unsang:: Insert Tree Item : Short/Long term, Serviceability Load Combination Type..
		InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath); // Serviceability load combination type...
		
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_SHEAR_CONNECTOR, _LS(IDS_TREE_TEXT_PSC_SHEAR_CONNECTOR), _T("ico24_psc_pscdesigndata_shearconnector"), m_strSvgPath);
	}
	else if(PscdD.iDgnCode==BS5400_90_PSC || PscdD.iDgnCode == TMH07_3_1989 || PscdD.iDgnCode==BD_44_15_PSC || PscdD.iDgnCode==CS_455_PSC )
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
        InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
	}
	else if(PscdD.iDgnCode==IRC_112_2011_PSC || PscdD.iDgnCode == IRC_112_2020_PSC)
	{
	  InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);

	  //(2009.11.19) Add by Unsang:: Insert Tree Item : Short/Long term, Serviceability Load Combination Type..
	  InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath); // Short/Long term load case... 
	  InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath); // Serviceability load combination type...

	  InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
	  InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
	  InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
	  InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
	  InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
	  InsertTreeItem(ThTi, ID_PSC_SHEAR_CONNECTOR, _LS(IDS_TREE_TEXT_PSC_SHEAR_CONNECTOR), _T("ico24_psc_pscdesigndata_shearconnector"), m_strSvgPath);
	}
	else if (PscdD.iDgnCode==IRS_PSC)
	{
		InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);

		//(2009.11.19) Add by Unsang:: Insert Tree Item : Short/Long term, Serviceability Load Combination Type..
		InsertTreeItem(ThTi, ID_DGN_GEN_LOAD_DIRECTION, _LS(IDS_TREE_TEXT_GEN_LOAD_DIRECTION), _T("ico24_psc_pscdesigndata_shortlongtermloadcase"), m_strSvgPath); // Short/Long term load case... 
		InsertTreeItem(ThTi, ID_DGN_GEN_SERV_LCOM_TYPE, _LS(IDS_TREE_TEXT_GEN_SERV_LCOM_TYPE), _T("ico24_psc_pscdesigndata_serviceabilityloadcombinationtype"), m_strSvgPath); // Serviceability load combination type...

		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_DGRM, _LS(IDS_TM_Psc_Design_Result_Diagram), _T("ico24_psc_pscresults_pscresultdiagram"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_SHEAR_CONNECTOR, _LS(IDS_TREE_TEXT_PSC_TOR_LONG_SHEAR_IRC), _T("ico24_psc_pscdesigndata_shearconnector"), m_strSvgPath);
	}
	else 
	{
		ASSERT(0);
	}
		
		//ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_PSC_PARAM), _T("ico24_temp"), m_strSvgPath);
		//{
		//InsertTreeItem(ThTi, ID_PSC_DESIGN_PARAMETER, _LS(IDS_TREE_TEXT_PSC_PARAMETER), _T("ico24_psc_designparameter_parameters"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_PSC_DESIGN_LCOM, _LS(IDS_TREE_TEXT_PSC_LCOM), _T("ico24_psc_pscdesigndata_transferloadcombination"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_PSC_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_CMPW_DESIGN_MATL, _LS(IDS_TREE_TEXT_CMPW_MATL), _T("Todo_JP"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		//InsertTreeItem(ThTi, ID_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_PRINT_OPTION), _T("ico24_psc_pscdesigndata_designoutputposition"), m_strSvgPath);
		//} 

#endif
}

void CTreeMenuCtrl::SetRatingDgnTableItemTree()
{
#if defined(_CIVIL)
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
				
	T_RACD_D RacdD;
	RacdD.Initialize();
	if(pDoc)
	{
		if(pDoc->m_pAttrCtrl2->ExistRacd())
			pDoc->m_pAttrCtrl2->GetRacd(RacdD);
		else
		{
			T_PREFERENCE rPref;
			rPref.Initialize();
			pDoc->m_pInitCtrl->GetPreference(rPref);					
			RacdD.nRatingCode = CDBLib::GetPscRatCodeNo(rPref.DgnCode.RCCode);
		}
	}
	else
		RacdD.nRatingCode = KSCE_USD05_RATING;
	

	HTREEITEM ThTi			 = GetHTreeItemByID(cDGN_RATING_DGN);
	if(ThTi==0) return; // 20080424 mylee
	HTREEITEM hChildItem = GetChildItem(ThTi);	// Rating Design Table Level.
	while(hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}
	
	if(RacdD.nRatingCode==KSCE_USD05_RATING)
	{
		InsertTreeItem(ThTi, ID_RATING_DESIGN_CODE, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_CODE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesigncode"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_PARAM_KR, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PARAM), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesigncode"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_LOADCASE_KR, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_LOADCASE_KR), _T("ico24_temp"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_OPTION_KR, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_OPTION), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineratingoption"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_PRINT_KR, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PRINT), _T("ico24_temp"), m_strSvgPath);
	}
	else if(RacdD.nRatingCode==AASHTO_LRFD05_PSC_RATING )
	{
		InsertTreeItem(ThTi, ID_RATING_DESIGN_CODE, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_CODE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesigncode"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_LOADCASE, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_LOADCASE), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineratingcase"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_PARAM, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PARAM), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesigncode"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_GROUP, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_GROUP), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratinggroupsetting"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
	}
	else if (RacdD.nRatingCode==AASHTO_LRFD12_PSC_RATING || RacdD.nRatingCode==AASHTO_LRFD19_PSC_RATING )
	{
		InsertTreeItem(ThTi, ID_RATING_DESIGN_CODE, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_CODE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesigncode"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_LOADCASE, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_LOADCASE), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineratingcase"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_PARAM, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PARAM), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesigncode"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_RATING_DESIGN_GROUP, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_GROUP), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratinggroupsetting"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_RATING_DESIGN_OPTION, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_OPTION), _T("ico24_rating_bridgeratingdesign_dropmenu3_diagonostictestresult"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_RATING_DESIGN_PRINT, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PRINT), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineprintoption"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_PSC_DESIGN_MATL, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_MATL), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignmaterial"), m_strSvgPath);
	}
	else 
	{
		ASSERT(0);
	}   
		
#endif
}

void CTreeMenuCtrl::SetTowerItemTree()
{
	HTREEITEM hParent = GetHTreeItemByID(cSTRUCT_GEOMETRY);
	if(hParent == NULL) {ASSERT(0); return;}

	HTREEITEM hChildItem = GetChildItem(hParent);	// Design General Level

	while(hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		UINT Data = GetItemData(hChildItem);
		if(Data == ID_STRUCT_SW_TOWER_LEG || Data == ID_STRUCT_SW_TOWER_BODY || Data == ID_STRUCT_SW_TOWER_ARM ||
			 Data == ID_STRUCT_SW_WIND_STRUCTURE || Data == ID_SW_PIPE_SUPPORT_STRUCTURE || Data == ID_STRUCT_SW_PIPE_CONNECTION ||
			 Data == ID_MAIN_RIBBON_STRUCT_TANK_WIZARD)  
			DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}


	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc)  return;

	// PMS:XXXX-JWKWON-20100323 : 일본 버전이고 Tower인증을 받았을 때만 Tower메뉴를 넣어줌
	if (pDoc->EnableTower()) 
	{
		HTREEITEM ThTi			 = GetHTreeItemByID(cSTRUCT_GEOMETRY);
		if(ThTi==0) return; 

		InsertTreeItem(ThTi, ID_STRUCT_SW_TOWER_LEG, _LS(IDS_TM_Tower_Leg ), _T("ico24_structure_wizard_towerleg"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_SW_TOWER_BODY, _LS(IDS_TM_Tower_Body), _T("ico24_structure_wizard_towerbody"), m_strSvgPath);
		InsertTreeItem(ThTi, ID_STRUCT_SW_TOWER_ARM, _LS(IDS_TM_Tower_Arm ), _T("ico24_structure_wizard_towerarm"), m_strSvgPath);			
	}

	if (CProduct::IsMovingType(D_PRODUCT_MOVING_ORG))
	{
		if (pDoc->EnableTower()) 
		{
			HTREEITEM ThTi			 = GetHTreeItemByID(cSTRUCT_GEOMETRY);
			if(ThTi==0) return; 
			InsertTreeItem(ThTi, ID_STRUCT_SW_WIND_STRUCTURE, _STR("Window Frame"), _T("ico24_structure_wizard_windowframe"), m_strSvgPath);
		}
	}

	if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) && pDoc->AllowCommand(D_OPTN_ID_PIPE_WIZARD))
	{
		HTREEITEM ThTi			 = GetHTreeItemByID(cSTRUCT_GEOMETRY);
		if(ThTi==0) return; 
		InsertTreeItem(ThTi, ID_SW_PIPE_SUPPORT_STRUCTURE, _STR("Pipe Support"), _T("ico24_structure_wizard_pipesupport"), m_strSvgPath);
	}

	if(CProduct::IsMovingType(D_PRODUCT_MOVING_US))
	{
		HTREEITEM ThTi			 = GetHTreeItemByID(cSTRUCT_GEOMETRY);
		if(ThTi==0) return; 
		InsertTreeItem(ThTi, ID_MAIN_RIBBON_STRUCT_TANK_WIZARD, _LS(IDS_MAIN_RIB_MENU_STRUCT_TOWER_TANK), _T("ico24_structure_wizard_tank"), m_strSvgPath);
	}
	/* // 20151006 shjung 현재는 옵션 안보이지만 기존에 켜진 상태가 유지되어 보이고 있기 때문에 아예 Pipe Connection 메뉴 안보이게 처리 (이해주 대리 요청)
	if(CProduct::IsMovingType(D_PRODUCT_MOVING_ORG) && pDoc->AllowCommand(D_OPTN_ID_PIPE_CONNECT))
	{
		HTREEITEM ThTi			 = GetHTreeItemByID(cSTRUCT_GEOMETRY);
		if(ThTi==0) return; 
		InsertTreeItem(ThTi, ID_STRUCT_SW_PIPE_CONNECTION, _LS(IDS_TM_Pipe_Connection), _T("ico24_temp"), m_strSvgPath);
	}
	*/
}

void CTreeMenuCtrl::DestroyTree()
{
	// 생성되지 않거나 Destroy 함수가 두번 호출되는 경우 비정상종료 방지 
	if(!m_ImageList.GetSafeHandle()) return;

	for (int i=0; i<m_ImageList.GetImageCount(); i++)
		m_ImageList.Remove(0);
	m_ImageList.DeleteImageList();
}

int CTreeMenuCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
	  return -1;
	
	CreateTree();
	LoadMenu();
	EndInsertTreeItem();

	//ToggleStyle(TVS_LINESATROOT);  
	return 0;
}

void CTreeMenuCtrl::OnDestroy() 
{
	MBaseTreeCtrl::OnDestroy();
	DestroyTree();
}

void CTreeMenuCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
/*
	HTREEITEM ht;
	ht = GetSelectedItem();
	if(FALSE == ItemHasChildren(ht))
	{
		CLinkAll::SetMenuBarDialogMode();
	}


	ItemInfo.hItem = ht;
	ItemInfo.mask  = TVIF_CHILDREN;   
	GetItem(&ItemInfo);
	
	if(ItemInfo.cChildren == 0) CLinkAll::SetMenuBarDialogMode();
*/	
/*
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	TV_ITEM ItemInfo = pNMTreeView->itemNew;
	if(ItemInfo.cChildren == 0) CLinkAll::SetMenuBarDialogMode();
*/
	*pResult = 0;
}

/*
WPARAM wParam = MAKEWPARAM(m_pCommandTable[i],0);
AfxGetMainWnd()->SendMessage(WM_COMMAND,wParam,0L);
*/

void CTreeMenuCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM ht;
	ht = GetSelectedItem();
	if(FALSE == ItemHasChildren(ht))
	{
		UINT Data = GetItemData(ht);
		if(Data != 0)
		{
			WPARAM wParam = MAKEWPARAM(Data,0);
			AfxGetMainWnd()->SendMessage(WM_COMMAND,wParam,0L);
		}
	}
}

void CTreeMenuCtrl::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM ht;
	ht = GetSelectedItem();
	if(FALSE == ItemHasChildren(ht))
	{
		UINT Data = GetItemData(ht);
		if(Data != 0)
		{
			WPARAM wParam = MAKEWPARAM(Data,0);
			AfxGetMainWnd()->SendMessage(WM_COMMAND,wParam,0L);
		}
	}
	*pResult = 0;
}


void CTreeMenuCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTreeMenuCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	MBaseTreeCtrl::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	//if(bShow == TRUE) EnableToolTips(TRUE);
}

BOOL CTreeMenuCtrl::GetCustomColor(HTREEITEM hItem)
{
	MBaseTreeCtrl::GetCustomColor(hItem);

	return TRUE;
}

COLORREF CTreeMenuCtrl::GetBkColor()
{
	return RGB(233, 235, 239);
}

void CTreeMenuCtrl::ToggleStyle(DWORD lStyleMask )
{
	/*
	long		lStyleOld;
 
	lStyleOld = m_pTreeCtrl->GetStyle();

	BOOL bSetBits  = !(lStyleMask&lStyleOld);
	if (bSetBits)
		lStyleOld  |= lStyleMask;
	else
		lStyleOld &= ~(lStyleMask);

	SetWindowLongPtr( m_pTreeCtrl->m_hWnd, GWL_STYLE, lStyleOld);
	m_pTreeCtrl->SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	DWORD dwStyle, dwExStyle;
	m_pTreeCtrlX->GetTreeCtrlStyles(dwStyle, dwExStyle);
	if (dwStyle & lStyleMask)
		m_pTreeCtrlX->ModifyTreeCtrlStyles( lStyleMask, 0, 0, 0);
	else
		m_pTreeCtrlX->ModifyTreeCtrlStyles( 0, lStyleMask, 0, 0);
		*/
	long		lStyleOld;

	lStyleOld = GetStyle();
	BOOL bSetBits  = !(lStyleMask&lStyleOld);
	if (bSetBits)
		lStyleOld  |= lStyleMask;
	else
		lStyleOld &= ~(lStyleMask);

	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos( NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
	
	DWORD dwStyle, dwExStyle;
	GetTreeCtrlStyles(dwStyle, dwExStyle);
	if (dwStyle & lStyleMask)
		ModifyTreeCtrlStyles( lStyleMask, 0, 0, 0);
	else
		ModifyTreeCtrlStyles( 0, lStyleMask, 0, 0);
}

// Add by ZINU.('01.1.5).
void CTreeMenuCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;
	
	switch(lHint)
	{
		case D_UPDATE_DEFAULT:
		case D_UPDATE_DB_OPEN: //*/ZINU.
			{
				UpdateBuffer(TRUE);
				break;
			}
		case D_UPDATE_BUFFER_BEFORE:
			// do something...
			break;
		case D_UPDATE_BUFFER_AFTER:
			{
				UpdateBuffer(FALSE);
				break;
			}
		case D_UPDATE_UNIT:
			// do something...
			break;
		case D_UPDATE_DBALL:
			{
				UpdateBuffer(TRUE);
				break;
			}
		default:
			//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
			ASSERT(TRUE);
	}
}

void CTreeMenuCtrl::AdjustTreeMenu(int nCode)
{
	/* 요기 할 차례 
	CMenuCtrl* pMenuCtrl = CMenuCtrl::GetMenuCtrl();
	if (!pMenuCtrl) { ASSERT(0) return; }

	pMenuCtrl->GetMenuListByCode(D_MENU_KIND_MVCD, nCode);

	HTREEITEM hItem;
	hItem = GetHTreeItemByID(ID_DGN_GEN_LENG);
	if (hItem)	SetItemText(hItem, strTitle);
	*/
}

// Add by ZINU.('01.1.5).
void CTreeMenuCtrl::Update_InitDataByCode()
{
	HTREEITEM hItem;
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	// General.
	// Length.
	strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
	CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_DGN_GEN_LENG);
	if(hItem)	SetItemText(hItem, strTitle);
	// K.
	strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
	CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_DGN_GEN_KFAC);
	if(hItem)	SetItemText(hItem, strTitle);
	// Cm.
	strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
	CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_CMFT, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_DGN_GEN_CMFT);
	if(hItem)	SetItemText(hItem, strTitle);
	// Steel.
	// Strength Reduction Factor.
	strTitle = _LS(IDS_TREE_TEXT_STL_ST_REDUCT_FACTOR_LSD);
	CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_DSTL_SRED, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_DGN_STL_DSTL_SRED);
	if(hItem)	SetItemText(hItem, strTitle);
	// Cb.
	strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
	CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_DGN_STL_CBFT);
	if(hItem)	SetItemText(hItem, strTitle);
	// Concrete.
	// Strength Reduction Factor.
	strTitle = _LS(IDS_TREE_TEXT_CON_ST_REDUCT_FACTOR_USD);
	CDBDoc::GetDocPoint()->GetDesignTitleCon(ID_DGN_CON_DCON_SRED, strTitle, bShow, FALSE);
	hItem = GetHTreeItemByID(ID_DGN_CON_DCON_SRED);
	if(hItem)	SetItemText(hItem, strTitle);

	BOOL bUseColdFormed = TRUE;
	if(CTestEnvMgr::IsTestEnvST(_T("UseColdFormed"), _T("yes"))) bUseColdFormed = TRUE;

	if(bUseColdFormed)
	{
		// Cold Formed Steel.    
		// Partial Safety Factors
		strTitle = _LS(IDS_MAIN_RIBBON_PARTIAL_FACT);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_DCFS_PSFT, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_DGN_CF_DCFS_PSFT);
		if(hItem)	SetItemText(hItem, strTitle);
		// Bending Coefficient(Cb)
		strTitle = _LS(IDS_MAIN_RIBBON_BENDING_COEFF_CB);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_DGN_CF_CBFT, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_DGN_CF_CBFT);
		if(hItem)	SetItemText(hItem, strTitle);
	}

}

// Add by ZINU.('01.1.6).
HTREEITEM CTreeMenuCtrl::GetHTreeItemByID(UINT nID)
{
	TVITEM tvitem;
	tvitem.mask		= TVIF_PARAM;
	tvitem.lParam = nID;
	HTREEITEM hItem = FindNextItem(&tvitem, 0);
	if(hItem == 0)	return NULL;
	else						return hItem;
}

// Add by ZINU.('01.1.6).
HTREEITEM CTreeMenuCtrl::FindNextItem(TV_ITEM* pItem, HTREEITEM hItem)
{
	ASSERT(::IsWindow(m_hWnd));
	
	TV_ITEM hNextItem;

	//Clear Item data
	ZeroMemory(&hNextItem, sizeof(hNextItem));
		 
	//The mask is used to retrieve the data to compare
	hNextItem.mask = pItem->mask;
	hNextItem.hItem = (hItem) ? GetNextItem(hItem) : GetRootItem();
	
	//Prepare to compare pszText
	//Testing pItem->pszText protects the code from a client setting the
	//TVIF_TEXT bit but passing in a NULL pointer.
	if((pItem->mask & TVIF_TEXT) && pItem->pszText)
	{
		hNextItem.cchTextMax = (int)_tcslen(pItem->pszText);

		if(hNextItem.cchTextMax)
			 hNextItem.pszText = new TCHAR[++hNextItem.cchTextMax];
	}

	while(hNextItem.hItem)
	{
		if(Compare(pItem, hNextItem))
		{         
			//Copy all the information into pItem and return
			memcpy(pItem, &hNextItem, sizeof(TV_ITEM));

			//Free resources
			if(hNextItem.pszText)	delete hNextItem.pszText;
			return pItem->hItem;
		}
		//The mask is used to retrieve the data to compare and must be
		//reset before calling Compare
		hNextItem.mask = pItem->mask;
		hNextItem.hItem = GetNextItem(hNextItem.hItem);
	}   
	//Set hItem in pItem
	pItem->hItem = NULL;

	//Free resources
	if(hNextItem.pszText)	delete hNextItem.pszText;
	
	return NULL;
}

// Add by ZINU.('01.1.6).
BOOL CTreeMenuCtrl::Compare(TV_ITEM* pItem, TV_ITEM& tvTempItem)
{
	 //This call uses the .mask setting to just retrieve the values
	 //that the client wants to compare.
	 //Get all the data passed in by pItem
	 GetItem(&tvTempItem);

	 //Reset the mask so I can keep track of the matching attributes
	 tvTempItem.mask = 0;

	 if((pItem->mask & TVIF_STATE) && (pItem->state == tvTempItem.state))		
		 tvTempItem.mask |= TVIF_STATE;

	 if((pItem->mask & TVIF_IMAGE) && (pItem->iImage == tvTempItem.iImage))	
		 tvTempItem.mask |= TVIF_IMAGE;

	 if((pItem->mask & TVIF_PARAM) && (pItem->lParam == tvTempItem.lParam))
		 tvTempItem.mask |= TVIF_PARAM;

	 if((pItem->mask & TVIF_TEXT) && pItem->pszText && tvTempItem.pszText && //Don't compare if either is NULL
			!_tcscmp(pItem->pszText, tvTempItem.pszText))
			tvTempItem.mask |= TVIF_TEXT;

	 if((pItem->mask & TVIF_CHILDREN) && (pItem->cChildren == tvTempItem.cChildren))
		 tvTempItem.mask |= TVIF_CHILDREN;

	 if((pItem->mask & TVIF_SELECTEDIMAGE) && (pItem->iSelectedImage == tvTempItem.iSelectedImage))
		 tvTempItem.mask |= TVIF_SELECTEDIMAGE;
	 
	 //If by this point these two values are the same.
	 //tvTempItem.hItem is the desired item
	 return (pItem->mask == tvTempItem.mask);
}

// Add by ZINU.('01.1.6).
// GetNextItem  - Get next item as if outline was completely expanded
// Returns      - The item immediately below the reference item
// hItem        - The reference item
HTREEITEM CTreeMenuCtrl::GetNextItem(HTREEITEM hItem)
{
	HTREEITEM hti;

	if(ItemHasChildren(hItem))	return GetChildItem(hItem);	// return first child
	else
	{
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if((hItem = GetParentItem(hItem)) == NULL) return NULL;
		}
	}
	return hti;
}

void CTreeMenuCtrl::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if(GetSafeHwnd())
	{
		//COLORREF clrBack =  (numColors == -1 ) ? RGB( 192, 220, 192) : RGB( 255, 255, 255);
		//COLORREF clrBack =  (numColors == -1 ) ? GetNearestColor(GetDC()->m_hDC,RGB(192, 220, 192)) : GetNearestColor(GetDC()->m_hDC,RGB(192, 220, 192));
		// change the background color
		//SetBkColor( m_BkColor );
		Invalidate();
	}
}
