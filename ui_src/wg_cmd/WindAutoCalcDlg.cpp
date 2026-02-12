// CWindAutoCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindAutoCalcDlg.h"
#include "WindSelStoryDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_Qsort.h"
#include "..\wg_db\Wind_KDS41_10_15_2021.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CWindAutoCalcDlg::COLCOUNT = 4;

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsMatlAddDlg dialog

CWindAutoCalcDlg::CWindAutoCalcDlg(CWnd* pParent /*=NULL*/, T_WIND_D& WindD /*= T_WIND_D()*/, int nInitStructType/*=0*/)
: CDialogMove(CWindAutoCalcDlg::IDD, pParent), m_WindD(WindD)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nInitStructType = nInitStructType;

	CArray<T_STOR_K, T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);
	int nCnt = aStorK.GetSize();
	for (int i = 0; i < nCnt; ++i)
	{
		double dQZ = m_pDoc->m_pAttrCtrl2->GetWindQz(m_WindD, aStorK[i]);
		m_mapQZ.SetAt(aStorK[i], dQZ);
	}

	// 상/하층 연결정보 구성
	m_pLinkedKeyMgr_stor =  new CLinkedKeyMgr;
	m_pDoc->m_pAttrCtrl->GetStorLinkedData(*m_pLinkedKeyMgr_stor, NULL);

	T_BLDC_D DataBLDC;
	if(!m_pDoc->m_pAttrCtrl->GetBldc(DataBLDC)) DataBLDC.Initialize();
	m_dBaseLevel=0.;
	if(DataBLDC.bUseBaseLevel)
	{
		m_dBaseLevel = DataBLDC.dBaseLevel;
	}
	else // find the lowest z value;      // CIVIL인 경우와 Gen의 일부 경우
	{
		T_NODE_K key;
		T_NODE_D DataNode;
		POSITION pos;
		pos = m_pDoc->m_pAttrCtrl->GetStartNode();
		double dLowest = 1e10;
		while(pos!=NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextNode(pos,key,DataNode);
			if(dLowest > DataNode.z) dLowest = DataNode.z;
		}
		m_dBaseLevel = dLowest;
	}

	//{{AFX_DATA_INIT(CWindAutoCalcDlg)
	m_mapWnatStor.RemoveAll();
	m_edtCfx.SetEditUnit(0.0);
	m_edtCfy.SetEditUnit(0.0);
	m_strTargerStory = _T("");
	m_rdoHorCrosSectType = 0;
	m_rdoInstallType = 0;
	m_rdoSectMemType = 0;
	m_rdoPlanType = 0;
	m_rdoMemType = 0;
	//
	m_edtChimneysTSSDiaXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDiaYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDepthXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDepthYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysHeight.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_untChimneysHeight.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysArea.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dArea);
	m_untChimneysArea.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dArea);
	//
	m_edtSolSignsHeightXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolSignsHeightYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolSignsBreathXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolSignsBreathYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	//
	m_edtOpenSignsLFDiaXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtOpenSignsLFDiaYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	//
	m_edtDiaOfCicularPlan.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtHeightOfCicularPlan.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	//
	m_edtSolidWLSignsBreadthXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolidWLSignsBreadthYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolidWLSignsVerticalXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolidWLSignsVerticalYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolidWLSignsHeightXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolidWLSignsHeightYDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	//}}AFX_DATA_INIT
	
	m_aHorCrsSectTypeShowHideCtrl.RemoveAll();
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_STATIC_DEPTHXY);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_EDIT_DEPTHXY_XDIR);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_EDIT_DEPTHXY_YDIR);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_DEPTHXY_UNIT);
	m_aHorCrsSectTypeShowHideCtrl.FreeExtra();
	
	m_aKDS2021_HorCrsSectOther.RemoveAll();
	m_aKDS2021_HorCrsSectOther.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS_XDIR);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS_YDIR);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSSTSS_DIAXY);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WIND_AUTOCALC_EDIT_DIAXY_XDIR);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WIND_AUTOCALC_EDIT_DIAXY_YDIR);
	m_aKDS2021_HorCrsSectOther.Add(IDC_WIND_AUTOCALC_DIAXY_UNIT);
	m_aKDS2021_HorCrsSectOther.Append(m_aOctagonParam);

	m_aOctagonParam.RemoveAll();
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSSTSS_HEIGHT);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_EDIT_CHIMNEYSSTSS_HEIGHT);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_UNIT_CHIMNEYSSTSS_HEIGHT);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSSTSS_AREA);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_EDIT_CHIMNEYSSTSS_AREA);
	m_aOctagonParam.Add(IDC_WIND_AUTOCALC_UNIT_CHIMNEYSSTSS_AREA);

	m_aKDS2021_Octagon.RemoveAll();
	m_aKDS2021_Octagon.Add(IDC_WIND_AUTOCALC_RDO_OCTA);
	m_aKDS2021_Octagon.Append(m_aOctagonParam);
	
	m_aChimneysTSS.RemoveAll();
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS_HCSTYPE);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_RDO_SQUARE_NORMAL);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_RDO_SQUARE_DIAGONAL);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_RDO_HEX_OCTA);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_RDO_ROUND);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS_XDIR);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS_YDIR);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSSTSS_DIAXY);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_EDIT_DIAXY_XDIR);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_EDIT_DIAXY_YDIR);
	m_aChimneysTSS.Add(IDC_WIND_AUTOCALC_DIAXY_UNIT);
	m_aChimneysTSS.Append(m_aHorCrsSectTypeShowHideCtrl);
	m_aChimneysTSS.Append(m_aKDS2021_Octagon);
	m_aChimneysTSS.FreeExtra();
	
	m_aSolSigns.RemoveAll();
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLID_SIGNS_INSTALL_TYPE);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_RDO_AT_GRAOUND_LEVEL);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_RDO_ABOVE_GROUND_LEVEL);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_HEIGHT);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_BREADTH);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_XDIR);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_YDIR);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_XDIR);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_YDIR);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_SOLSIGNS_HEIGHT_UNIT);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_BREADTH_XDIR);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_BREADTH_YDIR);
	m_aSolSigns.Add(IDC_WIND_AUTOCALC_SOLSIGNS_BREADTH_UNIT);
	m_aSolSigns.FreeExtra();
	
	m_aSectMemsTypeShowHideCtrl.RemoveAll();
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_STATIC_OPENSIGNSLF_DIA);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_DIA_YDIR);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WIND_AUTOCALC_OPENSIGNSLF_DIA_UNIT);
	m_aSectMemsTypeShowHideCtrl.FreeExtra();
	
	m_aOpenSignsLF.RemoveAll();
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_STATIC_OPENSINGSLF);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_STATIC_OPENSINGSLF_SECT_MEMTYPE);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_RDO_FLAT_SIDED_MEM);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_RDO_ROUNDED_MEMS);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_STATIC_OPENSIGNSLF_XDIR);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_STATIC_OPENSIGNSLF_YDIR);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_STATIC_OPENSIGNSLF_RATIO);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_XDIR);
	m_aOpenSignsLF.Add(IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_YDIR);
	m_aOpenSignsLF.Append(m_aSectMemsTypeShowHideCtrl);
	m_aOpenSignsLF.FreeExtra();
	
	m_aTrusTowers.RemoveAll();
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_STATIC_TRUSOWERS);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_STATIC_TRUSTOWERS_PLANTYPE);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_STATIC_TRUSTOWERS_MEMTYPE);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_RDO_SQUARE_PLAN);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_RDO_TRIANGLE_PLAN);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_RDO_STEEL_ROUNDEDMEMS);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_RDO_SQUARE_CROSSECT_MEMS);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_RDO_OTHER_TYPE);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_STATIC_TRUSTOWERS_RATIO);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_STATIC_TRUSTOWERS_XDIR);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_STATIC_TRUSTOWERS_YDIR);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_EDIT_TRUSTOWERS_RATIO_XDIR);
	m_aTrusTowers.Add(IDC_WIND_AUTOCALC_EDIT_TRUSTOWERS_RATIO_YDIR);
	m_aTrusTowers.FreeExtra();

	m_aCirclePlan.RemoveAll();
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_STATIC_CIRCLE_PLAN);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_ROUGHNESS_STC);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_ROUGHNESS_CMB);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_DIA_STC);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_DIA_EDT);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_DIA_UNIT);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_STC);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_EDT);
	m_aCirclePlan.Add(IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_UNIT);
	m_aCirclePlan.FreeExtra();

	m_aSolWallSigns.RemoveAll();
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_STATIC_SOLID_WL_SIGN);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_TYPE_GRB);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_TYPE_RDO1);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_TYPE_RDO2);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_TYPE_RDO3);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_SOLID_WL_SIGN_X_DIR_STC);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_SOLID_WL_SIGN_Y_DIR_STC);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_BREADTH_STC);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_BREADTH_X_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_BREADTH_Y_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_BREADTH_UNIT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_VERTICAL_STC);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_VERTICAL_X_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_VERTICAL_Y_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_VERTICAL_UNIT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_HEIGHT_STC);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_HEIGHT_X_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_HEIGHT_Y_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_HEIGHT_UNIT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_RATIO_AREA_STC);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_RATIO_AREA_X_EDT);
	m_aSolWallSigns.Add(IDC_WIND_AUTOCALC_RATIO_AREA_Y_EDT);
	m_aSolWallSigns.FreeExtra();

	m_aFencesOnGrnd.RemoveAll();
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_STATIC_FENCEGRND);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_STC);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_X_DIR_STC);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_Y_DIR_STC);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_X_DIR_EDT);
	m_aFencesOnGrnd.Add(IDC_WIND_AUTOCALC_FENCEGRND_RATIO_Y_DIR_EDT);
	m_aFencesOnGrnd.FreeExtra();

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
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_RATIO_X_DIR_STC);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_RATIO_Y_DIR_STC);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_RATIO_X_DIR_EDT);
	m_aLatticeTowers.Add(IDC_WIND_AUTOCALC_LATOWERS_RATIO_Y_DIR_EDT);
	m_aLatticeTowers.FreeExtra();

	m_aLTowersPlanRdo.RemoveAll();
	m_aLTowersPlanRdo.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1);
	m_aLTowersPlanRdo.Add(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO2);
	m_aLTowersPlanRdo.FreeExtra();
}

