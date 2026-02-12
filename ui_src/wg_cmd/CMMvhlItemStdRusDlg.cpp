// CMMvhlItemStdRusDlg.cpp : implementation file
//

#include "stdafx.h"

#include <array>

#include "wg_cmd.h"
#include "CMMvhlItemStdRusDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdRusDlg dialog
#define D_MAX_COUNT  9
#define COLCOUNT 3

CCMMvhlItemStdRusDlg::CCMMvhlItemStdRusDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdRusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdRusDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	m_bModify = FALSE;

	m_nStndCode = 0;
	m_nVehicleIndex = 0;

	m_nDynaFact = 0;
	m_nLoadFact = 0;
	m_nEpsilonFact = 0;

	m_bFatigue = FALSE;
	m_bSameLoadedL = FALSE;
	m_bTwoVehi = FALSE;
	m_b2ndGrpReduc = FALSE;

	m_aCtrl_BridgeType.Add(IDC_CMD_MVHL_BRIDGE_TYPE_STC);
	m_aCtrl_BridgeType.Add(IDC_CMD_MVHL_BRIDGE_TYPE_CMB);

	m_aCtrl_K.Add(IDC_CMD_MVHL_K_STC);
	m_aCtrl_K.Add(IDC_CMD_MVHL_K_EDT);
	
	m_aCtrl_W.Add(IDC_CMD_MVHL_W_STC);
	m_aCtrl_W.Add(IDC_CMD_MVHL_W_EDT);
	m_aCtrl_W.Add(IDC_CMD_MVHL_W_UNT);
	m_aCtrl_W.Add(IDC_CMD_MVHL_D_STC);
	m_aCtrl_W.Add(IDC_CMD_MVHL_D_EDT);
	m_aCtrl_W.Add(IDC_CMD_MVHL_D_UNT);

	m_aCtrl_List.Add(IDC_CMD_MVHL_PTLIST);

	m_aCtrl_Fatigue.Add(IDC_CMD_MVHL_FATIGUE_CHK);

	m_aCtrl_SameLoadedL.Add(IDC_CMD_MVHL_SAME_LOADED_L_CHK);

	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_GRP);
	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_CHK);
	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_FACT_STC);
	m_ACtrl_TwoVehi.Add(IDC_CMD_MVHL_TWO_VEHI_FACT_EDT);

	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_GRP);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_GRP);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_EDT);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_RDO);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_GRP);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_STC);
	m_aCtrl_DynaFact.Add(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT);

	m_aCtrl_DynaFact_UDL.Add(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_STC);
	m_aCtrl_DynaFact_UDL.Add(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_EDT);

	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_GRP);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_RDO);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_GRP);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_STC);
	m_aCtrl_LoadFact.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT);

	m_aCtrl_LoadFact_UDL.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_STC);
	m_aCtrl_LoadFact_UDL.Add(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT);

	m_aCtrl_2ndGrpReduc.Add(IDC_CMD_MVHL_2ND_REDUC_GRP);
	m_aCtrl_2ndGrpReduc.Add(IDC_CMD_MVHL_2ND_REDUC_CHK);
	m_aCtrl_2ndGrpReduc.Add(IDC_CMD_MVHL_2ND_REDUC_EDT);

	m_aCtrl_TramTop.Add(IDC_CMD_MVHL_IDC_CMD_MVHL_LOADED_STC);
	m_aCtrl_TramTop.Add(IDC_CMD_MVHL_IDC_CMD_MVHL_EMPTY_STC);
	m_aCtrl_TramTop.Add(IDC_CMD_MVHL_RATIO_RATIO_STC);
	m_aCtrl_TramTop.Add(IDC_CMD_MVHL_RATIO_RATIO_EDT);

	m_aCtrl_TramBot.Add(IDC_CMD_MVHL_VARIABLE_STC);
	m_aCtrl_TramBot.Add(IDC_CMD_MVHL_VARIABLE_EDT);
	m_aCtrl_TramBot.Add(IDC_CMD_MVHL_VARIABLE_UNT);

	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_GRP);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_LANE1_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_LANE2_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_LANE3_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT2);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT3);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_STC);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT2);
	m_aCtrl_LaneFacts1.Add(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT3);

	m_aCtrl_EpsilonFact.Add(IDC_CMD_MVHL_EPSILON_FACT_GRP);
	m_aCtrl_EpsilonFact.Add(IDC_CMD_MVHL_EPSILON_FACT_RDO1);
	m_aCtrl_EpsilonFact.Add(IDC_CMD_MVHL_EPSILON_FACT_RDO2);

	m_aCtrl_OKCancel.Add(IDOK);
	m_aCtrl_OKCancel.Add(IDCANCEL);
	m_aCtrl_OKCancel.Add(IDC_CMD_APPLY);
}

CCMMvhlItemStdRusDlg::~CCMMvhlItemStdRusDlg()
{
}

void CCMMvhlItemStdRusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdRusDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,        m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME,            m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_TYPE_COMBO,      m_cmbVehicleType);
	DDX_Control(pDX, IDC_CMD_MVHL_BRIDGE_TYPE_CMB, m_cmbBridgeType);
	DDX_Control(pDX, IDC_CMD_MVHL_PTLIST, m_List);
	DDX_Control(pDX, IDC_CMD_MVHL_K_EDT,  m_edtK);
	DDX_Control(pDX, IDC_CMD_MVHL_W_EDT,  m_edtW);
	DDX_Control(pDX, IDC_CMD_MVHL_W_UNT,  m_untW);
	DDX_Control(pDX, IDC_CMD_MVHL_D_EDT,  m_edtD);
	DDX_Control(pDX, IDC_CMD_MVHL_D_UNT,  m_untD);
	DDX_Radio  (pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO,           m_nDynaFact);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT,      m_edtMatlType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT,    m_edtBridgeType);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_EDT,      m_edtAutoDynaFact);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT,      m_edtUserDynaFact);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_EDT,  m_edtUserDynaFact_UDL);
	DDX_Check  (pDX, IDC_CMD_MVHL_FATIGUE_CHK,                  m_bFatigue);
	DDX_Check  (pDX, IDC_CMD_MVHL_SAME_LOADED_L_CHK,            m_bSameLoadedL);
	DDX_Radio  (pDX, IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO,           m_nLoadFact);
	DDX_Control(pDX, IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT,      m_edtUserLoadFact);
	DDX_Control(pDX, IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT,  m_edtUserLoadFact_UDL);
	DDX_Control(pDX, IDC_CMD_MVHL_RATIO_RATIO_EDT,  m_edtRatioEmptyCar);
	DDX_Control(pDX, IDC_CMD_MVHL_VARIABLE_EDT,  m_edtVariableD5);
	DDX_Control(pDX, IDC_CMD_MVHL_VARIABLE_UNT,  m_untVariableD5);
	DDX_Check  (pDX, IDC_CMD_MVHL_TWO_VEHI_CHK,       m_bTwoVehi);
	DDX_Control(pDX, IDC_CMD_MVHL_TWO_VEHI_FACT_EDT,  m_edtTwoVehi);
	DDX_Check  (pDX, IDC_CMD_MVHL_2ND_REDUC_CHK,      m_b2ndGrpReduc);
	DDX_Control(pDX, IDC_CMD_MVHL_2ND_REDUC_EDT,      m_edt2ndGrpReduc);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT,   m_edtLaneFacts1_Bogie[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT2,  m_edtLaneFacts1_Bogie[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT3,  m_edtLaneFacts1_Bogie[2]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT,     m_edtLaneFacts1_UDL[0]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT2,    m_edtLaneFacts1_UDL[1]);
	DDX_Control(pDX, IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT3,    m_edtLaneFacts1_UDL[2]);
	DDX_Radio  (pDX, IDC_CMD_MVHL_EPSILON_FACT_RDO1,       m_nEpsilonFact);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdRusDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdRusDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,        OnCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_TYPE_COMBO,      OnCmdMvhlTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_BRIDGE_TYPE_CMB, OnCmdMvhlBridgeTypeCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_FATIGUE_CHK,        OnCmdMvhlFatigueChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO, OnCmdMvhlDynaFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNA_FACT_USER_RDO, OnCmdMvhlDynaFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO, OnCmdMvhlLoadFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_LOAD_FACT_USER_RDO, OnCmdMvhlLoadFactRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_TWO_VEHI_CHK,       OnCmdMvhlTwoVehiChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_2ND_REDUC_CHK,      OnCmdMvhl2ndReducChk)
	ON_BN_CLICKED(IDC_CMD_APPLY,				           OnCmdMvhlApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CCMMvhlItemStdRusDlg::InitUnit()
{
	m_edtW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untW.SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_edtD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untD.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtVariableD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untVariableD5.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdRusDlg::InitCombo()
{
	int nCodeSeq[2];
	nCodeSeq[0] = D_MVHL_RUS_ROAD_BRG_AND_RAILBRG;
	nCodeSeq[1] = D_MVHL_RUS_ROAD_WALKWAY_AND_PBRG;
	
	for(int i=0; i<2; i++) 
	{
		CDlgUtil::CobxAddItem(m_cmbCodeName, CVehlDB::GetTypeNameAr()[nCodeSeq[i]], nCodeSeq[i]);
	}

	// hslee : 이거 CVehlDB::m_aVehiList에 넣어 놓고 쓰도록 수정 해야 함.. 시간이 없어서 리팩토링 못함...
	if(m_bModify)
	{
		if(m_Data.VehicleTypeName == _T("Uniform Load"))
		{
            const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Pedestrian Bridge"));
            const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway with AK"));
            const auto& str3 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway without AK"));

			m_cmbBridgeType.AddString(str1);
			m_cmbBridgeType.AddString(str2);
			m_cmbBridgeType.AddString(str3);
		}
		else if(m_Data.VehicleTypeName == _T("Uniform Load(w/o Other Loads)"))
		{
            const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Railroad Bridge"));
            const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Ballast-track Subway Bridge"));
            const auto& str3 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Other Bridge"));

            m_cmbBridgeType.AddString(str1);
            m_cmbBridgeType.AddString(str2);
            m_cmbBridgeType.AddString(str3);
		}
		else if(m_Data.VehicleTypeName == _T("Concentrated Load (w/o other loads)"))
		{
			const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Town Bridge"));
			const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Other Bridge"));

            m_cmbBridgeType.AddString(str1);
            m_cmbBridgeType.AddString(str2);
		}
		else // Default 값을 채워준다.
		{
			const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Pedestrian Bridge"));
			const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway with AK"));
			const auto& str3 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway without AK"));

            m_cmbBridgeType.AddString(str1);
            m_cmbBridgeType.AddString(str2);
            m_cmbBridgeType.AddString(str3);
		}
			
		AdjustComboListBox(m_cmbBridgeType);

		m_cmbBridgeType.SetCurSel(GetIndex_BridgeType());
	}
	else
	{
		m_cmbBridgeType.ResetContent();
        
        const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Pedestrian Bridge"));
        const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway with AK"));
        const auto& str3 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway without AK"));

        m_cmbBridgeType.AddString(str1);
        m_cmbBridgeType.AddString(str2);
        m_cmbBridgeType.AddString(str3);

		AdjustComboListBox(m_cmbBridgeType);

		m_cmbBridgeType.SetCurSel(0);
	}
}

void CCMMvhlItemStdRusDlg::ChangeCombo()
{
	UpdateData(TRUE);

	if(m_nVehicleIndex == m_cmbVehicleType.GetCurSel()) return;
	m_nVehicleIndex = m_cmbVehicleType.GetCurSel();
	
	if(m_cmbCodeName.GetCurSel() != 1) return;

	m_cmbBridgeType.ResetContent();
	
	if(m_cmbVehicleType.GetCurSel() == 0)
	{
        const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Pedestrian Bridge"));
        const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway with AK"));
        const auto& str3 = ConvBridgeTypeStrRaw2Trans(_T("Bridge Walkway without AK"));

        m_cmbBridgeType.AddString(str1);
        m_cmbBridgeType.AddString(str2);
        m_cmbBridgeType.AddString(str3);
	}
	else if(m_cmbVehicleType.GetCurSel() == 1)
	{
		const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Railroad Bridge"));
		const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Ballast-track Subway Bridge"));
		const auto& str3 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Other Bridge"));

        m_cmbBridgeType.AddString(str1);
        m_cmbBridgeType.AddString(str2);
        m_cmbBridgeType.AddString(str3);
	}
    else if (m_cmbVehicleType.GetCurSel() == 2)
    {
        const auto& str1 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Town Bridge"));
        const auto& str2 = ConvBridgeTypeStrRaw2Trans(_T("Walkway for Other Bridge"));

        m_cmbBridgeType.AddString(str1);
        m_cmbBridgeType.AddString(str2);
    }
	else
	{
		ASSERT(0);
	}

	AdjustComboListBox(m_cmbBridgeType);

	m_cmbBridgeType.SetCurSel(0);
}

void CCMMvhlItemStdRusDlg::SetDynaFactEditBox()
{
	T_MVCT_D MvctD; MvctD.Initialize();
	m_pDoc->m_pAttrCtrl->GetMvct(MvctD); 

	CString strVehicleType = GetVehicleTypeStr();

	if(strVehicleType == _T("AK"))
	{
		if(MvctD.nMatType_AK == 0)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL));
			
			if(MvctD.nBridgeType_AK == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType_AK == 1)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2_LAMBDA_FATIGUE));
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(MvctD.nMatType_AK == 1)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC));
			
			if(MvctD.nBridgeType_AK == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1_LAMBDA_FATIGUE));
			}
		}
	}
	else
	{
		if(MvctD.nMatType == 0)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL));
			
			if(MvctD.nBridgeType == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType == 1)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType == 2)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType == 3)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType == 4)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5_LAMBDA_FATIGUE));
			}
			else
			{
				ASSERT(0);
			}
		}
		else if(MvctD.nMatType == 1)
		{
			m_edtMatlType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC));
			
			if(MvctD.nBridgeType == 0)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType == 1)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2_LAMBDA_FATIGUE));
			}
			else if(MvctD.nBridgeType == 2)
			{
				m_edtBridgeType.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3));
				
				if(m_bFatigue) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3_LAMBDA_FATIGUE));
				else           m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3_LAMBDA));
				
				if(strVehicleType == _T("SK Fatigue")) m_edtAutoDynaFact.SetWindowText(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3_LAMBDA_FATIGUE));
			}
		}
	}

	if(strVehicleType == _T("NK-80"))
	{
		m_edtAutoDynaFact.SetWindowText(_STR(_T("1.1 ~ 1.3")));
	}
}

