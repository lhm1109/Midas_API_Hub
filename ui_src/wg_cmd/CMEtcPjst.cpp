// CMEtcPjst.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMEtcPjst.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"


#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_GEN   0x01
#define D_CVL   0x02
#define D_TOW   0x04

/////////////////////////////////////////////////////////////////////////////
// CCMEtcPjst dialog


CCMEtcPjst::CCMEtcPjst(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMEtcPjst::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMEtcPjst)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bAPIMode = false;
}

CCMEtcPjst::~CCMEtcPjst()
{
	bAPIMode = false;
}

void CCMEtcPjst::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMEtcPjst)
	DDX_Control(pDX, IDC_CMD_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndModel);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER2, m_wndLoad);
	//}}AFX_DATA_MAP
}

void CCMEtcPjst::GetData4API(std::vector<std::tuple<std::string, unsigned int, unsigned int, bool>>& vtplProjStatus)
{
	// AddModelStatus
	vtplProjStatus.clear();
	for (int i = 0; i < m_aModelName.GetSize(); ++i)
	{
		vtplProjStatus.push_back(std::make_tuple(std::string(CT2CA(m_aModelName[i])), m_aModelCount[i], m_aModelLastNo[i], true));
	}

	// AddLoadStatus
	for (int i = 0; i < m_aLoadName.GetSize(); ++i)
	{
		vtplProjStatus.push_back(std::make_tuple(std::string(CT2CA(m_aLoadName[i])), m_aLoadCount[i], 0, false));
	}
}

/////////////////////////////////////////////////////////////////////////////
// Implementations
/////////////////////////////////////////////////////////////////////////////
void CCMEtcPjst::AddModelStatus()
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	int nCount;
	
	nCount = pAttrCtrl->ExistStyp() ? 1 : 0;
	AddModelItem(_LS(IDS_CMD_PJST_STYP),                  nCount, 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_NUCS),                  pAttrCtrl->GetCountNucs(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_Named_Plane),           pAttrCtrl->GetCountNpln(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_Line_Grid),             pAttrCtrl->GetCountGdln(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_Group),                 pAttrCtrl->GetCountGrup(), 0); 
	AddModelItem(_LS(IDS_CMD_PJST_BNGR),                  pAttrCtrl->GetCountBngr(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_LDGR),                  pAttrCtrl->GetCountLdgr(), 0);

	AddModelItem(_LS(IDS_CMD_PJST_Node),                  pAttrCtrl->GetCountNode(), pAttrCtrl->GetLastNumNode());
	AddModelItem(_LS(IDS_CMD_PJST_Element),               pAttrCtrl->GetCountElem(), pAttrCtrl->GetLastNumElem());

	AddModelItem(_LS(IDS_CMD_PJST_Material),              pAttrCtrl->GetCountMatl(), pAttrCtrl->GetLastNumMatl());
	AddModelItem(_LS(IDS_CMD_PJST_TDMF),                  pAttrCtrl->GetCountTdmf(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_TDMT),                  pAttrCtrl->GetCountTdmt(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_TDME),                  pAttrCtrl->GetCountTdme(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_TMAT),                  pAttrCtrl->GetCountTmat(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_EDMP),                  pAttrCtrl->GetCountEdmp(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Section),               pAttrCtrl->GetCountSect(), pAttrCtrl->GetLastNumSect());
	AddModelItem(_LS(IDS_CMD_PJST_SECF),                  pAttrCtrl->GetCountSecf(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_TSGR),                  pAttrCtrl->GetCountTsgr(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Thickness),             pAttrCtrl->GetCountThik(), pAttrCtrl->GetLastNumThik(), D_TOW);

	AddModelItem(_LS(IDS_CMD_PJST_Support),               pAttrCtrl->GetCountCons(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_Point_Spring),          pAttrCtrl->GetCountNspr(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_General_Spring_Type),   pAttrCtrl->GetCountGstp(), pAttrCtrl->GetLastNumGstp(), D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_General_Spring),        pAttrCtrl->GetCountGspr(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Elastic_Link),          pAttrCtrl->GetCountElnk(), pAttrCtrl->GetLastNumElnk(), D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Beam_Release),          pAttrCtrl->GetCountFrls(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Plate_Release),         pAttrCtrl->GetCountPrls(), 0, D_TOW); 
	AddModelItem(_LS(IDS_CMD_PJST_Beam_Offset),           pAttrCtrl->GetCountOffs(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Rigid_Link),            pAttrCtrl->GetCountRigd(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Diaphragm_Disconnect),  pAttrCtrl->GetCountDrls(), 0, D_CVL+D_TOW);
	nCount = pAttrCtrl->ExistPzef() ? 1 : 0;
	AddModelItem(_LS(IDS_CMD_PJST_Panel_Zone),            nCount, 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Node_Local_Axis),       pAttrCtrl->GetCountSkew(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_STDG),                  pAttrCtrl->GetCountStdg(), 0, D_CVL+D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_NLLP),                  pAttrCtrl->GetCountNllp(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_NLNK),                  pAttrCtrl->GetCountNlnk(), 0);
	AddModelItem(_LS(IDS_CMD_PJST_IEHP),                  pAttrCtrl->GetCountIehp(), 0, D_CVL+D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_IEHG),                  pAttrCtrl->GetCountIehg(), 0, D_CVL+D_TOW);

	AddModelItem(_LS(IDS_CMD_PJST_Nodal_Mass),            pAttrCtrl->GetCountNmas(), 0, D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Diaphragm_Mass),        pAttrCtrl->GetCountDmas(), 0, D_CVL+D_TOW);
	nCount = pAttrCtrl->ExistLtom() ? 1 : 0;
	AddModelItem(_LS(IDS_CMD_PJST_Loads_to_Mass),         nCount, 0, D_TOW);

	nCount = pAttrCtrl->ExistBldc() ? 1 : 0;
	AddModelItem(_LS(IDS_CMD_PJST_BLDC),                  nCount, 0, D_CVL+D_TOW);
	AddModelItem(_LS(IDS_CMD_PJST_Story),                 pAttrCtrl->GetCountStor(), 0, D_CVL+D_TOW);
}