CWindAutoCalcDlg::~CWindAutoCalcDlg()
{
	if (m_pLinkedKeyMgr_stor) { delete m_pLinkedKeyMgr_stor;  m_pLinkedKeyMgr_stor = NULL; }
}

void CWindAutoCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMElsPlsMatlAddDlg)
	DDX_Control(pDX, IDC_WIND_AUTOCALC_CMB_STRUCTURE_TYPE, m_cmbStructureType);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_CFX, m_edtCfx);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_CFY, m_edtCfy);
	DDX_Text(pDX, IDC_WIND_AUTOCALC_EDIT_TARGET_STORY, m_strTargerStory);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_LIST_STORY, m_lstStory);
	
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_RDO_SQUARE_NORMAL, m_rdoHorCrosSectType);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_DIAXY_XDIR, m_edtChimneysTSSDiaXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_DIAXY_YDIR, m_edtChimneysTSSDiaYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_DIAXY_UNIT, m_unitChimneysTSSDia);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_DEPTHXY_XDIR, m_edtChimneysTSSDepthXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_DEPTHXY_YDIR, m_edtChimneysTSSDepthYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_DEPTHXY_UNIT, m_unitChimneysTSSDepth);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_CHIMNEYSSTSS_HEIGHT, m_edtChimneysHeight);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_UNIT_CHIMNEYSSTSS_HEIGHT, m_untChimneysHeight);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_CHIMNEYSSTSS_AREA, m_edtChimneysArea);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_UNIT_CHIMNEYSSTSS_AREA, m_untChimneysArea);

	DDX_Text(pDX, IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_HEIGHT, m_txtSolSignsHeight);
	DDX_Text(pDX, IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_BREADTH, m_txtSolSignsBreath);
	
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_RDO_AT_GRAOUND_LEVEL, m_rdoInstallType);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_XDIR, m_edtSolSignsHeightXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_YDIR, m_edtSolSignsHeightYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_SOLSIGNS_HEIGHT_UNIT, m_unitSolSignsHeight);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_BREADTH_XDIR, m_edtSolSignsBreathXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_SOLSIGNS_BREADTH_YDIR, m_edtSolSignsBreathYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_SOLSIGNS_BREADTH_UNIT, m_unitSolSignsBreath);
	
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_RDO_FLAT_SIDED_MEM, m_rdoSectMemType);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_XDIR, m_edtOpenSignsLFRatioXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_YDIR, m_edtOpenSignsLFRatioYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR, m_edtOpenSignsLFDiaXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_OPENSIGNSLF_DIA_YDIR, m_edtOpenSignsLFDiaYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_OPENSIGNSLF_DIA_UNIT, m_unitOpenSignsLFDia);
	
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_RDO_SQUARE_PLAN, m_rdoPlanType);
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_RDO_STEEL_ROUNDEDMEMS, m_rdoMemType);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_TRUSTOWERS_RATIO_XDIR, m_edtTrusTowersRatioXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_EDIT_TRUSTOWERS_RATIO_YDIR, m_edtTrusTowersRatioYDir);

	DDX_Control(pDX, IDC_WIND_AUTOCALC_ROUGHNESS_CMB, m_cmbRoughness);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_DIA_EDT, m_edtDiaOfCicularPlan);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_DIA_UNIT, m_unitDiaOfCicularPlan);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_EDT, m_edtHeightOfCicularPlan);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_CIRCLE_HEIGHT_UNIT, m_unitHeightOfCicularPlan);

	DDX_Radio(pDX, IDC_WIND_AUTOCALC_TYPE_RDO1, m_rdoSolidWLSignsType);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_BREADTH_X_EDT, m_edtSolidWLSignsBreadthXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_BREADTH_Y_EDT, m_edtSolidWLSignsBreadthYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_BREADTH_UNIT, m_unitSolidWLSignsBreadth);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_VERTICAL_X_EDT, m_edtSolidWLSignsVerticalXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_VERTICAL_Y_EDT, m_edtSolidWLSignsVerticalYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_VERTICAL_UNIT, m_unitSolidWLSignsVertical);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_HEIGHT_X_EDT, m_edtSolidWLSignsHeightXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_HEIGHT_Y_EDT, m_edtSolidWLSignsHeightYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_HEIGHT_UNIT, m_unitSolidWLSignsHeight);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_RATIO_AREA_X_EDT, m_edtSolidWLSignsRatioXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_RATIO_AREA_Y_EDT, m_edtSolidWLSignsRatioYDir);

	DDX_Radio(pDX, IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1, m_rdoLTowersPlanType);
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_LATOWERS_MEMBTYPE_RDO1, m_rdoLTowersMembType);
	DDX_Radio(pDX, IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_RDO1, m_rdoLTowersWindDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_LATOWERS_RATIO_X_DIR_EDT, m_edtLTowersRatioXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_LATOWERS_RATIO_Y_DIR_EDT, m_edtLTowersRatioYDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_FENCEGRND_RATIO_X_DIR_EDT, m_edtFenceGrndRatioXDir);
	DDX_Control(pDX, IDC_WIND_AUTOCALC_FENCEGRND_RATIO_Y_DIR_EDT, m_edtFenceGrndRatioYDir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindAutoCalcDlg, CDialogMove)