void CCMMvhlItemStdRusDlg::ChangeDynaFactText()
{
	CString strVehicleType = GetVehicleTypeStr();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dynamic Factor
	CString strDynaFact_Auto = _T("");
	CString strDynaFact_User = _T("");
	CString strDynaFact_UserUDL = _T("");

	if(m_bFatigue)
	{	
		strDynaFact_Auto = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_FATIGUE);

		if(strVehicleType == _T("AK"))  strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AK_BOGIE_FATIGUE);
		else                        strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_NOT_AK_FATIGUE);

		strDynaFact_UserUDL = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_USER_UDL_FATIGUE);
	}
	else
	{
		strDynaFact_Auto = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO);
		
		if(strVehicleType == _T("AK"))  strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AK_BOGIE);
		else                        strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_NOT_AK);
		
		strDynaFact_UserUDL = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_USER_UDL);
	}

	if(strVehicleType == _T("SK Fatigue"))
	{
		strDynaFact_Auto = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_FATIGUE);
		
		if(strVehicleType == _T("AK"))  strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AK_BOGIE_FATIGUE);
		else                        strDynaFact_User = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_NOT_AK_FATIGUE);
		
		strDynaFact_UserUDL = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_USER_UDL_FATIGUE);
	}

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_STC)->SetWindowText(strDynaFact_Auto);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_STC)->SetWindowText(strDynaFact_User);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_STC)->SetWindowText(strDynaFact_UserUDL);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dynamic Factor - Auto Calculation
	CString strDynaAutoCalc = _T("");
	
	if(strVehicleType == _T("SK") || strVehicleType == _T("SK Fatigue") || strVehicleType == _T("Subway Trains") ||
		 strVehicleType == _T("Tramcars"))
	{
		strDynaAutoCalc = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_CALC_SNIP_SP);
	}
	else if(strVehicleType == _T("AK") || strVehicleType == _T("NK-80"))
	{
		strDynaAutoCalc = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_CALC_SNIP);
	}
	else
	{
		strDynaAutoCalc = _LS(IDS_CMD_MVHL_DYNAMIC_FACTOR_AUTO_CALC);
	}
	
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO)->SetWindowText(strDynaAutoCalc);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Load Reliability Factor 
	CString strLoadFact = _T("");
	
	if(strVehicleType == _T("AK"))
	{
		strLoadFact = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AK_BOGIE);
	}
	else
	{
		strLoadFact = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_NOT_AK);
	}
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_STC)->SetWindowText(strLoadFact);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void CCMMvhlItemStdRusDlg::ChangeLoadFactText()
{
	CString strVehicleType = GetVehicleTypeStr();
	
	CString strLoadFactText = _T("");
	
	if(strVehicleType == _T("SK") || strVehicleType == _T("Subway Trains") || strVehicleType == _T("Tramcars"))
	{
		if(m_bFatigue) strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE_SNIP_SP);
		else           strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_SNIP_SP);		
	}
	else if(strVehicleType == _T("SK Fatigue")) 
	{
		strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE_SNIP_SP);
	}
	else if(strVehicleType == _T("AK"))
	{
		if(m_bFatigue) strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE_SNIP);
		else           strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_SNIP);		
	}
	else
	{
		if(m_bFatigue) strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO_FATIGUE);
		else           strLoadFactText = _LS(IDS_CMD_MVHL_LOAD_RELIABILITY_FACTOR_AUTO);
	}

	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO)->SetWindowText(strLoadFactText);
}

void CCMMvhlItemStdRusDlg::AlignControl()
{
	UpdateData(TRUE);

	CString strVehicleType = GetVehicleTypeStr();
	CString strBridgeType  = GetBridgeTypeStr();
	
	CRect rRef;
	CRect rToMove;
	int nDistX = 0;
	int nDistY = 0;	

	// K 
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_K_STC)->GetWindowRect(rToMove);
	
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(3);
 	nDistX = rRef.left - rToMove.left;
	
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrl_K, nDistX, nDistY);

	// W & D
	GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_W_STC)->GetWindowRect(rToMove);
	
	nDistY = rRef.top - rToMove.top + globalUtils.ScaleByDPI(3);
	nDistX = rRef.left - rToMove.left;
	
	CDlgUtil::CtrlMoveDistXY(this, m_aCtrl_W, nDistX, nDistY);

	if(strVehicleType == _T("AK") || strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)") )
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_K_STC)->GetWindowRect(rToMove);

		nDistY = rRef.top - rToMove.top;
		nDistX = rRef.right - rToMove.left + globalUtils.ScaleByDPI(20);

		CDlgUtil::CtrlMoveDistXY(this, m_aCtrl_K, nDistX, nDistY);
	}

	if(strVehicleType == _T("Subway Trains") || strVehicleType == _T("Tramcars"))
	{
		GetDlgItem(IDC_CMD_MVHL_PIC_WND)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_IDC_CMD_MVHL_LOADED_STC)->GetWindowRect(rToMove);
		
		nDistY = rRef.bottom + globalUtils.ScaleByDPI(10) - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl_TramTop, nDistY);

		GetDlgItem(IDC_CMD_MVHL_IDC_CMD_MVHL_LOADED_STC)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rToMove);

		nDistY = rRef.bottom + globalUtils.ScaleByDPI(7) - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl_List, nDistY);

		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_VARIABLE_EDT)->GetWindowRect(rToMove);
		
		nDistY = rRef.bottom + globalUtils.ScaleByDPI(5) - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl_TramBot, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_PIC_WND)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rToMove);
		
		nDistY = rRef.bottom + globalUtils.ScaleByDPI(5) - rToMove.top;
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl_List, nDistY);
	}
	
	// Group Box 크기
	BOOL bHideDynaGrp = FALSE;
	BOOL bHideLoadGrp = FALSE; // Load Reliability Factor
	BOOL bHideEpsilon = FALSE; // Epsilon Factor

	CRect rectTarget;
	CRect rect;
	CRect rectShort;
	
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rectShort);
	
	if(strVehicleType == _T("SK") || strVehicleType == _T("SK Fatigue"))
	{
		GetDlgItem(IDC_CMD_MVHL_K_EDT)->GetWindowRect(rect);
		nDistY = rect.bottom - rect.top;
	}
	else if(strVehicleType == _T("AK")            || strVehicleType == _T("N14")      || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)") || strVehicleType == _T("N11(2nd edition)") ||
		      strVehicleType == _T("NK-80"))
	{
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rect);
		nDistY = rect.bottom - rect.top;
	}
	else if(strVehicleType == _T("Subway Trains"))
	{
		GetDlgItem(IDC_CMD_MVHL_IDC_CMD_MVHL_LOADED_STC)->GetWindowRect(rectTarget);
		GetDlgItem(IDC_CMD_MVHL_PTLIST)->GetWindowRect(rect);
		nDistY = rect.bottom - rectTarget.top;
	}
	else if(strVehicleType == _T("Tramcars"))
	{
		GetDlgItem(IDC_CMD_MVHL_IDC_CMD_MVHL_LOADED_STC)->GetWindowRect(rectTarget);
		GetDlgItem(IDC_CMD_MVHL_VARIABLE_EDT)->GetWindowRect(rect);
		nDistY = rect.bottom - rectTarget.top;
	}
	else if(strVehicleType == _T("NG-60"))
	{
		GetDlgItem(IDC_CMD_MVHL_W_STC)->GetWindowRect(rectTarget);
		GetDlgItem(IDC_CMD_MVHL_D_STC)->GetWindowRect(rect);
		nDistY = rect.bottom - rectTarget.top;
	}
	else if(strVehicleType == _T("Uniform Load") || strVehicleType == _T("Uniform Load(w/o Other Loads)") ||
		      strVehicleType == _T("Concentrated Load (w/o other loads)"))
	{
		bHideDynaGrp = TRUE;
	}

	GetDlgItem(IDC_CMD_MVHL_NAME)->GetWindowRect(rectTarget);
	GetDlgItem(IDC_CMD_MVHL_PIC_WND)->GetWindowRect(rect);
	nDistY += rect.bottom - rectTarget.top + globalUtils.ScaleByDPI(40);

	rectShort.bottom = rectShort.top + nDistY;

	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->MoveWindow(rectShort);

	///////////////////////////////////////////////////////////////////////
	// Fatigue
	GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_Fatigue, nDistY);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Apply same Loaded Length between Bogie and UDL
	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_SAME_LOADED_L_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_SameLoadedL, nDistY);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Two Vehicle
