// CMMvctCHDlg.cpp : implementation file
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvctCHDlg.h"

#include "CMMvctCHUserPage.h"
#include "CMMvctCHSbemPage.h"
#include "CMMvctCHCbemPage.h"
#include "CMMvctCHArchPage.h"
#include "CMMvctCHCablPage.h"
#include "CMMvctCHSuspPage.h"
#include "CMMvctCHRCPage.h"
#include "CMMvctCHStlPage.h"
#include "CMMvctCHUrbanPage.h"
#include "CMMvctCHTrainPage.h"
#include "CMMvctCHRailPage.h"

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
// CCMMvctCHDlg dialog
//////////////////////////////////////////////////////////////////

CCMMvctCHDlg::CCMMvctCHDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMMvctCHDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMMvctCHDlg)
	m_nBrgClass = 0;
	m_nReaction = 0;
	m_nDisplacement = 0;
	m_nForceMoment = 0;
	m_nLink = 0;
	m_nLoadPointSel = 0;	
	m_bBrgClassChk = TRUE;
	m_bImpactFactor = TRUE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_pUserPage    = NULL;
	m_pSbemPage    = NULL;
	m_pCbemPage    = NULL;
	m_pArchPage    = NULL;
	m_pCablPage    = NULL;
	m_pSuspPage    = NULL;
	m_pRCPage      = NULL;
	m_pStlPage     = NULL;
	m_pUrbanPage   = NULL;
	m_pTrainPage   = NULL;
	m_pRailwayPage = NULL;
	m_pRailBrgPage = NULL;
	m_pRailCulPage = NULL;

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
	m_arLink.Add(IDC_CMD_MVCTCH_FILTERS_LINK_ALL);
	m_arLink.Add(IDC_CMD_MVCTCH_FILTERS_LINK_GROUP);
	m_arLink.Add(IDC_CMD_MVCTCH_FILTERS_LINK_CBO);  

	m_aCtrlCodeType.Add(IDC_WG_CMD_STATIC8);
	m_aCtrlCodeType.Add(IDC_CMD_MVCT_CODE_TYPE_MTD_CMB);
	
	// Natural Frequency Method  
	m_aCtrlFreqType.Add(IDC_CMD_MVCT_FREQ_MTD_TXT);
	m_aCtrlFreqType.Add(IDC_CMD_MVCT_FREQ_MTD_CMB);

	m_aCtrlArchType.Add(IDC_CMD_MVCT_ARCH_TYPE_TXT);
	m_aCtrlArchType.Add(IDC_CMD_MVCT_ARCH_TYPE_CMB);

	// Span Length Calc. Method
	m_aCtrlSpanType.Add(IDC_CMD_MVCT_SPAN_CALC_MTD_TXT);
	m_aCtrlSpanType.Add(IDC_CMD_MVCT_SPAN_CALC_MTD_CMB);
	
	// Bridge Type for Other Codes
	m_aCtrlBrgType.Add(IDC_CMD_MVCT_BRG_TYPE_TXT);
	m_aCtrlBrgType.Add(IDC_CMD_MVCT_BRG_TYPE_CMB);  
	
	m_aCtrlBrgClass.Add(IDC_CMD_MVCTCH_BRG_CLASS_RDO);
	m_aCtrlBrgClass.Add(IDC_CMD_MVCTCH_BRG_CLASS_RDO2);
		  
	m_aCtrlIGPMethod.Add(IDC_CMD_INFL_GP1_RDO);
	m_aCtrlIGPMethod.Add(IDC_CMD_INFL_GP2_RDO);

	m_aCtrlArchGrp.Add(IDC_CMD_MVCT_MAIN_ARCH_TXT);
	m_aCtrlArchGrp.Add(IDC_CMD_MVCT_MAIN_ARCH_EDT);
	m_aCtrlArchGrp.Add(IDC_CMD_MVCT_MAIN_ARCH_UNT);
	m_aCtrlArchGrp.Add(IDC_CMD_MVCT_ARCH_GROUP_TXT);
	m_aCtrlArchGrp.Add(IDC_CMD_MVCT_ARCH_GROUP_CMB);
	m_aCtrlArchGrp.Add(IDC_CMD_MVCT_COMPONENT_CHK);		
}

void CCMMvctCHDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMMvctCHDlg)
	DDX_Control(pDX, IDC_CMD_INFL_DIST_EDT, m_edtIGPDist);
	DDX_Control(pDX, IDC_CMD_INFL_DIST_UNT, m_untIGPDist);
	DDX_Control(pDX, IDC_CMD_MVCT_MAIN_ARCH_EDT, m_edtArchL);
	DDX_Control(pDX, IDC_CMD_MVCT_MAIN_ARCH_UNT, m_untArchL);
	DDX_Control(pDX, IDC_CMD_MVCT_SPAN_CALC_MTD_CMB, m_wndSpanTypeCmb);
	DDX_Control(pDX, IDC_CMD_MVCT_FREQ_MTD_CMB, m_wndFreqTypeCmb);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_TYPE_CMB, m_wndArchTypeCmb);
	DDX_Control(pDX, IDC_CMD_MVCT_BRG_TYPE_CMB, m_wndBrgTypeCmb);
	DDX_Control(pDX, IDC_CMD_MVCT_CODE_TYPE_MTD_CMB, m_wndCodeTypeCmb);
	DDX_Radio(pDX, IDC_CMD_MVCTCH_BRG_CLASS_RDO, m_nBrgClass);
	DDX_Check(pDX, IDC_CMD_MVCTCH_BRG_CLASS_CHK, m_bBrgClassChk);  
	DDX_Control(pDX, IDC_CMD_IGPN_EDIT, m_spinIGPN);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE_CBO, m_cboForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP_CBO, m_cboDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION_CBO, m_cboReaction);
	DDX_Control(pDX, IDC_CMD_MVCTCH_FILTERS_LINK_CBO, m_cboLink);
	DDX_Control(pDX, IDC_CMD_MVCT_ARCH_GROUP_CMB, m_cboArchGrup);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_FORCE, m_chkForceMoment);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_DISP, m_chkDisplacement);
	DDX_Control(pDX, IDC_CMD_MVCT_FILTERS_REACTION, m_chkReaction);
	DDX_Control(pDX, IDC_CMD_MVCTCH_FILTERS_LINK, m_chkLink);
	DDX_Control(pDX, IDC_CMD_PLATE_STRESS_CALC, m_chkStressCalc);
	DDX_Control(pDX, IDC_CMD_FRAME_STRESS_CALC, m_chkStressCalcFrame);
	DDX_Control(pDX, IDC_CMD_MVCT_COMPONENT_CHK, m_chkComponent);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_REACTION_ALL, m_nReaction);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_DISP_ALL, m_nDisplacement);
	DDX_Radio(pDX, IDC_CMD_MVCT_FILTERS_FORCE_ALL, m_nForceMoment);
	DDX_Radio(pDX, IDC_CMD_MVCTCH_FILTERS_LINK_ALL, m_nLink);
	DDX_Radio(pDX, IDC_CMD_InfPoint, m_nLoadPointSel);	
	DDX_Check(pDX, IDC_CMD_MVCT_IMPACT_FACTOR_CHK, m_bImpactFactor);
	//}}AFX_DATA_MAP
}