//{{AFX_MSG_MAP(CCMElsPlsMatlAddDlg)
ON_CBN_SELCHANGE(IDC_WIND_AUTOCALC_CMB_STRUCTURE_TYPE, OnSelchangeTypeCombo)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_WIND_AUTOCALC_LIST_STORY, OnSelchangeStoryListCtrl)
ON_NOTIFY(LVN_KEYDOWN, IDC_WIND_AUTOCALC_LIST_STORY, OnStoryListKeyDown)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_BTN_SEL_STORY_DLG, OnSelectStoryDlgCall)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_BTN_APPLY_REPLACE, OnTgtStorAddReplace)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_BTN_DELETE, OnSelStorDelete)
// ChimneysTSS
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_SQUARE_NORMAL, OnRdoClikedChimneysTSS)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_SQUARE_DIAGONAL, OnRdoClikedChimneysTSS)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_HEX_OCTA, OnRdoClikedChimneysTSS)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_ROUND, OnRdoClikedChimneysTSS)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_OCTA, OnRdoClikedChimneysTSS)
// SolSigns
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_AT_GRAOUND_LEVEL, OnRdoClikedSolSigns)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_ABOVE_GROUND_LEVEL, OnRdoClikedSolSigns)
// OpenSignsLF
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_FLAT_SIDED_MEM, OnRdoClikedOpenSignsLF)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_RDO_ROUNDED_MEMS, OnRdoClikedOpenSignsLF)
// Lattice Towers
ON_BN_CLICKED(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO1, OnRdoClikedLTowersPlan)
ON_BN_CLICKED(IDC_WIND_AUTOCALC_LATOWERS_PLANTYPE_RDO2, OnRdoClikedLTowersPlan)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindAutoCalcDlg message handlers
BOOL CWindAutoCalcDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	InitCtrls();

	T_WNAT_STOR WnatStorD;
	WnatStorD.Initialize();
	WnatStorD.nStructType = m_nInitStructType;
	if(m_nInitStructType==ENM_T_CIRCLE_PLAN)
	{
        switch ( m_WindD.nCodeType )
        {
        case KBC_W_2016:
        case KDS_W_2019:
            {
                WnatStorD.dXDir[0] = 0.75;
                WnatStorD.dXDir[2] = m_WindD.GetKBC2015()->dRoofHeight;
            }
            break;
        case KDS_W_2022:
            {
                WnatStorD.dXDir[0] = 0.75;
                WnatStorD.dXDir[2] = m_WindD.CodeParam.KDS2021.dRoofHeight;
            }
            break;
        default:
            ASSERT(0); 
            return FALSE;
        }
	}
	if (m_WindD.nCodeType == KDS_W_2022)
	{
		WnatStorD.dHeight = m_WindD.CodeParam.KDS2021.dRoofHeight;
	}
	Data2Dlg(WnatStorD);

	SetDlgByComb();
	
	SetLstHeaderTitle();
	MakeLstItem();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWindAutoCalcDlg::OnOK() 
{
	// TODO: Add extra validation here
	CDialogMove::OnOK();
}

void CWindAutoCalcDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}


void CWindAutoCalcDlg::OnSelchangeTypeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	T_WNAT_STOR WnatStorD;
	WnatStorD.Initialize();
	
	WnatStorD.nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	Data2Dlg(WnatStorD);

	SetDlgByComb();
	
	UpdateData(FALSE);
}

void CWindAutoCalcDlg::OnSelchangeStoryListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//if (!(pNMLV->uChanged == LVIF_STATE && (pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)))) return;
	BOOL IsOriginSel = (pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED));
	if (!(pNMLV->uChanged == LVIF_STATE && (IsOriginSel || (pNMLV->uNewState == LVIS_SELECTED)))) return; 
	
	int nSelectItem = pNMLV->iItem;
	if (nSelectItem == -1) return;
	T_STOR_K StorK = (T_STOR_K)pNMLV->lParam;

	Data2Dlg(m_mapWnatStor[StorK]);
	m_aTgtStorK.RemoveAll();
	int nCnt = m_lstStory.GetSelectedCount();
	m_aTgtStorK.SetSize(nCnt);
	POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
	int nIndex = 0;
	while (pos != NULL)
	{
		int nSelItem = m_lstStory.GetNextSelectedItem(pos);
		LVITEM lvItem = {0};
		lvItem.iItem = nSelItem;
		lvItem.mask = (LVIF_TEXT | LVIF_PARAM);
		m_lstStory.GetItem(&lvItem);
		T_STOR_K nKey = (T_STOR_K)lvItem.lParam;
		m_aTgtStorK.SetAt(nIndex++, nKey);
	}
	SetDlgByComb();

