#include "stdafx.h"
#include "TreeJUDCtrl.h"
#include "TreeMenuCtrl.h"

#include "..\wg_main\wg_mainres2.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_dbLock\LockMgr.h"

#include "..\wg_db\DispOptMgr.h"
#include "..\wg_db\DispOptColorObj.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"

#include "..\wg_jud\JUDExportFunc.h"
#include "..\wg_jud\JUDDgnAnalMainDlg.h"
#include "..\wg_jud\JUDStrtGroupDlg.h"
#include "..\wg_jud\JUDJointDlg.h"
#include "..\wg_jud\JUDDiapTypeDlg.h"
#include "..\wg_jud\JUDBasePlateTypeDlg.h"
#include "..\wg_jud\JUDLateralLoadGraphDlg.h"
#include "..\wg_jud\JUDBlcfDlg.h"
#include "..\wg_jud\JUDRankDlg.h"

#include "..\wg_jfd\JFDExportFunc.h"
#include "..\wg_jfd\JFD_FoundationInfoDlg.h"

#include "..\wg_base\ServiceProvider.h"
#include "..\wg_jfd\Jfd_ServiceDef.h"

using namespace mit::frx;
IMPLEMENT_DYNAMIC(CTreeJUDCtrl, MBaseTreeCtrl);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* 아이콘 갯수 변경시 수정해주세요 !!! pyj0827 */
const int IMAGE1_SIZE = 64;   // IMAGE1_SIZE개
const int IMAGE2_SIZE = IMAGE1_SIZE + 64;   // IMAGE1_SIZE개
const int IMAGE3_SIZE = IMAGE2_SIZE + 64;   // IMAGE1_SIZE개
const int IMAGE4_SIZE = IMAGE3_SIZE + 64;   // IMAGE1_SIZE개
const int IMAGE5_SIZE = IMAGE4_SIZE + 64;   // IMAGE1_SIZE개
const int IMAGE6_SIZE = IMAGE5_SIZE + 83;   // 83개
const int IMAGE7_SIZE = IMAGE6_SIZE + 64;   // IMAGE1_SIZE개
const int IMAGE8_SIZE = IMAGE7_SIZE + 64;   // IMAGE1_SIZE개
/////////////////////////////////////////////////////////////////////////////
// CTreeJUDCtrl

CTreeJUDCtrl::CTreeJUDCtrl()
{
	m_BkColor = RGB( 192, 220, 192);
}

CTreeJUDCtrl::~CTreeJUDCtrl()
{
}

#define CTreeCtrl MBaseTreeCtrl
BEGIN_MESSAGE_MAP(CTreeJUDCtrl, CTreeCtrl)
#undef CTreeCtrl
	//{{AFX_MSG_MAP(CTreeJUDCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeJUDCtrl message handlers

void CTreeJUDCtrl::CreateTree()
{
	return;
}

