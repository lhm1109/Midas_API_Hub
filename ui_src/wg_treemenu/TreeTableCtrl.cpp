// TreeTableCtrl.cpp : implementation file
//
// 항목 추가 및 수정시 주의사항!!
//
// 1. 꼭 순서를 지켜 주세요 (메인메뉴의 같은 순서로)
// 2. 꼭 아이콘(비트맵)을 만들어 같이 붙여 주세요 
//    -> 안 만드시는 분은 나중에 찾아서 응징할 겁니다!! *^^*
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TreeTableCtrl.h"

#include "TreeMenuCtrl.h"

#include "..\wg_main\wg_mainres2.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\AluDgnTool.h"

#include "..\wg_db\DispOptMgr.h"

using namespace mit::frx;

const unsigned int cSTORY_TABLE = 9998;
const unsigned int cDGN_PSC_DGN = 9999;
const unsigned int cDGN_RATING_DGN = 9997;
const unsigned int cDGN_RC_DGN = 9996;

IMPLEMENT_DYNAMIC(CTreeTableCtrl, MBaseTreeCtrl);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeTableCtrl

CTreeTableCtrl::CTreeTableCtrl()
{
	m_BkColor = RGB(192, 220, 192);
}

CTreeTableCtrl::~CTreeTableCtrl()
{
}


BEGIN_MESSAGE_MAP(CTreeTableCtrl, MBaseTreeCtrl)
	//{{AFX_MSG_MAP(CTreeTableCtrl)
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
// CTreeTableCtrl message handlers
/////////////////////////////////////////////////////////////////////////////
// CTreeTableCtrl message handlers

void CTreeTableCtrl::CreateTree()
{
	SetBkColor(RGB(255, 255, 255));
	SetSelBkColor(RGB(192, 192, 192));

	SetTextColor(RGB(0, 0, 0));
	SetSelTextColor(RGB(255, 255, 255));

	((MTreeCtrlFilter*)CreateFilter())->SetFunction([](HTREEITEM hTItem, UINT nCommandID) -> BOOL
	{
		// True == Hidden
		switch (nCommandID)
		{
		case ID_QUERY_DGN_STL_PHIB:
		case ID_QUERY_DGN_PERFOR_MIN_COEFFICEN:
		case ID_QUERY_DGN_PERFOR_DUCTILITY_LEVEL:
		case ID_QUERY_DGN_PERFOR_MEMBER_COEFFICIENT:
		case ID_QUERY_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT:
		case ID_QUERY_DGN_PERFOR_MEMBER_PROPERTIES:
			return CDBLib::GetStlCodeName() == "GB50017-17";
		default:
			return FALSE;
		}
	});
	return;
}

void CTreeTableCtrl::LoadMenu()
{
	HTREEITEM	hTI[20];
	HTREEITEM RoothTi, ThTi;
	// Load the tree with some items to make the demo interesting
	RoothTi = InsertTreeItem(_LS(IDS_TMTB_Tables), _T("ico24_treemenu_tables_tables"));
	// RoothTi=NULL;
//-------------------------------------------------------------------------------------------------
// Structural Table
//-------------------------------------------------------------------------------------------------
	int nRootIndex = 0;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TMTB_Structure_Tables), _T("ico24_treemenu_tables_structuretables"));
	{
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_NODE, _LS(IDS_TMTB_Nodes), _T("ico24_nodeelement_nodes_nodestable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_ELEM, _LS(IDS_TMTB_Elements), _T("ico24_nodeelement_element_elementtable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_SKEWCS, _LS(IDS_TMTB_Node_Local_Axis), _T("ico24_boundary_tables_nodelocalaxis"));
		////////////////////////
		// Properties
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Properties), _T("ico24_properties_tables_propertytables"));
		{
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_MATERIAL, _LS(IDS_TMTB_Material), _T("ico24_properties_tables_material"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_SECTION, _LS(IDS_TMTB_Section), _T("ico24_properties_tables_section"));
#ifndef _TOWER   
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_THICKNESS, _LS(IDS_TMTB_Thickness), _T("ico24_properties_tables_thickness"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_EDMP, _LS(IDS_TMTB_Change_Emem_TimeDep_Property), _T("ico24_properties_tables_changeelementdependentmaterialproperty"));
#endif		

#ifdef _MGEN
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_SECF, _LS(IDS_WG_TREEMENU_ADD3_Section_Stiffness_Scale_Factor), _T("ico24_properties_tables_sectionstiffnessscalefactortable"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_ESSF, _LS(IDS_TM_WORKTREE_Element_Stiffness_Scale_Factor), _T("ico24_properties_tables_elementstiffnessscalefactortable"));

#ifdef _MGEN
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_WSSF, _LS(IDS_TM_Wall_Shear_Stiffness_Reduction_Factor), _T("ico24_properties_tables_wallstiffnessscalefactortable"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PSSF, _LS(IDS_TM_Plate_Shear_Stiffness_Reduction_Factor), _T("ico24_properties_tables_platestiffnessscalefactortable"));
#ifdef _CIVIL   
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_EWSF, _LS(IDS_TMTB_Eff_Width_Scale_Factor), _T("ico24_boundary_tables_effectivewidthscalefactor"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_IEHG, _LS(IDS_TMTB_Inelastic_Hinge), _T("ico24_properties_tables_inelastichinge"));

#ifdef _CIVIL   
#ifdef _JP
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
#endif
#endif				

			BOOL bDumb = FALSE;
#ifdef _CIVIL_CH   
			bDumb = TRUE;
#endif
			if (CTestEnvMgr::GetTestEnvST(_LSX(Test_DUMB)) == _LSX(yes))
				bDumb = TRUE;
			if (bDumb)
			{
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_DUMB, _LS(IDS_MAIN_RIBBON_STRUCT_DUMB), _T("ico24_properties_tables_section"));
			}
		}
		////////////////////////
		// Boundaries
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Boundaries), _T("ico24_boundary_tables_boundarytables"));
		{
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_CONSTRAINT, _LS(IDS_TMTB_Supports), _T("ico24_boundary_tables_supports"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_NSPR, _LS(IDS_TMTB_Point_Spring_Supports), _T("ico24_boundary_tables_pointspringsupports"));
#ifndef _TOWER  
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_GENSPRING, _LS(IDS_TMTB_General_Spring_Supports), _T("ico24_boundary_tables_genenralspringsupports"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_SURFACESPRING, _LS(IDS_TMTB_Surface_Spring_Supports), _T("ico24_boundary_tables_surfacespring"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_ELLINK, _LS(IDS_TMTB_Elastic_Link), _T("ico24_boundary_tables_elasticlink"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_NLNK, _LS(IDS_TMTB_Nonlinear_Link), _T("ico24_boundary_tables_generallink"));//Add 김제헌20020621
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_CGLP, _LS(IDS_TMTB_Change_General_Link_Property), _T("ico24_boundary_tables_changegenerallinkproperties"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FRELEASE, _LS(IDS_TMTB_Beam_End_Release), _T("ico24_boundary_tables_beamendrelease"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_OFFSET, _LS(IDS_TMTB_Beam_End_Offsets), _T("ico24_boundary_tables_beamendoffsets"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PRELEASE, _LS(IDS_TMTB_Plate_End_Release), _T("ico24_boundary_tables_plateendrelease"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_RIGD, _LS(IDS_TMTB_Rigid_Link), _T("ico24_boundary_tables_rigidlink"));
			//InsertTreeItem(ThTi,ID_QUERY_STRUCT_MCON,_LS(IDS_TMTB_Linear_Constraints), _T("ico24_Temp"));
#endif				
			// MNET:XXXX-CJJEONG-20080317
#ifdef _JP
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_MPCT, _LS(IDS_TM_LINEAR_CONSTRAINTS_TABLE), _T("ico24_Temp"));
#endif		
		}
		////////////////////////
		// Mass
#ifndef _TOWER   //추가(김영민)
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Masses), _T("ico24_query_massloadtable_masssummarytable"));
		{
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_NMAS, _LS(IDS_TMTB_Nodal_Mass), _T("ico24_load_loadtables_nodalmasses"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_NBOF, _LS(IDS_TMTB_Nodal_Body_Force), _T("ico24_load_loadtables_nodalbodyforce"));
		}
#endif					//추가(김영민)

		//-------------------------------------------------------------------------------------------------
		// Static Load
		//-------------------------------------------------------------------------------------------------
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Static_Loads), _T("ico24_load_loadtables_staticload"));
		{
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_CNLD, _LS(IDS_TMTB_Nodal_Loads), _T("ico24_load_loadtables_nodalloads"));
   //추가(김영민)
#ifndef _TOWER
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_BMLD, _LS(IDS_TMTB_Beam_Loads), _T("ico24_load_loadtables_beamloads"));
#endif 
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_SDSP, _LS(IDS_TMTB_Specified_Displacements), _T("ico24_load_loadtables_specifieddisplacements"));
#ifndef _TOWER
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PRES, _LS(IDS_TMTB_Pressure_Loads), _T("ico24_load_loadtables_pressureloads"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PNLA, _LS(IDS_TMTB_Plane_Loads), _T("ico24_load_loadtables_planeloads"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FBLD, _LS(IDS_TMTB_Floor_Load_Type), _T("ico24_load_loadtables_floorloadtype"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FBLA, _LS(IDS_TMTB_Floor_Loads), _T("ico24_load_loadtables_floorloads"));
#if !(defined(_US) || defined(_RUS))   // US에서는 마감하중 제거(Civil, Gen)
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FMLD, _LS(IDS_TMTB_Finishing_Loads), _T("ico24_load_loadtables_finishingmaterialloads"));  // added by KYM
#endif
			
#ifdef _MGEN 
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_ARPR, _LS(IDS_DB_DT_ARPR), _T("ico24_load_loadtables_areapressureloads"));
#endif
#ifdef _MGEN_JP
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_EARTHP_STATIC, _LS(IDS_TM_Earth_Pressure_Static), _T("ico24_load_loadtables_staticearthpressure"));
#endif
			
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_NTMP, _LS(IDS_TMTB_Nodal_Temperature), _T("ico24_load_loadtables_nodaltemperatures"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_ETMP, _LS(IDS_TMTB_Element_Temperature), _T("ico24_load_loadtables_elementtemperatures"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_GTMP, _LS(IDS_TMTB_Temperature_Gradient), _T("ico24_load_loadtables_temperaturegradient"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_BTMP, _LS(IDS_TMTB_Beam_Section_Temperature), _T("ico24_load_loadtables_beamsectiontemperatures"));
#if defined(_CIVIL)
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PTMP, _LS(IDS_TMTB_Plate_Thickness_Temperature), _T("ico24_load_loadtables_elementtemperatures"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PRESTR, _LS(IDS_TMTB_Prestress_Loads), _T("ico24_load_loadtables_prestressloads"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_PRETENS, _LS(IDS_TMTB_Pretension_Loads), _T("ico24_load_loadtables_pretensionloads"));
#endif					//추가(김영민)
			//ID_QUERY_STRUCT_TDPL //Tendon Prestress...         "Tendon Prestress Loads..."
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_TDPL, _LS(IDS_TMTB_Tendon_Prestress_Load), _T("ico24_load_loadtables_tendonprestressloads"));
			if (CDBDoc::IsEnableWindPressure())
			{
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_WDPR_AREA, _LS(IDS_TM_Wind_Pressure_Area), _T("ico24_load_loadtables_areawindpressure"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_WDPR_BEAM, _LS(IDS_TM_Wind_Pressure_Beam), _T("ico24_load_loadtables_beamwindpressure"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_WDPR_NODAL, _LS(IDS_TM_Wind_Pressure_Nodal), _T("ico24_load_loadtables_nodalwindpressure"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_WDPR_FUNC, _LS(IDS_TM_Wind_Pressure_Function), _T("ico24_load_loadtables_functionwindpressure"));
			}
#ifdef _MGEN
#ifndef _JP
			if (CDBDoc::IsEnableEarthPressureSeis())
			{
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_EARTHP_SEIS, _LS(IDS_TM_Earth_Pressure_Seismic), _T("ico24_load_loadtables_seismicearthpressure"));
			}
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_EARTHP_STATIC, _LS(IDS_TM_Earth_Pressure_Static), _T("ico24_load_loadtables_staticearthpressure"));
#endif // !_JP
#endif
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_IELC, _LS(IDS_MAIN_RIB_MENU_BNDR_IGNORE_ELEMENT), _T("ico24_boundary_tables_ignoreelementsforloadcases"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_THNL, _LS(IDS_TMTB_Dynamic_Nodal_Loads), _T("ico24_load_loadtables_dynamicnodalloads"));

#ifdef _MGEN
#ifdef _CH
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FIRE, _LS(IDS_TMTB_Temperature_FIRE), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FIPA, _LS(IDS_TMTB_Temperature_FIPA), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_FSSF, _LS(IDS_TMTB_Temperature_FSSF), _T("ico24_Temp"));
#endif
#endif
		}
#if defined(_CIVIL) || defined(_MGEN)
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_IFGS, _LS(IDS_TM1_Initial_Forces_for_Geometric_Stiffness___), _T("ico24_load_loadtables_initialforcesforgeometricstiffness"));
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_THMS, _LS(IDS_TM_Multiple_Support_Excitation), _T("ico24_load_loadtables_multiplesupportexcitation"));
#endif
#if defined(_CIVIL) || defined(_MGEN)
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_TMLD, _LS(IDS_TM1_Time_Loads_For_Construction_Stage___), _T("ico24_load_loadtables_timeloadsforconstructionstage"));
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_CRPC, _LS(IDS_TM1_Creep_Coefficient_for_Construction_Stage___), _T("ico24_load_loadtables_creepcoefficientforconstructionstage"));		
#endif
#ifdef _CIVIL
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_CMCS, _LS(IDS_TM1_Camber_for_Construction_Stage___), _T("ico24_load_loadtables_camberforconstructionstage"));
#endif

#if defined(_CIVIL)
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_STBK, _LS(IDS_TM1_Set_Back_for_Construction_Stage___), _T("ico24_load_loadtables_setbackloadsfornonlinearconstructionstage"));
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_STRUCT_ULCB, _LS(IDS_TM1_Unstrained_Length_of_Cable_for_Construction_Stage___), _T("To_Do_XXXXXXXXXXXXX"));
#endif

#if defined(_MGEN) || defined(_CIVIL)
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Hydration), _T("ico24_load_loadtables_heatofhydration"));
		{
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_HECB, _LS(IDS_TMTB_Element_Convection_Boundary), _T("ico24_load_loadtables_elementconvectionboundary"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_HSPT, _LS(IDS_TMTB_Prescribed_Temperature), _T("ico24_load_loadtables_prescribedtemperatures"));
			InsertTreeItem(ThTi, ID_QUERY_STRUCT_HAHS, _LS(IDS_TMTB_Assign_Heat_Source), _T("ico24_load_loadtables_assignheatsource"));
		}
#endif

