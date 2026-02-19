// CPGFatigueParamTabIDlg_AASHTO19.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "CPGFatigueParamTabIDlg_AASHTO19.h"
#include "CPGFatigueParamDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabJDlg dialog


CCPGFatigueParamTabIDlg_AASHTO19::CCPGFatigueParamTabIDlg_AASHTO19(CWnd* pParent /*=NULL*/, int nIJ/*=0*/)
	: CChildDialog(CCPGFatigueParamTabIDlg_AASHTO19::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPGFatigueParamTabIDlg_AASHTO19)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nIJ = nIJ;
	m_nWarpType = 0;

	ASSERT(pParent);
	m_pParent = (CCPGFatigueParamDlg*)pParent;

	m_Data.Initialize();
	m_pDoc = CDBDoc::GetDocPoint();

// 	m_arWarpingCtrlID.RemoveAll();
// 	m_arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO);
// 	m_arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO);
// 	m_arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_TOP_EDT);
// 	m_arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_TOP_UNT);
// 	m_arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_BOT_EDT);
// 	m_arWarpingCtrlID.Add(IDC_DGN_CPG_FATIGUE_BOT_UNT);
}

void CCPGFatigueParamTabIDlg_AASHTO19::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPGFatigueParamTabIDlg_AASHTO19)
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_NL_EDIT,              m_dNL);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_ADTT_P_EDIT,          m_dADTTpr);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_ADTT_EDIT,            m_dADTTsl);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_NUM_EDIT,             m_dNum);

	DDX_Check  (pDX, IDC_DGN_CPG_FATIGUE_POSITION_TOP_TOP_CHK, m_bTopF_Top);
	DDX_Check  (pDX, IDC_DGN_CPG_FATIGUE_POSITION_TOP_BOT_CHK, m_bTopF_Bot);
	DDX_Check  (pDX, IDC_DGN_CPG_FATIGUE_POSITION_BOT_TOP_CHK, m_bBotF_Top);
	DDX_Check  (pDX, IDC_DGN_CPG_FATIGUE_POSITION_BOT_BOT_CHK, m_bBotF_Bot);

	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_POSITION_TOP_TOP_CMB, m_cmbTopF_Top);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_POSITION_TOP_BOT_CMB, m_cmbTopF_Bot);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_POSITION_BOT_TOP_CMB, m_cmbBotF_Top);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_POSITION_BOT_BOT_CMB, m_cmbBotF_Bot);

	DDX_Radio  (pDX, IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO,     m_nWarpType);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_TOP_EDT,              m_edtTopFlange);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_TOP_UNT,              m_untTopFlange);

	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_TOP_BOT_EDT,          m_edtTopFlangeBot);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_TOP_BOT_UNT,          m_untTopFlangeBot);

	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_BOT_TOP_EDT,          m_edtBotFlangeTop);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_BOT_TOP_UNT,          m_untBotFlangeTop);

	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_BOT_EDT,              m_edtBotFlange);
	DDX_Control(pDX, IDC_DGN_CPG_FATIGUE_BOT_UNT,              m_untBotFlange);

	DDX_Control(pDX,  IDC_CPFP_FOLDE_GROUP_BOX,                m_chkGroup);
	//}}AFX_DATA_MAP 
}