void CCMEtcPjst::AddLoadStatus()
{
	CAttrCtrl* pAttrCtrl = m_pDoc->m_pAttrCtrl;
	int nCount;

	AddLoadItem(_LS(IDS_CMD_PJST_Static_Load_Case),       pAttrCtrl->GetCountStld());
	AddLoadItem(_LS(IDS_CMD_PJST_Self_Weight),            pAttrCtrl->GetCountBodf(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Nodal_Body_Force),       pAttrCtrl->GetCountNbof(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Nodal_Load),             pAttrCtrl->GetCountCnld());
	AddLoadItem(_LS(IDS_CMD_PJST_Specified_Displacement), pAttrCtrl->GetCountSdsp());

	AddLoadItem(_LS(IDS_CMD_PJST_Beam_Load),              pAttrCtrl->GetCountBmld(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Floor_Load_Type),        pAttrCtrl->GetCountFbld(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Floor_Load),             pAttrCtrl->GetCountFbla(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Plane_Load_Type),        pAttrCtrl->GetCountPnld(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Plane_Load),             pAttrCtrl->GetCountPnla(), D_TOW);
#if !defined(_US) || !defined(_RUS)
	AddLoadItem(_LS(IDS_CMD_PJST_FMLD),                   pAttrCtrl->GetCountFmld(), D_TOW);
#endif
	AddLoadItem(_LS(IDS_CMD_PJST_Pressure),               pAttrCtrl->GetCountPres(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_System_Temperature),     pAttrCtrl->GetCountStmp(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Nodal_Temperature),      pAttrCtrl->GetCountNtmp(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_ETMP),                   pAttrCtrl->GetCountEtmp(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_BTMP),                   pAttrCtrl->GetCountBtmp(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Gradient_Temperature),   pAttrCtrl->GetCountGtmp(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Prestress),              pAttrCtrl->GetCountPrst(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Pretension),             pAttrCtrl->GetCountPtns(), D_TOW);

	T_EXLD_D exld;
	if (pAttrCtrl->GetExld(exld)) nCount = exld.nNumExldCase;
	else nCount = 0;
	AddLoadItem(_LS(IDS_CMD_PJST_ExtLoad), nCount, D_GEN+D_TOW);

	AddLoadItem(_LS(IDS_CMD_PJST_EquForce), pAttrCtrl->GetCountEqmf(), D_GEN+D_TOW);

	AddLoadItem(_LS(IDS_CMD_PJST_TDNT),                   pAttrCtrl->GetCountTdnt(), D_GEN+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_TDNA),                   pAttrCtrl->GetCountTdna(), D_GEN+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_TDPL),                   pAttrCtrl->GetCountTdpl(), D_GEN+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_TMLD),                   pAttrCtrl->GetCountTmld(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_CRPC),                   pAttrCtrl->GetCountCrpc(), D_TOW);

	AddLoadItem(_LS(IDS_CMD_PJST_Wind_Load),              pAttrCtrl->GetCountWind(), D_CVL+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Static_Seismic_Load),    pAttrCtrl->GetCountSeis(), D_CVL+D_TOW);