#if defined(_CIVIL)
		if (CProduct::IsMovingType(D_PRODUCT_MOVING_JP))   // 일본버전에만 
		{
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_GRID_ANALYSIS_MODEL), _T("ico24_Temp"));
			{
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_MGDR, _LS(IDS_TMTB_GRID_MAIN_GIRDER), _T("ico24_loads_grid_jpn3"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_CBEM, _LS(IDS_TMTB_GRID_CROSS_BEAM), _T("ico24_loads_grid_jpn4"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_GSTY, _LS(IDS_TMTB_GRID_ELEM_STRUCT_TYPE), _T("ico24_loads_grid_jpn9"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_GIMP, _LS(IDS_TMTB_GRID_IMPACT_FACTOR), _T("ico24_loads_grid_jpn10"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_P1LA, _LS(IDS_TMTB_GRID_P1_LOAD_FACTOR), _T("ico24_loads_grid_jpn11"));
				InsertTreeItem(ThTi, ID_QUERY_STRUCT_GMAS, _LS(IDS_TMTB_GRID_GRID_NODAL_MASS), _T("ico24_load_structureloadsmasses_nodalmasses"));
			}
		}
#endif
	}

	//-------------------------------------------------------------------------------------------------
	// Result
	//-------------------------------------------------------------------------------------------------
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TMTB_Result_Tables), _T("ico24_treemenu_tables_resultstables"));
	{
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_REAC, _LS(IDS_TMTB_Reaction), _T("ico24_results_tables_reaction"));
#if defined(_CIVIL)
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_CONC, _LS(IDS_TMTB_Conc_Reaction), _T("ico24_results_tables_concurrent(maxmin)reaction"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_CJFG, _LS(IDS_TMTB_Conc_Reaction_Joint_Force), _T("ico24_results_tables_concurrent(maxmin)reaction"));
#endif
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_DISP, _LS(IDS_TMTB_Displacements), _T("ico24_results_tables_displacements"));
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Truss), _T("ico24_results_tables_truss"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_TRUSS, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_truss"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_TRUSS_F, _LS(IDS_TMTB_Force), _T("ico24_results_tables_truss"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_TRUSS_S, _LS(IDS_TMTB_Stress), _T("ico24_results_tables_truss"));
		}
#ifndef _TOWER   //추가(김영민)
		// MNET:XXXX-CJJEONG-20080317 순서바꿈
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Cable), _T("ico24_results_tables_cable"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CABLE, _LS(IDS_TMTB_Force_Configuration), _T("ico24_results_tables_cable"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CABLE_F, _LS(IDS_TMTB_Force), _T("ico24_results_tables_cable"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CABLE_C, _LS(IDS_TMTB_Configuration), _T("ico24_results_tables_cable"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CABLE_EFFICIENCY, _LS(IDS_TMTB_Cable_Efficiency), _T("ico24_results_tables_cable"));
		}

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Beam), _T("ico24_results_tables_beam"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_BEAM, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_beam"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_BEAM_F, _LS(IDS_TMTB_Force), _T("ico24_results_tables_beam"));
#ifdef _CIVIL
			InsertTreeItem(ThTi, ID_QUERY_RESULT_BEAM_FP, _LS(IDS_TMTB_Force_P), _T("ico24_results_tables_beam"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_RESULT_BEAM_S, _LS(IDS_TMTB_Stress), _T("ico24_results_tables_beam"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_BEAM_SD, _LS(IDS_TMTB_Stress_Detail), _T("ico24_results_tables_beam"));
#ifdef _CIVIL
			InsertTreeItem(ThTi, ID_QUERY_RESULT_BEAM_SI, _LS(IDS_TMTB_Stress_IC), _T("ico24_results_tables_beam"));
#endif
		}

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Plate), _T("ico24_results_tables_plate"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_FL, _LS(IDS_TMTB_Force_Local_), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_FG, _LS(IDS_TMTB_Force_Global_), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_FUL, _LS(IDS_TMTB_Force_Unit_Length_), _T("ico24_results_tables_plate"));
			//InsertTreeItem(ThTi,ID_QUERY_RESULT_PLATE_FULG,_LS(IDS_TMTB_Force_Unit_Length_Global_), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_SL, _LS(IDS_TMTB_Stress_Local_), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_SG, _LS(IDS_TMTB_Stress_Global_), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_STRAIN_L, _LS(IDS_MAIN_RIBBON_QUERY_RESULT_PLATE_StrL), _T("ico24_results_tables_plate"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLATE_STRAIN_G, _LS(IDS_MAIN_RIBBON_QUERY_RESULT_PLATE_StrG), _T("ico24_results_tables_plate"));
		}

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Plane_Stress), _T("ico24_results_tables_planestress"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLANE, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_planestress"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLANE_FL, _LS(IDS_TMTB_Force_Local_), _T("ico24_results_tables_planestress"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLANE_FG, _LS(IDS_TMTB_Force_Global_), _T("ico24_results_tables_planestress"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLANE_SL, _LS(IDS_TMTB_Stress_Local_), _T("ico24_results_tables_planestress"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLANE_SG, _LS(IDS_TMTB_Stress_Global_), _T("ico24_results_tables_planestress"));
		}

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Plane_Strain), _T("ico24_results_tables_planestrain"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLSTRN, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_planestrain"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLSTRN_FL, _LS(IDS_TMTB_Force_Local_), _T("ico24_results_tables_planestrain"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLSTRN_FG, _LS(IDS_TMTB_Force_Global_), _T("ico24_results_tables_planestrain"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLSTRN_SL, _LS(IDS_TMTB_Stress_Local_), _T("ico24_results_tables_planestrain"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PLSTRN_SG, _LS(IDS_TMTB_Stress_Global_), _T("ico24_results_tables_planestrain"));
		}

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Axisymmetric), _T("ico24_results_tables_axisymmetric"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_AXISYM, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_axisymmetric"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_AXISYM_FL, _LS(IDS_TMTB_Force_Local_), _T("ico24_results_tables_axisymmetric"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_AXISYM_FG, _LS(IDS_TMTB_Force_Global_), _T("ico24_results_tables_axisymmetric"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_AXISYM_SL, _LS(IDS_TMTB_Stress_Local_), _T("ico24_results_tables_axisymmetric"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_AXISYM_SG, _LS(IDS_TMTB_Stress_Global_), _T("ico24_results_tables_axisymmetric"));
		}

		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Solid), _T("ico24_results_tables_solid"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID, _LS(IDS_TMTB_Force___Stress), _T("ico24_results_tables_solid"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID_FL, _LS(IDS_TMTB_Force_Local_), _T("ico24_results_tables_solid"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID_FG, _LS(IDS_TMTB_Force_Global_), _T("ico24_results_tables_solid"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID_SL, _LS(IDS_TMTB_Stress_Local_), _T("ico24_results_tables_solid"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID_SG, _LS(IDS_TMTB_Stress_Global_), _T("ico24_results_tables_solid"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID_STRAIN_L, _LS(IDS_MAIN_RIBBON_QUERY_RESULT_SOLID_StrL), _T("ico24_results_tables_solid"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_SOLID_STRAIN_G, _LS(IDS_MAIN_RIBBON_QUERY_RESULT_SOLID_StrG), _T("ico24_results_tables_solid"));
		}
#ifdef _MGEN
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Wall), _T("ico24_results_tables_wall"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_WALL, _LS(IDS_TMTB_Force_Moment), _T("ico24_results_results_wallforcesmoments"));
		}
#endif
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_ELNK, _LS(IDS_TMTB_Elastic_Link), _T("ico24_results_tables_elasticlink"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_NLNK, _LS(IDS_TMTB_Nonlinear_Link), _T("ico24_results_tables_generallink"));
#ifdef _CIVIL
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_FORCES_RFRCD, _LS(IDS_TMTB_RESULTANT_FORCE), _T("ico24_results_results_resultantforcediagram"));
		if (CDBLib::ActivateVBEM()) // 960_R2
		{
			InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STRESS_SECV, _LS(IDS_MAIN_RIBBON_RESULT_STRESS_SECV), _T("ico24_results_results_resultantforcediagram"));
		}
#endif
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_EIMODE, _LS(IDS_TMTB_Vibration_Mode_Shape), _T("ico24_results_tables_vibrationmodeshape"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_BKMODE, _LS(IDS_TMTB_Buckling_Mode_Shape), _T("ico24_results_tables_bucklingmodeshape"));
		// Nodal Results of RS Table
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_RSNODAL, _LS(IDS_TMTB_Nodal_Results_RS), _T("ico24_results_tables_nodalresultsofrs"));

		// MNET:1952-BJLEE(BBONG)-20060320
#endif   //추가(김영민)

#ifdef _MGEN    
		ThTi = InsertTreeItem(hTI[nRootIndex], cSTORY_TABLE, _LS(IDS_TMTB_Story), _T("ico24_results_tables_story"));
		{
			SetStoryTable();
		}
#endif

		// Inelastic Hinge
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Inelastic_Hinge), _T("ico24_results_tables_inelastichinge"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_EVENT_TIME, _LS(IDS_TMTB_IEHG_EVENT_TIME), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_BEAM_SUMMARY, _LS(IDS_TMTB_IEHG_BEAM_SUMMARY), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_WALL_SUMMARY, _LS(IDS_TMTB_IEHG_WALL_SUMMARY), _T("ico24_pushover_pushovertables_wallsummary"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_TRUS_SUMMARY, _LS(IDS_TMTB_IEHG_TRUSS_SUMMARY), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_SPRING_SUMMARY, _LS(IDS_TMTB_IEHG_SPRING_SUMMARY), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_FIBER_BEAM_SUMMARY, _LS(IDS_TMTB_FIBR_BEAM_SUMMARY), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_FIBER_WALL_SUMMARY, _LS(IDS_TMTB_FIBR_WALL_SUMMARY), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_FORCE, _LS(IDS_TMTB_IEHG_Force), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_DEFORM, _LS(IDS_TMTB_IEHG_Deformation), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_ELEM_ROT, _LS(IDS_MAIN_RIBBON_QUERY_RESULT_IEHG_ELEM_ROT), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_DUCT1, _LS(IDS_TMTB_IEHG_Duct_Factor1), _T("ico24_results_tables_inelastichinge"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_IEHG_DUCT2, _LS(IDS_TMTB_IEHG_Duct_Factor2), _T("ico24_results_tables_inelastichinge"));
		}

		// Time History Analysis
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TMTB_Time_History_Analysis), _T("ico24_results_tables_timehistoryanalysis"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_THIS_DVA, _LS(IDS_TMTB_Displ_Velo_Accel), _T("ico24_results_tables_timehistoryanalysis"));
#if defined(_CIVIL)
			InsertTreeItem(ThTi,ID_QUERY_RESULT_THIS_DVA_CONCUR, _LS(IDS_TMTB_Displ_Velo_Accel_Cuncur), _T("ico24_results_tables_timehistoryanalysis"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_RESULT_THIS_BFORC, _LS(IDS_TMTB_THIS_Force_Beam), _T("ico24_results_tables_timehistoryanalysis"));

#if !defined(_CIVIL)
			InsertTreeItem(ThTi, ID_QUERY_RESULT_THIS_WFORC, _LS(IDS_TMTB_THIS_Force_Wall), _T("ico24_results_tables_timehistoryanalysis"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_RESULT_THIS_TFORC, _LS(IDS_TMTB_THIS_Force_Trus), _T("ico24_results_tables_timehistoryanalysis"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_THIS_NLNK, _LS(IDS_TMTB_THIS_General_Link), _T("ico24_results_tables_timehistoryanalysis"));
		}


		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_HYDH_Hydration_Heat_Analysis), _T("ico24_results_tables_heatofhydrationanalysis"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_HY_STRS, _LS(IDS_TM_HYDH_Stress), _T("ico24_results_tables_heatofhydrationanalysis"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_HY_TEMPER, _LS(IDS_TM_HYDH_Temperature), _T("ico24_results_tables_heatofhydrationanalysis"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_HY_DISP, _LS(IDS_TM_HYDH_Displacements), _T("ico24_results_tables_heatofhydrationanalysis"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_HY_TENSION, _LS(IDS_TM_HYDH_Allowable_Tensile_Stress), _T("ico24_results_tables_heatofhydrationanalysis"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_HY_PNTEMP, _LS(IDS_TM_HYDH_Pipe_Cooling_Nodal_Temperature), _T("ico24_results_tables_heatofhydrationanalysis"));
		}

		// KYE-HONG추가 
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_TANDON_Tendon), _T("ico24_results_tables_tendon"));
		{
			InsertTreeItem(ThTi, ID_QUERY_TENDON_COORD, _LS(IDS_TM_TANDON_Tendon_Coordinate), _T("ico24_results_tables_tendon"));
			InsertTreeItem(ThTi, ID_QUERY_TENDON_ELONGATION, _LS(IDS_TM_TANDON_Tendon_Elongation), _T("ico24_results_tables_tendon"));
#ifdef _CIVIL
			InsertTreeItem(ThTi, ID_QUERY_TENDON_ARRANGE, _LS(IDS_TM_TANDON_Tenddon_Arrangement), _T("ico24_results_tables_tendon"));
			InsertTreeItem(ThTi, ID_QUERY_TENDON_LOSS, _LS(IDS_TM_TANDON_Tenddon_Loss), _T("ico24_results_tables_tendon"));
			// MNET:XXXX-CJJEONG-20080317
#if defined(_US) || defined(_RUS)
			InsertTreeItem(ThTi, ID_QUERY_TENDON_APPROX_LOSS, _LS(IDS_TM_TANDON_Tenddon_Approx_Loss), _T("ico24_results_tables_tendon"));
#endif
#endif
			InsertTreeItem(ThTi, ID_QUERY_TENDON_WEIGHT, _LS(IDS_TM_TANDON_Tenddon_Weight), _T("ico24_results_tables_tendon"));
			// MNET:XXXX-CJJEONG-20080519
#if defined(_CIVIL) && (defined(_US) || defined(_RUS))
			InsertTreeItem(ThTi, ID_QUERY_TENDON_STRESS_LIMIT_CHECK, _LS(IDS_TM_TANDON_Tenddon_Stress_Limit_Check), _T("ico24_results_tables_tendon"));
#endif    
		}

#ifdef _CIVIL
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_COMPOSITE_SECTION_CS), _T("ico24_results_tables_compositesectionforcs"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CSCS_BFBS, _LS(IDS_TM_BEAM_FORCE_STRESS), _T("ico24_results_tables_compositesectionforcs"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CSCS_BFRC, _LS(IDS_TM_BEAM_FORCE), _T("ico24_results_tables_compositesectionforcs"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CSCS_BSTR, _LS(IDS_TM_BEAM_STRESS), _T("ico24_results_tables_compositesectionforcs"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CSCS_SCFS, _LS(IDS_TM_BEAM_SELF_CONST_FORCE_STRESS), _T("ico24_results_tables_compositesectionforcs"));
		}
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_CONSTRUCTION_STAGE), _T("ico24_results_tables_constructionstage"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CS_ELEM_PROP, _LS(IDS_TM_ELEMENT_PROPERTY_STAGE), _T("ico24_results_tables_constructionstage"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CS_SECT_PROP, _LS(IDS_TM_BEAM_SECTION_PROPERTY_STAGE), _T("ico24_results_tables_constructionstage"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CS_LOFF_TRUSS, _LS(IDS_TM_LOFF_TRUSS), _T("ico24_results_tables_lackoffitforce"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CS_LOFF_BEAM, _LS(IDS_TM_LOFF_BEAM), _T("ico24_results_tables_lackoffitforce"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CS_LOFF_PLATE, _LS(IDS_TM_LOFF_PLATE), _T("ico24_results_tables_lackoffitforce"));
		}
#endif

#ifdef _MGEN
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TM_DISPLACEMENT_PARTICIPATION), _T("ico24_results_table_displacementparticipationfactor"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PARTFACTOR_ELEM, _LS(IDS_TM_PARICIPATION_ELEMENT), _T("ico24_results_table_displacementparticipationfactor"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_PARTFACTOR_SECT, _LS(IDS_TM_PARICIPATION_SECTTHICK), _T("ico24_results_table_displacementparticipationfactor"));
		}
#endif

#ifdef _CIVIL
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_EQMF, _LS(IDS_TM_EQMF_FORCE), _T("ico24_results_tables_equilibriumelementnodalforce"));
#endif
		ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_INMF, _LS(IDS_TM_INMF_FORCE), _T("ico24_results_tables_initialelementforce"));
#ifdef _MGEN
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_IPDT, _LS(IDS_TMTB_Imperfection), _T("ico24_results_tables_imperfection"));
#endif
	}

	//-------------------------------------------------------------------------------------------------
	// Design
	//-------------------------------------------------------------------------------------------------
	// Add by ZINU.('02.4.15).