//----------------------------------------------------------------------
// Implementation
void CCMMvctCHDlg::Data2Dlg()
{
	T_GRUP_D GroupData;
	T_BNGR_D BngrData;

	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame-1);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlPosPlate, m_Data.nCalcPositionPlate-1);
	
	m_nLoadPointSel = m_Data.nLoadPointSelection-1;
	
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlIGPMethod, m_Data.nInflGenPoint);
	m_spinIGPN.SetValue(m_Data.nNumUnitLoad);
	m_edtIGPDist.SetEditUnit(m_Data.dDistance);
	
	m_chkStressCalc.SetCheck(m_Data.bStressCalc);
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

	//if (m_Data.nCodeType == 4)
	if (m_Data.FreqArch.dn > 0.0 || m_Data.FreqArch.df > 0.0 || m_Data.FreqArch.dL > 0.0 ||
		m_Data.FreqArch.dE > 0.0 || m_Data.FreqArch.dIc > 0.0 || m_Data.FreqArch.dmc > 0.0)
	{
		m_chkComponent.SetCheck(TRUE);
	}

	m_nBrgClass = m_Data.nBridgeClass;
	m_bBrgClassChk = m_Data.bBridgeClass;  
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBrgClass, m_Data.bBridgeClass);	
//	int i = 0;
// 	for(i=0; i<m_wndCodeTypeCmb.GetCount(); i++)
// 	{
// 		if(m_Data.nCodeType != m_wndCodeTypeCmb.GetItemData(i)) continue;
// 		break;
// 	}
	CDlgUtil::CobxSetCurSelItemData(m_wndCodeTypeCmb, m_Data.nCodeType); //m_wndCodeTypeCmb.SetCurSel(i);

	CDlgUtil::CobxSetCurSelItemData(m_wndSpanTypeCmb,m_Data.nSpanLength);    //m_wndSpanTypeCmb.SetCurSel(m_Data.nSpanLength);
	CDlgUtil::CobxSetCurSelItemData(m_wndBrgTypeCmb, m_Data.nBridgeType);    //m_wndBrgTypeCmb.SetCurSel (m_Data.nBridgeType);
	CDlgUtil::CobxSetCurSelItemData(m_wndFreqTypeCmb,m_Data.nFrequencyType); //m_wndFreqTypeCmb.SetCurSel(m_Data.nFrequencyType);
	CDlgUtil::CobxSetCurSelItemData(m_wndArchTypeCmb,m_Data.nArchType);      //m_wndArchTypeCmb.SetCurSel(m_Data.nArchType);


	m_nCodeType = m_Data.nCodeType;
	m_nSpanType = m_Data.nSpanLength;
	m_nBrgType  = m_Data.nBridgeType;
	m_nFreqType = m_Data.nFrequencyType;
	m_nArchType = m_Data.nArchType;

	m_bImpactFactor = m_Data.bImpactFactor;

	m_edtArchL.SetEditUnit(m_Data.dArchL);
	m_pDoc->m_pAttrCtrl->GetGrup(m_Data.ArchGrup, GroupData);
	m_cboArchGrup.SelectString(-1, GroupData.GroupName);

	UpdateData(FALSE);

	ChangeBridgeTypeCombo();

	Data2Dlg_ImpactFactor();  

	OnChangeIGPMethod();
}

BOOL CCMMvctCHDlg::Dlg2Data()
{
	UpdateData(TRUE);
	m_Data.Initialize();

	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosFrame, m_Data.nCalcPositionFrame);
	m_Data.nCalcPositionFrame += 1;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlPosPlate, m_Data.nCalcPositionPlate);
	m_Data.nCalcPositionPlate += 1;

	m_Data.nLoadPointSelection = m_nLoadPointSel+1;
	
	CString strTemp;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGPMethod, m_Data.nInflGenPoint);
	if (m_Data.nInflGenPoint == 0)
		m_Data.nNumUnitLoad = m_spinIGPN.GetEditValueInt();
	else m_Data.dDistance = m_edtIGPDist.GetEditValue();

	m_Data.bStressCalc = m_chkStressCalc.GetCheck();
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

	// 교량 등급(JTJB01-2003)
	m_Data.bBridgeClass = m_bBrgClassChk;
	m_Data.nBridgeClass = m_nBrgClass;
	  
	m_Data.nCodeType = m_nCodeType;
	m_Data.nSpanLength = m_nSpanType;
	m_Data.nBridgeType = m_nBrgType;
	m_Data.nFrequencyType = m_nFreqType;
	m_Data.nArchType = m_nArchType;

	m_Data.bImpactFactor = m_bImpactFactor;

	m_Data.dArchL = m_edtArchL.GetEditValue();
	if (m_cboArchGrup.GetCurSel() == -1)
	{
		//GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__Error___No_selected_Group_Name_));
		//return FALSE;
		m_Data.ArchGrup = 0; 
	}
	else
	{
		m_cboArchGrup.GetLBText(m_cboArchGrup.GetCurSel(), strTemp);
		m_Data.ArchGrup = m_pDoc->m_pAttrCtrl->GetGrupKey(strTemp);
	}

	Dlg2Data_ImpactFactor();
	  
	return TRUE;
}

