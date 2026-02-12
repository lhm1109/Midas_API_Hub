// CMMvctIDDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctIDDlg.h"

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
// CCMMvctIDDlg dialog
CCMMvctIDDlg::CCMMvctIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvctIDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctIDDlg)
	m_nReaction = 0;
	m_nDisplacement = 0;
	m_nForceMoment = 0;
	m_nLink = 0;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
	
	m_aCtrlPosFrame.RemoveAll();
	m_aCtrlPosFrame.Add(IDC_CMD_FRAME_OPT1);
	m_aCtrlPosFrame.Add(IDC_CMD_FRAME_OPT2);

	m_aCtrlPosPlate.RemoveAll();
	m_aCtrlPosPlate.Add(IDC_CMD_PLATE_OPT1);
	m_aCtrlPosPlate.Add(IDC_CMD_PLATE_OPT2);

	m_arReaction.RemoveAll();
	m_arReaction.Add(IDC_CMD_MVCT_FILTERS_REACTION_ALL);
	m_arReaction.Add(IDC_CMD_MVCT_FILTERS_REACTION_GROUP);
	m_arReaction.Add(IDC_CMD_MVCT_FILTERS_REACTION_CBO);
	
	m_arDisplacement.RemoveAll();
	m_arDisplacement.Add(IDC_CMD_MVCT_FILTERS_DISP_ALL);
	m_arDisplacement.Add(IDC_CMD_MVCT_FILTERS_DISP_GROUP);
	m_arDisplacement.Add(IDC_CMD_MVCT_FILTERS_DISP_CBO);
	
	m_arForceMoment.RemoveAll();
	m_arForceMoment.Add(IDC_CMD_MVCT_FILTERS_FORCE_ALL);
	m_arForceMoment.Add(IDC_CMD_MVCT_FILTERS_FORCE_GROUP);
	m_arForceMoment.Add(IDC_CMD_MVCT_FILTERS_FORCE_CBO);

	m_arLink.RemoveAll();
	m_arLink.Add(IDC_CMD_MVCT_FILTERS_LINK_ALL);
	m_arLink.Add(IDC_CMD_MVCT_FILTERS_LINK_GROUP);
	m_arLink.Add(IDC_CMD_MVCT_FILTERS_LINK_CBO);  

	m_aCtrlBrgType.RemoveAll();
	m_aCtrlBrgType.Add(IDC_CMD_MVCT_BTYPE_STEEL);
	m_aCtrlBrgType.Add(IDC_CMD_MVCT_BTYPE_RC);

	m_aCtrlIGPMethod.RemoveAll();
	m_aCtrlIGPMethod.Add(IDC_CMD_INFL_GP1_RDO);
	m_aCtrlIGPMethod.Add(IDC_CMD_INFL_GP2_RDO);
}

void CCMMvctIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctIDDlg)
	DDX_Control(pDX, IDC_CMD_INFL_DIST_EDT, m_edtIGPDist);
	DDX_Control(pDX, IDC_CMD_INFL_DIST_UNT, m_untIGPDist);
	DDX_Control(pDX, IDC_CMD_MVCT_MAX_VEHICLE_EDIT, m_spinMaxVehicle);
	DDX_Control(pDX, IDC_CMD_IGPN_EDIT, m_spinIGPN);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE_CBO, m_cboForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP_CBO, m_cboDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION_CBO, m_cboReaction);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_LINK_CBO, m_cboLink);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE, m_chkForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP, m_chkDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION, m_chkReaction);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_LINK, m_chkLink);
	DDX_Control(pDX, IDC_CMD_PLATE_STRESS_CALC, m_chkStressCalc);
	DDX_Control(pDX, IDC_CMD_FRAME_STRESS_CALC, m_chkStressCalcFrame);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_REACTION_ALL, m_nReaction);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_DISP_ALL, m_nDisplacement);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_FORCE_ALL, m_nForceMoment);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_LINK_ALL, m_nLink);
	DDX_Control(pDX, IDC_CMD_MVCT_RAIL_TRACKS_CMB,                 m_cmbTracks);
	DDX_Control(pDX, IDC_CMD_MVCT_RAIL_LONGI_DISPERSION_WIDTH_CMB, m_cmbWidth);
	DDX_Control(pDX, IDC_CMD_MVCT_RAIL_LONGI_DISPERSION_WIDTH_EDT, m_edtWidth);
	DDX_Control(pDX, IDC_CMD_MVCT_RAIL_LONGI_DISPERSION_WIDTH_UNT, m_untWidth);
	DDX_Control(pDX, IDC_CMD_MVCT_RAIL_DEPTH_EDT,                  m_edtDepth);
	DDX_Control(pDX, IDC_CMD_MVCT_RAIL_DEPTH_UNT,                  m_untDepth);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------