#if defined(_JP)  
	AddLoadItem(_LS(IDS_CMD_PJST_Super_Seismic_Load),     pAttrCtrl->GetCountSsei(), D_GEN+D_TOW);
#endif
	// initial forces for geometric stiffness
	nCount = pAttrCtrl->ExistIfct() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_IFCT),                   nCount, D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Initial_Forecs_for_GS),  pAttrCtrl->GetCountIfgs(), D_TOW);

	// response spectrum
	AddLoadItem(_LS(IDS_CMD_PJST_Spectrum_Function),      pAttrCtrl->GetCountSpfc(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Spectrum_Load),          pAttrCtrl->GetCountSplc(), D_TOW);

	// time history
	AddLoadItem(_LS(IDS_CMD_PJST_Time_History_Function),  pAttrCtrl->GetCountThfc(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Dynamic_Nodal_Load),     pAttrCtrl->GetCountThnl(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Ground_Acceleration),    pAttrCtrl->GetCountThga(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Dynamic_Spatial_Load),   pAttrCtrl->GetCountThsl(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Time_History_Load),      pAttrCtrl->GetCountThis(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_THMS),                   pAttrCtrl->GetCountThms(), D_TOW);

	// Moving Load
	// �ڵ忡 ���� ������� ���� 
	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support),           pAttrCtrl->GetCountMlsp(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	case D_MOVE_CODE_AASHTO_LRFD: 
	case D_MOVE_CODE_PENDOT:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support),           pAttrCtrl->GetCountMlsp(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support_Reactions), pAttrCtrl->GetCountMlsr(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	case D_MOVE_CODE_CHINA:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanch(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlanch(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf()  , D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl()  , D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc()  , D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldch(), D_TOW);
		break;
	case D_MOVE_CODE_JAPAN:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanjp(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldjp(), D_TOW);
		break;
	case D_MOVE_CODE_JAPAN_RAIL:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanjp(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(),   D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldjp(), D_TOW);
		break;
	case D_MOVE_CODE_INDIA:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanid(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(),   D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(),   D_TOW);      
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldid(), D_TOW);
		break;
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_NEWZEALAND:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane), pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane), pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence), pAttrCtrl->GetCountSinf(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle), pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class), pAttrCtrl->GetCountMvhc(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case), pAttrCtrl->GetCountMvld(), D_TOW);
	case D_MOVE_CODE_BRAZIL:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	case D_MOVE_CODE_BS:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(),  D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(),  D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(),  D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldbs(), D_TOW);
		break;
	case D_MOVE_CODE_FRANCE:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanfr(),  D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlanfr(),  D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(),  D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldfr(), D_TOW);
		break;
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_SOUTH_AFRICA:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	case D_MOVE_CODE_KOREA_LRFD_2011:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	case D_MOVE_CODE_TRANS:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlantr(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhltr(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldtr(), D_TOW);
		break;
	default:
		ASSERT(0);
		break;
	}

	/*
	switch(CProduct::GetMovingType())
	{
	case D_PRODUCT_MOVING_JP:
		#if defined(_CIVIL)
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->ExistLlanjp() ? 1:0, D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldjp(), D_TOW);
		#endif
		break;
	case D_PRODUCT_MOVING_CH:
		#if defined(_CIVIL)
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanch(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlanch(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf()  , D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl()  , D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc()  , D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldch(), D_TOW);
		#endif
		break;
	case D_PRODUCT_MOVING_US:
		#if defined(_CIVIL)
		if(CProduct::IsIndia())
		{
			AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlanid(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);      
			AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvldid(), D_TOW);
		}
		else
		{
			AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support),           pAttrCtrl->GetCountMlsp(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support_Reactions), pAttrCtrl->GetCountMlsr(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc(), D_TOW);
			AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		}
		#endif
		break;
	default:
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Line_Lane),      pAttrCtrl->GetCountLlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Traffic_Surface_Lane),   pAttrCtrl->GetCountSlan(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Surface_Influence),      pAttrCtrl->GetCountSinf(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support),           pAttrCtrl->GetCountMlsp(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Lane_Support_Reactions), pAttrCtrl->GetCountMlsr(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle),                pAttrCtrl->GetCountMvhl(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Vehicle_Class),          pAttrCtrl->GetCountMvhc(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Moving_Load_Case),       pAttrCtrl->GetCountMvld(), D_TOW);
		break;
	}
	*/

	// settlement 