void CTreeJUDCtrl::LoadMenu()
{	
	HTREEITEM	hTI[20];
	HTREEITEM RoothTi,ThTi,SeTi;
	int nRootIndex=0;
	CString strText;

	RoothTi = InsertTreeItem(_LS(IDS_TREE_JUD), _T("ico24_Temp"));

	//기본정보
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Configuration), _T("ico24_Temp"));
	{
		//프로젝트 정보 
		InsertTreeItem( hTI[nRootIndex],ID_FILE_PJINFO,_LS(IDS_TM_Project_Information), _T("ico24_Temp"));
		//구조형식
		InsertTreeItem( hTI[nRootIndex],ID_STRUCT_CONFIG,_LS(IDS_TM_Structure_Type), _T("ico24_Temp"));
	}   	  
	
	nRootIndex++;
	//재질
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Material), _T("ico24_Temp"));
	{
		CString strText;
		strText.Format(_T("%s/%s/%s"),_LS(IDS_TREE_JUD_MATL_CONC),_LS(IDS_TREE_JUD_MATL_STEEL),_LS(IDS_TREE_JUD_MATL_SRC));
		//Concrete/Steel/SRC
		InsertTreeItem( hTI[nRootIndex],ID_STRUCT_PROP_MATERIAL, strText, _T("ico24_Temp"));
		//철근         
		InsertTreeItem( hTI[nRootIndex],ID_DGN_CON_MATD,_LS(IDS_TREE_JUD_MATL_REBAR), _T("ico24_Temp"));
	} 

	nRootIndex++;
	//단면/두께
	strText.Format(_T("%s/%s"), _LS(IDS_TM_Section), _LS(IDS_TM_Thickness));
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,strText, _T("ico24_Temp"));
	{
		strText.Format(_T("%s/%s/%s"), _LS(IDS_TM_Beam), _LS(IDS_TM_Column), _LS(IDS_TM_BRACE));
		// 보/기둥/브레이스
		InsertTreeItem( hTI[nRootIndex],ID_STRUCT_PROP_SECTION,strText, _T("ico24_Temp"));
		// 벽 
		InsertTreeItem( hTI[nRootIndex],ID_STRUCT_PROP_THICKNESS,_LS(IDS_TMTB_Wall), _T("ico24_Temp"));
		// 개구부 - 2차반영
		//InsertTreeItem( hTI[nRootIndex],0,_LS(IDS_TREE_JUD_SECT_OPEN ),_T("ico24_Temp"));  
	}

	nRootIndex++;
	// Geometry
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Geometry), _T("ico24_Temp"));
	{
		// Import DXF File
		InsertTreeItem( hTI[nRootIndex],ID_FILE_IMPORT_DXF,_LS(IDS_TREE_JUD_IMPORT_DXF), _T("ico24_Temp"));
		// Node
		ThTi = InsertTreeItem( hTI[nRootIndex],0,_LS(IDS_TM_Nodes), _T("ico24_Temp"));
		{
			InsertTreeItem( ThTi,ID_STRUCT_NODE_CREATE,     _LS(IDS_TM_Create)         , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_DELETE,     _LS(IDS_TM_Delete)         , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_TRANSLATE,  _LS(IDS_TM_Translate)      , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_ROTATE,     _LS(IDS_TM_Rotate)         , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_PROJECT,    _LS(IDS_TM_Project)        , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_MIRROR,     _LS(IDS_TM_Mirror)         , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_DIVIDE,     _LS(IDS_TM_Divide)         , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_MERGE,      _LS(IDS_TM_Merge)          , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_SCALE,      _LS(IDS_TM_Scale)          , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_COMPACT,    _LS(IDS_TM_Compact_Numbers), _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_RENUMBER,   _LS(IDS_TM_Renumbering)    , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_NODE_STARTNUMBER,_LS(IDS_TM_Start_Number)   , _T("ico24_Temp"));
		}
		// Element
		ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TM_Elements),_T("ico24_temp"));	
		{
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_CREATE,     _LS(IDS_TM_Create)                        , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_CURVE,      _LS(IDS_TM_Create_Line_Elements_on_Curve) , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_CONV_LINE_CREATE,_LS(IDS_TM_Create_Converted_Line_Elements), _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_OPEN_WALL_CREATE,_LS(IDS_TM_Create_Opening_Wall_Elements), _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_DELETE,     _LS(IDS_TM_Delete)                        , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_TRANSLATE,  _LS(IDS_TM_Translate)                     , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_ROTATE,     _LS(IDS_TM_Rotate)                        , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_EXTRUDE,    _LS(IDS_TM_Extrude)                       , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_MIRROR,     _LS(IDS_TM_Mirror)                        , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_DIVIDE,     _LS(IDS_TM_Divide)                        , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_MERGE,      _LS(IDS_TM_Merge)                         , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_INTERSECT,  _LS(IDS_TM_Intersect)                     , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_CHANGE,     _LS(IDS_TM_Change_Element_Parameters)     , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_COMPACT,    _LS(IDS_TM_Compact_Numbers)               , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_RENUMBER,   _LS(IDS_TM_Renumbering)                   , _T("ico24_Temp"));
			InsertTreeItem( ThTi,ID_STRUCT_ELEM_STARTNUMBER,_LS(IDS_TM_Start_Number)                  , _T("ico24_Temp"));
		}
	}

	nRootIndex++;
	//Plane Assignment 
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_PLANE_ASSIGN), _T("ico24_Temp"));
	{
		// Structure Group Assignment
		InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_STRUCTURE_GROUP,_LS(IDS_TREE_JUD_STRUCTURE_GROUP), _T("ico24_Temp"));
		// Named Plane
		InsertTreeItem( hTI[nRootIndex],ID_STRUCT_NAMEDPLANE,_LS(IDS_TM_Named_Plane), _T("ico24_Temp"));
		// 슬래브
		InsertTreeItem( hTI[nRootIndex],ID_JUD_SLAB,_LS(IDS_TREE_JUD_BEAM_STIF_SLAB), _T("ico24_Temp"));
	}

	nRootIndex++;
	//Bilding 
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Building), _T("ico24_Temp"));
	{
		// Control Data
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_BDC,_LS(IDS_TM_Control_Data), _T("ico24_Temp"));
		// Story
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_STORY,_LS(IDS_TM_Story), _T("ico24_Temp"));
	}  

	nRootIndex++;
	//경계조건
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Boundaries), _T("ico24_Temp"));
	{
		// 지점
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_BNDR_SUPPORT,_LS(IDS_TM_Supports), _T("ico24_Temp"));
		// Point Spring
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_BNDR_PSPRING,_LS(IDS_TM_Point_Spring_Supports), _T("ico24_Temp"));
		// 보 단부 해제
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_BNDR_BRELEASE,_LS(IDS_TM_Beam_End_Release), _T("ico24_Temp"));
	}
	
	nRootIndex++;
	//하중
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Static_Loads),_T("ico24_temp"));
	{
		// 자중
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_STATIC_BODYFORCE,_LS(IDS_TM_Self_Weight), _T("ico24_Temp"));
		// 물체력
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_STATIC_NODALBODF,_LS(IDS_TM_DT_NBOF), _T("ico24_Temp"));
		// 바닥하중(고정+활하중)
		ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_LOAD_SLAB), _T("ico24_Temp"));
		{
			// 바닥하중 정의
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_BLOAD_DEFFBL,_LS(IDS_TM_Define_Floor_Load_Type), _T("ico24_Temp"));
			// 바닥하중 할당
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_BLOAD_FBLOAD,_LS(IDS_TM_Assign_Floor_Loads), _T("ico24_Temp"));
		}    
		// Finishing Material Loads
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_STATIC_FMLOAD,_LS(IDS_TM_Finishing_Loads), _T("ico24_Temp"));
		// Snow Load
		//InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_SNOW_LOAD,_LS(IDS_TREE_JUD_SNOW_LOAD),_T("ico24_Temp"));    
		// 하중 직접 입력
		ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_LOAD_CUSTOM), _T("ico24_Temp"));
		{
			// 절점하중
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_NODALLOADS,_LS(IDS_TM_Nodal_Loads), _T("ico24_Temp"));
			// 선하중
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_BLOAD_LBLOAD,_LS(IDS_TM_Line_Beam_Loads), _T("ico24_Temp"));
			// 요소하중
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_BLOAD_EBLOAD,_LS(IDS_TM_Element_Beam_Loads), _T("ico24_Temp"));
			/*
			// 압력하중
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_PRESSURE,_LS(IDS_TM_Pressure_Loads),_T("ico24_Temp"));
			// 토압,수압
			InsertTreeItem(ThTi,ID_STRUCT_STATIC_HYDPRESSURE,_LS(IDS_TM_Hydrostatic_Pressure_Loads),_T("ico24_Temp"));		    
			// Temperature Load
			SeTi = InsertTreeItem(ThTi,0,_LS(IDS_TM_Temperature__Loads),_T("ico24_Temp"));
			{
				// 시스템 온도하중
				InsertTreeItem(SeTi,ID_STRUCT_STATIC_TEMP_SYSTEM,_LS(IDS_TM_System_Temperature),_T("ico24_Temp"));
				// 절점 온도하중
				InsertTreeItem(SeTi,ID_STRUCT_STATIC_TEMP_NODAL,_LS(IDS_TM_Nodal_Temperatures),_T("ico24_Temp"));
				// 요소 온도하중
				InsertTreeItem(SeTi,ID_STRUCT_STATIC_TEMP_ELEM ,_LS(IDS_TM_Element_Temperatures),_T("ico24_Temp"));        
			}
			*/
		}
	}

	nRootIndex++;
	// Masses
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TM_Masses), _T("ico24_Temp"));
	{
		// Nodal Masses
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_MASS_NMASS,_LS(IDS_TM_Nodal_Masses), _T("ico24_Temp"));
		// Load to Masses
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_MASS_LTMASS,_LS(IDS_TM_Loads_to_Masses), _T("ico24_Temp"));
	}

	nRootIndex++;
	// Lateral Loads
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_LATERAL_LOADS), _T("ico24_Temp"));
	{
		// 풍하중
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_STATIC_WIND,_LS(IDS_TM_Wind_Loads), _T("ico24_Temp"));
		// 정적지진하중
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_STATIC_SEISMIC,_LS(IDS_TM_Static_Seismic_Loads), _T("ico24_Temp"));
		// 수평하중의 방향 지정
		InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_LATERAL_LC,_LS(IDS_TREE_JUD_LATERAL_LC), _T("ico24_Temp"));
		
		// 응답스펙트럼
		/*
		ThTi = InsertTreeItem( hTI[nRootIndex],0,_LS(IDS_TM_Response_Spectrum_Analysis),_T("ico24_Temp"));	    
		{
			InsertTreeItem(ThTi,ID_STRUCT_SPEC_DEFINE,_LS(IDS_TREE_JUD_LOAD_SPEC),_T("ico24_Temp"));            
			InsertTreeItem(ThTi,ID_STRUCT_SPEC_ASSIGN,_LS(IDS_TM_Response_Spectrum_Load_Cases),_T("ico24_Temp"));
		}
		*/
	}    

	nRootIndex++;
	// 입력정보 일괄제어
	InsertTreeItem(RoothTi,D_IMPL_JUD_CONFIG,_LS(IDS_TREE_JUD_CONFIG), _T("ico24_Temp"));
	
	nRootIndex++;
	//계산정보
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_CALC_INFO), _T("ico24_Temp"));
	{    
		//모델&강성
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_MODEL,_LS(IDS_TREE_JUD_CALC_MODEL), _T("ico24_Temp"));
		//하중&응력
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_LOAD,_LS(IDS_TREE_JUD_CALC_LOAD), _T("ico24_Temp"));
		//단면 공통
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_SECT,_LS(IDS_TREE_JUD_CALC_SECT), _T("ico24_Temp"));
		//단면 RC
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_SECT_RC,_LS(IDS_TREE_JUD_CALC_SECT_RC), _T("ico24_Temp"));
		//단면 SRC
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_SECT_SRC,_LS(IDS_TREE_JUD_CALC_SECT_SRC), _T("ico24_Temp"));
		//단면 S
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_SECT_S,_LS(IDS_TREE_JUD_CALC_SECT_S), _T("ico24_Temp"));
		//접합부
		InsertTreeItem( hTI[nRootIndex],D_IMPL_JUD_CALC_JOINT,_LS(IDS_TREE_JUD_CALC_CONNECT), _T("ico24_Temp"));
	}  

	nRootIndex++;
	// 단면산정정보 
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_SECT_CALC_INFO), _T("ico24_Temp"));
	{    
		//Member Assign
		InsertTreeItem(hTI[nRootIndex],ID_DGN_GEN_MEMB,_LS(IDS_TREE_TEXT_GEN_MEMBER), _T("ico24_Temp"));
		//Unbraced Length
		BOOL bShow = FALSE;
		strText = _LS(IDS_TREE_TEXT_GEN_UNBRACED_LEN);    
		CDBDoc::GetDocPoint()->GetDesignTitleGen(ID_DGN_GEN_LENG, strText, bShow);
		InsertTreeItem(hTI[nRootIndex],ID_DGN_GEN_LENG,strText, _T("ico24_Temp"));
		//Modify member type
		InsertTreeItem(hTI[nRootIndex],ID_DGN_GEN_MBTP,_LS(IDS_TREE_TEXT_GEN_MOD_MEMBER_TYPE), _T("ico24_Temp"));
		//Bending Coeff
		strText = _LS(IDS_TREE_TEXT_STL_BEND_COEFF);
		CDBDoc::GetDocPoint()->GetDesignTitleStl(ID_DGN_STL_CBFT, strText, bShow);
	  InsertTreeItem(hTI[nRootIndex],ID_DGN_STL_CBFT,strText,_T("ico24_temp"));
		//보횡보강지정
		InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_BEAM_LATERAL_CONF,_LS(IDS_TREE_JUD_BEAM_LATERAL_CONF), _T("ico24_Temp"));
		//다이아프램 형식
		InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_DIAPHRAGM_TYPE,_LS(IDS_TREE_JUD_DIAPHRAGM_TYPE), _T("ico24_Temp"));
		//접합부 패널 보강
		InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_JOINT_PANNEL,_LS(IDS_TREE_JUD_JOINT_PANNEL), _T("ico24_Temp"));
		//주각형식
		InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_BASE_PLATE_TYPE,_LS(IDS_TREE_JUD_BASE_PLATE_TYPE), _T("ico24_Temp"));
	}
	
	nRootIndex++;
	//단면선정정보
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_SECT_SELECT), _T("ico24_Temp"));
	{
		// 철근정보(일괄)
		InsertTreeItem(hTI[nRootIndex],ID_DGN_CON_DCRB,_LS(IDS_TREE_TEXT_CON_BAR_CRITERIA), _T("ico24_Temp"));
		// 철근정보(부재별)
		ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_TEXT_CON_BAR_MEMBER), _T("ico24_Temp"));
		{
			// RC
			SeTi = InsertTreeItem(ThTi,0,_LS(IDS_TREE_JUD_RC),_T("ico24_temp"));    
			{
				// 보
				InsertTreeItem(SeTi,ID_DGN_CON_REBAR_MEMBER,_LS(IDS_TM_Beam), _T("ico24_Temp"));
				// 기둥
				InsertTreeItem(SeTi,ID_DGN_CON_REBAR_MEMBER,_LS(IDS_TM_Column), _T("ico24_Temp"));
				// 브레이스
				InsertTreeItem(SeTi,ID_DGN_CON_REBAR_MEMBER,_LS(IDS_TM_BRACE), _T("ico24_Temp"));
				// 벽
				InsertTreeItem(SeTi,ID_DGN_CON_REBAR_MEMBER,_LS(IDS_TMTB_Wall), _T("ico24_Temp"));
			}      
		}         
	}

	nRootIndex++;
	//단면검정정보
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_SECT_CALC), _T("ico24_Temp"));
	{
		// 배근정보
		ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_SECT_REBAR), _T("ico24_Temp"));
		{
			// RC
			SeTi = InsertTreeItem(ThTi,0,_LS(IDS_TREE_JUD_RC), _T("ico24_Temp"));
			{
				// 보
				InsertTreeItem(SeTi,ID_DGN_CON_REBB,_LS(IDS_TM_Beam), _T("ico24_Temp"));
				// 기둥
				InsertTreeItem(SeTi,ID_DGN_CON_REBC,_LS(IDS_TM_Column), _T("ico24_Temp"));
				// 브레이스
				InsertTreeItem(SeTi,ID_DGN_CON_REBR,_LS(IDS_TM_BRACE), _T("ico24_Temp"));
				// 벽
				InsertTreeItem(SeTi,ID_DGN_CON_REBW,_LS(IDS_TMTB_Wall), _T("ico24_Temp"));
			}   
			// SRC
			SeTi = InsertTreeItem(ThTi,0,_LS(IDS_TREE_JUD_MATL_SRC), _T("ico24_Temp"));
			{
				// 보
				InsertTreeItem(SeTi,ID_DGN_SRC_RBSB,_LS(IDS_TM_Beam), _T("ico24_Temp"));
				// 기둥
				InsertTreeItem(SeTi,ID_DGN_SRC_REBS,_LS(IDS_TM_Column), _T("ico24_Temp"));
			}   
		}    
	}

	// [Check before Release...!!] KIM GEUN-YOUNG20101014 : 2차 반영
	/*
	nRootIndex++;
	//기초
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_FOOTING),_T("ico24_Temp"));
	{
		// 독립기초
		InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_FOOTING_MAT),_T("ico24_Temp"));
		// 말뚝기초
		InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_FOOTING_PILE),_T("ico24_Temp"));
	}
	*/

	BOOL bEnableJUD2nd = CLockMgr::GetInstance()->AllowJUD2nd();
	if (bEnableJUD2nd)
	{
		nRootIndex++;
		//보유수평내력
		hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_YIELD_STRN), _T("ico24_Temp"));
		{
			// 내력 정보
			ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_YIELD_STRN_INFO), _T("ico24_Temp"));
			{
				// 균열
				InsertTreeItem(ThTi,D_IMPL_JUD_HORLOAD_CRACK,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_CRACK), _T("ico24_Temp"));
				// 종국내력
				InsertTreeItem(ThTi,D_IMPL_JUD_HORLOAD_ULTIMATE,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_YIELD), _T("ico24_Temp"));
				// 하중증분
				InsertTreeItem(ThTi,D_IMPL_JUD_HORLOAD_LOAD,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_LOAD), _T("ico24_Temp"));
				// Ds 산정
				InsertTreeItem(ThTi,D_IMPL_JUD_HORLOAD_DS,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_Ds), _T("ico24_Temp"));
				// 보유수평내력산정      
				InsertTreeItem(ThTi,D_IMPL_JUD_HORLOAD_CAP,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_SHEAR), _T("ico24_Temp"));
				// 부재종별
				InsertTreeItem(ThTi,D_IMPL_JUD_HORLOAD_ELEM,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_ELEM), _T("ico24_Temp"));
				// [Check before Release...!!] KIM GEUN-YOUNG20101014 : 2차 반영
				// 전단설계 
				//InsertTreeItem(ThTi,0,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_DGN),_T("ico24_temp"));          
			}
			// Fes
			InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_FES,_LS(IDS_TREE_JUD_YIELD_STRN_Fes), _T("ico24_Temp"));
			// Ds
			InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_DS,_LS(IDS_TREE_JUD_YIELD_STRN_Ds), _T("ico24_Temp"));
		
			/*
			// 외력 분포
			ThTi = InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_YIELD_STRN_DISP),_T("ico24_Temp"));    
			{
				// Ds 산정
				InsertTreeItem(ThTi,D_IMPL_JUD_FORCE_DS,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_Ds),_T("ico24_Temp"));    
				// 보유수평내력산정
				InsertTreeItem(ThTi,D_IMPL_JUD_FORCE_HORLOAD,_LS(IDS_TREE_JUD_YIELD_STRN_INFO_SHEAR),_T("ico24_Temp"));          
			}
			*/

			// Qud
			InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_FORCE_QUD,_T("Qud"), _T("ico24_Temp"));
			// 냉간선형강관의 내력 저감
			InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_FORCE_SNOW_FORM,_LS(IDS_TREE_JUD_SNOW_FORM), _T("ico24_Temp"));

			// 부재종별 직접지정
			InsertTreeItem(hTI[nRootIndex],D_IMPL_JUD_ASSIGN_RANK,_LS(IDS_TREE_JUD_ASSIGN_RANK), _T("ico24_Temp"));
		}
	}
	
	// MNET:XXXX-HSSHIM-20141103. iGen 기초 모듈 막아달라는 요청에 의해 기능 막음 (from 이해주 대리)
	//   //CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//   //[Check before Test...!!] Hjang:
	//   //if(pDoc->EnableJFD())
	//   //{
	//     LoadMenu_FoundationModule(this, RoothTi);
	//   //}  
	
	/*
	nRootIndex++;
	//내력 직접 입력
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_YIELD_STRN_USER),_T("ico24_temp"));
	{
		// 내력 정의
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_PUSHOVER_DEFINE_HINGE,_LS(IDS_TREE_TEXT_PHGT),_T("ico24_temp"));
		// 내력 할당
		InsertTreeItem(hTI[nRootIndex],ID_STRUCT_PUSHOVER_ASSIGN,_LS(IDS_TREE_TEXT_PHGE),_T("ico24_temp"));
		// Pushover Hinge Table
		InsertTreeItem(hTI[nRootIndex],ID_QUERY_DGN_POVR_HNGEPROP,_LS(IDS_TREE_MENU_PHGE_PROP),_T("ico24_temp"));
	}
	*/

	nRootIndex++;
	// 일괄 설계 수행
	InsertTreeItem(RoothTi,D_IMPL_JUD_RUN_DESIGN,_LS(IDS_TREE_JUD_RUN_DESIGN),_T("ico24_temp"));  

	nRootIndex++;
	//분석편의기능
	InsertTreeItem(RoothTi,D_IMPL_JUD_DGN_ANAL_MAIN,_LS(IDS_TREE_JUD_CONVENIENCE),_T("ico24_temp"));
	
	if (bEnableJUD2nd)
	{
		nRootIndex++;
		//보유수평내력 그래프
		InsertTreeItem(RoothTi,D_IMPL_JUD_DGN_ANAL_GRAPH,_LS(IDS_TREE_JUD_GRAPH),_T("ico24_temp"));
	}
	
	/*
	nRootIndex++;
	//출력 결과
	hTI[nRootIndex] = InsertTreeItem(RoothTi,0,_LS(IDS_TREE_JUD_RESULT),_T("ico24_temp"));
	{
		// 입력데이타
		InsertTreeItem(hTI[nRootIndex],ID_FILE_MODEL_DATA_TEXT,_LS(IDS_TREE_JUD_RESULT_INPUT_DATA),_T("ico24_temp"));
		// 해석결과
		InsertTreeItem(hTI[nRootIndex],ID_RESULT_TEXTOUT,_LS(IDS_TREE_JUD_RESULT_ANAL_RESULT),_T("ico24_temp"));
		// 설계결과
		InsertTreeItem(hTI[nRootIndex],0,_LS(IDS_TREE_JUD_RESULT_RESULT),_T("ico24_temp"));
	}
	*/

	nRootIndex++;
	//구조계산서
	InsertTreeItem(RoothTi,D_IMPL_JUD_TEXTOUT,_LS(IDS_TREE_JUD_TEXTOUT),_T("ico24_temp"));

	SelectItem(RoothTi);
	Expand(RoothTi,TVE_TOGGLE);
}

