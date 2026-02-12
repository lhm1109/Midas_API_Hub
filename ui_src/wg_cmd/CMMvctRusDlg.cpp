// CMMvctRusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctRusDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvctRusDlg dialog


CCMMvctRusDlg::CCMMvctRusDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvctRusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctRusDlg)
	m_nReaction = 0;
	m_nDisplacement = 0;
	m_nForceMoment = 0;
	m_nLink = 0;
	m_nLoadPointSel = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlMethod.Add(IDC_CMD_EXACT);
	m_aCtrlMethod.Add(IDC_CMD_QUICK);
	m_aCtrlMethod.Add(IDC_CMD_PIVOT);

	m_aCtrlPosFrame.Add(IDC_CMD_FRAME_OPT1);
	m_aCtrlPosFrame.Add(IDC_CMD_FRAME_OPT2);

	m_aCtrlPosPlate.Add(IDC_CMD_PLATE_OPT1);
	m_aCtrlPosPlate.Add(IDC_CMD_PLATE_OPT2);

	m_arReaction.Add(IDC_CMD_MVCT_FILTERS_REACTION_ALL);
	m_arReaction.Add(IDC_CMD_MVCT_FILTERS_REACTION_GROUP);
	m_arReaction.Add(IDC_CMD_MVCT_FILTERS_REACTION_CBO);
	
	m_arDisplacement.Add(IDC_CMD_MVCT_FILTERS_DISP_ALL);
	m_arDisplacement.Add(IDC_CMD_MVCT_FILTERS_DISP_GROUP);
	m_arDisplacement.Add(IDC_CMD_MVCT_FILTERS_DISP_CBO);
	
	m_arForceMoment.Add(IDC_CMD_MVCT_FILTERS_FORCE_ALL);
	m_arForceMoment.Add(IDC_CMD_MVCT_FILTERS_FORCE_GROUP);
	m_arForceMoment.Add(IDC_CMD_MVCT_FILTERS_FORCE_CBO);  

	m_arLink.RemoveAll();
	m_arLink.Add(IDC_CMD_MVCT_FILTERS_LINK_ALL);
	m_arLink.Add(IDC_CMD_MVCT_FILTERS_LINK_GROUP);
	m_arLink.Add(IDC_CMD_MVCT_FILTERS_LINK_CBO);  

	m_aCtrlIGPMethod.Add(IDC_CMD_INFL_GP1_RDO);
	m_aCtrlIGPMethod.Add(IDC_CMD_INFL_GP2_RDO);
	
	m_aAnalysis.Add(IDC_CMD_MVCT_ANALYSIS);
	m_aAnalysis.Add(IDC_CMD_EXACT);
	m_aAnalysis.Add(IDC_CMD_PIVOT);
	m_aAnalysis.Add(IDC_CMD_QUICK);

	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_GRP);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_MATL_STC);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_MATL_CMB);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_BRIDGE_STC);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_BRIDGE_CMB);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_STC);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_DYNA_FACT_EDT);
	
	m_arRussia.Add(IDC_CMD_MVCT_RUS_AK_DYNA_FACT_GRP);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_AK_DYNA_FACT_MATL_STC);
	m_arRussia.Add(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_MATL_CMB);
	m_arRussia.Add(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_BRIDGE_STC);
	m_arRussia.Add(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_BRIDGE_CMB);
	m_arRussia.Add(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_STC);
	m_arRussia.Add(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_EDT);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_MIN_FACTOR_S2_GRP);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_MIN_FACTOR_S2_STC);
	m_arRussia.Add(IDC_CMD_MVCT_RUS_MIN_FACTOR_S2_EDT);

	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_TRAM_OPT_STC);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_MAX_VEHICLE_STC);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_MAX_VEHICLE_EDIT);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_MAX_SPACE_TRAIN_STC);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_MAX_SPACE_TRAIN_EDT);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_MAX_SPACE_TRAIN_UNT);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_INCRE_VEHICLE_STC);
	m_arRussia2_TramcarControl.Add(IDC_CMD_MVCT_INCRE_VEHICLE_EDT);

	m_arLineOption.Add(IDC_CMD_MVCT_OPTION);
	m_arLineOption.Add(IDC_CMD_MVCT_OPTION_STATIC);
	m_arLineOption.Add(IDC_CMD_MVCT_OPTION_EDIT);
}


void CCMMvctRusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctRusDlg)
	DDX_Control(pDX, IDC_CMD_INFL_DIST_EDT, m_edtIGPDist);
	DDX_Control(pDX, IDC_CMD_INFL_DIST_UNT, m_untIGPDist);
	DDX_Control(pDX, IDC_CMD_IGPN_EDIT, m_spinGPN);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE_CBO, m_cboForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP_CBO, m_cboDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION_CBO, m_cboReaction);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_LINK_CBO, m_cboLink);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE, m_chkForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP, m_chkDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION, m_chkReaction);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_LINK, m_chkLink);
	DDX_Control(pDX, IDC_CMD_PLATE_STRESS_CALC, m_chkStressCalc);
	DDX_Control(pDX, IDC_CMD_PLATE_CONCURRENT_CALC_CHK, m_chkConcurrentCalc);
	DDX_Control(pDX, IDC_CMD_FRAME_STRESS_CALC, m_chkStressCalcFrame);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_REACTION_ALL, m_nReaction);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_DISP_ALL, m_nDisplacement);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_FORCE_ALL, m_nForceMoment);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_LINK_ALL, m_nLink);
	DDX_Radio(pDX, IDC_CMD_InfPoint, m_nLoadPointSel);
	DDX_Control(pDX, IDC_CMD_MVCT_RUS_DYNA_FACT_MATL_CMB, m_cmbMatlType);
	DDX_Control(pDX, IDC_CMD_MVCT_RUS_DYNA_FACT_BRIDGE_CMB, m_cmbBridgeType);
	DDX_Control(pDX, IDC_CMD_MVCT_RUS_DYNA_FACT_EDT, m_edtDynaFact);
	DDX_Control(pDX, IDC_CMD_MVCT_MAX_VEHICLE_EDIT, m_spinMaxVehicle);
	DDX_Control(pDX, IDC_CMD_MVCT_MAX_SPACE_TRAIN_EDT, m_edtMaxSpacing);
	DDX_Control(pDX, IDC_CMD_MVCT_MAX_SPACE_TRAIN_UNT, m_untMaxSpacing);
	DDX_Control(pDX, IDC_CMD_MVCT_INCRE_VEHICLE_EDT  , m_spinIncrementVehicle);
	DDX_Control(pDX, IDC_CMD_MVCT_AK_RUS_DYNA_FACT_MATL_CMB, m_cmbMatlType_AK);
	DDX_Control(pDX, IDC_CMD_MVCT_AK_RUS_DYNA_FACT_BRIDGE_CMB, m_cmbBridgeType_AK);
	DDX_Control(pDX, IDC_CMD_MVCT_AK_RUS_DYNA_FACT_EDT, m_edtDynaFact_AK);
	DDX_Control(pDX, IDC_CMD_MVCT_RUS_MIN_FACTOR_S2_EDT, m_edtMinFactorS2);
	
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS,        m_chkFilter);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------
// Implementation
void CCMMvctRusDlg::Data2Dlg()
{
	T_GRUP_D GroupData;
	T_BNGR_D BngrData;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlMethod, m_Data.nCalculationMethod-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosPlate, m_Data.nCalcPositionPlate-1);

	// Added by BugBoy 2000.11.11 for Load Point Selection
	m_nLoadPointSel = m_Data.nLoadPointSelection-1;
	
	//CString strTemp;
	//strTemp.Format(_T("%d"), m_Data.nNumUnitLoad);
	//m_edtLineOption.SetWindowText(strTemp);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIGPMethod, m_Data.nInflGenPoint);
	m_spinGPN.SetValue(m_Data.nNumUnitLoad);
	m_edtIGPDist.SetEditUnit(m_Data.dDistance);
	
	m_chkStressCalc.SetCheck(m_Data.bStressCalc);
	m_chkConcurrentCalc.SetCheck(m_Data.bConcurrentCalc);
	m_chkStressCalcFrame.SetCheck(m_Data.bStressCalcFrame);

	m_chkReaction.SetCheck(m_Data.bReaction);
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, m_Data.bReaction == 1);	
	m_nReaction = m_Data.nReacFilter;
	if(m_nReaction)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ReacGrup, GroupData);
		m_cboReaction.SelectString(-1,GroupData.GroupName);
	}
	else
		m_cboReaction.EnableWindow(FALSE);

	m_chkDisplacement.SetCheck(m_Data.bDisplacement);
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, m_Data.bDisplacement == 1);	
	m_nDisplacement = m_Data.nDispFilter;
	if(m_nDisplacement)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.DispGrup, GroupData);
		m_cboDisplacement.SelectString(-1,GroupData.GroupName);
	}
	else
		m_cboDisplacement.EnableWindow(FALSE);


	m_chkForceMoment.SetCheck(m_Data.bForce);
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, m_Data.bForce == 1);	
	m_nForceMoment = m_Data.nForcFilter;
	if(m_nForceMoment)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ForcGrup, GroupData);
		m_cboForceMoment.SelectString(-1,GroupData.GroupName);
	}
	else
		m_cboForceMoment.EnableWindow(FALSE);

	m_chkLink.SetCheck(m_Data.bLink);
	CDlgUtil::CtrlEnableDisable(this, m_arLink, m_Data.bLink == 1);	
	m_nLink = m_Data.nLinkFilter;
	if(m_nLink)
	{
		m_pDoc->m_pAttrCtrl->GetBngr(m_Data.LinkGrup, BngrData);
		m_cboLink.SelectString(-1,BngrData.GroupName);
	}
	else
		m_cboLink.EnableWindow(FALSE);

	m_cmbMatlType.SetCurSel(m_Data.nMatType);
	m_cmbBridgeType.SetCurSel(m_Data.nBridgeType);

	OnChangeIGPMethod();

	m_spinMaxVehicle.SetValue(m_Data.nMaxSuccessiveVehicle);
	m_edtMaxSpacing.SetEditUnit(m_Data.dMaxSpacingTrain);
	m_spinIncrementVehicle.SetValue(m_Data.nIncrementVehicle);

	m_cmbMatlType_AK.SetCurSel(m_Data.nMatType_AK);
	m_cmbBridgeType_AK.SetCurSel(m_Data.nBridgeType_AK);
	m_edtMinFactorS2.SetEditUnit(m_Data.dMinFactors2);

	UpdateData(FALSE);
}