// 	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
// 	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_GRP)->GetWindowRect(rToMove);
// 	nDistY = rRef.top - rToMove.top;
// 	CDlgUtil::CtrlMoveDistY(this, m_ACtrl_TwoVehi, nDistY);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Dynamic Factor

	// User Input Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_GRP)->GetWindowRect(rectShort);

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT)->GetWindowRect(rect);
	nDistY = rect.bottom - rect.top + globalUtils.ScaleByDPI(30);
	
	if(strVehicleType == _T("AK"))
	{
		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_EDT)->GetWindowRect(rect);
	#if !defined(_RUS)
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(5);
	#else
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(14);
	#endif
	}
	
	rectShort.bottom = rectShort.top + nDistY;
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_GRP)->MoveWindow(rectShort);

	// Dynamic Factor Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rectShort);

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_GRP)->GetWindowRect(rect);
	rectShort.bottom = rectShort.top + rect.bottom - rect.top + nDistY + globalUtils.ScaleByDPI(35);
#if defined(_RUS)
	rectShort.bottom += globalUtils.ScaleByDPI(9);
#endif
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->MoveWindow(rectShort);
	
	// Dynamic Factor 
// 	if     (strVehicleType == _T("N14") || strVehicleType == _T("N11"))       GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_GRP)->GetWindowRect(rRef);
// 	else if(strVehicleType == _T("SK") || strVehicleType == _T("SK Fatigue")) GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
// 	else                                                              GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
// 	
// 	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rToMove);
// 	
// 	nDistY = rRef.bottom - rToMove.top + 7;
	
	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rToMove);

	if(strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)"))
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	}
	else
	{
		nDistY = rRef.top - rToMove.top;
	}

	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_DynaFact, nDistY);

	if(strVehicleType == _T("AK"))
	{
		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_EDT)->GetWindowRect(rToMove);
		
	#if !defined(_RUS)
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
	#else
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(14);
	#endif
		CDlgUtil::CtrlMoveDistY(this, m_aCtrl_DynaFact_UDL, nDistY);
	}	
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Load Reliability Factor

	// User Input Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_GRP)->GetWindowRect(rectShort);
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT)->GetWindowRect(rect);
	nDistY = rect.bottom - rect.top + globalUtils.ScaleByDPI(30);
	
	if(strVehicleType == _T("AK"))
	{
		GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT)->GetWindowRect(rect);

	#if !defined(_RUS)
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(5);
	#else
		nDistY += rect.bottom - rect.top + globalUtils.ScaleByDPI(14);
	#endif
	}
	
	rectShort.bottom = rectShort.top + nDistY;
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_GRP)->MoveWindow(rectShort);
	
	// Load Factor Group Box 크기
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rectShort);
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO)->GetWindowRect(rect);
	rectShort.bottom = rectShort.top + rect.bottom - rect.top + nDistY + globalUtils.ScaleByDPI(45);
	ScreenToClient(rectShort);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->MoveWindow(rectShort);

// 	if(strVehicleType == _T("Uniform Load") || strVehicleType == _T("Uniform Load(w/o Other Loads)"))
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
// 	}
// 	else
// 	{
// 		GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rRef);
// 	}
// 	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rToMove);
// 	
// 	nDistY = rRef.bottom - rToMove.top + 7;
// 	
// 	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LoadFact, nDistY);

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rToMove);

	if(strVehicleType == _T("Uniform Load") || strVehicleType == _T("Uniform Load(w/o Other Loads)"))
	{
		nDistY = rRef.top - rToMove.top;
	}
	else
	{
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	}
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LoadFact, nDistY);
	
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT)->GetWindowRect(rToMove);
	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(3);
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LoadFact_UDL, nDistY);

	if     (strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)"))      bHideLoadGrp = TRUE;
	else if(strVehicleType == _T("Concentrated Load (w/o other loads)")) bHideLoadGrp = TRUE;
	else                                                             bHideLoadGrp = FALSE;
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Lane Factor (s1)
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_GRP)->GetWindowRect(rToMove);
	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(3);
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_LaneFacts1, nDistY);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Reduction for Limit State Group 2
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_2ND_REDUC_GRP)->GetWindowRect(rToMove);
	
	nDistY = rRef.top - rToMove.top;
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_2ndGrpReduc, nDistY);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Epsilon Factor
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_EPSILON_FACT_GRP)->GetWindowRect(rToMove);
	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(7);
	
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_EpsilonFact, nDistY);
	///////////////////////////////////////////////////////////////////////

	if(strVehicleType == _T("SK")) bHideEpsilon = FALSE;
	else                       bHideEpsilon = TRUE;

	///////////////////////////////////////////////////////////////////////
	// OK, Cancel, Apply
	if     ( bHideLoadGrp &&  bHideDynaGrp) GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	else if(!bHideLoadGrp &&  bHideDynaGrp) GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rRef);
	else if( bHideLoadGrp && !bHideDynaGrp) GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_GRP)->GetWindowRect(rRef);
	else                                    GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_GRP)->GetWindowRect(rRef);

	if(!bHideEpsilon) GetDlgItem(IDC_CMD_MVHL_EPSILON_FACT_GRP)->GetWindowRect(rRef);

	// 이것만 특별히 예외 Case - 이렇게 만들면 안되는데...음..
	if(strVehicleType == _T("Concentrated Load (w/o other loads)"))
	{
		GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rRef);
	}
	else if(strVehicleType == _T("AK"))
	{
		GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_GRP)->GetWindowRect(rRef);
	}
	else if(strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)"))
	{
		GetDlgItem(IDC_CMD_MVHL_2ND_REDUC_GRP)->GetWindowRect(rRef);
	}
	else if (strVehicleType == _T("SK Fatigue"))
	{
		GetDlgItem(IDC_CMD_MVHL_VEHICULAR_LOAD_PROPERTIES)->GetWindowRect(rRef);
	}

	CRect rBotLeftGrp;
	GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->GetWindowRect(rBotLeftGrp);
	rRef.bottom = __max(rRef.bottom, rBotLeftGrp.bottom);

	//가장 아래 있는 Control 16 아래 System Button을 놓는다.
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rToMove);	
	nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(16);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrl_OKCancel, nDistY);

	//System Button 기준으로 대화상자 크기를 지정한다.
	CRect rectLast;
	GetDlgItem(IDC_CMD_APPLY)->GetWindowRect(rectLast);

	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(r);

	RedrawWindow();
}

