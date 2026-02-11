// CmdAutoLoadCombCvlAASHTOLRFD12Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlAASHTOLRFD12Dlg.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
#include "LoadCombCtrl.h"
#include "CmdSetOrthoLC_C3_Dlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_StageInfo.h"
#include "..\wg_db\wg_db_DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD12Dlg dialog
CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::CCmdAutoLoadCombCvlAASHTOLRFD12Dlg(CWnd* pParent /*=NULL*/)
	: CChildDialog(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

	m_nConstLoad = 0;

	m_nLrfd12DC = 0;
	m_nLrfd12DDType = 0;
	m_nLrfd12DD1 = 0;
	m_nLrfd12DD2 = 0;
	m_nLrfd12DD3 = 0;
	m_nLrfd12DW = 0;
	m_nLrfd12EHType = 0;
	m_nLrfd12EH1 = 0;
	m_nLrfd12EH2 = 0;
	m_nLrfd12EVType = 0;
	m_nLrfd12EV2 = 0;
	m_nLrfd12EV3 = 0;
	m_nLrfd12EV4 = 0;
	m_nLrfd12EV5 = 0;
	m_nLrfd12EV6 = 0;
	m_nLrfd12EV7 = 0;
	m_nLrfd12ES = 0;
	m_bLrfd12Culvert = FALSE;
	m_nLrfd12Deformation = 1;	
	m_bAASHTOLrfd16 = FALSE;
	m_dFacGammaLL = 0.0;
    m_bAASHTOLrfd18 = FALSE;
	m_bAASHTOLrfd20 = FALSE;
	m_bAASHTOLrfd24 = FALSE;

    m_nOrthoCombType = 0;
    m_a100_30_Load.RemoveAll();
    m_arrEff_MVData.RemoveAll();
	m_arrEff_LLData.RemoveAll();
    m_dEffLoadFactor = 0.5;
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg)  
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_LOAD_MODIFIER_EDT,  m_edtLrfd12Modifier);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DC_MAX_RDO,         m_nLrfd12DC);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DD1_RDO,            m_nLrfd12DDType);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DD1_MAX_RDO,        m_nLrfd12DD1);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DD2_MAX_RDO,        m_nLrfd12DD2);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DD3_MAX_RDO,        m_nLrfd12DD3);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DW_MAX_RDO,         m_nLrfd12DW);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EH1_RDO,            m_nLrfd12EHType);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EH1_MAX_RDO,        m_nLrfd12EH1);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EH2_MAX_RDO,        m_nLrfd12EH2);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV1_RDO,            m_nLrfd12EVType);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV2_MAX_RDO,        m_nLrfd12EV2);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV3_MAX_RDO,        m_nLrfd12EV3);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV4_MAX_RDO,        m_nLrfd12EV4);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV5_MAX_RDO,        m_nLrfd12EV5);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV6_MAX_RDO,        m_nLrfd12EV6);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_EV7_MAX_RDO,        m_nLrfd12EV7);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_ES_MAX_RDO,         m_nLrfd12ES);
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_PS_CR_SH_CMB,       m_cmbPSCRSHType);
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_PS_EDIT,            m_edtLrfd12PS);
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_CR_SH_EDIT,         m_edtLrfd12CRSH);
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_SETTLEMENT_EDT,     m_edtLrfd12Settlement);
	DDX_Check  (pDX, IDC_CMD_AASHTO_LRFD12_CULVERT_CHK,        m_bLrfd12Culvert);
	DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD12_DEFORMATION_RDO,    m_nLrfd12Deformation);
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD16_GAMMALL_EDT,        m_edtLrfd16_GammaLL);
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_LOAD_FACTOR_GRP,    m_chkLoadFactor);
    //DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_FRAME,    m_chkSeisLoad);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_CHK,      m_wndSeisChk    );
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_OTHO_CHK ,          m_wndOthoChk    );
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LST,   m_lstEffLoad);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_CMB,m_cmbEffLoadCase);
    DDX_Text   (pDX, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_EDT,m_dEffLoadFactor);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg, CChildDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlAASHTOLRFD12Dlg)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_DD1_RDO,  OnCmdAASHTOLRFD12DDTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_DD2_RDO,  OnCmdAASHTOLRFD12DDTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_DD3_RDO,  OnCmdAASHTOLRFD12DDTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EH1_RDO,  OnCmdAASHTOLRFD12EHTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EH2_RDO,  OnCmdAASHTOLRFD12EHTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EH3_RDO,  OnCmdAASHTOLRFD12EHTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV1_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV2_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV3_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV4_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV5_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV6_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_EV7_RDO,  OnCmdAASHTOLRFD12EVTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_CHK, OnCmdSeisChk)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_OTHO_CHK, OnCmdOthoChk)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_OTHOLC_SET_BTN, OnCmdSetOtholcBtn)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_ADD_BTN, OnEffLoadAddBtn)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_MOD_BTN, OnEffLoadModBtn)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_DEL_BTN, OnEffLoadDelBtn)
	
	ON_CBN_SELCHANGE(IDC_CMD_AASHTO_LRFD12_PS_CR_SH_CMB, OnSelchangeCmdAASHTOLRFD12PSCRSHCmb)
	ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD12_PS_CR_SH_BTN,  OnCmdAASHTOLRFD12PSCRSHBtn)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()