void CCMMvctCHDlg::CreatePage_ImpactFactor()
{
	m_pUserPage	= new	CCMMvctCHUserPage(&m_Data.FreqUser, this);
	ASSERT(m_pUserPage);	
	if(!m_pUserPage) return;

	m_pSbemPage	= new	CCMMvctCHSbemPage(&m_Data.FreqSbem, this);
	ASSERT(m_pSbemPage);	
	if(!m_pSbemPage) return;

	m_pCbemPage = new CCMMvctCHCbemPage(&m_Data.FreqCbem, this);
	ASSERT(m_pCbemPage);	
	if(!m_pCbemPage) return;

	m_pArchPage	= new	CCMMvctCHArchPage(&m_Data.FreqArch, this);
	ASSERT(m_pArchPage);	
	if(!m_pArchPage) return;

	m_pCablPage = new CCMMvctCHCablPage(&m_Data.FreqCabl, this);
	ASSERT(m_pCablPage);	
	if(!m_pCablPage) return;

	m_pSuspPage	= new	CCMMvctCHSuspPage(&m_Data.FreqSusp, this);
	ASSERT(m_pSuspPage);	
	if(!m_pSuspPage) return;

	m_pRCPage	= new	CCMMvctCHRCPage(&m_Data.BrdgRC, this);
	ASSERT(m_pRCPage);	
	if(!m_pRCPage) return;

	m_pStlPage	= new	CCMMvctCHStlPage(&m_Data.BrdgSTL, this);
	ASSERT(m_pStlPage);	
	if(!m_pStlPage) return;

	m_pUrbanPage	= new	CCMMvctCHUrbanPage(&m_Data.BrdgUrban, this);
	ASSERT(m_pUrbanPage);	
	if(!m_pUrbanPage) return;

	m_pTrainPage	= new	CCMMvctCHTrainPage(&m_Data.BrdgTrain, 1, this);
	ASSERT(m_pTrainPage);	
	if(!m_pTrainPage) return;

	m_pRailwayPage	= new	CCMMvctCHTrainPage(&m_Data.BrdgTrain, 2, this);
	ASSERT(m_pRailwayPage);	
	if(!m_pRailwayPage) return;

	m_pRailBrgPage = new CCMMvctCHRailPage(&m_Data.BrdgRail, 1, this);
	ASSERT(m_pRailBrgPage);	
	if(!m_pRailBrgPage) return;

	m_pRailCulPage = new CCMMvctCHRailPage(&m_Data.BrdgRail, 2, this);
	ASSERT(m_pRailCulPage);	
	if(!m_pRailCulPage) return;

	// holder
	CRect rc;
	CWnd *cwnd = (CWnd *)GetDlgItem(IDC_CMD_MVCT_PLACEHOLDER);	
	if(!cwnd)	return;
	cwnd->GetWindowRect(&rc);		// holder 위치.. 
	this->ScreenToClient(&rc);	// 현재 대화상자 기준좌표. 

	// create dlg
	CRect rDlg;
	if(m_pUserPage->GetSafeHwnd()==0)
	{
		m_pUserPage->Create(IDD_CMD_ML_MVCT_CHINA_USER,this);
		m_pUserPage->GetWindowRect(rDlg);
		m_pUserPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pSbemPage->GetSafeHwnd()==0)
	{
		m_pSbemPage->Create(IDD_CMD_ML_MVCT_CHINA_SBEM,this);
		m_pSbemPage->GetWindowRect(rDlg);
		m_pSbemPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pCbemPage->GetSafeHwnd()==0)
	{
		m_pCbemPage->Create(IDD_CMD_ML_MVCT_CHINA_CBEM,this);
		m_pCbemPage->GetWindowRect(rDlg);
		m_pCbemPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pArchPage->GetSafeHwnd()==0)
	{
		m_pArchPage->Create(IDD_CMD_ML_MVCT_CHINA_ARCH,this);
		m_pArchPage->GetWindowRect(rDlg);
		m_pArchPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pCablPage->GetSafeHwnd()==0)
	{
		m_pCablPage->Create(IDD_CMD_ML_MVCT_CHINA_CABL,this);
		m_pCablPage->GetWindowRect(rDlg);
		m_pCablPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pSuspPage->GetSafeHwnd()==0)
	{
		m_pSuspPage->Create(IDD_CMD_ML_MVCT_CHINA_SUSP,this);
		m_pSuspPage->GetWindowRect(rDlg);
		m_pSuspPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pRCPage->GetSafeHwnd()==0)
	{
		m_pRCPage->Create(IDD_CMD_ML_MVCT_CHINA_RC,this);
		m_pRCPage->GetWindowRect(rDlg);
		m_pRCPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pStlPage->GetSafeHwnd()==0)
	{
		m_pStlPage->Create(IDD_CMD_ML_MVCT_CHINA_STL,this);
		m_pStlPage->GetWindowRect(rDlg);
		m_pStlPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pUrbanPage->GetSafeHwnd()==0)
	{
		m_pUrbanPage->Create(IDD_CMD_ML_MVCT_CHINA_URBAN,this);
		m_pUrbanPage->GetWindowRect(rDlg);
		m_pUrbanPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pTrainPage->GetSafeHwnd()==0)
	{
		m_pTrainPage->Create(IDD_CMD_ML_MVCT_CHINA_TRAIN,this);
		m_pTrainPage->GetWindowRect(rDlg);
		m_pTrainPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pRailwayPage->GetSafeHwnd()==0)
	{
		m_pRailwayPage->Create(IDD_CMD_ML_MVCT_CHINA_TRAIN,this);
		m_pRailwayPage->GetWindowRect(rDlg);
		m_pRailwayPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
	
	if(m_pRailBrgPage->GetSafeHwnd()==0)
	{
		m_pRailBrgPage->Create(IDD_CMD_ML_MVCT_CHINA_RAIL,this);
		m_pRailBrgPage->GetWindowRect(rDlg);
		m_pRailBrgPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}

	if(m_pRailCulPage->GetSafeHwnd()==0)
	{
		m_pRailCulPage->Create(IDD_CMD_ML_MVCT_CHINA_RAIL,this);
		m_pRailCulPage->GetWindowRect(rDlg);
		m_pRailCulPage->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rDlg.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
	}
}

void CCMMvctCHDlg::Data2Dlg_ImpactFactor()
{
	if(m_nCodeType == 0)
	{
		if(m_nFreqType == 0)
		{
			if(m_pUserPage) m_pUserPage->SetData2Dlg();
		}
		else if(m_nFreqType == 1)
		{
			if(m_pSbemPage) m_pSbemPage->SetData2Dlg();
		}
		else if(m_nFreqType == 2)
		{
			if(m_pCbemPage) m_pCbemPage->SetData2Dlg();
		}
		else if(m_nFreqType == 3)
		{
			if(m_pArchPage) m_pArchPage->SetData2Dlg();
		}
		else if(m_nFreqType == 4)
		{
			if(m_pCablPage) m_pCablPage->SetData2Dlg();
		}
		else if(m_nFreqType == 5)
		{
			if(m_pSuspPage) m_pSuspPage->SetData2Dlg();
		}
	}
	else if(m_nCodeType == 1)
	{
		if(m_nBrgType == 0)
		{
			if(m_pRCPage) m_pRCPage->SetData2Dlg();
		}
		else if(m_nBrgType == 1)
		{
			if(m_pStlPage) m_pStlPage->SetData2Dlg();
		}
		else if(m_nBrgType == 2)
		{
			if(m_pUrbanPage) m_pUrbanPage->SetData2Dlg();
		}
		else if(m_nBrgType == 3)
		{
			if(m_pTrainPage) m_pTrainPage->SetData2Dlg();
		}
	}
	else if(m_nCodeType == 2)
	{
		if(m_nBrgType == 0)
		{
			if(m_pRailwayPage) m_pRailwayPage->SetData2Dlg();
		}
		else if(m_nBrgType == 1)
		{
			if(m_pRailBrgPage) m_pRailBrgPage->SetData2Dlg();
		}
		else if(m_nBrgType == 2)
		{
			if(m_pRailCulPage) m_pRailCulPage->SetData2Dlg();
		}
	}
	else if (m_nCodeType == 4)
	{
		if (m_nFreqType == 3)
		{
			if (m_pArchPage) m_pArchPage->SetData2Dlg();
		}
	}
}

BOOL CCMMvctCHDlg::Dlg2Data_ImpactFactor()
{
	if(m_nCodeType == 0)
	{
		if(m_nFreqType == 0)
		{
			if(m_pUserPage) m_pUserPage->SetDlg2Data();
		}
		else if(m_nFreqType == 1)
		{
			if(m_pSbemPage) m_pSbemPage->SetDlg2Data();
		}
		else if(m_nFreqType == 2)
		{
			if(m_pCbemPage) m_pCbemPage->SetDlg2Data();
		}
		else if(m_nFreqType == 3)
		{
			if(m_pArchPage) m_pArchPage->SetDlg2Data();
		}
		else if(m_nFreqType == 4)
		{
			if(m_pCablPage) m_pCablPage->SetDlg2Data();
		}
		else if(m_nFreqType == 5)
		{
			if(m_pSuspPage) m_pSuspPage->SetDlg2Data();
		}
	}
	else if(m_nCodeType == 1)
	{
		if(m_nBrgType == 0)
		{
			if(m_pRCPage) m_pRCPage->SetDlg2Data();
		}
		else if(m_nBrgType == 1)
		{
			if(m_pStlPage) m_pStlPage->SetDlg2Data();
		}
		else if(m_nBrgType == 2)
		{
			if(m_pUrbanPage) m_pUrbanPage->SetDlg2Data();
		}
		else if(m_nBrgType == 3)
		{
			if(m_pTrainPage) m_pTrainPage->SetDlg2Data();
		}
	}
	else if(m_nCodeType == 2)
	{
		if(m_nBrgType == 0)
		{
			if(m_pRailwayPage) m_pRailwayPage->SetDlg2Data();
		}
		else if(m_nBrgType == 1)
		{
			if(m_pRailBrgPage) m_pRailBrgPage->SetDlg2Data();
		}
		else if(m_nBrgType == 2)
		{
			if(m_pRailCulPage) m_pRailCulPage->SetDlg2Data();
		}
	}
	else if (m_nCodeType == 4)
	{
		if (m_nFreqType == 3)
		{
			if (m_pArchPage) m_pArchPage->SetDlg2Data();
		}
	}

	return TRUE;
}

void CCMMvctCHDlg::DestroyPage_ImpactFactor()
{
	if(m_pUserPage)
	{
		m_pUserPage->SendMessage(WM_DESTROY,0,0);
		delete m_pUserPage;
		m_pUserPage = NULL;
	}

	if(m_pSbemPage)
	{
		m_pSbemPage->SendMessage(WM_DESTROY,0,0);
		delete m_pSbemPage;
		m_pSbemPage = NULL;
	}

	if(m_pCbemPage)
	{
		m_pCbemPage->SendMessage(WM_DESTROY,0,0);
		delete m_pCbemPage;
		m_pCbemPage = NULL;
	}

	if(m_pArchPage)
	{
		m_pArchPage->SendMessage(WM_DESTROY,0,0);
		delete m_pArchPage;
		m_pArchPage = NULL;
	}

	if(m_pCablPage)
	{
		m_pCablPage->SendMessage(WM_DESTROY,0,0);
		delete m_pCablPage;
		m_pCablPage = NULL;
	}

	if(m_pSuspPage)
	{
		m_pSuspPage->SendMessage(WM_DESTROY,0,0);
		delete m_pSuspPage;
		m_pSuspPage = NULL;
	}

	if(m_pRCPage)
	{
		m_pRCPage->SendMessage(WM_DESTROY,0,0);
		delete m_pRCPage;
		m_pRCPage = NULL;
	}	

	if(m_pStlPage)
	{
		m_pStlPage->SendMessage(WM_DESTROY,0,0);
		delete m_pStlPage;
		m_pStlPage = NULL;
	}

	if(m_pUrbanPage)
	{
		m_pUrbanPage->SendMessage(WM_DESTROY,0,0);
		delete m_pUrbanPage;
		m_pUrbanPage = NULL;
	}

	if(m_pTrainPage)
	{
		m_pTrainPage->SendMessage(WM_DESTROY,0,0);
		delete m_pTrainPage;
		m_pTrainPage = NULL;
	}

	if(m_pRailwayPage)
	{
		m_pRailwayPage->SendMessage(WM_DESTROY,0,0);
		delete m_pRailwayPage;
		m_pRailwayPage = NULL;
	}
	
	if(m_pRailBrgPage)
	{
		m_pRailBrgPage->SendMessage(WM_DESTROY,0,0);
		delete m_pRailBrgPage;
		m_pRailBrgPage = NULL;
	}

	if(m_pRailCulPage)
	{
		m_pRailCulPage->SendMessage(WM_DESTROY,0,0);
		delete m_pRailCulPage;
		m_pRailCulPage = NULL;
	}
}

void CCMMvctCHDlg::InitGroupData() 
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
		m_cboArchGrup.AddString(GroupData.GroupName);
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

void CCMMvctCHDlg::AlignControl()
{
	CRect BasePos, MovePos;  
	int nDistX, nDistY;

	if (m_nCodeType == 4)
	{
		GetDlgItem(IDC_CMD_MVCT_SPAN_CALC_MTD_CMB)->GetWindowRect(BasePos);
		GetDlgItem(IDC_CMD_MVCT_ARCH_TYPE_CMB)->GetWindowRect(MovePos);
		nDistX = BasePos.left - MovePos.left;
		nDistY = BasePos.top - MovePos.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlArchType, nDistX, nDistY);

		GetDlgItem(IDC_CMD_MVCT_ARCH_TYPE_TXT)->GetWindowRect(BasePos);
		GetDlgItem(IDC_CMD_MVCT_MAIN_ARCH_TXT)->GetWindowRect(MovePos);
		nDistX = BasePos.left - MovePos.left;
		GetDlgItem(IDC_CMD_MVCT_BRG_TYPE_CMB)->GetWindowRect(BasePos);
		GetDlgItem(IDC_CMD_MVCT_ARCH_GROUP_CMB)->GetWindowRect(MovePos);
		nDistY = BasePos.top - MovePos.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlArchGrp, nDistX, nDistY);
	}
	else
	{
		GetDlgItem(IDC_CMD_MVCT_SPAN_CALC_MTD_CMB)->GetWindowRect(BasePos);
		GetDlgItem(IDC_CMD_MVCT_FREQ_MTD_CMB)->GetWindowRect(MovePos);
		nDistX = BasePos.left - MovePos.left;
		nDistY = BasePos.top - MovePos.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlFreqType, nDistX, nDistY);

		GetDlgItem(IDC_CMD_MVCT_BRG_TYPE_CMB)->GetWindowRect(BasePos);
		GetDlgItem(IDC_CMD_MVCT_ARCH_TYPE_CMB)->GetWindowRect(MovePos);
		nDistX = BasePos.left - MovePos.left;
		nDistY = BasePos.top - MovePos.top;
		CDlgUtil::CtrlMoveDistXY(this, m_aCtrlArchType, nDistX, nDistY);
	}
}