void CCMMvhlItemStdRusDlg::ControlsShowHide()
{
	UpdateData(TRUE);
	
	CString strVehicleType = GetVehicleTypeStr();
	CString strBridgeType  = GetBridgeTypeStr();
	
	CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,   TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_K,            TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_List,         TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_Fatigue,      TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  TRUE);
	CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact,     TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact,     TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   TRUE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  TRUE);
		
	if(strVehicleType == _T("SK"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_List,         FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_Fatigue,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
	}
	else if(strVehicleType == _T("SK Fatigue"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_List,         FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_Fatigue,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);
	}
	else if(strVehicleType == _T("AK"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,           FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);
	}
	else if(strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_Fatigue,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);

		if(strVehicleType == _T("N11(2nd edition)"))
		{
			CDlgUtil::CtrlShowHide(this, m_aCtrl_K, FALSE);
		}
	}
	else if(strVehicleType == _T("Subway Trains") || strVehicleType == _T("Tramcars") || strVehicleType == _T("NK-80"))
	{ 
		CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_K,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);
	}
	else if(strVehicleType == _T("NG-60"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_BridgeType,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_K,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_List,         FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);
	}
	else if(strVehicleType == _T("Uniform Load") || strVehicleType == _T("Uniform Load(w/o Other Loads)"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_K,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_List,         FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);
	}
	else if(strVehicleType == _T("Concentrated Load (w/o other loads)"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_K,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_W,            FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_List,         FALSE);
		CDlgUtil::CtrlShowHide(this, m_ACtrl_TwoVehi,      FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_SameLoadedL,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_DynaFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact,     FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LoadFact_UDL, FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_2ndGrpReduc,  FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_LaneFacts1,   FALSE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_EpsilonFact,  FALSE);
	}
	else 
	{
		ASSERT(0);
	}

	CDlgUtil::CtrlShowHide(this, m_aCtrl_TramTop, FALSE);
	CDlgUtil::CtrlShowHide(this, m_aCtrl_TramBot, FALSE);
	if(strVehicleType == _T("Subway Trains"))
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_TramTop, TRUE);
	}
	else if(strVehicleType == _T("Tramcars") )
	{
		CDlgUtil::CtrlShowHide(this, m_aCtrl_TramTop, TRUE);
		CDlgUtil::CtrlShowHide(this, m_aCtrl_TramBot, TRUE);
	}
}

void CCMMvhlItemStdRusDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_STC  )->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT  )->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_STC)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT)->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_STC  )->EnableWindow(m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_FACT_EDT  )->EnableWindow(m_nDynaFact==0);

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_STC    )->EnableWindow(m_nDynaFact==1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_EDT    )->EnableWindow(m_nDynaFact==1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_STC)->EnableWindow(m_nDynaFact==1);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_USER_FACT_UDL_EDT)->EnableWindow(m_nDynaFact==1);

	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_STC    )->EnableWindow(m_nLoadFact==1);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_EDT    )->EnableWindow(m_nLoadFact==1);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_STC)->EnableWindow(m_nLoadFact==1);
	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_USER_FACT_UDL_EDT)->EnableWindow(m_nLoadFact==1);

	CString strVehicleType = GetVehicleTypeStr();

	BOOL bUseDynaAuto = TRUE;
	if(/*strVehicleType == _T("AK") || */strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)") || strVehicleType == _T("NG-60")) bUseDynaAuto = FALSE;

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_STC)->EnableWindow(strVehicleType!=_T("NK-80") && m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_MATL_EDT)->EnableWindow(strVehicleType!=_T("NK-80") && m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_STC)->EnableWindow(strVehicleType!=_T("NK-80") && m_nDynaFact==0);
	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_BRIDGE_EDT)->EnableWindow(strVehicleType!=_T("NK-80") && m_nDynaFact==0);

	GetDlgItem(IDC_CMD_MVHL_DYNA_FACT_AUTO_RDO)->EnableWindow(bUseDynaAuto);

	//GetDlgItem(IDC_CMD_MVHL_FATIGUE_CHK)->EnableWindow(strVehicleType != _T("SK") && strVehicleType != _T("SK Fatigue"));

	BOOL bUseLoadAuto = TRUE;
	if(strVehicleType == _T("SK Fatigue") || strVehicleType == _T("N14") || strVehicleType == _T("N11") || strVehicleType == _T("N11(2nd edition)")  || strVehicleType == _T("NK-80") ||
		 strVehicleType == _T("NG-60") || strVehicleType == _T("Uniform Load") || strVehicleType == _T("Uniform Load(w/o Other Loads)") || 
		 strVehicleType == _T("Concentrated Load (w/o other loads)")) bUseLoadAuto = FALSE;

	GetDlgItem(IDC_CMD_MVHL_LOAD_FACT_AUTO_RDO)->EnableWindow(bUseLoadAuto);

	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_FACT_STC)->EnableWindow(m_bTwoVehi);
	GetDlgItem(IDC_CMD_MVHL_TWO_VEHI_FACT_EDT)->EnableWindow(m_bTwoVehi);
	GetDlgItem(IDC_CMD_MVHL_2ND_REDUC_EDT)->EnableWindow(m_b2ndGrpReduc);

	// 첫번째 값은 비활성화 처리 한다.
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_BOGIE_EDT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CMD_MVHL_LANE_FACT_S1_UDL_EDT)->EnableWindow(FALSE);
}

void CCMMvhlItemStdRusDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_CMD__ADDD__No), _LS(IDS_WG_CMD__ADDD__Load), _LS(IDS_WG_CMD__ADDD__Spacing)};
	
	// title에 단위 추가
	CString sLdUnit,sDtUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad,sLdUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance,sDtUnit);
	aTitle[1] = aTitle[1] +_T("(")+sLdUnit+_T(")");
	aTitle[2] = aTitle[2] +_T("(")+sDtUnit+_T(")");
	
	int nColWidth[COLCOUNT];
	
	CString title;
	int i;
	LV_COLUMN lvcolumn;
	
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	
	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 80;
	
	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;
		
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

BOOL CCMMvhlItemStdRusDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;
	
	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) 
		{
			if(m_Data.VehicleTypeName == _T("AK")  || m_Data.VehicleTypeName == _T("N14") || m_Data.VehicleTypeName == _T("N11"))
			{
				str.Format(_T("%gK"), dLoad);
			}
			else
			{
				str.Format(_T("%g"), dLoad);
			}
		}
		else if (i == 2)
		{
			double dDistTmp = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, dDist);

			if (dDistTmp == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (dDistTmp == -1.0) str = _LS(IDS_CMD_BUCK_Variable);
			else str.Format(_T("%g"), dDist); 
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;
		
		if(i == 0)
			m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
	
	return TRUE;
}

void CCMMvhlItemStdRusDlg::MakeItemEx()
{
	m_List.DeleteAllItems();    
	int nLoadCount = GetLoadCount();
	int nDistCount = GetDistCount();
	
	if (nLoadCount > 0)
	{
		int i = 0;
		for (i = 0; i < nDistCount; i++)
			InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);
		
		if (nLoadCount > nDistCount)
			InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
	}
	
	UpdateData(FALSE);
}