#if defined(_MGEN)  //kym
	#if defined(_ORG) || defined(_JP) || defined(_US) || defined(_RUS)
		AddLoadItem(_LS(IDS_CMD_PJST_Settlement_Group),       pAttrCtrl->GetCountSmpt(), D_TOW);
		AddLoadItem(_LS(IDS_CMD_PJST_Settlement_Load_Case),   pAttrCtrl->GetCountSmlc(), D_TOW);      
	#endif
#else
	AddLoadItem(_LS(IDS_CMD_PJST_Settlement_Group),       pAttrCtrl->GetCountSmpt(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Settlement_Load_Case),   pAttrCtrl->GetCountSmlc(), D_TOW);
#endif

	// erection
	/* ���� : by jbseon
	T_ESEQ_D eseq;
	if (!pAttrCtrl->GetEseq(eseq)) eseq.Initialize();
	AddLoadItem(_LS(IDS_CMD_PJST_Erection_Sequence),      eseq.arEseq.GetSize(), D_CVL+D_TOW);
	*/

	T_LDSQ_D ldsq;
	if (!pAttrCtrl->GetLdsq(ldsq)) ldsq.Initialize();
	AddLoadItem(_LS(IDS_CMD_PJST_LDSQ),                   ldsq.aLoadingSequence.GetSize(), D_TOW);

	T_PLCB_D plcb;
	if (!pAttrCtrl->GetPlcb(plcb)) plcb.Initialize();
	AddLoadItem(_LS(IDS_CMD_PJST_PLCB),                   plcb.arLoad.GetSize(), D_TOW);

	// Hydration
	nCount = pAttrCtrl->ExistHhct() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_HHCT),                   nCount, D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_ETFC),                   pAttrCtrl->GetCountEtfc(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_CCFC),                   pAttrCtrl->GetCountCcfc(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HECB),                   pAttrCtrl->GetCountHecb(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HSPT),                   pAttrCtrl->GetCountHspt(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HSFC),                   pAttrCtrl->GetCountHsfc(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HAHS),                   pAttrCtrl->GetCountHahs(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HPCE),                   pAttrCtrl->GetCountHpce(), D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HSTG),                   pAttrCtrl->GetCountHstg(), D_TOW);

	// construction stage
	nCount = pAttrCtrl->ExistConstStag() ? pAttrCtrl->GetCountStag() : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_STAG),                   nCount, D_TOW);

	// control data
	nCount = pAttrCtrl->ExistActl() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_ACTL), nCount);

	T_PDEL_D pdel;
	if (pAttrCtrl->GetPdel(pdel)) nCount = pdel.nNumPdelCase;
	else nCount = 0;
	AddLoadItem(_LS(IDS_CMD_PJST_PDelta), nCount, D_TOW);

	T_SBCT_D sbct;
	if (pAttrCtrl->GetSbct(sbct)) nCount = sbct.nNumSbctCase;
	else nCount = 0;
	AddLoadItem(_LS(IDS_CMD_PJST_SusBrg), nCount, D_GEN+D_TOW);

	T_EFCT_D efct;
	if (pAttrCtrl->GetEfct(efct)) nCount = efct.nNumEfctCase;
	else nCount = 0;
	AddLoadItem(_LS(IDS_CMD_PJST_EquFrc), nCount, D_GEN+D_TOW);

	T_BUCK_D buck;
	if (pAttrCtrl->GetBuck(buck)) nCount = buck.nNumMode;
	else nCount = 0;
	AddLoadItem(_LS(IDS_CMD_PJST_Buckling), nCount, D_TOW);

	T_EIGV_D eigv;
	//if (pAttrCtrl->GetEigv(eigv)) nCount = eigv.nNumMode;
	if (pAttrCtrl->GetEigv(eigv)) nCount = pAttrCtrl->GetNumEigv();
	else nCount = 0;
	AddLoadItem(_LS(IDS_CMD_PJST_Eigenvalue), nCount, D_TOW);
	
	// 7.0.2 ���� Response Spec. Ctrl. Data ������. 
	//nCount = pAttrCtrl->ExistSpct() ? 1 : 0;
	//AddLoadItem(_LS(IDS_CMD_PJST_SPCT), nCount, D_TOW);

	nCount = pAttrCtrl->ExistMvcd() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_MVCD), nCount, D_TOW);

	// �ڵ忡 ���� ������� ���� 
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_JAPAN_RAIL:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		nCount = pAttrCtrl->ExistMvct() ? 1 : 0;
		break;
	case D_MOVE_CODE_AASHTO_LRFD: case D_MOVE_CODE_PENDOT:
		nCount = pAttrCtrl->ExistMvct() ? 1 : 0;
		break;
	case D_MOVE_CODE_CHINA:
		nCount = pAttrCtrl->ExistMvctch() ? 1 : 0;
		break;
	case D_MOVE_CODE_JAPAN:
		nCount = pAttrCtrl->ExistMvctjp() ? 1 : 0;
		break;
	case D_MOVE_CODE_INDIA:
		nCount = pAttrCtrl->ExistMvctid() ? 1 : 0;
		break;
	case D_MOVE_CODE_TRANS:
		nCount = pAttrCtrl->ExistMvcttr() ? 1 : 0;
		break;
	case D_MOVE_CODE_FRANCE:
		nCount = pAttrCtrl->ExistMvctfr() ? 1 : 0;
		break;
	default:
		ASSERT(0);
		break;
	}
	AddLoadItem(_LS(IDS_CMD_PJST_MVCT), nCount, D_TOW);

	/*
	switch(CProduct::GetMovingType())
	{
	case D_PRODUCT_MOVING_JP:
		#if defined(_CIVIL)
		nCount = pAttrCtrl->ExistMvctjp() ? 1 : 0;
		AddLoadItem(_LS(IDS_CMD_PJST_MVCT), nCount, D_TOW);
		#endif
		break;
	case D_PRODUCT_MOVING_CH:
		#if defined(_CIVIL)
		nCount = pAttrCtrl->ExistMvctch() ? 1 : 0;
		AddLoadItem(_LS(IDS_CMD_PJST_MVCT), nCount, D_TOW);
		#endif
		break;
	case D_PRODUCT_MOVING_US:   
		#if defined(_CIVIL)
		if(CProduct::IsIndia())
		{
			nCount = pAttrCtrl->ExistMvctid() ? 1 : 0;
			AddLoadItem(_LS(IDS_CMD_PJST_MVCT), nCount, D_TOW);
		}
		else
		{
			nCount = pAttrCtrl->ExistMvct() ? 1 : 0;
			AddLoadItem(_LS(IDS_CMD_PJST_MVCT), nCount, D_TOW);
		}
		#endif
		break;
	default:
		nCount = pAttrCtrl->ExistMvct() ? 1 : 0;
		AddLoadItem(_LS(IDS_CMD_PJST_MVCT), nCount, D_TOW);
		break;
	}
	*/

	nCount = pAttrCtrl->ExistNlct() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_NLCT), nCount, D_TOW);

	nCount = pAttrCtrl->ExistStct() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_STCT), nCount, D_TOW);

	nCount = pAttrCtrl->ExistBcct() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_BCCT), nCount, D_TOW);

	// combination
	AddLoadItem(_LS(IDS_CMD_PJST_Load_Combination),       pAttrCtrl->GetCountLcom(D_LCOMTYPE_GENERAL));
	AddLoadItem(_LS(IDS_CMD_PJST_Load_Comb_Steel),        pAttrCtrl->GetCountLcom(D_LCOMTYPE_STEEL)     , D_CVL);
	AddLoadItem(_LS(IDS_CMD_PJST_Load_Comb_Concrete),     pAttrCtrl->GetCountLcom(D_LCOMTYPE_CONCRETE)  , D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Load_Comb_SRC),          pAttrCtrl->GetCountLcom(D_LCOMTYPE_SRC)       , D_CVL+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_Load_Comb_Footing),      pAttrCtrl->GetCountLcom(D_LCOMTYPE_FDN)       , D_CVL);

	// envelope