void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::InitCombo()
{
	m_cmbPSCRSHType.ResetContent();
	m_cmbPSCRSHType.AddString(_T("A"));
	m_cmbPSCRSHType.AddString(_T("B"));
	m_cmbPSCRSHType.AddString(_T("C"));
	m_cmbPSCRSHType.AddString(_T("D"));
	m_cmbPSCRSHType.AddString(_T("E"));
	m_cmbPSCRSHType.SetCurSel(0);

    InitMVCombo();
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::GetAASHTOLRFD12LoadFactor()
{
	UpdateData(TRUE);

	switch(m_nLrfd12DC)  // components and attachments
	{
		case 0: m_nAshLr12DcType = 0; m_dAshLr12Dc1 = 1.25; m_dAshLr12Dc2 = 1.25; break;
		case 1: m_nAshLr12DcType = 1; m_dAshLr12Dc1 = 0.90; m_dAshLr12Dc2 = 0.90; break;
		case 2: m_nAshLr12DcType = 2; m_dAshLr12Dc1 = 1.25; m_dAshLr12Dc2 = 0.90; break;
	}
	switch(m_nLrfd12DDType) // downdrag 
	{
		case 0: // Piles, alpha Tomlinson Method
			if     (m_nLrfd12DD1 == 0) {m_nAshLr12DdType = 0; m_dAshLr12Dd1 = 1.40; m_dAshLr12Dd2 = 1.40;}
			else if(m_nLrfd12DD1 == 1) {m_nAshLr12DdType = 1; m_dAshLr12Dd1 = 0.25; m_dAshLr12Dd2 = 0.25;} 
			else if(m_nLrfd12DD1 == 2) {m_nAshLr12DdType = 2; m_dAshLr12Dd1 = 1.40; m_dAshLr12Dd2 = 0.25;} 
			break;
		case 1: // Piles, gamma Method
			if     (m_nLrfd12DD2 == 0) {m_nAshLr12DdType = 0; m_dAshLr12Dd1 = 1.05; m_dAshLr12Dd2 = 1.05;}
			else if(m_nLrfd12DD2 == 1) {m_nAshLr12DdType = 1; m_dAshLr12Dd1 = 0.30; m_dAshLr12Dd2 = 0.30;} 
			else if(m_nLrfd12DD2 == 2) {m_nAshLr12DdType = 2; m_dAshLr12Dd1 = 1.05; m_dAshLr12Dd2 = 0.30;} 
			break;
		case 2: // Drilled Shafts, O'Neill and Reese (1999) Method
			if     (m_nLrfd12DD3 == 0) {m_nAshLr12DdType = 0; m_dAshLr12Dd1 = 1.25; m_dAshLr12Dd2 = 1.25;}
			else if(m_nLrfd12DD3 == 1) {m_nAshLr12DdType = 1; m_dAshLr12Dd1 = 0.35; m_dAshLr12Dd2 = 0.35;} 
			else if(m_nLrfd12DD3 == 2) {m_nAshLr12DdType = 2; m_dAshLr12Dd1 = 1.25; m_dAshLr12Dd2 = 0.35;} 
			break;
	}
	switch(m_nLrfd12DW)  // wearing surfaces and utilities
	{
		case 0: m_nAshLr12DwType = 0; m_dAshLr12Dw1 = 1.50; m_dAshLr12Dw2 = 1.50; break; 
		case 1: m_nAshLr12DwType = 1; m_dAshLr12Dw1 = 0.65; m_dAshLr12Dw2 = 0.65; break; 
		case 2: m_nAshLr12DwType = 2; m_dAshLr12Dw1 = 1.50; m_dAshLr12Dw2 = 0.65; break;
	}  
	switch(m_nLrfd12EHType)  // horizontal earth pressure
	{
		case 0:   // active
			if     (m_nLrfd12EH1 == 0) {m_nAshLr12EhType = 0; m_dAshLr12Eh1 = 1.50; m_dAshLr12Eh2 = 1.50;} 
			else if(m_nLrfd12EH1 == 1) {m_nAshLr12EhType = 1; m_dAshLr12Eh1 = 0.90; m_dAshLr12Eh2 = 0.90;} 
			else if(m_nLrfd12EH1 == 2) {m_nAshLr12EhType = 2; m_dAshLr12Eh1 = 1.50; m_dAshLr12Eh2 = 0.90;}      
			break;
		case 1:  // at-rest 
			if(m_nLrfd12EH2 == 0)      {m_nAshLr12EhType = 0; m_dAshLr12Eh1 = 1.35; m_dAshLr12Eh2 = 1.35;} 
			else if(m_nLrfd12EH2 == 1) {m_nAshLr12EhType = 1; m_dAshLr12Eh1 = 0.90; m_dAshLr12Eh2 = 0.90;} 
			else if(m_nLrfd12EH2 == 2) {m_nAshLr12EhType = 2; m_dAshLr12Eh1 = 1.35; m_dAshLr12Eh2 = 0.90;}            
			break;
		case 2:  // AEP for anchored walls
			m_nAshLr12EhType = 0; m_dAshLr12Eh1 = 1.35; m_dAshLr12Eh2 = 1.35;
			break;
	}
	m_nAshLr12ElType = 0; m_dAshLr12El1 = 1.00; m_dAshLr12El2 = 1.00;  // locked-in erection stresses    
	switch(m_nLrfd12EVType)
	{
		case 0: // Overall Stability
			m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.00; m_dAshLr12Ev2 = 1.00;      
			break;
		case 1: // Retaining Walls and Abutments
			if     (m_nLrfd12EV2 == 0) {m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.35; m_dAshLr12Ev2 = 1.35;}
			else if(m_nLrfd12EV2 == 1) {m_nAshLr12EvType = 1; m_dAshLr12Ev1 = 1.00; m_dAshLr12Ev2 = 1.00;} 
			else if(m_nLrfd12EV2 == 2) {m_nAshLr12EvType = 2; m_dAshLr12Ev1 = 1.35; m_dAshLr12Ev2 = 1.00;}      
			break;
		case 2:  // Rigid Buried Structure
			if(m_nLrfd12EV3 == 0)      {m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.30; m_dAshLr12Ev2 = 1.30;} 
			else if(m_nLrfd12EV3 == 1) {m_nAshLr12EvType = 1; m_dAshLr12Ev1 = 0.90; m_dAshLr12Ev2 = 0.90;} 
			else if(m_nLrfd12EV3 == 2) {m_nAshLr12EvType = 2; m_dAshLr12Ev1 = 1.30; m_dAshLr12Ev2 = 0.90;}      
			break;
		case 3:  // Rigid Frames
			if(m_nLrfd12EV4 == 0)      {m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.35; m_dAshLr12Ev2 = 1.35;} 
			else if(m_nLrfd12EV4 == 1) {m_nAshLr12EvType = 1; m_dAshLr12Ev1 = 0.90; m_dAshLr12Ev2 = 0.90;} 
			else if(m_nLrfd12EV4 == 2) {m_nAshLr12EvType = 2; m_dAshLr12Ev1 = 1.35; m_dAshLr12Ev2 = 0.90;}      
			break;
		case 4:  // Metal Box Culvers and Structural Culverts with Deep Corrrugations
			if(m_nLrfd12EV5 == 0)      {m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.50; m_dAshLr12Ev2 = 1.50;} 
			else if(m_nLrfd12EV5 == 1) {m_nAshLr12EvType = 1; m_dAshLr12Ev1 = 0.90; m_dAshLr12Ev2 = 0.90;} 
			else if(m_nLrfd12EV5 == 2) {m_nAshLr12EvType = 2; m_dAshLr12Ev1 = 1.50; m_dAshLr12Ev2 = 0.90;}      
			break;
		case 5:  // Thermoplastic culverts
			if(m_nLrfd12EV6 == 0)      {m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.30; m_dAshLr12Ev2 = 1.30;} 
			else if(m_nLrfd12EV6 == 1) {m_nAshLr12EvType = 1; m_dAshLr12Ev1 = 0.90; m_dAshLr12Ev2 = 0.90;} 
			else if(m_nLrfd12EV6 == 2) {m_nAshLr12EvType = 2; m_dAshLr12Ev1 = 1.30; m_dAshLr12Ev2 = 0.90;}      
			break;
		case 6:  // All others
			if(m_nLrfd12EV7 == 0)      {m_nAshLr12EvType = 0; m_dAshLr12Ev1 = 1.95; m_dAshLr12Ev2 = 1.95;} 
			else if(m_nLrfd12EV7 == 1) {m_nAshLr12EvType = 1; m_dAshLr12Ev1 = 0.90; m_dAshLr12Ev2 = 0.90;} 
			else if(m_nLrfd12EV7 == 2) {m_nAshLr12EvType = 2; m_dAshLr12Ev1 = 1.95; m_dAshLr12Ev2 = 0.90;}      
			break;
	}
	switch(m_nLrfd12ES) // earth surcharge
	{
		case 0: m_nAshLr12EsType = 0; m_dAshLr12Es1 = 1.50; m_dAshLr12Es2 = 1.50; break;
		case 1: m_nAshLr12EsType = 1; m_dAshLr12Es1 = 0.75; m_dAshLr12Es2 = 0.75; break;
		case 2: m_nAshLr12EsType = 2; m_dAshLr12Es1 = 1.50; m_dAshLr12Es2 = 0.75; break;
	}  

	int nPSCRSHType = m_cmbPSCRSHType.GetCurSel();

	if(nPSCRSHType == 0) // A : Superstructures - Segmental , Concrete Substructures supporting Segmental, Superstructures 
	{
		m_nAshLr12PsType = 0; m_dAshLr12Ps1 = 1.0; m_dAshLr12Ps2 = 1.0;
		if(m_nLrfd12DC == 0) 
		{
			m_nAshLr12CrType = 0; m_dAshLr12Cr1 = 1.25; m_dAshLr12Cr2 = 1.25;
			m_nAshLr12ShType = 0; m_dAshLr12Sh1 = 1.25; m_dAshLr12Sh2 = 1.25;
		}
		else if(m_nLrfd12DC == 1) 
		{
			m_nAshLr12CrType = 1; m_dAshLr12Cr1 = 0.90; m_dAshLr12Cr2 = 0.90;
			m_nAshLr12ShType = 1; m_dAshLr12Sh1 = 0.90; m_dAshLr12Sh2 = 0.90;
		}
		else if(m_nLrfd12DC == 2) 
		{
			m_nAshLr12CrType = 2; m_dAshLr12Cr1 = 1.25; m_dAshLr12Cr2 = 0.90;
			m_nAshLr12ShType = 2; m_dAshLr12Sh1 = 1.25; m_dAshLr12Sh2 = 0.90;
		}
	}
	else if(nPSCRSHType == 1) // B : Concrete Superstructures - non-segmental
	{
		m_nAshLr12PsType = 0; m_dAshLr12Ps1 = 1.0; m_dAshLr12Ps2 = 1.0;
		m_nAshLr12CrType = 0; m_dAshLr12Cr1 = 1.0; m_dAshLr12Cr2 = 1.0;
		m_nAshLr12ShType = 0; m_dAshLr12Sh1 = 1.0; m_dAshLr12Sh2 = 1.0;
	}
	else if(nPSCRSHType == 2) // C : SubStructures supporting non-segmental Superstructures (using Ig)
	{
		m_nAshLr12PsType = 0; m_dAshLr12Ps1 = 0.5; m_dAshLr12Ps2 = 0.5;
		m_nAshLr12CrType = 0; m_dAshLr12Cr1 = 0.5; m_dAshLr12Cr2 = 0.5;
		m_nAshLr12ShType = 0; m_dAshLr12Sh1 = 0.5; m_dAshLr12Sh2 = 0.5;
	}
	else if(nPSCRSHType == 3) // D : SubStructures supporting non-segmental Superstructures (using Ieffective)
	{
		m_nAshLr12PsType = 0; m_dAshLr12Ps1 = 1.0; m_dAshLr12Ps2 = 1.0;
		m_nAshLr12CrType = 0; m_dAshLr12Cr1 = 1.0; m_dAshLr12Cr2 = 1.0;
		m_nAshLr12ShType = 0; m_dAshLr12Sh1 = 1.0; m_dAshLr12Sh2 = 1.0;
	}
	else if(nPSCRSHType == 4) // E : Steel Substructures
	{
		m_nAshLr12PsType = 0; m_dAshLr12Ps1 = 1.0; m_dAshLr12Ps2 = 1.0;
		m_nAshLr12CrType = 0; m_dAshLr12Cr1 = 1.0; m_dAshLr12Cr2 = 1.0;
		m_nAshLr12ShType = 0; m_dAshLr12Sh1 = 1.0; m_dAshLr12Sh2 = 1.0;
	}
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdAASHTOLRFD12EnableDisable()
{  
	CArray <unsigned int, unsigned int> arMLoad;   // Moving Load
	CArray <unsigned int, unsigned int> arSmLoad;  // Settlement (not static)
	
	CArray<T_STLD_K, T_STLD_K> arKeyList;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
	int iCount = m_pDoc->m_pAttrCtrl->GetCountStld();
	BOOL bDcExist = FALSE;
	BOOL bDdExist = FALSE;
	BOOL bDwExist = FALSE;
	BOOL bEhExist = FALSE;
	BOOL bEvExist = FALSE;
	BOOL bEsExist = FALSE;
	BOOL bLExist  = FALSE;
	BOOL bTExist  = FALSE;  
	BOOL bPsExist = FALSE;
	BOOL bCrExist = FALSE;
	BOOL bShExist = FALSE;
	BOOL bStlExist= FALSE;

	if(iCount!=0)
	{
		for(int i = 0; i < iCount; i++)
		{
			int iKey = arKeyList.GetAt(i);
			T_STLD_D rData;
			rData.Initialize();
			BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
			ASSERT(bCheck);            
			if     (rData.LoadCaseType==_T("D"))	 bDcExist = TRUE;  // General Dead Load
			else if(rData.LoadCaseType==_T("DC"))	 bDcExist = TRUE;  // Component and Attachments
			else if(rData.LoadCaseType==_T("DD"))	 bDdExist = TRUE;  // Downdrag
			else if(rData.LoadCaseType==_T("DW"))	 bDwExist = TRUE;  // Wearing Surfaces and Utilities
			else if(rData.LoadCaseType==_T("EP"))	 bEhExist = TRUE;  // General earth pressure load
			else if(rData.LoadCaseType==_T("EH"))	 bEhExist = TRUE;  // horizontal earth pressure load
			else if(rData.LoadCaseType==_T("EV"))	 bEvExist = TRUE;  // vertical earth pressure load
			else if(rData.LoadCaseType==_T("ES"))	 bEsExist = TRUE;  // Earth Surcharge      
			else if(rData.LoadCaseType==_T("L"))	 bLExist  = TRUE;  // Live Load
			else if(rData.LoadCaseType==_T("T"))	 bTExist  = TRUE;	 // 온도변화 하중
			else if(rData.LoadCaseType==_T("CR"))	 bCrExist = TRUE;  // 크리프 하중
			else if(rData.LoadCaseType==_T("SH"))	 bShExist = TRUE;  // 건조수축 하중
			else if(rData.LoadCaseType==_T("STL")) bStlExist= TRUE;  // 부등침하 하중      
		}
		m_pDoc->m_pAttrCtrl->GetSmlcKeyList(arSmLoad);

		// 코드에 따른 방식으로 수정 
		T_MVCD_D DataMvcd;
		if(!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
		switch (DataMvcd.nCodeType)
		{
		case D_MOVE_CODE_NONE:
			break;
		case D_MOVE_CODE_AASHTO_STAN:
		case D_MOVE_CODE_AASHTO_LRFD:
		case D_MOVE_CODE_TAIWAN:
		case D_MOVE_CODE_KOREA:
		case D_MOVE_CODE_CANADA:
		case D_MOVE_CODE_PENDOT:
		case D_MOVE_CODE_EURO_BS:
		case D_MOVE_CODE_RUSSIA:
		case D_MOVE_CODE_KOREA_LRFD_2011:
		case D_MOVE_CODE_AUSTRALIA:
		case D_MOVE_CODE_POLAND:
		case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
		case D_MOVE_CODE_NEWZEALAND:
		case D_MOVE_CODE_BRAZIL:
			m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);
			break;
		case D_MOVE_CODE_CHINA:
			m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMLoad);
			break;
		case D_MOVE_CODE_JAPAN:
			m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMLoad);
			break;
		case D_MOVE_CODE_INDIA:
			m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMLoad);
			break;
		case D_MOVE_CODE_BS:
			m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(arMLoad);
			break;
		case D_MOVE_CODE_FRANCE:
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(arMLoad);
			break;
		case D_MOVE_CODE_TRANS:
			m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(arMLoad);
			break;
		default:
			ASSERT(0);
			break;
		}
		if(arMLoad.GetSize() > 0)  bLExist = TRUE;  
		if(arSmLoad.GetSize() > 0) bStlExist = TRUE;
	}

	//////////////////////////////////////////////////////////////////
	UpdateData(TRUE);
	if(m_nConstLoad!=0)
	{
		CArray<T_SGLD_K,T_SGLD_K> aSgldK; aSgldK.RemoveAll();
		if(m_pDoc->IsPostMode()) m_pDoc->m_pPostCtrl->GetStageInfo()->GetSgldKeyListPost(aSgldK);
		else                     m_pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldK);

		T_STCT_D StctD; StctD.Initialize();
		if(m_pDoc->m_pAttrCtrl->ExistStct()) m_pDoc->m_pAttrCtrl->GetStct(StctD);
			
		BOOL bCEL = FALSE;
		for(int iKey=0; iKey<aSgldK.GetSize(); iKey++)
		{
			T_SGLD_K SgldK = aSgldK.GetAt(iKey);
			if     (SgldK==D_SGLD_DL_CVL)    { bDcExist = TRUE; }
			else if(SgldK >= D_SGLD_ER1_CVL && SgldK <= D_SGLD_ER20_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER2_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER3_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER4_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER5_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER6_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER7_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER8_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER9_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER10_CVL) { bCEL = TRUE; }
			else if(SgldK==D_SGLD_CP_CVL)    { bCrExist = TRUE; bTExist  = TRUE; }
			else if(SgldK==D_SGLD_CS_CVL)    { bCrExist = TRUE; bTExist  = TRUE; }
			else if(SgldK==D_SGLD_SP_CVL)    { bShExist = TRUE; bTExist  = TRUE; }
			else if(SgldK==D_SGLD_SS_CVL)    { bShExist = TRUE; bTExist  = TRUE; }
		}

		if(bCEL)
		{
			for(int i=0; i<StctD.aErection.GetSize(); i++)
			{
				if     (StctD.aErection[i].LoadCaseType==_T("D"))	bDcExist = TRUE;  // General Dead Load
				else if(StctD.aErection[i].LoadCaseType==_T("DC"))	bDcExist = TRUE;  // Component and Attachments
				else if(StctD.aErection[i].LoadCaseType==_T("DW"))	bDwExist = TRUE;  // Wearing Surfaces and Utilities
				else if(StctD.aErection[i].LoadCaseType==_T("L"))	bLExist  = TRUE;  // Live Load
				else if(StctD.aErection[i].LoadCaseType==_T("T"))	bTExist  = TRUE;  // 온도변화, 크리프, 건조수축 하중
			}			
		}
	}
	//////////////////////////////////////////////////////////////////

	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DC_MAX_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DC_MIN_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DC_BOTH_RDO)->EnableWindow(bDcExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_MAX_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_MIN_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_BOTH_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_MAX_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_MIN_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_BOTH_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_MAX_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_MIN_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_BOTH_RDO)->EnableWindow(bDdExist && m_nLrfd12DDType==2);

	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DW_MAX_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DW_MIN_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DW_BOTH_RDO)->EnableWindow(bDwExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_MAX_RDO)->EnableWindow(bEhExist && m_nLrfd12EHType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_MIN_RDO)->EnableWindow(bEhExist && m_nLrfd12EHType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_BOTH_RDO)->EnableWindow(bEhExist && m_nLrfd12EHType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_MAX_RDO)->EnableWindow(bEhExist && m_nLrfd12EHType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_MIN_RDO)->EnableWindow(bEhExist && m_nLrfd12EHType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_BOTH_RDO)->EnableWindow(bEhExist && m_nLrfd12EHType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH3_RDO)->EnableWindow(bEhExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV1_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==6);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==6);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd12EVType==6);

	GetDlgItem(IDC_CMD_AASHTO_LRFD12_ES_MAX_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_ES_MIN_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_ES_BOTH_RDO)->EnableWindow(bEsExist);

	if(!bPsExist && !bCrExist && !bShExist)
	{
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_CR_SH_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_CR_SH_BTN)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_AASHTO_LRFD12_CR_SH_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_CR_SH_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_CR_SH_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_CR_SH_BTN)->EnableWindow(TRUE);

		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_TEXT)->EnableWindow(bPsExist);
		GetDlgItem(IDC_CMD_AASHTO_LRFD12_PS_EDIT)->EnableWindow(bPsExist);

		if(!bCrExist && !bShExist)
		{
			GetDlgItem(IDC_CMD_AASHTO_LRFD12_CR_SH_TEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_AASHTO_LRFD12_CR_SH_EDIT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CMD_AASHTO_LRFD12_CR_SH_TEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_AASHTO_LRFD12_CR_SH_EDIT)->EnableWindow(TRUE);
		}
	}
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_SETTLEMENT_EDT)->EnableWindow(bStlExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_CULVERT_CHK)->EnableWindow(bLExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DEFORMATION_RDO)->EnableWindow(bTExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EFFECT_RDO)->EnableWindow(bTExist);
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdAASHTOLRFDEnableDisable(BOOL bAlignCtrl)
{
    OnCmdAASHTOLRFD12EnableDisable();

    if (!bAlignCtrl) return;

	BOOL bEnable = (m_bAASHTOLrfd16 || m_bAASHTOLrfd18 || m_bAASHTOLrfd20 || m_bAASHTOLrfd24);

    GetDlgItem(IDC_CMD_AASHTO_LRFD16_GAMMALL_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_CMD_AASHTO_LRFD16_GAMMALL_EDT)->EnableWindow(bEnable);

    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_CHK)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_FRAME)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_GRP)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_STC)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_CMB)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_STC)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_EDT)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LST)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_ADD_BTN)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_MOD_BTN)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_DEL_BTN)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHO_CHK)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHO_LCB_FRAME)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHOLC_SET_BTN)->EnableWindow(bEnable);
    OnCmdSeisChk();
    OnCmdOthoChk();

    AlignControl();
}

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD12Dlg message handlers
BOOL CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

    // 여러개인 경우 초기화 먼저 해야함.
	m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::IDD, IDC_CMD_AASHTO_LRFD12_LOAD_FACTOR_GRP,TRUE,TRUE);
    //m_chkSeisLoad.InitControl(this, CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::IDD, IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_FRAME, TRUE, TRUE);
    // 이것들은 모두 초기화된 다음에
    m_chkLoadFactor.SetFoldCurHeight(TRUE);
    //m_chkSeisLoad.SetFoldCurHeight(TRUE);
    //m_chkSeisLoad.SetFoldState(TRUE);
    
	m_edtLrfd12Modifier.SetEditUnit(1.0);
	m_edtLrfd12PS.SetEditUnit(1.0);
	m_edtLrfd12CRSH.SetEditUnit(0.0);
	m_edtLrfd12Settlement.SetEditUnit(1.0);
	m_edtLrfd16_GammaLL.SetEditUnit(0.8);
    m_wndSeisChk.SetCheck(0);
    m_wndOthoChk.SetCheck(0);

	InitCombo();

    CString aTitle_eff[] ={_LS(IDS_WG_CMD__ADDD__Load_Case), _LS(IDS_WG_CMD__ADDD__Factor)};
    int nColWidth_eff[] ={105, 70};
    SetListCtrlHeader(m_lstEffLoad, 2, aTitle_eff, nColWidth_eff);

	UpdateData(FALSE);

	OnCmdAASHTOLRFD12DDTypeRdo();
	OnCmdAASHTOLRFD12EHTypeRdo();
	OnCmdAASHTOLRFD12EVTypeRdo();
	OnCmdAASHTOLRFDEnableDisable();   // Load Case의 존재 여부에 따라 Ctrl을 활성화 및 비활성화

	OnSelchangeCmdAASHTOLRFD12PSCRSHCmb();

	return TRUE;
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::SetListCtrlHeader(CListCtrl& list, const int nColNum, const CString* aTitle, const int* nColWidth)
{
    // set full row select mode
    DWORD dwStyle;
    dwStyle = ListView_GetExtendedListViewStyle(list.GetSafeHwnd());
    dwStyle |= LVS_EX_FULLROWSELECT;
    ListView_SetExtendedListViewStyle(list.GetSafeHwnd(), dwStyle);

    // Set Title
    for (int i = 0; i<nColNum; i++)
    {
        CString title = aTitle[i];
        LV_COLUMN lvcolumn;
        lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
        lvcolumn.fmt = i==IDX_NAME ? LVCFMT_LEFT : LVCFMT_CENTER;

        lvcolumn.iSubItem = i;
        lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
        lvcolumn.pszText = title.GetBuffer(0);
        list.InsertColumn(i, &lvcolumn);
        list.SetColumn(i, &lvcolumn);
        title.ReleaseBuffer();
    }
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::InitMVCombo()
{
    m_cmbEffLoadCase.ResetContent();
    //m_cmbEffLoadCase.AddString(_LS(IDS_WG_CMD__ADDD__None));

    CArray<T_MVLD_K, T_MVLD_K> KeyList; KeyList.RemoveAll();
    CArray<T_MVLDjp_K, T_MVLDjp_K> KeyListjp; KeyListjp.RemoveAll();
    CArray<T_MVLDch_K, T_MVLDch_K> KeyListch; KeyListch.RemoveAll();
    CArray<T_MVLDid_K, T_MVLDid_K> KeyListid; KeyListid.RemoveAll();
    CArray<T_MVLDbs_K, T_MVLDbs_K> KeyListbs; KeyListbs.RemoveAll();
	CArray<T_MVLDfr_K, T_MVLDfr_K> KeyListfr; KeyListfr.RemoveAll();
    CArray<T_MVLDtr_K, T_MVLDtr_K> KeyListtr; KeyListtr.RemoveAll();

    // 코드에 따른 방식으로 수정 
    int nCount = 0;
    T_MVCD_D DataMvcd; DataMvcd.Initialize();
    if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();

    switch (DataMvcd.nCodeType)
    {
        case D_MOVE_CODE_NONE:
            break;
        case D_MOVE_CODE_AASHTO_STAN:
        case D_MOVE_CODE_AASHTO_LRFD:
        case D_MOVE_CODE_TAIWAN:
        case D_MOVE_CODE_KOREA:
        case D_MOVE_CODE_CANADA:
        case D_MOVE_CODE_PENDOT:
        case D_MOVE_CODE_EURO_BS:
        case D_MOVE_CODE_RUSSIA:
        case D_MOVE_CODE_KOREA_LRFD_2011:
        case D_MOVE_CODE_AUSTRALIA:
        case D_MOVE_CODE_POLAND:
        case D_MOVE_CODE_SOUTH_AFRICA:
		case D_MOVE_CODE_JAPAN_RAIL:
        case D_MOVE_CODE_NEWZEALAND:
        case D_MOVE_CODE_BRAZIL:
            m_pDoc->m_pAttrCtrl->GetMvldKeyList(KeyList);      nCount = KeyList.GetSize();
            break;
        case D_MOVE_CODE_CHINA:
            m_pDoc->m_pAttrCtrl->GetMvldchKeyList(KeyListch);  nCount = KeyListch.GetSize();
            break;
        case D_MOVE_CODE_JAPAN:
            m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(KeyListjp);  nCount = KeyListjp.GetSize();
            break;
        case D_MOVE_CODE_INDIA:
            m_pDoc->m_pAttrCtrl->GetMvldidKeyList(KeyListid);  nCount = KeyListid.GetSize();
            break;
        case D_MOVE_CODE_BS:
            m_pDoc->m_pAttrCtrl->GetMvldbsKeyList(KeyListbs);  nCount = KeyListbs.GetSize();
            break;
		case D_MOVE_CODE_FRANCE:
			m_pDoc->m_pAttrCtrl->GetMvldfrKeyList(KeyListfr);  nCount = KeyListfr.GetSize();
			break;
        case D_MOVE_CODE_TRANS:
            m_pDoc->m_pAttrCtrl->GetMvldtrKeyList(KeyListtr);  nCount = KeyListtr.GetSize();
            break;
        default:
            ASSERT(0);
            break;
    }

    for (int i=0; i<nCount; i++)
    {
        T_MVLD_D rData; rData.Initialize();
        T_MVLDjp_D rDatajp; m_pDoc->m_pAttrCtrl->InitializeMvldjp(rDatajp);
        T_MVLDch_D rDatach; rDatach.Initialize();
        T_MVLDid_D rDataid; rDataid.Initialize();
        T_MVLDbs_D rDatabs; rDatabs.Initialize();
		T_MVLDfr_D rDatafr; rDatafr.Initialize();
        T_MVLDtr_D rDatatr; rDatatr.Initialize();

        T_MVCD_D DataMvcd;
        if (!m_pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
        CString strLoadCase = _T("");
        switch (DataMvcd.nCodeType)
        {
            case D_MOVE_CODE_NONE:
                break;
            case D_MOVE_CODE_AASHTO_STAN:
            case D_MOVE_CODE_AASHTO_LRFD:
            case D_MOVE_CODE_TAIWAN:
            case D_MOVE_CODE_KOREA:
            case D_MOVE_CODE_CANADA:
            case D_MOVE_CODE_PENDOT:
            case D_MOVE_CODE_EURO_BS:
            case D_MOVE_CODE_RUSSIA:
            case D_MOVE_CODE_KOREA_LRFD_2011:
            case D_MOVE_CODE_AUSTRALIA:
            case D_MOVE_CODE_POLAND:
            case D_MOVE_CODE_SOUTH_AFRICA:
			case D_MOVE_CODE_JAPAN_RAIL:
            case D_MOVE_CODE_NEWZEALAND:
            case D_MOVE_CODE_BRAZIL:
            {
                if (!m_pDoc->m_pAttrCtrl->GetMvld(KeyList.GetAt(i), rData)) ASSERT(0);
                strLoadCase = rData.LoadCaseName;
            }
            break;
            case D_MOVE_CODE_CHINA:
            {
                if (!m_pDoc->m_pAttrCtrl->GetMvldch(KeyListch.GetAt(i), rDatach)) ASSERT(0);
                strLoadCase = rDatach.LoadCaseName;
            }
            break;
            case D_MOVE_CODE_JAPAN:
            {
                if (!m_pDoc->m_pAttrCtrl->GetMvldjp(KeyListjp.GetAt(i), rDatajp)) ASSERT(0);
                strLoadCase = rDatajp.LoadCaseName;
            }
            break;
            case D_MOVE_CODE_INDIA:
            {
                if (!m_pDoc->m_pAttrCtrl->GetMvldid(KeyListid.GetAt(i), rDataid)) ASSERT(0);
                strLoadCase = rDataid.LoadCaseName;
            }
            break;
            case D_MOVE_CODE_BS:
            {
                if (!m_pDoc->m_pAttrCtrl->GetMvldbs(KeyListbs.GetAt(i), rDatabs)) ASSERT(0);
                strLoadCase = rDatabs.LoadCaseName;
            }
            break;
			case D_MOVE_CODE_FRANCE:
			{
				if (!m_pDoc->m_pAttrCtrl->GetMvldfr(KeyListfr.GetAt(i), rDatafr)) ASSERT(0);
				strLoadCase = rDatabs.LoadCaseName;
			}
			break;
            case D_MOVE_CODE_TRANS:
            {
                if (!m_pDoc->m_pAttrCtrl->GetMvldtr(KeyListtr.GetAt(i), rDatatr)) ASSERT(0);
                strLoadCase = rDatatr.LoadCaseName;
            }
            break;
            default:
                ASSERT(0);
                break;
        }
        m_cmbEffLoadCase.AddString(strLoadCase);
        m_arrEff_MVData.Add(strLoadCase); // MV Type 저장
    }

	//moving load를 LiveLoad로 대체해서 사용하는 경우에 대한 처리
	if ( nCount == 0 )
    {
        CArray<T_STLD_K, T_STLD_K> arKeyList;
        CArray<T_STLD_K, T_STLD_K> aLL;
        m_pDoc->m_pAttrCtrl->GetStldKeyList(arKeyList);
        int iCountStld = arKeyList.GetSize();
        for ( int i = 0; i < iCountStld; i++ )
        {
            int iKey = arKeyList.GetAt(i);
            T_STLD_D rData;
            rData.Initialize();
            BOOL bCheck = m_pDoc->m_pAttrCtrl->GetStld(iKey, rData);
            if ( rData.LoadCaseType==_T("L") )
            {
                m_cmbEffLoadCase.AddString(rData.LoadCaseName);
                m_arrEff_LLData.Add(rData.LoadCaseName);
            }
        }
    }

    m_cmbEffLoadCase.SetCurSel(0);
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdAASHTOLRFD12DDTypeRdo()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_MAX_RDO)->EnableWindow(m_nLrfd12DDType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_MIN_RDO)->EnableWindow(m_nLrfd12DDType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD1_BOTH_RDO)->EnableWindow(m_nLrfd12DDType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_MAX_RDO)->EnableWindow(m_nLrfd12DDType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_MIN_RDO)->EnableWindow(m_nLrfd12DDType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD2_BOTH_RDO)->EnableWindow(m_nLrfd12DDType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_MAX_RDO)->EnableWindow(m_nLrfd12DDType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_MIN_RDO)->EnableWindow(m_nLrfd12DDType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_DD3_BOTH_RDO)->EnableWindow(m_nLrfd12DDType==2);
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdAASHTOLRFD12EHTypeRdo()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_MAX_RDO)->EnableWindow(m_nLrfd12EHType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_MIN_RDO)->EnableWindow(m_nLrfd12EHType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH1_BOTH_RDO)->EnableWindow(m_nLrfd12EHType==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_MAX_RDO)->EnableWindow(m_nLrfd12EHType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_MIN_RDO)->EnableWindow(m_nLrfd12EHType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EH2_BOTH_RDO)->EnableWindow(m_nLrfd12EHType==1);
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdAASHTOLRFD12EVTypeRdo() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_MAX_RDO)->EnableWindow(m_nLrfd12EVType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_MIN_RDO)->EnableWindow(m_nLrfd12EVType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV2_BOTH_RDO)->EnableWindow(m_nLrfd12EVType==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_MAX_RDO)->EnableWindow(m_nLrfd12EVType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_MIN_RDO)->EnableWindow(m_nLrfd12EVType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV3_BOTH_RDO)->EnableWindow(m_nLrfd12EVType==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_MAX_RDO)->EnableWindow(m_nLrfd12EVType==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_MIN_RDO)->EnableWindow(m_nLrfd12EVType==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV4_BOTH_RDO)->EnableWindow(m_nLrfd12EVType==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_MAX_RDO)->EnableWindow(m_nLrfd12EVType==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_MIN_RDO)->EnableWindow(m_nLrfd12EVType==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV5_BOTH_RDO)->EnableWindow(m_nLrfd12EVType==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_MAX_RDO)->EnableWindow(m_nLrfd12EVType==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_MIN_RDO)->EnableWindow(m_nLrfd12EVType==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV6_BOTH_RDO)->EnableWindow(m_nLrfd12EVType==5);		
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_MAX_RDO)->EnableWindow(m_nLrfd12EVType==6);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_MIN_RDO)->EnableWindow(m_nLrfd12EVType==6);
	GetDlgItem(IDC_CMD_AASHTO_LRFD12_EV7_BOTH_RDO)->EnableWindow(m_nLrfd12EVType==6);		
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnSelchangeCmdAASHTOLRFD12PSCRSHCmb()
{
	UpdateData(TRUE);
	
	int nIndex = m_cmbPSCRSHType.GetCurSel();
	
	if(nIndex == 0) // A : Superstructures - Segmental , Concrete Substructures supporting Segmental, Superstructures 
	{
		m_edtLrfd12PS.SetEditUnit(1.0);
		m_edtLrfd12CRSH.SetEditUnit(0.0);
	}
	else if(nIndex == 1) // B : Concrete Superstructures - non-segmental
	{
		m_edtLrfd12PS.SetEditUnit(1.0);
		m_edtLrfd12CRSH.SetEditUnit(1.0);
	} 
	else if(nIndex == 2) // C : SubStructures supporting non-segmental Superstructures (using Ig)
	{
		m_edtLrfd12PS.SetEditUnit(0.5);
		m_edtLrfd12CRSH.SetEditUnit(0.5);
	}
	else if(nIndex == 3) // D : SubStructures supporting non-segmental Superstructures (using Ieffective)
	{
		m_edtLrfd12PS.SetEditUnit(1.0);
		m_edtLrfd12CRSH.SetEditUnit(1.0);
	}
	else // E : Steel Substructures
	{
		m_edtLrfd12PS.SetEditUnit(1.0);
		m_edtLrfd12CRSH.SetEditUnit(1.0);
	}
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdAASHTOLRFD12PSCRSHBtn()
{
	CCmdAutoLoadCombCvlHelpDlg dlg;
	dlg.DoModal();
}

LRESULT CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->AlignControl();

	return 0L;
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdSetOtholcBtn()
{
    CCmdSetOrthoLC_C3_Dlg dlg;
    dlg.SetData(m_nOrthoCombType, m_a100_30_Load);
    if (dlg.DoModal()==IDOK)
    {
        dlg.GetData(m_nOrthoCombType, m_a100_30_Load);
    }
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdSeisChk()
{
    BOOL bCheck = m_wndSeisChk.GetCheck();
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_GRP)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_STC)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_CMB)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_STC)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_EDT)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LST)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_ADD_BTN)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_MOD_BTN)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_DEL_BTN)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHO_CHK)->EnableWindow(bCheck);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHO_LCB_FRAME)->EnableWindow(bCheck);
    if (bCheck) OnCmdOthoChk();
    else
    {
        GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHOLC_SET_BTN)->EnableWindow(bCheck);
    }
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnCmdOthoChk()
{
    BOOL bCheck = m_wndOthoChk.GetCheck();
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_OTHOLC_SET_BTN)->EnableWindow(bCheck);

}