int CCMMvhlItemStdRusDlg::GetLoadCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdRusDlg::GetDistCount()
{
	int i = 0;
	for(i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdRusDlg::ChangeBitmap(int nIndex, int nBitmap, int nBridgeType)
{
	CString astrSVG0[] = {
		_T("cmd_mvhl_99.svg"),   _T("cmd_mvhl_100.svg"),   _T("cmd_mvhl_101.svg"), _T("cmd_mvhl_102.svg"), 
		_T("cmd_mvhl_103.svg"), _T("cmd_mvhl_106.svg"), 	_T("cmd_mvhl_104.svg"),  _T("cmd_mvhl_105.svg"),   
		_T("cmd_mvhl_106.svg"), _T("cmd_mvhl_107.svg")
	};
	CString astrSVG1[] = {
		_T("cmd_mvhl_108.svg"), _T("cmd_mvhl_109.svg"),  _T("cmd_mvhl_108.svg"),   _T("cmd_mvhl_110.svg"), _T("cmd_mvhl_110.svg"),
		_T("cmd_mvhl_108.svg"), _T("cmd_mvhl_111.svg"),  _T("cmd_mvhl_112.svg")
	};
		
	CString strSVG; 
	int nSelBitmap = 0;
	switch(nIndex)
	{
		case D_MVHL_RUS_ROAD_BRG_AND_RAILBRG:        strSVG = astrSVG0[nBitmap];  break;
		case D_MVHL_RUS_ROAD_WALKWAY_AND_PBRG:  
			{
				if(nBitmap == 0)
				{
					if     (nBridgeType == 0) nSelBitmap = 0;
					else if(nBridgeType == 1) nSelBitmap = 1;
					else if(nBridgeType == 2) nSelBitmap = 2;
					else ASSERT(0);
				}
				else if(nBitmap == 1)
				{
					if     (nBridgeType == 0) nSelBitmap = 3;
					else if(nBridgeType == 1) nSelBitmap = 4;
					else if(nBridgeType == 2) nSelBitmap = 5;
					else ASSERT(0);
				}
				else if(nBitmap == 2)
				{
					if     (nBridgeType == 0) nSelBitmap = 6;
					else if(nBridgeType == 1) nSelBitmap = 7;
					else ASSERT(0);
				}
				else ASSERT(0);

				strSVG = astrSVG1[nSelBitmap];
			}
			break;
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdRusDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode;
	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleLoadName));
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex, GetIndex_BridgeType());

	m_cmbBridgeType.SetCurSel(GetIndex_BridgeType());
	
	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data))	MakeItemEx();  

	//if(m_Data.VehicleTypeName == _T("N11")) m_Data.nRussiaK = 11;
	
	m_edtK.SetEditUnit(m_Data.nRussiaK);
	m_edtW.SetEditUnit(m_Data.dRussiaW);
	m_edtD.SetEditUnit(m_Data.dRussiaD);
	
	m_nDynaFact = m_Data.nDynaFactor;
	m_edtUserDynaFact.SetEditUnit(m_Data.dDynaFactor);
	m_edtUserDynaFact_UDL.SetEditUnit(m_Data.dDynaFactor_UDL);

	m_edtRatioEmptyCar.SetEditUnit(m_Data.dRatioEmptyCar);
	m_edtVariableD5.SetEditUnit(m_Data.dRussiaVariable);

	m_bFatigue = m_Data.bFatigue;
	m_bSameLoadedL = m_Data.bSameLoadedL;
	m_nLoadFact = m_Data.nLoadFactor;
	m_edtUserLoadFact.SetEditUnit(m_Data.dLoadFactor);
	m_edtUserLoadFact_UDL.SetEditUnit(m_Data.dLoadFactor_UDL);

	m_bTwoVehi = m_Data.bTwoVehicle;
	m_edtTwoVehi.SetEditUnit(m_Data.dTwoVehicleFactor);
	m_b2ndGrpReduc = m_Data.b2ndGrpReducFactor;
	m_edt2ndGrpReduc.SetEditUnit(m_Data.d2ndGrpReducFactor);
	m_edtLaneFacts1_Bogie[0].SetEditUnit(m_Data.dLaneFactors1[0]);
	m_edtLaneFacts1_Bogie[1].SetEditUnit(m_Data.dLaneFactors1[1]);
	m_edtLaneFacts1_Bogie[2].SetEditUnit(m_Data.dLaneFactors1[2]);
	m_edtLaneFacts1_UDL[0].SetEditUnit(m_Data.dLaneFactors1_UDL[0]);
	m_edtLaneFacts1_UDL[1].SetEditUnit(m_Data.dLaneFactors1_UDL[1]);
	m_edtLaneFacts1_UDL[2].SetEditUnit(m_Data.dLaneFactors1_UDL[2]);
	m_nEpsilonFact = m_Data.nEpsilonFact;
	
	SetDynaFactEditBox();
	ChangeDynaFactText();
	ChangeLoadFactText();
	
	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdRusDlg::Dlg2Data()
{
	UpdateData(TRUE);
	
	m_Data.Initialize();  
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);
	m_Data.bStandard = TRUE;

    m_Data.VehicleTypeName = GetVehicleTypeStr();
    m_Data.strBridgeType = GetBridgeTypeStr();
	
	m_Data.nStandardCode = m_nStndCode;
	
	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  
	
	m_Data.nRussiaK = m_edtK.GetEditValue();
	m_Data.dRussiaW = m_edtW.GetEditValue();
	m_Data.dRussiaD = m_edtD.GetEditValue();
	m_Data.nDynaFactor = m_nDynaFact;
	m_Data.dDynaFactor = m_edtUserDynaFact.GetEditValue();
	m_Data.dDynaFactor_UDL = m_edtUserDynaFact_UDL.GetEditValue();
	m_Data.dRatioEmptyCar = m_edtRatioEmptyCar.GetEditValue();
	m_Data.dRussiaVariable = m_edtVariableD5.GetEditValue();

	m_Data.bFatigue = m_bFatigue;
	m_Data.bSameLoadedL = m_bSameLoadedL;
	m_Data.nLoadFactor = m_nLoadFact;
	m_Data.dLoadFactor = m_edtUserLoadFact.GetEditValue();
	m_Data.dLoadFactor_UDL = m_edtUserLoadFact_UDL.GetEditValue();

	m_Data.bTwoVehicle = m_bTwoVehi;
	m_Data.dTwoVehicleFactor = m_edtTwoVehi.GetEditValue();
	m_Data.b2ndGrpReducFactor = m_b2ndGrpReduc;		
	m_Data.d2ndGrpReducFactor = m_edt2ndGrpReduc.GetEditValue();
	m_Data.dLaneFactors1[0] = m_edtLaneFacts1_Bogie[0].GetEditValue();
	m_Data.dLaneFactors1[1] = m_edtLaneFacts1_Bogie[1].GetEditValue();
	m_Data.dLaneFactors1[2] = m_edtLaneFacts1_Bogie[2].GetEditValue();
	m_Data.dLaneFactors1_UDL[0] = m_edtLaneFacts1_UDL[0].GetEditValue();
	m_Data.dLaneFactors1_UDL[1] = m_edtLaneFacts1_UDL[1].GetEditValue();
	m_Data.dLaneFactors1_UDL[2] = m_edtLaneFacts1_UDL[2].GetEditValue();
	m_Data.nEpsilonFact = m_nEpsilonFact;

	return TRUE;
}

BOOL CCMMvhlItemStdRusDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;
	
	return TRUE;
}

BOOL CCMMvhlItemStdRusDlg::ChangeData()
{
	m_Data.Initialize();
	m_Data.bStandard = TRUE;

    m_Data.VehicleTypeName = GetVehicleTypeStr();
    m_Data.strBridgeType = GetBridgeTypeStr();

	m_Data.nStandardCode = m_nStndCode;
	m_Data.dRussiaVariable = M_InitValueCurUnit(8.5, N, M, D_UNITSYS_BASE_LENGTH);

	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_cmbVehicleType.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode, sIndex, GetIndex_BridgeType());
	
	CVehlDB db(m_pDoc);
	if(db.GetStandardVehicleLoadValue(m_Data)) MakeItemEx();    

	if     (m_Data.VehicleTypeName == _T("N11")             ) m_Data.nRussiaK = 11;
	else if(m_Data.VehicleTypeName == _T("N11(2nd edition)")) m_Data.nRussiaK = 1;
	
	if(m_Data.VehicleTypeName == _T("AK")  || m_Data.VehicleTypeName == _T("N14") ||
		 m_Data.VehicleTypeName == _T("N11") || m_Data.VehicleTypeName == _T("N11(2nd edition)") || m_Data.VehicleTypeName == _T("NG-60"))
	{
		m_Data.nDynaFactor = 1;
	}
 	if     (m_Data.VehicleTypeName == _T("AK"))    m_Data.dDynaFactor = 1.4;
 	else if(m_Data.VehicleTypeName == _T("NG-60")) m_Data.dDynaFactor = 1.1;
	else                                       m_Data.dDynaFactor = 1.0;

	// Fatigue
	if(m_Data.VehicleTypeName == _T("SK") || m_Data.VehicleTypeName == _T("SK Fatigue"))
	{
		m_Data.bFatigue = FALSE;
	}

	// Load Reliability Factor
	if(m_Data.VehicleTypeName == _T("SK")) 
	{
		m_Data.nLoadFactor = 0;
		m_Data.dLoadFactor = 1.3;
		m_Data.dLoadFactor_UDL = 1.0;
	}
	else if(m_Data.VehicleTypeName == _T("SK Fatigue")) 
	{
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.0;
		m_Data.dLoadFactor_UDL = 1.0;
	}
	else if(m_Data.VehicleTypeName == _T("AK"))
	{
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.5;
		m_Data.dLoadFactor_UDL = 1.25;  //1.15->1.25로 변경
	}
	else if(m_Data.VehicleTypeName == _T("N14") || m_Data.VehicleTypeName == _T("N11") || m_Data.VehicleTypeName == _T("N11(2nd edition)"))
	{
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.1;
		m_Data.dLoadFactor_UDL = 1.1;
	}
	else if(m_Data.VehicleTypeName == _T("Subway Trains") || m_Data.VehicleTypeName == _T("Tramcars"))
	{
		m_Data.nLoadFactor = 0;
		m_Data.dLoadFactor = 1.0;
		m_Data.dLoadFactor_UDL = 1.0;
	}
	else if(m_Data.VehicleTypeName == _T("NK-80") || m_Data.VehicleTypeName == _T("NG-60"))
	{
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.0;
		m_Data.dLoadFactor_UDL = 1.0;
	}
	else if(m_Data.VehicleTypeName == _T("Uniform Load"))
	{
		if(m_Data.strBridgeType == _T("Pedestrian Bridge") || m_Data.strBridgeType == _T("Bridge Walkway without AK"))
		{
			m_Data.nLoadFactor = 1;
			m_Data.dLoadFactor = 1.4;
			m_Data.dLoadFactor_UDL = 1.0;
		}
		else if(m_Data.strBridgeType == _T("Bridge Walkway with AK"))
		{
			m_Data.nLoadFactor = 1;
			m_Data.dLoadFactor = 1.2;
			m_Data.dLoadFactor_UDL = 1.0;
		}
	}	
	else if(m_Data.VehicleTypeName == _T("Uniform Load(w/o Other Loads)")) // 3개 차량 모드 같은 값 
	{
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.4;
		m_Data.dLoadFactor_UDL = 1.0;
	}
	else if(m_Data.VehicleTypeName == _T("Concentrated Load (w/o other loads)"))
	{
		// Nothing
	}
	else
	{
		ASSERT(0);
	}
	
	Data2Dlg();
	
	m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleTypeName));
	
	return TRUE;
}

void CCMMvhlItemStdRusDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;
	
	m_cmbVehicleType.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && CVehlDB::GetTypeListAr()[nIndex][i] != _T(""))
	{
        const auto& strTranslation = 
            ConvVehicleTypeStrRaw2Trans(CVehlDB::GetTypeListAr()[nIndex][i++]);

		m_cmbVehicleType.AddString(strTranslation);
	}

	AdjustComboListBox(m_cmbVehicleType);

	if(nIndex == D_MVHL_RUS_ROAD_BRG_AND_RAILBRG) m_cmbVehicleType.SetCurSel(2);
	else            m_cmbVehicleType.SetCurSel(0); 
}