double dCfx = 0.0;
double dCfy = 0.0;
	
	switch (m_WindD.nCodeType)
	{
	case KBC_W_2016:
	case KDS_W_2019:
	{
		const double dUpperStorLevel = GetUpperStorLevel(StorK);
		m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalcKBC2015(*m_WindD.GetKBC2015(), m_mapWnatStor[StorK], m_mapQZ[StorK], dUpperStorLevel, dCfx, dCfy);
	}
	break;
	case KDS_W_2022:
	{
		const double dUpperStorLevel = GetUpperStorLevel(StorK);
		T_RES_KDS41_10_15_2021 WindRes;
		CWind_KDS41_10_15_2021 WindCalc;
		WindCalc.CalcWindForceCoeff(m_WindD.CodeParam.KDS2021, m_mapWnatStor[StorK], m_mapQZ[StorK], dUpperStorLevel, dCfx, dCfy);
	}
	break;
	case IBC_W_2000:
	case KBC_W_2009:
		m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalc(m_mapWnatStor[StorK], m_mapQZ[StorK], dCfx, dCfy);
		break;
	default:
		ASSERT(0);
		return;
	}

m_edtCfx.SetEditUnit(dCfx);
m_edtCfy.SetEditUnit(dCfy);

MakeTgtEditStory();
UpdateData(FALSE);
}

void CWindAutoCalcDlg::OnStoryListKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	/* ESC가 눌러졌을때 처리 */
	if (pLKeyDown->wVKey == VK_ESCAPE)
	{
		int nCnt = m_lstStory.GetSelectedCount();
		if (nCnt > 0)
		{
			T_WNAT_STOR WnatStorD;
			WnatStorD.Initialize();
			WnatStorD.nStructType = m_nInitStructType;

			Data2Dlg(WnatStorD);

			SetDlgByComb();

			POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
			while (pos != NULL)
			{
				int nSelItem = m_lstStory.GetNextSelectedItem(pos);
				m_lstStory.SetItemState(nSelItem, FALSE, LVIS_SELECTED | LVIS_FOCUSED);
			}
			m_aTgtStorK.RemoveAll();
			MakeTgtEditStory();

			m_edtCfx.SetEditUnit(0.0);
			m_edtCfy.SetEditUnit(0.0);

			UpdateData(FALSE);
		}
	}
}

void CWindAutoCalcDlg::OnSelectStoryDlgCall()
{
	CWindSelStoryDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_aTgtStorK.RemoveAll();
		dlg.GetSelStorK(m_aTgtStorK);
		MakeTgtEditStory();
		m_edtCfx.SetEditUnit(0.0);
		m_edtCfy.SetEditUnit(0.0);
		SetDlgItemText(IDC_WIND_AUTOCALC_EDIT_TARGET_STORY, m_strTargerStory);
	}
}

void CWindAutoCalcDlg::OnTgtStorAddReplace()
{
	int nCnt = m_aTgtStorK.GetSize();
	if (nCnt < 1) return;

	T_WNAT_STOR WnatStorD;
	Dlg2Data(WnatStorD);

	if (WnatStorD.nStructType == ENM_T_CHIMNEYSTSS || WnatStorD.nStructType == ENM_T_ROOF_ST)
	{
		if (WnatStorD.nSubType1 == 4)
		{
			if(WnatStorD.dHeight <= 0.0)
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Height_Spire), MB_OK);
				return;
			}
			if (WnatStorD.dArea <= 0.0)
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Area_Floor), MB_OK);
				return;
			}
		}
		else
		{
			if (WnatStorD.dXDir[0] <= 0.0 || WnatStorD.dYDir[0] <= 0.0)
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Dim), MB_OK);
				return;
			}
		}
		
	}
	else if (WnatStorD.nStructType == ENM_T_SOLSIGNS)
	{
		if(WnatStorD.dXDir[1] <= 0.0 || WnatStorD.dYDir[1] <= 0.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Signs),MB_OK);
			return;
		}
	}
	else if(WnatStorD.nStructType == ENM_T_OPENSIGNSLF)
	{
		if(WnatStorD.dXDir[0] > 0.7 || WnatStorD.dYDir[0] > 0.7) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Solid),MB_OK);
			return;
		}
	}
	else if(WnatStorD.nStructType == ENM_T_CIRCLE_PLAN)
	{
		if(WnatStorD.dXDir[0] < 0.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Roughness),MB_OK);
			return;
		}
		if(WnatStorD.dXDir[1] <= 0.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Diameter),MB_OK);
			return;
		}
		if(WnatStorD.dXDir[2] < 0.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_Height),MB_OK);
			return;
		}

		double dRatioHD = WnatStorD.dXDir[2]/WnatStorD.dXDir[1];
		if(dRatioHD>8.0) // H/D 값은 8을 초과할 수 없음
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_HD),MB_OK);
			return;
		}
	}
	else if(WnatStorD.nStructType == ENM_T_SOLID_WL_SIGNS)
	{
		if(WnatStorD.dXDir[0] > 1.0 || WnatStorD.dYDir[0] > 1.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Solid_Wall_Sign),MB_OK);
			return;
		}
	}
	else if(WnatStorD.nStructType == ENM_T_LATTICE_TOWERS)
	{
		if(WnatStorD.dXDir[0] > 0.6 || WnatStorD.dYDir[0] > 0.6) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Lattice),MB_OK);
			return;
		}
	}
	else if(WnatStorD.nStructType == ENM_T_FENCES_ON_GRND)
	{
		if(WnatStorD.dXDir[0] > 1.0 || WnatStorD.dYDir[0] > 1.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Fence),MB_OK);
			return;
		}
	}
	
	for(int i = 0; i < 4; ++i)
	{
		if(WnatStorD.dXDir[i] < 0.0 || WnatStorD.dYDir[i] < 0.0) 
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir),MB_OK);
			return;
		}
	}

	for (int i = 0; i < nCnt; ++i)
	{
		WnatStorD.KeyStor = m_aTgtStorK[i];
		m_mapWnatStor.SetAt(m_aTgtStorK[i], WnatStorD);
	}
		
	/* ListView update */
	MakeLstItem();
}

void CWindAutoCalcDlg::OnSelStorDelete()
{
	POSITION pos;
	while (pos = m_lstStory.GetFirstSelectedItemPosition())
	{
		int nSelItem = m_lstStory.GetNextSelectedItem(pos);
		LVITEM lvItem = {0};
		lvItem.iItem = nSelItem;	
		lvItem.mask = (LVIF_TEXT | LVIF_PARAM);
		m_lstStory.GetItem(&lvItem);
		T_STOR_K nStorK = (T_STOR_K)lvItem.lParam;
		m_mapWnatStor.RemoveKey(nStorK);
		m_lstStory.DeleteItem(nSelItem);
	}
}