BEGIN_MESSAGE_MAP(CCPGFatigueParamTabIDlg_AASHTO19, CChildDialog)
	//{{AFX_MSG_MAP(CCPGFatigueParamTabIDlg_AASHTO19)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_POSITION_TOP_TOP_CHK, OnTopF_Top_Chk)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_POSITION_TOP_BOT_CHK, OnTopF_Bot_Chk)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_POSITION_BOT_TOP_CHK, OnBotF_Top_Chk)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_POSITION_BOT_BOT_CHK, OnBotF_Bot_Chk)

	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO, OnWarpingStressTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO, OnWarpingStressTypeRdo)

	ON_REGISTERED_MESSAGE(CFoldGroupBox::WM_FOLDGROUPBOX_STATE_CHANGED, OnFoldButtonClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCPGFatigueParamTabIDlg_AASHTO19 message handlers

BOOL CCPGFatigueParamTabIDlg_AASHTO19::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	InitCombo();
	InitUnit();

	Data2Dlg();
	OnTopF_Top_Chk();
	OnTopF_Bot_Chk();
	OnBotF_Top_Chk();
	OnBotF_Bot_Chk();

	ControlsEnableDisable();

	m_chkGroup.InitControl(this, CCPGFatigueParamTabIDlg_AASHTO19::IDD, IDC_CPFP_FOLDE_GROUP_BOX,TRUE,TRUE);
	ResizeWindow();
	
	m_chkGroup.ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCPGFatigueParamTabIDlg_AASHTO19::DestroyWindow() 
{
	Dlg2Data();
	if(m_nIJ==0)
		m_pParent->m_I_Data = m_Data;
	else
		m_pParent->m_J_Data = m_Data;

	m_chkGroup.SetFoldState(FALSE);

	return CChildDialog::DestroyWindow();
}

void CCPGFatigueParamTabIDlg_AASHTO19::InitCombo()
{
	m_cmbTopF_Top.ResetContent();
	m_cmbTopF_Top.SetItemData(0, m_cmbTopF_Top.AddString(_T("A")));
	m_cmbTopF_Top.SetItemData(1, m_cmbTopF_Top.AddString(_T("B")));
	m_cmbTopF_Top.SetItemData(2, m_cmbTopF_Top.AddString(_T("B'")));
	m_cmbTopF_Top.SetItemData(3, m_cmbTopF_Top.AddString(_T("C")));
	m_cmbTopF_Top.SetItemData(4, m_cmbTopF_Top.AddString(_T("C'")));
	m_cmbTopF_Top.SetItemData(5, m_cmbTopF_Top.AddString(_T("D")));
	m_cmbTopF_Top.SetItemData(6, m_cmbTopF_Top.AddString(_T("E")));
	m_cmbTopF_Top.SetItemData(7, m_cmbTopF_Top.AddString(_T("E'")));
	m_cmbTopF_Top.SetCurSel(0);

	m_cmbTopF_Bot.ResetContent();
	m_cmbTopF_Bot.SetItemData(0, m_cmbTopF_Bot.AddString(_T("A")));
	m_cmbTopF_Bot.SetItemData(1, m_cmbTopF_Bot.AddString(_T("B")));
	m_cmbTopF_Bot.SetItemData(2, m_cmbTopF_Bot.AddString(_T("B'")));
	m_cmbTopF_Bot.SetItemData(3, m_cmbTopF_Bot.AddString(_T("C")));
	m_cmbTopF_Bot.SetItemData(4, m_cmbTopF_Bot.AddString(_T("C'")));
	m_cmbTopF_Bot.SetItemData(5, m_cmbTopF_Bot.AddString(_T("D")));
	m_cmbTopF_Bot.SetItemData(6, m_cmbTopF_Bot.AddString(_T("E")));
	m_cmbTopF_Bot.SetItemData(7, m_cmbTopF_Bot.AddString(_T("E'")));
	m_cmbTopF_Bot.SetCurSel(0);

	m_cmbBotF_Top.ResetContent();
	m_cmbBotF_Top.SetItemData(0, m_cmbBotF_Top.AddString(_T("A")));
	m_cmbBotF_Top.SetItemData(1, m_cmbBotF_Top.AddString(_T("B")));
	m_cmbBotF_Top.SetItemData(2, m_cmbBotF_Top.AddString(_T("B'")));
	m_cmbBotF_Top.SetItemData(3, m_cmbBotF_Top.AddString(_T("C")));
	m_cmbBotF_Top.SetItemData(4, m_cmbBotF_Top.AddString(_T("C'")));
	m_cmbBotF_Top.SetItemData(5, m_cmbBotF_Top.AddString(_T("D")));
	m_cmbBotF_Top.SetItemData(6, m_cmbBotF_Top.AddString(_T("E")));
	m_cmbBotF_Top.SetItemData(7, m_cmbBotF_Top.AddString(_T("E'")));
	m_cmbBotF_Top.SetCurSel(0);

	m_cmbBotF_Bot.ResetContent();
	m_cmbBotF_Bot.SetItemData(0, m_cmbBotF_Bot.AddString(_T("A")));
	m_cmbBotF_Bot.SetItemData(1, m_cmbBotF_Bot.AddString(_T("B")));
	m_cmbBotF_Bot.SetItemData(2, m_cmbBotF_Bot.AddString(_T("B'")));
	m_cmbBotF_Bot.SetItemData(3, m_cmbBotF_Bot.AddString(_T("C")));
	m_cmbBotF_Bot.SetItemData(4, m_cmbBotF_Bot.AddString(_T("C'")));
	m_cmbBotF_Bot.SetItemData(5, m_cmbBotF_Bot.AddString(_T("D")));
	m_cmbBotF_Bot.SetItemData(6, m_cmbBotF_Bot.AddString(_T("E")));
	m_cmbBotF_Bot.SetItemData(7, m_cmbBotF_Bot.AddString(_T("E'")));
	m_cmbBotF_Bot.SetCurSel(0);

}

void CCPGFatigueParamTabIDlg_AASHTO19::InitUnit()
{
	m_edtTopFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untTopFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtTopFlangeBot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untTopFlangeBot.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtBotFlangeTop.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untBotFlangeTop.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_edtBotFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untBotFlange.SetUnitType(D_UNITSYS_BASE_STRESS);
	
	m_dNL  .SetUnitType(D_UNITSYS_NONE);
	m_dADTTpr.SetUnitType(D_UNITSYS_NONE);
	m_dADTTsl.SetUnitType(D_UNITSYS_NONE);
	m_dNum .SetUnitType(D_UNITSYS_NONE);
}

void CCPGFatigueParamTabIDlg_AASHTO19::OnTopF_Top_Chk() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_TOP_TOP_CMB)->EnableWindow(m_bTopF_Top);

	UpdateData(FALSE);
}
void CCPGFatigueParamTabIDlg_AASHTO19::OnTopF_Bot_Chk() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_TOP_BOT_CMB)->EnableWindow(m_bTopF_Bot);

	UpdateData(FALSE);
}
void CCPGFatigueParamTabIDlg_AASHTO19::OnBotF_Top_Chk() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_BOT_TOP_CMB)->EnableWindow(m_bBotF_Top);

	UpdateData(FALSE);
}
void CCPGFatigueParamTabIDlg_AASHTO19::OnBotF_Bot_Chk() 
{
	UpdateData(TRUE);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_BOT_BOT_CMB)->EnableWindow(m_bBotF_Bot);

	UpdateData(FALSE);
}