void CTreeJUDCtrl::LoadMenu_FoundationModule(MBaseTreeCtrl* ctrl,HTREEITEM hTreeParent, HTREEITEM hInsertAfter/* = TVI_LAST*/)
{ 
	HTREEITEM hTI_FoundModule = InsertTreeItem(hTreeParent,0,_LS(IDS_TREE_JUD_FOUND_MODULE),_T("ico24_temp"));
	{
		HTREEITEM hTI_Settings = InsertTreeItem(hTI_FoundModule,0,_LS(IDS_TREE_JUD_FOUND_SETTINGS),_T("ico24_temp"));
		{
			InsertTreeItem(hTI_Settings,ID_IMPL_JFD_FOUND_DEGN_SET,_LS(IDS_TREE_JUD_FOUND_DEGN_SET),_T("ico24_temp"));    
			InsertTreeItem(hTI_Settings,ID_IMPL_JFD_FOUND_ANAL_SET,_LS(IDS_TREE_JUD_FOUND_ANAL_SET),_T("ico24_temp"));    
			InsertTreeItem(hTI_Settings,ID_IMPL_JFD_FOUND_REBA_SET,_LS(IDS_TREE_JUD_FOUND_REBA_SET),_T("ico24_temp"));    
		}
		InsertTreeItem(hTI_FoundModule,ID_IMPL_JFD_FOUND_INPUT_INFO,_LS(IDS_TREE_JUD_FOUND_INPUT_INFO),_T("ico24_temp"));
		InsertTreeItem(hTI_FoundModule,ID_IMPL_JFD_FOUND_DEGNSP_VAL,_LS(IDS_TREE_JUD_FOUND_DEGN_SPACE_VAL),_T("ico24_temp"));
		/*
		// MNET:XXXX-CJJEONG-20121009 (2088)
		// 메뉴나 항목에 대한 이야기가 아직 안 되고 있기 때문에 우선 지우둔다.
		HTREEITEM hTI_Optimize = InsertTreeItem(hTI_FoundModule,0,_LS(IDS_TREE_JUD_FOUND_DEGN_OPTIMIZE),_T("ico24_temp"));
		{
			InsertTreeItem(hTI_Optimize,D_IMPL_JFD_FOUND_DEGN_OPTSET,_LS(IDS_TREE_JUD_FOUND_DEGN_OPTIMIZE_SET),_T("ico24_temp"));    
			InsertTreeItem(hTI_Optimize,D_IMPL_JFD_FOUND_DEGN_OPTMIZ,_LS(IDS_TREE_JUD_FOUND_DEGN_OPTIMIZE),_T("ico24_temp"));    
		}*/
		InsertTreeItem(hTI_FoundModule,ID_IMPL_JFD_FOUND_ANAL_RESULT,_LS(IDS_TREE_JUD_FOUND_DEGN_ANAL_REAULT),_T("ico24_temp"));
		InsertTreeItem(hTI_FoundModule,ID_IMPL_JFD_TEXTOUT,_LS(IDS_TREE_JUD_FOUND_DEGN_ANAL_RPT),_T("ico24_temp"));
	}
}

