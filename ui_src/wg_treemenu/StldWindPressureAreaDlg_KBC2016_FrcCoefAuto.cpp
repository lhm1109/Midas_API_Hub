// CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_KBC2016_FrcCoefAuto.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_Qsort.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"
#include "..\wg_db\Wind_KDS41_10_15_2021.h"
#include "..\wg_db\Wind_ASCE7_16.h"
#include "..\wg_db\Wind_NSCP2024.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum enmHorCrossSectType
{
	ENM_T_SQUARE_NORMAL = 0,
	ENM_T_SQUARE_DIA,
	ENM_T_HEX_OCTA,
	ENM_T_ROUND,
	ENM_T_OCTAGON
};
enum enmInstallType
{
	ENM_T_AT_GR_LEVEL = 0,
	ENM_T_ABOVE_GR_LEVEL
};
enum enmSectMemType
{
	ENM_T_FLAT_SIDED_MEMS = 0,
	ENM_T_ROUNDED_MEMS
};
enum enmPlanType
{
	ENM_T_PLAN_SQUARE = 0,
	ENM_T_PLAN_TRIANGLE
};
enum enmMemType
{
	ENM_T_STEEL_RND_MEMS = 0,
	ENM_T_SQUARE_CRSSECT_MEMS,
	ENM_T_OTHER_TYPE
};

const int CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::COLCOUNT = 3;

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto dialog

CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto(CWnd* pParent/*= NULL*/, int nInitStructType/*=0*/, T_WVEP_K WvepK/*=NULL*/)
	: CDialogMove(CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nInitStructType = nInitStructType;
	m_WvepK = WvepK;

	m_mapIndex.RemoveAll();

	T_NODE_K NodeK;
	T_NODE_D NodeD;
	POSITION posNode = m_pDoc->m_pAttrCtrl->GetStartNode();
	double dMaxZ = 0.0;
	double dMinZ = 0.0;
	BOOL bInit = TRUE;
	while(posNode)
	{
		m_pDoc->m_pAttrCtrl->GetNextNode(posNode, NodeK, NodeD);
		if(bInit) { dMaxZ = NodeD.z; dMinZ = NodeD.z; bInit = FALSE; }
		else
		{
			if(dMaxZ < NodeD.z) dMaxZ = NodeD.z;
			if(dMinZ > NodeD.z) dMinZ = NodeD.z;
		}
	}

	T_WVEP_D WvepD;
	if(!m_pDoc->m_pAttrCtrl2->GetWvep(m_WvepK, WvepD)) WvepD.Initialize();
	m_nExposureCategory = WvepD.CodeParam.KBC2016.nExposureCategory;

	T_BLDC_D BldcD;
	if (m_pDoc->m_pAttrCtrl->GetBldc(BldcD))
	{
		if (BldcD.bUseBaseLevel) dMinZ = BldcD.dBaseLevel;
	}

	m_dTotalHeight = dMaxZ - dMinZ;

	m_Data.Initialize();
	m_DataAutoCalc.Initialize();
	m_strTargerLevel = _T("");
	m_rdoHorCrosSectType = 0;
	m_rdoSectMemType = 0;
	//
	m_edtChimneysTSSDiaHc.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDiaXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDepthXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);

	m_edtChimneysHeight.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_untChimneysHeight.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysArea.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dArea);
	m_untChimneysArea.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dArea);

	m_edtOpenSignsLFDiaXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);

	m_edtDiaOfCicularPlan.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtHeightOfCicularPlan.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);

	m_aHorCrsSectTypeShowHideCtrl.RemoveAll();
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_STATIC_DEPTHXY);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR_UNIT);
	m_aHorCrsSectTypeShowHideCtrl.FreeExtra();
	
	m_aKDS2021_HorCrsSectOther.RemoveAll();
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_HC);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_EDIT_CHIM_HC);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_EDIT_CHIM_HC_UNIT);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_DIAXY);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR);
 	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR_UNIT);
 	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_STATIC_DEPTHXY);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR_UNIT);
 	m_aKDS2021_HorCrsSectOther.Append(m_aOctagonParam);

	m_aOctagonParam.RemoveAll();
	m_aOctagonParam.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_HEIGHT);
	m_aOctagonParam.Add(IDC_WINDP_AUTOCALC_EDIT_CHIMNEYSSTSS_HEIGHT);
	m_aOctagonParam.Add(IDC_WINDP_AUTOCALC_UNIT_CHIMNEYSSTSS_HEIGHT);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSSTSS_AREA);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_EDIT_CHIMNEYSSTSS_AREA);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_UNIT_CHIMNEYSSTSS_AREA);

	m_aKDS2021_Octagon.RemoveAll();
	m_aKDS2021_Octagon.Add(IDC_WINDP_AUTOCALC_RDO_OCTA);
	m_aKDS2021_Octagon.Append(m_aOctagonParam);
	m_aChimneysTSS.RemoveAll();
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS_HCSTYPE);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_SQUARE_NORMAL);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_SQUARE_DIAGONAL);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_HEX_OCTA);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_ROUND);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_DIAXY);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR_UNIT);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_HC);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_CHIM_HC);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_CHIM_HC_UNIT);
	m_aChimneysTSS.Append(m_aHorCrsSectTypeShowHideCtrl);
	m_aChimneysTSS.Append(m_aKDS2021_Octagon);
	m_aChimneysTSS.FreeExtra();

	m_aSectMemsTypeShowHideCtrl.RemoveAll();
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSIGNSLF_DIA);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR_UNIT);
	m_aSectMemsTypeShowHideCtrl.FreeExtra();

	m_aOpenSignsLF.RemoveAll();
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF_SECT_MEMTYPE);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_RDO_FLAT_SIDED_MEM);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_RDO_ROUNDED_MEMS);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSIGNSLF_RATIO);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_XDIR);
	m_aOpenSignsLF.Append(m_aSectMemsTypeShowHideCtrl);
	m_aOpenSignsLF.FreeExtra();

	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_ROUGHNESS_STC);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_ROUGHNESS_CMB);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_DIA_STC);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_DIA_EDT);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_DIA_UNIT);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_STC);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_EDT);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_UNIT);

	
	m_aCirclePlan_KBC.RemoveAll();
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_ROUGHNESS_STC);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_ROUGHNESS_CMB);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_DIA_STC);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_DIA_EDT);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_DIA_UNIT);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_STC);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_EDT);
	m_aCirclePlan_KBC.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_UNIT);
	m_aCirclePlan_KBC.FreeExtra();

	m_aCirclePlan_ASCE7.RemoveAll();
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_STATIC_CIRCLE_HC);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_EDIT_CIRCLE_HC);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_EDIT_CIRCLE_HC_UNIT);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_STATIC_CIRCLE_DIAXY);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DIAXY_XDIR);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DIAXY_XDIR_UNIT);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_STATIC_CIRCLE_DEPTHXY);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DEPTHXY_XDIR);
	m_aCirclePlan_ASCE7.Add(IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DEPTHXY_XDIR_UNIT);
	m_aCirclePlan_ASCE7.FreeExtra();

	m_aCirclePlan.RemoveAll();
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_STATIC_CIRCLE_PLAN);
	m_aCirclePlan.Append(m_aCirclePlan_KBC);
	m_aCirclePlan.Append(m_aCirclePlan_ASCE7);
	m_aCirclePlan.FreeExtra();

	m_aLatticeTowers.RemoveAll();
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_STATIC_LATOWERS);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_GRB);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO2);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_MEMBTYPE_GRB);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_MEMBTYPE_RDO1);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_MEMBTYPE_RDO2);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_GRB);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_RDO1);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_RDO2);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_RATIO_STC);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_RATIO_X_DIR_EDT);
	m_aLatticeTowers.FreeExtra();

	m_aFencesOnGrnd.RemoveAll();
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_STATIC_FENCEGRND);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_STC);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_X_DIR_EDT);
	m_aFencesOnGrnd.FreeExtra();

	m_aLTowersPlanRdo.RemoveAll();
	m_aLTowersPlanRdo.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1);
	m_aLTowersPlanRdo.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO2);
	m_aLTowersPlanRdo.FreeExtra();

	m_aSolidFree.RemoveAll();
	m_aSolidFree.Add(IDC_WIND_AUTOCALC_STATIC_SOLID_FREE);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_STATIC_SOLID_FREE_B);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_B);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_B_UNIT);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_STATIC_SOLID_FREE_H);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_H);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_H_UNIT);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_STATIC_SOLID_FREE_S);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_S);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_S_UNIT);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_STATIC_SOLID_FREE_R);
	m_aSolidFree.Add(IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_R);
	m_aSolidFree.FreeExtra();


}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WINDP_AUTOCALC_CMB_STRUCTURE_TYPE, m_cmbStructureType);
	DDX_Text(pDX,    IDC_WINDP_AUTOCALC_EDIT_TARGET_STORY, m_strTargerLevel);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_LIST_STORY, m_lstStory);
	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_SQUARE_NORMAL, m_rdoHorCrosSectType);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CHIM_HC, m_edtChimneysTSSDiaHc);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CHIM_HC_UNIT, m_unitChimneysTSSDiaHc);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR, m_edtChimneysTSSDiaXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR_UNIT, m_unitChimneysTSSDia);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR, m_edtChimneysTSSDepthXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR_UNIT, m_unitChimneysTSSDepth);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CHIMNEYSSTSS_HEIGHT, m_edtChimneysHeight);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_UNIT_CHIMNEYSSTSS_HEIGHT, m_untChimneysHeight);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_CHIMNEYSSTSS_AREA, m_edtChimneysArea);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_UNIT_CHIMNEYSSTSS_AREA, m_untChimneysArea);

	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_FLAT_SIDED_MEM, m_rdoSectMemType);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_XDIR, m_edtOpenSignsLFRatioXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR, m_edtOpenSignsLFDiaXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR_UNIT, m_unitOpenSignsLFDia);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_ROUGHNESS_CMB, m_cmbRoughness);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_DIA_EDT, m_edtDiaOfCicularPlan);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_DIA_UNIT, m_unitDiaOfCicularPlan);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_EDT, m_edtHeightOfCicularPlan);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_UNIT, m_unitHeightOfCicularPlan);
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1, m_rdoLTowersPlanType);
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_LATOWERS_MEMBTYPE_RDO1, m_rdoLTowersMembType);
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_RDO1, m_rdoLTowersWindDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_LATOWERS_RATIO_X_DIR_EDT, m_edtLTowersRatioXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_FENCEGRND_RATIO_X_DIR_EDT, m_edtFenceGrndRatioXDir);

	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CIRCLE_HC, m_edtCircleTSSDiaHc);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CIRCLE_HC_UNIT, m_unitCircleTSSDiaHc);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DIAXY_XDIR, m_edtCircleTSSDiaXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DIAXY_XDIR_UNIT, m_unitCircleTSSDia);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DEPTHXY_XDIR, m_edtCircleTSSDepthXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CIRCLE_DEPTHXY_XDIR_UNIT, m_unitCircleTSSDepth);

	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_B, m_edtSolidFree_B);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_H, m_edtSolidFree_H);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_S, m_edtSolidFree_S);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_R, m_edtSolidFree_R);

	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_B_UNIT, m_untSolidFree_B);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_H_UNIT, m_untSolidFree_H);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLID_FREE_S_UNIT, m_untSolidFree_S);

}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto, CDialogMove)
	ON_CBN_SELCHANGE(IDC_WINDP_AUTOCALC_CMB_STRUCTURE_TYPE,   OnSelchangeTypeCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WINDP_AUTOCALC_LIST_STORY, OnSelchangeStoryListCtrl)
	ON_NOTIFY(LVN_KEYDOWN, IDC_WINDP_AUTOCALC_LIST_STORY,     OnStoryListKeyDown)
	// button command
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_GENERATEALL,       OnCmdGenerate)  
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_BTN_APPLY_REPLACE, OnTgtStorAddReplace)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_BTN_SEL_STORY_DLG, OnSelectAll)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_BTN_DELETE,        OnSelStorDelete)
	// ChimneysTSS
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_SQUARE_NORMAL,   OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_SQUARE_DIAGONAL, OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_HEX_OCTA,        OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_ROUND,           OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_OCTA,			  OnRdoClikedChimneysTSS)
	// OpenSignsLF
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_FLAT_SIDED_MEM, OnRdoClikedOpenSignsLF)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_ROUNDED_MEMS,   OnRdoClikedOpenSignsLF)
	// Lattice Towers
	ON_BN_CLICKED(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1, OnRdoClikedLTowersPlan)
	ON_BN_CLICKED(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO2, OnRdoClikedLTowersPlan)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto message handlers