//  AddLoadItem(_LS(IDS_CMD_PJST_Envelope),               pAttrCtrl->GetCountEnvl(), D_CVL+D_TOW);

	// pushover
	nCount = pAttrCtrl->ExistPoct() ? 1 : 0;
	AddLoadItem(_LS(IDS_CMD_PJST_POCT), nCount, D_CVL+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_POLD),                   pAttrCtrl->GetCountPold(), D_CVL+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HNGT),                   pAttrCtrl->GetCountHngt(), D_CVL+D_TOW);
	AddLoadItem(_LS(IDS_CMD_PJST_HNGE),                   pAttrCtrl->GetCountHnge(), D_CVL+D_TOW);
	
	// post
	//AddLoadItem(_LS(IDS_CMD_PJST_THRD),                   pAttrCtrl->GetCountThrd(), D_TOW);
	//AddLoadItem(_LS(IDS_CMD_PJST_CUTL),                   pAttrCtrl->GetCountCutl());
	//AddLoadItem(_LS(IDS_CMD_PJST_ULFC),                   pAttrCtrl->GetCountUlfc(), D_TOW);
	//AddLoadItem(_LS(IDS_CMD_PJST_ULFD),                   pAttrCtrl->GetCountUlfd(), D_TOW);
	//AddLoadItem(_LS(IDS_CMD_PJST_HHND),                   pAttrCtrl->GetCountHhnd(), D_TOW);
	//nCount = pAttrCtrl->ExistCamb() ? 1 : 0;
	//AddLoadItem(_LS(IDS_CMD_PJST_CAMB),                   nCount, D_GEN+D_TOW);
}