BEGIN_MESSAGE_MAP(CCMMvctCHDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMMvctCHDlg)
	ON_BN_CLICKED(IDC_CMD_INFL_GP1_RDO, OnChangeIGPMethod)
	ON_BN_CLICKED(IDC_CMD_INFL_GP2_RDO, OnChangeIGPMethod)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION_ALL, OnCmdMvctFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP_ALL, OnCmdMvctFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE_ALL, OnCmdMvctFiltersForce)
	ON_BN_CLICKED(IDC_CMD_MVCTCH_FILTERS_LINK_ALL, OnCmdMvctFiltersLink)
	ON_BN_CLICKED(IDC_CMD_MVCT_OK, OnCmdMvctOk)
	ON_BN_CLICKED(IDC_CMD_MVCT_CANCEL, OnCmdMvctCancel)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION, OnCmdMvctFiltersReactions)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP, OnCmdMvctFiltersDisplacements)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE, OnCmdMvctFiltersForceMoments)
	ON_BN_CLICKED(IDC_CMD_MVCTCH_FILTERS_LINK, OnCmdMvctFiltersLinks)
	ON_BN_CLICKED(IDC_CMD_MVCTCH_BRG_CLASS_CHK, OnCmdMvctBrgClassCheck)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_CODE_TYPE_MTD_CMB, OnSelchangeCmdMvctCodeTypeMtdCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_BRG_TYPE_CMB, OnSelchangeCmdMvctBrgTypeCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_SPAN_CALC_MTD_CMB, OnSelchangeCmdMvctSpanCalcMtdCmb)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_REACTION_GROUP, OnCmdMvctFiltersReaction)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_DISP_GROUP, OnCmdMvctFiltersDisplacement)
	ON_BN_CLICKED(IDC_CMD_MVCT_FILTERS_FORCE_GROUP, OnCmdMvctFiltersForce)
	ON_BN_CLICKED(IDC_CMD_MVCTCH_FILTERS_LINK_GROUP, OnCmdMvctFiltersLink)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_FREQ_MTD_CMB, OnSelchangeCmdMvctFreqMtdCmb)
	ON_CBN_SELCHANGE(IDC_CMD_MVCT_ARCH_TYPE_CMB, OnSelchangeCmdMvctArchTypeCmb)
	ON_BN_CLICKED(IDC_CMD_MVCT_IMPACT_FACTOR_CHK, OnCmdMvctImpactFactorChk)
	ON_BN_CLICKED(IDC_CMD_MVCT_COMPONENT_CHK, OnCmdComponentChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMMvctCHDlg message handlers


BOOL CCMMvctCHDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	//AlignControl(); //나중에 지우면 Line Option 가능함.
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!m_pDoc->m_pAttrCtrl->GetMvctch(m_Data))
	{
		m_Data.Initialize();

		// dDistance는 초기화 해도 된다.
		// Mvct 안 넣은 경우 Default는 Number/Line Element 옵션이기 때문. 아래 값 사용 안됨
		m_Data.dDistance = M_InitValueCurUnit(0.3, TON, M, D_UNITSYS_BASE_LENGTH);
	}
	
	m_spinIGPN.SetRange(2, 10);
	m_spinIGPN.SetInteger(TRUE);
	
	m_edtIGPDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untIGPDist.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_edtArchL.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_untArchL.SetUnitType(D_UNITSYS_BASE_LENGTH);

	CreatePage_ImpactFactor();
			
	InitGroupData();
	InitSetCmb4ImpactFactor();
	Data2Dlg();
	CtrlManager();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMMvctCHDlg::InitSetCmb4ImpactFactor()
{
	m_wndCodeTypeCmb.ResetContent();
	m_wndBrgTypeCmb.ResetContent();
	m_wndSpanTypeCmb.ResetContent();
	m_wndFreqTypeCmb.ResetContent();  
	m_wndArchTypeCmb.ResetContent();

	m_wndCodeTypeCmb.SetItemData(m_wndCodeTypeCmb.AddString(_T("JTG/T D65-06-2015")),         4);
	m_wndCodeTypeCmb.SetItemData(m_wndCodeTypeCmb.AddString(_LS(IDS_CMD_MVCT_QCR9300_2018)),  3);
	m_wndCodeTypeCmb.SetItemData(m_wndCodeTypeCmb.AddString(_LS(IDS_CMD_MVCT_TB_10002_2017)), 2);
	m_wndCodeTypeCmb.SetItemData(m_wndCodeTypeCmb.AddString(_T("JTG D60-2015/JTG04")),        0);
	m_wndCodeTypeCmb.SetItemData(m_wndCodeTypeCmb.AddString(_LS(IDS_CMD_MVCT_JTGB01_PREV)),   1);	

	m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_RC)),                 0);
	m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_WG_CMD__ADDD__Steel)),    1);
	m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVLD_Urban_Bridge)),  2);
	m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVCT_TRAIN_SUBWAY)),  3);

	m_wndSpanTypeCmb.SetItemData(m_wndSpanTypeCmb.AddString(_LS(IDS_CMD_MVCT_SPAN_LENGTH1)),   0);
	m_wndSpanTypeCmb.SetItemData(m_wndSpanTypeCmb.AddString(_LS(IDS_CMD_MVCT_SPAN_LENGTH2)),   1);

	m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_USER_INPUT)),       0);
	m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_SIMPLE_BEAM)),      1);
	m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_CONTINU_BEAM)),     2);
	m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_ARCH_BRG)),         3);
	m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_CABLE_STAY_BRG)),   4);
	m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_PJST_SusBrg)),           5);

	m_wndArchTypeCmb.SetItemData(m_wndArchTypeCmb.AddString(_LS(IDS_CMD_MVCT_ARCH_UNIFORM)),   0);
	m_wndArchTypeCmb.SetItemData(m_wndArchTypeCmb.AddString(_LS(IDS_CMD_MVCT_ARCH_TAPERED)),   1);
	
	CDlgUtil::CobxSetCurSelItemData(m_wndCodeTypeCmb, 1); // m_wndCodeTypeCmb.SetCurSel(1);
	CDlgUtil::CobxSetCurSelItemData(m_wndBrgTypeCmb,  0); // m_wndBrgTypeCmb .SetCurSel(0);
	CDlgUtil::CobxSetCurSelItemData(m_wndSpanTypeCmb, 0); // m_wndSpanTypeCmb.SetCurSel(0);
	CDlgUtil::CobxSetCurSelItemData(m_wndFreqTypeCmb, 0); // m_wndFreqTypeCmb.SetCurSel(0);
	CDlgUtil::CobxSetCurSelItemData(m_wndArchTypeCmb, 0); // m_wndArchTypeCmb.SetCurSel(0);

	CDlgUtil::CobxAdjustListBoxWidth(m_wndCodeTypeCmb);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndBrgTypeCmb);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndSpanTypeCmb);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndFreqTypeCmb);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndArchTypeCmb);
}