void CWindAutoCalcDlg::OnRdoClikedChimneysTSS() 
{
	UpdateData(TRUE);
	ShowCtrlByHorCrsSectType();
}

void CWindAutoCalcDlg::OnRdoClikedSolSigns()
{
	UpdateData(TRUE);
	ShowCtrlByInstallType();
}

void CWindAutoCalcDlg::OnRdoClikedOpenSignsLF() 
{
	UpdateData(TRUE);
	ShowCtrlBySectMemsType();
}

void CWindAutoCalcDlg::OnRdoClikedLTowersPlan() 
{
	EnableCtrlByPlanType();
}

/////////////////////////////////////////////////////////////////////////////
// CWindAutoCalcDlg member method
void CWindAutoCalcDlg::InitCtrls()
{	
	AlignControl();	
	// UnitEdit - None Type
	
	// Structure Combo
	m_cmbStructureType.ResetContent();
	int nIndex = 0;

	if(m_nInitStructType<0)
	{
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_SOLSIGNS);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
		nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_TRUSTOWERS);
		m_nInitStructType = 0;
	}
	else
	{
		CString strTitle = _T("");
		switch (m_nInitStructType)
		{
		case ENM_T_CIRCLE_PLAN:     strTitle = _LS(IDS_CMD_WIND_WNAT_CIRCLE_PLAN); break;
		case ENM_T_SOLID_WL_SIGNS:  strTitle = _LS(IDS_CMD_WIND_WNAT_SOLID_WALLS_SIGNS); break;
		case ENM_T_LATTICE_TOWERS:  strTitle = _LS(IDS_CMD_WIND_WNAT_LATTICE_TOWERS); break;
		case ENM_T_OPENSIGNSLF:     strTitle = _LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS); break;
		case ENM_T_CHIMNEYSTSS:     strTitle = _LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES); break;
		case ENM_T_ROOF_ST:         strTitle = _LS(IDS_CMD_WIND_WNAT_ROOF_STRUCTURES); break;
		case ENM_T_FENCES_ON_GRND:  strTitle = _LS(IDS_CMD_WIND_WNAT_FENCES_ON_GROUND); break;
		default: ASSERT(0); break;
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
}
void CWindAutoCalcDlg::Data2Dlg(const T_WNAT_STOR& WnatStorD)
{
	int nSelect = -1;
	int nCount = m_cmbStructureType.GetCount();
	for(int nIndex=0; nIndex<nCount; nIndex++)
	{
		if(WnatStorD.nStructType != m_cmbStructureType.GetItemData(nIndex)) continue;
		nSelect = nIndex;
		break;
	}
	m_cmbStructureType.SetCurSel(nSelect);

	int nSubType1 = WnatStorD.nSubType1;
	int nSubType2 = WnatStorD.nSubType2;
	int nSubType3 = WnatStorD.nSubType3;
	double dXDir[4] = {WnatStorD.dXDir[0], WnatStorD.dXDir[1], WnatStorD.dXDir[2], WnatStorD.dXDir[3]};
	double dYDir[4] = {WnatStorD.dYDir[0], WnatStorD.dYDir[1], WnatStorD.dYDir[2], WnatStorD.dYDir[3]};

	switch(WnatStorD.nStructType)
	{
		case ENM_T_CHIMNEYSTSS:			m_rdoHorCrosSectType = nSubType1; break;
		case ENM_T_SOLSIGNS:				m_rdoInstallType = nSubType1; break;
		case ENM_T_OPENSIGNSLF:			m_rdoSectMemType = nSubType1; break;
		case ENM_T_TRUSTOWERS:			m_rdoPlanType = nSubType1; m_rdoMemType  = nSubType2; break;
		case ENM_T_CIRCLE_PLAN:			break;
		case ENM_T_SOLID_WL_SIGNS:	m_rdoSolidWLSignsType = nSubType1; break;
		case ENM_T_LATTICE_TOWERS:	m_rdoLTowersPlanType = nSubType1; m_rdoLTowersMembType = nSubType2; m_rdoLTowersWindDir = nSubType3; break;
		case ENM_T_ROOF_ST:					m_rdoHorCrosSectType = nSubType1; break;
		case ENM_T_FENCES_ON_GRND:	break;
	}

	if(WnatStorD.nStructType==ENM_T_CIRCLE_PLAN)
	{
		CString strValue = _T("");
		if (dXDir[0] == 0.75  || dXDir[0] == 0.9  || dXDir[0] == 1.)
		{
			strValue.Format(_T("%g"), dXDir[0]);
			m_cmbRoughness.SelectString(-1, strValue);
		}
		else
		{
			strValue.Format(_T("%g"), dXDir[0]);
			m_cmbRoughness.SetWindowText(strValue);
		}
	}
	
	m_edtChimneysTSSDiaXDir.SetEditUnit(dXDir[0]);
	m_edtChimneysTSSDiaYDir.SetEditUnit(dYDir[0]);
	m_edtChimneysTSSDepthXDir.SetEditUnit(dXDir[1]);
	m_edtChimneysTSSDepthYDir.SetEditUnit(dYDir[1]);
	m_edtChimneysHeight.SetEditUnit(WnatStorD.dHeight);
	m_edtChimneysArea.SetEditUnit(WnatStorD.dArea);

	m_edtSolSignsHeightXDir.SetEditUnit(dXDir[0]); 
	m_edtSolSignsHeightYDir.SetEditUnit(dYDir[0]); 
	m_edtSolSignsBreathXDir.SetEditUnit(dXDir[1]); 
	m_edtSolSignsBreathYDir.SetEditUnit(dYDir[1]); 
	
	m_edtOpenSignsLFRatioXDir.SetEditUnit(dXDir[0]);
	m_edtOpenSignsLFRatioYDir.SetEditUnit(dYDir[0]);
	m_edtOpenSignsLFDiaXDir.SetEditUnit(dXDir[1]);
	m_edtOpenSignsLFDiaYDir.SetEditUnit(dYDir[1]);
	
	m_edtTrusTowersRatioXDir.SetEditUnit(dXDir[0]); 
	m_edtTrusTowersRatioYDir.SetEditUnit(dYDir[0]);

	m_edtDiaOfCicularPlan.SetEditUnit(dXDir[1]);
	m_edtHeightOfCicularPlan.SetEditUnit(dXDir[2]);

	m_edtSolidWLSignsBreadthXDir.SetEditUnit(dXDir[0]);
	m_edtSolidWLSignsBreadthYDir.SetEditUnit(dYDir[0]);
	m_edtSolidWLSignsVerticalXDir.SetEditUnit(dXDir[1]);
	m_edtSolidWLSignsVerticalYDir.SetEditUnit(dYDir[1]);
	m_edtSolidWLSignsHeightXDir.SetEditUnit(dXDir[2]);
	m_edtSolidWLSignsHeightYDir.SetEditUnit(dYDir[2]);
	m_edtSolidWLSignsRatioXDir.SetEditUnit(dXDir[3]);
	m_edtSolidWLSignsRatioYDir.SetEditUnit(dYDir[3]);

	m_edtLTowersRatioXDir.SetEditUnit(dXDir[0]);
	m_edtLTowersRatioYDir.SetEditUnit(dYDir[0]);

	m_edtFenceGrndRatioXDir.SetEditUnit(dXDir[0]);
	m_edtFenceGrndRatioYDir.SetEditUnit(dYDir[0]);
	
	UpdateData(FALSE);
}
void CWindAutoCalcDlg::Dlg2Data(T_WNAT_STOR& WnatStorD)
{	
	UpdateData(TRUE);
	
	WnatStorD.nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());
	
	int nSubType1 = 0;
	int nSubType2 = 0;
	int nSubType3 = 0;
	double dXDir[4] = {0., };
	double dYDir[4] = {0., };
	double dHeight = 0.;
	double dArea = 0.;
	switch(WnatStorD.nStructType)
	{
		case ENM_T_CHIMNEYSTSS:
		case ENM_T_ROOF_ST:
			{
				nSubType1 = m_rdoHorCrosSectType; 
				dXDir[0]	= m_edtChimneysTSSDiaXDir.GetEditValue();
				dYDir[0]	= m_edtChimneysTSSDiaYDir.GetEditValue();
				dXDir[1]	= m_edtChimneysTSSDepthXDir.GetEditValue();
				dYDir[1]	= m_edtChimneysTSSDepthYDir.GetEditValue();
				dHeight     = m_edtChimneysHeight.GetEditValue();
				dArea       = m_edtChimneysArea.GetEditValue();
				break;
			}
		case ENM_T_SOLSIGNS:			
			{
				nSubType1 = m_rdoInstallType; 
				dXDir[0]	= m_edtSolSignsHeightXDir.GetEditValue();
				dYDir[0]	= m_edtSolSignsHeightYDir.GetEditValue();
				dXDir[1]	= m_edtSolSignsBreathXDir.GetEditValue();
				dYDir[1]	= m_edtSolSignsBreathYDir.GetEditValue();
				break;
			}
		case ENM_T_OPENSIGNSLF:		
			{
				nSubType1 = m_rdoSectMemType; 
				dXDir[0]	= m_edtOpenSignsLFRatioXDir.GetEditValue();
				dYDir[0]	= m_edtOpenSignsLFRatioYDir.GetEditValue();
				dXDir[1]	= m_edtOpenSignsLFDiaXDir.GetEditValue();
				dYDir[1]	= m_edtOpenSignsLFDiaYDir.GetEditValue();
				break;
			}
		case ENM_T_TRUSTOWERS:		
			{
				nSubType1 = m_rdoPlanType; 
				nSubType2 = m_rdoMemType; 
				dXDir[0]	= m_edtTrusTowersRatioXDir.GetEditValue();
				dYDir[0]	= m_edtTrusTowersRatioYDir.GetEditValue();
				break;
			}
		case ENM_T_CIRCLE_PLAN:
			{
				CString strValue=_T("");
				m_cmbRoughness.GetWindowText(strValue);
				dXDir[0]	= _tstof(strValue);
				dXDir[1]	= m_edtDiaOfCicularPlan.GetEditValue();
				dXDir[2]	= m_edtHeightOfCicularPlan.GetEditValue();
				break;
			}
		case ENM_T_SOLID_WL_SIGNS:		
			{
				nSubType1 = m_rdoSolidWLSignsType; 
				dXDir[0]	= m_edtSolidWLSignsBreadthXDir.GetEditValue();
				dYDir[0]	= m_edtSolidWLSignsBreadthYDir.GetEditValue();
				dXDir[1]	= m_edtSolidWLSignsVerticalXDir.GetEditValue();
				dYDir[1]	= m_edtSolidWLSignsVerticalYDir.GetEditValue();
				dXDir[2]	= m_edtSolidWLSignsHeightXDir.GetEditValue();
				dYDir[2]	= m_edtSolidWLSignsHeightYDir.GetEditValue();
				dXDir[3]	= m_edtSolidWLSignsRatioXDir.GetEditValue();
				dYDir[3]	= m_edtSolidWLSignsRatioYDir.GetEditValue();
				break;
			}
		case ENM_T_LATTICE_TOWERS:		
			{
				nSubType1 = m_rdoLTowersPlanType;
				nSubType2 = m_rdoLTowersMembType;
				nSubType3 = m_rdoLTowersWindDir;
				dXDir[0]	= m_edtLTowersRatioXDir.GetEditValue();
				dYDir[0]	= m_edtLTowersRatioYDir.GetEditValue();
				break;
			}
		case ENM_T_FENCES_ON_GRND:		
			{
				dXDir[0]	= m_edtFenceGrndRatioXDir.GetEditValue();
				dYDir[0]	= m_edtFenceGrndRatioYDir.GetEditValue();
				break;
			}
	}
	
	WnatStorD.nSubType1 = nSubType1;
	WnatStorD.nSubType2 = nSubType2;
	WnatStorD.nSubType3 = nSubType3;
	WnatStorD.dXDir[0] = dXDir[0]; 
	WnatStorD.dYDir[0] = dYDir[0];
	WnatStorD.dXDir[1] = dXDir[1];
	WnatStorD.dYDir[1] = dYDir[1];
	WnatStorD.dXDir[2] = dXDir[2]; 
	WnatStorD.dYDir[2] = dYDir[2];
	WnatStorD.dXDir[3] = dXDir[3];
	WnatStorD.dYDir[3] = dYDir[3];
	WnatStorD.dHeight  = dHeight;
	WnatStorD.dArea    = dArea;
}
void CWindAutoCalcDlg::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;
	
	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS)->GetWindowRect(rRef);

	/** Resource 작업 후 제거 **/ 
	this->GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_HEIGHT)->GetWindowRect(rToMove);
	this->ScreenToClient(rToMove);
	rToMove.right += 20;
	this->GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_HEIGHT)->MoveWindow(rToMove);

	this->GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_BREADTH)->GetWindowRect(rToMove);
	this->ScreenToClient(rToMove);
	rToMove.right += 20;
	this->GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS_BREADTH)->MoveWindow(rToMove);
	/** Resource 작업 후 제거 **/ 

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLSIGNS)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSolSigns, nDistY);
	
	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_OPENSINGSLF)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aOpenSignsLF, nDistY);
	
	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_TRUSOWERS)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aTrusTowers, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_CIRCLE_PLAN)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aCirclePlan, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_SOLID_WL_SIGN)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSolWallSigns, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_LATOWERS)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aLatticeTowers, nDistY);

	GetDlgItem(IDC_WIND_AUTOCALC_STATIC_FENCEGRND)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aFencesOnGrnd, nDistY);
	
	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	
	//MoveWindow(r);
	SetWindowPos(NULL, r.left, r.top, r.Width(), r.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CWindAutoCalcDlg::SetDlgByComb()
{
	int nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	CDlgUtil::CtrlShowHide(this, m_aChimneysTSS   , nStructType==ENM_T_CHIMNEYSTSS || nStructType==ENM_T_ROOF_ST);
	CDlgUtil::CtrlShowHide(this, m_aSolSigns      , nStructType==ENM_T_SOLSIGNS);
	CDlgUtil::CtrlShowHide(this, m_aOpenSignsLF   , nStructType==ENM_T_OPENSIGNSLF);
	CDlgUtil::CtrlShowHide(this, m_aTrusTowers    , nStructType==ENM_T_TRUSTOWERS);
	CDlgUtil::CtrlShowHide(this, m_aCirclePlan    , nStructType==ENM_T_CIRCLE_PLAN);
	CDlgUtil::CtrlShowHide(this, m_aSolWallSigns  , nStructType==ENM_T_SOLID_WL_SIGNS);
	CDlgUtil::CtrlShowHide(this, m_aLatticeTowers , nStructType==ENM_T_LATTICE_TOWERS);
	CDlgUtil::CtrlShowHide(this, m_aFencesOnGrnd	, nStructType==ENM_T_FENCES_ON_GRND);
	
	switch(nStructType)
	{
	case ENM_T_CHIMNEYSTSS:
	case ENM_T_ROOF_ST:
		{
			CDlgUtil::CtrlShowHide(this, m_aKDS2021_Octagon, m_WindD.nCodeType == KDS_W_2022);

			ShowCtrlByHorCrsSectType();
			int nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());
			CString strTitle = nStructType==ENM_T_CHIMNEYSTSS? _LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES) : _LS(IDS_CMD_WIND_WNAT_ROOF_STRUCTURES);
			GetDlgItem(IDC_WIND_AUTOCALC_STATIC_CHIMNEYSTSS)->SetWindowText(strTitle);
			break;
		}
	case ENM_T_SOLSIGNS:
		{
			ShowCtrlByInstallType();
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
	}
}