void CCMEtcPjst::OutputModelStatus()
{
	int nItemNum, nColNum;
	nItemNum = m_aModelName.GetSize();
	nColNum = 3;

	CString csValue;

	InitGridCommonParameter(m_pModel);
	m_pModel->GetParam()->SetLockReadOnly(FALSE);

	ROWCOL nStartRow=1, nStartCol=1;
	m_pModel->SetRowCount(nStartRow-1+nItemNum);
	m_pModel->SetColCount(nStartCol-1+nColNum);
	m_pModel->SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));
	m_pModel->SetRowHeight(1, m_pModel->GetRowCount(), globalUtils.ScaleByDPI(18));

	globalUtils.GetDPIScale();

	int aColWidth[] = { 0, globalUtils.ScaleByDPI(200), globalUtils.ScaleByDPI(55), globalUtils.ScaleByDPI(55) };
	if (CProduct::IsRusLocal())
	{
		aColWidth[1] = 360;
		aColWidth[2] = 80;
		aColWidth[3] = 80;
	}
	else if (fabs(globalUtils.GetDPIScale() - 1.25) < 1e-5)
	{
		aColWidth[1] = 250;
		aColWidth[2] = 75;
		aColWidth[3] = 75;
	}
	else if(fabs(globalUtils.GetDPIScale()-1.5) < 1e-5)
	{
		aColWidth[1] = 280;
		aColWidth[2] = 80;
		aColWidth[3] = 80;
	}

	m_pModel->SetColWidth(0, nColNum, 0, aColWidth);
	CString aTitle[] = {_LS(IDS_CMD_PJST_Name), _LS(IDS_CMD_PJST_Count), _LS(IDS_CMD_PJST_Last_No)};
	for (int j = 0; j < nColNum; j++)
		m_pModel->SetStyleRange(CGXRange(nStartRow-1, nStartCol+j), CGXStyle()
							.SetControl(GX_IDS_CTRL_HEADER)
							.SetHorizontalAlignment(DT_CENTER)
							.SetValue(aTitle[j])
							.SetVertScrollBar(FALSE));

	m_pModel->SetStyleRange(CGXRange().SetCols(nStartCol, m_pModel->GetColCount()),
		CGXStyle().SetControl(GX_IDS_CTRL_STATIC));

	m_pModel->SetStyleRange(CGXRange().SetCols(nStartCol), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT));
	m_pModel->SetStyleRange(CGXRange().SetCols(nStartCol+1, nStartCol+2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));
	
	for (int i = 0; i < nItemNum; i++)
	{
		m_pModel->SetValueRange(CGXRange(nStartRow+i, nStartCol), m_aModelName[i]);
		if (m_aModelCount[i] == 0) csValue = _LS(IDS_WG_CMD__ADDD__None);
		else csValue.Format(_T("%u"), m_aModelCount[i]);
		m_pModel->SetValueRange(CGXRange(nStartRow+i, nStartCol+1), csValue);
		if (m_aModelLastNo[i] == 0) csValue = _T("");
		else csValue.Format(_T("%u"), m_aModelLastNo[i]);
		m_pModel->SetValueRange(CGXRange(nStartRow+i, nStartCol+2), csValue);
	}
	m_pModel->GetParam()->SetLockReadOnly(TRUE);
	m_pModel->EnableMouseWheel();
}

