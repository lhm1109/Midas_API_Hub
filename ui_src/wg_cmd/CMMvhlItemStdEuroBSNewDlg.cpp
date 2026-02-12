// CMMvhlItemStdEuroBSNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdEuroBSNewDlg.h"

#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_MvhlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "CMMvhlStdEuroBSNewTempForRefactor.h"
#include "CMMvhlItemStdPatchLoadInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdEuroBSNewDlg dialog

#define COLCOUNT 3

CCMMvhlItemStdEuroBSNewDlg::CCMMvhlItemStdEuroBSNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvhlItemStdEuroBSNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvhlItemStdEuroBSNewDlg)	
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aThreeGrid.RemoveAll();
	m_aThreeGrid.Add(IDC_CMD_FIRST_GRID);
	m_aThreeGrid.Add(IDC_CMD_SECOND_GRID);
	m_aThreeGrid.Add(IDC_CMD_THIRD_GRID);

	m_aThreeGridTxt.RemoveAll();
	m_aThreeGridTxt.Add(IDC_CMD_MVHL_FIRST_TXT);
	m_aThreeGridTxt.Add(IDC_CMD_MVHL_SECOND_TXT);
	m_aThreeGridTxt.Add(IDC_CMD_MVHL_THIRD_TXT);

	m_aThreeGridChk1.RemoveAll();
	m_aThreeGridChk1.Add(IDC_CMD_MVHL_FIRST_CHK);

	m_aThreeGridChk2.RemoveAll();
	m_aThreeGridChk2.Add(IDC_CMD_MVHL_SECOND_CHK);

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

	m_aFootway.RemoveAll();
	m_aFootway.Add(IDC_CMD_MVHL_FOOTWAY_W_STC);
	m_aFootway.Add(IDC_CMD_MVHL_FOOTWAY_W_EDT);
	m_aFootway.Add(IDC_CMD_MVHL_FOOTWAY_W_UNT);

	m_aDynamic.RemoveAll();
	m_aDynamic.Add(IDC_CMD_MVHL_DYNAMIC_TXT);
	m_aDynamic.Add(IDC_CMD_MVHL_DYNAMIC_EDT);
	
	m_aInterval.RemoveAll();
	m_aInterval.Add(IDC_CMD_MVHL_INTERVAL_TXT);
	m_aInterval.Add(IDC_CMD_MVHL_INTEVAL_EDT);
	m_aInterval.Add(IDC_CMD_MVHL_INTEVAL_UNT);

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
	m_aETC.Add(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT);
	m_aETC.Add(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT);

	m_aCtrlPatch.RemoveAll();
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_CHK);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_BTN);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_W_UNT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_STC);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_EDT);
	m_aCtrlPatch.Add(IDC_CMD_MVHL_PATCH_L_UNT);

	m_Data.Initialize();

	m_nStndCode = 0;
	m_bModify = FALSE;
	m_bInit = FALSE;
	m_bLongiDist = FALSE;
	m_bLM3LoadCase1 = TRUE;
	m_bLM3LoadCase2 = TRUE;
	m_bEccenVertLoad = FALSE;
	m_bPatchGrid = FALSE;
}

CCMMvhlItemStdEuroBSNewDlg::~CCMMvhlItemStdEuroBSNewDlg()
{
}

void CCMMvhlItemStdEuroBSNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvhlItemStdEuroBSNewDlg)  
	DDX_Control(pDX, IDC_CMD_MVHL_CODE_CMB,                 m_cmbCodeName);
	DDX_Control(pDX, IDC_CMD_MVHL_NAME,                     m_edtVehicleName);
	DDX_Control(pDX, IDC_CMD_MVHL_TYPE_COMBO,               m_cmbVehicleTypeList);
	DDX_Control(pDX, IDC_CMD_MVHL_DYNAMIC_CHK,              m_wndDynamic);
	DDX_Control(pDX, IDC_CMD_MVHL_PHI_EDT,                  m_wndPhi);
	DDX_Control(pDX, IDC_CMD_MVHL_INTEVAL_EDT,              m_edtInterval);
	DDX_Control(pDX, IDC_CMD_MVHL_INTEVAL_UNT,              m_untInterval);
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
	DDX_Check  (pDX, IDC_CMD_MVHL_FIRST_CHK,                m_bLM3LoadCase1);
	DDX_Check  (pDX, IDC_CMD_MVHL_SECOND_CHK,               m_bLM3LoadCase2);
	DDX_Control(pDX, IDC_CMD_MVHL_FOOTWAY_W_EDT,            m_edtFootway);
	DDX_Control(pDX, IDC_CMD_MVHL_FOOTWAY_W_UNT,            m_untFootway);
	DDX_Check  (pDX, IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK, m_bEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT, m_edtEccenVertLoad);
	DDX_Control(pDX, IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT, m_untEccenVertLoad);

	DDX_Control(pDX, IDC_CMD_ONE_GRID, m_wndOneGrid);
	DDX_Control(pDX, IDC_CMD_FIRST_GRID, m_wndThreeGrid[0]);
	DDX_Control(pDX, IDC_CMD_SECOND_GRID, m_wndThreeGrid[1]);
	DDX_Control(pDX, IDC_CMD_THIRD_GRID, m_wndThreeGrid[2]);
	DDX_Control(pDX, IDC_CMD_MVHL_PIC_WND, m_wndPicture);

	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_CHK, m_chkPatch);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_EDT, m_edtPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_W_UNT, m_untPatchW);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_EDT, m_edtPatchL);
	DDX_Control(pDX, IDC_CMD_MVHL_PATCH_L_UNT, m_untPatchL);

	//}}AFX_DATA_MAP
}

BOOL CCMMvhlItemStdEuroBSNewDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	CDlgUtil::CobxAddItem(m_cmbCodeName, _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE),         D_MVHL_EURO_BS_ROADBRIDGE);
	CDlgUtil::CobxAddItem(m_cmbCodeName, _LS(IDS_CMD_MVHL_EURO_BS_FOOTBRIDGE),         D_MVHL_EURO_BS_FOOTBRIDGE);
	CDlgUtil::CobxAddItem(m_cmbCodeName, _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE_FATIGUE), D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE);
	CDlgUtil::CobxAddItem(m_cmbCodeName, _LS(IDS_CMD_MVHL_EURO_BS_TRAIL_LOAD),         D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD);

	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);

	// TODO: Add extra initialization here
	if(!m_bModify) // New
	{
		m_Data.Initialize();
		m_Data.bStandard = TRUE;    
		m_Data.VehicleTypeName = _T("Load Model 1");
		m_nStndCode = D_MVHL_EURO_BS_ROADBRIDGE+1;
		m_Data.nStandardCode = m_nStndCode+18;
		if(m_Data.nStandardCode == 22) m_Data.nStandardCode = 23; // 22 번은 AASHTO Legal Load 

		if (!db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB)) ASSERT(0);
		m_Data.PatchLoad = PatchLoadDB;
	}
	else
	{
		if (!m_Data.PatchLoad.bPatchLoad)
		{
			T_MVHL_D TempData = m_Data;
			if (!db.GetStandardVehicleLoadValue(TempData, &PatchLoadDB)) ASSERT(0);
			m_Data.PatchLoad = PatchLoadDB;
		}
	}
	int nLoadType = 0; 
	int nSelVehicle = 0;
	int nCodeType = GetIndex(nLoadType, nSelVehicle);
	m_nStndCode = nCodeType + 1;
	if (nCodeType < 0) return TRUE;

	InitControls();
	InitUnit();
	SetLoadTypeList(nCodeType);
	SetSelVehicleList();
	AlignControl();		// 코드타입에 따라 프레임 정렬
	ControlsShowHide();

	m_wndOneGrid.SetDataSource(&m_Data,this,TRUE);

	m_wndThreeGrid[0].SetDataSource(&m_Data,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,0),nLoadType);
	m_wndThreeGrid[1].SetDataSource(&m_Data,this,1,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,1),nLoadType);
	m_wndThreeGrid[2].SetDataSource(&m_Data,this,2,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,2),nLoadType);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,0));
	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,1));
	GetDlgItem(IDC_CMD_MVHL_THIRD_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,2));
	GetDlgItem(IDC_CMD_MVHL_FIRST_CHK)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListChk(m_Data,0));
	GetDlgItem(IDC_CMD_MVHL_SECOND_CHK)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListChk(m_Data,1));
	
	SetHeaderTitle();
	Data2Dlg();
	ChangeCtrlText();
	EnableDisableControls();
	ChangeEditText();

	if(m_bModify)	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