void CCMMvctCHDlg::OnCmdMvctOk() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return;
	
	if (!m_pDoc->m_pDataCtrl->AddMvctch(m_Data)) return;

	DestroyPage_ImpactFactor();
	
	CDialogMove::OnOK();
}

void CCMMvctCHDlg::OnCmdMvctCancel() 
{
	CDialogMove::OnCancel();	
}

void CCMMvctCHDlg::OnCmdMvctFiltersReaction() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_REACTION_CBO))->EnableWindow(m_nReaction == 1);
}

void CCMMvctCHDlg::OnCmdMvctFiltersDisplacement() 
{
	UpdateData(TRUE);	
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_DISP_CBO))->EnableWindow(m_nDisplacement == 1);
}

void CCMMvctCHDlg::OnCmdMvctFiltersForce() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCT_FILTERS_FORCE_CBO))->EnableWindow(m_nForceMoment == 1);
}

void CCMMvctCHDlg::OnCmdMvctFiltersLink() 
{
	UpdateData(TRUE);
	((CComboBox*)GetDlgItem(IDC_CMD_MVCTCH_FILTERS_LINK_CBO))->EnableWindow(m_nLink == 1);
}

void CCMMvctCHDlg::OnCmdMvctFiltersReactions() 
{
	int nCheck = m_chkReaction.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arReaction, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersReaction();
}