void CCMEtcPjst::OutputLoadStatus()
{
	int nItemNum, nColNum;
	nItemNum = m_aLoadName.GetSize();
	nColNum = 2;

	CString csValue;

	InitGridCommonParameter(m_pLoad);
	m_pLoad->GetParam()->SetLockReadOnly(FALSE);

	ROWCOL nStartRow=1, nStartCol=1;
	m_pLoad->SetRowCount(nStartRow-1+nItemNum);
	m_pLoad->SetColCount(nStartCol-1+nColNum);
	m_pLoad->SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));
	m_pLoad->SetRowHeight(1, m_pLoad->GetRowCount(), globalUtils.ScaleByDPI(18));

	double dtest = globalUtils.GetDPIScale();

	int aColWidth[] = { 0, globalUtils.ScaleByDPI(205), globalUtils.ScaleByDPI(50) };
	if (CProduct::IsRusLocal())
	{
		aColWidth[1] = 360;
		aColWidth[2] = 80;
	}
	else if (fabs(globalUtils.GetDPIScale() - 1.25) < 1e-5)
	{
		aColWidth[1] = 250;
		aColWidth[2] = 75;
	}
	else if (fabs(globalUtils.GetDPIScale() - 1.5) < 1e-5)
	{
		aColWidth[1] = 280;
		aColWidth[2] = 80;
	}

	m_pLoad->SetColWidth(0, nColNum, 0, aColWidth);
	CString aTitle[] = {_LS(IDS_CMD_PJST_Name), _LS(IDS_CMD_PJST_Count)};
	for (int j = 0; j < nColNum; j++)
		m_pLoad->SetStyleRange(CGXRange(nStartRow-1, nStartCol+j), CGXStyle()
						.SetControl(GX_IDS_CTRL_HEADER)
						.SetHorizontalAlignment(DT_CENTER)
						.SetValue(aTitle[j])
						.SetVertScrollBar(FALSE));

	m_pLoad->SetStyleRange(CGXRange().SetCols(nStartCol, m_pLoad->GetColCount()),
		CGXStyle().SetControl(GX_IDS_CTRL_STATIC));

	m_pLoad->SetStyleRange(CGXRange().SetCols(nStartCol), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT));
	m_pLoad->SetStyleRange(CGXRange().SetCols(nStartCol+1), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));
	
	for (int i = 0; i < nItemNum; i++)
	{
		m_pLoad->SetValueRange(CGXRange(nStartRow+i, nStartCol), m_aLoadName[i]);
		if (m_aLoadCount[i] == 0) csValue = _LS(IDS_WG_CMD__ADDD__None);
		else csValue.Format(_T("%u"), m_aLoadCount[i]);
		m_pLoad->SetValueRange(CGXRange(nStartRow+i, nStartCol+1), csValue);
	}  
	m_pLoad->GetParam()->SetLockReadOnly(TRUE);
	m_pLoad->EnableMouseWheel();
}