#if defined(_MGEN)
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT)->ShowWindow(SW_HIDE);
#endif
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-------------------------------------------------------------------------
// Implementation

void CCMMvhlItemStdEuroBSNewDlg::SetHeaderTitle()
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

void CCMMvhlItemStdEuroBSNewDlg::MakeItemEx()
{
	// List 관련 된 것만 여기서 처리 해준다.
	m_lstTrain.DeleteAllItems();    
	int nLoadCount = GetTrainLoadCount();
	int nDistCount = GetTrainDistCount();
	
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

BOOL CCMMvhlItemStdEuroBSNewDlg::InsertItem(int nPos, int nNo, double dLoad, double dDist)
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

int CCMMvhlItemStdEuroBSNewDlg::GetTrainLoadCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointLoad[i] == 0.0) break;
	}
	return i;
}

int CCMMvhlItemStdEuroBSNewDlg::GetTrainDistCount()
{
	int i = 0;
	for( i = 0; i < D_MVHL_NUMLOAD; i++)
	{
		if (m_Data.dPointDistance[i] == 0.0) break;
	}  
	return i;
}

void CCMMvhlItemStdEuroBSNewDlg::InitControls()
{
	m_wndOneGrid.InitGrid();

	m_wndThreeGrid[0].InitGrid(110);
	m_wndThreeGrid[1].InitGrid(110);
	m_wndThreeGrid[2].InitGrid(110);
}

void CCMMvhlItemStdEuroBSNewDlg::InitUnit()
{
	m_edtTSFactor.SetUnitType(D_UNITSYS_NONE);
	m_edtUDLFactor.SetUnitType(D_UNITSYS_NONE);

	m_edtInterval.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untInterval.SetUnitType(D_UNITSYS_BASE_LENGTH);

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
	
	m_edtFootway.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untFootway.SetUnitType(D_UNITSYS_BASE_STRESS);

	m_edtEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untEccenVertLoad.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchW.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untPatchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMMvhlItemStdEuroBSNewDlg::Data2Dlg()
{
	m_nStndCode = m_Data.nStandardCode-18;
	if(m_Data.nStandardCode == 23) m_nStndCode = 4; // 22 번은 AASHTO Legal Load 

	if(m_Data.VehicleLoadName.IsEmpty())
		m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleTypeName));
	else
		m_edtVehicleName.SetWindowText(ConvVehicleTypeStrRaw2Trans(m_Data.VehicleLoadName));
	
	int nLoadType = 0; 
	int nSelVehicle = 0;
	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode-1);
	GetIndex(nLoadType, nSelVehicle);
	m_cmbVehicleTypeList.SetCurSel(nLoadType);
	ChangeBitmap(m_nStndCode-1, nLoadType, nSelVehicle);
	m_cmbSelVihicle.SetCurSel(nSelVehicle);

	T_MVHL_PATCH_LOAD PatchLoadDB;
	T_MVHL_D tempMvhl;
	tempMvhl = m_Data;
	CVehlDB db(m_pDoc);  
	if (db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))
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
				m_Data.dFootway2 = tempMvhl.dFootway2;

				if(!m_Data.PatchLoad.bPatchLoad)
					m_Data.PatchLoad = PatchLoadDB;
			}
			else
			{
				m_Data.PatchLoad = PatchLoadDB;
			}
			m_bInit = TRUE;
		}
		else
		{
			if (m_Data.PatchLoad.aPatchLoad.GetSize() == 0)
			{
				m_Data.PatchLoad = PatchLoadDB;
			}
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
	m_edtInterval.SetEditUnit(m_Data.dInterval);
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

	m_bLM3LoadCase1 = m_Data.bLM3LoadCase[0];
	m_bLM3LoadCase2 = m_Data.bLM3LoadCase[1];

	m_edtFootway.SetEditUnit(m_Data.dFootway2);

	m_bEccenVertLoad = m_Data.bEccenVertLoad;
	m_edtEccenVertLoad.SetEditUnit(m_Data.dEccenVertLoad);

	if (m_Data.PatchLoad.aPatchLoad.GetSize() > 0)
	{
		m_chkPatch.SetCheck(m_Data.PatchLoad.bPatchLoad);
		m_edtPatchW.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dWidth);
		m_edtPatchL.SetEditUnit(m_Data.PatchLoad.aPatchLoad[0].dLength);
	}
	else
	{
		m_edtPatchW.SetEditUnit(0.0);
		m_edtPatchW.SetEditUnit(0.0);
	}
	OnChkPatch();

	UpdateData(FALSE);
}

BOOL CCMMvhlItemStdEuroBSNewDlg::Dlg2Data()
{
	UpdateData(TRUE);

	T_MVHL_PATCH_LOAD PatchLoadOld;
	PatchLoadOld = m_Data.PatchLoad;

	m_Data.Initialize();  
	m_Data.PatchLoad = PatchLoadOld;
	m_edtVehicleName.GetWindowText(m_Data.VehicleLoadName);   
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_cmbSelVihicle.GetWindowText(m_Data.SelVehicle);
	//m_Data.dDynLoadAllowance = m_wndDynLoadAllowance.GetEditValue();
	m_Data.nStandardCode = m_nStndCode+18;
	if(m_Data.nStandardCode == 22) m_Data.nStandardCode = 23; // 22 번은 AASHTO Legal Load 
	CString strTemp;
	CVehlDB db(m_pDoc);

	T_MVHL_PATCH_LOAD PatchLoadDB;
	if(!db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB)) return FALSE;

	m_wndAdjust2.GetWindowText(strTemp);
	m_Data.dAdjustment2 = _tstof(strTemp);
	m_wndAdjust.GetWindowText(strTemp);
	m_Data.dAdjustment = _tstof(strTemp);

	CString strTypeName = m_Data.VehicleTypeName;
	strTypeName.MakeUpper();
	if(strTypeName==_T("UNLOADED TRAIN"))
	{
		m_Data.dAdjustment=1.0;
	}

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
		m_Data.dInterval = m_edtInterval.GetEditValue();
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

	m_Data.bLM3LoadCase[0] = m_bLM3LoadCase1;

	if(m_Data.SelVehicle == _T("600/150") || m_Data.SelVehicle == _T("900/150")) m_Data.bLM3LoadCase[1] = FALSE;
	else  m_Data.bLM3LoadCase[1] = m_bLM3LoadCase2;
	
	m_Data.dFootway2 = m_edtFootway.GetEditValue();

	m_Data.bEccenVertLoad = m_bEccenVertLoad;
	m_Data.dEccenVertLoad = m_edtEccenVertLoad.GetEditValue();

	m_Data.PatchLoad.bPatchLoad = m_chkPatch.GetCheck();
	if (!m_Data.PatchLoad.bPatchLoad)
	{
		m_Data.PatchLoad = PatchLoadDB;
	}
	else
	{
		if (m_bPatchGrid) {/*skip*/ }
		else
		{
			T_MVHL_PATCH_ITEM Patch;
			Patch.dWidth = m_edtPatchW.GetEditValue();
			Patch.dLength = m_edtPatchL.GetEditValue();
			Patch.nPatchType = PatchLoadDB.aPatchLoad[0].nPatchType;
			m_Data.PatchLoad.aPatchLoad.RemoveAll();
			m_Data.PatchLoad.aPatchLoad.Add(Patch);
		}
	}

	return TRUE;
}