#if defined(_TOWER)
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TABLE_TITLE), _T("ico24_Temp"));
	ThTi = InsertTreeItem(hTI[nRootIndex], ID_QUERY_DESIGN_TOWER_SLTYPE, _LS(IDS_TREE_TEXT_TWR_SLENDER_TYPE), _T("ico24_Temp"));
#endif

#ifndef _CIVIL
#ifndef _TOWER    //추가(김영민)  
	// Change by ZINU.('01.1.5).
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TABLE_TITLE), _T("ico24_treemenu_tables_designtables"));
	{
		CString strTitle = _T("");
		BOOL bShow = FALSE;
		///////////////////////////////////
		// General Design
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_GEN_DGN_TITLE), _T("ico24_mods_design_generaldesigntable"));
		{
			strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_LENG, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LENG, strTitle, _T("ico24_mods_design_generaldesigntableunbracedlength(l,lb)"));
			strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_KFAC, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_KFAC, strTitle, _T("ico24_mods_design_generaldesigntableeffectivelengthfactor(k)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_mods_design_generaldesigntablelimitingslendernessratio"));
			strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_CMFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_generaldesigntablemomentfactor(cm)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));

			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_BCIF, _LS(IDS_MAIN_BUCKING_CURVE_IMPERFACTION_FACTOR), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_PMCR, _LS(IDS_MAIN_BUCKING_CURVE_POINT_LOAD_APPL_MCR), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));

			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_REDU, _LS(IDS_TREE_TEXT_GEN_MOD_L_REDUCT_FACTOR), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
#if !(defined(_US) || defined(_RUS)) // Change by ZINU.('02.12.20). <- #if defined(_CH).
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_SUEQ, _LS(IDS_TREE_TEXT_GEN_SUEQ), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_GREQ, _LS(IDS_TREE_TEXT_GEN_GREQ), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
			//InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_SLCM, _LS(IDS_TREE_TEXT_GEN_SPECIAL_LOAD_COMB), _T("ico24_design_general_generaldesigntableseismicloadcombinationtype"));   // gen 에서만 사용됨..
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_SLCM, _LS(IDS_TREE_TEXT_GEN_MDESIGNATION_SEIS_DESIGN), _T("ico24_design_general_generaldesigntableseismicloadcombinationtype"));   // gen 에서만 사용됨..
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_SDGN, _LS(IDS_TREE_TEXT_GEN_SEIS_DGN_TYPE), _T("ico24_mods_design_definitionofframe"));   // gen 에서만 사용됨..
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_ULCT, _LS(IDS_TREE_TEXT_GEN_ULCT), _T("ico24_design_general_generaldesigntableundergroundloadcombinationtype"));   // gen 에서만 사용됨..
#ifdef _MGEN_CH
			InsertTreeItem(ThTi, ID_QUERY_DAMP_CODE_CHECK_TYPE_TB, _LS(IDS_TREE_TEXT_GEN_DAMP_TYPE), _T("ico24_Temp"));   // gen 에서만 사용됨..
			InsertTreeItem(ThTi, ID_QUERY_DAMP_CODE_CHECK_TYPE_TB, _LS(IDS_TREE_TEXT_GEN_DAMP_TYPE), 118, 118);
			InsertTreeItem(ThTi, ID_QUERY_REINFORCE_SECT_TB, _LS(IDS_TREE_TEXT_GEN_REINFORCE_SECT), 118, 118);
#endif
		}
		///////////////////////////////////
		// Steel
		m_hTIStlDgn = ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_mods_design_dropmenu1_steeldesignresulttable"));
		{
			strTitle = _LS(IDS_TREE_TEXT_CON_MOD_SERV);
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_SERV, strTitle, _T("ico24_mods_design_dropmenu1_serviceabilityparameters2"));
			strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
			CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_QUERY_DGN_STL_CBFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress2"));
			//InsertTreeItem(ThTi,ID_QUERY_DESIGN_STEEL_UCFA,	_LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_SLRS, _LS(IDS_TREE_TEXT_STL_SLRS), _T("ico24_design_design_dropmenu1_seismicloadresistingsystembymember"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_SMSP, _LS(IDS_MAIN_COMBINED_STRENGTH_METHOD_OF_SOLID_PIPE), _T("ico24_mods_design_dropmenu1_combinedratiocalculationmethodforcircularsection"));
			//
#if defined(_MGEN_CH)
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PERFOR_MIN_COEFFICEN, _LS(IDS_TREE_TEXT_PERFORMANCE_COEFFICIENT_MIN), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PERFOR_MEMBER_COEFFICIENT, _LS(IDS_TREE_TEXT_PERFORMANCE_MEMBER_COEFFICIENT), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PERFOR_DUCTILITY_LEVEL, _LS(IDS_TREE_TEXT_PERFORMANCE_DUCTILITY_LEVEL), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PERFOR_MEMBER_PROPERTIES, _LS(IDS_TREE_TEXT_PERFORMANCE_MEMBER_PROPERTIES), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT, _LS(IDS_TREE_TEXT_PERFORMANCE_NON_PLASTIC_FORCE_COEFFICIENT), _T("ico24_mods_design_dropmenu1_steeloptimaldesign"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_SECTION_TYPE, _LS(IDS_TREE_TEXT_STL_SECT_CLASSIFY), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_BEND_MODULUS_RTO, _LS(IDS_TREE_TEXT_STL_BEND_MODULUS_RTO), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_SPDF,				_LS(IDS_TREE_TEXT_STL_CROSSECT_PLASTIC_DEVELOP_FAC), 35, 35);
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_SDSW,				_LS(IDS_TREE_TEXT_STL_SIDESWAY_CHARACT), 35, 35);
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_APPRAISAL_IMEL,		_LS(IDS_TREE_TEXT_STL_APPRASIAL_ASSIGN_IMPORTANT_ELEMENT), 16, 16);
#endif
		}
		///////////////////////////////////
		// Concrete
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_CON_DGN_TITLE), _T("ico24_design_design_dropmenu2_modifycolumnrebardata2"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_MRFT, _LS(IDS_MAIN_MOMENT_REDIST_FACT), _T("ico24_design_design_dropmenu2_momentredistributionfactor2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_TRFT, _LS(IDS_MAIN_TORSION_REDUCT_FAC), _T("ico24_design_design_dropmenu2_torsionreductionfactor2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_SERV, _LS(IDS_MAIN_SERIVECE_PARAMS), _T("ico24_mods_design_dropmenu1_serviceabilityparameters2"));
#ifndef _IGEN
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_UCCF, _LS(IDS_MAIN_UNCERTAIN_LOAD_COMB_FACT), _T("ico24_design_design_dropmenu2_uncertaintyloadcombinationfactor2"));
#endif
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_STAN, _LS(IDS_MAIN_STRUT_ANGLE), _T("ico24_design_design_dropmenu2_modifymemberstrutangle2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_MCMB, _LS(IDS_TREE_TEXT_CON_MOD_MCMB), _T("ico24_design_design_dropmenu2_momentcalculationmethodforbeam2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_DFBA, _LS(IDS_TREE_TEXT_CON_MOD_DFBA), _T("ico24_design_design_dropmenu2_designforceforbeamassignedasmember2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_REXC, _LS(IDS_MAIN_REBAR_EXPOSURE_CONDITION), _T("ico24_design_design_dropmenu2_momentcalculationmethodforbeam2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_PMDM, _LS(IDS_MAIN_RIBBON_MENU_PMCM), _T("ico24_design_design_dropmenu2_pmcurvecalculationmethod2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_SCOL, _LS(IDS_MAIN_RIBBON_MENU_SCOL), _T("ico24_design_design_dropmenu2_seismiccolumntype2"));

			BOOL bORG = CProduct::IsMovingType(D_PRODUCT_MOVING_ORG);
			if (bORG)
			{
				InsertTreeItem(ThTi, ID_QUERY_DGN_CON_JSFT, _LS(IDS_MAIN_MODI_JACKING_STRESS), _T("ico24_design_design_dropmenu2_modifyjackingstresses2"));
				InsertTreeItem(ThTi, ID_QUERY_DGN_CON_PSAS, _LS(IDS_MAIN_SPECIAL_PRESTRESS_ALLOW), _T("ico24_design_design_dropmenu2_modifyjackingstresses2"));
			}

			strTitle = _LS(IDS_TREE_TEXT_STL_DGN_CRITERIA);
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_DCBALL, strTitle, _T("ico24_design_design_dropmenu2_uncertaintyloadcombinationfactor2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_REBB, _LS(IDS_TREE_TEXT_CON_MOD_BEAM_REBAR), _T("ico24_design_design_dropmenu2_modifybeamrebardata2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_REBC, _LS(IDS_TREE_TEXT_CON_MOD_COLM_REBAR), _T("ico24_design_design_dropmenu2_modifycolumnrebardata2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_REBR, _LS(IDS_TREE_TEXT_CON_MOD_BRCE_REBAR), _T("ico24_design_design_dropmenu2_modifycolumnrebardata2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_REBW, _LS(IDS_TREE_TEXT_CON_MOD_WALL_REBAR), _T("ico24_design_design_dropmenu2_modifywallrebardata2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_BEMW, _LS(IDS_TREE_TEXT_CON_BEM_WALL_ID), _T("ico24_design_design_dropmenu2_boundaryelementmethodbywallid2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_EBMW, _LS(IDS_TREE_TEXT_CON_EBM_WALL_ID), _T("ico24_mods_design_dropmenu1_serviceabilityparameters2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_CON_SERV_SLAB, _LS(IDS_MAIN_RIBBON_MENU_MeshedDesign_Slab_Serv_TB), _T("ico24_design_design_dropmenu3_slabdesigncontrol"));
		}
		///////////////////////////////////
		// SRC
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_SRC_DGN_TITLE), _T("ico24_treemenu_tables_srcdesigntable"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DGN_SRC_RBSB, _LS(IDS_TREE_TEXT_SRC_MOD_BEAM), _T("ico24_mods_design_generaldesigntable"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_SRC_REBS, _LS(IDS_TREE_TEXT_SRC_MOD_COLM), _T("ico24_mods_design_generaldesigntable"));
		}

		BOOL bUseColdFormed = TRUE;
		if (CTestEnvMgr::IsTestEnvST(_T("UseColdFormed"), _T("yes"))) bUseColdFormed = TRUE;

		if (bUseColdFormed)
		{
			///////////////////////////////////
			// Cold Formed Steel
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_CFS_DGN_TITLE), _T("ico24_treemenu_tables_coldformedsteeldesigntable"));
			{
				strTitle = _LS(IDS_TREE_TEXT_CON_MOD_SERV);
				InsertTreeItem(ThTi, ID_QUERY_DGN_CF_SERV, strTitle, _T("ico24_design_design_dropmenu5_serviceabilityparameters2"));
				strTitle = _LS(IDS_MAIN_RIBBON_BENDING_COEFF_CB);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_CBFT, strTitle, bShow);
				InsertTreeItem(ThTi, ID_QUERY_DGN_CF_CBFT, strTitle, _T("ico24_design_design_dropmenu5_serviceabilityparameters2"));

#if defined(_MGEN_CH)
				//Add by tss(2017.8.10)
				strTitle = _LS(IDS_MAIN_RIBBON_STABILITY_COEF_PAI);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_STABILITY_COEF, strTitle, bShow);
				InsertTreeItem(ThTi, ID_QUERY_DGN_CF_STABILITY_COEF, strTitle, _T("ico24_design_design_dropmenu5_serviceabilityparameters2"));
				strTitle = _LS(IDS_MAIN_RIBBON_OPENSECTION_COEF);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_OPENSECT_RESTCOEF, strTitle, bShow);
				InsertTreeItem(ThTi, ID_QUERY_DGN_CF_OPENSECT_RESTCOEF, strTitle, _T("ico24_design_design_dropmenu5_serviceabilityparameters2"));
				
				strTitle = _LS(IDS_MAIN_RIBBON_MENU_TABLE_CF_PHI);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_SPHI, strTitle, bShow);
				InsertTreeItem(ThTi, ID_QUERY_DGN_CF_SPHI, strTitle, _T("ico24_design_design_dropmenu5_serviceabilityparameters2"));

				strTitle = _LS(IDS_MAIN_RIBBON_PURLIN);
				CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_PURLIN, strTitle, bShow);
				InsertTreeItem(ThTi, ID_QUERY_DGN_CF_PURLIN, strTitle, 64 + 45, 64 + 45);