// Implementation
void CCMMvctIDDlg::Data2Dlg()
{
	T_GRUP_D GroupData;
	T_BNGR_D BngrData;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIGPMethod, m_Data.nInflGenPoint);
	m_spinIGPN.SetValue(m_Data.nUnitNum);
	m_edtIGPDist.SetEditUnit(m_Data.dDistance);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosPlate, m_Data.nCalcPositionPlate-1);

	m_chkStressCalc.SetCheck(m_Data.bStressCalcPlate);
	m_chkStressCalcFrame.SetCheck(m_Data.bStressCalcFrame);

	m_chkReaction.SetCheck(m_Data.bReaction);
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, m_Data.bReaction == 1);	
	m_nReaction = m_Data.nReacFilter;
	if(m_nReaction)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ReacGrup, GroupData);
		m_cboReaction.SelectString(-1, GroupData.GroupName);
	}
	else
		m_cboReaction.EnableWindow(FALSE);

	m_chkDisplacement.SetCheck(m_Data.bDisplacement);
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, m_Data.bDisplacement == 1);	
	m_nDisplacement = m_Data.nDispFilter;
	if(m_nDisplacement)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.DispGrup, GroupData);
		m_cboDisplacement.SelectString(-1, GroupData.GroupName);
	}
	else
		m_cboDisplacement.EnableWindow(FALSE);

	m_chkForceMoment.SetCheck(m_Data.bForce);
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, m_Data.bForce == 1);	
	m_nForceMoment = m_Data.nForcFilter;
	if(m_nForceMoment)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ForcGrup, GroupData);
		m_cboForceMoment.SelectString(-1, GroupData.GroupName);
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

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlBrgType, m_Data.nBridgeType);
	m_spinMaxVehicle.SetValue(m_Data.nMaxSuccessiveVehicle);
	
	m_cmbTracks.SetCurSel(m_Data.nTracks);
	m_cmbWidth.SetCurSel(m_Data.nSleeperWidth);
	m_edtWidth.SetEditUnit(m_Data.dSleeperWidth);
	m_edtDepth.SetEditUnit(m_Data.dDepthFill);

	UpdateData(FALSE);
}

BOOL CCMMvctIDDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGPMethod, m_Data.nInflGenPoint);
	if (m_Data.nInflGenPoint == 0)
		m_Data.nUnitNum = m_spinIGPN.GetEditValueInt();
	else m_Data.dDistance = m_edtIGPDist.GetEditValue();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame);
	m_Data.nCalcPositionFrame += 1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosPlate, m_Data.nCalcPositionPlate);
	m_Data.nCalcPositionPlate += 1;
	
	m_Data.bStressCalcPlate = m_chkStressCalc.GetCheck();
	m_Data.bStressCalcFrame = m_chkStressCalcFrame.GetCheck();
	
	CString strTemp;
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

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBrgType, m_Data.nBridgeType);
	m_Data.nMaxSuccessiveVehicle = m_spinMaxVehicle.GetEditValueInt();

	m_Data.nTracks = m_cmbTracks.GetCurSel();
	m_Data.nSleeperWidth = m_cmbWidth.GetCurSel();
	m_Data.dSleeperWidth = m_edtWidth.GetEditValue();
	m_Data.dDepthFill = m_edtDepth.GetEditValue();

	return TRUE;
}

void CCMMvctIDDlg::InitCombo() 
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

	m_cmbTracks.ResetContent();
	m_cmbTracks.AddString(_LS(IDS_CMD_MVCT_INDIA_SINGLE));
	m_cmbTracks.AddString(_LS(IDS_CMD_MVCT_INDIA_DOUBLE));
	m_cmbTracks.AddString(_LS(IDS_CMD_MVCT_INDIA_MULTIPLE));
	m_cmbTracks.SetCurSel(0);

	m_cmbWidth.ResetContent();
	m_cmbWidth.AddString(_LS(IDS_CMD_MVCT_INDIA_TYPE1));
	m_cmbWidth.AddString(_LS(IDS_CMD_MVCT_INDIA_TYPE2));
	m_cmbWidth.AddString(_LS(IDS_CMD_MVCT_INDIA_USER));
	m_cmbWidth.SetCurSel(0);
}