void CCMMvhlItemStdEuroBSNewDlg::AlignControl()
{
	int nType = m_nStndCode-1;
	int nLoadType = 0; 
	int nSelVehicle = 0;
	GetIndex(nLoadType, nSelVehicle);

	CRect rRef, rRef2;
	CRect rToMove;
	CRect rToMove2;
	int nDistY,nDistY2;
	int nDistXp = 0.0;
	int nDistYp = 0.0;

	CArray<UINT, UINT> aControls; aControls.RemoveAll();

	switch(nType)
	{
	case 0:  // Road Bridge
		switch(nLoadType)
		{
		case 0: // Load Model 1
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ONE_GRID)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aOneGrid, nDistY);
			CDlgUtil::CtrlMoveDistY(this, m_aOneGrid1, nDistY);

			GetDlgItem(IDC_CMD_MVHL_UDLFACT_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
			nDistXp = rRef.left - rToMove.left;
			nDistYp = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(15);
			break;
		case 1: // Load Model 2
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST2_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust2, nDistY);

			GetDlgItem(IDC_CMD_MVHL_ADJUST2_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(5);
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);

			GetDlgItem(IDC_CMD_MVHL_ADJUST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
			nDistXp = rRef.left - rToMove.left;
			nDistYp = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(15);
			break;
		case 2: // Load Model 3
			aControls.Add(IDC_CMD_MVHL_PHI_TXT);
			aControls.Add(IDC_CMD_MVHL_PHI_EDT);
			
			GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);
		
			GetDlgItem(IDC_CMD_MVHL_TSFACT_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->GetWindowRect(rRef2);
			GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
			nDistXp = rRef.left - rToMove.left;
			nDistYp = rRef2.top - rToMove.top;
			break;
		case 3: // Load Model (UK NA)
			aControls.Add(IDC_CMD_MVHL_PHI_TXT);
			aControls.Add(IDC_CMD_MVHL_PHI_EDT);
			
			GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, aControls, nDistY);

			GetDlgItem(IDC_CMD_MVHL_TSFACT_EDT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_DYNAMIC_CHK)->GetWindowRect(rRef2);
			GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
			nDistXp = rRef.left - rToMove.left;
			nDistYp = rRef2.top - rToMove.top;
			break;
		case 4:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);
			break;
		default:
			break;
		}break;
	case 1: // Footway
		switch(nLoadType)
		{
		case 0:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_FOOTWAY_W_STC)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aFootway, nDistY);

			GetDlgItem(IDC_CMD_MVHL_FOOTWAY_W_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);
			break;
		case 1:
		case 3:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_ADJUST_EDT)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aAdjust, nDistY);
			break;
		case 2:
			break;
		}break;
	case 2: // Fatigue
		if(nLoadType==0)
		{
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_ONE_GRID)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aOneGrid, nDistY);
		}
		GetDlgItem(IDC_CMD_FIRST_GRID)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_EDT)->GetWindowRect(rToMove);
		GetDlgItem(IDC_CMD_MVHL_INTERVAL_TXT)->GetWindowRect(rToMove2);
		nDistY = rRef.bottom+ globalUtils.ScaleByDPI(20) - rToMove.top;
		nDistY2 = rRef.bottom+ globalUtils.ScaleByDPI(20) + (rToMove2.bottom - rToMove2.top+ globalUtils.ScaleByDPI(10)) - rToMove2.top;
		if(nLoadType == 7)
		{
			nDistY +=  (rToMove2.bottom - rToMove2.top+ globalUtils.ScaleByDPI(10));
			nDistY2 -= (rToMove2.bottom - rToMove2.top+ globalUtils.ScaleByDPI(10));
		}
		CDlgUtil::CtrlMoveDistY(this, m_aDynamic, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aInterval, nDistY2);

		GetDlgItem(IDC_CMD_MVHL_DYNAMIC_TXT)->GetWindowRect(rRef);
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK)->GetWindowRect(rToMove);
		nDistXp = rRef.left - rToMove.left;
		nDistYp = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(15);

		break;
	case 3: // Rail
		switch(nLoadType)
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
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
			CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			break;
		case 4:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_A_N_STC)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aHSLM_A, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_A_P_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(20);
			CDlgUtil::CtrlMoveDistY(this, m_aDynEff, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(20);
			CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			break;
		case 5:
			GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_B_N_STC)->GetWindowRect(rToMove);
			nDistY = rRef.top - rToMove.top;
			CDlgUtil::CtrlMoveDistY(this, m_aHSLM_B, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_HSLM_B_P_STC)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(20);
			CDlgUtil::CtrlMoveDistY(this, m_aDynEff, nDistY);

			GetDlgItem(IDC_CMD_MVHL_EURO_DYN_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(20);
			CDlgUtil::CtrlMoveDistY(this, m_aETC, nDistY);
			break;
		}
	}

	CDlgUtil::CtrlMoveDistXY(this, m_aCtrlPatch, nDistXp, nDistYp);

	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_FIRST_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aThreeGridChk1, nDistY);

	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_MVHL_SECOND_CHK)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aThreeGridChk2, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_APPLY);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);
	MoveWindow(r);
	Invalidate(TRUE);
	UpdateWindow();
}