void CWindAutoCalcDlg::SetLstHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_CMD_WIND_WNAT_STORY), _LS(IDS_CMD_WIND_WNAT_HEIGHT), _LS(IDS_CMD_WIND_WNAT_CFX), _LS(IDS_CMD_WIND_WNAT_CFY) };
	int nColWidth[COLCOUNT] = {90, 90, 70, 70};
	
	m_lstStory.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	for(int i = 0; i < COLCOUNT; ++i)
	{
		CString strTitle = aTitle[i];
		LV_COLUMN lvCol;
		lvCol.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvCol.fmt = (i == 0 ? LVCFMT_LEFT : LVCFMT_RIGHT); // _T("Story")만 left align
		lvCol.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvCol.pszText = strTitle.GetBuffer(0);
		m_lstStory.InsertColumn(i,&lvCol);
		strTitle.ReleaseBuffer();
	}
}

void CWindAutoCalcDlg::MakeLstItem()
{
	m_lstStory.DeleteAllItems();
	
	CArray<T_STOR_K, T_STOR_K> aStorK;
	CArray<double, double> aStorLevel;
		
	POSITION pos = m_mapWnatStor.GetStartPosition();
	while (pos != NULL)
	{
		T_STOR_K StorK;
		T_WNAT_STOR WnatStorD;
		m_mapWnatStor.GetNextAssoc(pos, StorK, WnatStorD);
		T_STOR_D StorD;
		if(!m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD)) continue;
		if(m_nInitStructType!=WnatStorD.nStructType && m_cmbStructureType.GetCount()==1) continue;

        switch ( m_WindD.nCodeType )
        {
        case KBC_W_2016:
        case KDS_W_2019:
        case KDS_W_2022:
            {
                int nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());
                if ( nStructType!=WnatStorD.nStructType ) { continue; }
            }            
            break;
		case IBC_W_2000:
		case KBC_W_2009:
			break;
        default:
            ASSERT(0);
            break;
        }
		
		aStorK.Add(StorK);
		aStorLevel.Add(StorD.dStoryLevel);
	}	

	int nStorKCnt = aStorK.GetSize() ;
	if(nStorKCnt < 1) return;

	// level에 따라 오름차순 key 정렬
	CQSort::QSortDblWithIndex(aStorK.GetData(), aStorLevel.GetData(), nStorKCnt);
	
	// 내림차순으로 ListView에 출력
	for (int i = nStorKCnt - 1; i >= 0; --i)
	{
		InsertItem(m_mapWnatStor[aStorK[i]]);
	}
}