void CCMMvctCHDlg::OnCmdMvctFiltersDisplacements() 
{
	int nCheck = m_chkDisplacement.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arDisplacement, nCheck == 1);	
	if(nCheck)
		OnCmdMvctFiltersDisplacement();
}

void CCMMvctCHDlg::OnCmdMvctFiltersForceMoments() 
{
	int nCheck = m_chkForceMoment.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arForceMoment, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersForce(); 
}

void CCMMvctCHDlg::OnCmdMvctFiltersLinks() 
{
	int nCheck = m_chkLink.GetCheck();
	CDlgUtil::CtrlEnableDisable(this, m_arLink, nCheck == 1);		
	if(nCheck)
		OnCmdMvctFiltersLink(); 
}

void CCMMvctCHDlg::OnCmdMvctBrgClassCheck()
{
	UpdateData(TRUE);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBrgClass, m_bBrgClassChk);
}
	
void CCMMvctCHDlg::CtrlManager()
{
	UpdateData(TRUE);
	
	m_nCodeType = m_wndCodeTypeCmb.GetItemData(m_wndCodeTypeCmb.GetCurSel());
	m_nSpanType = m_wndSpanTypeCmb.GetItemData(m_wndSpanTypeCmb.GetCurSel());
	m_nBrgType  = m_wndBrgTypeCmb.GetItemData(m_wndBrgTypeCmb.GetCurSel());
	m_nFreqType = m_wndFreqTypeCmb.GetItemData(m_wndFreqTypeCmb.GetCurSel());
	m_nArchType = m_wndArchTypeCmb.GetItemData(m_wndArchTypeCmb.GetCurSel());

	AlignControl();

	BOOL bJTG2015 = (m_nCodeType == 0 || m_nCodeType == 4) ? TRUE : FALSE;
	// JTG D60-2004인 경우 Natural Frequency Method 입력 Combo를 Show
	CDlgUtil::CtrlShowHide(this, m_aCtrlFreqType, m_nCodeType == 0);
	CDlgUtil::CtrlShowHide(this, m_aCtrlArchType, bJTG2015);
	CDlgUtil::CtrlShowHide(this, m_aCtrlArchGrp,  m_nCodeType == 4);
	// JTG D60-2004이고, Natural Frequency Method가 Arch Bridge인 경우 Arch Type 선택 Combo를 Show

	// Other Codes인 경우 Span Length Calc. Method와 Bridge Type Combo를 Show 
	CDlgUtil::CtrlShowHide(this, m_aCtrlSpanType, m_nCodeType == 1 || m_nCodeType == 2 || m_nCodeType == 3);
	CDlgUtil::CtrlShowHide(this, m_aCtrlBrgType,  m_nCodeType == 1 || m_nCodeType == 2 || m_nCodeType == 3);

	if(m_pUserPage) m_pUserPage->ShowWindow(m_nCodeType == 0 && m_nFreqType == 0);
	if(m_pSbemPage) m_pSbemPage->ShowWindow(m_nCodeType == 0 && m_nFreqType == 1);
	if(m_pCbemPage) m_pCbemPage->ShowWindow(m_nCodeType == 0 && m_nFreqType == 2);
	if(m_pArchPage) m_pArchPage->ShowWindow(bJTG2015         && m_nFreqType == 3);
	if(m_pCablPage) m_pCablPage->ShowWindow(m_nCodeType == 0 && m_nFreqType == 4);
	if(m_pSuspPage) m_pSuspPage->ShowWindow(m_nCodeType == 0 && m_nFreqType == 5);

	if(m_pRCPage   ) m_pRCPage    ->ShowWindow(m_nCodeType==1 && m_nBrgType == 0);
	if(m_pStlPage  ) m_pStlPage   ->ShowWindow(m_nCodeType==1 && m_nBrgType == 1);
	if(m_pUrbanPage) m_pUrbanPage ->ShowWindow(m_nCodeType==1 && m_nBrgType == 2);
	if(m_pTrainPage) m_pTrainPage ->ShowWindow(m_nCodeType==1 && m_nBrgType == 3);

	if(m_pRailwayPage) m_pRailwayPage->ShowWindow((m_nCodeType==2 || m_nCodeType==3) && m_nBrgType == 0);
	if(m_pRailBrgPage) m_pRailBrgPage->ShowWindow((m_nCodeType==2 || m_nCodeType==3) && m_nBrgType == 1);
	if(m_pRailCulPage) m_pRailCulPage->ShowWindow((m_nCodeType==2 || m_nCodeType==3) && m_nBrgType == 2);


	//////////////////////////////////////////////////////////
	if(m_pArchPage) m_pArchPage->ControlsShowHide(m_nArchType);

	//////////////////////////////////////////////////////////

	CDlgUtil::CtrlEnableDisable(this, m_aCtrlCodeType, m_bImpactFactor);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlFreqType, m_bImpactFactor);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlArchType, m_bImpactFactor && bJTG2015 && m_nFreqType == 3);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlSpanType, m_bImpactFactor);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlBrgType,  m_bImpactFactor);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrlArchGrp, m_bImpactFactor && bJTG2015 && m_nFreqType == 3);
		
	if(m_pUserPage)    m_pUserPage   ->EnableDisableControls(m_bImpactFactor);
	if(m_pSbemPage)    m_pSbemPage   ->EnableDisableControls(m_bImpactFactor);
	if(m_pCbemPage)    m_pCbemPage   ->EnableDisableControls(m_bImpactFactor);
 	if(m_pArchPage)    m_pArchPage   ->EnableDisableControls(m_bImpactFactor, bJTG2015 && m_nFreqType == 3);
 	if(m_pCablPage)    m_pCablPage   ->EnableDisableControls(m_bImpactFactor);
 	if(m_pSuspPage)    m_pSuspPage   ->EnableDisableControls(m_bImpactFactor);
 	if(m_pRCPage)      m_pRCPage     ->EnableDisableControls(m_bImpactFactor);
 	if(m_pStlPage)     m_pStlPage    ->EnableDisableControls(m_bImpactFactor);
 	if(m_pUrbanPage)   m_pUrbanPage  ->EnableDisableControls(m_bImpactFactor);
	if(m_pTrainPage)   m_pTrainPage  ->EnableDisableControls(m_bImpactFactor, m_nSpanType);
	if(m_pRailwayPage) m_pRailwayPage->EnableDisableControls(m_bImpactFactor, m_nSpanType);
	if(m_pRailBrgPage) m_pRailBrgPage->EnableDisableControls(m_bImpactFactor, m_nSpanType);
	if(m_pRailCulPage) m_pRailCulPage->EnableDisableControls(m_bImpactFactor, m_nSpanType);
		
	UpdateData(FALSE);

	if (m_nCodeType == 4 || m_nFreqType == 3)
		OnCmdComponentChk();
}