BOOL CCMMvctRusDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlMethod, m_Data.nCalculationMethod);
	m_Data.nCalculationMethod += 1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame);
	m_Data.nCalcPositionFrame += 1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosPlate, m_Data.nCalcPositionPlate);
	m_Data.nCalcPositionPlate += 1;

	// Added by BugBoy 2000.11.11 for Load Point Selection
	m_Data.nLoadPointSelection = m_nLoadPointSel+1;
	
	CString strTemp;
	//m_edtLineOption.GetWindowText(strTemp);
	//m_Data.nNumUnitLoad = _ttoi(strTemp);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGPMethod, m_Data.nInflGenPoint);
	if (m_Data.nInflGenPoint == 0)
		m_Data.nNumUnitLoad = m_spinGPN.GetEditValueInt();
	else m_Data.dDistance = m_edtIGPDist.GetEditValue();

	m_Data.bStressCalc = m_chkStressCalc.GetCheck();
	m_Data.bConcurrentCalc = m_chkConcurrentCalc.GetCheck();
	m_Data.bStressCalcFrame = m_chkStressCalcFrame.GetCheck();
	
	m_Data.bReaction = m_chkReaction.GetCheck();
	if(m_Data.bReaction)
	{
		m_Data.nReacFilter = m_nReaction;
		if(m_nReaction)
		{
			if(m_cboReaction.GetCurSel() == -1)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Group_Name_));
				return FALSE;
			}
			m_cboReaction.GetLBText(m_cboReaction.GetCurSel(), strTemp);
			m_Data.ReacGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
		}
	}

	m_Data.bDisplacement = m_chkDisplacement.GetCheck();
	if(m_Data.bDisplacement)
	{
		m_Data.nDispFilter = m_nDisplacement;
		if(m_nDisplacement)
		{
			if(m_cboDisplacement.GetCurSel() == -1)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Group_Name_));
				return FALSE;
			}
			m_cboDisplacement.GetLBText(m_cboDisplacement.GetCurSel(), strTemp);
			m_Data.DispGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
		}
	}

	m_Data.bForce = m_chkForceMoment.GetCheck();
	if(m_Data.bForce)
	{
		m_Data.nForcFilter = m_nForceMoment;
		if(m_nForceMoment)
		{
			if(m_cboForceMoment.GetCurSel() == -1)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Group_Name_));
				return FALSE;
			}
			m_cboForceMoment.GetLBText(m_cboForceMoment.GetCurSel(), strTemp);
			m_Data.ForcGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
		}	
	}

	m_Data.bLink = m_chkLink.GetCheck();
	if(m_Data.bLink)
	{
		m_Data.nLinkFilter = m_nLink;
		if(m_nLink)
		{
			if(m_cboLink.GetCurSel() == -1)
			{
				GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Group_Name_));
				return FALSE;
			}
			m_cboLink.GetLBText(m_cboLink.GetCurSel(), strTemp);
			m_Data.LinkGrup = m_pDoc->m_pAttrCtrl->GetBngrKey(strTemp);
		}	
	}

	m_Data.nMatType = m_cmbMatlType.GetCurSel();
	m_Data.nBridgeType = m_cmbBridgeType.GetCurSel();
	
	m_Data.nMaxSuccessiveVehicle = m_spinMaxVehicle.GetEditValueInt();
	m_Data.dMaxSpacingTrain = m_edtMaxSpacing.GetEditValue();
	m_Data.nIncrementVehicle = m_spinIncrementVehicle.GetEditValueInt();

	m_Data.nMatType_AK = m_cmbMatlType_AK.GetCurSel();
	m_Data.nBridgeType_AK = m_cmbBridgeType_AK.GetCurSel();
	m_Data.dMinFactors2 = m_edtMinFactorS2.GetEditValue();

	return TRUE;
}