#endif
			}
		}
		//////////////////////////////////////////////////////////////////////////
		if (IAluDgnTool::IsUseAluDgn())
		{
			ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_DB_DT_DALU), _T("ico24_Temp"));
			{
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_TABLE_ALU_FORCERIB);
				InsertTreeItem(ThTi, ID_TABLE_ALU_FORCERIB, strTitle, _T("ico24_Temp"));
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_TABLE_ALU_HEATRANGE);
				InsertTreeItem(ThTi, ID_TABLE_ALU_HEATRANGE, strTitle, _T("ico24_Temp"));
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_TABLE_ALU_WELD_RATIO);
				InsertTreeItem(ThTi, ID_TABLE_ALU_WELD_RATIO, strTitle, _T("ico24_Temp"));
				strTitle = CDBDoc::GetDocPoint()->GetDesignTitleAlu(ID_TABLE_ALU_BEND_RATIO);
				InsertTreeItem(ThTi, ID_TABLE_ALU_BEND_RATIO, strTitle, _T("ico24_Temp"));
			}
		}
		///////////////////////////////////
		// Pushover
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_POVR_DGN_TITLE), _T("ico24_pushover_pushovertables_pushoverhingeresult"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DGN_POVR_HNGE, _LS(IDS_TREE_TEXT_CON_POVR_HNGE), _T("ico24_pushover_pushovertables_pushoverhinge"));
			//!@#$ MNET:1367 20050804 LBI
			InsertTreeItem(ThTi, ID_QUERY_DGN_HNGE_STAT, _LS(IDS_TREE_TEXT_CON_HNGE_STATUS), _T("ico24_pushover_pushovertables_pushoverhingeproperties"));

			// MNET:2762-JUAE-20070702
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_EVENT_STEP_TB, _LS(IDS_TREE_TEXT_PO_TB_EVENTSTEP), _T("ico24_pushover_pushovertables_yieldstep"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SHEAR_YIELD_TB, _LS(IDS_TREE_TEXT_PO_SHEAR_YIELD_TB), _T("ico24_pushover_pushovertables_shearyieldelement"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_BEAM_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_BEAMSUMM), _T("ico24_pushover_pushovertables_beamsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_WALL_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_WALLSUMM), _T("ico24_pushover_pushovertables_wallsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_TRUSS_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_TRUSSSUMM), _T("ico24_pushover_pushovertables_trusssummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SPRING_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_SPRINGSUMM), _T("ico24_pushover_pushovertables_gl-linksummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SPRING_SUPPORT_SUMM_TB, _LS(IDS_MAIN_RIBBON_PO_SPRING_SUPPORT_SUMM_TB), _T("ico24_pushover_pushovertables_springsupportsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_FIBER_BEAM_SUM_TB, _T("Fiber Beam Summary"), _T("ico24_pushover_pushovertables_fiberbeamsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_FIBER_WALL_SUM_TB, _T("Fiber Wall Summary"), _T("ico24_pushover_pushovertables_fiberwallsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_FORCE_TB, _LS(IDS_TREE_TEXT_PO_TB_FORCE), _T("ico24_pushover_pushovertables_force"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_DEFORM), _T("ico24_pushover_pushovertables_deformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_PLASTIC_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_PLASTICDEFORM), _T("ico24_pushover_pushovertables_plasticdeformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT1_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT1), _T("ico24_pushover_pushovertables_ductilityfactor(dd1)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT2_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT2), _T("ico24_pushover_pushovertables_ductilityfactor(dd2)"));			
			if (CProduct::GetMovingType() != D_PRODUCT_MOVING_ORG) // MNET:4000 20080325 - Gen !KOR
				InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SAFETY_TB, _LS(IDS_TREE_TEXT_PO_TB_SAFETY), _T("ico24_pushover_pushovertables_safetyverification"));

		}
	}
#endif		
#else     // CIVIL이면...

#if defined(_ORG)//한국 버전일경우 add 071212 sshan (Civil Steel KR 메뉴정리작업)

	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TABLE_TITLE), _T("ico24_treemenu_tables_designtables"));
	{
		/*
		///////////////////////////////////
		// Steel
	  ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_Temp"));
	  {
		  InsertTreeItem(ThTi, ID_QUERY_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection2"));
			InsertTreeItem(ThTi, ID_DGN_STEEL_KR_PARAM_TABLE, _LS(IDS_TREE_TEXT_DGN_STEEL_PARAM_TABLE), _T("ico24_mods_design_dropmenu1_steeldesignparametertable"));
			InsertTreeItem(ThTi, ID_DGN_STEEL_KR_RESULT_TABLE, _LS(IDS_TREE_TEXT_DGN_STEEL_RESULT_TABLE), _T("ico24_mods_design_dropmenu1_steeldesignresulttable"));
		}
		///////////////////////////////////
		// RC
	  ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_RC_DESIGN), _T("ico24_Temp"));
	  {
		  InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
		}
		*/
		///////////////////////////////////
		// PSC
		ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_PSC_DGN, _LS(IDS_TREE_TEXT_PSC_DGN_TITLE), _T("ico24_psc_designparameter_parameters"));
		CStringArray aPscCodeName;
		CDBLib::GetPscCodeNameList(aPscCodeName);
		if (aPscCodeName.GetSize() > 0)
		{
			SetPscDgnTableItemTree();
		}

		////////////////////////////////////
		//Bridge Load Rating
		//ThTi = InsertTreeItem( hTI[nRootIndex],cDGN_RATING_DGN, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_Temp"));
	  //SetBridgeLoadRatingDesignTableItemTree();
	  /*
		  ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_Temp"));
		  {
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES11, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SUMMARY_SERVICE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES12, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SUMMARY_STRENGTH), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES01, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_CONCRETE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES02, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STEEL_TENSION), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES03, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FLEXURE_STR), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES04, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SHEAR_STR), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES05, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_CONCRETE_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES06, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STEEL_TENSION_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES07, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FLEXURE_STR_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			  InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES08, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SHEAR_STR_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		  }
	  #endif*/


	  //nRootIndex++;
	  //International Design
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_DGN_INTERNATIONAL), _T("ico24_treemenu_tables_internationaldesign"));
		{
			CString strTitle = _T("");
			BOOL bShow = FALSE;

			// General Design
			HTREEITEM ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_DGN_COMMON_PARAM), _T("ico24_mods_design_commonpara"));
			{
				strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
				CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_LENG, strTitle, bShow);
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_GEN_LENG, strTitle, _T("ico24_mods_design_generaldesigntableunbracedlength(l,lb)"));
				strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
				CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_KFAC, strTitle, bShow);
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_GEN_KFAC, strTitle, _T("ico24_mods_design_generaldesigntableeffectivelengthfactor(k)"));
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_mods_design_generaldesigntablelimitingslendernessratio"));
				strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
				CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_CMFT, strTitle, bShow);
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_generaldesigntablemomentfactor(cm)"));
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
			}
			// Steel
			ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_mods_design_dropmenu1_steeldesign"));
			{
				strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
				CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_QUERY_DGN_STL_CBFT, strTitle, bShow);
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)2"));
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress2"));
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection2"));
				//InsertTreeItem(ThTiDgn, ID_QUERY_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));  //add by maxiao 2015-10-12
			}
			// Concrete        	      
			ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_CON_DGN_TITLE), _T("ico24_mods_design_dropmenu2_rcdesign"));
			{
				InsertTreeItem(ThTiDgn, ID_QUERY_DESIGN_CONC_CVL_CTC_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR), _T("ico24_mods_design_dropmenu2_scaleupfactorforcolumn2"));
				//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
				/*InsertTreeItem(ThTiDgn,ID_QUERY_DESIGN_CONC_CVL_ULS_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR), _T("ico24_Temp"));*/
			}
			// SRC        	      
			ThTiDgn = InsertTreeItem(ThTi, 0, _LS(IDS_TREE_TEXT_SRC_DGN_TITLE), _T("ico24_mods_design_dropmenu3_srcdesign"));
			{
				InsertTreeItem(ThTiDgn, ID_QUERY_DGN_SRC_REBS, _LS(IDS_TREE_TEXT_SRC_MOD_COLM), _T("ico24_mods_design_dropmenu3_modifysrcsectiondata2"));
			}
		}
		///////////////////////////////////
		// Pushover    
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_POVR_DGN_TITLE), _T("ico24_pushover_pushovertables_pushoverhingeresult"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DGN_POVR_HNGE, _LS(IDS_TREE_TEXT_CON_POVR_HNGE), _T("ico24_pushover_pushovertables_pushoverhinge"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_HNGE_STAT, _LS(IDS_TREE_TEXT_CON_HNGE_STATUS), _T("ico24_pushover_pushovertables_pushoverhingeproperties"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_EVENT_STEP_TB, _LS(IDS_TREE_TEXT_PO_TB_EVENTSTEP), _T("ico24_pushover_pushovertables_yieldstep"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SHEAR_YIELD_TB, _LS(IDS_TREE_TEXT_PO_SHEAR_YIELD_TB), _T("ico24_pushover_pushovertables_shearyieldelement"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_BEAM_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_BEAMSUMM), _T("ico24_pushover_pushovertables_beamsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_TRUSS_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_TRUSSSUMM), _T("ico24_pushover_pushovertables_trusssummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SPRING_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_SPRINGSUMM), _T("ico24_pushover_pushovertables_gl-linksummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_FORCE_TB, _LS(IDS_TREE_TEXT_PO_TB_FORCE), _T("ico24_pushover_pushovertables_force"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_DEFORM), _T("ico24_pushover_pushovertables_deformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_PLASTIC_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_PLASTICDEFORM), _T("ico24_pushover_pushovertables_plasticdeformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT1_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT1), _T("ico24_pushover_pushovertables_ductilityfactor(dd1)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT2_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT2), _T("ico24_pushover_pushovertables_ductilityfactor(dd2)"));
			if (CProduct::GetMovingType() == D_PRODUCT_MOVING_US)
				InsertTreeItem(ThTi, ID_QUERY_DGN_CHORK_ROATION_TB, _LS(IDS_TREE_TEXT_PO_TB_CHORD), _T("ico24_pushover_pushovertables_chordrotationcheck"));
			//InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SAFETY_TB, _LS(IDS_TREE_TEXT_PO_TB_SAFETY       ), _T("ico24_pushover_pushovertables_safetyverification")); // MNET:4000 20080325 - Civil Kor
		}
	}

#elif defined(_JP)
	{ // 설계 테이블 생략
	}
#else//한국 이외의 국가
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TREE_TEXT_DGN_TABLE_TITLE), _T("ico24_Temp"));
	{
#if defined _CH	//CH 일경우
		//////////////////////////////////
		//RC Design (CH)
		// Add by sshan. Civil RC(CH) Result Table MNET:2607.('20070102)
		ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_RC_DGN, _LS(IDS_TREE_TEXT_RC_CH_DESIGN), _T("ico24_Temp"));
		{
			//Beam
			InsertTreeItem(ThTi, ID_QUERY_RC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_OPTION), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RC_PRINT_OPTION, _LS(IDS_TREE_TEXT_RC_CH_PRINT_OPTION), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_RC_CH_CRACK_WIDTH), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RC_DESIGN_SEISMIC, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_SEISMIC), _T("ico24_Temp"));
			//InsertTreeItem(ThTi,ID_QUERY_RC_EFFECT_LENGTH,	_LS(IDS_TREE_TEXT_RC_CH_DESIGN_EFFECT_LENG), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES01, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_STRESS), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES02, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_TENSILE), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES03, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_COMP), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES04, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_CRACK), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES05, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_REQUIRED_LONGI), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES06, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_REQUIRED_COMM), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES07, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_BENDING), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES08, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_SHEAR), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES09, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_TORSION), _T("ico24_Temp"));
			//Column
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES01, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_CHECK_CRACK), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES02, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_REQUIRED_LONGI), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES03, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_AXES_COMP), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES04, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_AXES_TENSILE), _T("ico24_Temp"));
			// [06/02/2007 : maxiao] For CIVIL(CH RC Design) 2007
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RCBEAM_FORCE, _LS(IDS_TREE_TEXT_RC_CH_DGN_BEAM_FORCE), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RCCOLUMN_FORCE, _LS(IDS_TREE_TEXT_RC_CH_DGN_COLUMN_FORCE), _T("ico24_Temp"));
			// [3992] bjshin 09.05.27					
		}

		///////////////////////////////////
		// PSC Design
		ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_PSC_DGN, _LS(IDS_TREE_TEXT_PSC_DGN_TITLE), _T("ico24_Temp"));
		CStringArray aPscCodeName;
		CDBLib::GetPscCodeNameList(aPscCodeName);
		if (aPscCodeName.GetSize() > 0)
		{
			SetPscDgnTableItemTree();
		}

		CString strTitle = _T("");
		BOOL bShow = FALSE;
		///////////////////////////////////
		// General Design
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_GEN_DGN_TITLE), _T("ico24_Temp"));
		{
			strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_LENG, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LENG, strTitle, _T("ico24_mods_design_generaldesigntableunbracedlength(l,lb)"));
			strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_KFAC, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_KFAC, strTitle, _T("ico24_mods_design_generaldesigntableeffectivelengthfactor(k)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_mods_design_generaldesigntablelimitingslendernessratio"));
			strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_CMFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_generaldesigntablemomentfactor(cm)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
		}
		///////////////////////////////////
		// Steel
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_Temp"));
		{
			strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
			CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_QUERY_DGN_STL_CBFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress2"));
			//InsertTreeItem(ThTi,ID_QUERY_DESIGN_STEEL_UCFA,	_LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection2"));
			//InsertTreeItem(ThTi, ID_QUERY_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));  //add by maxiao 2015-10-12
		}

		///////////////////////////////////
		// Pushover
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_POVR_DGN_TITLE), _T("ico24_pushover_pushovertables_pushoverhingeresult"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DGN_POVR_HNGE, _LS(IDS_TREE_TEXT_CON_POVR_HNGE), _T("ico24_pushover_pushovertables_pushoverhinge"));
			//!@#$ MNET:1367 20050804 LBI
			InsertTreeItem(ThTi, ID_QUERY_DGN_HNGE_STAT, _LS(IDS_TREE_TEXT_CON_HNGE_STATUS), _T("ico24_pushover_pushovertables_pushoverhingeproperties"));
			// MNET:2762-JUAE-20070702
			// Civil은 Wall제외 
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_EVENT_STEP_TB, _LS(IDS_TREE_TEXT_PO_TB_EVENTSTEP), _T("ico24_pushover_pushovertables_yieldstep"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SHEAR_YIELD_TB, _LS(IDS_TREE_TEXT_PO_SHEAR_YIELD_TB), _T("ico24_pushover_pushovertables_shearyieldelement"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_BEAM_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_BEAMSUMM), _T("ico24_pushover_pushovertables_beamsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_TRUSS_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_TRUSSSUMM), _T("ico24_pushover_pushovertables_trusssummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SPRING_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_SPRINGSUMM), _T("ico24_pushover_pushovertables_gl-linksummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_FORCE_TB, _LS(IDS_TREE_TEXT_PO_TB_FORCE), _T("ico24_pushover_pushovertables_force"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_DEFORM), _T("ico24_pushover_pushovertables_deformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_PLASTIC_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_PLASTICDEFORM), _T("ico24_pushover_pushovertables_plasticdeformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT1_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT1), _T("ico24_pushover_pushovertables_ductilityfactor(dd1)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT2_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT2), _T("ico24_pushover_pushovertables_ductilityfactor(dd2)"));
			if (CProduct::GetMovingType() == D_PRODUCT_MOVING_US)
				InsertTreeItem(ThTi, ID_QUERY_DGN_CHORK_ROATION_TB, _LS(IDS_TREE_TEXT_PO_TB_CHORD), _T("ico24_pushover_pushovertables_chordrotationcheck"));
			//InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SAFETY_TB, _LS(IDS_TREE_TEXT_PO_TB_SAFETY       ), _T("ico24_pushover_pushovertables_safetyverification")); // MNET:4000 20080325 - Civil !KOR !JP
		}