void CCMMvhlItemStdRusDlg::AdjustComboListBox(CComboBox& Cbx)
/*
콤보박스의 길이를 현재 사용되고 있는 리스트의 길이에
맞추어 변경한다.
*/
{
	CString str;
	CSize   sz;
	int     dx = 0;
	CDC*    pDC = Cbx.GetDC();
	for (int i = 0; i < Cbx.GetCount(); i++)
	{
		Cbx.GetLBText(i, str);
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	Cbx.ReleaseDC(pDC);

	if (Cbx.GetDroppedWidth() < dx)
	{
		Cbx.SetDroppedWidth(dx);
		ASSERT(Cbx.GetDroppedWidth() == dx);
	}
}


void CCMMvhlItemStdRusDlg::GetIndex(CString &csName, int *sitem)
{
	for (int i = 0; i < D_MAX_COUNT; i++)
	{
		if(CVehlDB::GetTypeListAr()[m_Data.nStandardCode][i] == csName)
		{
			if(sitem) *sitem = i;
			return ;
		}
	}
	
	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
}

int CCMMvhlItemStdRusDlg::GetIndex_BridgeType()
{
	int sIndex = 0;
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	if(m_Data.nStandardCode != D_MVHL_RUS_ROAD_WALKWAY_AND_PBRG)
	{
		return 0;
	}
	
	if(sIndex == 0)
	{
		if     (m_Data.strBridgeType == _T("Pedestrian Bridge"))         return 0;
		else if(m_Data.strBridgeType == _T("Bridge Walkway with AK"))    return 1;
		else if(m_Data.strBridgeType == _T("Bridge Walkway without AK")) return 2;
		else				         																				 return 0;
	}
	else if(sIndex == 1)
	{
		if     (m_Data.strBridgeType == _T("Walkway for Railroad Bridge"))              return 0;
		else if(m_Data.strBridgeType == _T("Walkway for Ballast-track Subway Bridge"))  return 1;
		else if(m_Data.strBridgeType == _T("Walkway for Other Bridge"))                 return 2;
		else																											                  return 0;
	}
	else if(sIndex == 2)
	{
		if     (m_Data.strBridgeType == _T("Walkway for Town Bridge"))  return 0;
		else if(m_Data.strBridgeType == _T("Walkway for Other Bridge")) return 1;
		else																											  return 0;
	}
	else
	{
		ASSERT(0); return 0;			
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdRusDlg message handlers

BOOL CCMMvhlItemStdRusDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("AK");
		m_nStndCode = D_MVHL_RUS_ROAD_BRG_AND_RAILBRG;
		m_Data.nStandardCode = D_MVHL_RUS_ROAD_BRG_AND_RAILBRG;
		m_Data.nDynaFactor = 1;
		m_Data.dDynaFactor = 1.4;
		m_Data.dRussiaVariable = M_InitValueCurUnit(8.5, N, M, D_UNITSYS_BASE_LENGTH);
		m_Data.nLoadFactor = 1;
		m_Data.dLoadFactor = 1.5;
		m_Data.dLoadFactor_UDL = 1.25; //1.15->1.25로 변경
	}
	
	GetIndex(m_Data.VehicleTypeName);
	int nIndex = m_Data.nStandardCode;
	if (nIndex != D_MVHL_RUS_ROAD_BRG_AND_RAILBRG && nIndex != D_MVHL_RUS_ROAD_WALKWAY_AND_PBRG) return TRUE;

	InitUnit();
	InitCombo();
	SetHeaderTitle();

	SetLoadTypeList(nIndex);
	
	Data2Dlg();

	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
	
	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvhlItemStdRusDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);
	
	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex);
	
	ChangeCombo();
	
	ChangeData();
	
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlTypeCmb() 
{
	ChangeCombo();

	ChangeData();  
	
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlBridgeTypeCmb()
{
	ChangeData();
	
	AlignControl();
	ControlsShowHide();
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlFatigueChk()
{
	UpdateData(TRUE);
	
	SetDynaFactEditBox();
	ChangeDynaFactText();
	ChangeLoadFactText();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlDynaFactRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlLoadFactRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlTwoVehiChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhl2ndReducChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdRusDlg::OnCmdMvhlApply() 
{	
	ApplyOrOK();
}

typedef std::pair<CString, CString> pair_cstring;
template <std::size_t N> using array_pair_cstring = std::array<pair_cstring, N>;

template <std::size_t N>
CString ConvStrRaw2Trans(const array_pair_cstring<N>& aPairStr, 
                         const CString& strRaw)
{
    for ( const auto& strPair : aPairStr )
    {
        const auto& strTarget = strPair.first;
        if ( strRaw == strTarget )
        {
            return strPair.second;
        }
    }

    return strRaw;
}

template <std::size_t N>
CString ConvStrTrans2Raw(const array_pair_cstring<N>& aPairStr,
                         const CString& strTrans)
{
    for ( const auto& strPair : aPairStr )
    {
        const auto& strTarget = strPair.second;
        if ( strTrans == strTarget )
        {
            return strPair.first;
        }
    }

    return strTrans;
}

#define __static_caVehicleLoadType static const array_pair_cstring<12> caVehicleLoadType = \
{ \
    std::make_pair(_T("SK"),                                  _LS(IDS_WG_CMD_SK)), \
    std::make_pair(_T("SK Fatigue"),                          _LS(IDS_WG_CMD_SK_FATIGUE)), \
    std::make_pair(_T("N14"),                                 _LS(IDS_WG_CMD_N14)), \
    std::make_pair(_T("N11"),                                 _LS(IDS_WG_CMD_N11)), \
    std::make_pair(_T("N11(2nd edition)"),                    _LS(IDS_WG_CMD_N11_N11_2nd_edition)), \
    std::make_pair(_T("Subway Trains"),                       _LS(IDS_WG_CMD_SUBWAY_TRAINS)), \
    std::make_pair(_T("Tramcars"),                            _LS(IDS_WG_CMD_TRAMCARS)), \
    std::make_pair(_T("NK-80"),                               _LS(IDS_WG_CMD_NK_80)), \
    std::make_pair(_T("NG-60"),                               _LS(IDS_WG_CMD_NG_60)), \
    std::make_pair(_T("Uniform Load"),                        _LS(IDS_WG_CMD_UNIFORM_LOAD)), \
    std::make_pair(_T("Uniform Load(w/o Other Loads)"),       _LS(IDS_WG_CMD_UNIFORM_LOAD_OTHER)), \
    std::make_pair(_T("Concentrated Load (w/o other loads)"), _LS(IDS_WG_CMD_CONCENTRATED_LOAD_OHTER)), \
}; \

CString CCMMvhlItemStdRusDlg::ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const
{
    __static_caVehicleLoadType;
    return ConvStrRaw2Trans(caVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdRusDlg::ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const
{
    __static_caVehicleLoadType;
    return ConvStrTrans2Raw(caVehicleLoadType, strTrans);
}

#undef __static_caVehicleLoadType

#define __static_caBridgeType const array_pair_cstring<7> caBridgeType = \
{ \
    std::make_pair(_T("Pedestrian Bridge"),                       _LS(IDS_WG_CMD_PEDESTRIAN)), \
    std::make_pair(_T("Bridge Walkway with AK"),                  _LS(IDS_WG_CMD_WALKWAY_WITH_AK)), \
    std::make_pair(_T("Bridge Walkway without AK"),               _LS(IDS_WG_CMD_WALKWAY_WITHOUT_AK)), \
    std::make_pair(_T("Walkway for Railroad Bridge"),             _LS(IDS_WG_CMD_WALKWAY_RAILROAD)), \
    std::make_pair(_T("Walkway for Ballast-track Subway Bridge"), _LS(IDS_WG_CMD_WALKWAY_BALLAST_TRACK)), \
    std::make_pair(_T("Walkway for Other Bridge"),                _LS(IDS_WG_CMD_WALKWAY_OTHER)), \
    std::make_pair(_T("Walkway for Town Bridge"),                 _LS(IDS_WG_CMD_WALKWAY_TOWN)), \
}; \

CString CCMMvhlItemStdRusDlg::ConvBridgeTypeStrRaw2Trans(const CString& strRaw) const
{
    __static_caBridgeType;
    return ConvStrRaw2Trans(caBridgeType, strRaw);
}

CString CCMMvhlItemStdRusDlg::ConvBridgeTypeStrTrans2Raw(const CString& strTrans) const
{
    __static_caBridgeType;
    return ConvStrTrans2Raw(caBridgeType, strTrans);
}

#undef __static_caBridgeType

/* 
 * 지금 콤보박스를 사용하는 곳이 String으로 비교를 하고 있어
 * 일괄적으로 적용하기에는 리스크가 존재해서..
 * 번역이 필요한 String만 따로 추출해서 변환시켜주는 역할ㅠㅠ
 */
CString CCMMvhlItemStdRusDlg::GetVehicleTypeStr() const
{
    CString CurTransStr;
    m_cmbVehicleType.GetWindowText(CurTransStr);

    return ConvVehicleTypeStrTrans2Raw(CurTransStr);
}

CString CCMMvhlItemStdRusDlg::GetBridgeTypeStr() const
{
    CString CurTransStr;
    m_cmbBridgeType.GetWindowText(CurTransStr);

    return ConvBridgeTypeStrTrans2Raw(CurTransStr);
}