void CCMMvctRusDlg::InitGroupData() 
{
	CArray<T_GRUP_K, T_GRUP_K> GroupKeyList;
	T_GRUP_D GroupData;	

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(GroupKeyList);

	for(int i=0; i<GroupKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(GroupKeyList.GetAt(i), GroupData);
		m_cboReaction.AddString(GroupData.GroupName);
		m_cboDisplacement.AddString(GroupData.GroupName);
		m_cboForceMoment.AddString(GroupData.GroupName);
	}

	CArray<T_BNGR_K, T_BNGR_K> BngrKeyList;
	T_BNGR_D BngrData;	

	m_pDoc->m_pAttrCtrl->GetBngrKeyList(BngrKeyList);

	for(int i=0; i<BngrKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetBngr(BngrKeyList.GetAt(i), BngrData);
		m_cboLink.AddString(BngrData.GroupName);
	}
}

void CCMMvctRusDlg::InitComboBox()
{
	m_cmbMatlType.ResetContent();
	m_cmbMatlType.AddString(_LS(IDS_CMD_MVCT_MATL_STL));
	m_cmbMatlType.AddString(_LS(IDS_CMD_MVCT_MATL_RC));
	m_cmbMatlType.SetCurSel(0);

	if(m_Data.nMatType == 0)
	{
		m_cmbBridgeType.ResetContent();
		m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1));
		m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2));
		m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3));
		//m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4));
		//m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5));
		m_cmbBridgeType.SetCurSel(0);
	}
	else
	{
		m_cmbBridgeType.ResetContent();
		m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1));
		m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2));
		//m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3));
		m_cmbBridgeType.SetCurSel(0);
	}

	m_cmbMatlType_AK.ResetContent();
	m_cmbMatlType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_STL));
	m_cmbMatlType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_RC));
	m_cmbMatlType_AK.SetCurSel(0);

	if(m_Data.nMatType_AK == 0)
	{
		m_cmbBridgeType_AK.ResetContent();
		m_cmbBridgeType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1));
		m_cmbBridgeType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2));
		m_cmbBridgeType_AK.SetCurSel(0);
	}
	else
	{
		m_cmbBridgeType_AK.ResetContent();
		m_cmbBridgeType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1));
		m_cmbBridgeType_AK.SetCurSel(0);
	}
}