void CCMMvhlItemStdEuroBSNewDlg::ControlsShowHide()
{
	int nType = m_nStndCode-1;
	int nLoadType = 0; 
	int nSelVehicle = 0;
	GetIndex(nLoadType, nSelVehicle);

	BOOL bOneGrid  = FALSE, bOneGrid1     = FALSE, bThreeGrid     = FALSE, bAdjust   = FALSE;
	BOOL bAdjust2  = FALSE, bFootway      = FALSE, bPhi           = FALSE, bDynamic  = FALSE;
	BOOL bInterval = FALSE, bSOSOV        = FALSE, bSelVehicle    = FALSE, bListEtc  = FALSE;
	BOOL bHSLM_A   = FALSE, bHSLM_B       = FALSE, bDynEff        = FALSE, bETC      = FALSE;
	BOOL bSOSOVA   = FALSE, bThreeGridTxt = FALSE, bThreeGridChk1 = FALSE, bThreeGridChk2 = FALSE;
	BOOL bPatch    = FALSE;

	if(nType == 0) // Road Bridge
	{
		if     (nLoadType == 0)  { bOneGrid = TRUE; bOneGrid1 = TRUE; bPatch=TRUE;} // Load Model 1
		else if(nLoadType == 1)  { bAdjust  = TRUE;  bAdjust2 = TRUE; bPatch=TRUE;} // Load Model 2
		else if(nLoadType == 3)  { bThreeGrid = TRUE; bThreeGridTxt = TRUE; bSOSOV = TRUE; bSOSOVA = TRUE; bSelVehicle = TRUE; bPatch=TRUE;} // Load Model 3 (UK NA)
		else if(nLoadType == 4) { bAdjust  = TRUE; } // Load Model 4
		else // Load Model 3
		{ 
			bPhi = TRUE; bThreeGrid = TRUE; bSelVehicle = TRUE; bPatch = TRUE;
		
// 			if (nSelVehicle == 0 || nSelVehicle == 1) { bThreeGridChk1 = TRUE; }
// 			else { bThreeGridChk1 = TRUE; bThreeGridChk2 = TRUE; }
			if(nSelVehicle != 0 && nSelVehicle != 1) { bThreeGridChk1 = TRUE; bThreeGridChk2 = TRUE; }
		} 
	}
	else if(nType == 1) // Footway
	{
		if     (nLoadType == 0) { bFootway = TRUE; bAdjust = TRUE; }
		else if(nLoadType == 1) { bAdjust = TRUE; }
		else if(nLoadType == 3) { bAdjust = TRUE; }
	}
	else if(nType == 2) // Fatigue
	{
		bOneGrid = (nLoadType==0);
		bThreeGrid = (nLoadType!=0);
		bThreeGridTxt = (nLoadType!=0);
		if(nLoadType == 7) bInterval = TRUE;
		bDynamic = TRUE;

		if (nLoadType == 0 ||                  // Fatigue Load Model 1
			nLoadType == 6 || nLoadType == 7)  // Fatigue Load Model 3
			bPatch = TRUE;
	}
	else if(nType == 3) // Rail
	{
		if     (nLoadType == 4)  { bSelVehicle = TRUE; bHSLM_A = TRUE; bDynEff = TRUE; bETC = TRUE;}
		else if(nLoadType == 5)  { bHSLM_B = TRUE; bDynEff = TRUE; bETC = TRUE;}
		else if(nLoadType == 0 || nLoadType == 1 || nLoadType == 2 || nLoadType == 3)
		{
			bListEtc = TRUE; bETC = TRUE;
		}
	}

	CDlgUtil::CtrlShowHide(this,m_aOneGrid,       bOneGrid);
	CDlgUtil::CtrlShowHide(this,m_aOneGrid1,      bOneGrid1);
	CDlgUtil::CtrlShowHide(this,m_aThreeGrid,     bThreeGrid);
	CDlgUtil::CtrlShowHide(this,m_aThreeGridTxt,  bThreeGridTxt);
	CDlgUtil::CtrlShowHide(this,m_aThreeGridChk1, bThreeGridChk1);
	CDlgUtil::CtrlShowHide(this,m_aThreeGridChk2, bThreeGridChk2);
	CDlgUtil::CtrlShowHide(this,m_aPhi,           bPhi);
	CDlgUtil::CtrlShowHide(this,m_aDynamic,       bDynamic);
	CDlgUtil::CtrlShowHide(this,m_aAdjust,        bAdjust);
	CDlgUtil::CtrlShowHide(this,m_aAdjust2,       bAdjust2);
	CDlgUtil::CtrlShowHide(this,m_aFootway,       bFootway);
	CDlgUtil::CtrlShowHide(this,m_aInterval,      bInterval);
	if(bSOSOV) CDlgUtil::CtrlShowHide(this, m_aSOSOV, bSOSOV);
	CDlgUtil::CtrlShowHide(this,m_aSelVehicle,    bSelVehicle);
	CDlgUtil::CtrlShowHide(this,m_aListEtc,       bListEtc);
	CDlgUtil::CtrlShowHide(this,m_aHSLM_A,        bHSLM_A);
	CDlgUtil::CtrlShowHide(this,m_aHSLM_B,        bHSLM_B);
	CDlgUtil::CtrlShowHide(this,m_aDynEff,        bDynEff);
	CDlgUtil::CtrlShowHide(this,m_aETC,           bETC);
	GetDlgItem(IDC_CMD_MVHL_SVSOV_A_STC)->ShowWindow(bSOSOVA);

	m_bPatchGrid = FALSE;
	CDlgUtil::CtrlShowHide(this, m_aCtrlPatch, bPatch);
	if (nType == 2) // Fatigue
	{
		BOOL bBtn = FALSE;
		BOOL bCtr = FALSE;
		if (nLoadType == 0 || nLoadType == 6 || nLoadType == 7)  // Fatigue Load Model 1, 3
		{
			bBtn = FALSE;
			bCtr = TRUE;
		}
		else // 2,4
		{
			bBtn = TRUE;
			bCtr = FALSE;
		}
		GetDlgItem(IDC_CMD_MVHL_PATCH_CHK  )->ShowWindow(bBtn || bCtr);
		GetDlgItem(IDC_CMD_MVHL_PATCH_BTN  )->ShowWindow(bBtn);
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->ShowWindow(bCtr);
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->ShowWindow(bCtr);
		GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->ShowWindow(bCtr);
		GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->ShowWindow(bCtr);
		GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->ShowWindow(bCtr);
		GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->ShowWindow(bCtr);

		m_bPatchGrid = bBtn;
	}
	else
	{
		GetDlgItem(IDC_CMD_MVHL_PATCH_BTN)->ShowWindow(FALSE);
	}
}

void CCMMvhlItemStdEuroBSNewDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	int nType = m_nStndCode-1;
	int nLoadType = 0; 
	int nSelVehicle = 0;
	GetIndex(nLoadType, nSelVehicle);

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

	// Trail Load
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_STC)->EnableWindow(nType == 3 && (nLoadType==0 || nLoadType==1));
	GetDlgItem(IDC_CMD_MVHL_EURO_ALPHA_EDT)->EnableWindow(nType == 3 && (nLoadType==0 || nLoadType==1));
	GetDlgItem(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK)->EnableWindow(nType == 3 && (nLoadType==0 || nLoadType==4));
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_STC)->EnableWindow(nType == 3 && (nLoadType==0 || nLoadType==4) && m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_EDT)->EnableWindow(nType == 3 && (nLoadType==0 || nLoadType==4) && m_bLongiDist);
	GetDlgItem(IDC_CMD_MVHL_EURO_DIST_POINT_UNT)->EnableWindow(nType == 3 && (nLoadType==0 || nLoadType==4) && m_bLongiDist);

	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_EDT)->EnableWindow(nType == 3 && m_bEccenVertLoad);
	GetDlgItem(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_UNT)->EnableWindow(nType == 3 && m_bEccenVertLoad);
}

BOOL CCMMvhlItemStdEuroBSNewDlg::ApplyOrOK()
{
	if(!Dlg2Data()) return FALSE;
	BOOL bSuccess;
	if(m_bModify) bSuccess = m_pDoc->m_pDataCtrl->ModifyMvhl(m_csOldName, m_Data);
	else          bSuccess = m_pDoc->m_pDataCtrl->AddMvhl(m_Data);
	if(!bSuccess) return FALSE;

	return TRUE;
}