void CCMMvctCHDlg::ChangeBridgeTypeCombo()
{
	UpdateData(TRUE);

	int nCodeType = m_wndCodeTypeCmb.GetItemData(m_wndCodeTypeCmb.GetCurSel());

	if(m_nCodeType != nCodeType) { m_nCodeType = nCodeType; m_nBrgType = 0; }
	
	m_wndBrgTypeCmb.ResetContent();

	if(m_nCodeType == 1)
	{
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_RC)),                0);
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_WG_CMD__ADDD__Steel)),   1);
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVLD_Urban_Bridge)), 2);
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVCT_TRAIN_SUBWAY)), 3);
	}
	else if(m_nCodeType == 2 || m_nCodeType == 3) // TB 10002-2017, Q/CR 9300-2018
	{
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVCT_CH_RAILWAY)),          0);
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVCT_CH_RAILWAY_BRIDGE)),   1);
		m_wndBrgTypeCmb.SetItemData(m_wndBrgTypeCmb.AddString(_LS(IDS_CMD_MVCT_CH_RAILWAY_CULVERT)),  2);
	}

	m_wndFreqTypeCmb.ResetContent();
	if (m_nCodeType == 4)
	{
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_ARCH_BRG)), 3); // Arch Bridge
		m_nFreqType = 3;
	}
	else
	{
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_USER_INPUT)),       0);
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_SIMPLE_BEAM)),      1);
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_CONTINU_BEAM)),     2);
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_ARCH_BRG)),         3);
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_MVCT_CABLE_STAY_BRG)),   4);
		m_wndFreqTypeCmb.SetItemData(m_wndFreqTypeCmb.AddString(_LS(IDS_CMD_PJST_SusBrg)),           5);
	}

	CDlgUtil::CobxSetCurSelItemData(m_wndBrgTypeCmb, m_nBrgType);   //m_wndBrgTypeCmb.SetCurSel(m_nBrgType);
	CDlgUtil::CobxSetCurSelItemData(m_wndFreqTypeCmb, m_nFreqType); //m_wndFreqTypeCmb.SetCurSel(m_nFreqType);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndBrgTypeCmb);
	CDlgUtil::CobxAdjustListBoxWidth(m_wndFreqTypeCmb);
}