void CCMMvctRusDlg::AlignControl()
{
	CArray<UINT,UINT> arFilters;
	CArray<UINT,UINT> arMove;
	CArray<UINT,UINT> arMove2;
	CArray<UINT,UINT> arOKCancel;

	arMove.Add(IDC_CMD_MVCT_LOAD_POINT);
	arMove.Add(IDC_CMD_InfPoint);
	arMove.Add(IDC_CMD_AllPoint);
	arMove.Add(IDC_CMD_MVCT_INFLUENCE_GEN);
	arMove.Add(IDC_CMD_INFL_GP1_RDO);
	arMove.Add(IDC_CMD_IGPN_EDIT);
	arMove.Add(IDC_CMD_INFL_GP2_RDO);
	arMove.Add(IDC_CMD_INFL_DIST_EDT);
	arMove.Add(IDC_CMD_INFL_DIST_UNT);
	arMove.Add(IDC_CMD_MVCT_ANALYSIS_RESULTS);
	arMove.Add(IDC_CMD_MVCT_PLATE);
	arMove.Add(IDC_CMD_PLATE_OPT1);
	arMove.Add(IDC_CMD_PLATE_OPT2);
	arMove.Add(IDC_CMD_PLATE_STRESS_CALC);
	arMove.Add(IDC_CMD_PLATE_CONCURRENT_CALC_CHK);
	arMove.Add(IDC_CMD_MVCT_FRAME);
	arMove.Add(IDC_CMD_FRAME_OPT1);
	arMove.Add(IDC_CMD_FRAME_OPT2);
	arMove.Add(IDC_CMD_FRAME_STRESS_CALC);
	arMove.Add(IDC_CMD_MVCT_OPTION);
	arMove.Add(IDC_CMD_MVCT_OPTION_STATIC);
	arMove.Add(IDC_CMD_MVCT_OPTION_EDIT);
	arMove.Add(IDC_CMD_MVCT_FILTERS);
	arMove.Add(IDC_CMD_MVCT_FILTERS_REACTION);
	arMove.Add(IDC_CMD_MVCT_FILTERS_REACTION_ALL);
	arMove.Add(IDC_CMD_MVCT_FILTERS_REACTION_GROUP);
	arMove.Add(IDC_CMD_MVCT_FILTERS_REACTION_CBO);
	arMove.Add(IDC_CMD_MVCT_FILTERS_DISP);
	arMove.Add(IDC_CMD_MVCT_FILTERS_DISP_ALL);
	arMove.Add(IDC_CMD_MVCT_FILTERS_DISP_GROUP);
	arMove.Add(IDC_CMD_MVCT_FILTERS_DISP_CBO);
	arMove.Add(IDC_CMD_MVCT_FILTERS_FORCE);
	arMove.Add(IDC_CMD_MVCT_FILTERS_FORCE_ALL);
	arMove.Add(IDC_CMD_MVCT_FILTERS_FORCE_GROUP);
	arMove.Add(IDC_CMD_MVCT_FILTERS_FORCE_CBO);
	arMove.Add(IDC_CMD_MVCT_FILTERS_LINK);
	arMove.Add(IDC_CMD_MVCT_FILTERS_LINK_ALL);
	arMove.Add(IDC_CMD_MVCT_FILTERS_LINK_GROUP);
	arMove.Add(IDC_CMD_MVCT_FILTERS_LINK_CBO);  
	
	arFilters.Add(IDC_CMD_MVCT_FILTERS);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_REACTION);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_REACTION_ALL);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_REACTION_GROUP);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_REACTION_CBO);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_DISP);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_DISP_ALL);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_DISP_GROUP);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_DISP_CBO);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_FORCE);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_FORCE_ALL);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_FORCE_GROUP);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_FORCE_CBO);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_LINK);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_LINK_ALL);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_LINK_GROUP);
	arFilters.Add(IDC_CMD_MVCT_FILTERS_LINK_CBO);  
	arFilters.Add(IDC_CMD_MVCT_OK);
	arFilters.Add(IDC_CMD_MVCT_CANCEL);

	arMove2.Add(IDC_CMD_MVCT_TRUCK_TRAIN);

	arOKCancel.Add(IDC_CMD_MVCT_OK);
	arOKCancel.Add(IDC_CMD_MVCT_CANCEL);
		
	CRect rectTarget;
	CRect rect;
	CRect rect2;
	CRect rectShort;

	T_MVCD_D MvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();

	int nMoveDistY = 0;
	int nDistY = 0;
	
	((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_RUS_DYNA_FACT_GRP))->GetWindowRect(rect);
	((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_RUS_MIN_FACTOR_S2_GRP))->GetWindowRect(rect2);
	if(MvcdData.nCodeType != D_MOVE_CODE_RUSSIA) 
	{
		nMoveDistY = rect.top - rect2.bottom - globalUtils.ScaleByDPI(8);
		CDlgUtil::CtrlShowHide(this, m_arRussia, FALSE);

		CDlgUtil::CtrlMoveDistY(this, arFilters, nMoveDistY);
		CDlgUtil::CtrlMoveDistY(this, arMove, nMoveDistY);
		CDlgUtil::CtrlMoveDistY(this, arMove2, nMoveDistY);
		CDlgUtil::CtrlMoveDistY(this, m_aAnalysis, nMoveDistY);

		CDlgUtil::CtrlShowHide(this, m_arRussia2_TramcarControl, FALSE);
	}
	
	if(MvcdData.nCodeType == D_MOVE_CODE_EURO_BS || MvcdData.nCodeType == D_MOVE_CODE_RUSSIA)
	{
	  ((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_ANALYSIS))->GetWindowRect(rectTarget);
	  ((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_LOAD_POINT))->GetWindowRect(rect);
		((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_TRUCK_TRAIN))->GetWindowRect(rectShort);

	  nDistY = rectTarget.top - rect.top;

	  CDlgUtil::CtrlShowHide(this, m_aAnalysis, FALSE);
	  CDlgUtil::CtrlMoveDistY(this, arMove, nDistY);
		CDlgUtil::CtrlMoveDistY(this, m_arRussia2_TramcarControl, nDistY);
		rectShort.bottom += nDistY;
		ScreenToClient(rectShort);
		GetDlgItem(IDC_CMD_MVCT_TRUCK_TRAIN)->MoveWindow(rectShort);
	}

	((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_OPTION))->GetWindowRect(rectTarget);
	((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_FILTERS))->GetWindowRect(rect);

	nDistY = rectTarget.top - rect.top;

	CDlgUtil::CtrlShowHide(this, m_arLineOption, FALSE);
	CDlgUtil::CtrlMoveDistY(this, arFilters, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_arRussia2_TramcarControl, nDistY);

	// OK, Cancel 버튼 옮긴다.

	if(MvcdData.nCodeType != D_MOVE_CODE_RUSSIA)
	  ((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_FILTERS))->GetWindowRect(rectTarget);
	else
		((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_TRAM_OPT_STC))->GetWindowRect(rectTarget);

	((CBCGPStatic*)GetDlgItem(IDC_CMD_MVCT_OK))->GetWindowRect(rect);

	nDistY = rectTarget.bottom - rect.top + globalUtils.ScaleByDPI(16);

	CDlgUtil::CtrlMoveDistY(this, arOKCancel, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_CMD_MVCT_OK);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+ globalUtils.ScaleByDPI(8);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);  
}