BEGIN_MESSAGE_MAP(CCMMvhlItemStdEuroBSNewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvhlItemStdEuroBSNewDlg)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_TYPE_COMBO,           OnSelchangeCmdMvhlTypeCombo)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_CODE_CMB,             OnSelchangeCmdMvhlCodeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVHL_EURO_SEL_VEHICLE_CMB, OnCmdMvhlEuroSelVehicleCmb)
	ON_BN_CLICKED(IDC_CMD_MVHL_DYNAMIC_CHK,         OnCmdMvhlEuroDynamicChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_AUTO_RDO,            OnCmdMvhlEuroAutoRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_USER_RDO,            OnCmdMvhlEuroAutoRdo)
	ON_BN_CLICKED(IDC_CMD_MVHL_EURO_LONGI_DIST_CHK, OnCmdMvhlEuroLongiDistChk)
	ON_BN_CLICKED(IDC_CMD_MVHL_EURO_ECCEN_VERT_LOAD_CHK, OnCmdMvhlEuroEccenVertLoadChk)
	ON_BN_CLICKED(IDC_CMD_APPLY,                    OnCmdMvhlEuroApply)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_CHK,           OnChkPatch)
	ON_BN_CLICKED(IDC_CMD_MVHL_PATCH_BTN,           OnPatchBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdEuroBSNewDlg message handlers

void CCMMvhlItemStdEuroBSNewDlg::OnOK() 
{
	if (!ApplyOrOK()) return;	
	CDialogMove::OnOK();
}

void CCMMvhlItemStdEuroBSNewDlg::OnCmdMvhlEuroApply() 
{	
	ApplyOrOK();
}

void CCMMvhlItemStdEuroBSNewDlg::OnChkPatch()
{
	BOOL bPatchLoad = m_chkPatch.GetCheck();

	GetDlgItem(IDC_CMD_MVHL_PATCH_BTN  )->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_W_UNT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_STC)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_EDT)->EnableWindow(bPatchLoad);
	GetDlgItem(IDC_CMD_MVHL_PATCH_L_UNT)->EnableWindow(bPatchLoad);
}

void CCMMvhlItemStdEuroBSNewDlg::ChangeBitmap(int nCodeType, int nLoadType, int nSelVehicle)
{
	CString strSVG;
	switch(nCodeType)
	{
		case D_MVHL_EURO_BS_ROADBRIDGE:
			if     (nLoadType == 0) strSVG = _T("cmd_mvhl_51.svg");
			else if(nLoadType == 1) strSVG = _T("cmd_mvhl_52.svg");
			else if(nLoadType == 2) 
			{
				if     (nSelVehicle == 0) strSVG = _T("cmd_mvhl_53_1.svg");
				else if(nSelVehicle == 1) strSVG = _T("cmd_mvhl_53_1.svg");
				else if(nSelVehicle == 2) strSVG = _T("cmd_mvhl_53_2.svg");
				else if(nSelVehicle == 3) strSVG = _T("cmd_mvhl_53_2.svg");
				else if(nSelVehicle == 4) strSVG = _T("cmd_mvhl_53_2.svg");
				else if(nSelVehicle == 5) strSVG = _T("cmd_mvhl_53_2.svg");
				else if(nSelVehicle == 6) strSVG = _T("cmd_mvhl_53_2.svg");
				else if(nSelVehicle == 7) strSVG = _T("cmd_mvhl_53_2.svg");
				else ASSERT(0);
			}
			else if(nLoadType == 3)
			{
				if (nSelVehicle == 0) strSVG = _T("cmd_mvhl_88.svg");
				else if (nSelVehicle == 1) strSVG = _T("cmd_mvhl_88.svg");
				else if (nSelVehicle == 2) strSVG = _T("cmd_mvhl_89.svg");
				else if (nSelVehicle == 3) strSVG = _T("cmd_mvhl_90.svg");
				else if (nSelVehicle == 4) strSVG = _T("cmd_mvhl_91.svg");
				else if (nSelVehicle == 5) strSVG = _T("cmd_mvhl_92.svg");
				else if (nSelVehicle == 6) strSVG = _T("cmd_mvhl_93.svg");
				else ASSERT(0);
			}
			else if(nLoadType == 4) strSVG = _T("cmd_mvhl_54.svg");
			else ASSERT(0);
			break;
		case D_MVHL_EURO_BS_FOOTBRIDGE:
			if     (nLoadType == 0) strSVG = _T("cmd_mvhl_55.svg");
			else if(nLoadType == 1) strSVG = _T("cmd_mvhl_56.svg");
			else if(nLoadType == 2) strSVG = _T("cmd_mvhl_57.svg");
			else if(nLoadType == 3) strSVG = _T("cmd_mvhl_56_NA.svg");
			else ASSERT(0);
			break;
		case D_MVHL_EURO_BS_ROADBRIDGE_FATIGUE:
			if     (nLoadType == 0)  strSVG = _T("cmd_mvhl_58.svg"); 
			else if(nLoadType == 1)  strSVG = _T("cmd_mvhl_59.svg"); 
			else if(nLoadType == 2)  strSVG = _T("cmd_mvhl_60.svg"); 
			else if(nLoadType == 3)  strSVG = _T("cmd_mvhl_61.svg"); 
			else if(nLoadType == 4)  strSVG = _T("cmd_mvhl_62.svg"); 
			else if(nLoadType == 5)  strSVG = _T("cmd_mvhl_63.svg"); 
			else if(nLoadType == 6)  strSVG = _T("cmd_mvhl_64_1.svg"); 
			else if(nLoadType == 7)  strSVG = _T("cmd_mvhl_64_2.svg"); 
			else if(nLoadType == 8)  strSVG = _T("cmd_mvhl_65.svg"); 
			else if(nLoadType == 9)  strSVG = _T("cmd_mvhl_66.svg"); 
			else if(nLoadType == 10) strSVG = _T("cmd_mvhl_67.svg"); 
			else if(nLoadType == 11) strSVG = _T("cmd_mvhl_68.svg"); 
			else if(nLoadType == 12) strSVG = _T("cmd_mvhl_69.svg"); 
			else ASSERT(0);
			break;
		case D_MVHL_EURO_BS_RAIL_TRAFFIC_LOAD:
			if     (nLoadType == 0)  strSVG = _T("cmd_mvhl_94.svg"); 
			else if(nLoadType == 1)  strSVG = _T("cmd_mvhl_95.svg"); 
			else if(nLoadType == 2)  strSVG = _T("cmd_mvhl_95.svg"); 
			else if(nLoadType == 3)  strSVG = _T("cmd_mvhl_98.svg"); 
			else if(nLoadType == 4)  strSVG = _T("cmd_mvhl_96.svg"); 
			else if(nLoadType == 5)  strSVG = _T("cmd_mvhl_97.svg"); 
			else ASSERT(0);
			break;
		default : ASSERT(0);
	}

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture.SetImage(strSVGPath + strSVG);
}