void CWindAutoCalcDlg::InsertItem(const T_WNAT_STOR& WnatStorD)
{
	T_STOR_D StorD;
	if(!m_pDoc->m_pAttrCtrl->GetStor(WnatStorD.KeyStor, StorD)) return;
	
	CString aValue[] = {StorD.StoryName, _T(""), _T(""), _T("") };
	double dHeight = m_pDoc->m_pAttrCtrl2->GetStorHeight(WnatStorD.KeyStor);
	aValue[1].Format(_T("%.2f"), dHeight);
	double dCfx = 0.0;
	double dCfy = 0.0;
	
        switch ( m_WindD.nCodeType )
        {
        case KBC_W_2016:
        case KDS_W_2019:
            {
                double dUpperStorLevel = GetUpperStorLevel(WnatStorD.KeyStor);
                m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalcKBC2015(*m_WindD.GetKBC2015(), WnatStorD, m_mapQZ[WnatStorD.KeyStor], dUpperStorLevel, dCfx, dCfy);
            }
            break;
        case KDS_W_2022:
            {
                double dUpperStorLevel = GetUpperStorLevel(WnatStorD.KeyStor);
                CWind_KDS41_10_15_2021 WindCalc;
                WindCalc.CalcWindForceCoeff(m_WindD.CodeParam.KDS2021, WnatStorD, m_mapQZ[WnatStorD.KeyStor], dUpperStorLevel, dCfx, dCfy);                
            }
            break;
	case IBC_W_2000:
	case KBC_W_2009:
		m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalc(WnatStorD, m_mapQZ[WnatStorD.KeyStor], dCfx, dCfy);
		break;
        default:
            ASSERT(0);
            return;            
        }		

	aValue[2].Format(_T("%.2f"), dCfx);
	aValue[3].Format(_T("%.2f"), dCfy);
	
	LVITEM lvitem = {0};
	lvitem.iItem = m_lstStory.GetItemCount();
	for(int i = 0; i < COLCOUNT; ++i)
	{
		lvitem.iSubItem = i;
		lvitem.pszText = aValue[i].GetBuffer(0);
		lvitem.mask = LVIF_TEXT;
		
		if(i > 0) 
			m_lstStory.SetItem(&lvitem);
		else
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)WnatStorD.KeyStor;
			m_lstStory.InsertItem(&lvitem);		
		}
	}
}