void CCMMvctRusDlg::ResetDynamicFactorData(BOOL bMatl)
{
	UpdateData(TRUE);

	int nMatlType = m_cmbMatlType.GetCurSel();

	if(bMatl)
	{
		if(nMatlType == 0)
		{
			m_cmbBridgeType.ResetContent();
			m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1));
			m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2));
			m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3));
			//m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4));
			//m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5));
			m_cmbBridgeType.SetCurSel(0);
		}
		else
		{
			m_cmbBridgeType.ResetContent();
			m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1));
			m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2));
			//m_cmbBridgeType.AddString(_LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3));
			m_cmbBridgeType.SetCurSel(0);
		}
	}

	int nBridgeType = m_cmbBridgeType.GetCurSel();

	CString strEdtName = _T("");
	
	if(nMatlType == 0)
	{
		if     (nBridgeType == 0) strEdtName = _LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_1_LAMBDA);
		else if(nBridgeType == 1) strEdtName = _LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_2_LAMBDA);
		else if(nBridgeType == 2) strEdtName = _LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_3_LAMBDA);
		else if(nBridgeType == 3) strEdtName = _LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_4_LAMBDA);
		else if(nBridgeType == 4) strEdtName = _LS(IDS_CMD_MVCT_MATL_STL_BRIDGE_5_LAMBDA);
		else    ASSERT(0);
	}
	else 
	{
		if     (nBridgeType == 0) strEdtName = _LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_1_LAMBDA);
		else if(nBridgeType == 1) strEdtName = _LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_2_LAMBDA);
		else if(nBridgeType == 2) strEdtName = _LS(IDS_CMD_MVCT_MATL_RC_BRIDGE_3_LAMBDA);
		else    ASSERT(0);
	}

	GetDlgItem(IDC_CMD_MVCT_RUS_DYNA_FACT_EDT)->SetWindowText(strEdtName);
}

void CCMMvctRusDlg::ResetAKDynamicFactorData(BOOL bMatl)
{
	UpdateData(TRUE);
	
	int nMatlType = m_cmbMatlType_AK.GetCurSel();
	
	if(bMatl)
	{
		if(nMatlType == 0)
		{
			m_cmbBridgeType_AK.ResetContent();
			m_cmbBridgeType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1));
			m_cmbBridgeType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2));
			m_cmbBridgeType_AK.SetCurSel(0);
		}
		else
		{
			m_cmbBridgeType_AK.ResetContent();
			m_cmbBridgeType_AK.AddString(_LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1));
			m_cmbBridgeType_AK.SetCurSel(0);
		}
	}
	
	int nBridgeType = m_cmbBridgeType_AK.GetCurSel();
	
	CString strEdtName = _T("");
	
	if(nMatlType == 0)
	{
		if     (nBridgeType == 0) strEdtName = _LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_1_LAMBDA);
		else if(nBridgeType == 1) strEdtName = _LS(IDS_CMD_MVCT_AK_MATL_STL_BRIDGE_2_LAMBDA);
		else    ASSERT(0);
	}
	else 
	{
		if     (nBridgeType == 0) strEdtName = _LS(IDS_CMD_MVCT_AK_MATL_RC_BRIDGE_1_LAMBDA);
		else    ASSERT(0);
	}

	GetDlgItem(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_EDT)->SetWindowText(strEdtName);
}