BOOL CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCtrls();  
	m_DataAutoCalc.nStructType = m_nInitStructType;
	if(m_nInitStructType==ENM_T_CIRCLE_PLAN)
	{
		m_DataAutoCalc.dDirVal1 = 0.75;

		T_WVEP_D WvepD;
		if(m_pDoc->m_pAttrCtrl2->GetWvep(m_WvepK, WvepD))
		{
			if (m_Data.GetASCE7Series() != NULL)
			{
				m_DataAutoCalc.dDirVal3 = WvepD.GetASCE7Series()->dMeanRoofHeight;
				m_DataAutoCalc.dDirVal1 = 0.0;
			}
			else
			{
				m_DataAutoCalc.dDirVal3 = WvepD.CodeParam.KBC2016.nMethod == 0 ? WvepD.CodeParam.KBC2016.dSimpleRoofHeight : WvepD.CodeParam.KBC2016.dRoofHeight;
			}
		}
	}

	const int nCodeType = m_Data.nCodeType;
	if (nCodeType == ASCE7_W_2016 || nCodeType == ASCE7_W_2022 || nCodeType == NSCP_W_2024)
	{
		int    nStore = 0;
		double dHeight = 0.0;
		double dWidth = 0.0;

		if (CWindLoadGeneratorCtrl::GetModelHeightWidthTotal(NULL, dHeight, nStore, dWidth))
		{
			m_DataAutoCalc.dHc = dHeight;
		}
	}
// 	if (m_Data.nCodeType == KDS_W_2021)
// 	{
// 		WnatStorD.dHeight = m_WindD.CodeParam.KDS2021.dRoofHeight;
// 	}
	Data2Dlg(m_DataAutoCalc);

	SetDlgByCombo();

	SetLstHeaderTitle();
	MakeLstItem();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnOK() 
{
	// TODO: Add extra validation here
	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}