void CWindAutoCalcDlg::ShowCtrlByHorCrsSectType()
{
	if (m_WindD.nCodeType == KDS_W_2022)
	{
		BOOL bOctogon = m_rdoHorCrosSectType == ENM_T_OCTAGON;
		CDlgUtil::CtrlShowHide(this, m_aKDS2021_HorCrsSectOther, !bOctogon);
		CDlgUtil::CtrlShowHide(this, m_aOctagonParam, bOctogon);
	}
	
	BOOL IsShowCtrl = (m_rdoHorCrosSectType == ENM_T_ROUND);
	CDlgUtil::CtrlShowHide(this, m_aHorCrsSectTypeShowHideCtrl, IsShowCtrl);

}
void CWindAutoCalcDlg::ShowCtrlByInstallType()
{
	
	if(m_rdoInstallType == ENM_T_AT_GR_LEVEL) 
	{
		m_txtSolSignsHeight = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_HEIGHT);
		m_txtSolSignsBreath = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_BREADTH);
		UpdateData(FALSE);
	}
	else if (m_rdoInstallType == ENM_T_ABOVE_GR_LEVEL)
	{
		m_txtSolSignsHeight = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_LARGER_DIM);
		m_txtSolSignsBreath = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_SMALLER_DIM);
		UpdateData(FALSE);
	}
}
void CWindAutoCalcDlg::ShowCtrlBySectMemsType()
{
	BOOL IsShowCtrl = (m_rdoSectMemType == ENM_T_ROUNDED_MEMS);
	CDlgUtil::CtrlShowHide(this, m_aSectMemsTypeShowHideCtrl, IsShowCtrl);
}

void CWindAutoCalcDlg::EnableCtrlByPlanType()
{
	int nPlanType=0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aLTowersPlanRdo, nPlanType);
	CDlgUtil::CtrlEnableDisableByRect(this, IDC_WIND_AUTOCALC_LATOWERS_WINDDIR_GRB, nPlanType==0, TRUE);
}

void CWindAutoCalcDlg::MakeTgtEditStory()
{
	m_strTargerStory.Empty();
	int nCnt = m_aTgtStorK.GetSize();
	for (int i = 0; i < nCnt; ++i)
	{
		T_STOR_D StorD;
		if(m_pDoc->m_pAttrCtrl->GetStor(m_aTgtStorK[i], StorD))
		{
			m_strTargerStory += _T("<") + StorD.StoryName + _T(">");
		}
	}
}

double CWindAutoCalcDlg::GetUpperStorLevel(UINT StorK)
{
	double dUpperStorLevel = 0.;
	if(m_pLinkedKeyMgr_stor)
	{
		T_STOR_D DataStory, DataStoryU;
		if(!m_pDoc->m_pAttrCtrl->GetStor(StorK, DataStory)) return dUpperStorLevel;
		BOOL bRoof = FALSE;
		CArray<T_STOR_K,T_STOR_K> aStorK_temp;  aStorK_temp.RemoveAll();
		if (m_pLinkedKeyMgr_stor->GetNextKeyList(StorK, aStorK_temp) == 0)	bRoof = TRUE;
		else                                                                bRoof = FALSE;
		if (!bRoof)
		{
			// get above story
			m_pLinkedKeyMgr_stor->GetNextKeyList(StorK, aStorK_temp);

			T_STOR_D DataStoryU_max;  DataStoryU_max.Initialize();
			DataStoryU_max.dStoryLevel = -DBL_MAX;
			for (int k=0; k<aStorK_temp.GetSize(); ++k)
			{
				if (!m_pDoc->m_pAttrCtrl->GetStor(aStorK_temp[k],DataStoryU))  { ASSERT(0);  DataStoryU.Initialize(); }
				if (DataStoryU_max.dStoryLevel < DataStoryU.dStoryLevel)  DataStoryU_max = DataStoryU;
			}
			DataStoryU = DataStoryU_max;    // 제일 위 Level층을 위층으로 설정한다. (최대 층고)
		}
		else  // 최상층인 경우
		{ 
			DataStoryU.Initialize();
			DataStoryU.dStoryLevel = DataStory.dStoryLevel;
		}

		dUpperStorLevel = DataStoryU.dStoryLevel - m_dBaseLevel;
	}
	return dUpperStorLevel;
}

void CWindAutoCalcDlg::SetData(T_WNAT_D& WnatD)
{
	int nCnt = WnatD.arWnatStor.GetSize();
	m_mapWnatStor.RemoveAll();
	if (nCnt>0)
	{
		for (int i = 0; i < nCnt; ++i)
		{
			m_mapWnatStor.SetAt(WnatD.arWnatStor[i].KeyStor, WnatD.arWnatStor[i]);
		}
	}
	
	if(m_lstStory) MakeLstItem();
}

void CWindAutoCalcDlg::GetData(T_WNAT_D& WnatD)
{
	WnatD.arWnatStor.RemoveAll();
	
	CArray<T_STOR_K, T_STOR_K> aStorK;
	CArray<double, double> aStorLevel;

	POSITION pos = m_mapWnatStor.GetStartPosition();
	while (pos != NULL)
	{
		T_STOR_K StorK;
		T_WNAT_STOR WnatStorD;
		m_mapWnatStor.GetNextAssoc(pos, StorK, WnatStorD);
		T_STOR_D StorD;
		if(!m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD)) continue;
		aStorK.Add(StorK);
		aStorLevel.Add(StorD.dStoryLevel);
	}	
	
	int nStorKCnt = aStorK.GetSize() ;
	if(nStorKCnt < 1) return;
	
	WnatD.arWnatStor.SetSize(nStorKCnt);
	// level에 따라 오름차순 key 정렬
	CQSort::QSortDblWithIndex(aStorK.GetData(), aStorLevel.GetData(), nStorKCnt);
	
	// 내림차순으로 ListView에 출력
	int nIndex = 0;
	for (int i = nStorKCnt - 1; i >= 0; --i)
	{
		WnatD.arWnatStor.SetAt(nIndex++, m_mapWnatStor[aStorK[i]]);
	}
}