BEGIN_MESSAGE_MAP(CCMMvctRusDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvctRusDlg)
	ON_BN_CLICKED(IDC_CMD_INFL_GP1_RDO, OnChangeIGPMethod)
	ON_BN_CLICKED(IDC_CMD_INFL_GP2_RDO, OnChangeIGPMethod)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION_ALL, OnCmdMvctFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP_ALL, OnCmdMvctFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE_ALL, OnCmdMvctFiltersForce)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_LINK_ALL, OnCmdMvctFiltersLink)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_CMD_MVCT_OPTION_SPIN, OnDeltaposCmdMvctOptionSpin)
	ON_BN_CLICKED(IDC_CMD_MVCT_OK, OnCmdMvctOk)
	ON_BN_CLICKED(IDC_CMD_MVCT_CANCEL, OnCmdMvctCancel)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION, OnCmdMvctFiltersReactions)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP, OnCmdMvctFiltersDisplacements)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION_GROUP, OnCmdMvctFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP_GROUP, OnCmdMvctFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE_GROUP, OnCmdMvctFiltersForce)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE, OnCmdMvctFiltersForceMoments)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_LINK_GROUP, OnCmdMvctFiltersLink)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_LINK, OnCmdMvctFiltersLinks)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_RUS_DYNA_FACT_MATL_CMB, OnCmbMvctMatlTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_RUS_DYNA_FACT_BRIDGE_CMB, OnCmbMvctBridgeTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_MATL_CMB, OnCmbMvctAKMatlTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_AK_RUS_DYNA_FACT_BRIDGE_CMB, OnCmbMvctAKBridgeTypeCmb)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvctRusDlg message handlers

BOOL CCMMvctRusDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	AlignControl(); //나중에 지우면 Line Option 가능함.
	CDialogMove::OnInitDialog();

	m_chkFilter.InitControl(this, CCMMvctRusDlg::IDD, IDC_CMD_MVCT_FILTERS,TRUE,TRUE);
	
	// TODO: Add extra initialization here

	T_MVCD_D DataMvcd;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();

	if (!m_pDoc->m_pAttrCtrl->GetMvct(m_Data))
	{
		m_Data.Initialize();    
		// 맞춘다. Mvct를 넣지 않고, 해석하면 대화상자하고 짝이 맞지 않는다. by jbseon : 05.04.14
		// with myshin 협의
		//if(DataMvcd.nCodeType == D_MOVE_CODE_CANADA) m_Data.nLoadPointSelection = 1;

		// dDistance는 초기화 해도 된다.
		// Mvct 안 넣은 경우 Default는 Number/Line Element 옵션이기 때문. 아래 값 사용 안됨
		m_Data.dDistance = M_InitValueCurUnit(0.3, TON, M, D_UNITSYS_BASE_LENGTH);
		m_Data.dMaxSpacingTrain = M_InitValueCurUnit(8.5, N, M, D_UNITSYS_BASE_LENGTH);

		// MQC 16504 
		if(DataMvcd.nCodeType == D_MOVE_CODE_EURO_BS) 
		{
			m_Data.nCalcPositionFrame = 2;
		}
	}

	// 호주 이동하중 All Point 선택 불가능하도록 처리 20130704 jhyun 
	// 이부분 풀때는 AnalCtrl 도 같이 수정 해야한다~
	// MQC 9205
	if(DataMvcd.nCodeType == D_MOVE_CODE_AUSTRALIA) 
	{
		m_Data.nCalculationMethod = 1;
		m_Data.nLoadPointSelection = 1;
		GetDlgItem(IDC_CMD_AllPoint)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_QUICK)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_PIVOT)->EnableWindow(FALSE);
	}
	
	// MNET:XXXX-HSSHIM-20121023 : 캐나다 이동하중에서 Pivot과 Quick 방법을 모두 사용가능 하도록 열어줌. (박정웅 차장님 요청. v810부터)