BOOL CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::CheckMovingCaseName(CString str, int& Index)
{
    BOOL bCheck=TRUE;
    int Count = m_lstEffLoad.GetItemCount();
    for (int i = 0; i < Count; i++)
    {
        CString strLcName = m_lstEffLoad.GetItemText(i, 0);
        if (str == strLcName)
        {
            Index = i;
            bCheck = FALSE;
        }
    }
    return bCheck;
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnEffLoadAddBtn()
{
    UpdateData();
    int Index = m_cmbEffLoadCase.GetCurSel();
    if (Index != -1)
    {
        int ListNo = 0;
        CString strName=_T("");
        m_cmbEffLoadCase.GetLBText(Index, strName);
        if (CheckMovingCaseName(strName, ListNo))
        {
            // 리스트에 있나?
            LVFINDINFO FindInfo;
            FindInfo.flags = LVFI_STRING;
            FindInfo.psz = strName;
            int nItem = m_lstEffLoad.FindItem(&FindInfo);
            if (nItem != -1)
                return;

            LV_ITEM lvitem;
            lvitem.mask = LVIF_TEXT | LVFI_STRING;
            lvitem.iItem  	= m_lstEffLoad.GetItemCount();
            lvitem.iSubItem = IDX_NAME;
            lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
            m_lstEffLoad.InsertItem(&lvitem);

            lvitem.iSubItem = IDX_FACTOR;
            lvitem.mask	= LVIF_TEXT;
            CString strFactor;
            strFactor.Format(_T("%g"), m_dEffLoadFactor);
            lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
            m_lstEffLoad.SetItem(&lvitem);
        }
        else
        {
            CString msg;
            msg.Format(_LS(IDS_WG_CMD__ADDD__Loadcase__s_already_exist_), strName);
            AfxMessageBox(msg);
            return;
        }
    }
    else AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_item_in_the_list__nCh));
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnEffLoadModBtn()
{
    UpdateData();
    int Index = m_cmbEffLoadCase.GetCurSel();
    if (Index != -1) return;

    int ListNo = 0;
    CString strName=_T("");
    m_cmbEffLoadCase.GetLBText(Index, strName);

    // 리스트에 있나?
    LVFINDINFO FindInfo;
    FindInfo.flags = LVFI_STRING;
    FindInfo.psz = strName;
    int iItem = m_lstEffLoad.FindItem(&FindInfo);
    if (iItem == -1)
        return;

    LV_ITEM lvitem;
    lvitem.mask = LVIF_TEXT | LVFI_STRING;
    lvitem.iItem  	= iItem;
    lvitem.iSubItem = IDX_NAME;
    lvitem.pszText	= (LPTSTR)(LPCTSTR)strName;
    m_lstEffLoad.SetItem(&lvitem);

    lvitem.iSubItem = IDX_FACTOR;
    lvitem.mask	= LVIF_TEXT;
    CString strFactor;
    strFactor.Format(_T("%g"), m_dEffLoadFactor);
    lvitem.pszText	= (LPTSTR)(LPCTSTR)strFactor;
    m_lstEffLoad.SetItem(&lvitem);
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::OnEffLoadDelBtn()
{
    int iItem = m_lstEffLoad.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
    if (iItem == -1)
    {
        AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___No_selected_item_to_delet));
        return;
    }
    m_lstEffLoad.DeleteItem(iItem);
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::Get_LiveLoadData(CArray<_LCOM_GRAV_EFF_MV, _LCOM_GRAV_EFF_MV&>& aEffectiveMVLoad)
{
    aEffectiveMVLoad.RemoveAll();

    for (int i=0; i<m_lstEffLoad.GetItemCount(); i++)
    {
        CString strStldName = m_lstEffLoad.GetItemText(i, IDX_NAME);
		if ( IsEff_MV_Type(strStldName) )
		{
			_LCOM_GRAV_EFF_MV gEff_MV;
			gEff_MV.MvldK = m_pDoc->m_pAttrCtrl->GetMvldKeyNew(strStldName);
			gEff_MV.dFactor = _ttof(m_lstEffLoad.GetItemText(i, IDX_FACTOR));
			aEffectiveMVLoad.Add(gEff_MV);
		}
		else if ( IsEff_LL_Type(strStldName) )
		{
			continue;
		}
		else ASSERT(0); //여기 들어오면 절대안됨
    }
}