#else//CH 아닐경우.


		CString strTitle = _T("");
		BOOL bShow = FALSE;
		///////////////////////////////////
		// General Design
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_GEN_DGN_TITLE), _T("ico24_Temp"));
		{
			strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_LENG, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LENG, strTitle, _T("ico24_mods_design_generaldesigntableunbracedlength(l,lb)"));
			strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_KFAC, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_KFAC, strTitle, _T("ico24_mods_design_generaldesigntableeffectivelengthfactor(k)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_mods_design_generaldesigntablelimitingslendernessratio"));
			strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
			CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_CMFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_CMFT, strTitle, _T("ico24_mods_design_generaldesigntablemomentfactor(cm)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_FMAG, _LS(IDS_TREE_TEXT_GEN_MOM_MAGNIFIER), _T("ico24_mods_design_generaldesigntablemomentmagnifier(b1delta-b,b2delta-s)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
		}
		///////////////////////////////////
		// Steel
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_Temp"));
		{
			strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
			CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_QUERY_DGN_STL_CBFT, strTitle, bShow);
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_CBFT, strTitle, _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_CVFT, _LS(IDS_TREE_TEXT_STL_SHEAR_COEFF), _T("ico24_mods_design_dropmenu1_shearcoefficient(cv)2"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_DALW, _LS(IDS_TREE_TEXT_STL_ALLOW_STRESS), _T("ico24_mods_design_dropmenu1_specifyallowablestress2"));
			//InsertTreeItem(ThTi,ID_QUERY_DESIGN_STEEL_UCFA,	_LS(IDS_TREE_TEXT_STL_UNITY_CTRL_FACTOR), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection2"));
			//InsertTreeItem(ThTi, ID_QUERY_DGN_STL_PHIB, _LS(IDS_TREE_TEXT_STL_STAB_COEFF), _T("ico24_mods_design_dropmenu1_bendingcoefficient(cb)2"));  //add by maxiao 2015-10-12
		}

		///////////////////////////////////
		// Concrete
		// Change by Jaeoh ('05.07.15) if define(_ORG) || define(_CH) 삭제
		// Change by sshan .('06.03.02) #if defined(_ORG) 일 경우 Add 하지 않는다
#if defined(_US) || defined(_RUS) || defined(_JP)
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_CON_DGN_TITLE), _T("ico24_Temp"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DESIGN_CONC_CVL_CTC_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_CTC_FACTOR), _T("ico24_mods_design_dropmenu2_scaleupfactorforcolumn2"));
			//(2009.07.10) Add by Unsang :: SPEC변경에 따라 해당 입력창은 EC2-2:05, Colm_Dgn에 적용하지 않음.
			/*InsertTreeItem(ThTi,ID_QUERY_DESIGN_CONC_CVL_ULS_FACTOR, _LS(IDS_TREE_TEXT_CON_CVL_ULS_FACTOR), _T("ico24_Temp"));*/
		}
#endif

		///////////////////////////////////
		// Pushover
#if !defined(_JP)   // 일본 토목용 버전은 제외
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_POVR_DGN_TITLE), _T("ico24_pushover_pushovertables_pushoverhingeresult"));
		{
			InsertTreeItem(ThTi, ID_QUERY_DGN_POVR_HNGE, _LS(IDS_TREE_TEXT_CON_POVR_HNGE), _T("ico24_pushover_pushovertables_pushoverhinge"));
			//!@#$ MNET:1367 20050804 LBI
			InsertTreeItem(ThTi, ID_QUERY_DGN_HNGE_STAT, _LS(IDS_TREE_TEXT_CON_HNGE_STATUS), _T("ico24_pushover_pushovertables_pushoverhingeproperties"));
			// MNET:2762-JUAE-20070702
			// Civil은 Wall제외 
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_EVENT_STEP_TB, _LS(IDS_TREE_TEXT_PO_TB_EVENTSTEP), _T("ico24_pushover_pushovertables_yieldstep"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SHEAR_YIELD_TB, _LS(IDS_TREE_TEXT_PO_SHEAR_YIELD_TB), _T("ico24_pushover_pushovertables_shearyieldelement"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_BEAM_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_BEAMSUMM), _T("ico24_pushover_pushovertables_beamsummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_TRUSS_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_TRUSSSUMM), _T("ico24_pushover_pushovertables_trusssummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SPRING_SUMM_TB, _LS(IDS_TREE_TEXT_PO_TB_SPRINGSUMM), _T("ico24_pushover_pushovertables_gl-linksummary"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_FORCE_TB, _LS(IDS_TREE_TEXT_PO_TB_FORCE), _T("ico24_pushover_pushovertables_force"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_DEFORM), _T("ico24_pushover_pushovertables_deformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_PLASTIC_DFRM_TB, _LS(IDS_TREE_TEXT_PO_TB_PLASTICDEFORM), _T("ico24_pushover_pushovertables_plasticdeformation"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT1_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT1), _T("ico24_pushover_pushovertables_ductilityfactor(dd1)"));
			InsertTreeItem(ThTi, ID_QUERY_DGN_PO_DUCT2_TB, _LS(IDS_TREE_TEXT_PO_TB_DUCT2), _T("ico24_pushover_pushovertables_ductilityfactor(dd2)"));
			if (CProduct::GetMovingType() == D_PRODUCT_MOVING_US)
				InsertTreeItem(ThTi, ID_QUERY_DGN_CHORK_ROATION_TB, _LS(IDS_TREE_TEXT_PO_TB_CHORD), _T("ico24_pushover_pushovertables_chordrotationcheck"));
			//InsertTreeItem(ThTi, ID_QUERY_DGN_PO_SAFETY_TB, _LS(IDS_TREE_TEXT_PO_TB_SAFETY       ), _T("ico24_pushover_pushovertables_safetyverification")); // MNET:4000 20080325 - Civil !KOR !JP
		}
#endif
		///////////////////////////////////
		// PSC Design
#if defined(_CH) || defined(_JP)  || defined(_US)|| defined(_RUS)
		ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_PSC_DGN, _LS(IDS_TREE_TEXT_PSC_DGN_TITLE), _T("ico24_Temp"));
		CStringArray aPscCodeName;
		CDBLib::GetPscCodeNameList(aPscCodeName);
		if (aPscCodeName.GetSize() > 0)
		{
			SetPscDgnTableItemTree();
		}
#endif

		//Bridge Load Rating
#if defined(_US) || defined(_RUS) || defined(_ORG)
		ThTi = InsertTreeItem(hTI[nRootIndex], cDGN_RATING_DGN, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_Temp"));
		SetBridgeLoadRatingDesignTableItemTree();

		/*
		ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_Temp"));
		{
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES11, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SUMMARY_SERVICE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES12, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SUMMARY_STRENGTH), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES01, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_CONCRETE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES02, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STEEL_TENSION), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES03, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FLEXURE_STR), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES04, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SHEAR_STR), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES05, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_CONCRETE_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES06, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STEEL_TENSION_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES07, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FLEXURE_STR_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES08, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SHEAR_STR_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		}*/
#endif

		// Add by sshan. CPG Design Result Table('20070111)
#if defined(_US) || defined(_RUS)// || _ORG
		ThTi = InsertTreeItem(hTI[nRootIndex], 0, _LS(IDS_TREE_TEXT_DGN_CPG_TABLE), _T("ico24_Temp"));
		{
			InsertTreeItem(ThTi, ID_QUERY_CPG_DESIGN_POSI, _LS(IDS_TREE_TEXT_DGN_CPG_CHECK), _T("ico24_mods_design_dropmenu4_designtables"));
			InsertTreeItem(ThTi, ID_QUERY_CPG_POSI_DGN_OUT, _LS(IDS_TREE_TEXT_DGN_CPG_POSI_DGN_OUT), _T("ico24_mods_design_dropmenu4_designtables"));
			InsertTreeItem(ThTi, ID_QUERY_CPG_DESIGN_SHEAR_CONNECT, _LS(IDS_TREE_TEXT_DGN_CPG_SHEAR), _T("ico24_mods_design_dropmenu4_designtables"));
			InsertTreeItem(ThTi, ID_QUERY_CPG_DESIGN_TRAN_STIF, _LS(IDS_TREE_TEXT_DGN_CPG_TREN), _T("ico24_mods_design_dropmenu4_designtables"));
			InsertTreeItem(ThTi, ID_QUERY_CPG_DESIGN_TYPE_LOAD, _LS(IDS_TREE_TEXT_DGN_CPG_LOAD), _T("ico24_mods_design_dropmenu4_designtables"));
			InsertTreeItem(ThTi, ID_QUERY_CPG_DESIGN_TORSION, _LS(IDS_TREE_TEXT_DGN_CPG_TORSION), _T("ico24_mods_design_dropmenu4_designtables"));
			InsertTreeItem(ThTi, ID_QUERY_CPG_DESIGN_DAMAGE, _LS(IDS_TREE_TEXT_DGN_CPG_DAMAGE), _T("ico24_mods_design_dropmenu4_designtables"));

			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES01, _LS(IDS_TREE_TEXT_DGN_CPG_BEND_RESIST), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES02, _LS(IDS_TREE_TEXT_DGN_CPG_VERT_SHEAR), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES03, _LS(IDS_TREE_TEXT_DGN_CPG_LATER_TORS), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES04, _LS(IDS_TREE_TEXT_DGN_CPG_TRAN_FORCE), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES05, _LS(IDS_TREE_TEXT_DGN_CPG_LONG_SHEAR), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES06, _LS(IDS_TREE_TEXT_DGN_CPG_FATIGUE), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES07, _LS(IDS_TREE_TEXT_DGN_CPG_BEAM_STRESS), _T("ico24_mods_design_dropmenu4_designresulttables"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_CPG_RES08, _LS(IDS_TREE_TEXT_DGN_CPG_LONG_SHEAR_SLS), _T("ico24_mods_design_dropmenu4_designresulttables"));
		}
#endif

#endif//CH 아니면 끝
	}

#endif  //한국이 아니면 끝  


#endif    
	//-------------------------------------------------------------------------------------------------
	// Query
	//-------------------------------------------------------------------------------------------------
	nRootIndex++;
	hTI[nRootIndex] = InsertTreeItem(RoothTi, 0, _LS(IDS_TMTB_Query_Tables), _T("ico24_treemenu_tables_querytables"));
	{
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_NODE, _LS(IDS_TMTB_Node_Detail), _T("ico24_query_detailtable_nodedetailtable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_ELEMENT, _LS(IDS_TMTB_Element_Detail), _T("ico24_query_detailtable_elementdetailtable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_ELEMWGT, _LS(IDS_TMTB_Element_Weight), _T("ico24_query_detailtable_elementweighttable"));
#ifdef _MGEN
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_DESIGN, _LS(IDS_TMTB_Design_Parameter_Detail), _T("ico24_query_detailtable_designparameterdetailtable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STOR_WEIGHT, _LS(IDS_TMTB_Story_Weight), _T("ico24_query_weightmassloadtable_storyweighttable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STOR_LOAD, _LS(IDS_TMTB_Story_Load), _T("ico24_query_weightmassloadtable_storyloadtable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_STOR_MASS, _LS(IDS_TMTB_Story_Mass), _T("ico24_query_weightmassloadtable_storymasstable"));
#endif

#ifndef _TOWER 
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_NODAL_MASS, _LS(IDS_TMTB_Mass_Summary), _T("ico24_query_massloadtable_masssummarytable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_TOTAL_LOAD, _LS(IDS_TMTB_Load_Summary), _T("ico24_query_massloadtable_loadsummarytable"));
		InsertTreeItem(hTI[nRootIndex], ID_QUERY_RESULT_CS_GROUP, _LS(IDS_TM_Group_Activation_Summary_Table), _T("ico24_GroupActivationofCS"));
#endif  
	}

	//-------------------------------------------------------------------------------------------------
	// MODS
	//-------------------------------------------------------------------------------------------------

		// Add, Jaeoh. (08.11.03)
		// 20120316 KIMJM MODS Module 메뉴 삭제 (이정우 대리님 요청)
	// #if defined(_CIVIL)
	//   #if defined(_ORG)
	//     nRootIndex++;
	//     hTI[nRootIndex] = InsertTreeItem(RoothTi,0, _LS(IDS_TREE_TEXT_MODS_MODULE), _T("ico24_Temp")); // _T("MODS Module")
	//     {
	//       //InsertTreeItem(hTI[nRootIndex], ID_QUERY_PROJECT, _LS(IDS_TM_Project_Status), _T("ico24_query_status_projectstatus"));
	//       
	//       //(2009.12.15) Add by Unsang :: Common Parameter's Table!
	//       ///////////////////////////////////
	//       // Common Parameter
	//       ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_DGN_COMMON_PARAM), _T("ico24_Temp"));
	// 		  {
	//         CString strTitle = _T("");
	//         BOOL bShow = FALSE;
	//         // Unbraced Length
	//         strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
	// 			  CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strTitle, bShow);
	// 			  InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LENG, strTitle, _T("ico24_mods_design_generaldesigntableunbracedlength(l,lb)"));
	//         // Effective Length Factor
	//         strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
	// 			  CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_KFAC, strTitle, bShow);
	// 			  InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_KFAC, strTitle, _T("ico24_mods_design_generaldesigntableeffectivelengthfactor(k)"));
	//         // Limiting Slenderness ratio
	//         InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_LTSR, _LS(IDS_TREE_TEXT_GEN_SLEN_RATIO), _T("ico24_mods_design_generaldesigntablelimitingslendernessratio"));
	//         // Modify Member Type
	//         InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));
	//       }
	// 
	//       ///////////////////////////////////
	//       // Steel
	// 		  ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_STL_DGN_TITLE), _T("ico24_Temp"));
	// 		  {
	// 			  InsertTreeItem(ThTi, ID_QUERY_DGN_STL_STFN, _LS(IDS_TREE_TEXT_STL_STIFF_BOX), _T("ico24_mods_design_dropmenu1_longitudinalstiffenerofboxsection2"));
	//         InsertTreeItem(ThTi, ID_DGN_STEEL_KR_PARAM_TABLE, _LS(IDS_TREE_TEXT_DGN_STEEL_PARAM_TABLE), _T("ico24_mods_design_dropmenu1_steeldesignparametertable"));
	//         InsertTreeItem(ThTi, ID_DGN_STEEL_KR_RESULT_TABLE, _LS(IDS_TREE_TEXT_DGN_STEEL_RESULT_TABLE), _T("ico24_mods_design_dropmenu1_steeldesignresulttable"));
	//       }
	// 
	//       //(2009.12.15) Add by Unsang :: Common Parameter 추가로 중복입력 발생하여 생략함.
	//       /*
	//       ///////////////////////////////////
	//       // RC
	// 		  ThTi = InsertTreeItem( hTI[nRootIndex],0, _LS(IDS_TREE_TEXT_RC_DESIGN), _T("ico24_Temp"));
	// 		  {
	// 			  InsertTreeItem(ThTi, ID_QUERY_DGN_GEN_MBTP, _LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_mods_design_generaldesigntablemodifymembertype"));        
	//       }
	//       */
	// 
	// 			////////////////////////////////////
	// 			//Bridge Load Rating
	// 			ThTi = InsertTreeItem( hTI[nRootIndex],cDGN_RATING_DGN, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING), _T("ico24_Temp"));
	// 		  SetBridgeLoadRatingDesignTableItemTree();
	// 
	//     }
	//   #endif
	// #endif

	SelectItem(RoothTi);
	Expand(RoothTi, TVE_TOGGLE);
}

void CTreeTableCtrl::DestroyTree()
{
}

int CTreeTableCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateTree();
	LoadMenu();
	EndInsertTreeItem();
	return 0;
}

void CTreeTableCtrl::OnDestroy()
{
	MBaseTreeCtrl::OnDestroy();
	DestroyTree();
}

void CTreeTableCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
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

void CTreeTableCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM ht;
	ht = GetSelectedItem();
	if (FALSE == ItemHasChildren(ht))
	{
		UINT Data = GetItemData(ht);
		if (Data != 0)
		{
			WPARAM wParam = MAKEWPARAM(Data, 0);
			AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, 0L);
		}
	}
}

void CTreeTableCtrl::OnReturn(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM ht;
	ht = GetSelectedItem();
	if (FALSE == ItemHasChildren(ht))
	{
		UINT Data = GetItemData(ht);
		if (Data != 0)
		{
			WPARAM wParam = MAKEWPARAM(Data, 0);
			AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, 0L);
		}
	}
	*pResult = 0;
}


void CTreeTableCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTreeTableCtrl::OnShowWindow(BOOL bShow, UINT nStatus)
{
	MBaseTreeCtrl::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if (bShow == TRUE) EnableToolTips(TRUE);
}


void CTreeTableCtrl::ToggleStyle(DWORD lStyleMask)
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
	BOOL bSetBits = !(lStyleMask&lStyleOld);
	if (bSetBits)
		lStyleOld |= lStyleMask;
	else
		lStyleOld &= ~(lStyleMask);

	SetWindowLongPtr(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

	DWORD dwStyle, dwExStyle;
	GetTreeCtrlStyles(dwStyle, dwExStyle);
	if (dwStyle & lStyleMask)
		ModifyTreeCtrlStyles(lStyleMask, 0, 0, 0);
	else
		ModifyTreeCtrlStyles(0, lStyleMask, 0, 0);
}

// Add by ZINU.('01.1.5).
void CTreeTableCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
	case D_UPDATE_DB_OPEN: //*/ZINU.
	{
		UpdateBuffer(TRUE);
		SetStoryTable(); // Add by GAY. PMS:4267. ('11.12.16). New/Open 시에 Preference>Results>Contury Code 설정에 따라 Tree Menu Setting 위함.
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
	case D_UPDATE_PREF_CHANGED:
		SetStoryTable();
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

// Add by ZINU.('01.1.5).
void CTreeTableCtrl::Update_InitDataByCode()
{
	HTREEITEM hItem;
	CString strTitle = _T("");
	BOOL bShow = FALSE;
	// General.
	// Length.
	strTitle = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);
	CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_LENG, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_QUERY_DGN_GEN_LENG);
	if (hItem)	SetItemText(hItem, strTitle);
	// K.
	strTitle = _LS(IDS_TREE_TEXT_GEN_EFFECT_LEN_FACTOR);
	CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_KFAC, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_QUERY_DGN_GEN_KFAC);
	if (hItem)	SetItemText(hItem, strTitle);
	// Cm.
	strTitle = _LS(IDS_TREE_TEXT_GEN_MOM_FACTOR);
	CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_QUERY_DGN_GEN_CMFT, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_QUERY_DGN_GEN_CMFT);
	if (hItem)	SetItemText(hItem, strTitle);
	// Steel.
	// Cb.
	strTitle = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
	CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_QUERY_DGN_STL_CBFT, strTitle, bShow);
	hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_CBFT);
	if (hItem)	SetItemText(hItem, strTitle);

	BOOL bUseColdFormed = TRUE;
	if (CTestEnvMgr::IsTestEnvST(_T("UseColdFormed"), _T("yes"))) bUseColdFormed = TRUE;

	if (bUseColdFormed)
	{
		// Cold Formed Steel.
		// Cb.
		strTitle = _LS(IDS_MAIN_RIBBON_BENDING_COEFF_CB);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_CBFT, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_QUERY_DGN_CF_CBFT);
		if (hItem)	SetItemText(hItem, strTitle);