void CCPGFatigueParamTabIDlg_AASHTO19::OnWarpingStressTypeRdo() 
{
	ControlsEnableDisable();
}

void CCPGFatigueParamTabIDlg_AASHTO19::Data2Dlg()
{
	m_dNL  .SetEditUnit(m_Data.dNL);
	m_dADTTpr.SetEditUnit(m_Data.dADTT_Pr);
	m_dADTTsl.SetEditUnit(m_Data.dADTT);
	m_dNum .SetEditUnit(m_Data.dNum);

	m_bTopF_Top = m_Data.bTopF_Top;
	m_bTopF_Bot = m_Data.bTopF_Bot;
	m_bBotF_Top = m_Data.bBotF_Top;
	m_bBotF_Bot = m_Data.bBotF_Bot;

	m_cmbTopF_Top.SetCurSel(m_Data.nDetailCategory);
	m_cmbTopF_Bot.SetCurSel(m_Data.nDetailCategory2);
	m_cmbBotF_Top.SetCurSel(m_Data.nDetailCategory3);
	m_cmbBotF_Bot.SetCurSel(m_Data.nDetailCategory4);

	m_nWarpType = m_Data.nWarpType;

	m_edtTopFlange   .SetEditUnit(m_Data.dTopFlange);
	m_edtTopFlangeBot.SetEditUnit(m_Data.dTopFlangeBot);
	m_edtBotFlangeTop.SetEditUnit(m_Data.dBotFlangeTop);	
	m_edtBotFlange   .SetEditUnit(m_Data.dBotFlange);	
	
	UpdateData(FALSE);
}