void CCMEtcPjst::AddModelItem(CString csItemName, UINT nCount, UINT nLastNo, int nIgnoreWhen)
{
	int nIgnoreTarget;
#if defined(_MGEN)
	nIgnoreTarget = D_GEN;
#elif defined(_CIVIL)
	nIgnoreTarget = D_CVL;
#elif defined(_TOWER)
	nIgnoreTarget = D_TOW;
#else
	#error Unknown Product Definition
#endif
	if ((nIgnoreWhen & nIgnoreTarget) != 0) return;
	m_aModelName.Add(csItemName);
	m_aModelCount.Add(nCount);
	m_aModelLastNo.Add(nLastNo);
}

void CCMEtcPjst::AddLoadItem(CString csItemName, UINT nCount, int nIgnoreWhen)
{
	int nIgnoreTarget;
#if defined(_MGEN)
	nIgnoreTarget = D_GEN;
#elif defined(_CIVIL)
	nIgnoreTarget = D_CVL;
#elif defined(_TOWER)
	nIgnoreTarget = D_TOW;
#else
	#error Unknown Product Definition
#endif
	if ((nIgnoreWhen & nIgnoreTarget) != 0) return;
	m_aLoadName.Add(csItemName);
	m_aLoadCount.Add(nCount);
}

void CCMEtcPjst::InitGridCommonParameter(CGXGridWnd* pGrid)
{
	pGrid->GetParam()->EnableUndo(FALSE);

	// ǥ�� ��Ÿ�� ����
	pGrid->ChangeStandardStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetSize(9))
					.SetVerticalAlignment(DT_BOTTOM)
					.SetAllowEnter(FALSE));

	// Row�� Column �̵��� ������Ų��.
	pGrid->GetParam()->EnableMoveRows(FALSE);
	pGrid->GetParam()->EnableMoveCols(FALSE);

	// Row�� ���� ������ �����Ѵ�.
	pGrid->GetParam()->EnableTrackRowHeight(FALSE); 
	pGrid->GetParam()->EnableTrackColWidth(FALSE);

	// ���� ���� ������ Focus�� �Ҵ��� ǥ�õǰ� �Ѵ�.
	pGrid->GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	pGrid->SetDrawingTechnique(gxDrawUsingMemDC); // Memory DC�� �̿��� ������� ����
	//pGrid->EnableOleDataSource(GX_DNDDISABLED); // copy & paste ����

	pGrid->GetParam()->GetProperties()->SetMarkColHeader(FALSE);    // Turn off pressed button effect for column headers
	pGrid->GetParam()->GetProperties()->SetPrintRowHeaders(FALSE);  // Don't print column headers
}

BEGIN_MESSAGE_MAP(CCMEtcPjst, CDialogMove)
	//{{AFX_MSG_MAP(CCMEtcPjst)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnCmdClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMEtcPjst message handlers

BOOL CCMEtcPjst::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	// model grid
	m_pModel = &m_wndModel;
	m_pModel->Initialize();
	
	// cell �ٱ����� ���� ���� 
	m_pModel->GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header ���� ���� 
	m_pModel->ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	m_pModel->ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// ��Ÿ Cells ���� ����
	m_pModel->ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	m_pModel->ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(10))
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE));

	// load grid
	m_pLoad = &m_wndLoad;
	m_pLoad->Initialize();

	// cell �ٱ����� ���� ���� 
	m_pLoad->GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header ���� ���� 
	m_pLoad->ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	m_pLoad->ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// ��Ÿ Cells ���� ����
	m_pLoad->ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	m_pLoad->ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(10))
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE));

	AddModelStatus();
	AddLoadStatus();

	if ( bAPIMode )
	{
		OnCmdClose();
		return TRUE;
	}

	OutputModelStatus();
	OutputLoadStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMEtcPjst::OnCmdClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}