void CTreeJUDCtrl::UpdateBuffer(BOOL bAlwaysSet)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if(!pDoc) return;
	CViewBuff* pViewBuff = pDoc->m_pViewBuff;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
			case(UR_JDCR_ADD): //case(UR_JDUB_ADD): case(UR_JDUC_ADD): case(UR_JDAC_ADD): case(UR_JDDS_ADD): case(UR_JDHL_ADD): case(UR_JDEL_ADD):
			case(UR_JDMS_ADD): //case(UR_JDRT_ADD): case(UR_JDLS_ADD): case(UR_JDSC_ADD): case(UR_JDRC_ADD): case(UR_JDST_ADD): case(UR_JDJO_ADD):
			case(UR_GRUP_ADD): 
			case(UR_NPLN_ADD):
				Invalidate();

			default:
				break;
		}
	}
	// Remember Current Position at Tree.
	HTREEITEM   hTIKeep = GetFirstVisibleItem();
	int indx = ItemIndex(hTIKeep);	
	
	// Restore Previous Position at Tree.
	hTIKeep = GetItemAt(indx);
	if(hTIKeep)	SelectSetFirstVisible(hTIKeep);
}

void CTreeJUDCtrl::DestroyTree()
{
	for (int i=0; i<m_ImageList.GetImageCount(); i++)
		m_ImageList.Remove(0);
	m_ImageList.DeleteImageList();
}

int CTreeJUDCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (MBaseTreeCtrl::OnCreate(lpCreateStruct) == -1)
	  return -1;
	
	CreateTree();
	LoadMenu();
	EndInsertTreeItem();

	//ToggleStyle(TVS_LINESATROOT);  
	return 0;
}

void CTreeJUDCtrl::OnDestroy() 
{
	MBaseTreeCtrl::OnDestroy();
	DestroyTree();
}

void CTreeJUDCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM ht;
	ht = GetSelectedItem();
	if(FALSE == ItemHasChildren(ht))
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();

		UINT Data = GetItemData(ht);
		if(Data == 0) return;  
		else if(Data == D_IMPL_JUD_CALC_MODEL      || Data == D_IMPL_JUD_CALC_LOAD        || Data == D_IMPL_JUD_CALC_SECT    ||
						Data == D_IMPL_JUD_CALC_SECT_RC    || Data == D_IMPL_JUD_CALC_SECT_SRC    || Data == D_IMPL_JUD_CALC_SECT_S  ||
						Data == D_IMPL_JUD_CALC_JOINT      ||
						Data == D_IMPL_JUD_HORLOAD_CRACK   || Data == D_IMPL_JUD_HORLOAD_ULTIMATE || Data == D_IMPL_JUD_HORLOAD_LOAD ||
						Data == D_IMPL_JUD_HORLOAD_DS      || Data == D_IMPL_JUD_HORLOAD_CAP      || Data == D_IMPL_JUD_HORLOAD_ELEM || 
						Data == D_IMPL_JUD_FES             || Data == D_IMPL_JUD_DS               || Data == D_IMPL_JUD_FORCE_DS     ||
						Data == D_IMPL_JUD_FORCE_HORLOAD   || Data == D_IMPL_JUD_RUN_DESIGN       || Data == D_IMPL_JUD_SNOW_LOAD    || 
						Data == D_IMPL_JUD_FORCE_SNOW_FORM || Data == D_IMPL_JUD_CONFIG           || Data == D_IMPL_JUD_LATERAL_LC   ||
						Data == D_IMPL_JUD_FORCE_QUD       )
		{
			JUD_DoModal(Data);
		}
		else if(Data == D_IMPL_JUD_TEXTOUT)
		{
			if(!pDoc->IsPostMode())
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JUD_POST_MODE_ERROR));
				return;  
			}
			// 1차설계 결과만이라도 계산서기능에서 제공.
			/*
			if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() || (pDoc->GetPushoverAnalysisMode() != 1))
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JUD_ERROR_PUSHOVER_RESULT));
				return;
			}
			*/

			if (CTestEnvMgr::IsTestEnvST(_LSX(jud_test), _LSX(yes)))
			{
				JUD_DoModal(Data);
			}
			else
			{
				WPARAM wParam = MAKEWPARAM(ID_IMPL_JUD_TEXTOUT, 0);
				AfxGetMainWnd()->PostMessage(WM_COMMAND,wParam,0L);
			}
		}

		else if(Data == D_IMPL_JUD_DGN_ANAL_MAIN)
		{
			if(!pDoc->IsPostMode())
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JUD_POST_MODE_ERROR));
				return;  
			}
			// 1차설계 결과만이라도 분석편의기능에서 제공.
			/*
			if(!pDoc->m_pPostCtrl->IsPushOverResultEnable() || (pDoc->GetPushoverAnalysisMode() != 1))
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JUD_ERROR_PUSHOVER_RESULT));
				return;
			}
			*/
			if (CTestEnvMgr::IsTestEnvST(_LSX(jud_test), _LSX(yes)))
			{
				CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDDgnAnalMainDlg::IDD);
			}
			else
			{
				WPARAM wParam = MAKEWPARAM(ID_IMPL_JUD_DGN_ANAL_MAIN, 0);
				AfxGetMainWnd()->PostMessage(WM_COMMAND,wParam,0L);
			}      
		}
		else if(Data == D_IMPL_JUD_DGN_ANAL_GRAPH)
		{
			if(!pDoc->IsPostMode())
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JUD_POST_MODE_ERROR));
				return;  
			}
			if (CTestEnvMgr::IsTestEnvST(_LSX(jud_test), _LSX(yes)))
			{
				CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDLateralLoadGraphDlg::IDD);
			}
			else
			{
				WPARAM wParam = MAKEWPARAM(ID_IMPL_JUD_DGN_ANAL_GRAPH, 0);
				AfxGetMainWnd()->PostMessage(WM_COMMAND,wParam,0L);
			}           
		}
		else if(Data == D_IMPL_JUD_STRUCTURE_GROUP)
		{
			CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDStrtGroupDlg::IDD);
		}
		else if(Data == D_IMPL_JUD_JOINT_PANNEL)
		{
			CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDJointDlg::IDD);
		}
		else if(Data == D_IMPL_JUD_DIAPHRAGM_TYPE)
		{
			CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDDiapTypeDlg::IDD);
		}
		else if(Data == D_IMPL_JUD_BASE_PLATE_TYPE)
		{
			CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDBasePlateTypeDlg::IDD);
		}
		else if(Data == D_IMPL_JUD_BEAM_LATERAL_CONF)
		{
			CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDBlcfDlg::IDD);
		}
		else if(Data == D_IMPL_JUD_ASSIGN_RANK)
		{
			CreateOrActivateJUDDlg(CDBDoc::GetDocPoint(),CJUDRankDlg::IDD);
		}
		else if(Data == ID_IMPL_JFD_FOUND_DEGN_SET   )
		{
			JFD_DoModal(D_IMPL_JFD_FOUND_DEGN_SET);
		}
		else if(Data == ID_IMPL_JFD_FOUND_ANAL_SET    )
		{
			JFD_DoModal(D_IMPL_JFD_FOUND_ANAL_SET);
		}
		else if(Data == ID_IMPL_JFD_FOUND_REBA_SET)
		{
			JFD_DoModal(D_IMPL_JFD_FOUND_REBA_SET);
		}
		else if(Data == ID_IMPL_JFD_FOUND_DEGNSP_VAL  )
		{
			JFD_DoModal(D_IMPL_JFD_FOUND_DEGNSP_VAL);
		}
		else if(Data == ID_IMPL_JFD_FOUND_ANAL_RESULT )
		{
			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			BOOL bPYResult = pServiceHeadOffice->ReqService(_ULS(jfd), D_JFD_FOUND_HASPY_RESULT, NULL);
			if(bPYResult)
			{
				JFD_DoModal(D_IMPL_JFD_FOUND_ANAL_RESULT);
			}
			else
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JFD_PY_RESULT_NOT_EXIST));
			}
		}
		else if(Data == ID_IMPL_JFD_TEXTOUT)
		{
			CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
			BOOL bResult = pServiceHeadOffice->ReqService(_ULS(jfd), D_JFD_FOUND_HAS_RESULT, NULL);
			if(bResult)
			{
				JUD_DoModal(D_IMPL_JFD_TEXTOUT);
			}
			else
			{
				pDoc->DisplayHistoryMessage(_LS(IDS_TREE_JFD_RESULT_NOT_EXIST));    
			}
		}
		else if(Data == ID_IMPL_JFD_FOUND_INPUT_INFO)
		{
			CreateOrActivateJFDDlg(CDBDoc::GetDocPoint(),CJFD_FoundationInfoDlg::IDD);
		}
		else 
		{
			WPARAM wParam = MAKEWPARAM(Data,0);
			AfxGetMainWnd()->SendMessage(WM_COMMAND,wParam,0L);
		}      
	}
}

void CTreeJUDCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CTreeJUDCtrl::ToggleStyle(DWORD lStyleMask )
{
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

void CTreeJUDCtrl::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
			break;
	}
}

void CTreeJUDCtrl::SetBkGroundColor(COLORREF BkColor)
{
	m_BkColor = BkColor;
	if(GetSafeHwnd())
	{
		// change the background color
		SetBkColor( m_BkColor );
		Invalidate();
	}
}


BOOL CTreeJUDCtrl::GetCustomColor(HTREEITEM hItem)
{
	MBaseTreeCtrl::GetCustomColor(hItem);

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == 0) return FALSE;

	UINT Data = GetItemData(hItem);
	CString strItem = GetItemText(hItem);

	BOOL bSet = TRUE;

	// 필수입력항목 하이라이트 : Plan Assignment, 계산정보, 보유수평내력
	if (strItem == _LS(IDS_TREE_JUD_PLANE_ASSIGN))  bSet = IsExistPlanAssign();
	else if (strItem == _LS(IDS_TREE_JUD_CALC_INFO)   )  bSet = IsExistCalcInfo();
	else if (strItem == _LS(IDS_TREE_JUD_YIELD_STRN)  )  bSet = IsExistHorLoad();
	
	// MNET:XXXX-JBSEON-20080916 : 메인 색상 다르게 하기
	m_ColorData.m_clrText = (RGB(0,0,0));
		
	if (bSet == FALSE)
		m_ColorData.m_clrBackground = RGB(190,190,190);  // 필수입력항목 하이라이트 배경 색상 지정.

	return TRUE;
}