void CCMMvctCHDlg::OnSelchangeCmdMvctCodeTypeMtdCmb() 
{	
	ChangeBridgeTypeCombo();
	CtrlManager();
}

void CCMMvctCHDlg::OnSelchangeCmdMvctBrgTypeCmb() 
{
	CtrlManager();
}

void CCMMvctCHDlg::OnSelchangeCmdMvctSpanCalcMtdCmb() 
{
	CtrlManager();
}

void CCMMvctCHDlg::OnSelchangeCmdMvctFreqMtdCmb() 
{
	CtrlManager();
}

void CCMMvctCHDlg::OnSelchangeCmdMvctArchTypeCmb() 
{
	CtrlManager();
}

void CCMMvctCHDlg::OnChangeIGPMethod() 
{
	int nCheck;
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlIGPMethod, nCheck);

	m_spinIGPN.EnableWindow(nCheck == 0);
	m_edtIGPDist.EnableWindow(nCheck != 0);
	m_untIGPDist.EnableWindow(nCheck != 0);
}

void CCMMvctCHDlg::OnCmdMvctImpactFactorChk() 
{
	CtrlManager();
}

void CCMMvctCHDlg::OnCmdComponentChk()
{
	if (m_nFreqType == 3)
	{
		BOOL bChk = m_chkComponent.GetCheck();

		if (m_pArchPage)
		{
			if (!bChk)
			{
				m_Data.FreqArch.Initialize();
				m_pArchPage->SetData2Dlg();
				m_pArchPage->EnableDisableComponent(FALSE);
			}
			else
			{
				m_pArchPage->EnableDisableComponent(TRUE);
			}
		}
	}
	else
		ASSERT(0);
}

	