void CCMMvhlItemStdEuroBSNewDlg::OnSelchangeCmdMvhlCodeCmb() 
{
	int nCodeType = CDlgUtil::CobxGetCurSelItemData(m_cmbCodeName, m_cmbCodeName.GetCurSel());
	SetLoadTypeList(nCodeType);
	SetSelVehicleList();

	int nIndex = m_cmbCodeName.GetCurSel();
	m_nStndCode = m_cmbCodeName.GetItemData(nIndex) + 1;

	ChangeData();
}
void CCMMvhlItemStdEuroBSNewDlg::OnCmdMvhlEuroDynamicChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSNewDlg::OnCmdMvhlEuroAutoRdo()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSNewDlg::OnCmdMvhlEuroSelVehicleCmb()
{
	ChangeData();
	ChangeCtrlText();
}

void CCMMvhlItemStdEuroBSNewDlg::OnCmdMvhlEuroLongiDistChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSNewDlg::OnCmdMvhlEuroEccenVertLoadChk()
{
	EnableDisableControls();
}

void CCMMvhlItemStdEuroBSNewDlg::OnPatchBtn()
{
	CCMMvhlItemStdPatchLoadInfoDlg Dlg;
	Dlg.SetInitPos(D_INIT_POS_RT);
	Dlg.SetData(m_Data);

	if (Dlg.DoModal() == IDOK)
	{
		m_Data.PatchLoad = Dlg.m_Data.PatchLoad;
	}
}

void CCMMvhlItemStdEuroBSNewDlg::OnSelchangeCmdMvhlTypeCombo() 
{
	m_Data.VehicleTypeName = GetVehicleTypeStr();

	SetSelVehicleList();
	ChangeData();
	ChangeCtrlText();
}

BOOL CCMMvhlItemStdEuroBSNewDlg::ChangeData()
{
	m_Data.Initialize();
	m_Data.bStandard = TRUE;
	m_Data.VehicleTypeName = GetVehicleTypeStr();
	m_cmbSelVihicle.GetWindowText(m_Data.SelVehicle);
	m_Data.nStandardCode = m_nStndCode+18;
	if(m_Data.nStandardCode == 22) m_Data.nStandardCode = 23; // 22 번은 AASHTO Legal Load 
	
	T_MVHL_PATCH_LOAD PatchLoadDB;
	CVehlDB db(m_pDoc);
	if (db.GetStandardVehicleLoadValue(m_Data, &PatchLoadDB))
	{
		m_Data.PatchLoad = PatchLoadDB;
	}

	CDlgUtil::CobxSetCurSelItemData(m_cmbCodeName, m_nStndCode-1);
	
	int nLoadType = 0; 
	int nSelVehicle = 0;
	GetIndex(nLoadType, nSelVehicle);
	m_cmbVehicleTypeList.SetCurSel(nLoadType);
	ChangeBitmap(m_nStndCode-1, nLoadType, nSelVehicle);

	m_edtVehicleName.SetWindowText(m_Data.VehicleTypeName);

	m_wndThreeGrid[0].SetDataSource(&m_Data,this,0,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,0),nLoadType);
	m_wndThreeGrid[1].SetDataSource(&m_Data,this,1,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,1),nLoadType);
	m_wndThreeGrid[2].SetDataSource(&m_Data,this,2,CMMvhlStdEuroBSNewTempForRefactor::GetGridListSize(m_Data,2),nLoadType);
	GetDlgItem(IDC_CMD_MVHL_FIRST_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,0));
	GetDlgItem(IDC_CMD_MVHL_SECOND_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,1));
	GetDlgItem(IDC_CMD_MVHL_THIRD_TXT)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListTxt(m_Data,2));
	GetDlgItem(IDC_CMD_MVHL_FIRST_CHK)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListChk(m_Data,0));
	GetDlgItem(IDC_CMD_MVHL_SECOND_CHK)->SetWindowText(CMMvhlStdEuroBSNewTempForRefactor::GetDescListChk(m_Data,1));
	AlignControl();		// 코드타입에 따라 프레임 정렬
	ControlsShowHide();
	Data2Dlg();
	EnableDisableControls();
	ChangeEditText();

	return TRUE;
}

void CCMMvhlItemStdEuroBSNewDlg::SetLoadTypeList(int nIndex)
{
	if (nIndex < 0) return;

	m_cmbVehicleTypeList.ResetContent();
	
	CString str;

	if(nIndex == 0) // EN 1991-2:2003 - RoadBridge
	{
		str = _T("Load Model 1");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Load Model 2");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Load Model 3");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Load Model 3 (UK NA)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Load Model 4");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
	}
	else if(nIndex == 1) // EN 1991-2:2003 - Footway and FootBridge
	{
		str = _T("Uniform load (Road bridge footway)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Uniform load (Footbridge)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Concentrated Load");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Uniform load (Road bridge footway) UK NA");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
	}
	else if(nIndex == 2) // EN 1991-2:2003 - RoadBridge Fatigue
	{
		str = _T("Fatigue Load Model 1");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 2 (280)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 2 (360)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 2 (630)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 2 (560)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 2 (610)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 3 (One Vehicle)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 3 (Two Vehicle)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 4 (200)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 4 (310)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 4 (490)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 4 (390)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Fatigue Load Model 4 (450)");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
	}
	else if(nIndex == 3) // EN 1991-2:2003 - Rail Traffic Load
	{
		str = _T("Load Model 71");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Load Model SW/0");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Load Model SW/2");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("Unloaded Train");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("HSLM A1 ~ HSLM A10");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
		str = _T("HSLM B");
		m_cmbVehicleTypeList.AddString(ConvVehicleTypeStrRaw2Trans(str));
	}
	else
	{
		ASSERT(0);
	}
	
	m_cmbVehicleTypeList.SetCurSel(0);
}

void CCMMvhlItemStdEuroBSNewDlg::SetSelVehicleList()
{
	UpdateData(TRUE);

	int nType = m_nStndCode-1;

	m_cmbSelVihicle.ResetContent();
	if(nType==0) // RoadBridge
	{
		if(m_Data.VehicleTypeName == _T("Load Model 3"))
		{
			m_cmbSelVihicle.AddString(_T("600/150"));
			m_cmbSelVihicle.AddString(_T("900/150"));
			m_cmbSelVihicle.AddString(_T("1200/150/200"));
			m_cmbSelVihicle.AddString(_T("1500/150/200"));
			m_cmbSelVihicle.AddString(_T("1800/150/200"));
			m_cmbSelVihicle.AddString(_T("2400/200"));
			m_cmbSelVihicle.AddString(_T("3000/200"));
			m_cmbSelVihicle.AddString(_T("3600/200"));
		}
		else if(m_Data.VehicleTypeName == _T("Load Model 3 (UK NA)"))
		{
			m_cmbSelVihicle.AddString(_T("SV 80"));
			m_cmbSelVihicle.AddString(_T("SV 100"));
			m_cmbSelVihicle.AddString(_T("SV 196"));
			m_cmbSelVihicle.AddString(_T("SOV 250"));
			m_cmbSelVihicle.AddString(_T("SOV 350"));
			m_cmbSelVihicle.AddString(_T("SOV 450"));
			m_cmbSelVihicle.AddString(_T("SOV 600"));
		}
		else
		{
			// Nothing
		}
	}
	else if(nType==3) // Rail Traffic Load
	{
		if(m_Data.VehicleTypeName == _T("HSLM A1 ~ HSLM A10"))
		{
			m_cmbSelVihicle.AddString(_T("A1"));
			m_cmbSelVihicle.AddString(_T("A2"));
			m_cmbSelVihicle.AddString(_T("A3"));
			m_cmbSelVihicle.AddString(_T("A4"));
			m_cmbSelVihicle.AddString(_T("A5"));
			m_cmbSelVihicle.AddString(_T("A6"));
			m_cmbSelVihicle.AddString(_T("A7"));
			m_cmbSelVihicle.AddString(_T("A8"));
			m_cmbSelVihicle.AddString(_T("A9"));
			m_cmbSelVihicle.AddString(_T("A10"));
		}
		else
		{
			// Nothing
		}
	}
	else
	{
		// Nothing
	}
	m_cmbSelVihicle.SetCurSel(0);
}