#if defined(_MGEN_CH)
		//Phi Add by tss(2017.8.10)
		strTitle = _LS(IDS_MAIN_RIBBON_STABILITY_COEF_PAI);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_STABILITY_COEF, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_QUERY_DGN_CF_STABILITY_COEF);
		if (hItem)	SetItemText(hItem, strTitle);

		//OpenSectionCoef Add by tss(2017.8.10)
		strTitle = _LS(IDS_MAIN_RIBBON_OPENSECTION_COEF);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_OPENSECT_RESTCOEF, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_QUERY_DGN_CF_OPENSECT_RESTCOEF);
		if(hItem)	SetItemText(hItem, strTitle);

		//Add by tss 2022/10/12
		strTitle = _LS(IDS_MAIN_RIBBON_CFS_PHI);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_SPHI, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_QUERY_DGN_CF_SPHI);
		if (hItem)	SetItemText(hItem, strTitle);

		strTitle = _LS(IDS_MAIN_RIBBON_PURLIN);
		CDBDoc::GetDocPoint()->GetDesignTitleCfs(ID_QUERY_DGN_CF_PURLIN, strTitle, bShow);
		hItem = GetHTreeItemByID(ID_QUERY_DGN_CF_PURLIN);
		if(hItem)	SetItemText(hItem, strTitle);
#endif
	}
	//
	//BOOL bGB50017_17 = CDBLib::GetStlCodeName() == _T("GB50017-17");
	//hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_PHIB);
	//if(hItem)
	//	this->HideItem(hItem, bGB50017_17 == FALSE);

	//hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_PHIB);
	//UINT nID[] = { ID_QUERY_DGN_PERFOR_MIN_COEFFICEN,ID_QUERY_DGN_PERFOR_DUCTILITY_LEVEL,ID_QUERY_DGN_PERFOR_MEMBER_COEFFICIENT,
	//ID_QUERY_DGN_PERFOR_NON_PLASTIC_FORCE_COEFFICIENT,ID_QUERY_DGN_PERFOR_MEMBER_PROPERTIES };
	//for(int i = 0; i < 5; i++)
	//{
	//	hItem = GetHTreeItemByID(nID[i]);
	//	if(hItem)
	//		this->HideItem(hItem, bGB50017_17 == FALSE);
	//}
	//Sect_Classify Add by tss(2020/01/12)
	hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_SECTION_TYPE);
	//Bend Modulus Ratio Add by tss(2021/01/12)
	hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_BEND_MODULUS_RTO);
	//
	hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_SPDF);
	hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_SDSW);

#ifdef _MGEN_CH
	hItem = GetHTreeItemByID(ID_QUERY_REINFORCE_DGN_STL_INDF);
	hItem = GetHTreeItemByID(ID_QUERY_DGN_STL_APPRAISAL_IMEL);
#endif
}

// Add by ZINU.('01.1.6).
HTREEITEM CTreeTableCtrl::GetHTreeItemByID(UINT nID)
{
	TVITEM tvitem;
	tvitem.mask = TVIF_PARAM;
	tvitem.lParam = nID;
	HTREEITEM hItem = FindNextItem(&tvitem, 0);
	if (hItem == 0)	return NULL;
	else						return hItem;
}

// Add by ZINU.('01.1.6).
HTREEITEM CTreeTableCtrl::FindNextItem(TV_ITEM* pItem, HTREEITEM hItem)
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
	if ((pItem->mask & TVIF_TEXT) && pItem->pszText)
	{
		hNextItem.cchTextMax = (int)_tcslen(pItem->pszText);

		if(hNextItem.cchTextMax)
			 hNextItem.pszText = new TCHAR[++hNextItem.cchTextMax];
	}

	while (hNextItem.hItem)
	{
		if (Compare(pItem, hNextItem))
		{
			//Copy all the information into pItem and return
			memcpy(pItem, &hNextItem, sizeof(TV_ITEM));

			//Free resources
			if (hNextItem.pszText)	delete hNextItem.pszText;
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
	if (hNextItem.pszText)	delete hNextItem.pszText;

	return NULL;
}

// Add by ZINU.('01.1.6).
BOOL CTreeTableCtrl::Compare(TV_ITEM* pItem, TV_ITEM& tvTempItem)
{
	//This call uses the .mask setting to just retrieve the values
	//that the client wants to compare.
	//Get all the data passed in by pItem
	GetItem(&tvTempItem);

	//Reset the mask so I can keep track of the matching attributes
	tvTempItem.mask = 0;

	if ((pItem->mask & TVIF_STATE) && (pItem->state == tvTempItem.state))
		tvTempItem.mask |= TVIF_STATE;

	if ((pItem->mask & TVIF_IMAGE) && (pItem->iImage == tvTempItem.iImage))
		tvTempItem.mask |= TVIF_IMAGE;

	if ((pItem->mask & TVIF_PARAM) && (pItem->lParam == tvTempItem.lParam))
		tvTempItem.mask |= TVIF_PARAM;

	if ((pItem->mask & TVIF_TEXT) && pItem->pszText && tvTempItem.pszText && //Don't compare if either is NULL
		!_tcscmp(pItem->pszText, tvTempItem.pszText))
		tvTempItem.mask |= TVIF_TEXT;

	 if((pItem->mask & TVIF_TEXT) && pItem->pszText && tvTempItem.pszText && //Don't compare if either is NULL
			!_tcscmp(pItem->pszText, tvTempItem.pszText))
			tvTempItem.mask |= TVIF_TEXT;

	if ((pItem->mask & TVIF_SELECTEDIMAGE) && (pItem->iSelectedImage == tvTempItem.iSelectedImage))
		tvTempItem.mask |= TVIF_SELECTEDIMAGE;

	//If by this point these two values are the same.
	//tvTempItem.hItem is the desired item
	return (pItem->mask == tvTempItem.mask);
}

// Add by ZINU.('01.1.6).
// GetNextItem  - Get next item as if outline was completely expanded
// Returns      - The item immediately below the reference item
// hItem        - The reference item
HTREEITEM CTreeTableCtrl::GetNextItem(HTREEITEM hItem)
{
	HTREEITEM hti;

	if (ItemHasChildren(hItem))	return GetChildItem(hItem);	// return first child
	else
	{
		// return next sibling item
		// Go up the tree to find a parent's sibling if needed.
		while ((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if ((hItem = GetParentItem(hItem)) == NULL) return NULL;
		}
	}
	return hti;
}

// Add by ZINU.('04.08.20). For PSC Design.
void CTreeTableCtrl::UpdateBuffer(BOOL bAlwaysSet)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (!pDoc) return;
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;

	LockWindowUpdate();
	SetInUpdateBuffer(TRUE);

	BOOL bPscdItem = FALSE;//PSC
	BOOL bRcdItem = FALSE;
	BOOL bRacdItem = FALSE;//Rating

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while (pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch (nCmd)
		{
		case(UR_PSCD_ADD): case(UR_PSCD_DEL):
			bPscdItem = TRUE;
			break;
		case(UR_RACD_ADD): case(UR_RACD_DEL):
			bRacdItem = TRUE;
			break;
		case(UR_CHRP_ADD):case(UR_CHRP_DEL):
			bRcdItem = TRUE;
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
	if (aPscCodeName.GetSize() > 0)
	{
		if (bAlwaysSet || bPscdItem) SetPscDgnTableItemTree();
	}

	// Redraw Changed Directory - Speed Up.
	if (bAlwaysSet || bRacdItem) SetBridgeLoadRatingDesignTableItemTree();

	//RC  dedign Redraw changed Directory-speed up
	CStringArray aRcCodeName;
	CDBLib::GetCHRcCodeNameList(aRcCodeName);
	if (aRcCodeName.GetSize() > 0) {
		if (bAlwaysSet || bRcdItem) SetRcDgnTableItemTree();
	}

	UnlockWindowUpdate();
	SetInUpdateBuffer(FALSE);
	// Restore Previous Position at Tree.
	hTIKeep = GetItemAt(indx);
	if (hTIKeep)	SelectSetFirstVisible(hTIKeep);
}

// Add by ZINU.('04.08.20). For PSC Design.
void CTreeTableCtrl::SetPscDgnTableItemTree()
{
#if defined(_CIVIL)
	T_PSCD_D PscdD;
	PscdD.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc)	pDoc->m_pAttrCtrl->GetDgnPscd(PscdD);

	HTREEITEM ThTi = GetHTreeItemByID(cDGN_PSC_DGN);
	if (ThTi == 0) return; // 20080424 mylee
	HTREEITEM hChildItem = GetChildItem(ThTi);	// PSC Design Table Level.
	while (hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

    switch ( PscdD.iDgnCode )
    {
    case KSCE_USD03:
    case KSCE_USD05:
    case KSCE_USD10:
    case KSCE_RAIL_USD04:
    case KSCE_RAIL_USD11:
        SetPscDgnTableItemTreeKSCE(PscdD, ThTi);
        break;
    case KDS_24_14_21_2022_PSC:
    case KSCE_LSD15_PSC:
        SetPscDgnTableItemTreeKSCELSD(PscdD, ThTi);
        break;
    case JSCE02:
        SetPscDgnTableItemTreeJSCE(PscdD, ThTi);
        break;
    case JTG_D62_04:
    case CJJ_11_2011:
        SetPscDgnTableItemTreeJTG(PscdD, ThTi);
        break;
    case TB_10002_3_05:
        SetPscDgnTableItemTreeTB(PscdD, ThTi);
        break;
    case AASHTO_LRFD08:
    case AASHTO_LRFD12:
    case AASHTO_LRFD14:
    case AASHTO_LRFD16:
    case AASHTO_LRFD17:
    case AASHTO_LRFD20:
	case AASHTO_LRFD24:
    case CSA_S6_10:
    case CSA_S6_14:
	case CSA_S6_19:
	case AREMA_23:
        SetPscDgnTableItemTreeUS(PscdD, ThTi);
        break;
    case EUROCODE2_2_05_PSC:
        SetPscDgnTableItemTreeEURO(PscdD, ThTi);
        break;
	case ABNT_NBR_6118_2023_PSC:
		SetPscDgnTableItemTreeEURO(PscdD, ThTi);
		break;
	case AS_5100_5_17_PSC:
        SetPscDgnTableItemTreeAS(PscdD, ThTi);
        break;
    case SNiP_20503_84_PSC:
    case SP_35_13330_11_PSC:
    case SNiP_20503_84_PSC_MKS:
    case SP_35_13330_11_PSC_MKS:
        SetPscDgnTableItemTreeRUS(PscdD, ThTi);
        break;
    case IRC_112_2011_PSC:
    case IRC_112_2020_PSC:
    case IRS_PSC:
        SetPscDgnTableItemTreeIND(PscdD, ThTi);
        break;
    case BS5400_90_PSC:
	case TMH07_3_1989:
    case BD_44_15_PSC:
    case CS_455_PSC:
        SetPscDgnTableItemTreeBS(PscdD, ThTi);
        break;
    default:
        ASSERT(0);
        break;
    }
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_FORCE, _LS(IDS_TREE_TEXT_PSC_DGN_COMF), _T("ico24_psc_pscresults_designforces"));
#endif
}

void CTreeTableCtrl::SetPscDgnTableItemTreeKSCE(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	// Add by ZINU.('04.08.24). Check Option. Agree with Kouno.
	BOOL bNoPC = (PscdD.iBrgType == 0 ? FALSE : TRUE);	// 교종(0=PC, 1=PRC).
	BOOL bNo2D = (PscdD.iDimType == 0 ? FALSE : TRUE);	// 설계차원 정보(0=2D, 1=2D+Torsion, 2=3D).
	BOOL bOk3D = (PscdD.iDimType == 2 ? TRUE : FALSE);	// 설계차원 정보(0=2D, 1=2D+Torsion, 2=3D).
	BOOL bNoAll = (bNoPC && bNo2D ? TRUE : FALSE);
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_KSCE_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_KSCE_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_KSCE_FYCM), _T("ico24_psc_pscresults_resulttables"));
	if (PscdD.iDgnCode == KSCE_USD03 || PscdD.iDgnCode == KSCE_USD05 || PscdD.iDgnCode == KSCE_RAIL_USD04)
	{
		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_KSCE_TBAR), _T("ico24_psc_pscresults_resulttables"));
	}
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_KSCE_FSSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, _LS(IDS_TREE_TEXT_KSCE_FSMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_KSCE_BDCY), _T("ico24_psc_pscresults_resulttables"));
	if (bOk3D)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, _LS(IDS_TREE_TEXT_KSCE_BDCZ), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_KSCE_CUMS), _T("ico24_psc_pscresults_resulttables"));
	if (bNo2D)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_KSCE_CRMT), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeJSCE(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	// Add by ZINU.('04.08.24). Check Option. Agree with Kouno.
	BOOL bNoPC = (PscdD.iBrgType == 0 ? FALSE : TRUE);	// 교종(0=PC, 1=PRC).
	BOOL bNo2D = (PscdD.iDimType == 0 ? FALSE : TRUE);	// 설계차원 정보(0=2D, 1=2D+Torsion, 2=3D).
	BOOL bOk3D = (PscdD.iDimType == 2 ? TRUE : FALSE);	// 설계차원 정보(0=2D, 1=2D+Torsion, 2=3D).
	BOOL bNoAll = (bNoPC && bNo2D ? TRUE : FALSE);
	BOOL bCmpW = PscdD.bCmpWeb;
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_PSC_DGN_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_PSC_DGN_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_PSC_DGN_FYCM), _T("ico24_psc_pscresults_resulttables"));
	if (bNoPC)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_PSC_DGN_BDCW), _T("ico24_psc_pscresults_resulttables"));
	if (!bNoPC)	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_PSC_DGN_TBAR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, _LS(IDS_TREE_TEXT_PSC_DGN_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_PSC_DGN_FSSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, _LS(IDS_TREE_TEXT_PSC_DGN_FSMS), _T("ico24_psc_pscresults_resulttables"));
	if (bNoPC)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_PSC_DGN_CWMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_PSC_DGN_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES11, _LS(IDS_TREE_TEXT_PSC_DGN_CRMS), _T("ico24_psc_pscresults_resulttables"));
	if (bNo2D)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES12, _LS(IDS_TREE_TEXT_PSC_DGN_FSMT), _T("ico24_psc_pscresults_resulttables"));
	if (bNoAll)	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES13, _LS(IDS_TREE_TEXT_PSC_DGN_CWMT), _T("ico24_psc_pscresults_resulttables"));
	if (bNo2D)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES14, _LS(IDS_TREE_TEXT_PSC_DGN_CUMT), _T("ico24_psc_pscresults_resulttables"));
	if (bNo2D)		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES15, _LS(IDS_TREE_TEXT_PSC_DGN_CRMT), _T("ico24_psc_pscresults_resulttables"));


	if (bCmpW)
	{
		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES16, _LS(IDS_TREE_TEXT_PSC_DGN_FASS), _T("ico24_psc_pscresults_resulttables"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES17, _LS(IDS_TREE_TEXT_PSC_DGN_FMSS), _T("ico24_psc_pscresults_resulttables"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES18, _LS(IDS_TREE_TEXT_PSC_DGN_FPRT), _T("ico24_psc_pscresults_resulttables"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES19, _LS(IDS_TREE_TEXT_PSC_DGN_FWHL), _T("ico24_psc_pscresults_resulttables"));
	}
}

void CTreeTableCtrl::SetPscDgnTableItemTreeJTG(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_PSC_MOD_PSCW), _T("ico24_psc_pscdesigndata_psccrackwidthcoefficient"));
	// Add by ZINU.('04.11.26). Check Option. Agree with Kye BJ.
	BOOL bOkPaAB = (PscdD.iBrgType == 0 ? FALSE : TRUE);	// 교종(0=Fully, 1=Partially(A), 2=Partially(B)).
	BOOL bOkParB = (PscdD.iBrgType == 2 ? TRUE : FALSE);	// 교종(0=Fully, 1=Partially(A), 2=Partially(B)).
	BOOL bNo2D = (PscdD.iDimType == 0 ? FALSE : TRUE);	// 설계차원 정보(0=2D, 1=2D+Torsion, 2=3D).
	BOOL bOkParT = (bOkPaAB && bNo2D ? TRUE : FALSE);

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_JTG_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_JTG_FYPC), _T("ico24_psc_pscresults_resulttables"));
	if (!bOkParB)InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_JTG_FYCM_MIN), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_JTG_FPMS_MIN), _T("ico24_psc_pscresults_resulttables"));
	if (bOkParB)	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_JTG_BDCW), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, _LS(IDS_TREE_TEXT_JTG_FYCM_MAX), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_JTG_FPMS_MAX), _T("ico24_psc_pscresults_resulttables"));
	if (bOkPaAB)	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, _LS(IDS_TREE_TEXT_JTG_TBAR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_JTG_TNDN), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_JTG_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES11, _LS(IDS_TREE_TEXT_JTG_CUMS), _T("ico24_psc_pscresults_resulttables"));
	if (bNo2D)	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES12, _LS(IDS_TREE_TEXT_JTG_CRMT), _T("ico24_psc_pscresults_resulttables"));

}

