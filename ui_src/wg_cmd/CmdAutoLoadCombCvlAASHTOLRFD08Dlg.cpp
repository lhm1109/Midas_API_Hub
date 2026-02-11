// CmdAutoLoadCombCvlAASHTOLRFD08Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlAASHTOLRFD08Dlg.h"
#include "CmdAutoLoadCombCvlDlg.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"
#include "LoadCombCtrl.h"


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
// CCmdAutoLoadCombCvlAASHTOLRFD08Dlg dialog
CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::CCmdAutoLoadCombCvlAASHTOLRFD08Dlg(CWnd* pParent /*=NULL*/)
    : CChildDialog(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg)
    //}}AFX_DATA_INIT
    m_pDoc = CDBDoc::GetDocPoint();

    m_pParent = (CCmdAutoLoadCombCvlDlg*)pParent;

    m_nConstLoad = 0;

    // AASHTO LRFD 08
    m_nLrfd08DCRdo = 0;
    m_nLrfd08DDTypeRdo = 0;
    m_nLrfd08DD1Rdo = 0;
    m_nLrfd08DD2Rdo = 0;
    m_nLrfd08DD3Rdo = 0;
    m_nLrfd08DWRdo = 0;
    m_nLrfd08EHTypeRdo = 0;
    m_nLrfd08EH1Rdo = 0;
    m_nLrfd08EH2Rdo = 0;
    m_nLrfd08EVTypeRdo = 0;
    m_nLrfd08EV2Rdo = 0;
    m_nLrfd08EV3Rdo = 0;
    m_nLrfd08EV4Rdo = 0;
    m_nLrfd08EV5Rdo = 0;
    m_nLrfd08EV6Rdo = 0;
    m_nLrfd08ESRdo = 0;
    m_nLrfd08DeformationRdo = 1;

}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::DoDataExchange(CDataExchange* pDX)
{
    CChildDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg)  

    // AASHTO LRFD 08
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_LOAD_MODIFIER_EDIT, m_dLrfd08ModifierEdt);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DC_MAX_RDO,         m_nLrfd08DCRdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DD1_RDO,            m_nLrfd08DDTypeRdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DD1_MAX_RDO,        m_nLrfd08DD1Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DD2_MAX_RDO,        m_nLrfd08DD2Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DD3_MAX_RDO,        m_nLrfd08DD3Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DW_MAX_RDO,         m_nLrfd08DWRdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EH1_RDO,            m_nLrfd08EHTypeRdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EH1_MAX_RDO,        m_nLrfd08EH1Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EH2_MAX_RDO,        m_nLrfd08EH2Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EV1_RDO,            m_nLrfd08EVTypeRdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EV2_MAX_RDO,        m_nLrfd08EV2Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EV3_MAX_RDO,        m_nLrfd08EV3Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EV4_MAX_RDO,        m_nLrfd08EV4Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EV5_MAX_RDO,        m_nLrfd08EV5Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_EV6_MAX_RDO,        m_nLrfd08EV6Rdo);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_ES_MAX_RDO,         m_nLrfd08ESRdo);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_PS_CR_SH_CMB,       m_nPSCRSHTypeCmb);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_PS_EDIT,            m_dLrfd08PSEdt);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_CR_SH_EDIT,         m_dLrfd08CRSHEdt);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_SETTLEMENT_EDIT,    m_dLrfd08SettlementEdt);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_CULVERT_CHK,        m_bLrfd08CulvertChk);
    DDX_Radio  (pDX, IDC_CMD_AASHTO_LRFD08_DEFORMATION_RDO,    m_nLrfd08DeformationRdo);
    DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_LOAD_FACTOR_FRAME,  m_chkLoadFactor);


    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg, CChildDialog)
    //{{AFX_MSG_MAP(CCmdAutoLoadCombCvlAASHTOLRFD08Dlg)

    // AASHTO LRFD 08
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_DD1_RDO,  OnCmdAASHTOLRFD08DDTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_DD2_RDO,  OnCmdAASHTOLRFD08DDTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_DD3_RDO,  OnCmdAASHTOLRFD08DDTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EH1_RDO,  OnCmdAASHTOLRFD08EHTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EH2_RDO,  OnCmdAASHTOLRFD08EHTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EH3_RDO,  OnCmdAASHTOLRFD08EHTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EV1_RDO,  OnCmdAASHTOLRFD08EVTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EV2_RDO,  OnCmdAASHTOLRFD08EVTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EV3_RDO,  OnCmdAASHTOLRFD08EVTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EV4_RDO,  OnCmdAASHTOLRFD08EVTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EV5_RDO,  OnCmdAASHTOLRFD08EVTypeRdo)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_EV6_RDO,  OnCmdAASHTOLRFD08EVTypeRdo)

    ON_CBN_SELCHANGE(IDC_CMD_AASHTO_LRFD08_PS_CR_SH_CMB, OnSelchangeCmdAASHTOLRFD08PSCRSHCmb)
    ON_BN_CLICKED(IDC_CMD_AASHTO_LRFD08_PS_CR_SH_BTN,  OnCmdAASHTOLRFD08PSCRSHBtn)


    //}}AFX_MSG_MAP

    ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlAASHTOLRFD12Dlg message handlers