int CCMMvhlItemStdEuroBSNewDlg::GetIndex(int &nLoadType, int &nSelVehicle)
{
	int nCode = -1;

	// EN 1991-2:2003 - RoadBridge
	if     (m_Data.VehicleTypeName == _T("Load Model 1")) { nLoadType = 0; nSelVehicle = 0; nCode = 0; }
	else if(m_Data.VehicleTypeName == _T("Load Model 2")) { nLoadType = 1; nSelVehicle = 0; nCode = 0; }
	else if(m_Data.VehicleTypeName == _T("Load Model 3"))
	{
		if     (m_Data.SelVehicle == _T("600/150"))      { nLoadType = 2; nSelVehicle = 0; nCode = 0; }
		else if(m_Data.SelVehicle == _T("900/150"))      { nLoadType = 2; nSelVehicle = 1; nCode = 0; }
		else if(m_Data.SelVehicle == _T("1200/150/200")) { nLoadType = 2; nSelVehicle = 2; nCode = 0; }
		else if(m_Data.SelVehicle == _T("1500/150/200")) { nLoadType = 2; nSelVehicle = 3; nCode = 0; }
		else if(m_Data.SelVehicle == _T("1800/150/200")) { nLoadType = 2; nSelVehicle = 4; nCode = 0; }
		else if(m_Data.SelVehicle == _T("2400/200"))     { nLoadType = 2; nSelVehicle = 5; nCode = 0; }
		else if(m_Data.SelVehicle == _T("3000/200"))     { nLoadType = 2; nSelVehicle = 6; nCode = 0; }
		else if(m_Data.SelVehicle == _T("3600/200"))     { nLoadType = 2; nSelVehicle = 7; nCode = 0; }
		else { ASSERT(0); }
	}
	else if(m_Data.VehicleTypeName == _T("Load Model 3 (UK NA)"))
	{
		if     (m_Data.SelVehicle == _T("SV 80"))   { nLoadType = 3; nSelVehicle = 0; nCode = 0; }
		else if(m_Data.SelVehicle == _T("SV 100"))  { nLoadType = 3; nSelVehicle = 1; nCode = 0; }
		else if(m_Data.SelVehicle == _T("SV 196"))  { nLoadType = 3; nSelVehicle = 2; nCode = 0; }
		else if(m_Data.SelVehicle == _T("SOV 250")) { nLoadType = 3; nSelVehicle = 3; nCode = 0; }
		else if(m_Data.SelVehicle == _T("SOV 350")) { nLoadType = 3; nSelVehicle = 4; nCode = 0; }
		else if(m_Data.SelVehicle == _T("SOV 450")) { nLoadType = 3; nSelVehicle = 5; nCode = 0; }
		else if(m_Data.SelVehicle == _T("SOV 600")) { nLoadType = 3; nSelVehicle = 6; nCode = 0; }
		else { ASSERT(0); }
	}
	else if(m_Data.VehicleTypeName == _T("Load Model 4")) { nLoadType = 4; nSelVehicle = 0; nCode = 0; }
	// EN 1991-2:2003 - Footway and FootBridge
	else if(m_Data.VehicleTypeName == _T("Uniform load (Road bridge footway)")) { nLoadType = 0; nSelVehicle = 0; nCode = 1; }
	else if(m_Data.VehicleTypeName == _T("Uniform load (Footbridge)"))          { nLoadType = 1; nSelVehicle = 0; nCode = 1; }
	else if(m_Data.VehicleTypeName == _T("Concentrated Load"))                  { nLoadType = 2; nSelVehicle = 0; nCode = 1; }
	else if(m_Data.VehicleTypeName == _T("Uniform load (Road bridge footway) UK NA"))    { nLoadType = 3; nSelVehicle = 0; nCode = 1; }
	// EN 1991-2:2003 - RoadBridge Fatigue
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 1"))       { nLoadType = 0;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (280)")) { nLoadType = 1;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (360)")) { nLoadType = 2;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (630)")) { nLoadType = 3;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (560)")) { nLoadType = 4;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 2 (610)")) { nLoadType = 5;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (One Vehicle)")) { nLoadType = 6; nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 3 (Two Vehicle)")) { nLoadType = 7; nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (200)")) { nLoadType = 8;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (310)")) { nLoadType = 9;  nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (490)")) { nLoadType = 10; nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (390)")) { nLoadType = 11; nSelVehicle = 0; nCode = 2; }
	else if(m_Data.VehicleTypeName == _T("Fatigue Load Model 4 (450)")) { nLoadType = 12; nSelVehicle = 0; nCode = 2; }
	// EN 1991-2:2003 - Rail Traffic Load
	else if(m_Data.VehicleTypeName == _T("Load Model 71"))   { nLoadType = 0; nSelVehicle = 0; nCode = 3; }
	else if(m_Data.VehicleTypeName == _T("Load Model SW/0")) { nLoadType = 1; nSelVehicle = 0; nCode = 3; }
	else if(m_Data.VehicleTypeName == _T("Load Model SW/2")) { nLoadType = 2; nSelVehicle = 0; nCode = 3; }
	else if(m_Data.VehicleTypeName == _T("Unloaded Train"))  { nLoadType = 3; nSelVehicle = 0; nCode = 3; }
	else if(m_Data.VehicleTypeName == _T("HSLM A1 ~ HSLM A10"))
	{
		if     (m_Data.SelVehicle == _T("A1"))  { nLoadType = 4; nSelVehicle = 0; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A2"))  { nLoadType = 4; nSelVehicle = 1; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A3"))  { nLoadType = 4; nSelVehicle = 2; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A4"))  { nLoadType = 4; nSelVehicle = 3; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A5"))  { nLoadType = 4; nSelVehicle = 4; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A6"))  { nLoadType = 4; nSelVehicle = 5; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A7"))  { nLoadType = 4; nSelVehicle = 6; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A8"))  { nLoadType = 4; nSelVehicle = 7; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A9"))  { nLoadType = 4; nSelVehicle = 8; nCode = 3; }
		else if(m_Data.SelVehicle == _T("A10")) { nLoadType = 4; nSelVehicle = 9; nCode = 3; }
		else { ASSERT(0); }
	}
	else if(m_Data.VehicleTypeName == _T("HSLM B")) { nLoadType = 5; nSelVehicle = 0; nCode = 3; }
	else { ASSERT(0); }

	if(nCode < 0) AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_vehicle_load_name));
	 
	return nCode;
}