void CTreeTableCtrl::SetPscDgnTableItemTreeTB(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_PSC_MOD_PSCW), _T("ico24_psc_pscdesigndata_psccrackwidthcoefficient"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_TB_BEAM_PSC_DEGREE), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_TB_BEAM_BEND), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_TB_BEAM_SHEAR_S), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_TB_BEAM_CROSS_FRACTURE), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_TB_BEAM_INCLINE_FRACTURE), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_TB_BEAM_CON_CSS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_TB_BEAM_TENDON_T), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_TB_BEAM_DELTA_TENDON), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES11, _LS(IDS_TREE_TEXT_TB_BEAM_SHEAR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES12, _LS(IDS_TREE_TEXT_TB_BEAM_CRACK), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES13, _LS(IDS_TREE_TEXT_TB_BEAM_ANCHOR_TS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES14, _LS(IDS_TREE_TEXT_TB_BEAM_ANCHOR_CS), _T("ico24_psc_pscresults_resulttables"));

	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_TB_COLUMN_PSC_DEGREE), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_TB_COLUMN_CS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_TB_COLUMN_TS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_TB_COLUMN_CROSS_FRACTURE), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_TB_COLUMN_CON_CSS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_TB_COLUMN_TENDON_SS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES08, _LS(IDS_TREE_TEXT_TB_COLUMN_SHEAR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_TB_COLUMN_TENSION), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_COLUMN_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_TB_COLUMN_COMPRESSIVE), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeUS(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_AASHTO_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_AASHTO_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_AASHTO_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_AASHTO_FSSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_AASHTO_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, _LS(IDS_TREE_TEXT_AASHTO_FPMT), _T("ico24_psc_pscresults_resulttables"));

    if ( PscdD.iDgnCode==AASHTO_LRFD08 )
    {
        // Add, Nishant. 
        int iBrgType = PscdD.iBrgType; // 0=Fully PSC, 1=Partially PSC
        if ( iBrgType==1 )
        {
            InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_AASHTO_BDCW), _T("ico24_psc_pscresults_resulttables"));
        }
    }
	else if ( PscdD.iDgnCode==CSA_S6_19 )
	{
		InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_CSA_BDCW), _T("ico24_psc_pscresults_resulttables"));
	}
    else
    {
        InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_AASHTO_BDCW), _T("ico24_psc_pscresults_resulttables"));
    }

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_AASHTO_BDCY), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_AASHTO_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES11, _LS(IDS_TREE_TEXT_AASHTO_CRMT), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeIND(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCW), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeABNT(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_ENVIRONMENTAL_AGGRESSIVENESS_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCW), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeEURO(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCW), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeRUS(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, _LS(IDS_TREE_TEXT_SNiP_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, _LS(IDS_TREE_TEXT_SNiP_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, _LS(IDS_TREE_TEXT_SNiP_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, _LS(IDS_TREE_TEXT_SNiP_FATI), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, _LS(IDS_TREE_TEXT_SNiP_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, _LS(IDS_TREE_TEXT_SNiP_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, _LS(IDS_TREE_TEXT_SNiP_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, _LS(IDS_TREE_TEXT_SNiP_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, _LS(IDS_TREE_TEXT_SNiP_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, _LS(IDS_TREE_TEXT_SNiP_BDCW), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeAS(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_TRAN), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_TRCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES10, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES11, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCW), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeBS(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES21, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_SLJT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYPC), _T("ico24_psc_pscresults_resulttables"));
}

void CTreeTableCtrl::SetPscDgnTableItemTreeKSCELSD(T_PSCD_D PscdD, HTREEITEM ThTi)
{
	InsertTreeItem(ThTi, ID_QUERY_PSC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSC), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_PRINT_OPTION, _LS(IDS_TREE_TEXT_PSC_MOD_POSX), _T("ico24_psc_pscdesigndata_designoutputposition"));
	InsertTreeItem(ThTi, ID_QUERY_PSC_EXPOSURE_CLASS, _LS(IDS_TREE_TEXT_PSC_EXPOSURE_CLASS), _T("ico24_psc_pscdesigndata_exposureclass"));

	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES01, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCR), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES02, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CUMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES03, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_CRMT), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES04, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES05, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYPC), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES06, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FYCM), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES07, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPSG), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES08, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_FPMS), _T("ico24_psc_pscresults_resulttables"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_PSCD_RES09, GetPSCResultTitle(PscdD.iDgnCode, PSC_RES_BDCW), _T("ico24_psc_pscresults_resulttables"));
}

//Add by maxiao(2007-12-07) For CIVIL CHina rc design
void CTreeTableCtrl::SetRcDgnTableItemTree() {
#if defined(_CIVIL)
#if defined _CH// || 
	T_CHRP_D ChrcD;   ChrcD.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc)    pDoc->m_pAttrCtrl->GetChrp(ChrcD);
	HTREEITEM ThTi = GetHTreeItemByID(cDGN_RC_DGN);
	if (ThTi == 0) return; // 20080424 mylee
	HTREEITEM hChildItem = GetChildItem(ThTi);	// RC Design Table Level.
	while (hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}


	if (ChrcD.iDgnCode == RC_JTG_D62_04 || ChrcD.iDgnCode == RC_CJJ_11_2011) {//RC high way
		InsertTreeItem(ThTi, ID_QUERY_RC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_OPTION), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RC_PRINT_OPTION, _LS(IDS_TREE_TEXT_RC_CH_PRINT_OPTION), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_RC_CH_CRACK_WIDTH), _T("ico24_Temp"));

		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES01, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_STRESS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES02, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_TENSILE), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES03, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_COMP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES04, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_CRACK), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES05, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_REQUIRED_LONGI), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES06, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_REQUIRED_COMM), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES07, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_BENDING), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES08, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_SHEAR), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES09, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_BEAM_CHECK_TORSION), _T("ico24_Temp"));
		//Column
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES01, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_CHECK_CRACK), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES02, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_REQUIRED_LONGI), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES03, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_AXES_COMP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES04, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_COLUMN_AXES_TENSILE), _T("ico24_Temp"));
		//force
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RCBEAM_FORCE, _LS(IDS_TREE_TEXT_RC_CH_DGN_BEAM_FORCE), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RCCOLUMN_FORCE, _LS(IDS_TREE_TEXT_RC_CH_DGN_COLUMN_FORCE), _T("ico24_Temp"));
	}
	else if (ChrcD.iDgnCode == RC_TB_10002_3_05) {//RC railway
		InsertTreeItem(ThTi, ID_QUERY_RC_DESIGN_OPTION, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_OPTION), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RC_PRINT_OPTION, _LS(IDS_TREE_TEXT_RC_CH_PRINT_OPTION), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RC_CRACK_WIDTH, _LS(IDS_TREE_TEXT_RC_CH_CRACK_WIDTH), _T("ico24_Temp"));
		//beam
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES01, _LS(IDS_TREE_TEXT_RAIL_BEAM_CCS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES02, _LS(IDS_TREE_TEXT_RAIL_BEAM_ST), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES03, _LS(IDS_TREE_TEXT_RAIL_BEAM_RT), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES04, _LS(IDS_TREE_TEXT_RAIL_BEAM_CD), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES05, _LS(IDS_TREE_TEXT_RAIL_BEAM_TSS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_BEAM_RES06, _LS(IDS_TREE_TEXT_RAIL_BEAM_BSS), _T("ico24_Temp"));
		//Column
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES01, _LS(IDS_TREE_TEXT_RAIL_COLUMN_CCS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES02, _LS(IDS_TREE_TEXT_RAIL_COLUMN_CTS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES03, _LS(IDS_TREE_TEXT_RAIL_COLUMN_RCS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES04, _LS(IDS_TREE_TEXT_RAIL_COLUMN_RTS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES05, _LS(IDS_TREE_TEXT_RAIL_COLUMN_CD), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_COLUMN_RES06, _LS(IDS_TREE_TEXT_RAIL_COLUMN_S), _T("ico24_Temp"));
		//Seismic
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES01, _LS(IDS_TREE_TEXT_RAIL_SEISMIC_CON), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES02, _LS(IDS_TREE_TEXT_RAIL_SEISMIC_RC), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES03, _LS(IDS_TREE_TEXT_RAIL_SEISMIC_RS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES04, _LS(IDS_TREE_TEXT_RAIL_SEISMIC_E), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES05, _LS(IDS_TREE_TEXT_RAIL_SEISMIC_STAB), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES06, _LS(IDS_TREE_TEXT_RAIL_SEISMIC_DUCT), _T("ico24_Temp"));
		//force
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RCBEAM_FORCE, _LS(IDS_TREE_TEXT_RC_CH_DGN_BEAM_FORCE), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RCCOLUMN_FORCE, _LS(IDS_TREE_TEXT_RC_CH_DGN_COLUMN_FORCE), _T("ico24_Temp"));
	}
	else if (ChrcD.iDgnCode == RC_JTG_B02_01_2008 || ChrcD.iDgnCode == RC_CJJ_166_2011) {//RC Seismic
		InsertTreeItem(ThTi, ID_QUERY_RC_DESIGN_SEISMIC, _LS(IDS_TREE_TEXT_RC_CH_DESIGN_SEISMIC), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES01, _LS(IDS_TREE_TEXT_JTG_SEISMIC_PIERCOMP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES02, _LS(IDS_TREE_TEXT_JTG_SEISMIC_ABUTMENTCOMP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES03, _LS(IDS_TREE_TEXT_JTG_SEISMIC_ARCHCROSS), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES04, _LS(IDS_TREE_TEXT_JTG_SEISMIC_ARCHCOMP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES05, _LS(IDS_TREE_TEXT_JTG_SEISMIC_BENTCAPBEND), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES06, _LS(IDS_TREE_TEXT_JTG_SEISMIC_BENTCAPSHEAR), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES07, _LS(IDS_TREE_TEXT_JTG_SEISMIC_FOUNDATION), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES08, _LS(IDS_TREE_TEXT_JTG_SEISMIC_RUBBERTHICK), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES09, _LS(IDS_TREE_TEXT_JTG_SEISMIC_RUBBERSLIP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES10, _LS(IDS_TREE_TEXT_JTG_SEISMIC_HORIDISP), _T("ico24_Temp"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES11, _LS(IDS_TREE_TEXT_JTG_SEISMIC_HORIFORCE), _T("ico24_Temp"));
		if (ChrcD.iEarthquakeType != 0) {
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES12, _LS(IDS_TREE_TEXT_JTG_SEISMIC_PIERSHEAR), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES13, _LS(IDS_TREE_TEXT_JTG_SEISMIC_TOPDISP), _T("ico24_Temp"));
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RC_SEISMIC_RES14, _LS(IDS_TREE_TEXT_JTG_SEISMIC_CORNERDISP), _T("ico24_Temp"));
		}
	}
#endif
#endif	
}

// Add by sshan.('06.06.02). For Bridge Load Rating Design
void CTreeTableCtrl::SetBridgeLoadRatingDesignTableItemTree()
{
#if defined(_CIVIL)
#if defined(_US) || defined(_RUS) || defined(_ORG)

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_RACD_D RacdD;
	RacdD.Initialize();
	if (pDoc)
	{
		if (pDoc->m_pAttrCtrl2->ExistRacd())
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

	HTREEITEM ThTi = GetHTreeItemByID(cDGN_RATING_DGN);
	if (ThTi == 0) return; // 20080424 mylee
	HTREEITEM hChildItem = GetChildItem(ThTi);	// Rating Design Table Level.
	while (hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

	if (RacdD.nRatingCode == KSCE_USD05_RATING)
	{
		//(2009.12.16) Add Table..Item (Design/Print Option.)
		InsertTreeItem(ThTi, ID_QUERY_RATING_DESIGN_OPTION, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_OPTION), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineratingoptiontable"));
		InsertTreeItem(ThTi, ID_QUERY_RATING_PRINT_OPTION, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PRINT), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineprintoptiontable"));
		InsertTreeItem(ThTi, ID_QUERY_STL_RATING_PRINT_OPTION, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PRINT), _T("ico24_rating_bridgeratingdesign_dropmenu1_defineprintoptiontable"));


		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES01, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_ALLOWFORCE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES02, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FORCEDGN), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
	}
	else if (RacdD.nRatingCode == AASHTO_LRFD05_PSC_RATING || RacdD.nRatingCode == AASHTO_LRFD12_PSC_RATING || RacdD.nRatingCode == AASHTO_LRFD19_PSC_RATING)
	{
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES11, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SUMMARY_SERVICE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES12, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SUMMARY_STRENGTH), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		if (RacdD.nRatingCode == AASHTO_LRFD19_PSC_RATING)
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES01, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STRESS), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		else
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES01, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_CONCRETE), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES02, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STEEL_TENSION), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES03, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FLEXURE_STR), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES04, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SHEAR_STR), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		if (RacdD.nRatingCode == AASHTO_LRFD19_PSC_RATING)
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES05, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STRESS_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		else
			InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES05, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_CONCRETE_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES06, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_STEEL_TENSION_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES07, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_FLEXURE_STR_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));
		InsertTreeItem(ThTi, ID_QUERY_RESULT_RATING_RES08, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_TABLE_SHEAR_STR_DATA), _T("ico24_rating_bridgeratingdesign_dropmenu3_ratingdesignresulttables2"));

		InsertTreeItem(ThTi, ID_QUERY_RATING_DESIGN_OPTION, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_OPTION), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineratingoptiontable"));
		InsertTreeItem(ThTi, ID_QUERY_RATING_PRINT_OPTION, _LS(IDS_TREE_TEXT_DGN_BRIDGE_LOAD_RATING_PRINT), _T("ico24_rating_bridgeratingdesign_dropmenu3_defineprintoptiontable"));
	}