void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnSelchangeTypeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_DataAutoCalc.Initialize();
	m_DataAutoCalc.nStructType = m_cmbStructureType.GetCurSel();

	Data2Dlg(m_DataAutoCalc);

	SetDlgByCombo();

	UpdateData(FALSE);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnSelchangeStoryListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = 0;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nSelectItem = pNMLV->iItem;
	if (nSelectItem == -1) return;
	nIndex = (int)pNMLV->lParam;

	int nArrayIndex = 0;
	if(!m_mapIndex.Lookup(nIndex, nArrayIndex)) { ASSERT(0); nArrayIndex=0; }

	T_WDPR_KBC_AUTOCALC* pAutoCalc = &m_Data.arKBC2009ForceCoef[nArrayIndex];

	Data2Dlg(*pAutoCalc);
	SetDlgByCombo();

	m_strTargerLevel.Empty();
	CString strTemp;
	POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nSelItem = m_lstStory.GetNextSelectedItem(pos);
		LVITEM lvItem = {0};
		lvItem.iItem = nSelItem;
		lvItem.mask = (LVIF_TEXT | LVIF_PARAM);
		m_lstStory.GetItem(&lvItem);
		nIndex = (int)lvItem.lParam;
		if(!m_mapIndex.Lookup(nIndex, nArrayIndex)) { ASSERT(0); nArrayIndex=0; }

		T_WDPR_KBC_AUTOCALC& rAutoCalc2 = m_Data.arKBC2009ForceCoef[nArrayIndex];

		if(m_strTargerLevel.IsEmpty())
			strTemp.Format(_T("%.3f"), rAutoCalc2.dLevel);
		else
			strTemp.Format(_T(", %.3f"), rAutoCalc2.dLevel);
		m_strTargerLevel += strTemp;
	}

	UpdateData(FALSE);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnStoryListKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	/* ESC가 눌러졌을때 처리 */
	if (pLKeyDown->wVKey == VK_ESCAPE)
	{
		int nCnt = m_lstStory.GetSelectedCount();
		if (nCnt > 0)
		{
			m_DataAutoCalc.Initialize();
			m_DataAutoCalc.nStructType = m_nInitStructType;
			Data2Dlg(m_DataAutoCalc);
			SetDlgByCombo();

			POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
			while (pos != NULL)
			{
				int nSelItem = m_lstStory.GetNextSelectedItem(pos);
				m_lstStory.SetItemState(nSelItem, FALSE, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		m_strTargerLevel.Empty();    
		UpdateData(FALSE);
	}
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnSelectAll()
{
	int i;
	int nCnt = m_lstStory.GetItemCount();
	for( i = 0 ; i < nCnt; ++i)
	{
		m_lstStory.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}
	m_lstStory.SetFocus();
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnTgtStorAddReplace()
{
	if(!Dlg2Data(m_DataAutoCalc))
		return;

	UpdateData(TRUE);

	CArray<double, double> caDblLevel;
	TCHAR* pCharTgt = m_strTargerLevel.GetBuffer(m_strTargerLevel.GetLength());
	TCHAR* pCh;
	pCh = _tcstok(pCharTgt, _T(" ,"));
	while(pCh != NULL)
	{
		caDblLevel.Add(_tstof(pCh));
		pCh = _tcstok(NULL, _T(" ,"));
	}

	double dQz;
	double dCf;
	double dLevel;
	int i, j;
	int nSize = caDblLevel.GetSize();
	int nSizeData;
	int nIdxInsert;
	BOOL bReplace = FALSE;
	for(i = 0 ; i < nSize; i++)
	{
		dLevel = caDblLevel[i];
		if(!m_pDoc->m_pAttrCtrl2->GetWindQz(m_Data, dLevel, dQz))
			continue;    
		m_DataAutoCalc.dLevel = dLevel;
		m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalcKBC2016(m_DataAutoCalc, m_DataAutoCalc.dHc, dLevel, m_nExposureCategory, dQz, dCf);
		
		m_DataAutoCalc.dCoefFactor = dCf;
		// 일단 추가될 데이터를 만든다.

		bReplace = FALSE;

		const int nCodeType = m_Data.nCodeType;
		if (nCodeType == KBC_W_2016)
		{
			// Replace를 검사하고 같은 Level이 존재하면 Replace..
			nSizeData = m_Data.arKBC2009ForceCoef.GetSize();
			for (j = 0; j < nSizeData; j++)
			{
				if (fabs(dLevel - m_Data.arKBC2009ForceCoef[j].dLevel) <= 1E-5)
				{
					bReplace = TRUE;
					m_Data.arKBC2009ForceCoef[j] = m_DataAutoCalc;
					break;
				}
			}
			if (bReplace) continue;

			//같은 값이 없을때는 추가한다. 추가하는 위치를 찾고 그위치에 insert.
			if (nSizeData == 0) nIdxInsert = 0;
			else
			{
				if (dLevel < m_Data.arKBC2009ForceCoef[0].dLevel)                  nIdxInsert = 0;
				else if (dLevel > m_Data.arKBC2009ForceCoef[nSizeData - 1].dLevel) nIdxInsert = nSizeData;
				else
				{
					for (j = 1; j < nSizeData; j++)
					{
						if (dLevel <= m_Data.arKBC2009ForceCoef[j].dLevel)
						{
							nIdxInsert = j;
							break;
						}
					}
				}
			}

			m_Data.arKBC2009ForceCoef.InsertAt(nIdxInsert, m_DataAutoCalc);
		}
		else if (nCodeType == KDS_W_2019   || nCodeType == KDS_W_2022 ||
			nCodeType == ASCE7_W_2016 || nCodeType == ASCE7_W_2022 || nCodeType == NSCP_W_2024)
		{
			// Replace를 검사하고 같은 Level이 존재하면 Replace..
			nSizeData = m_Data.arKBC2009ForceCoef.GetSize();
			for(j = 0 ; j < nSizeData; j++)
			{
				if(fabs(dLevel - m_Data.arKBC2009ForceCoef[j].dLevel) <= 1E-5)
				{
					bReplace = TRUE;
					m_Data.arKBC2009ForceCoef[j] = m_DataAutoCalc;
					break;
				}
			}
			if(bReplace) continue;

			//같은 값이 없을때는 추가한다. 추가하는 위치를 찾고 그위치에 insert.
			if(nSizeData == 0) nIdxInsert = 0;
			else
			{
				if(dLevel < m_Data.arKBC2009ForceCoef[0].dLevel)                  nIdxInsert = 0;
				else if(dLevel > m_Data.arKBC2009ForceCoef[nSizeData - 1].dLevel) nIdxInsert = nSizeData;
				else
				{
					for(j = 1 ; j < nSizeData; j++)
					{
						if(dLevel <= m_Data.arKBC2009ForceCoef[j].dLevel)
						{
							nIdxInsert = j;
							break;
						}
					}
				}
			}

			m_Data.arKBC2009ForceCoef.InsertAt(nIdxInsert, m_DataAutoCalc);
		}
		else ASSERT(0);
	}

	MakeLstItem();
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnSelStorDelete()
{
	CArray<int, int> caIdx;
	POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nSelItem = m_lstStory.GetNextSelectedItem(pos);
		LVITEM lvItem = {0};
		lvItem.iItem = nSelItem;	
		lvItem.mask = (LVIF_TEXT | LVIF_PARAM);
		m_lstStory.GetItem(&lvItem);
		int nSelIdx = (int)lvItem.lParam;
		caIdx.Add(nSelIdx);
	}

	int nSize = caIdx.GetSize();
	int nIdxPre = -1;
	CQSort::QSortInt(caIdx.GetData(), nSize); //오름차순으로 정렬된다.
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		if(nIdxPre == caIdx[i])
			continue;


		if (caIdx[i] >= 0 && caIdx[i] < m_Data.arKBC2009ForceCoef.GetSize())
		{
            m_Data.arKBC2009ForceCoef.RemoveAt(caIdx[i]);
            nIdxPre = caIdx[i];
		}
		
	}

	MakeLstItem();

	m_strTargerLevel.Empty();    
	UpdateData(FALSE);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnRdoClikedChimneysTSS() 
{
	UpdateData(TRUE);
	ShowCtrlByHorCrsSectType();
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnRdoClikedOpenSignsLF() 
{
	UpdateData(TRUE);
	ShowCtrlBySectMemsType();
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnRdoClikedLTowersPlan() 
{
	EnableCtrlByPlanType();
}

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto member method
void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::InitCtrls()
{	
	AlignControl();	

	// Structure Combo
	m_cmbStructureType.ResetContent();
	int nIndex = 0;

	const bool bASCE7_16Series = [](int nCodeType)
	{
		switch (nCodeType)
		{
		case ASCE7_W_2016:
		case ASCE7_W_2022:
		case NSCP_W_2024:
			return true;
		default:
			return false;
		}
	}(m_Data.nCodeType);

	CString strTitle = _T("");
	switch (m_nInitStructType)
	{
	case ENM_T_CIRCLE_PLAN:     strTitle = _LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN); break;
	case ENM_T_LATTICE_TOWERS: 
		{
			strTitle = bASCE7_16Series ? _LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS) : _LS(IDS_CMD_WIND_WNAT_LATTICE_TOWERS);
		}
		break;
	case ENM_T_OPENSIGNSLF:  
		{
			strTitle = bASCE7_16Series ? _LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_SINGLEPLANE) : _LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS);
		}
		break;
	case ENM_T_CHIMNEYSTSS:     strTitle = _LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES); break;
	case ENM_T_FENCES_ON_GRND:  strTitle = _LS(IDS_CMD_WIND_WNAT_FENCES_ON_GROUND); break;
	case ENM_T_SOLID_FREE:		strTitle = _LS(IDS_CMD_WIND_WNAT_SOLID_FREESTANDING); break;
	case ENM_T_TRUSTOWERS:		strTitle = _LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS); break;
	default: ASSERT(0); break;
	}

	if (bASCE7_16Series)
	{
		GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF)->SetWindowText(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_SINGLEPLANE));
		GetDlgItem(IDC_WIND_AUTOCALC_STATIC_LATOWERS)->SetWindowText(_LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS));
		GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF)->SetWindowText(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_SINGLEPLANE));
	}

	nIndex = m_cmbStructureType.AddString(strTitle);
	m_cmbStructureType.SetItemData(nIndex, m_nInitStructType);
	m_cmbStructureType.EnableWindow(FALSE);

	m_cmbRoughness.ResetContent();
	CString aItem[] = {_T("0.75"), _T("0.9"), _T("1.0")};
	int nNum = sizeof(aItem) / sizeof(CString);
	for (int i=0; i<nNum; i++)
		m_cmbRoughness.AddString(aItem[i]);
	m_cmbRoughness.SetCurSel(0); 
}
void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::Data2Dlg(const T_WDPR_KBC_AUTOCALC& WdprKBC)
{
	int nSelect = -1;
	int nCount = m_cmbStructureType.GetCount();
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		if(WdprKBC.nStructType != m_cmbStructureType.GetItemData(nIndex)) continue;
		nSelect = nIndex;
		break;
	}
	m_cmbStructureType.SetCurSel(nSelect);

	int nSubType1 = WdprKBC.nSubType1;
	int nSubType2 = WdprKBC.nSubType2;
	int nSubType3 = WdprKBC.nSubType3;
	double dDir[4] = {WdprKBC.dDirVal1, WdprKBC.dDirVal2, WdprKBC.dDirVal3, WdprKBC.dDirVal4};

	switch(WdprKBC.nStructType)
	{
	case ENM_T_CHIMNEYSTSS:			m_rdoHorCrosSectType = nSubType1; break;
	case ENM_T_OPENSIGNSLF:			m_rdoSectMemType = nSubType1; break;
	case ENM_T_CIRCLE_PLAN:			break;
	case ENM_T_LATTICE_TOWERS:	m_rdoLTowersPlanType = nSubType1; m_rdoLTowersMembType = nSubType2; m_rdoLTowersWindDir = nSubType3; break;
	case ENM_T_FENCES_ON_GRND:	break;
	}

	if(WdprKBC.nStructType==ENM_T_CIRCLE_PLAN)
	{
		CString strValue = _T("");
		if (dDir[0] == 0.75  || dDir[0] == 0.9  || dDir[0] == 1.)
		{
			strValue.Format(_T("%g"), dDir[0]);
			m_cmbRoughness.SelectString(-1, strValue);
		}
		else
		{
			strValue.Format(_T("%g"), dDir[0]);
			m_cmbRoughness.SetWindowText(strValue);
		}
	}

	m_edtChimneysTSSDiaXDir.SetEditUnit(dDir[0]);
	m_edtChimneysTSSDiaHc.SetEditUnit(WdprKBC.dHc);
	m_edtChimneysTSSDepthXDir.SetEditUnit(dDir[1]);
	m_edtChimneysHeight.SetEditUnit(WdprKBC.dHeight);
	m_edtChimneysArea.SetEditUnit(WdprKBC.dArea);

	m_edtCircleTSSDiaXDir.SetEditUnit(dDir[0]);
	m_edtCircleTSSDiaHc.SetEditUnit(WdprKBC.dHc);
	m_edtCircleTSSDepthXDir.SetEditUnit(dDir[1]);

	m_edtOpenSignsLFRatioXDir.SetEditUnit(dDir[0]);
	m_edtOpenSignsLFDiaXDir.SetEditUnit(dDir[1]);

	m_edtDiaOfCicularPlan.SetEditUnit(dDir[1]);
	m_edtHeightOfCicularPlan.SetEditUnit(dDir[2]);

	m_edtLTowersRatioXDir.SetEditUnit(dDir[0]);

	m_edtFenceGrndRatioXDir.SetEditUnit(dDir[0]);

	m_edtSolidFree_B.SetEditUnit(dDir[0]);
	m_edtSolidFree_H.SetEditUnit(dDir[1]);
	m_edtSolidFree_S.SetEditUnit(dDir[2]);
	m_edtSolidFree_R.SetEditUnit(dDir[3]);

	UpdateData(FALSE);
}

