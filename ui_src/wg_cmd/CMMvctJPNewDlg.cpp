// CMMvctJPNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctJPNewDlg.h"

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
// CCMMvctJPNewDlg dialog


CCMMvctJPNewDlg::CCMMvctJPNewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvctJPNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctJPNewDlg)
	m_nReaction = 0;
	m_nDisplacement = 0;
	m_nForceMoment = 0;
	m_nLink = 0;

	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_aCtrlPosFrame.RemoveAll();
	m_aCtrlPosFrame.Add(IDC_CMD_FRAME_OPT1);
	m_aCtrlPosFrame.Add(IDC_CMD_FRAME_OPT2);

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

}


void CCMMvctJPNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctJPNewDlg)
	DDX_Control(pDX, IDC_CMD_IGPN_EDIT, m_spinGPN);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE_CBO, m_cboForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP_CBO, m_cboDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION_CBO, m_cboReaction);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_LINK_CBO, m_cboLink);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE, m_chkForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP, m_chkDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION, m_chkReaction);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_LINK, m_chkLink);
	DDX_Control(pDX, IDC_CMD_LINK_CONCURRENT_CALC_CHK, m_chkConcurrentLink);
	DDX_Control(pDX, IDC_CMD_FRAME_STRESS_CALC, m_chkStressCalcFrame);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_REACTION_ALL, m_nReaction);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_DISP_ALL, m_nDisplacement);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_FORCE_ALL, m_nForceMoment);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_LINK_ALL, m_nLink);
	
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS,        m_chkFilter);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------
// Implementation
void CCMMvctJPNewDlg::Data2Dlg()
{
	T_GRUP_D GroupData;
	T_BNGR_D BngrData;

	m_spinGPN.SetValue(m_Data.nNumUnitLoad);

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame-1);
	m_chkStressCalcFrame.SetCheck(m_Data.bStressCalcFrame);

	m_chkConcurrentLink.SetCheck(m_Data.bConcurrentLink);

	// Filter
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


	UpdateData(FALSE);
}

BOOL CCMMvctJPNewDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	// Analysis method - 사용안함.
	m_Data.nCalculationMethod = 1; // Exact 고정

	// Load Point Selection - 사용안함.
	m_Data.nLoadPointSelection = 2; // All Point 고정

	// Generating point 
	m_Data.nInflGenPoint = 0; // :Number / Line Element 고정
	m_Data.nNumUnitLoad = m_spinGPN.GetEditValueInt();
	m_Data.dDistance = 0.0;   // 사용안함.
	
	// Plate -사용안함.
	m_Data.nCalcPositionPlate = 2; // Center+Node  
	m_Data.bStressCalc        = TRUE;
	m_Data.bConcurrentCalc    = FALSE;

	// Frame
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame);
	m_Data.nCalcPositionFrame += 1;
	m_Data.bStressCalcFrame = m_chkStressCalcFrame.GetCheck();

	// Link
	m_Data.bConcurrentLink = m_chkConcurrentLink.GetCheck();

	// Filter
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
		
	return TRUE;
}

void CCMMvctJPNewDlg::InitGroupData() 
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

void CCMMvctJPNewDlg::AlignControl()
{

}


BEGIN_MESSAGE_MAP(CCMMvctJPNewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvctJPNewDlg)
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

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvctJPNewDlg message handlers

BOOL CCMMvctJPNewDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	AlignControl(); //나중에 지우면 Line Option 가능함.
	CDialogMove::OnInitDialog();

	m_chkFilter.InitControl(this, CCMMvctJPNewDlg::IDD, IDC_CMD_MVCT_FILTERS,TRUE,TRUE);
	
	T_MVCD_D DataMvcd;
	if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();

	if (!m_pDoc->m_pAttrCtrl->GetMvct(m_Data))
	{
		m_Data.Initialize();
		m_Data.dDistance = M_InitValueCurUnit(0.3, KN, M, D_UNITSYS_BASE_LENGTH);
	}

	m_spinGPN.SetRange(2, 10);
	m_spinGPN.SetInteger(TRUE);

	InitGroupData();
	Data2Dlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctJPNewDlg::OnCmdMvctOk() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	if (!m_pDoc->m_pDataCtrl->AddMvct(m_Data)) return;
	
	CDialogMove::OnOK();
}

void CCMMvctJPNewDlg::OnCmdMvctCancel() 
{
	CDialogMove::OnCancel();	
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersReaction() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_REACTION_CBO))->EnableWindow(m_nReaction == 1);
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersDisplacement() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_DISP_CBO))->EnableWindow(m_nDisplacement == 1);
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersForce() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_FORCE_CBO))->EnableWindow(m_nForceMoment == 1);
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersLink() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_LINK_CBO))->EnableWindow(m_nLink == 1);
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersReactions() 
{
	int nCheck = m_chkReaction.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersReaction();
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersDisplacements() 
{
	int nCheck = m_chkDisplacement.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersDisplacement();
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersForceMoments() 
{
	int nCheck = m_chkForceMoment.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersForce(); 
}

void CCMMvctJPNewDlg::OnCmdMvctFiltersLinks() 
{
	int nCheck = m_chkLink.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arLink, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersLink(); 
}

LRESULT CCMMvctJPNewDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	T_MVCD_D MvcdData;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdData)) MvcdData.Initialize();
		
	return 0L;
}