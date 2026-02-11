// CMMvhlItemStdEuroBSDlg.cpp : implementation file
//

#include "stdafx.h"
/* 이 대화상자 안씀
#include "wg_cmd.h"
#include "CMMvhlItemStdEuroBSDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdEuroBSDlg dialog

#define D_TYPE_COUNT 4 // code
#define D_TABLE_COUNT  3
#define D_MAX_COUNT  18
#define D_MAX_POINT_LOAD  D_MVHL_NUMLOAD
#define COLCOUNT 3



CString CCMMvhlItemStdEuroBSDlg::m_aTypeList[D_TYPE_COUNT][D_MAX_COUNT] = {
		{_T("Load Model 1"), _T("Load Model 2"), _T("Load Model 3 (600/150)"),_T("Load Model 3 (900/150)"),_T("Load Model 3 (1200/150/200)"),_T("Load Model 3 (1500/150/200)"),_T("Load Model 3 (1800/150/200)"),_T("Load Model 3 (2400/200)"),_T("Load Model 3 (3000/200)"),_T("Load Model 3 (3600/200)"),_T("Load Model 3 (SV 80)"),_T("Load Model 3 (SV 100)"),_T("Load Model 3 (SV 196)"),_T("Load Model 3 (SOV 250)"),_T("Load Model 3 (SOV 350)"),_T("Load Model 3 (SOV 450)"),_T("Load Model 3 (SOV 600)"),_T("Load Model 4")},
		{_T("Uniform load (Road bridge footway)"), _T("Uniform load (Footbridge)"), _T("Concentrated Load"), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T("Fatigue Load Model 1"), _T("Fatigue Load Model 2 (280)"), _T("Fatigue Load Model 2 (360)"), _T("Fatigue Load Model 2 (630)"),_T("Fatigue Load Model 2 (560)"),_T("Fatigue Load Model 2 (610)"),_T("Fatigue Load Model 3 (One Vehicle)"),_T("Fatigue Load Model 3 (Two Vehicle)"),_T("Fatigue Load Model 4 (200)"),_T("Fatigue Load Model 4 (310)"),_T("Fatigue Load Model 4 (490)"),_T("Fatigue Load Model 4 (390)"),_T("Fatigue Load Model 4 (450)"), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T("Load Model 71"), _T("Load Model SW/0"), _T("Load Model SW/2"), _T("Unloaded Train"), _T("HSLM A1 ~ HSLM A10"), _T("HSLM B"), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")}
};

int CCMMvhlItemStdEuroBSDlg::m_aCountList[D_TABLE_COUNT][D_TYPE_COUNT][D_MAX_COUNT] = 
{
	{
		{0,  0,  4,  6,  8, 10, 12, 12, 15, 18,  6,  6, 12, 15, 20, 24, 31,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  2,  3,  5,  4,  5,  4,  4,  2,  3,  5,  4,  5,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
	},
	{
		{0,  0,  0,  0,  6,  8,  9, 12, 15, 18,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
	},
	{
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}
	}
};

CString CCMMvhlItemStdEuroBSDlg::m_aDescList[D_TABLE_COUNT][D_TYPE_COUNT][D_MAX_COUNT] = 
{
	{
		{_T(""), _T(""), _T("* 600/150"), _T("* 900/150"), _T("* 1200/150"), _T("* 1500/150"), _T("* 1800/150"), _T("* 2400/200"), _T("* 3000/200"), _T("* 3600/200"), _T("* SV 80"), _T("* SV 100"), _T("* SV 196"), _T("* SOV-250"), _T("* SOV-350"), _T("* SOV-450"), _T("* SOV-600"), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T("* 280"), _T("* 360"), _T("* 630"), _T("* 560"), _T("* 610"),  _T("One Vehicle"), _T("First Vehicle"), _T("* 200"), _T("* 310"),  _T("* 490"),  _T("* 390"), _T("* 450"), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")}
	},
	{
		{_T(""), _T(""), _T(""), _T(""), _T("* 1200/200"), _T("* 1500/200"), _T("* 1800/200"), _T("* 2400/200/200"), _T("* 3000/200/200"), _T("* 3600/200/200"), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T("") , _T(""), _T(""),  _T(""),  _T(""),  _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T("") , _T(""), _T("Second Vehicle"), _T(""),  _T(""),  _T(""),  _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")}
	},
	{
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")},
		{_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T("")}
	},
};

CString CCMMvhlItemStdEuroBSDlg::m_aSelVehicleList[10] = {_T("A1"), _T("A2"), _T("A3"), _T("A4"), _T("A5"), _T("A6"), _T("A7"), _T("A8"), _T("A9"), _T("A10")};

CCMMvhlItemStdEuroBSDlg::CCMMvhlItemStdEuroBSDlg(CWnd* pParent )
	: CDialogMove(CCMMvhlItemStdEuroBSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdEuroBSDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aThreeGrid.RemoveAll();
	m_aThreeGrid.Add(IDC_CMD_FIRST_GRID);
	m_aThreeGrid.Add(IDC_CMD_SECOND_GRID);
	m_aThreeGrid.Add(IDC_CMD_THIRD_GRID);
	m_aThreeGrid.Add(IDC_CMD_MVHL_FIRST_TXT);
	m_aThreeGrid.Add(IDC_CMD_MVHL_SECOND_TXT);
	m_aThreeGrid.Add(IDC_CMD_MVHL_THIRD_TXT);

	m_aOneGrid.RemoveAll();
	m_aOneGrid.Add(IDC_CMD_ONE_GRID);

	m_aOneGrid1.RemoveAll();
	m_aOneGrid1.Add(IDC_CMD_MVHL_TSFACT_STC);
	m_aOneGrid1.Add(IDC_CMD_MVHL_TSFACT_EDT);
	m_aOneGrid1.Add(IDC_CMD_MVHL_UDLFACT_STC);
	m_aOneGrid1.Add(IDC_CMD_MVHL_UDLFACT_EDT);

	m_aPhi.RemoveAll();
	m_aPhi.Add(IDC_CMD_MVHL_PHI_GRUP);
	m_aPhi.Add(IDC_CMD_MVHL_DYNAMIC_CHK);
	m_aPhi.Add(IDC_CMD_MVHL_AUTO_RDO);
	m_aPhi.Add(IDC_CMD_MVHL_USER_RDO);
	m_aPhi.Add(IDC_CMD_MVHL_PHI_EQUATION_TXT);
	m_aPhi.Add(IDC_CMD_MVHL_PHI_TXT);
	m_aPhi.Add(IDC_CMD_MVHL_PHI_EDT);

	m_aAdjust.RemoveAll();
	m_aAdjust.Add(IDC_CMD_MVHL_ADJUST_TXT);
	m_aAdjust.Add(IDC_CMD_MVHL_ADJUST_EDT);

	m_aAdjust2.RemoveAll();
	m_aAdjust2.Add(IDC_CMD_MVHL_ADJUST2_TXT);
	m_aAdjust2.Add(IDC_CMD_MVHL_ADJUST2_EDT);

	m_aDynamic.RemoveAll();
	m_aDynamic.Add(IDC_CMD_MVHL_DYNAMIC_TXT);
	m_aDynamic.Add(IDC_CMD_MVHL_DYNAMIC_EDT);
	
	m_aInterval.RemoveAll();
	m_aInterval.Add(IDC_CMD_MVHL_INTERVAL_TXT);
	m_aInterval.Add(IDC_CMD_MVHL_INTEVAL_EDT);

	m_aSOSOV.RemoveAll();
	m_aSOSOV.Add(IDC_CMD_MVHL_DYNAMIC_CHK);
	m_aSOSOV.Add(IDC_CMD_MVHL_PHI_GRUP);
	m_aSOSOV.Add(IDC_CMD_MVHL_AUTO_RDO);
	m_aSOSOV.Add(IDC_CMD_MVHL_USER_RDO);
	m_aSOSOV.Add(IDC_CMD_MVHL_PHI_TXT);
	m_aSOSOV.Add(IDC_CMD_MVHL_PHI_EDT);

	m_aSelVehicle.RemoveAll();
	m_aSelVehicle.Add(IDC_CMD_MVHL_EURO_SEL_VEHICLE_STC);
	m_aSelVehicle.Add(IDC_CMD_MVHL_EURO_SEL_VEHICLE_CMB);

	m_aListEtc.RemoveAll();
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_LST);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W1_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W1_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W1_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD1_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD1_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD1_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D1_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D1_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D1_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W2_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W2_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_W2_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD2_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD2_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_DD2_UNT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D2_STC);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D2_EDT);
	m_aListEtc.Add(IDC_CMD_MVHL_EURO_D2_UNT);

	m_aHSLM_A.RemoveAll();
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_N_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_N_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D_UNT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D2_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D2_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_D2_UNT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_P_STC);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_P_EDT);
	m_aHSLM_A.Add(IDC_CMD_MVHL_EURO_HSLM_A_P_UNT);

	m_aHSLM_B.RemoveAll();
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_N_STC);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_N_EDT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_P_STC);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_P_EDT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_P_UNT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_D_STC);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_D_EDT);
	m_aHSLM_B.Add(IDC_CMD_MVHL_EURO_HSLM_B_D_UNT);

	m_aDynEff.RemoveAll();
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_GRP);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_STC);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT_STC);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT_EDT);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT2_STC);
	m_aDynEff.Add(IDC_CMD_MVHL_EURO_DYN_FACT2_EDT);

	m_aETC.RemoveAll();
	m_aETC.Add(IDC_CMD_MVHL_EURO_ALPHA_STC);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ALPHA_EDT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK);
	m_aETC.Add(IDC_CMD_MVHL_EURO_DIST_POINT_STC);
	m_aETC.Add(IDC_CMD_MVHL_EURO_DIST_POINT_EDT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_DIST_POINT_UNT);

	m_Data.Initialize();

	m_nStndCode = 0;
	m_bModify = FALSE;
	m_pBitmap = 0;
	m_bInit = FALSE;
	m_bLongiDist = FALSE;
}

CCMMvhlItemStdEuroBSDlg::~CCMMvhlItemStdEuroBSDlg()
{
	if(m_pBitmap) delete m_pBitmap;
}

void CCMMvhlItemStdEuroBSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdEuroBSDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,                 m_wndCodeName);
	DDX_Control(pDX, IDC_CMD_APPLY,                         m_wndBtnApply);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME,                     m_wndVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_TYPE_COMBO,               m_wndVehicleTypeList);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC_CHK,              m_wndDynamic);
	DDX_Control(pDX, IDC_CMD_MVHL_PHI_EDT,                  m_wndPhi);
	DDX_Control(pDX, IDC_CMD_MVHL_INTEVAL_EDT,              m_wndInterval);
	DDX_Control(pDX, IDC_CMD_MVHL_ADJUST_EDT,               m_wndAdjust);
	DDX_Control(pDX, IDC_CMD_MVHL_ADJUST2_EDT,              m_wndAdjust2);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC_EDT,              m_wndAmplificaton);
	DDX_Control(pDX, IDC_CMD_MVHL_TSFACT_EDT,               m_edtTSFactor);
	DDX_Control(pDX, IDC_CMD_MVHL_UDLFACT_EDT,              m_edtUDLFactor);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_SEL_VEHICLE_CMB,     m_cmbSelVihicle);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_LST,                 m_lstTrain);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W1_EDT,              m_edtTrainW1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W1_UNT,              m_untTrainW1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD1_EDT,             m_edtTrainDD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD1_UNT,             m_untTrainDD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D1_EDT,              m_edtTrainD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D1_UNT,              m_untTrainD1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W2_EDT,              m_edtTrainW2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_W2_UNT,              m_untTrainW2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD2_EDT,             m_edtTrainDD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DD2_UNT,             m_untTrainDD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D2_EDT,              m_edtTrainD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_D2_UNT,              m_untTrainD2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_N_EDT,        m_edtHSLMANum);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D_EDT,        m_edtHSLMALength);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D_UNT,        m_untHSLMALength);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D2_EDT,       m_edtHSLMASpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_D2_UNT,       m_untHSLMASpacing);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_P_EDT,        m_edtHSLMAForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_A_P_UNT,        m_untHSLMAForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DYN_FACT_EDT,        m_edtDynEffFact1);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DYN_FACT2_EDT,       m_edtDynEffFact2);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_N_EDT,        m_edtHSLMBNum);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_P_EDT,        m_edtHSLMBForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_P_UNT,        m_untHSLMBForce);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_D_EDT,        m_edtHSLMBDist);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_HSLM_B_D_UNT,        m_untHSLMBDist);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_ALPHA_EDT,           m_edtAlpha);
	DDX_Check  (pDX, IDC_CMD_MVHL_EURO_LONGI_DIST_CHK,      m_bLongiDist);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DIST_POINT_EDT,      m_edtDistPoint);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_DIST_POINT_UNT,      m_untDistPoint);
	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdEuroBSDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CString strCodeName[] = {_LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE),           // 0
													 _LS(IDS_CMD_MVHL_EURO_BS_FOOTBRIDGE),           // 1
													 _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE_FATIGUE),   // 2     
													 _LS(IDS_CMD_MVHL_EURO_BS_TRAIL_LOAD)            // 3
	};
	CString csDefault;
 
	int nCodeSeq[4];
	nCodeSeq[0] = D_MVHL_EURO_BS_ROADBRIDGE;
	nCodeSeq[1] = D_MVHL_EURO_BS_FOOTBRIDGE;
	nCodeSeq[2] = D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE;
	nCodeSeq[3] = D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD;
	
	csDefault = _T("Load Model 1"); 
	
	for(int i = 0; i < 4; i++) 
	{
		CDlgUtil::CobxAddItem(m_wndCodeName, strCodeName[nCodeSeq[i]], nCodeSeq[i]);
	}

	// TODO: Add extra initialization here
	if(!m_bModify)
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = csDefault;
		m_nStndCode = D_MVHL_EURO_BS_ROADBRIDGE+1;
		m_Data.nStandardCode = m_nStndCode+18;
		if(m_Data.nStandardCode == 22) m_Data.nStandardCode = 23; // 22 번은 AASHTO Legal Load 
	}
	int sIndex;
	int nIndex = GetIndex(m_Data.VehicleTypeName,&sIndex);
	m_nStndCode = nIndex + 1;
	if (nIndex < 0) return TRUE;

	InitControls();
	InitUnit();
	SetLoadTypeList(nIndex);
	SetSelVehicleList();
	AlignControl();		// 코드타입에 따라 프레임 정렬
	ControlsShowHide();

	m_wndOneGrid.SetDataSource(&m_Data,this,TRUE);

	m_wndThreeGrid[0].SetDataSource(&m_Data,this,0,m_aCountList[0][m_nStndCode-1][sIndex],sIndex);
	m_wndThreeGrid[1].SetDataSource(&m_Data,this,1,m_aCountList[1][m_nStndCode-1][sIndex],sIndex);
	m_wndThreeGrid[2].SetDataSource(&m_Data,this,2,m_aCountList[2][m_nStndCode-1][sIndex],sIndex);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->SetWindowText(m_aDescList[0][m_nStndCode-1][sIndex]);
	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->SetWindowText(m_aDescList[1][m_nStndCode-1][sIndex]);
	GetDlgItem(IDC_CMD_MVHL_THIRD_TXT)->SetWindowText(m_aDescList[2][m_nStndCode-1][sIndex]);
	
	SetHeaderTitle();
	Data2Dlg();
	ChangeCtrlText();
	EnableDisableControls();
	ChangeEditText();

	if(m_bModify)	m_wndBtnApply.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------------------------------------------------
// Implementation

void CCMMvhlItemStdEuroBSDlg::SetHeaderTitle()
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
	dwStyle = ListView_GetExtendedListViewStyle(m_lstTrain.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_lstTrain.GetSafeHwnd(), dwStyle);

	// Calculate width for each column
	nColWidth[0] = 28; nColWidth[1] = 70; nColWidth[2] = 90;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstTrain.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMMvhlItemStdEuroBSDlg::MakeItemEx()
{
	// List 관련 된 것만 여기서 처리 해준다.
	m_lstTrain.DeleteAllItems();    
	int nLoadCount = GetTrainLoadCount();
	int nDistCount = GetTrainDistCount();
	
	if (nLoadCount > 0)
	{
		for (int i = 0; i < nDistCount; i++)
			InsertItem(i, i+1, m_Data.dPointLoad[i], m_Data.dPointDistance[i]);

		if (nLoadCount > nDistCount)
			InsertItem(i, i+1, m_Data.dPointLoad[i], 0.0);
	}

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdEuroBSDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
{
	LVITEM lvitem;
	CString str;

	lvitem.iItem = nPos;
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iSubItem=i;
		if (i == 0) str.Format(_T("%d"), nNo);
		else if (i == 1) str.Format(_T("%g"), dLoad);
		else if (i == 2)
		{
			if (dDist == 0.0) str = _LS(IDS_WG_CMD__ADDD__end);
			else if (dDist == -1.0) str = _LS(IDS_WG_CMD_INFINITE);
			else str.Format(_T("%g"), dDist);
		}
		else str = _LS(IDS_WG_CMD__ADDD__Error);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if(i == 0)
			m_lstTrain.InsertItem(&lvitem);
		else m_lstTrain.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

int CCMMvhlItemStdEuroBSDlg::GetTrainLoadCount()
{
	for(int i = 0; i < D_MAX_POINT_LOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdEuroBSDlg::GetTrainDistCount()
{
	for(int i = 0; i < D_MAX_POINT_LOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdEuroBSDlg::InitControls()
{
	m_wndOneGrid.SubclassDlgItem(IDC_CMD_ONE_GRID, this);
	m_wndOneGrid.InitGrid();

	m_wndThreeGrid[0].SubclassDlgItem(IDC_CMD_FIRST_GRID,this);
	m_wndThreeGrid[1].SubclassDlgItem(IDC_CMD_SECOND_GRID,this);
	m_wndThreeGrid[2].SubclassDlgItem(IDC_CMD_THIRD_GRID,this);

	m_wndThreeGrid[0].InitGrid();
	m_wndThreeGrid[1].InitGrid();
	m_wndThreeGrid[2].InitGrid();
}

void CCMMvhlItemStdEuroBSDlg::InitUnit()
{
	m_edtTSFactor.SetUnitType(0);
	m_edtUDLFactor.SetUnitType(0);

	m_untTrainW1     .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untTrainDD1    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainD1     .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainW2     .SetUnitType(D_UNITSYS_BASE_UNITFORCE);
	m_untTrainDD2    .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untTrainD2     .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHSLMALength .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHSLMASpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untHSLMAForce  .SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untHSLMBForce  .SetUnitType(D_UNITSYS_BASE_FORCE);
	m_untHSLMBDist   .SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDistPoint   .SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdEuroBSDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode-18;
	if(m_Data.nStandardCode == 23) m_nStndCode = 4; // 22 번은 AASHTO Legal Load 

	if(m_Data.VehicleLoadName.IsEmpty())
		m_wndVehicleName.SetWindowText(m_Data.VehicleTypeName);
	else
		m_wndVehicleName.SetWindowText(m_Data.VehicleLoadName);
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, m_nStndCode-1);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_wndVehicleTypeList.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode-1, sIndex);

	if     (m_Data.SelVehicle == _T("A1"))  m_cmbSelVihicle.SetCurSel(0);
	else if(m_Data.SelVehicle == _T("A2"))  m_cmbSelVihicle.SetCurSel(1);
	else if(m_Data.SelVehicle == _T("A3"))  m_cmbSelVihicle.SetCurSel(2);
	else if(m_Data.SelVehicle == _T("A4"))  m_cmbSelVihicle.SetCurSel(3);
	else if(m_Data.SelVehicle == _T("A5"))  m_cmbSelVihicle.SetCurSel(4);
	else if(m_Data.SelVehicle == _T("A6"))  m_cmbSelVihicle.SetCurSel(5);
	else if(m_Data.SelVehicle == _T("A7"))  m_cmbSelVihicle.SetCurSel(6);
	else if(m_Data.SelVehicle == _T("A8"))  m_cmbSelVihicle.SetCurSel(7);
	else if(m_Data.SelVehicle == _T("A9"))  m_cmbSelVihicle.SetCurSel(8);
	else if(m_Data.SelVehicle == _T("A10")) m_cmbSelVihicle.SetCurSel(9);
	else {} // Nothing

	//m_wndDynLoadAllowance.SetEditUnit(m_Data.dDynLoadAllowance);

	T_MVHL_D tempMvhl;
	tempMvhl = m_Data;
	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data))
	{
		if(!m_bInit)
		{
			if(m_bModify)
			{
				memcpy(m_Data.dTandemAdjust,tempMvhl.dTandemAdjust,sizeof(tempMvhl.dTandemAdjust));
				memcpy(m_Data.dUDLAdjust,tempMvhl.dUDLAdjust,sizeof(tempMvhl.dUDLAdjust));
				m_Data.dAdjustment = tempMvhl.dAdjustment;
				m_Data.dAdjustment2 = tempMvhl.dAdjustment2;
				m_Data.bDynamicFactor = tempMvhl.bDynamicFactor;
				m_Data.bUserInput = tempMvhl.bUserInput;
				m_Data.dAmplification = tempMvhl.dAmplification; 
				m_Data.dInterval = tempMvhl.dInterval;
				memcpy(m_Data.dAmplification2, tempMvhl.dAmplification2, sizeof(tempMvhl.dAmplification2));
			}
			m_bInit = TRUE;
		}
		m_wndOneGrid.ShowData();  
		m_wndThreeGrid[0].ShowData();  
		m_wndThreeGrid[1].ShowData();  
		m_wndThreeGrid[2].ShowData();  

		m_edtTrainW1.SetEditUnit(m_Data.dTrainW1);
		m_edtTrainDD1.SetEditUnit(m_Data.dTrainDD1);
		m_edtTrainD1.SetEditUnit(m_Data.dTrainD1);
		m_edtTrainW2.SetEditUnit(m_Data.dTrainW2);
		m_edtTrainDD2.SetEditUnit(m_Data.dTrainDD2);
		m_edtTrainD2.SetEditUnit(m_Data.dTrainD2);
		m_edtHSLMANum.SetEditUnit(m_Data.nHSLMANum);
		m_edtHSLMALength.SetEditUnit(m_Data.dHSLMALength);
		m_edtHSLMASpacing.SetEditUnit(m_Data.dHSLMASpacing);
		m_edtHSLMAForce.SetEditUnit(m_Data.dHSLMAForce);
		m_edtHSLMBForce.SetEditUnit(m_Data.dHSLMBForce);

		MakeItemEx();
	}
	CArray<UINT,UINT> aRadio;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	m_wndDynamic.SetCheck(m_Data.bDynamicFactor);
	int nRadio = (m_Data.bUserInput)?1:0;
	CDlgUtil::CtrlRadioSetCheck(this,aRadio,nRadio);
	CString strTemp;
	strTemp.Format(_T("%g"),m_Data.dAmplification);
	m_wndPhi.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data.dInterval);
	m_wndInterval.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data.dAdjustment2);
	m_wndAdjust2.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data.dAdjustment);
	m_wndAdjust.SetWindowText(strTemp);
	strTemp.Format(_T("%g"),m_Data.dAmplification);
	m_wndAmplificaton.SetWindowText(strTemp);
	
	m_edtTSFactor.SetEditUnit(m_Data.dAmplification2[0]);
	m_edtUDLFactor.SetEditUnit(m_Data.dAmplification2[1]);

	// Train Load
	m_edtDynEffFact1.SetEditUnit(m_Data.dPhiDynEff1);
	m_edtDynEffFact2.SetEditUnit(m_Data.dPhiDynEff2);
	m_edtHSLMBNum.SetEditUnit(m_Data.nHSLMBNum);
	m_edtHSLMBDist.SetEditUnit(m_Data.dHSLMBDist);
	m_edtAlpha.SetEditUnit(m_Data.dFactorofVLoad);
	m_bLongiDist = m_Data.bLongiDist;
	m_edtDistPoint.SetEditUnit(m_Data.dRailSupPoints);

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdEuroBSDlg::Dlg2Data()
{
	m_Data.Initialize();  
	m_wndVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_wndVehicleTypeList.GetWindowText(m_Data.VehicleTypeName);
	m_cmbSelVihicle.GetWindowText(m_Data.SelVehicle);
	//m_Data.dDynLoadAllowance = m_wndDynLoadAllowance.GetEditValue();
	m_Data.nStandardCode = m_nStndCode+18;
	if(m_Data.nStandardCode == 22) m_Data.nStandardCode = 23; // 22 번은 AASHTO Legal Load 
	CString strTemp;
	CVehlDB db(m_pDoc);
	if(!db.GetStandardVehicleLoadValue(m_Data))  return FALSE;  
	m_wndAdjust2.GetWindowText(strTemp);
	m_Data.dAdjustment2 = _tstof(strTemp);
	m_wndAdjust.GetWindowText(strTemp);
	m_Data.dAdjustment = _tstof(strTemp);

	BOOL bAmplification2 = FALSE;
	if((m_nStndCode-1)==D_MVHL_EURO_BS_ROADBRIDGE)
	{
		BOOL bDynamic;
		bDynamic = m_wndDynamic.GetCheck();
		m_Data.bDynamicFactor = bDynamic;
		if(bDynamic)
		{
			CArray<UINT,UINT> aRadio;
			BOOL bUser;
			aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
			aRadio.Add(IDC_CMD_MVHL_USER_RDO);
			CDlgUtil::CtrlRadioGetCheck(this,aRadio,bUser);
			m_Data.bUserInput = bUser;

			if(bUser)
			{
				m_wndPhi.GetWindowText(strTemp);
				m_Data.dAmplification = _tstof(strTemp);
			}
			else 
				m_Data.dAmplification = 1.0;
		}
		else
		{
			m_Data.bUserInput=TRUE;
			m_Data.dAmplification = 1.0;
		}
		
		if(m_Data.VehicleTypeName==_T("Load Model 1"))
		{
			m_Data.bDynamicFactor2[0] = m_Data.bDynamicFactor2[1] = TRUE;
			m_Data.dAmplification2[0] = m_edtTSFactor.GetEditValue();
			m_Data.dAmplification2[1] = m_edtUDLFactor.GetEditValue();
			bAmplification2 = TRUE;
		}
	}
	else
	{
		m_wndAmplificaton.GetWindowText(strTemp);
		m_Data.dAmplification = _tstof(strTemp);

		m_wndInterval.GetWindowText(strTemp);
		m_Data.dInterval = _tstof(strTemp);
	}
	if(!bAmplification2)
	{
		m_Data.bDynamicFactor2[0] = m_Data.bDynamicFactor2[1] = FALSE;
		m_Data.dAmplification2[0] = 1.0;
		m_Data.dAmplification2[1] = 1.0;
	}

	m_wndOneGrid.SaveData();
	m_wndThreeGrid[0].SaveData();
	m_wndThreeGrid[1].SaveData();
	m_wndThreeGrid[2].SaveData();

	m_Data.nHSLMBNum = m_edtHSLMBNum.GetEditValue();
	m_Data.dHSLMBDist = m_edtHSLMBDist.GetEditValue();
	m_Data.dPhiDynEff1 = m_edtDynEffFact1.GetEditValue();
	m_Data.dPhiDynEff2 = m_edtDynEffFact2.GetEditValue();
	m_Data.dFactorofVLoad = m_edtAlpha.GetEditValue();
	m_Data.bLongiDist = m_bLongiDist;
	m_Data.dRailSupPoints = m_edtDistPoint.GetEditValue();
	
	return TRUE;
}

void CCMMvhlItemStdEuroBSDlg::AlignControl()
{
	int nType = m_nStndCode-1;
	int nIndex = 0;
	GetIndex(m_Data.VehicleTypeName, &nIndex);

	CRect rRef;
	CRect rToMove;
	CRect rToMove2;
	int nDistY,nDistY2;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();

	switch(nType)
	{
	case 0:
		switch(nIndex)
		{
		case 0:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ONE_GRID)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aOneGrid, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aOneGrid1, nDistY);
			break;
		case 1:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST2_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust2, nDistY);

			GetDlgItem(IDC_CMD_MVHL_ADJUST2_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 5;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);
			break;
		case 17:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			aControls.Add(IDC_CMD_MVHL_PHI_TXT);
			aControls.Add(IDC_CMD_MVHL_PHI_EDT);
			
			GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 10;
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
			break;
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
			aControls.Add(IDC_CMD_MVHL_PHI_TXT);
			aControls.Add(IDC_CMD_MVHL_PHI_EDT);

			GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
			break;
		default:
			break;
		}break;
	case 1:
		switch(nIndex)
		{
		case 0:
		case 1:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);
			break;
		case 2:
			break;
		}break;
	case 2:
		if(nIndex==0)
		{
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ONE_GRID)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aOneGrid, nDistY);
		}
		GetDlgItem(IDC_CMD_FIRST_GRID)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_EDT)->GetWindowRect(rToMove);
		GetDlgItem(IDC_CMD_MVHL_INTERVAL_TXT)->GetWindowRect(rToMove2);
		nDistY = rRef.bottom+20 - rToMove.top;
		nDistY2 = rRef.bottom+20 + (rToMove2.bottom - rToMove2.top+10) - rToMove2.top;
		if(nIndex == 7)
		{
			nDistY +=  (rToMove2.bottom - rToMove2.top+10);
			nDistY2 -= (rToMove2.bottom - rToMove2.top+10);
		}
		CDlgUtil::CtrlMoveDistY(this, m_aDynamic, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aInterval, nDistY2);
		break;
	case 3:
		switch(nIndex)
		{
		case 0: 
		case 1:
		case 2:
		case 3:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_LST)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aListEtc, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_LST)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 20;
			CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			break;
		case 4:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_A_N_STC)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aHSLM_A, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_A_P_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 20;
			CDlgUtil::CtrlMoveDistY(this, m_aDynEff, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 20;
			CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			break;
		case 5:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_B_N_STC)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aHSLM_B, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_B_P_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 20;
			CDlgUtil::CtrlMoveDistY(this, m_aDynEff, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + 20;
			CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			break;
		}
	}

	// 리뉴얼에서 적용되는 Control 들 //////////////////////////////////
	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_FIRST_CHK);
	aControls.Add(IDC_CMD_MVHL_SECOND_CHK);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_FIRST_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

	aControls.RemoveAll();
	aControls.Add(IDC_CMD_MVHL_FOOTWAY_W_STC);
	aControls.Add(IDC_CMD_MVHL_FOOTWAY_W_EDT);
	aControls.Add(IDC_CMD_MVHL_FOOTWAY_W_UNT);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_FOOTWAY_W_STC)->GetWindowRect(rToMove);	
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
	////////////////////////////////////////////////////////////////////

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+16;
	MoveWindow(r);
	Invalidate(TRUE);
	UpdateWindow();
}

void CCMMvhlItemStdEuroBSDlg::ControlsShowHide()
{
	int nType = m_nStndCode-1;
	int nIndex = 0;
	GetIndex(m_Data.VehicleTypeName, &nIndex);

	BOOL bOneGrid = FALSE, bOneGrid1   = FALSE, bThreeGrid = FALSE, bAdjust   = FALSE;
	BOOL bAdjust2 = FALSE, bPhi        = FALSE, bDynamic   = FALSE, bInterval = FALSE;
	BOOL bSOSOV   = FALSE, bSelVehicle = FALSE, bListEtc   = FALSE, bHSLM_A   = FALSE;
	BOOL bHSLM_B  = FALSE, bDynEff     = FALSE, bETC       = FALSE, bSOSOVA   = FALSE;

	if(nType == 0)
	{
		if     (nIndex == 0)  { bOneGrid = TRUE; bOneGrid1 = TRUE; }
		else if(nIndex == 1)  { bAdjust  = TRUE;  bAdjust2 = TRUE; }
		else if(nIndex == 17) { bAdjust  = TRUE; }
		else if(nIndex == 10 || nIndex == 11 || nIndex == 12 || nIndex == 13 || nIndex == 14 || nIndex == 15 || nIndex == 16)
		{
			bThreeGrid = TRUE; bSOSOV = TRUE; bSOSOVA = TRUE;
		}
		else { bPhi = TRUE; bThreeGrid = TRUE; }
	}
	else if(nType == 1)
	{
		if (nIndex == 0 || nIndex == 1) { bAdjust = TRUE; }
	}
	else if(nType == 2)
	{
		bOneGrid = (nIndex==0);
		bThreeGrid = (nIndex!=0);
		if(nIndex == 7) bInterval = TRUE;
		bDynamic = TRUE;
	}
	else if(nType == 3)
	{
		if     (nIndex == 4)  { bSelVehicle = TRUE; bHSLM_A = TRUE; bDynEff = TRUE; bETC = TRUE;}
		else if(nIndex == 5)  { bHSLM_B = TRUE; bDynEff = TRUE; bETC = TRUE;}
		else if(nIndex == 0 || nIndex == 1 || nIndex == 2 || nIndex == 3)
		{
			bListEtc = TRUE; bETC = TRUE;
		}
	}

	CDlgUtil::CtrlShowHide(this,m_aOneGrid,    bOneGrid);
	CDlgUtil::CtrlShowHide(this,m_aOneGrid1,   bOneGrid1);
	CDlgUtil::CtrlShowHide(this,m_aThreeGrid,  bThreeGrid);
	CDlgUtil::CtrlShowHide(this,m_aPhi,        bPhi);
	CDlgUtil::CtrlShowHide(this,m_aDynamic,    bDynamic);
	CDlgUtil::CtrlShowHide(this,m_aAdjust,     bAdjust);
	CDlgUtil::CtrlShowHide(this,m_aAdjust2,    bAdjust2);
	CDlgUtil::CtrlShowHide(this,m_aInterval,   bInterval);
	if(bSOSOV) CDlgUtil::CtrlShowHide(this, m_aSOSOV, bSOSOV);
	CDlgUtil::CtrlShowHide(this,m_aSelVehicle, bSelVehicle);
	CDlgUtil::CtrlShowHide(this,m_aListEtc,    bListEtc);
	CDlgUtil::CtrlShowHide(this,m_aHSLM_A,     bHSLM_A);
	CDlgUtil::CtrlShowHide(this,m_aHSLM_B,     bHSLM_B);
	CDlgUtil::CtrlShowHide(this,m_aDynEff,     bDynEff);
	CDlgUtil::CtrlShowHide(this,m_aETC,        bETC);
	GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->ShowWindow(bSOSOVA);

	// 리뉴얼에서 적용되는 Control 이기 때문에 무조건 HIDE
	GetDlgItem(IDC_CMD_MVHL_FIRST_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_SECOND_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_FOOTWAY_W_STC)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_FOOTWAY_W_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_FOOTWAY_W_UNT)->ShowWindow(SW_HIDE);
}

void CCMMvhlItemStdEuroBSDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	int nType = m_nStndCode-1;
	int nIndex = 0;
	GetIndex(m_Data.VehicleTypeName, &nIndex);

	BOOL bDynamic = m_wndDynamic.GetCheck();

	CArray<UINT,UINT> aRadio;
	int nAuto = 0;
	aRadio.Add(IDC_CMD_MVHL_AUTO_RDO);
	aRadio.Add(IDC_CMD_MVHL_USER_RDO);
	CDlgUtil::CtrlRadioGetCheck(this, aRadio, nAuto);
	
	GetDlgItem(IDC_CMD_MVHL_AUTO_RDO)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_USER_RDO)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->EnableWindow(bDynamic);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->EnableWindow(bDynamic && (nAuto==1));
	GetDlgItem(IDC_CMD_MVHL_PHI_EDT)->EnableWindow(bDynamic && (nAuto==1));

	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->EnableWindow(bDynamic && (nAuto==1));
	GetDlgItem(IDC_CMD_MVHL_PHI_EDT)->EnableWindow(bDynamic && (nAuto==1));

	// Trail Load
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->EnableWindow(nType == 3 && (nIndex==0 || nIndex==1));
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_EDT)->EnableWindow(nType == 3 && (nIndex==0 || nIndex==1));
	GetDlgItem(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK)->EnableWindow(nType == 3 && (nIndex==0 || nIndex==4));
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_STC)->EnableWindow(nType == 3 && (nIndex==0 || nIndex==4) && m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_EDT)->EnableWindow(nType == 3 && (nIndex==0 || nIndex==4) && m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_UNT)->EnableWindow(nType == 3 && (nIndex==0 || nIndex==4) && m_bLongiDist);
}

BOOL CCMMvhlItemStdEuroBSDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdEuroBSDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdEuroBSDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_TYPE_COMBO,           OnSelchangeCmdMvhlTypeCombo)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,             OnSelchangeCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_EURO_SEL_VEHICLE_CMB, OnCmdMvhlEuroSelVehicleCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNAMIC_CHK,         OnCmdMvhlEuroDynamicChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_AUTO_RDO,            OnCmdMvhlEuroAutoRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_USER_RDO,            OnCmdMvhlEuroAutoRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK, OnCmdMvhlEuroLongiDistChk)
	ON_BN_CLICKED(IDC_CMD_APPLY,                    OnCmdMvhlEuroApply)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdEuroBSDlg message handlers

void CCMMvhlItemStdEuroBSDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdEuroBSDlg::OnCmdMvhlEuroApply() 
{	
	ApplyOrOK();
}

void CCMMvhlItemStdEuroBSDlg::ChangeBitmap(int nIndex, int nBitmap)
{
	UINT aBitmapID0[] = { 
		IDB_CMD_MVHL_51,   IDB_CMD_MVHL_52,   IDB_CMD_MVHL_53_1, IDB_CMD_MVHL_53_1, IDB_CMD_MVHL_53_2, 
		IDB_CMD_MVHL_53_2, IDB_CMD_MVHL_53_2, IDB_CMD_MVHL_53_2, IDB_CMD_MVHL_53_2, IDB_CMD_MVHL_53_2, 
		IDB_CMD_MVHL_88,   IDB_CMD_MVHL_88,   IDB_CMD_MVHL_89,   IDB_CMD_MVHL_90,   IDB_CMD_MVHL_91,
		IDB_CMD_MVHL_92,   IDB_CMD_MVHL_93,   IDB_CMD_MVHL_54
	};
	UINT aBitmapID1[] = { 
		IDB_CMD_MVHL_55, IDB_CMD_MVHL_56, IDB_CMD_MVHL_57
	};
	UINT aBitmapID2[] = { // AASHTO Standard Load
		IDB_CMD_MVHL_58, IDB_CMD_MVHL_59, IDB_CMD_MVHL_60, IDB_CMD_MVHL_61, IDB_CMD_MVHL_62, IDB_CMD_MVHL_63,
		IDB_CMD_MVHL_64_1, IDB_CMD_MVHL_64_2, IDB_CMD_MVHL_65, IDB_CMD_MVHL_66, IDB_CMD_MVHL_67, IDB_CMD_MVHL_68, IDB_CMD_MVHL_69
	};
	UINT aBitmapID3[] = { // Train Load
		IDB_CMD_MVHL_94, IDB_CMD_MVHL_95, IDB_CMD_MVHL_95, IDB_CMD_MVHL_98, IDB_CMD_MVHL_96, IDB_CMD_MVHL_97, 
	};

	UINT bitmapID;  
	switch(nIndex)
	{
		case D_MVHL_EURO_BS_ROADBRIDGE:           bitmapID = aBitmapID0[nBitmap];  break;
		case D_MVHL_EURO_BS_FOOTBRIDGE:           bitmapID = aBitmapID1[nBitmap];  break;
		case D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE:   bitmapID = aBitmapID2[nBitmap];  break;
		case D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD:    bitmapID = aBitmapID3[nBitmap];  break;
		default : ASSERT(0);
	}

	if(m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_CMD_MVHL_PIC_WND);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(bitmapID);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
}

void CCMMvhlItemStdEuroBSDlg::OnSelchangeCmdMvhlCodeCmb() 
{
	int m_nCodeType = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, m_wndCodeName.GetCurSel());
	SetLoadTypeList(m_nCodeType);
	SetSelVehicleList();

	int nIndex = m_wndCodeName.GetCurSel();
	m_nStndCode = m_wndCodeName.GetItemData(nIndex) + 1;

	ChangeData();
}
void CCMMvhlItemStdEuroBSDlg::OnCmdMvhlEuroDynamicChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSDlg::OnCmdMvhlEuroAutoRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSDlg::OnCmdMvhlEuroSelVehicleCmb()
{
	ChangeData();
}

void CCMMvhlItemStdEuroBSDlg::OnCmdMvhlEuroLongiDistChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSDlg::OnSelchangeCmdMvhlTypeCombo() 
{
	ChangeData();
	ChangeCtrlText();
}

BOOL CCMMvhlItemStdEuroBSDlg::ChangeData()
{
	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_wndVehicleTypeList.GetWindowText(m_Data.VehicleTypeName);
	m_cmbSelVihicle.GetWindowText(m_Data.SelVehicle);
	m_Data.nStandardCode = m_nStndCode+18;
	if(m_Data.nStandardCode == 22) m_Data.nStandardCode = 23; // 22 번은 AASHTO Legal Load 
	
	int sIndex;
	CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, m_nStndCode-1);
	GetIndex(m_Data.VehicleTypeName, &sIndex);
	m_wndVehicleTypeList.SetCurSel(sIndex);
	ChangeBitmap(m_nStndCode-1, sIndex);

	m_wndVehicleName.SetWindowText(m_Data.VehicleTypeName);

	m_wndThreeGrid[0].SetDataSource(&m_Data,this,0,m_aCountList[0][m_nStndCode-1][sIndex],sIndex);
	m_wndThreeGrid[1].SetDataSource(&m_Data,this,1,m_aCountList[1][m_nStndCode-1][sIndex],sIndex);
	m_wndThreeGrid[2].SetDataSource(&m_Data,this,2,m_aCountList[2][m_nStndCode-1][sIndex],sIndex);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->SetWindowText(m_aDescList[0][m_nStndCode-1][sIndex]);
	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->SetWindowText(m_aDescList[1][m_nStndCode-1][sIndex]);
	GetDlgItem(IDC_CMD_MVHL_THIRD_TXT)->SetWindowText(m_aDescList[2][m_nStndCode-1][sIndex]);
	AlignControl();		// 코드타입에 따라 프레임 정렬
	ControlsShowHide();
	Data2Dlg();
	EnableDisableControls();
	ChangeEditText();

	return TRUE;
}

void CCMMvhlItemStdEuroBSDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;

	m_wndVehicleTypeList.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && m_aTypeList[nIndex][i] != _T(""))
	{
		m_wndVehicleTypeList.AddString(m_aTypeList[nIndex][i++]);
	}
	m_wndVehicleTypeList.SetCurSel(0);
}

void CCMMvhlItemStdEuroBSDlg::SetSelVehicleList()
{
	m_cmbSelVihicle.ResetContent();
	int i = 0;
	while (i < D_MAX_COUNT && m_aSelVehicleList[i] != _T(""))
	{
		m_cmbSelVihicle.AddString(m_aSelVehicleList[i++]);
	}
	m_cmbSelVihicle.SetCurSel(0);
}

int CCMMvhlItemStdEuroBSDlg::GetIndex(CString &csName, int *sitem)
{
	for (int i = 0; i < D_TYPE_COUNT; i++)
	{
		for (int j = 0; j < D_MAX_COUNT && m_aTypeList[i][j] != csName; j++);
		if (j != D_MAX_COUNT) 
		{
			if(sitem) *sitem = j;
			return i;
		}
	}

	ASSERT(0);
	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
	return -1;
}

void CCMMvhlItemStdEuroBSDlg::ChangeCtrlText()
{
	CString strText;
	strText.Format(_T("%s = 1.40 -L / 500  (1<= %s<= 1.40 )"), CSpecialChar::m_csPhi, CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->SetWindowText(strText);
	
	strText.Format(_T("%s :"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->SetWindowText(strText);  
	
	strText.Format(_T("%s factor for Tandem System"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_TSFACT_STC)->SetWindowText(strText);
	
	strText.Format(_T("%s factor for UDL System"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_UDLFACT_STC)->SetWindowText(strText);
	
	strText.Format(_T("%s factor :"), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_ADJUST_TXT)->SetWindowText(strText);
	
	strText.Format(_T("Dynamic Amplification Factor, %s :"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_DYNAMIC_TXT)->SetWindowText(strText);
	
	strText.Format(_T("* Two Vehicle Interval :  D4 >="));
	GetDlgItem(IDC_CMD_MVHL_INTERVAL_TXT)->SetWindowText(strText);

	int nType = m_nStndCode-1;
	int nIndex = 0;
	GetIndex(m_Data.VehicleTypeName, &nIndex);

	if(nType == 0)
	{
		if(nIndex == 10 || nIndex == 11 || nIndex == 12)
		{
			GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_SOSOV_TEXT_1));
		}
		else
		{
			GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->SetWindowText(_LS(IDS_CMD_MVHL_EURO_BS_SOSOV_TEXT_2));
		}
	}

	strText.Format(_T("(1+%s%s(dyn) + %s%s/2) X HSLM"), CSpecialChar::m_csPhi, CSpecialChar::m_csPrime, CSpecialChar::m_csPhi, CSpecialChar::m_csDPrime);
	GetDlgItem(IDC_CMD_MVHL_EURO_DYN_STC)->SetWindowText(strText);

	strText.Format(_T("%s%s(dyn) : "), CSpecialChar::m_csPhi, CSpecialChar::m_csPrime);
	GetDlgItem(IDC_CMD_MVHL_EURO_DYN_FACT_STC)->SetWindowText(strText);

	strText.Format(_T("%s%s : "), CSpecialChar::m_csPhi, CSpecialChar::m_csDPrime);
	GetDlgItem(IDC_CMD_MVHL_EURO_DYN_FACT2_STC)->SetWindowText(strText);

	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_VERTICAL_LOADS), CSpecialChar::m_csAlpha);
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->SetWindowText(strText);
}

void CCMMvhlItemStdEuroBSDlg::ChangeEditText()
{
	int nType = m_nStndCode-1;
	int nIndex = 0;
	GetIndex(m_Data.VehicleTypeName, &nIndex);

	if(nType == 3) // Train Load
	{
		if(nIndex == 0) // Load Model 71
		{
			m_edtTrainDD1.SetWindowText(CSpecialChar::m_csInfinity);
			m_edtTrainDD2.SetWindowText(CSpecialChar::m_csInfinity);
		}
		else if(nIndex == 1 || nIndex == 2) // Load Model SW/0, Load Model SW/2
		{
			m_edtTrainD2.SetWindowText(_T(""));
		}
		else if(nIndex == 3) // Unloaded Train
		{
			m_edtTrainDD1.SetWindowText(CSpecialChar::m_csInfinity);
			m_edtTrainD1 .SetWindowText(_T(""));
			m_edtTrainW2 .SetWindowText(_T(""));
			m_edtTrainDD2.SetWindowText(_T(""));
			m_edtTrainD2 .SetWindowText(_T(""));
		}
	}
}
*/