// CMGridAnalCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGridAnalCtrlDlg.h"

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
// CCMGridAnalCtrlDlg dialog


CCMGridAnalCtrlDlg::CCMGridAnalCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMGridAnalCtrlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMGridAnalCtrlDlg)
	m_nReaction = 0;
	m_nDisplacement = 0;
	m_nForceMoment = 0;
	m_nLink = 0;

	//}}AFX_DATA_INIT
	m_Data.Initialize();

	m_pDoc = CDBDoc::GetDocPoint();

	m_arAnalCtrl.Add(IDC_CMD_GACD_LIVELOAD_CTRL_GRP);
	m_arAnalCtrl.Add(IDC_CMD_GACD_LIVELOAD_CTRL_NUM_STC);
	m_arAnalCtrl.Add(IDC_CMD_GACD_LIVELOAD_CTRL_NUM_EDT);
	m_arAnalCtrl.Add(IDC_CMD_GACD_LIVELOAD_CTRL_DIST_STC);
	m_arAnalCtrl.Add(IDC_CMD_GACD_LIVELOAD_CTRL_DIST_EDT);
	m_arAnalCtrl.Add(IDC_CMD_GACD_LIVELOAD_CTRL_DIST_UNT);

	m_arReaction.Add(IDC_CMD_GACD_FILTERS_REACTION_ALL);
	m_arReaction.Add(IDC_CMD_GACD_FILTERS_REACTION_GROUP);
	m_arReaction.Add(IDC_CMD_GACD_FILTERS_REACTION_CBO);
	
	m_arDisplacement.Add(IDC_CMD_GACD_FILTERS_DISP_ALL);
	m_arDisplacement.Add(IDC_CMD_GACD_FILTERS_DISP_GROUP);
	m_arDisplacement.Add(IDC_CMD_GACD_FILTERS_DISP_CBO);
	
	m_arForceMoment.Add(IDC_CMD_GACD_FILTERS_FORCE_ALL);
	m_arForceMoment.Add(IDC_CMD_GACD_FILTERS_FORCE_GROUP);
	m_arForceMoment.Add(IDC_CMD_GACD_FILTERS_FORCE_CBO);  

	m_arLink.Add(IDC_CMD_GACD_FILTERS_LINK_ALL);
	m_arLink.Add(IDC_CMD_GACD_FILTERS_LINK_GROUP);
	m_arLink.Add(IDC_CMD_GACD_FILTERS_LINK_CBO);  

}


void CCMGridAnalCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMGridAnalCtrlDlg)
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_FORCE_CBO,    m_cboForceMoment);
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_DISP_CBO,     m_cboDisplacement);
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_REACTION_CBO, m_cboReaction);
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_LINK_CBO,     m_cboLink); 

	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_FORCE,        m_chkForceMoment);
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_DISP,         m_chkDisplacement);
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_REACTION,     m_chkReaction);
	DDX_Control(pDX, IDC_CMD_GACD_FILTERS_LINK,         m_chkLink);
	
	DDX_Radio(pDX, IDC_CMD_GACD_FILTERS_REACTION_ALL,   m_nReaction);
	DDX_Radio(pDX, IDC_CMD_GACD_FILTERS_DISP_ALL,       m_nDisplacement);
	DDX_Radio(pDX, IDC_CMD_GACD_FILTERS_FORCE_ALL,      m_nForceMoment);
	DDX_Radio(pDX, IDC_CMD_GACD_FILTERS_LINK_ALL,       m_nLink);
 
	DDX_Control(pDX, IDC_CMD_GACD_LIVELOAD_CTRL_NUM_EDT, m_edtNum);
	DDX_Control(pDX, IDC_CMD_GACD_LIVELOAD_CTRL_DIST_EDT,m_edtDist);
	DDX_Control(pDX, IDC_CMD_GACD_LIVELOAD_CTRL_DIST_UNT,m_untDist);
	
	//DDX_Control(pDX, IDC_CMD_GACD_FILTERS,              m_chkFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMGridAnalCtrlDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMGridAnalCtrlDlg)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_REACTION_ALL, OnCmdFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_DISP_ALL,     OnCmdFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_FORCE_ALL,    OnCmdFiltersForce)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_LINK_ALL,     OnCmdFiltersLink)

	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_REACTION_GROUP, OnCmdFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_DISP_GROUP,     OnCmdFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_FORCE_GROUP,    OnCmdFiltersForce)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_LINK_GROUP,     OnCmdFiltersLink)

	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_REACTION,       OnCmdFiltersReactions)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_DISP,           OnCmdFiltersDisplacements)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_FORCE,          OnCmdFiltersForceMoments)
	ON_BN_CLICKED(IDC_CMD_GACD_FILTERS_LINK,           OnCmdFiltersLinks)

	ON_BN_CLICKED(IDC_CMD_GACD_OK,                     OnCmdOk)
	ON_BN_CLICKED(IDC_CMD_GACD_CANCEL,                 OnCmdCancel)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMGridAnalCtrlDlg message handlers

BOOL CCMGridAnalCtrlDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	if (!m_pDoc->m_pAttrCtrl->GetGacd(m_Data)) m_Data.Initialize();

	//m_chkFilter.InitControl(this, CCMGridAnalCtrlDlg::IDD, IDC_CMD_GACD_FILTERS,TRUE,TRUE);
	
	InitUnit();
	InitGroupData();
	Data2Dlg();

	return TRUE;	            
}

void CCMGridAnalCtrlDlg::OnCmdOk() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	if (!m_pDoc->m_pDataCtrl->AddGacd(m_Data)) return;
	
	CDialogMove::OnOK();
}