BOOL CCPGFatigueParamTabIDlg_AASHTO19::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.dNL   = m_dNL.GetEditValue();
	m_Data.dADTT_Pr = m_dADTTpr.GetEditValue();
	m_Data.dADTT    = m_dADTTsl.GetEditValue();
	m_Data.dNum  = m_dNum.GetEditValue();	

	m_Data.bTopF_Top = m_bTopF_Top;
	m_Data.bTopF_Bot = m_bTopF_Bot;
	m_Data.bBotF_Top = m_bBotF_Top;
	m_Data.bBotF_Bot = m_bBotF_Bot;

	m_Data.nDetailCategory  = m_cmbTopF_Top.GetItemData(m_cmbTopF_Top.GetCurSel());
	m_Data.nDetailCategory2 = m_cmbTopF_Bot.GetItemData(m_cmbTopF_Bot.GetCurSel());
	m_Data.nDetailCategory3 = m_cmbBotF_Top.GetItemData(m_cmbBotF_Top.GetCurSel());
	m_Data.nDetailCategory4 = m_cmbBotF_Bot.GetItemData(m_cmbBotF_Bot.GetCurSel());

	m_Data.nWarpType = m_nWarpType;

	m_Data.dTopFlange    = m_edtTopFlange.GetEditValue();
	m_Data.dTopFlangeBot = m_edtTopFlangeBot.GetEditValue();
	m_Data.dBotFlangeTop = m_edtBotFlangeTop.GetEditValue();
	m_Data.dBotFlange    = m_edtBotFlange.GetEditValue();

	return TRUE;
}

void CCPGFatigueParamTabIDlg_AASHTO19::GetCpfpData()
{
	Dlg2Data();

	if(m_nIJ==0)
		m_pParent->m_I_Data = m_Data;
	else
		m_pParent->m_J_Data = m_Data;
}

void CCPGFatigueParamTabIDlg_AASHTO19::SetCpfpData()
{
	Data2Dlg();
}

void CCPGFatigueParamTabIDlg_AASHTO19::ControlsEnableDisable()
{
	UpdateData(TRUE);

	BOOL bEnable = FALSE;

	if(m_nIJ == 0)
	{
		if(m_pParent->m_nOption==0) {bEnable = TRUE; }
		else						{bEnable = FALSE;} 
	}
	else
	{
		if(m_pParent->m_nOption==0 && m_pParent->m_bBothIJ) {bEnable = TRUE; }
		else												{bEnable = FALSE;} 
	}
	
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NL_STC     )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NL_EDIT    )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_P_STC )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_P_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_STC   )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_ADTT_EDIT  )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_STC    )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_NUM_EDIT   )->EnableWindow(bEnable);

	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_CATEGORY_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_TOP_TOP_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_TOP_TOP_CMB)->EnableWindow(bEnable && m_bTopF_Top);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_TOP_BOT_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_TOP_BOT_CMB)->EnableWindow(bEnable && m_bTopF_Bot);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_BOT_TOP_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_BOT_TOP_CMB)->EnableWindow(bEnable && m_bBotF_Top);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_BOT_BOT_CHK)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_POSITION_BOT_BOT_CMB)->EnableWindow(bEnable && m_bBotF_Bot);
    	
	GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_GRP     )->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_AUTO_RDO)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_WARPING_USER_RDO)->EnableWindow(bEnable);	
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_STC    )->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_EDT    )->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_UNT    )->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_BOT_STC)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_BOT_EDT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_TOP_BOT_UNT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_TOP_STC)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_TOP_EDT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_TOP_UNT)->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_STC    )->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_EDT    )->EnableWindow(bEnable && m_nWarpType==1);
	GetDlgItem(IDC_DGN_CPG_FATIGUE_BOT_UNT    )->EnableWindow(bEnable && m_nWarpType==1);

	this->RedrawWindow();
}

CRect CCPGFatigueParamTabIDlg_AASHTO19::GetResizeHeight()
{	
	CRect r;
	if(m_hWnd != NULL) GetWindowRect(r);
	return r;
}

void CCPGFatigueParamTabIDlg_AASHTO19::ResizeWindow()
{
	m_chkGroup.SetFoldState(FALSE);

//     switch ( m_pParent->m_nDgnCode )
//     {
//     case AASHTO_LRFD07_CSG:
//     case AASHTO_LRFD12_CSG:
//     case AASHTO_LRFD16_CSG:
//     case AASHTO_LRFD17_CSG:
//     case STL_AASHTO_LRFD12:
//     case STL_AASHTO_LRFD19:
//     case KSSC_2014_CSG:
//         m_chkGroup.SetFoldState(FALSE);
//         break;
//     default:
//         m_chkGroup.SetFoldState(TRUE);
//         break;
//     }
	
	m_pParent->ResizeWindow();
}

LRESULT CCPGFatigueParamTabIDlg_AASHTO19::OnFoldButtonClicked(WPARAM wParam, LPARAM lParam)
{
	m_pParent->ResizeWindow();
	return 0L;
}