//   if(DataMvcd.nCodeType == D_MOVE_CODE_CANADA)
//   {
// //    CWnd* pWnd1 = GetDlgItem(IDC_CMD_QUICK);
// //    pWnd1->EnableWindow(FALSE);
// //    CWnd* pWnd2 = GetDlgItem(IDC_CMD_PIVOT);
// //    pWnd2->EnableWindow(FALSE);
//     GetDlgItem(IDC_CMD_QUICK)->EnableWindow(FALSE);
//     GetDlgItem(IDC_CMD_PIVOT)->EnableWindow(FALSE);
//   }
	m_spinGPN.SetRange(2, 10);
	m_spinGPN.SetInteger(TRUE);

	m_edtIGPDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIGPDist.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtMaxSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untMaxSpacing.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_spinMaxVehicle.SetRange(1, 10000);
	m_spinMaxVehicle.SetInteger(TRUE);

	m_spinIncrementVehicle.SetRange(1, 10000);
	m_spinIncrementVehicle.SetInteger(TRUE);

	InitGroupData();
	InitComboBox();
	Data2Dlg();
	ResetDynamicFactorData(FALSE);
	ResetAKDynamicFactorData(FALSE);

	if(DataMvcd.nCodeType == D_MOVE_CODE_RUSSIA) 
	{
		GetDlgItem(IDC_CMD_PLATE_CONCURRENT_CALC_CHK)->ShowWindow(SW_HIDE);
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctRusDlg::OnCmdMvctOk() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	if (!m_pDoc->m_pDataCtrl->AddMvct(m_Data)) return;
	
	CDialogMove::OnOK();
}

void CCMMvctRusDlg::OnCmdMvctCancel() 
{
	CDialogMove::OnCancel();	
}

/*
void CCMMvctRusDlg::OnDeltaposCmdMvctOptionSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	CString strTemp;
	m_edtLineOption.GetWindowText(strTemp);	

	if(pNMUpDown->iDelta < 0)
	{
		if(_ttoi(strTemp) < 10)
			strTemp.Format(_T("%d"), _ttoi(strTemp) + 1);
		m_edtLineOption.SetWindowText(strTemp);
	}
	else
	{
		if(_ttoi(strTemp) > 2)
			strTemp.Format(_T("%d"), _ttoi(strTemp) - 1);
		m_edtLineOption.SetWindowText(strTemp);
	}

	*pResult = 0;
}
*/

void CCMMvctRusDlg::OnCmdMvctFiltersReaction() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_REACTION_CBO))->EnableWindow(m_nReaction == 1);
}

void CCMMvctRusDlg::OnCmdMvctFiltersDisplacement() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_DISP_CBO))->EnableWindow(m_nDisplacement == 1);
}

void CCMMvctRusDlg::OnCmdMvctFiltersForce() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_FORCE_CBO))->EnableWindow(m_nForceMoment == 1);
}

void CCMMvctRusDlg::OnCmdMvctFiltersLink() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_LINK_CBO))->EnableWindow(m_nLink == 1);
}

void CCMMvctRusDlg::OnCmdMvctFiltersReactions() 
{
	int nCheck = m_chkReaction.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersReaction();
}

void CCMMvctRusDlg::OnCmdMvctFiltersDisplacements() 
{
	int nCheck = m_chkDisplacement.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersDisplacement();
}

void CCMMvctRusDlg::OnCmdMvctFiltersForceMoments() 
{
	int nCheck = m_chkForceMoment.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersForce(); 
}

void CCMMvctRusDlg::OnCmdMvctFiltersLinks() 
{
	int nCheck = m_chkLink.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arLink, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersLink(); 
}

void CCMMvctRusDlg::OnChangeIGPMethod() 
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGPMethod, nCheck);

	m_spinGPN.EnableWindow(nCheck == 0);
	m_edtIGPDist.EnableWindow(nCheck != 0);
	m_untIGPDist.EnableWindow(nCheck != 0);
}

void CCMMvctRusDlg::OnCmbMvctMatlTypeCmb() 
{
	ResetDynamicFactorData(TRUE);
}

void CCMMvctRusDlg::OnCmbMvctBridgeTypeCmb() 
{
	ResetDynamicFactorData(FALSE);
}

void CCMMvctRusDlg::OnCmbMvctAKMatlTypeCmb() 
{
	ResetAKDynamicFactorData(TRUE);
}

void CCMMvctRusDlg::OnCmbMvctAKBridgeTypeCmb() 
{
	ResetAKDynamicFactorData(FALSE);
}

LRESULT CCMMvctRusDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	T_MVCD_D MvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();

	if(MvcdData.nCodeType != D_MOVE_CODE_RUSSIA) 
	{
		CDlgUtil::CtrlShowHide(this, m_arRussia, FALSE);
		CDlgUtil::CtrlShowHide(this, m_arRussia2_TramcarControl, FALSE);
	}
	
	if(MvcdData.nCodeType == D_MOVE_CODE_EURO_BS || MvcdData.nCodeType == D_MOVE_CODE_RUSSIA)
	{		
		CDlgUtil::CtrlShowHide(this, m_aAnalysis, FALSE);
	}
	
	CDlgUtil::CtrlShowHide(this, m_arLineOption, FALSE);

	return 0L;
}