BOOL CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnInitDialog() 
{
    CChildDialog::OnInitDialog();

    m_chkLoadFactor.InitControl(this, CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::IDD, IDC_CMD_AASHTO_LRFD08_LOAD_FACTOR_FRAME,TRUE,TRUE);

    // AASHTO LRFD 08
    m_dLrfd08ModifierEdt.SetEditUnit(1.0);
    m_dLrfd08PSEdt.SetEditUnit(1.0);
    m_dLrfd08CRSHEdt.SetEditUnit(0.0);
    m_dLrfd08SettlementEdt.SetEditUnit(1.0);

    InitCombo();

    UpdateData(FALSE);

    OnCmdAASHTOLRFD08DDTypeRdo();
    OnCmdAASHTOLRFD08EHTypeRdo();
    OnCmdAASHTOLRFD08EVTypeRdo();
    OnCmdAASHTOLRFD08EnableDisable();

    OnSelchangeCmdAASHTOLRFD08PSCRSHCmb();

    return TRUE;
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::InitCombo()
{
    m_nPSCRSHTypeCmb.ResetContent();
    m_nPSCRSHTypeCmb.AddString(_T("A"));
    m_nPSCRSHTypeCmb.AddString(_T("B"));
    m_nPSCRSHTypeCmb.AddString(_T("C"));
    m_nPSCRSHTypeCmb.AddString(_T("D"));
    m_nPSCRSHTypeCmb.AddString(_T("E"));
    m_nPSCRSHTypeCmb.SetCurSel(0);
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::GetAASHTOLRFD08LoadFactor()
{
	UpdateData(TRUE);

	switch(m_nLrfd08DCRdo)  // components and attachments
	{
	case 0: m_nAshLr08DcNum = 1; m_dAshLr08Dc1 = 1.25; m_dAshLr08Dc2 = 1.25; break; 
	case 1: m_nAshLr08DcNum = 1; m_dAshLr08Dc1 = 0.90; m_dAshLr08Dc2 = 0.90; break; 
	case 2: m_nAshLr08DcNum = 2; m_dAshLr08Dc1 = 1.25; m_dAshLr08Dc2 = 0.90; break;
	}
	switch(m_nLrfd08DDTypeRdo) // downdrag 
	{
	case 0: // Piles, alpha Tomlinson Method
		if     (m_nLrfd08DD1Rdo == 0) {m_nAshLr08DdNum = 1; m_dAshLr08Dd1 = 1.40; m_dAshLr08Dd2 = 1.40;}
		else if(m_nLrfd08DD1Rdo == 1) {m_nAshLr08DdNum = 1; m_dAshLr08Dd1 = 0.25; m_dAshLr08Dd2 = 0.25;} 
		else if(m_nLrfd08DD1Rdo == 2) {m_nAshLr08DdNum = 2; m_dAshLr08Dd1 = 1.40; m_dAshLr08Dd2 = 0.25;} 
		break;
	case 1: // Piles, gamma Method
		if     (m_nLrfd08DD2Rdo == 0) {m_nAshLr08DdNum = 1; m_dAshLr08Dd1 = 1.05; m_dAshLr08Dd2 = 1.05;}
		else if(m_nLrfd08DD2Rdo == 1) {m_nAshLr08DdNum = 1; m_dAshLr08Dd1 = 0.30; m_dAshLr08Dd2 = 0.30;} 
		else if(m_nLrfd08DD2Rdo == 2) {m_nAshLr08DdNum = 2; m_dAshLr08Dd1 = 1.05; m_dAshLr08Dd2 = 0.30;} 
		break;
	case 2: // Drilled Shafts, O'Neill and Reese (1999) Method
		if     (m_nLrfd08DD3Rdo == 0) {m_nAshLr08DdNum = 1; m_dAshLr08Dd1 = 1.25; m_dAshLr08Dd2 = 1.25;}
		else if(m_nLrfd08DD3Rdo == 1) {m_nAshLr08DdNum = 1; m_dAshLr08Dd1 = 0.35; m_dAshLr08Dd2 = 0.35;} 
		else if(m_nLrfd08DD3Rdo == 2) {m_nAshLr08DdNum = 2; m_dAshLr08Dd1 = 1.25; m_dAshLr08Dd2 = 0.35;} 
		break;
	}
	switch(m_nLrfd08DWRdo)  // wearing surfaces and utilities
	{
	case 0: m_nAshLr08DwNum = 1; m_dAshLr08Dw1 = 1.50; m_dAshLr08Dw2 = 1.50; break; 
	case 1: m_nAshLr08DwNum = 1; m_dAshLr08Dw1 = 0.65; m_dAshLr08Dw2 = 0.65; break; 
	case 2: m_nAshLr08DwNum = 2; m_dAshLr08Dw1 = 1.50; m_dAshLr08Dw2 = 0.65; break;
	}  
	switch(m_nLrfd08EHTypeRdo)  // horizontal earth pressure
	{
	case 0:   // active
		if     (m_nLrfd08EH1Rdo == 0) {m_nAshLr08EhNum = 1; m_dAshLr08Eh1 = 1.50; m_dAshLr08Eh2 = 1.50;} 
		else if(m_nLrfd08EH1Rdo == 1) {m_nAshLr08EhNum = 1; m_dAshLr08Eh1 = 0.90; m_dAshLr08Eh2 = 0.90;} 
		else if(m_nLrfd08EH1Rdo == 2) {m_nAshLr08EhNum = 2; m_dAshLr08Eh1 = 1.50; m_dAshLr08Eh2 = 0.90;}      
		break;
	case 1:  // at-rest 
		if(m_nLrfd08EH2Rdo == 0)      {m_nAshLr08EhNum = 1; m_dAshLr08Eh1 = 1.35; m_dAshLr08Eh2 = 1.35;} 
		else if(m_nLrfd08EH2Rdo == 1) {m_nAshLr08EhNum = 1; m_dAshLr08Eh1 = 0.90; m_dAshLr08Eh2 = 0.90;} 
		else if(m_nLrfd08EH2Rdo == 2) {m_nAshLr08EhNum = 2; m_dAshLr08Eh1 = 1.35; m_dAshLr08Eh2 = 0.90;}            
		break;
	case 2:  // AEP for anchored walls
		//if(m_nLrfd08EH3Rdo == 0)      {m_nAshLr08EhNum = 1; m_dAshLr08Eh1 = 1.35; m_dAshLr08Eh2 = 1.35;} 
		m_nAshLr08EhNum = 1; m_dAshLr08Eh1 = 1.35; m_dAshLr08Eh2 = 1.35;
		break;
	}
	m_nAshLr08ElNum = 1; m_dAshLr08El1 = 1.00; m_dAshLr08El2 = 1.00;  // locked-in erection stresses    
	switch(m_nLrfd08EVTypeRdo)
	{
	case 0: // Overall Stability
		m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.00; m_dAshLr08Ev2 = 1.00;      
		break;
	case 1: // Retaining Walls and Abutments
		if     (m_nLrfd08EV2Rdo == 0) {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.35; m_dAshLr08Ev2 = 1.35;}
		else if(m_nLrfd08EV2Rdo == 1) {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.00; m_dAshLr08Ev2 = 1.00;} 
		else if(m_nLrfd08EV2Rdo == 2) {m_nAshLr08EvNum = 2; m_dAshLr08Ev1 = 1.35; m_dAshLr08Ev2 = 1.00;}      
		break;
	case 2:  // Rigid Buried Structure
		if(m_nLrfd08EV3Rdo == 0)      {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.30; m_dAshLr08Ev2 = 1.30;} 
		else if(m_nLrfd08EV3Rdo == 1) {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 0.90; m_dAshLr08Ev2 = 0.90;} 
		else if(m_nLrfd08EV3Rdo == 2) {m_nAshLr08EvNum = 2; m_dAshLr08Ev1 = 1.30; m_dAshLr08Ev2 = 0.90;}      
		break;
	case 3:  // Rigid Frames
		if(m_nLrfd08EV4Rdo == 0)      {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.35; m_dAshLr08Ev2 = 1.35;} 
		else if(m_nLrfd08EV4Rdo == 1) {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 0.90; m_dAshLr08Ev2 = 0.90;} 
		else if(m_nLrfd08EV4Rdo == 2) {m_nAshLr08EvNum = 2; m_dAshLr08Ev1 = 1.35; m_dAshLr08Ev2 = 0.90;}      
		break;
	case 4:  // Flexible buried structures other than metal box culverts
		if(m_nLrfd08EV5Rdo == 0)      {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.95; m_dAshLr08Ev2 = 1.95;} 
		else if(m_nLrfd08EV5Rdo == 1) {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 0.90; m_dAshLr08Ev2 = 0.90;} 
		else if(m_nLrfd08EV5Rdo == 2) {m_nAshLr08EvNum = 2; m_dAshLr08Ev1 = 1.95; m_dAshLr08Ev2 = 0.90;}      
		break;
	case 5:  // flexible metal box culverts
		if(m_nLrfd08EV6Rdo == 0)      {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 1.50; m_dAshLr08Ev2 = 1.50;} 
		else if(m_nLrfd08EV6Rdo == 1) {m_nAshLr08EvNum = 1; m_dAshLr08Ev1 = 0.90; m_dAshLr08Ev2 = 0.90;} 
		else if(m_nLrfd08EV6Rdo == 2) {m_nAshLr08EvNum = 2; m_dAshLr08Ev1 = 1.50; m_dAshLr08Ev2 = 0.90;}      
		break;
	}
	switch(m_nLrfd08ESRdo) // earth surcharge
	{
	case 0: m_nAshLr08EsNum = 1; m_dAshLr08Es1 = 1.50; m_dAshLr08Es2 = 1.50; break; 
	case 1: m_nAshLr08EsNum = 1; m_dAshLr08Es1 = 0.75; m_dAshLr08Es2 = 0.75; break; 
	case 2: m_nAshLr08EsNum = 2; m_dAshLr08Es1 = 1.50; m_dAshLr08Es2 = 0.75; break;
	}  

	int nPSCRSHType = m_nPSCRSHTypeCmb.GetCurSel();

	if(nPSCRSHType == 0) // A : Superstructures - Segmental , Concrete Substructures supporting Segmental, Superstructures 
	{
		m_nAshLr08PsNum = 1; m_dAshLr08Ps1 = 1.0; m_dAshLr08Ps2 = 1.0;
		if(m_nLrfd08DCRdo == 0) 
		{
			m_nAshLr08CrNum = 1; m_dAshLr08Cr1 = 1.25; m_dAshLr08Cr2 = 1.25;
			m_nAshLr08ShNum = 1; m_dAshLr08Sh1 = 1.25; m_dAshLr08Sh2 = 1.25;
		}
		else if(m_nLrfd08DCRdo == 1) 
		{
			m_nAshLr08CrNum = 1; m_dAshLr08Cr1 = 0.90; m_dAshLr08Cr2 = 0.90;
			m_nAshLr08ShNum = 1; m_dAshLr08Sh1 = 0.90; m_dAshLr08Sh2 = 0.90;
		}
		else if(m_nLrfd08DCRdo == 2) 
		{
			m_nAshLr08CrNum = 2; m_dAshLr08Cr1 = 1.25; m_dAshLr08Cr2 = 0.90;
			m_nAshLr08ShNum = 2; m_dAshLr08Sh1 = 1.25; m_dAshLr08Sh2 = 0.90;
		}
	}
	else if(nPSCRSHType == 1) // B : Concrete Superstructures - non-segmental
	{
		m_nAshLr08PsNum = 1; m_dAshLr08Ps1 = 1.0; m_dAshLr08Ps2 = 1.0;
		m_nAshLr08CrNum = 1; m_dAshLr08Cr1 = 1.0; m_dAshLr08Cr2 = 1.0;
		m_nAshLr08ShNum = 1; m_dAshLr08Sh1 = 1.0; m_dAshLr08Sh2 = 1.0;
	}
	else if(nPSCRSHType == 2) // C : SubStructures supporting non-segmental Superstructures (using Ig)
	{
		m_nAshLr08PsNum = 1; m_dAshLr08Ps1 = 0.5; m_dAshLr08Ps2 = 0.5;
		m_nAshLr08CrNum = 1; m_dAshLr08Cr1 = 0.5; m_dAshLr08Cr2 = 0.5;
		m_nAshLr08ShNum = 1; m_dAshLr08Sh1 = 0.5; m_dAshLr08Sh2 = 0.5;
	}
	else if(nPSCRSHType == 3) // D : SubStructures supporting non-segmental Superstructures (using Ieffective)
	{
		m_nAshLr08PsNum = 1; m_dAshLr08Ps1 = 1.0; m_dAshLr08Ps2 = 1.0;
		m_nAshLr08CrNum = 1; m_dAshLr08Cr1 = 1.0; m_dAshLr08Cr2 = 1.0;
		m_nAshLr08ShNum = 1; m_dAshLr08Sh1 = 1.0; m_dAshLr08Sh2 = 1.0;
	}
	else if(nPSCRSHType == 4) // E : Steel Substructures
	{
		m_nAshLr08PsNum = 1; m_dAshLr08Ps1 = 1.0; m_dAshLr08Ps2 = 1.0;
		m_nAshLr08CrNum = 1; m_dAshLr08Cr1 = 1.0; m_dAshLr08Cr2 = 1.0;
		m_nAshLr08ShNum = 1; m_dAshLr08Sh1 = 1.0; m_dAshLr08Sh2 = 1.0;
	}
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnCmdAASHTOLRFD08DDTypeRdo()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_MAX_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_MIN_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_BOTH_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_MAX_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_MIN_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_BOTH_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_MAX_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_MIN_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_BOTH_RDO)->EnableWindow(m_nLrfd08DDTypeRdo==2);
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnCmdAASHTOLRFD08EHTypeRdo()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_MAX_RDO)->EnableWindow(m_nLrfd08EHTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_MIN_RDO)->EnableWindow(m_nLrfd08EHTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_BOTH_RDO)->EnableWindow(m_nLrfd08EHTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_MAX_RDO)->EnableWindow(m_nLrfd08EHTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_MIN_RDO)->EnableWindow(m_nLrfd08EHTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_BOTH_RDO)->EnableWindow(m_nLrfd08EHTypeRdo==1);
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnCmdAASHTOLRFD08EVTypeRdo() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_MAX_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_MIN_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_BOTH_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_MAX_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_MIN_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_BOTH_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_MAX_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_MIN_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_BOTH_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_MAX_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_MIN_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_BOTH_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_MAX_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_MIN_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_BOTH_RDO)->EnableWindow(m_nLrfd08EVTypeRdo==5);		
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnCmdAASHTOLRFD08EnableDisable()
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
		/*
		int nMovingType = CProduct::GetMovingType();
		switch(nMovingType)
		{
		case D_PRODUCT_MOVING_ORG: m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);   break;
		case D_PRODUCT_MOVING_JP:  m_pDoc->m_pAttrCtrl->GetMvldjpKeyList(arMLoad); break;
		case D_PRODUCT_MOVING_CH:  m_pDoc->m_pAttrCtrl->GetMvldchKeyList(arMLoad); break;
		case D_PRODUCT_MOVING_US:  
		if(CProduct::IsIndia()) {m_pDoc->m_pAttrCtrl->GetMvldidKeyList(arMLoad); break;}
		else                    {m_pDoc->m_pAttrCtrl->GetMvldKeyList(arMLoad);   break;}
		}
		*/
		if(arMLoad.GetSize() > 0)  bLExist = TRUE;  
		if(arSmLoad.GetSize() > 0) bStlExist = TRUE;
	}
	////////////////////////////////////////////////////////////////// Add by KJH20050104, requester:강희정
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
// 			else if (SgldK == D_SGLD_ER1_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER2_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER3_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER4_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER5_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER6_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER7_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER8_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER9_CVL) { bCEL = TRUE; }
// 			else if (SgldK == D_SGLD_ER10_CVL) { bCEL = TRUE; }
			else if(SgldK==D_SGLD_CP_CVL)    { bTExist  = TRUE; }
			else if(SgldK==D_SGLD_CS_CVL)    { bTExist  = TRUE; }
			else if(SgldK==D_SGLD_SP_CVL)    { bTExist  = TRUE; }
			else if(SgldK==D_SGLD_SS_CVL)    { bTExist  = TRUE; }
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

	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DC_MAX_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DC_MIN_RDO)->EnableWindow(bDcExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DC_BOTH_RDO)->EnableWindow(bDcExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_MAX_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_MIN_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD1_BOTH_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_MAX_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_MIN_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD2_BOTH_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_RDO)->EnableWindow(bDdExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_MAX_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_MIN_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DD3_BOTH_RDO)->EnableWindow(bDdExist && m_nLrfd08DDTypeRdo==2);

	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DW_MAX_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DW_MIN_RDO)->EnableWindow(bDwExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DW_BOTH_RDO)->EnableWindow(bDwExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_MAX_RDO)->EnableWindow(bEhExist && m_nLrfd08EHTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_MIN_RDO)->EnableWindow(bEhExist && m_nLrfd08EHTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH1_BOTH_RDO)->EnableWindow(bEhExist && m_nLrfd08EHTypeRdo==0);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_RDO)->EnableWindow(bEhExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_MAX_RDO)->EnableWindow(bEhExist && m_nLrfd08EHTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_MIN_RDO)->EnableWindow(bEhExist && m_nLrfd08EHTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH2_BOTH_RDO)->EnableWindow(bEhExist && m_nLrfd08EHTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EH3_RDO)->EnableWindow(bEhExist);

	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV1_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV2_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==1);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV3_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==2);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV4_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==3);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV5_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==4);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_RDO)->EnableWindow(bEvExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_MAX_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_MIN_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==5);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EV6_BOTH_RDO)->EnableWindow(bEvExist && m_nLrfd08EVTypeRdo==5);

	GetDlgItem(IDC_CMD_AASHTO_LRFD08_ES_MAX_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_ES_MIN_RDO)->EnableWindow(bEsExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_ES_BOTH_RDO)->EnableWindow(bEsExist);

	if(!bPsExist && !bCrExist && !bShExist)
	{
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_CR_SH_CMB)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_CR_SH_BTN)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_EDIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_CMD_AASHTO_LRFD08_CR_SH_TEXT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_CR_SH_EDIT)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_CR_SH_CMB)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_CR_SH_BTN)->EnableWindow(TRUE);

		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_TEXT)->EnableWindow(bPsExist);
		GetDlgItem(IDC_CMD_AASHTO_LRFD08_PS_EDIT)->EnableWindow(bPsExist);

		if(!bCrExist && !bShExist)
		{
			GetDlgItem(IDC_CMD_AASHTO_LRFD08_CR_SH_TEXT)->EnableWindow(FALSE);
			GetDlgItem(IDC_CMD_AASHTO_LRFD08_CR_SH_EDIT)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_CMD_AASHTO_LRFD08_CR_SH_TEXT)->EnableWindow(TRUE);
			GetDlgItem(IDC_CMD_AASHTO_LRFD08_CR_SH_EDIT)->EnableWindow(TRUE);
		}
	}

	m_bLrfd08CulvertChk.EnableWindow(bLExist);
	m_dLrfd08SettlementEdt.EnableWindow(bStlExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_DEFORMATION_RDO)->EnableWindow(bTExist);
	GetDlgItem(IDC_CMD_AASHTO_LRFD08_EFFECT_RDO)->EnableWindow(bTExist);
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnSelchangeCmdAASHTOLRFD08PSCRSHCmb()
{
    UpdateData(TRUE);

    int nIndex = m_nPSCRSHTypeCmb.GetCurSel();

    if(nIndex == 0) // A : Superstructures - Segmental , Concrete Substructures supporting Segmental, Superstructures 
    {
        m_dLrfd08PSEdt.SetEditUnit(1.0);
        m_dLrfd08CRSHEdt.SetEditUnit(0.0);
    }
    else if(nIndex == 1) // B : Concrete Superstructures - non-segmental
    {
        m_dLrfd08PSEdt.SetEditUnit(1.0);
        m_dLrfd08CRSHEdt.SetEditUnit(1.0);
    } 
    else if(nIndex == 2) // C : SubStructures supporting non-segmental Superstructures (using Ig)
    {
        m_dLrfd08PSEdt.SetEditUnit(0.5);
        m_dLrfd08CRSHEdt.SetEditUnit(0.5);
    }
    else if(nIndex == 3) // D : SubStructures supporting non-segmental Superstructures (using Ieffective)
    {
        m_dLrfd08PSEdt.SetEditUnit(1.0);
        m_dLrfd08CRSHEdt.SetEditUnit(1.0);
    }
    else // E : Steel Substructures
    {
        m_dLrfd08PSEdt.SetEditUnit(1.0);
        m_dLrfd08CRSHEdt.SetEditUnit(1.0);
    }
}

void CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnCmdAASHTOLRFD08PSCRSHBtn()
{
    CCmdAutoLoadCombCvlHelpDlg dlg;
    dlg.DoModal();
}

LRESULT CCmdAutoLoadCombCvlAASHTOLRFD08Dlg::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
    m_pParent->AlignControl();

    return 0L;
}