BEGIN_MESSAGE_MAP(CCMMvctIDDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvctIDDlg)
	ON_BN_CLICKED(IDC_CMD_INFL_GP1_RDO, OnChangeIGPMethod)
	ON_BN_CLICKED(IDC_CMD_INFL_GP2_RDO, OnChangeIGPMethod)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION_ALL, OnCmdMvctFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP_ALL, OnCmdMvctFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE_ALL, OnCmdMvctFiltersForce)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_LINK_ALL, OnCmdMvctFiltersLink)
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
	ON_BN_CLICKED(IDC_CMD_MVCT_BTYPE_STEEL, OnCmdMvctBridgeTypeRdo)
	ON_BN_CLICKED(IDC_CMD_MVCT_BTYPE_RC, OnCmdMvctBridgeTypeRdo)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_RAIL_LONGI_DISPERSION_WIDTH_CMB,OnCmdMvctSleeperWidthCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMMvctIDDlg message handlers

BOOL CCMMvctIDDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	// TODO: Add extra initialization here
	if (!m_pDoc->m_pAttrCtrl->GetMvctid(m_Data))
	{
		m_Data.Initialize();

		// dDistance는 초기화 해도 된다.
		// Mvct 안 넣은 경우 Default는 Number/Line Element 옵션이기 때문. 아래 값 사용 안됨
		m_Data.dDistance = M_InitValueCurUnit(0.3, TON, M, D_UNITSYS_BASE_LENGTH);

		// MQC 16504 
		m_Data.nCalcPositionFrame = 2;
		m_Data.bStressCalcFrame = TRUE;
	}
	  
	m_spinIGPN.SetRange(2, 10);
	m_spinIGPN.SetInteger(TRUE);
	m_edtIGPDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIGPDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_spinMaxVehicle.SetRange(1, 10);
	m_spinMaxVehicle.SetInteger(TRUE);
	m_edtWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untWidth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtDepth.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDepth.SetUnitType(D_UNITSYS_BASE_LENGTH);

	InitCombo();
	Data2Dlg();

	OnChangeIGPMethod();
	OnCmdMvctSleeperWidthCmb();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctIDDlg::OnCmdMvctOk() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	if (!m_pDoc->m_pDataCtrl->AddMvctid(m_Data)) return;
	
	CDialogMove::OnOK();
}

void CCMMvctIDDlg::OnCmdMvctCancel() 
{
	CDialogMove::OnCancel();	
}

void CCMMvctIDDlg::OnCmdMvctFiltersReaction() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_REACTION_CBO))->EnableWindow(m_nReaction == 1);
}

void CCMMvctIDDlg::OnCmdMvctFiltersDisplacement() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_DISP_CBO))->EnableWindow(m_nDisplacement == 1);
}

void CCMMvctIDDlg::OnCmdMvctFiltersForce() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_FORCE_CBO))->EnableWindow(m_nForceMoment == 1);
}

void CCMMvctIDDlg::OnCmdMvctFiltersLink() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_LINK_CBO))->EnableWindow(m_nLink == 1);
}

void CCMMvctIDDlg::OnCmdMvctFiltersReactions() 
{
	int nCheck = m_chkReaction.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersReaction();
}

void CCMMvctIDDlg::OnCmdMvctFiltersDisplacements() 
{
	int nCheck = m_chkDisplacement.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersDisplacement();
}

void CCMMvctIDDlg::OnCmdMvctFiltersForceMoments() 
{
	int nCheck = m_chkForceMoment.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersForce(); 
}

void CCMMvctIDDlg::OnCmdMvctFiltersLinks() 
{
	int nCheck = m_chkLink.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arLink, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersLink(); 
}

void CCMMvctIDDlg::OnChangeIGPMethod() 
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGPMethod, nCheck);

	m_spinIGPN.EnableWindow(nCheck == 0);
	m_edtIGPDist.EnableWindow(nCheck != 0);
	m_untIGPDist.EnableWindow(nCheck != 0);
}

void CCMMvctIDDlg::OnCmdMvctBridgeTypeRdo() 
{
	int nBridgeType = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlBrgType, nBridgeType);

	double dDepth = 0.0;

	if(nBridgeType == 0)
	{
		m_edtDepth.SetEditUnit(dDepth);
	}
	else
	{
		dDepth = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 300);   
		m_edtDepth.SetEditUnit(dDepth);
	}

	UpdateData(FALSE);
}

void CCMMvctIDDlg::OnCmdMvctSleeperWidthCmb() 
{
	UpdateData(TRUE);

	int nSleeperWidth = m_cmbWidth.GetCurSel();

	GetDlgItem(IDC_CMD_MVCT_RAIL_LONGI_DISPERSION_WIDTH_EDT)->EnableWindow(nSleeperWidth == 2);
	GetDlgItem(IDC_CMD_MVCT_RAIL_LONGI_DISPERSION_WIDTH_UNT)->EnableWindow(nSleeperWidth == 2);
}