#endif
#endif
}



void CTreeTableCtrl::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if (GetSafeHwnd())
	{
		//SetBkColor( m_BkColor );
		//SetSelIconBkColor( m_BkColor );
		//SetIconBkColor( m_BkColor );
		Invalidate();
	}
}

//!@#$ MNET:1489 20050823 LBI
void CTreeTableCtrl::SetStoryTable()
{
#ifdef _MGEN
	T_PREFERENCE pref;  pref.Initialize();
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc)
		pDoc->m_pInitCtrl->GetPreference(pref);
	BOOL bCNCode = pref.Results.nCountryCode == 1 ? TRUE : FALSE;
	BOOL bJPCode = pref.Results.nCountryCode == 2 ? TRUE : FALSE; //!@#$ MNET:1724 20050916 LBI

	HTREEITEM ThTi = GetHTreeItemByID(cSTORY_TABLE);
	HTREEITEM hChildItem = GetChildItem(ThTi);	// PSC Design Table Level.
	while (hChildItem)
	{
		HTREEITEM hNextItem = GetNextSiblingItem(hChildItem);
		DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

	int nMovingType = CProduct::GetMovingType();
	BOOL bTTW = (nMovingType == D_PRODUCT_MOVING_US && CProduct::IsTaiwan());
	if (nMovingType == D_PRODUCT_MOVING_ALL) bTTW = TRUE;

	BOOL bUS = (nMovingType == D_PRODUCT_MOVING_US);

	//InsertTreeItem(ThTi,ID_QUERY_RESULT_STOR_MODULE,_LS(IDS_TMTB_Story_Module),_T("ico24_Temp"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_DRIFT, _LS(IDS_TMTB_Story_Drift), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_DRIFT4TH, _LS(IDS_TMTB_Story_Drift4TH), _T("ico24_results_tables_timehistoryanalysis")); // MNET:2382-GSJI-20060816  
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_DISPL, _LS(IDS_TMTB_Story_Displacement), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_SHEAR, _LS(IDS_TMTB_Story_Shear_Response_Spectrum_Analysis_), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_SHEAR_TH, _LS(IDS_TMTB_Story_Shear_Time_History_Analysis_), _T("ico24_results_tables_timehistoryanalysis"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_MODE, _LS(IDS_TMTB_Story_Mode_Shape), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_ECCENT, _LS(IDS_TMTB_Story_Eccentricity__), _T("ico24_results_tables_story"));
	if (bJPCode) InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_STIFF_RATIO, _LS(IDS_TMTB_Story_Stiffness_Ratio__), _T("ico24_Temp"));
	//InsertTreeItem(ThTi,ID_QUERY_RESULT_STOR_STIFF,_LS(IDS_TMTB_Story_Stiffness_Ratio__),_T("ico24_Temp")); // 기능 삭제됨 05.05.16
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_SSRT, _LS(IDS_TM_Story_Shear_Force_Ratio__), _T("ico24_results_tables_story"));
	if (!bCNCode) InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_TAF, _LS(IDS_TM_STOR_TAF), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_OTM, _LS(IDS_TM_STOR_OTM), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_VFS, _LS(IDS_TM_STOR_VFS), _T("ico24_results_tables_story"));
	if (!bCNCode) InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_STC, _LS(IDS_TM_STOR_STC), _T("ico24_results_tables_story"));
	// Modify by GAY. PMS:4267. ('12.05.18). 층질량비 Table 우선 제외 : 층질량 Table이 중국에서 요청한 형태로 수정이 되어야 의미가 있음.
	//if(bCNCode)  InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_MASS_RATIO, _LS(IDS_TM_STOR_MASS_RATIO), _T("ico24_results_tables_story")); // Add by GAY. PMS:4267. ('11.12.09).
	if (bUS)     InsertTreeItem(ThTi, ID_QUERY_RESULT_IRR_CHECK_PARAM, _LS(IDS_TM_IRR_CHECK_PARAM), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_TIC, _LS(IDS_TM_STOR_TIC), _T("ico24_results_tables_story"));
	if (pDoc && pDoc->IsCodeIS1893_2025())
	{
		InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_TFC, _LS(IDS_TM_STOR_TFC), _T("ico24_results_tables_story"));
	}
	if (bUS) InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_CRP, _LS(IDS_TB_RESULTVIEW_CRP), _T("ico24_results_tables_story"));
	if (!bJPCode) InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_SIC, _LS(IDS_TM_STOR_SIC), _T("ico24_results_tables_story"));
	if (!bCNCode) InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_WIC, _LS(IDS_TM_STOR_WIC), _T("ico24_results_tables_story"));
	InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_CIC, _LS(IDS_TM_STOR_CIC), _T("ico24_results_tables_story"));
	if (bCNCode)//add by maxiao for 0.2Q0 2012-2-15
	{
		T_DCON_D DconD; DconD.Initialize();
		if (pDoc)    pDoc->m_pAttrCtrl->GetDcon(DconD);
		if(DconD.DesignCode == _T("GB/T50010-10"))
			InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_02Q0, _LS(IDS_TM_STOR_02Q0), _T("ico24_Temp"));
	}
	// MNET:2669-GSJI-20070305
#ifdef _DEBUG
	bTTW = TRUE;
#endif

	if (bTTW)  InsertTreeItem(ThTi, ID_QUERY_RESULT_STOR_USC, _LS(IDS_TM_STOR_USC), _T("ico24_results_tables_story"));

#endif
}

COLORREF CTreeTableCtrl::GetBkColor()
{
	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//COLORREF Color;
	//if (pDoc != 0 && pDoc->GetDispOpt() != 0)
	//{
	//	if (m_strBkColorName.IsEmpty())
	//	{
	//		COptColorObjNames CC;
	//		m_strBkColorName = CC.m_TreeMenu_Background;
	//	}
	//	CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(m_strBkColorName);
	//	Color = pColor->GetCurVal(0);
	//}
	//else
	//{
	//	CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
	//	CString csSection, csEntry, csValue;
	//	csSection = "DispOpt-Color";
	//	csEntry = "Treemenu	Background:Value";
	//	csValue = pApp->GetProfileString(csSection, csEntry, NULL);
	//	Color = RGB(128, 128, 128);
	//	if (!csValue.IsEmpty()) Color = (COLORREF)atoi(csValue);
	//}
	return RGB(233, 235, 239);
}

CString CTreeTableCtrl::GetPSCResultTitle(int iDgnCode, int iPSCResType)
{
	CString strTitle = _T("");

	if (iDgnCode == EUROCODE2_2_05_PSC)
	{
		switch (iPSCResType)
		{
		case PSC_RES_FYSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYSG); break;
		case PSC_RES_FYPC: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYPC); break;
		case PSC_RES_FYCM: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYCM); break;
		case PSC_RES_FPSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPSG); break;
		case PSC_RES_FPMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPMS); break;
		case PSC_RES_BDCW: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCW); break;
		case PSC_RES_TBAR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_TBAR); break;
		case PSC_RES_BDCR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCR); break;
		case PSC_RES_CUMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CUMS); break;
		case PSC_RES_CRMT: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CRMT); break;
			//case PSC_RES_FATG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FATG); break;
		default:
			ASSERT(0);
		}
	}
	else if (iDgnCode == ABNT_NBR_6118_2023_PSC)
	{
		switch (iPSCResType)
		{
		case PSC_RES_FYSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYSG); break;
		case PSC_RES_FYPC: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYPC); break;
		case PSC_RES_FYCM: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYCM); break;
		case PSC_RES_FPSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPSG); break;
		case PSC_RES_FPMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPMS); break;
		case PSC_RES_BDCW: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCW); break;
		case PSC_RES_TBAR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_TBAR); break;
		case PSC_RES_BDCR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCR); break;
		case PSC_RES_CUMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CUMS); break;
		case PSC_RES_CRMT: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CRMT); break;
			//case PSC_RES_FATG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FATG); break;
		default:
			ASSERT(0);
		}
	}
	else if (iDgnCode == AS_5100_5_17_PSC)
	{
		switch (iPSCResType)
		{
		case PSC_RES_TRAN: strTitle = _LS(IDS_TREE_TEXT_PSC_AS_TRAN); break;
		case PSC_RES_TRCR: strTitle = _LS(IDS_TREE_TEXT_PSC_AS_TRCR); break;
		case PSC_RES_FYSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYSG); break;
		case PSC_RES_FYPC: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYPC); break;
		case PSC_RES_FYCM: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYCM); break;
		case PSC_RES_FPSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPSG); break;
		case PSC_RES_FPMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPMS); break;
		case PSC_RES_BDCW: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCW); break;
		case PSC_RES_TBAR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_TBAR); break;
		case PSC_RES_BDCR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCR); break;
		case PSC_RES_CUMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CUMS); break;
		case PSC_RES_CRMT: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CRMT); break;
			//case PSC_RES_FATG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FATG); break;
		default:
			ASSERT(0);
		}
	}
	else if (iDgnCode == KSCE_LSD15_PSC || iDgnCode == KDS_24_14_21_2022_PSC)
	{
		switch (iPSCResType)
		{
		case PSC_RES_FYSG: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_FYSG); break;
		case PSC_RES_FYPC: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_FYPC); break;
		case PSC_RES_FYCM: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_FYCM); break;
		case PSC_RES_FPSG: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_FPSG); break;
		case PSC_RES_FPMS: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_FPMS); break;
		case PSC_RES_BDCW: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_BDCW); break;
		case PSC_RES_TBAR: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_TBAR); break;
		case PSC_RES_BDCR: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_BDCR); break;
		case PSC_RES_CUMS: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_CUMS); break;
		case PSC_RES_CRMT: strTitle = _LS(IDS_TREE_TEXT_PSC_KSCE_LSD_CRMT); break;
		default:
			ASSERT(0);
		}
	}
	else if(iDgnCode==BS5400_90_PSC|| iDgnCode == TMH07_3_1989 || iDgnCode==BD_44_15_PSC || iDgnCode==CS_455_PSC )
	{
		switch (iPSCResType)
		{
			// BS에서는 Crack검토 대신 Stress검토를 수행하여 판단함.
		case PSC_RES_FYSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYSG); break;
		case PSC_RES_FYPC: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYPC); break;
		case PSC_RES_FYCM: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYCM); break;
		case PSC_RES_FPSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPSG); break;
		case PSC_RES_FPMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPMS); break;
			//case PSC_RES_BDCW: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCW); break;
		case PSC_RES_TBAR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_TBAR); break;
		case PSC_RES_BDCR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCR); break;
		case PSC_RES_CUMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CUMS); break;
		case PSC_RES_CRMT: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CRMT); break;
		case PSC_RES_SLJT: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_SLJT); break;
		default:
			ASSERT(0);
		}
	}
	else if (iDgnCode == IRC_112_2011_PSC || iDgnCode == IRS_PSC || iDgnCode == IRC_112_2020_PSC)
	{
		switch (iPSCResType)
		{
		case PSC_RES_FYSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYSG); break;
		case PSC_RES_FYPC: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYPC); break;
		case PSC_RES_FYCM: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FYCM); break;
		case PSC_RES_FPSG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPSG); break;
		case PSC_RES_FPMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FPMS); break;
		case PSC_RES_BDCW: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCW); break;
		case PSC_RES_TBAR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_TBAR); break;
		case PSC_RES_BDCR: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_BDCR); break;
		case PSC_RES_CUMS: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CUMS); break;
		case PSC_RES_CRMT: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_CRMT); break;
			//case PSC_RES_FATG: strTitle = _LS(IDS_TREE_TEXT_PSC_EURO_FATG); break;
		default:
			ASSERT(0);
		}
	}
	else ASSERT(0);

	return strTitle;
}