void CCMGridAnalCtrlDlg::OnCmdCancel() 
{
	CDialogMove::OnCancel();	
}


void CCMGridAnalCtrlDlg::OnCmdFiltersReaction() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_GACD_FILTERS_REACTION_CBO))->EnableWindow(m_nReaction == 1);
}

void CCMGridAnalCtrlDlg::OnCmdFiltersDisplacement() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_GACD_FILTERS_DISP_CBO))->EnableWindow(m_nDisplacement == 1);
}

void CCMGridAnalCtrlDlg::OnCmdFiltersForce() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_GACD_FILTERS_FORCE_CBO))->EnableWindow(m_nForceMoment == 1);
}

void CCMGridAnalCtrlDlg::OnCmdFiltersLink() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_GACD_FILTERS_LINK_CBO))->EnableWindow(m_nLink == 1);
}

void CCMGridAnalCtrlDlg::OnCmdFiltersReactions() 
{
	int nCheck = m_chkReaction.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, nCheck == 1);	
	if(nCheck)
		OnCmdFiltersReaction();
}

void CCMGridAnalCtrlDlg::OnCmdFiltersDisplacements() 
{
	int nCheck = m_chkDisplacement.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, nCheck == 1);	
	if(nCheck)
		OnCmdFiltersDisplacement();
}

void CCMGridAnalCtrlDlg::OnCmdFiltersForceMoments() 
{
	int nCheck = m_chkForceMoment.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, nCheck == 1);		
	if(nCheck)
		OnCmdFiltersForce(); 
}

void CCMGridAnalCtrlDlg::OnCmdFiltersLinks() 
{
	int nCheck = m_chkLink.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arLink, nCheck == 1);		
	if(nCheck)
		OnCmdFiltersLink(); 
}


LRESULT CCMGridAnalCtrlDlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	return 0L;
}



//----------------------------------------------------------------------
// Implementation
void CCMGridAnalCtrlDlg::Data2Dlg()
{
	T_GRUP_D GroupData;
	T_BNGR_D BndrData;
	//
	m_chkReaction.SetCheck(m_Data.bReaction);
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, m_Data.bReaction == 1);	
	m_nReaction = m_Data.nReacFilter;
	if(m_nReaction)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ReacGrup, GroupData);
		m_cboReaction.SelectString(0,GroupData.GroupName);
	}
	else
		m_cboReaction.EnableWindow(FALSE);

	//
	m_chkDisplacement.SetCheck(m_Data.bDisplacement);
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, m_Data.bDisplacement == 1);	
	m_nDisplacement = m_Data.nDispFilter;
	if(m_nDisplacement)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.DispGrup, GroupData);
		m_cboDisplacement.SelectString(0,GroupData.GroupName);
	}
	else
		m_cboDisplacement.EnableWindow(FALSE);

	//
	m_chkForceMoment.SetCheck(m_Data.bForce);
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, m_Data.bForce == 1);	
	m_nForceMoment = m_Data.nForcFilter;
	if(m_nForceMoment)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ForcGrup, GroupData);
		m_cboForceMoment.SelectString(0,GroupData.GroupName);
	}
	else
		m_cboForceMoment.EnableWindow(FALSE);


	//
	m_chkLink.SetCheck(m_Data.bLink);
	CDlgUtil::CtrlEnableDisable(this, m_arLink, m_Data.bLink == 1);	
	m_nLink = m_Data.nLinkFilter;
	if(m_nLink)
	{
		m_pDoc->m_pAttrCtrl->GetBngr(m_Data.LinkGrup, BndrData);
		m_cboLink.SelectString(0,BndrData.GroupName);
	}
	else
		m_cboLink.EnableWindow(FALSE);


	//
	m_edtNum.SetEditUnit(m_Data.nSubDivide);

	double dDist = 0.0;
	if (m_pDoc->m_pAttrCtrl->GetGacd(m_Data)) dDist = m_Data.dDistance;
	else
	{
		dDist=M_InitValueCurUnit(1.0, N, M, D_UNITSYS_BASE_LENGTH);
	}   
	m_edtDist.SetEditUnit(dDist);

	UpdateData(FALSE);
}

BOOL CCMGridAnalCtrlDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	CString strTemp;

	//
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

	//
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

	//
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

	//
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

	//
	m_Data.nSubDivide = m_edtNum.GetEditValue();
	m_Data.dDistance  = m_edtDist.GetEditValue();

	return TRUE;
}

void CCMGridAnalCtrlDlg::InitUnit() 
{
	m_edtNum.SetUnitType(D_UNITSYS_NONE);
	m_edtDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CCMGridAnalCtrlDlg::InitGroupData() 
{
	CArray<T_GRUP_K, T_GRUP_K> GroupKeyList;
	T_GRUP_D GroupData;	

	m_pDoc->m_pAttrCtrl->GetGrupKeyList(GroupKeyList);

	for(int i=0; i<GroupKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetGrup(GroupKeyList.GetAt(i), GroupData);
		m_cboReaction    .AddString(GroupData.GroupName);
		m_cboDisplacement.AddString(GroupData.GroupName);
		m_cboForceMoment .AddString(GroupData.GroupName);
	}

	CArray<T_BNGR_K, T_BNGR_K> rKeyList;
	T_BNGR_D rData;
	m_pDoc->m_pAttrCtrl->GetBngrKeyList(rKeyList); 
	for(int i=0; i<rKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetBngr(rKeyList.GetAt(i), rData);
		m_cboLink.AddString(rData.GroupName);
	}
}