COLORREF CTreeJUDCtrl::GetBkColor() const
{
	//CDBDoc* pDoc = CDBDoc::GetDocPoint();
	//COLORREF Color;
	//if (pDoc != 0 && pDoc->GetDispOpt() != 0)
	//{
	//	COptColorObjNames CC;  
	//	CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(CC.m_TreeMenu_Background);
	//	Color = pColor->GetCurVal(0);
	//}
	//else
	//{
	//	CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
	//	CString csSection, csEntry, csValue;
	//	csSection = _T("DispOpt-Color");
	//	csEntry = _T("Treemenu	Background:Value");
	//	csValue = pApp->GetProfileString(csSection, csEntry, NULL);
	//	Color = RGB(128,128,128);
	//	if (!csValue.IsEmpty()) Color = (COLORREF)_ttoi(csValue);
	//}
	//return Color;
	return RGB(233, 235, 239);
}


COLORREF CTreeJUDCtrl::GetBkColorTitle() const
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	COLORREF clrColor2;
	if (pDoc != 0 && pDoc->GetDispOpt() != 0)
	{
		COptColorObjNames CC;  
		CDispOptColorObj* pColor = pDoc->GetDispOpt()->GetColor(CC.m_MV_WorktreeBkTitle);
		clrColor2 = pColor->GetCurVal(0);
	}
	else
	{
		CWinApp* pApp = AfxGetApp(); ASSERT(pApp);
		CString csSection, csEntry, csValue;
		csSection = _T("DispOpt-Color");
		csEntry = _T("Tree Menu	Background Title:Value");
		csValue = pApp->GetProfileString(csSection, csEntry, NULL);
		clrColor2 = RGB(128,128,128);
		if (!csValue.IsEmpty()) clrColor2 = (COLORREF)_ttoi(csValue);
	}
	return clrColor2;
}

 
BOOL CTreeJUDCtrl::IsExistHorLoad()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == 0) { ASSERT(0);  return FALSE; } 
	
	BOOL bSet = FALSE;
	
	bSet = (pDoc->m_pAttrCtrl2->ExistJdcr()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdub()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdus()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdac()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdds()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdhl()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdel()) ? TRUE : FALSE;

	return bSet;
}

BOOL CTreeJUDCtrl::IsExistCalcInfo()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == 0) { ASSERT(0);  return FALSE; } 
	
	BOOL bSet = FALSE;
	
	bSet = (pDoc->m_pAttrCtrl2->ExistJdms()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdrt()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdls()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdsc()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdrc()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdst()) ? TRUE : FALSE;
	bSet = (pDoc->m_pAttrCtrl2->ExistJdjo()) ? TRUE : FALSE;
	
	return bSet;
}

BOOL CTreeJUDCtrl::IsExistPlanAssign()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == 0) { ASSERT(0);  return FALSE; } 
	
	CArray<T_GRUP_K, T_GRUP_K> GrupKeyList; GrupKeyList.RemoveAll();
	CArray<T_NPLN_K, T_NPLN_K> NplnKeyList; NplnKeyList.RemoveAll();

	BOOL bSet = FALSE;
	
	pDoc->m_pAttrCtrl->GetGrupKeyList(GrupKeyList);
	pDoc->m_pAttrCtrl->GetNplnKeyList(NplnKeyList);

	bSet = (GrupKeyList.GetSize() > 0 || NplnKeyList.GetSize() > 0) ? TRUE : FALSE;
	
	return bSet;
}