void CCMMvhlItemStdEuroBSNewDlg::ChangeCtrlText()
{
	CString strText;
	strText.Format(_T("%s = 1.40 -L / 500  (1<= %s<= 1.40 )"), CSpecialChar::m_csPhi, CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_PHI_EQUATION_TXT)->SetWindowText(strText);
	
	strText.Format(_T("%s :"), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_PHI_TXT)->SetWindowText(strText);  
	
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_s_FACTOR_FOR_TANDEM_SYSTEM), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_TSFACT_STC)->SetWindowText(strText);
	
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_s_FACTOR_FOR_UDL_SYSTEM), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_UDLFACT_STC)->SetWindowText(strText);
	
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_s_FACTOR), CSpecialChar::m_csPsi);
	GetDlgItem(IDC_CMD_MVHL_ADJUST_TXT)->SetWindowText(strText);
	
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_DYNA_AMPLIFICATION_FACTOR_s), CSpecialChar::m_csPhi);
	GetDlgItem(IDC_CMD_MVHL_DYNAMIC_TXT)->SetWindowText(strText);
	
	strText.Format(_LS(IDS_CMD_MVHL_EURO_BS_TWO_VEHICLE_INTERVAL));
	GetDlgItem(IDC_CMD_MVHL_INTERVAL_TXT)->SetWindowText(strText);

	int nType = m_nStndCode-1;
	int nSelVehicle = m_cmbSelVihicle.GetCurSel();

	int nLoadType = 0; 
	int nSelVehicle2 = 0;
	GetIndex(nLoadType, nSelVehicle2);

	if(nType == 0)
	{
		if(nSelVehicle == 0 || nSelVehicle == 1 || nSelVehicle == 2)
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

void CCMMvhlItemStdEuroBSNewDlg::ChangeEditText()
{
	int nType = m_nStndCode-1;
	int nLoadType = 0; 
	int nSelVehicle = 0;
	GetIndex(nLoadType, nSelVehicle);

	if(nType == 3) // Train Load
	{
		if(nLoadType == 0) // Load Model 71
		{
			m_edtTrainDD1.SetWindowText(CSpecialChar::m_csInfinity);
			m_edtTrainDD2.SetWindowText(CSpecialChar::m_csInfinity);
		}
		else if(nLoadType == 1 || nLoadType == 2) // Load Model SW/0, Load Model SW/2
		{
			m_edtTrainD2.SetWindowText(_T(""));
		}
		else if(nLoadType == 3) // Unloaded Train
		{
			m_edtTrainDD1.SetWindowText(CSpecialChar::m_csInfinity);
			m_edtTrainD1 .SetWindowText(_T(""));
			m_edtTrainW2 .SetWindowText(_T(""));
			m_edtTrainDD2.SetWindowText(_T(""));
			m_edtTrainD2 .SetWindowText(_T(""));
		}
	}
}


typedef std::pair<CString, CString> pair_cstring;
template <std::size_t N> using array_pair_cstring = std::array<pair_cstring, N>;

template <std::size_t N>
CString ConvStrRaw2Trans(const array_pair_cstring<N>& aPairStr,
	const CString& strRaw)
{
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.first;
		if (strRaw == strTarget)
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
	for (const auto& strPair : aPairStr)
	{
		const auto& strTarget = strPair.second;
		if (strTrans == strTarget)
		{
			return strPair.first;
		}
	}

	return strTrans;
}

#define __static_caVehicleLoadType static const array_pair_cstring<26> caVehicleLoadType = \
{ \
    std::make_pair(_T("Load Model 1"),                             _LS(IDS_WG_CMD_Load_Model_1)), \
    std::make_pair(_T("Load Model 2"),                             _LS(IDS_WG_CMD_Load_Model_2)), \
    std::make_pair(_T("Load Model 3"),                             _LS(IDS_WG_CMD_Load_Model_3)), \
    std::make_pair(_T("Load Model 3 (UK NA)"),                     _LS(IDS_WG_CMD_Load_Model_3_UK_NA)), \
    std::make_pair(_T("Load Model 4"),                             _LS(IDS_WG_CMD_Load_Model_4)), \
    std::make_pair(_T("Uniform load (Road bridge footway)"),       _LS(IDS_WG_CMD_Uniform_load__Road_bridge_footway)), \
    std::make_pair(_T("Uniform load (Footbridge)"),                _LS(IDS_WG_CMD_Uniform_load__Footbridge)), \
    std::make_pair(_T("Concentrated Load"),                        _LS(IDS_WG_CMD_Concentrated_Load)), \
    std::make_pair(_T("Uniform load (Road bridge footway) UK NA"), _LS(IDS_WG_CMD_Uniform_load__Road_bridge_footway_UK_NA)), \
    std::make_pair(_T("Fatigue Load Model 1"),                     _LS(IDS_WG_CMD_Fatigue_Load_Model_1)), \
    std::make_pair(_T("Fatigue Load Model 2 (280)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_2_280)), \
    std::make_pair(_T("Fatigue Load Model 2 (360)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_2_360)), \
    std::make_pair(_T("Fatigue Load Model 2 (630)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_2_630)), \
    std::make_pair(_T("Fatigue Load Model 2 (560)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_2_560)), \
    std::make_pair(_T("Fatigue Load Model 2 (610)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_2_610)), \
    std::make_pair(_T("Fatigue Load Model 3 (One Vehicle)"),       _LS(IDS_WG_CMD_Fatigue_Load_Model_3_One_Vehicle)), \
    std::make_pair(_T("Fatigue Load Model 3 (Two Vehicle)"),       _LS(IDS_WG_CMD_Fatigue_Load_Model_3_Two_Vehicle)), \
    std::make_pair(_T("Fatigue Load Model 4 (200)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_4_200)), \
    std::make_pair(_T("Fatigue Load Model 4 (310)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_4_310)), \
    std::make_pair(_T("Fatigue Load Model 4 (490)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_4_490)), \
    std::make_pair(_T("Fatigue Load Model 4 (390)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_4_390)), \
    std::make_pair(_T("Fatigue Load Model 4 (450)"),               _LS(IDS_WG_CMD_Fatigue_Load_Model_4_450)), \
    std::make_pair(_T("Load Model 71"),                            _LS(IDS_WG_CMD_Load_Model_71)), \
    std::make_pair(_T("Load Model SW/0"),                          _LS(IDS_WG_CMD_Load_Model_SW_0)), \
    std::make_pair(_T("Load Model SW/2"),                          _LS(IDS_WG_CMD_Load_Model_SW_2)), \
    std::make_pair(_T("Unloaded Train"),                           _LS(IDS_WG_CMD_Unloaded_Train)), \
}; \

CString CCMMvhlItemStdEuroBSNewDlg::ConvVehicleTypeStrRaw2Trans(const CString& strRaw) const
{
	__static_caVehicleLoadType;
	return ConvStrRaw2Trans(caVehicleLoadType, strRaw);
}

CString CCMMvhlItemStdEuroBSNewDlg::ConvVehicleTypeStrTrans2Raw(const CString& strTrans) const
{
	__static_caVehicleLoadType;
	return ConvStrTrans2Raw(caVehicleLoadType, strTrans);
}

#undef __static_caVehicleLoadType

/*
 * 지금 콤보박스를 사용하는 곳이 String으로 비교를 하고 있어
 * 일괄적으로 적용하기에는 리스크가 존재해서..
 * 번역이 필요한 String만 따로 추출해서 변환시켜주는 역할ㅠㅠ
 */
CString CCMMvhlItemStdEuroBSNewDlg::GetVehicleTypeStr() const
{
	CString CurTransStr;
	m_cmbVehicleTypeList.GetWindowText(CurTransStr);

	return ConvVehicleTypeStrTrans2Raw(CurTransStr);
}