BOOL CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::Dlg2Data(T_WDPR_KBC_AUTOCALC& WdprKBC)
{	
	UpdateData(TRUE);

	WdprKBC.nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	int nSubType1 = 0;
	int nSubType2 = 0;
	int nSubType3 = 0;
	double dHc = 0.0;
	double dDir[4] = {0., };
	double dHeight = 0.;
	double dArea = 0.;
	const bool bASCE7_16Series = [](int nCodeType)
	{
		switch (nCodeType)
		{
		case ASCE7_W_2016:
		case ASCE7_W_2022:
		case NSCP_W_2024:
			return true;
		default:
			return false;
		}
	}(m_Data.nCodeType);
	
	switch(WdprKBC.nStructType)
	{
	case ENM_T_CHIMNEYSTSS:		
		{
			nSubType1 = m_rdoHorCrosSectType; 
			dDir[0]	= m_edtChimneysTSSDiaXDir.GetEditValue();
			dHc = m_edtChimneysTSSDiaHc.GetEditValue();
			dDir[1]	= m_edtChimneysTSSDepthXDir.GetEditValue();
			dHeight = m_edtChimneysHeight.GetEditValue();
			dArea = m_edtChimneysArea.GetEditValue();
			break;
		}
	case ENM_T_OPENSIGNSLF:		
		{
			nSubType1 = m_rdoSectMemType; 
			dDir[0]	= m_edtOpenSignsLFRatioXDir.GetEditValue();
			dDir[1]	= m_edtOpenSignsLFDiaXDir.GetEditValue();
			break;
		}
	case ENM_T_CIRCLE_PLAN:
		{
			if (bASCE7_16Series)
			{
				dDir[0] = m_edtCircleTSSDiaXDir.GetEditValue();
				dHc = m_edtCircleTSSDiaHc.GetEditValue();
				dDir[1] = m_edtCircleTSSDepthXDir.GetEditValue();

			}
			else
			{
				CString strValue = _T("");
				m_cmbRoughness.GetWindowText(strValue);
				dDir[0] = _tstof(strValue);
				dDir[1] = m_edtDiaOfCicularPlan.GetEditValue();
				dDir[2] = m_edtHeightOfCicularPlan.GetEditValue();

			}
			break;
		}
	case ENM_T_LATTICE_TOWERS:		
		{
			nSubType1 = m_rdoLTowersPlanType;
			nSubType2 = m_rdoLTowersMembType;
			nSubType3 = m_rdoLTowersWindDir;
			dDir[0]	= m_edtLTowersRatioXDir.GetEditValue();
			break;
		}
	case ENM_T_FENCES_ON_GRND:		
		{
			dDir[0]	= m_edtFenceGrndRatioXDir.GetEditValue();
			break;
		}
	case ENM_T_SOLID_FREE:
		{
			dDir[0] = m_edtSolidFree_B.GetEditValue();
			dDir[1] = m_edtSolidFree_H.GetEditValue();
			dDir[2] = m_edtSolidFree_S.GetEditValue();
			dDir[3] = m_edtSolidFree_R.GetEditValue();
			break;
		}
	}

	WdprKBC.nSubType1 = nSubType1;
	WdprKBC.nSubType2 = nSubType2;
	WdprKBC.nSubType3 = nSubType3;
	WdprKBC.dDirVal1 = dDir[0]; 
	WdprKBC.dDirVal2 = dDir[1];
	WdprKBC.dDirVal3 = dDir[2];
	WdprKBC.dDirVal4 = dDir[3];
	WdprKBC.dHc       = dHc;
	WdprKBC.dHeight = dHeight;
	WdprKBC.dArea   = dArea;

	const double dTol = 1E-5;
	BOOL bWarning = TRUE;
	if(bWarning)
	{
		if(WdprKBC.nStructType == ENM_T_CHIMNEYSTSS)
		{
			if( nSubType1 != ENM_T_OCTAGON && (WdprKBC.dDirVal1 <= dTol || dHc <= dTol))
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Dim),MB_OK);
				return FALSE;
			}
			if (nSubType1 == ENM_T_OCTAGON && (dArea <= dTol || dHeight <= dTol))
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Dim), MB_OK);
				return FALSE;
			}
		}
		else if(WdprKBC.nStructType == ENM_T_OPENSIGNSLF )
		{
			if(WdprKBC.dDirVal1 > 0.7) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Solid),MB_OK);
				return FALSE;
			}
		}
		else if(WdprKBC.nStructType == ENM_T_CIRCLE_PLAN)
		{
			if(WdprKBC.dDirVal1 < 0.0) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Roughness),MB_OK);
				return FALSE;
			}
			if(WdprKBC.dDirVal2 <= 0.0) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Diameter),MB_OK);
				return FALSE;
			}
			if(WdprKBC.dDirVal3 < 0.0) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Height),MB_OK);
				return FALSE;
			}

			double dRatioHD = WdprKBC.dDirVal3/WdprKBC.dDirVal2;
			if(dRatioHD>8.0) // H/D 값은 8을 초과할 수 없음
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_HD),MB_OK);
				return FALSE;
			}
		}
		else if(WdprKBC.nStructType == ENM_T_LATTICE_TOWERS)
		{
			if ( WdprKBC.dDirVal1 > 1.0 || WdprKBC.dDirVal1 < 0.0 )
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Truss_Tower),MB_OK);
				return FALSE;
			}
		}
		else if(WdprKBC.nStructType == ENM_T_FENCES_ON_GRND)
		{
			if(WdprKBC.dDirVal1 > 1.0) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Fence),MB_OK);
				return FALSE;
			}
		}

        if ( WdprKBC.nStructType != ENM_T_CHIMNEYSTSS || nSubType1 != ENM_T_OCTAGON )
        {
            if ( WdprKBC.dDirVal1 <= -dTol || WdprKBC.dDirVal2 <= -dTol || WdprKBC.dDirVal3 <= -dTol || WdprKBC.dDirVal4 <= -dTol )
            {
                AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir), MB_OK);
                return FALSE;
            }
        }
	}

	return TRUE;
}
void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	//기본이 되는 프레임 위치.
	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS)->GetWindowRect(rRef);

	//아래의 프레임은 기본이 되는 프레임 위치로 옮긴다.

	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aOpenSignsLF, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_CIRCLE_PLAN)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCirclePlan, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_LATOWERS)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aLatticeTowers, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_FENCEGRND)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aFencesOnGrnd, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLID_FREE)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSolidFree, nDistY);

	CRect rRef_Circle;
	GetDlgItem(IDC_WIND_AUTOCALC_ROUGHNESS_STC)->GetWindowRect(rRef_Circle);
	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_CIRCLE_HC)->GetWindowRect(rToMove);
	nDistY = rRef_Circle.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCirclePlan_ASCE7, nDistY);

	switch(m_nInitStructType)
	{
	case ENM_T_CIRCLE_PLAN: GetDlgItem(IDC_WIND_AUTOCALC_STATIC_CIRCLE_PLAN)->GetWindowRect(rRef); break;
	case ENM_T_LATTICE_TOWERS: GetDlgItem(IDC_WIND_AUTOCALC_STATIC_LATOWERS)->GetWindowRect(rRef); break;
	case ENM_T_OPENSIGNSLF: GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF)->GetWindowRect(rRef); break;
	case ENM_T_CHIMNEYSTSS: GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS)->GetWindowRect(rRef); break;
	case ENM_T_FENCES_ON_GRND: GetDlgItem(IDC_WIND_AUTOCALC_STATIC_FENCEGRND)->GetWindowRect(rRef); break;
	case ENM_T_SOLID_FREE: GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLID_FREE)->GetWindowRect(rRef); break;
	case ENM_T_TRUSTOWERS: GetDlgItem(IDC_WIND_AUTOCALC_STATIC_CIRCLE_PLAN)->GetWindowRect(rRef); break;
	default:
		ASSERT(false);
		break;
	}

	CArray<UINT, UINT> aGenerateCtrl; aGenerateCtrl.Add(IDC_WINDP_AUTOCALC_GENERATEALL);
	GetDlgItem(IDC_WINDP_AUTOCALC_GENERATEALL)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.bottom - globalUtils.ScaleByDPI(4);
	CDlgUtil::CtrlMoveDistY(this, aGenerateCtrl, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);

	SetWindowPos(NULL, r.left, r.top, r.Width(), r.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::SetDlgByCombo()
{
	int nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	CDlgUtil::CtrlShowHide(this, m_aChimneysTSS		, nStructType==ENM_T_CHIMNEYSTSS);
	CDlgUtil::CtrlShowHide(this, m_aOpenSignsLF		, nStructType==ENM_T_OPENSIGNSLF);
	CDlgUtil::CtrlShowHide(this, m_aCirclePlan		, nStructType==ENM_T_CIRCLE_PLAN);
	CDlgUtil::CtrlShowHide(this, m_aLatticeTowers	, nStructType==ENM_T_LATTICE_TOWERS);
	CDlgUtil::CtrlShowHide(this, m_aFencesOnGrnd	, nStructType==ENM_T_FENCES_ON_GRND);
	CDlgUtil::CtrlShowHide(this, m_aSolidFree		, nStructType==ENM_T_SOLID_FREE);

	switch(nStructType)
	{
	case ENM_T_CHIMNEYSTSS:
		{
			CDlgUtil::CtrlShowHide(this, m_aKDS2021_Octagon, m_Data.nCodeType == KDS_W_2022);
			ShowCtrlByHorCrsSectType();
			break;
		}
	case ENM_T_OPENSIGNSLF:
		{
			ShowCtrlBySectMemsType();
			break;
		}
	case ENM_T_LATTICE_TOWERS:
		{
			EnableCtrlByPlanType();
			break;
		}
	case ENM_T_CIRCLE_PLAN:
		{
			const BOOL bIsASCE7Series = [](int nCodeType)
			{
				switch (nCodeType)
				{
				case ASCE7_W_2016:
				case ASCE7_W_2022:
				case NSCP_W_2024:
					return TRUE;
				default:
					return FALSE;
				}
			}(m_Data.nCodeType);
			
			CDlgUtil::CtrlShowHide(this, m_aCirclePlan_KBC, !bIsASCE7Series);
			CDlgUtil::CtrlShowHide(this, m_aCirclePlan_ASCE7, bIsASCE7Series);
			break;
		}
	}
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::SetLstHeaderTitle()
{
	CStringArray strAry;
	strAry.Add(_LS(IDS_TM_WINDP_KBC2009_LIST_INDEX));
	{
		CString strTemp;
		CString strUnit;
		m_pDoc->m_pUnitCtrl->GetUnitSystemLength(strUnit);
		strTemp.Format(_T("%s(%s)"), _LS(IDS_TM_WINDP_KBC2009_LIST_LEVEL), strUnit);
		strAry.Add(strTemp);
	}
	strAry.Add(_LS(IDS_TM_WINDP_KBC2009_LIST_COEFF));

	CRect listRect;
	m_lstStory.GetWindowRect(listRect);
	int nWidth = listRect.Width();
	int nColWidth[COLCOUNT];
	nColWidth[0] = nWidth * 0.18;
	nColWidth[1] = nWidth * 0.32;
	nColWidth[2] = nWidth - GetSystemMetrics(SM_CYVSCROLL) - nColWidth[0] - nColWidth[1] - 5;

	m_lstStory.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	for(int i = 0; i < COLCOUNT; ++i)
	{
		LV_COLUMN lvCol;
		lvCol.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvCol.fmt = i == 0 ? LVCFMT_LEFT : LVCFMT_RIGHT;
		lvCol.cx = nColWidth[i];
		lvCol.pszText = strAry[i].GetBuffer(strAry[i].GetLength());
		m_lstStory.InsertColumn(i, &lvCol);
	}
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::MakeLstItem()
{
	m_lstStory.DeleteAllItems();


	int nSize = m_Data.arKBC2009ForceCoef.GetSize();
	int nIndex = 0;
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		if(m_nInitStructType!=m_Data.arKBC2009ForceCoef[i].nStructType) continue;

		InsertItem(m_Data.arKBC2009ForceCoef[i], nIndex);
		m_mapIndex.SetAt(nIndex, i);
		nIndex++;
	}
	
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::InsertItem(const T_WDPR_KBC_AUTOCALC& WdprKBC, const int& nIndex)
{
	int nItemCnt = m_lstStory.GetItemCount();

	CStringArray strAry;
	CString strTemp;
	strTemp.Format(_T("%d"), nItemCnt + 1);
	strAry.Add(strTemp);
	strTemp.Format(_T("%.2f"), WdprKBC.dLevel);
	strAry.Add(strTemp);
	strTemp.Format(_T("%.4f"), WdprKBC.dCoefFactor);
	strAry.Add(strTemp);

	if(strAry.GetSize() != COLCOUNT)
	{
		ASSERT(0);
		return;
	}

	LVITEM lvitem = {0};
	lvitem.iItem = nItemCnt;
	for(int i = 0; i < COLCOUNT; ++i)
	{
		lvitem.iSubItem = i;
		lvitem.pszText = strAry[i].GetBuffer(strAry[i].GetLength());
		lvitem.mask = LVIF_TEXT;

		if(i > 0) 
			m_lstStory.SetItem(&lvitem);
		else
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)nIndex;
			m_lstStory.InsertItem(&lvitem);		
		}
	}
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::ShowCtrlByHorCrsSectType()
{
	if (m_Data.nCodeType == KDS_W_2022)
	{
		BOOL bOctogon = m_rdoHorCrosSectType == ENM_T_OCTAGON;
		CDlgUtil::CtrlShowHide(this, m_aKDS2021_HorCrsSectOther, !bOctogon);
		CDlgUtil::CtrlShowHide(this, m_aOctagonParam, bOctogon);
	}
	BOOL IsShowCtrl = (m_rdoHorCrosSectType == ENM_T_ROUND);
	CDlgUtil::CtrlShowHide(this, m_aHorCrsSectTypeShowHideCtrl, IsShowCtrl);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::ShowCtrlBySectMemsType()
{
	BOOL IsShowCtrl = (m_rdoSectMemType == ENM_T_ROUNDED_MEMS);
	CDlgUtil::CtrlShowHide(this, m_aSectMemsTypeShowHideCtrl, IsShowCtrl);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::EnableCtrlByPlanType()
{
	int nPlanType=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLTowersPlanRdo, nPlanType);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_GRB, nPlanType==0, TRUE);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::SetData( T_WDPR_D* pData )
{
	m_Data = *pData;
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::GetData( T_WDPR_D* pData )
{
	pData->arKBC2009ForceCoef.Copy(m_Data.arKBC2009ForceCoef);
}

void CStldWindPressureAreaDlg_KBC2016_FrcCoefAuto::OnCmdGenerate()
{
	CArray<double, double> caHeight;
	CArray<T_NODE_D, T_NODE_D&> aNode;

	if(!Dlg2Data(m_DataAutoCalc))
		return;

	m_lstStory.DeleteAllItems();
	if(CWindLoadGeneratorCtrl::GetModelHeightAry(caHeight, aNode))
	{
		double dQz;
		double dCf;
		int i;
		int nSize = caHeight.GetSize();

		m_Data.arKBC2009ForceCoef.RemoveAll();
		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pDoc->m_pAttrCtrl2->GetWindQz(m_Data, caHeight[i], dQz))
			{
				AfxMessageBox(_LS(IDS_TM_WINDP_NODAL_INVALID_FAIL_AUTOCOEF));
				break;
			}

			m_DataAutoCalc.dLevel = caHeight[i];
                        
            switch ( m_Data.nCodeType )
            {
            case KBC_W_2016:
            case KDS_W_2019:
                {
                    m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalcKBC2016(m_DataAutoCalc, m_DataAutoCalc.dHc, caHeight[i], m_nExposureCategory, dQz, dCf);
                }
                break;
            case KDS_W_2022:
                {
                    CWind_KDS41_10_15_2021 WindKDS2021;
                    dCf = WindKDS2021.CalcWindForceCoeff(m_DataAutoCalc, m_DataAutoCalc.dHc, caHeight[i], m_nExposureCategory, dQz);
                }
                break;
			case ASCE7_W_2016:
			case ASCE7_W_2022:
				{
					CWind_ASCE7_16 WindASCE7(m_Data.nCodeType, CWind_ASCE7_16::EN_UNIT_US);
					dCf = WindASCE7.CalcWindForceCoeff(m_DataAutoCalc, m_DataAutoCalc.dHc, dQz, caHeight[i]);
				}
				break;
			case NSCP_W_2024:
				{
					CWind_NSCP2024 CodeCalc;
					dCf = CodeCalc.CalcWindForceCoeff(m_DataAutoCalc, m_DataAutoCalc.dHc, dQz, caHeight[i]);
				}
				break;
            default:
                ASSERT(0);
                break;
            }
			
            if(fabs(dCf) >= 1E-5)
			{
				m_DataAutoCalc.dCoefFactor = dCf;
				m_Data.arKBC2009ForceCoef.Add(m_DataAutoCalc);
			}
		}

		MakeLstItem();
	}
}