void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::Get_LiveLoadData(CArray<_LCOM_GRAV_EFF, _LCOM_GRAV_EFF&>& aEffectiveLLLoad)
{
	aEffectiveLLLoad.RemoveAll();

    for ( int i=0; i<m_lstEffLoad.GetItemCount(); i++ )
    {
        CString strStldName = m_lstEffLoad.GetItemText(i, IDX_NAME);
		if ( IsEff_LL_Type(strStldName) )
		{
			_LCOM_GRAV_EFF gEff_MV;
			gEff_MV.StldK = m_pDoc->m_pAttrCtrl->GetStldKey(strStldName);
			gEff_MV.dFactor = _ttof(m_lstEffLoad.GetItemText(i, IDX_FACTOR));
			aEffectiveLLLoad.Add(gEff_MV);
		}
		else if ( IsEff_MV_Type(strStldName) )
			continue;
        else ASSERT(0); //여기 들어오면 절대안됨
    }
}

BOOL CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::IsEff_MV_Type(CString str)
{
    BOOL bMV_Type = FALSE;
    for (int i = 0; i < m_arrEff_MVData.GetCount(); ++i)
    {
        CString strMVData = m_arrEff_MVData.GetAt(i);
        if (str == strMVData)
        {
            bMV_Type = TRUE;
        }
    }
    return bMV_Type;
}

BOOL CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::IsEff_LL_Type(CString str)
{
    BOOL bLL_Type = FALSE;
    for ( int i = 0; i < m_arrEff_LLData.GetCount(); ++i )
    {
        CString strLLData = m_arrEff_LLData.GetAt(i);
        if ( str == strLLData )
        {
            bLL_Type = TRUE;
        }
    }
    return bLL_Type;
}


#include "..\wg_base\wg_base_DlgUtil.h"
void CCmdAutoLoadCombCvlAASHTOLRFD12Dlg::AlignControl()
{
    BOOL bShow = m_bAASHTOLrfd16 || m_bAASHTOLrfd18 || m_bAASHTOLrfd20 || m_bAASHTOLrfd24;

    CArray<UINT,UINT> aCtrlID_SeisHead;
    aCtrlID_SeisHead.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_CHK);
    aCtrlID_SeisHead.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_FRAME);

    CArray<UINT,UINT> aCtrlID_Seis;
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_CHK);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_FRAME);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_GRP);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_STC);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LC_CMB);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_STC);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LF_EDT);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_LST);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_ADD_BTN);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_MOD_BTN);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_LL_DEL_BTN);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_OTHO_CHK);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_OTHO_LCB_FRAME);
    aCtrlID_Seis.Add(IDC_CMD_AASHTO_LRFD12_OTHOLC_SET_BTN);

    CArray<UINT,UINT> aCtrlID_Move;
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD12_SETTLEMENT_TEXT);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD12_SETTLEMENT_EDT);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD12_CULVERT_CHK);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD16_GAMMALL_STC);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD16_GAMMALL_EDT);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD12_DEFORMATION_FRAME);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD12_DEFORMATION_RDO);
    aCtrlID_Move.Add(IDC_CMD_AASHTO_LRFD12_EFFECT_RDO);

    CRect Pos1, Pos2, refPos;
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_LOAD_FACTOR_GRP)->GetWindowRect(Pos1);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SEIS_LCOM_FRAME)->GetWindowRect(Pos2);
    GetDlgItem(IDC_CMD_AASHTO_LRFD12_SETTLEMENT_TEXT)->GetWindowRect(refPos);
    ScreenToClient(Pos1);
    ScreenToClient(Pos2);
    ScreenToClient(refPos);

    const int nMove = Pos2.bottom - Pos1.bottom;    
    if (bShow)
    {
        if (refPos.top < Pos2.bottom)
        {
            CDlgUtil::CtrlMoveDistY(this, aCtrlID_Move, nMove);
        }
        //if (m_chkSeisLoad.GetFoldState())
        //    CDlgUtil::CtrlShowHide(this, aCtrlID_SeisHead, bShow);
        //else
            CDlgUtil::CtrlShowHide(this, aCtrlID_Seis, bShow);
    }
    else
    {   
        if (refPos.top > Pos2.bottom)
        {
            CDlgUtil::CtrlMoveDistY(this, aCtrlID_Move, -nMove);
        }
        CDlgUtil::CtrlShowHide(this, aCtrlID_Seis, bShow);
    }
    CDlgUtil::ResizeDlgByBotItem(this, NULL, IDC_CMD_AASHTO_LRFD12_DEFORMATION_FRAME, globalUtils.ScaleByDPI(7));

    // 화면 잔상 문제 제거
    Invalidate(TRUE);
}